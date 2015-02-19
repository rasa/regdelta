// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#include <assert.h>
#include <tchar.h>
#include <windows.h>

#include "RegFileWriter.h"

RegFileWriter::RegFileWriter(/*const HANDLE _fh*/) {
	last_section	= empty_string;
	last_line		= empty_string;
};

RegFileWriter::~RegFileWriter() {
};

void RegFileWriter::writeComment(const TCHAR *line) {
	const static TCHAR *semi			= _T(";");
	const static DWORD semi_len		= _tcslen(semi);

	if (line != empty_string) {
		_write_file(semi, semi_len);
		_write_string(line);
		_write_eol();
	}
};

void RegFileWriter::writeDelLine(const TCHAR *line) {
	last_line = line;
	const TCHAR *p = line;
	int equals = 0;
	for (int i = 3; line[i]; ++i) {
		if (line[i] == '=') {
			equals = i + 1;
			break;
		}
	}

	if (equals) {
		_write_file(line, equals);
		_write_string(_T("-"));
		_write_eol();
	}
};

void RegFileWriter::writeDelSection(const TCHAR *section) {
	if (section == last_section)
		return;

	last_section	= section;
	_write_string(EOL _T("[-"));
	_write_string(&section[1]);
	_write_eol();
};

void RegFileWriter::writeLine(const TCHAR *line) {
	last_line = line;
	_write_string(line);
	_write_eol();
};

void RegFileWriter::writeSection(const TCHAR *section) {
	if (section == last_section)
		return;
	last_section = section;

	_write_eol();
	_write_string(section);
	_write_eol();
};
