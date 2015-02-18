/*

$Id$

Copyright (c) 2005-2006 Ross Smith II (http://smithii.com). All rights reserved.

This program is free software; you can redistribute it and/or modify it
under the terms of version 2 of the GNU General Public License
as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

*/

// #define TINY_TEST 1

/*

@todo rename regtool

@todo don't write HKEY_USERS tree for HKEY_CURRENT_USER branch

@todo Profile the code to see where the biggest speed improvements can be made

@todo Don't use .readall() to read in the entire file

@todo For speed, write output to memory, then write to disk when done

@todo add:

-r | --redo
-u | --undo

@todo Rename options:
	mv -x => -e | --export
	mv -e => -E | --execute
	mv -r => -R | --regedit
	mv -u => -U | --unsorted

@todo Deal with write errors!

@todo Add -s | --sort file.reg support

@todo Add -e | --execute cmd args support

@todo Add -x | --exclude key
NOTES:

  http://winhlp.com/WxRegistryRemoving.htm
A minus sign inserted directly after the opening bracket in an otherwise normal .reg file removes that key. It works only for keys, not for values, but you can remove, then recreate a key.

Example:

[-HKEY_LOCAL_MACHINE\Software\Adobe]

This entry in a .reg file will remove that entire key and all of its subkeys and values.

You can remove a value by entering a minus sign (without quotes) as the new value.

Example:

[HKEY_LOCAL_MACHINE\Software\Test]
"Test"=-

This removes the value named "Test", but leaves the key "Test" in place.

*/

#pragma warning(disable:4786)
#pragma comment(lib, "advapi32")

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#include <assert.h>
#include <tchar.h>
#include <tstl.h>
#include <windows.h>
#include <conio.h>

#include <algorithm>
#include <map>
#include <sstream>
#include <string>

#include "Stackwalker.h"
#include "Optimize.h"
#include "SystemMessage.h"
#include "debug.h"

#include "tgetopt.h"

#include "RegFileReader.h"
#include "RegFileWriter.h"

#include "version.h"

#define APPNAME			_T(VER_INTERNAL_NAME)
#define APPVERSION		_T(VER_STRING2)
#define APPCOPYRIGHT	_T(VER_LEGAL_COPYRIGHT)

using namespace std;

// http://msdn.microsoft.com/library/default.asp?url=/library/en-us/sysinfo/base/registry_element_size_limits.asp

//HKEY_String_Map hkey_name;

struct tchar_less_nocase {
	bool operator() (const TCHAR *p, const TCHAR *q) {
		return _tcsicmp(p, q) < 0;
	}
};

typedef map<TCHAR *, HKEY, tchar_less_nocase> TCHAR_HKEY_Map;

static TCHAR_HKEY_Map name_hkey;

static HKEY root_key[] = {
	HKEY_CURRENT_CONFIG,
	HKEY_CURRENT_USER,
	HKEY_CLASSES_ROOT,
	HKEY_LOCAL_MACHINE,
	HKEY_USERS
};

static const int root_keys = sizeof(root_key) / sizeof(root_key[0]);

const TCHAR *dash = _T("-");

static TCHAR *progname(const TCHAR *argv0);

struct _opt {
	bool all;
	TCHAR *execute;
	bool o_export;
	TCHAR *output;
	bool comments;
	bool quirks;
	bool regedit;
	bool sort;
	bool unsorted;
	bool verbose;
	bool wordwrap;
};

typedef struct _opt Opt;

Opt opt = {
	false,	// all
	NULL,	// execute
	false,	// export
	false,	// comments
	false,	// quirks
	false,	// regedit
	false,	// sort
	false,  // unsorted
	false,  // verbose
	false	// wordwrap
};

static const TCHAR *short_options = _T("aco:ruvVwx?"); // _T("q")

static const struct toption long_options[] = {
	{_T("all"),				no_argument,		0, 'a'},
	{_T("a"),				no_argument,		0, 'a'},
	{_T("export"),			no_argument,		0, 'x'},
	{_T("x"),				no_argument,		0, 'x'},
//	{_T("execute"),			required_argument,	0, 'e'},
//	{_T("e"),				required_argument,	0, 'e'},
	{_T("output"),			required_argument,	0, 'o'},
	{_T("o"),				required_argument,	0, 'o'},
	{_T("comments"),		no_argument,		0, 'c'},
	{_T("c"),				no_argument,		0, 'c'},
//	{_T("quirks"),			no_argument,		0, 'q'},
	{_T("regedit"),			no_argument,		0, 'r'},
	{_T("r"),				no_argument,		0, 'r'},
//	{_T("sort"),			no_argument,		0, 's'},
//	{_T("s"),				no_argument,		0, 's'},
	{_T("wordwrap"),		no_argument,		0, 'w'},
	{_T("w"),				no_argument,		0, 'w'},
	{_T("unsorted"),		no_argument,		0, 'u'},
	{_T("u"),				no_argument,		0, 'u'},
	{_T("verbose"),			no_argument,		0, 'v'},
	{_T("v"),				no_argument,		0, 'v'},
	{_T("version"),			no_argument,		0, 'V'},
	{_T("V"),				no_argument,		0, 'V'},
	{_T("help"),			no_argument,		0, '?'},
	{_T("h"),				no_argument,		0, '?'},
	{_T("?"),				no_argument,		0, '?'},
	{NULL,					0,					0, 0}
};

static void usage(int exitval) {
	TCHAR *me = progname(NULL);

	_tprintf(
_T("Usage: %s [options] [key | file.reg] [key | file.reg]\n")
_T("Options:\n"),
		me
	);
/*  12345678901234567890123456789012345678901234567890123456789012345678901234567890*/
	_tprintf(
_T("-a | --all                  Compare/export all keys (HKCC HKCU HKCR HKLM HKU)\n")
_T("-x | --export key(s) ex.reg Export key(s) to ex.reg\n")
//_T("-e | --execute 'cmd' ex.reg Execute cmd & save differences to ex.reg (TODO)\n")
_T("-o | --output output.reg    Set output filename to 'output.reg'\n")
_T("-c | --comments             Include original values as comments in diff\n")
// _T("-q | --quirks               Implement REGEDIT quirks\n")
_T("-r | --regedit              Export in RegEdit format (implies -u -w)\n")
//_T("-s | --sort in.reg out.reg  Sort in.reg to out.reg (TODO)\n")
_T("-u | --unsorted             Do not sort the keys or values, as RegEdit\n")
_T("-w | --wordwrap             Wordwrap at 78 chars, as RegEdit\n")
_T("-v | --verbose              Show warnings that are normally hidden\n")
_T("-V | --version              Show version & copyright information & quit\n")
_T("-? | --help                 Show this help message and quit\n")
_T("Examples:\n")
_T(" %s HKCU hkcu.reg              ; compare HKCU key with hkcu.reg\n")
_T(" %s HKCU\\1 HKCU\\2              ; compare HKCU\\1 key with HKCU\\2 key\n")
_T(" %s -o diff.reg HKCU hkcu.reg  ; compare HKCU key with hkcu.reg\n")
_T(" %s -x HKCU hkcu.reg           ; export HKCU key to hkcu.reg\n")
_T(" %s -xr HKCU\\Software ex.reg   ; export HKCU to ex.reg in REGEDIT format\n")
_T(" %s -ax allkeys.reg            ; export all keys to allkeys.reg\n")
//_T(" %s -s input.reg output.reg    ; sort input.reg to output.reg\n")
//_T(" %s -e 'setup.exe /S' ex.reg   ; run setup & save differences in ex.reg\n")
	,me, me, me, me, me, me //, me, me
);
	exit(exitval);
}

/* per http://www.scit.wlv.ac.uk/cgi-bin/mansec?3C+basename */
static TCHAR* basename(const TCHAR* s) {
	TCHAR* rv;

	if (!s || !*s)
		return _T(".");

	unsigned int ui = _tcslen(s);

	rv = const_cast<TCHAR *>(s);
	rv += (_tcslen(s) - 1) * sizeof(TCHAR);

	do {
		if (*rv == '\\' || *rv == '/')
			return rv + 1;
		--rv;
	} while (rv >= s);

	return const_cast<TCHAR *>(s);
}

static TCHAR *progname(const TCHAR *argv0) {
	static TCHAR *progname = NULL;
	if (progname == NULL && argv0 != NULL) {
		progname = basename(argv0);

		int len = _tcslen(progname);
		TCHAR *p = progname + (len - 4) * sizeof(TCHAR);
		if (len > 4 && _tcsicmp(p, _T(".exe")) == 0)
			progname[len - 4] = '\0';
	}

	return progname;
}

static DWORD export_key(RegFileWriter *f, TCHAR *start_key) {
	DWORD rv = 0;

	TCHAR *subkey = NULL;

	TCHAR *p = _tcschr(start_key, '\\');
	if (p != NULL) {
		*p = '\0';
		++p;
		subkey = p;
	}

	HKEY hkey = (HKEY) name_hkey[start_key];

#ifdef TINY_TEST
	hkey	= HKEY_CURRENT_USER;
	subkey	= _T("regdelta");
#endif
	if (hkey == 0) {
		_ftprintf(stderr, _T("Invalid key: '%s'. Must be one of:"), start_key);

		for (TCHAR_HKEY_Map::const_iterator it = name_hkey.begin(); it != name_hkey.end(); ++it) {
			TCHAR *abbr = (TCHAR *) it->first;
			if (_tcslen(abbr) < 5) {
				_ftprintf(stderr, _T(" %s"), abbr);
			}
		}
		_ftprintf(stderr, EOL);
		return 1;
	}

	rv = f->writeValues(hkey, subkey);
	return rv;
}

int export2(const int argc, TCHAR *const *argv, int toptind, const TCHAR *output_file) {
	DWORD rv = 0;

	if (!opt.all && toptind >= argc)
		usage(1);

	RegFileWriter f;

	f.setQuirks(opt.quirks);
	f.setUnsorted(opt.unsorted);
	f.setVerbose(opt.verbose);
	f.setWordwrap(opt.wordwrap);

	f.create(output_file);

	f.writeHeader(true);

	TCHAR *start_key = NULL;

	if (opt.all) {
		for (int i = 0; i < root_keys; ++i) {
			rv = f.writeValues(root_key[i], NULL);
			if (rv)
				break;
		}
	} else {
		while (toptind < argc) {
			start_key = argv[toptind++];
			rv = export_key(&f, start_key);
			if (rv)
				break;
		}
	}

	return rv;
}

int do_export(int argc, TCHAR *const *argv, int toptind) {
	DWORD rv = 0;

	TCHAR *output_file;

	if (opt.output) {
		output_file = opt.output;
	} else {
		if (toptind >= argc)
			usage(1);
		output_file = argv[--argc];
	}

	rv = export2(argc, argv, toptind, output_file);
	return rv;
}

static TCHAR* get_temp_name(const TCHAR *prefix) {
    TCHAR lpPathBuffer[MAX_PATH];

    DWORD rv = GetTempPath(
		MAX_PATH,				// length of the buffer
		lpPathBuffer			// buffer for path
	);

    if (rv == 0) {
		rv = GetLastError();
		_ftprintf(stderr, _T("Cannot get temporary path: Error %d: %s"), rv, SystemMessage(rv));
		return NULL;
    }

    static TCHAR szTempName[MAX_PATH];

	if (prefix == NULL)
		prefix = _T("~rd");

    rv = GetTempFileName(
		lpPathBuffer,	// directory for tmp files
		prefix,    		// temp file name prefix
		0,            	// create unique name
		szTempName		// buffer for name
	);

    if (rv == 0) {
		rv = GetLastError();
		_ftprintf(stderr, _T("Cannot get temporary path: Error %d: %s"), rv, SystemMessage(rv));
		return NULL;
    }

	return _tcsdup(szTempName);
}

static DWORD sort(const int argc, TCHAR *const *argv, int toptind) {
	DWORD rv = 0;

	if (toptind >= argc)
		usage(1);

	RegFileReader input;

	input.open(argv[toptind++]);

	if (!opt.output) {
		opt.output = const_cast<TCHAR*>(dash);
	} else {
		if (toptind >= argc)
			usage(1);
		opt.output = argv[toptind++];
	}

	RegFileWriter output;

	output.create(opt.output);

/****************************************************************************/
// @todo Add -s | --sort file.reg support
_ftprintf(stderr, _T("Option not yet implemented"));
exit(1);
/****************************************************************************/
	input.close();
	output.close();

	return rv;
}

static DWORD export_file(const TCHAR *filename, TCHAR *start_key) {
	DWORD rv = 0;

	RegFileWriter f;

	f.create(filename);

	f.writeHeader(true);

	rv = export_key(&f, start_key);

	f.close();

	return rv;
}

static DWORD diff_files(const TCHAR *file1, const TCHAR *file2) {
	DWORD rv = 0;

	RegFileReader r1;
	RegFileReader r2;

	rv = r1.open(file1);

	if (rv)
		return rv;

	rv = r2.open(file2);

	if (rv)
		return rv;

	RegFileWriter w;

	if (!opt.output)
		opt.output = const_cast<TCHAR*>(dash);

	rv = w.create(opt.output);
	if (rv)
		return rv;

	r1.readall();
	r2.readall();

	r1.skipHeader();
	r2.skipHeader();

	w.writeHeader(false);

	while (!r1.eof() || !r2.eof()) {
		if (r1.eof() && !r2.eof()) {
			w.writeSection(r2.getSection());
			if (!r2.isSection()) {
				w.writeLine(r2.getLine());
			}
			r2.readNextLine();
			continue;
		}

		if (!r1.eof() && r2.eof()) {
			if (r1.isSection()) {
				w.writeDelSection(r1.getSection());
				r1.skipDelSection();
				continue;
			}
			w.writeSection(r1.getSection());
			w.writeDelLine(r1.getLine());
			r1.readNextLine();
			continue;
		}

		if (r1.isSection() && r2.isSection()) {
			if (r2.sectionCompare(&r1) > 0) {
				w.writeDelSection(r1.getSection());
				r1.skipDelSection();
				continue;
			}

			if (r1.sectionCompare(&r2) > 0) {
				w.writeSection(r2.getSection());

				if (!r2.isSection()) {
					w.writeLine(r2.getLine());
				}
				while (!r2.eof()) {
					r2.readNextLine();
					if (r2.eof())
						break;
					if (r2.isSection())
						break;
					w.writeLine(r2.getLine());
				}
				continue;
			}

			// sections are the same
			if (r1.isSection())
				r1.readNextLine();
			if (r2.isSection())
				r2.readNextLine();
			continue;
		}

		if (r2.isSection() || (!r1.isSection() && (r1.keyCompare(&r2) < 0))) {
			w.writeSection(r1.getSection());
			w.writeDelLine(r1.getLine());

			r1.readNextLine();
			continue;
		}

		if (r1.isSection() || (!r2.isSection() && (r1.keyCompare(&r2) > 0))) {
			w.writeSection(r2.getSection());
			w.writeLine(r2.getLine());

			r2.readNextLine();
			continue;
		}

		if (!r1.areValuesEqual(&r2)) {
			w.writeSection(r2.getSection());
			if (opt.comments)
				w.writeComment(r1.getLine());
			w.writeLine(r2.getLine());
		}

		// same key, same or different values
		r1.readNextLine();
		r2.readNextLine();
	}

	r1.close();
	r2.close();
	w.close();

	return rv;
}

static DWORD diff(int argc, TCHAR *const *argv, int toptind) {
	DWORD rv;
	TCHAR *file1 = NULL;
	TCHAR *file2 = NULL;

	if (toptind >= argc)
		usage(1);

	TCHAR *file1_key = argv[toptind++];

	if (toptind >= argc)
		usage(1);

	TCHAR *file2_key = argv[toptind++];
	HKEY file1_hkey = NULL;
	TCHAR *p;

	TCHAR *file1_subkey = NULL;
	if (_tcsnicmp(file1_key, _T("HK"), 2) == 0) {
		p = _tcschr(file1_key, '\\');
		if (p != NULL) {
			*p = '\0';
			++p;
			file1_subkey = p;
			file1_hkey = (HKEY) name_hkey[file1_key];
		}
	}

	TCHAR *file2_subkey = NULL;
	HKEY file2_hkey = NULL;

	if (_tcsnicmp(file1_key, _T("HK"), 2) == 0) {
		p = _tcschr(file2_key, '\\');
		if (p != NULL) {
			*p = '\0';
			++p;
			file2_subkey = p;
			file2_hkey = (HKEY) name_hkey[file2_key];
		}
	}

	if (file1_hkey) {
		file1 = get_temp_name(_T("rd1"));
		opt.quirks = 0;
		opt.regedit = 0;
		opt.unsorted = 0;
		rv = export_file(file1, file1_key);
		if (rv)
			return rv;
	} else {
		file1 = file1_key;
	}

	if (file2_hkey) {
		file2 = get_temp_name(_T("rd2"));
		opt.quirks = 0;
		opt.regedit = 0;
		opt.unsorted = 0;
		rv = export_file(file2, file2_key);
		if (rv)
			return rv;
	} else {
		file2 = file2_key;
	}

	rv = diff_files(file1, file2);
	return rv;
}

static DWORD execute(const int argc, TCHAR *const *argv, int toptind) {
	DWORD rv = 0;

	TCHAR *before_file = get_temp_name(_T("re1"));

	rv = export2(argc, argv, toptind, before_file);
	if (rv)
		return rv;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	BOOL b = CreateProcess(
		NULL,	// LPCTSTR lpApplicationName,
		opt.execute,	// LPTSTR lpCommandLine,
		NULL,	// LPSECURITY_ATTRIBUTES lpProcessAttributes,
		NULL,	// LPSECURITY_ATTRIBUTES lpThreadAttributes,
		true,	// BOOL bInheritHandles,
		0,		// DWORD dwCreationFlags,
		NULL,	// LPVOID lpEnvironment,
		NULL,	// LPCTSTR lpCurrentDirectory,
		&si,	// LPSTARTUPINFO lpStartupInfo,
		&pi		// LPPROCESS_INFORMATION lpProcessInformation
	);
	if (!b) {
		rv = GetLastError();
		_ftprintf(stderr, _T("Failed to execute '%s': Error %d: %s"), opt.execute, rv, SystemMessage(rv));
		return rv;
	}

    // Wait until child process exits.
    DWORD dw = WaitForSingleObject(pi.hProcess, INFINITE);
	if (dw == WAIT_FAILED) {
		_ftprintf(stderr, _T("Failed to wait on '%s': Error %d: %s"), opt.execute, rv, SystemMessage(rv));
		return dw;
	}

	b = GetExitCodeProcess(pi.hProcess, &dw);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

	TCHAR *after_file = get_temp_name(_T("re2"));

	rv = export2(argc, argv, toptind, after_file);
	if (rv)
		return rv;

	rv = diff_files(before_file, after_file);

	return rv;
}

int _tmain(int argc, TCHAR *const *argv) {
	DWORD rv = 0;

#ifdef _DEBUG_ALLOC
    InitAllocCheck();
#endif
	progname(argv[0]);

	name_hkey[_T("HKCR")]			= HKEY_CLASSES_ROOT;
	name_hkey[_T("HKCC")]			= HKEY_CURRENT_CONFIG;
	name_hkey[_T("HKCU")]			= HKEY_CURRENT_USER;
	name_hkey[_T("HKLM")]			= HKEY_LOCAL_MACHINE;
	name_hkey[_T("HKU")]				= HKEY_USERS;

	name_hkey[_T("HKEY_CLASSES_ROOT")]		= HKEY_CLASSES_ROOT;
	name_hkey[_T("HKEY_CURRENT_CONFIG")]		= HKEY_CURRENT_CONFIG;
	name_hkey[_T("HKEY_CURRENT_USER")]		= HKEY_CURRENT_USER;
	name_hkey[_T("HKEY_LOCAL_MACHINE")]		= HKEY_LOCAL_MACHINE;
	name_hkey[_T("HKEY_USERS")]				= HKEY_USERS;

	topterr = 0;
	toptind = 1;

	while (1) {
		int c;
		int option_index = 0;

		if (toptind < argc && argv[toptind] && argv[toptind][0] == '/')
			argv[toptind][0] = '-';

		c = tgetopt_long(argc, argv, short_options, long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
			case 'a':	// all
				opt.all = true;
				opt.o_export = true;
				break;
/*
			case 'e':	// execute
				opt.execute = toptarg;
				break;
*/
			case 'o':	// output
				opt.output = toptarg;
				break;

			case 'c':	// comments
				opt.comments = true;
				break;

/*
			case 'q':	// quirks
				opt.quirks = true;
				break;
*/
			case 'r':	// regedit
				opt.regedit  = true;
				opt.quirks   = true;
				opt.unsorted = true;
				opt.wordwrap = true;
				break;

/*
			case 's':	// sort
				opt.sort = true;
				break;
*/

			case 'u':	// unsorted
				opt.unsorted = true;
				break;

			case 'v':	// verbose
				opt.verbose = true;
				break;

			case 'w':	// wordwrap
				opt.wordwrap = true;
				break;

			case 'x':	// export
				opt.o_export = true;
				break;

			case 'V': // version
				_tprintf(_T("%s - Version %s - %s\n"), APPNAME, APPVERSION, _T(__DATE__)
#ifdef _DEBUG
				 _T(" ") _T(__TIME__) _T(" (Debug build)")
#endif
				);
				_tprintf(APPCOPYRIGHT);
				_tprintf(_T("\n\n"));

				_tprintf(
					_T("This program is distributed in the hope that it will be useful,\n")
					_T("but WITHOUT ANY WARRANTY; without even the implied warranty of\n")
					_T("MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the\n")
					_T("GNU General Public License for more details.\n")
				);

				exit(0);
				break;

			case '?': // help
				usage(0);
				break;

			default:
				usage(1);
		}
	}

	if (opt.execute) {
		return execute(argc, argv, toptind);
	}

	if (opt.o_export) {
		return do_export(argc, argv, toptind);
	}

	if (opt.sort) {
		return sort(argc, argv, toptind);
	}

	return diff(argc, argv, toptind);

//	_getch();

	return rv;
}
