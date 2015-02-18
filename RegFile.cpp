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


#if _MSC_VER > 1000
#pragma once
#endif

#ifdef _MSC_VER
#pragma warning(disable:4786)
#pragma warning(disable:4996)
#endif

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#include <map>

#include <assert.h>
#include <tchar.h>
#include <tstl.h>
#include <windows.h>

#include "RegFile.h"
#include "RegValue.h"

#include "SystemMessage.h"

using namespace std;

typedef map<HKEY, tstring>		HKEY_String_Map;
typedef map<tstring, RegValue*> String_RegValue_Map;
typedef map<tstring, tstring>	String_String_Map;

static HKEY_String_Map hkey_name;

RegFile::RegFile() {
	filename = NULL;
	fh = NULL;
	size = 0;
	buffer = NULL;
	pos = 0;
	_length = 0;
	_eof = false;
	_opened = false;

	quirks = false;
	unsorted = false;
	verbose = false;
	wordwrap = false;

	if (hkey_name.empty()) {
		hkey_name[HKEY_CLASSES_ROOT]	= _T("HKEY_CLASSES_ROOT");
		hkey_name[HKEY_CURRENT_CONFIG]	= _T("HKEY_CURRENT_CONFIG");
		hkey_name[HKEY_CURRENT_USER]	= _T("HKEY_CURRENT_USER");
		hkey_name[HKEY_LOCAL_MACHINE]	= _T("HKEY_LOCAL_MACHINE");
		hkey_name[HKEY_USERS]			= _T("HKEY_USERS");
	}
};

RegFile::~RegFile() {
	close();
}

bool RegFile::getQuirks() {
	return quirks;
}

void RegFile::setQuirks(bool b) {
	quirks = b;
}

bool RegFile::getUnsorted() {
	return unsorted;
}

void RegFile::setUnsorted(bool b) {
	unsorted = b;
}

bool RegFile::getVerbose() {
	return verbose;
}

void RegFile::setVerbose(bool b) {
	verbose = b;
}

bool RegFile::getWordwrap() {
	return wordwrap;
}

void RegFile::setWordwrap(bool b) {
	wordwrap = b;
}

int RegFile::create(const TCHAR *filename) {
	return _open(filename, GENERIC_WRITE, CREATE_ALWAYS);
}

int RegFile::open(const TCHAR *filename) {
	return _open(filename, GENERIC_READ, OPEN_EXISTING);
}

int RegFile::_open(const TCHAR *filename, const DWORD dwDesiredAccess, const DWORD dwCreationDisposition) {
	this->filename = filename;
	_eof = false;

	if (_tcscmp(this->filename, _T("-")) == 0) {
		if (dwDesiredAccess == GENERIC_WRITE) {
			fh = GetStdHandle(STD_OUTPUT_HANDLE);
			return 0;
		} else {
			fh = GetStdHandle(STD_INPUT_HANDLE);
			return 0;
		}
	}
	
	fh = CreateFile(
		this->filename,
		dwDesiredAccess,
		FILE_SHARE_READ,		// DWORD dwShareMode,
		NULL,					// LPSECURITY_ATTRIBUTES lpSecurityAttributes,
		dwCreationDisposition,			// DWORD dwCreationDisposition,
		FILE_ATTRIBUTE_NORMAL,	// DWORD dwFlagsAndAttributes,
		NULL					// HANDLE hTemplateFile
	);

	if (fh == INVALID_HANDLE_VALUE) {
		DWORD rv = GetLastError();
//				return rv;
		_ftprintf(stderr, _T("Cannot open file %s: Error %d: %s"), filename, rv, SystemMessage(rv));
		exit(rv);
	}

	LARGE_INTEGER size64;

	DWORD rv = GetFileSizeEx(fh, &size64);
	if (rv == 0)
		return GetLastError();

	size = size64.LowPart;

	_opened = true;
	return 0;
}

int RegFile::readall() {
	if (buffer != NULL) {
		return 0;
	}

	buffer = new TCHAR[size + sizeof(TCHAR)];
	if (buffer == NULL) {
		return GetLastError();
	}

	DWORD read;

	BOOL b = ReadFile(
		fh,
		buffer,		// LPVOID lpBuffer,
		size,		// DWORD nNumberOfBytesToRead,
		&read,		// LPDWORD lpNumberOfBytesRead,
		NULL		// LPOVERLAPPED lpOverlapped
	);

	if (!b) {
		return GetLastError();
	}

	if (read != size) {
		return GetLastError();
	}

	return 0;
}

INLINE bool RegFile::isOpen() {
	return 	_opened;
}

INLINE bool RegFile::eof() {
	return _eof;
}

INLINE DWORD RegFile::length() {
	return _length;
}

TCHAR *RegFile::readline() {
	if (!_opened || _eof)
		return NULL;

	if (buffer == NULL) {
		int rv = readall();
		if (rv != 0)
			return NULL;
	}

	if (pos >= size) {
		_eof = true;
		_length = 0;
		return const_cast<TCHAR *>(empty_string);
	}

	TCHAR *rv = (TCHAR *) (buffer + pos);

	while (pos < size) {
		if (buffer[pos] != '\n' && buffer[pos] != '\r') {
			pos += sizeof(TCHAR);
			continue;
		}

		buffer[pos] = '\0';
		pos += sizeof(TCHAR);
		_length = buffer + pos - rv;
		if (pos < size && buffer[pos] == '\n' && buffer[pos - 1] == '\r') {
			buffer[pos] = '\0';
			pos += sizeof(TCHAR);
		}
		return rv;
	}

	buffer[pos] = '\0';
	pos += sizeof(TCHAR);
	
	_length = buffer + pos - rv;
	return rv;
}

int RegFile::close() {
	if (buffer != NULL) {
		delete [] buffer;
		buffer = NULL;
	}

	size = 0;
	pos = 0;
	_length = 0;
	_eof = false;
	_opened = false;

	if (fh == NULL)
		return 0;

	if (fh == GetStdHandle(STD_OUTPUT_HANDLE))
		return 0;

	if (fh == GetStdHandle(STD_INPUT_HANDLE))
		return 0;

	DWORD rv = CloseHandle(fh);
	fh = NULL;
	return rv;
}

void RegFile::_write_char(const TCHAR c) {
	if (!_opened)
		return;

	static DWORD len = sizeof(TCHAR);

	DWORD written = 0;

	BOOL b = WriteFile(fh, &c, len, &written, NULL);
	if (!b) {
		DWORD rv = GetLastError();
		_ftprintf(stderr, _T("Error %u: %s"), rv, SystemMessage(rv));
		exit(rv);
	}
}

void RegFile::_write_file(const TCHAR *p, DWORD len) {
	if (!_opened)
		return;

	if (!p || !*p)
		return;

#ifdef _UNICODE
	len *= sizeof(TCHAR);
#endif

	DWORD written = 0;
	BOOL b = WriteFile(fh, p, len, &written, NULL);
	if (!b) {
		DWORD rv = GetLastError();
		_ftprintf(stderr, _T("Error %u: %s"), rv, SystemMessage(rv));
		exit(rv);
	}
}

void RegFile::_write_bytes(const BYTE *bytes, const DWORD length, DWORD output_pos) {
	const static TCHAR *comma			= _T(",");
	const static DWORD comma_len		= _tcslen(comma);
	const static TCHAR *bs_eol_sp_sp	= _T("\\") EOL _T("  ");
	const static DWORD bs_eol_sp_sp_len	= _tcslen(bs_eol_sp_sp);

	if (!_opened)
		return;

	if (length == 0)
		return;

	DWORD tsize = 256 + length * 4;
//		80 + // fudge factor
//		(length + 1) * 5 / 2 + // 2 hex chars and a comma for each byte
//		(bs_eol_sp_sp_len * (1 + length / 70)); // 5 chars for each 70 bytes
	TCHAR *tchars = new TCHAR[tsize];
	DWORD tpos = 0;

	assert(bytes[0] >= 0 && bytes[0] < 256);

	tchars[tpos++] = hex_chars[bytes[0]][0];
	tchars[tpos++] = hex_chars[bytes[0]][1];
	
//	_write_file(hex_chars[bytes[0]], 2);

	output_pos += 2;

	for (int i = 1; i < (int) length; ++i) {
		assert(tpos < tsize);
		tchars[tpos++] = ',';
		//_write_file(comma, comma_len);
		if (wordwrap && output_pos > 75) {
			assert(tpos < tsize);
			tchars[tpos++] = '\\';
			tchars[tpos++] = '\r';
			tchars[tpos++] = '\n';
			tchars[tpos++] = ' ';
			tchars[tpos++] = ' ';
			// _write_file(bs_eol_sp_sp, bs_eol_sp_sp_len);
			output_pos = 1;
		}
		//assert(bytes[i] >= 0 && bytes[i] < 256);
		assert(tpos < tsize);
		tchars[tpos++] = hex_chars[bytes[i]][0];
		assert(tpos < tsize);
		tchars[tpos++] = hex_chars[bytes[i]][1];
		assert(tpos < tsize);
		//_write_file(hex_chars[bytes[i]], 2);
		output_pos += 3;
	}

	_write_file(tchars, tpos);
	delete [] tchars;
}

void RegFile::_write_byte(const BYTE byte) {
	assert(byte >= 0 && byte < 256);
	_write_file(hex_chars[byte], 2);
}

void RegFile::_write_eol() {
	const static TCHAR *eol			= EOL;
	const static DWORD eol_len		= _tcslen(eol);

	_write_file(eol, eol_len);
}

DWORD RegFile::_write_estring(const TCHAR *s) {
	const static TCHAR *bs = _T("\\");
	const static DWORD bs_len = _tcslen(bs);

	if (!_opened)
		return 0;

	if (!s || !*s)
		return 0;

	DWORD length = _tcslen(s);

	TCHAR *tchars = new TCHAR[length * 2];
	DWORD tpos = 0;
	
	int rv = length;
	for (int i = 0; i < (int) length; ++i) {
		TCHAR c = s[i];
		if (quirks) {
			if (c == '\r') {
				//_write_char(c);
				tchars[tpos++] = c;
				++rv;
			}
		}
		if (c == '"' || c == '\\') {
			// _write_file(bs, bs_len);
			tchars[tpos++] = '\\';
			++rv;
		}
		tchars[tpos++] = c;
		// _write_char(c);
	}

	_write_file(tchars, tpos);

	delete [] tchars;
	return rv;
}

void RegFile::writeHeader(bool eol) {
	static int reg_header_len = _tcslen(reg_header);

	_write_file(reg_header, reg_header_len);
	if (eol)
		_write_eol();
}

void RegFile::_write_string(const TCHAR *s) {
	if (!_opened)
		return;

	if (!s || !*s)
		return;

	int len = _tcslen(s);

	_write_file(s, len);
}

int RegFile::_write_reg_value(const DWORD reg_type, const BYTE *value_data, const DWORD value_data_len, const DWORD output_pos) {
	const static TCHAR *hex				= _T("hex:");
	const static DWORD hex_len			= _tcslen(hex);
	const static TCHAR *dword			= _T("dword:");
	const static DWORD dword_len		= _tcslen(dword);
	const static TCHAR *quote			= _T("\"");
	const static DWORD quote_len		= _tcslen(quote);
	const static TCHAR *hex_lparen		= _T("hex(");
	const static DWORD hex_lparen_len	= _tcslen(hex_lparen);
	const static TCHAR *rparen			= _T("):");
	const static DWORD rparen_len		= _tcslen(rparen);

	switch (reg_type) {
		case REG_BINARY:
			_write_file(hex, hex_len);
			_write_bytes(value_data, value_data_len, output_pos + 4);
			break;
			
		case REG_DWORD:
			_write_file(dword, dword_len);
#if REG_DWORD == REG_DWORD_LITTLE_ENDIAN
			_write_byte(value_data[3]);
			_write_byte(value_data[2]);
			_write_byte(value_data[1]);
			_write_byte(value_data[0]);
#else
			_write_byte(value_data[0]);
			_write_byte(value_data[1]);
			_write_byte(value_data[2]);
			_write_byte(value_data[3]);
#endif
			break;
			
#if REG_DWORD != REG_DWORD_LITTLE_ENDIAN
		case REG_DWORD_LITTLE_ENDIAN:
			_write_file(dword, dword_len);
			_write_byte(value_data[3]);
			_write_byte(value_data[2]);
			_write_byte(value_data[1]);
			_write_byte(value_data[0]);
			break;
#endif

#if REG_DWORD != REG_DWORD_BIG_ENDIAN
		case REG_DWORD_BIG_ENDIAN:
			_write_file(dword, dword_len);
			_write_byte(value_data[0]);
			_write_byte(value_data[1]);
			_write_byte(value_data[2]);
			_write_byte(value_data[3]);
			break;
#endif

		case REG_SZ:
			_write_file(quote, quote_len);
			if (value_data_len > 0) {
				_write_estring((TCHAR*) value_data);
			}
			_write_file(quote, quote_len);
			break;

		case REG_NONE:							// 0?
		case REG_EXPAND_SZ:						// 2
		case REG_LINK:							// 6 Reserved for system use.
		case REG_MULTI_SZ:						// 7: Array of null-terminated strings, terminated by two null characters.
		case REG_RESOURCE_LIST:					// 8
		case REG_FULL_RESOURCE_DESCRIPTOR:		// 9
		case REG_RESOURCE_REQUIREMENTS_LIST:	// 10
		case REG_QWORD:							// 11
			_write_file(hex_lparen, hex_lparen_len);
			if (reg_type < 16)
				_write_file(hex_char[reg_type], 1);
			else
				_write_file(hex_chars[reg_type], 2);
			_write_file(rparen, rparen_len);
			// Binary data in any form.
			_write_bytes(value_data, value_data_len, output_pos + 7);
			break;

		default:
			return ! ERROR_SUCCESS;
	}

	return ERROR_SUCCESS;
}

DWORD RegFile::writeValues(const HKEY hKey, const TCHAR *root) { 
	const static TCHAR *lbracket		= _T("[");
	const static DWORD lbracket_len		= _tcslen(lbracket);
	const static TCHAR *bs				= _T("\\");
	const static DWORD bs_len			= _tcslen(bs);
	const static TCHAR *rbracket		= _T("]");
	const static DWORD rbracket_len		= _tcslen(rbracket);
	const static TCHAR *rbracket_eol	= _T("]") EOL;
	const static DWORD rbracket_eol_len	= _tcslen(rbracket_eol);
	const static TCHAR *eol				= EOL;
	const static DWORD eol_len			= _tcslen(eol);
	const static TCHAR *at_equals		= _T("@=");
	const static DWORD at_equals_len	= _tcslen(at_equals);
	const static TCHAR *quote			= _T("\"");
	const static DWORD quote_len		= _tcslen(quote);
	const static TCHAR *quote_equals	= _T("\"=");
	const static DWORD quote_equals_len	= _tcslen(quote_equals);

	if (!_opened)
		return 0;

	HKEY hSubKey = 0;
	
	DWORD rv = RegOpenKeyEx(hKey, root, 0, KEY_READ | KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hSubKey);
	if (rv != ERROR_SUCCESS) {
		if (verbose || rv != ERROR_ACCESS_DENIED)
			_ftprintf(stderr, _T("Cannot open key %s\\%s: Error %u: %s"), hkey_name[hKey].c_str(), root ? root : empty_string, rv, SystemMessage(rv));
		if (rv == ERROR_ACCESS_DENIED)
			return 0;
		return rv;
	}

	while (1) {
		_write_file(lbracket, lbracket_len);
		
		_write_file(hkey_name[hKey].c_str(), hkey_name[hKey].length());

		if (root) {
			_write_file(bs, bs_len);
			_write_file(root, _tcslen(root));
		}
		_write_file(rbracket_eol, rbracket_eol_len);

		DWORD    num_sub_keys			= 0;
		DWORD    max_sub_key_len		= 0;
		DWORD    num_values				= 0;
		DWORD    max_value_len			= 0;
		DWORD    max_value_data_len		= 0;
		DWORD    security_descriptor	= 0;

		rv = RegQueryInfoKey(
			hSubKey,                // key handle 
			NULL,                   // buffer for class name 
			NULL,                   // size of class string 
			NULL,                   // reserved 
			&num_sub_keys,          // number of subkeys 
			&max_sub_key_len,       // longest subkey size 
			NULL,                   // longest class string 
			&num_values,            // number of values for this key 
			&max_value_len,         // longest value name 
			&max_value_data_len,    // longest value data 
			&security_descriptor,   // security descriptor 
			NULL);                  // last write time 

		if (rv != ERROR_SUCCESS) {
			_ftprintf(stderr, _T("Cannot query key %s\\%s: Error %u: %s"), hkey_name[hKey], root ? root : empty_string, rv, SystemMessage(rv));
			break;
		}

		max_value_len += 2;
		max_value_data_len += 2;

//max_value_len += 4;

		TCHAR	*value_name		= new TCHAR[max_value_len];
		BYTE	*value_data		= new BYTE[max_value_data_len];

		if (!value_name || !value_data) {
			rv = GetLastError();
			_ftprintf(stderr, _T("Out of memory for key %s\\%s: Error %u: %s"), hkey_name[hKey], root ? root : empty_string, rv, SystemMessage(rv));
			break;
		}

		String_RegValue_Map value_map;

		for (DWORD i = 0; i < num_values; ++i) {
			DWORD reg_type = 0;
			DWORD _value_data_len = max_value_data_len;

			ZeroMemory(value_name, max_value_len);
			ZeroMemory(value_data, max_value_data_len);

			DWORD value_len = max_value_len;

			rv = RegEnumValue(
				hSubKey, 
				i, 
				value_name, 
				&value_len, 
				NULL, 
				&reg_type,
				value_data,
				&_value_data_len);

			if (rv == ERROR_NO_MORE_ITEMS) {
				rv = ERROR_SUCCESS;
				break;
			}

			RegValue *value_node = new RegValue(reg_type, value_name, value_len, value_data, _value_data_len);

			tstring sort_key;
			if (unsorted) {
				static TCHAR s[16];
				_stprintf(s, _T("%09d"), i);
				sort_key = s;
			} else {
				sort_key = value_name;
			}

			value_map[sort_key] = value_node;
		}

		delete [] value_name;
		value_name = NULL;
		delete [] value_data;
		value_data = NULL;

		if (rv != ERROR_SUCCESS) {
			break;
		}

		for (String_RegValue_Map::const_iterator it = value_map.begin(); it != value_map.end(); ++it) {
			tstring sort_key = (tstring) it->first;
			RegValue *value_node = (RegValue *) it->second;

			int output_pos;

			if (value_node->value_len == 0) {
				output_pos = 2;
				_write_file(at_equals, at_equals_len);
			} else {
				_write_file(quote, quote_len);
				output_pos = _write_estring(value_node->value_name) + 3;
				_write_file(quote_equals, quote_equals_len);
			}

			rv = _write_reg_value(value_node->value_type, value_node->value_data, value_node->value_data_len, output_pos);
			if (rv != ERROR_SUCCESS) {
				_ftprintf(stderr, _T("Unknown registry type %d for %s\\%s"), value_node->value_type, hkey_name[hKey], root ? root : empty_string);
				break;
			}
			
			_write_file(eol, eol_len);
		}

		_write_file(eol, eol_len);

		if (rv != ERROR_SUCCESS) {
			break;
		}

		rv = _write_subkeys(hKey, root, num_sub_keys, hSubKey);

		break;
	}

	RegCloseKey(hSubKey);

	return rv;
}

DWORD RegFile::_write_subkeys(const HKEY hKey, const TCHAR *root, const DWORD num_sub_keys, const HKEY hSubKey) {
	if (!_opened)
		return 0;

	DWORD rv = 0;

	DWORD max_sub_key_len = MAX_KEY_NAME_LENGTH + 1;

	TCHAR *sub_key_name = new TCHAR[max_sub_key_len];

	if (!sub_key_name) {
		rv = GetLastError();
		_ftprintf(stderr, _T("Out of memory for key %s\\%s: Error %u: %s"), hkey_name[hKey], root ? root : empty_string, rv, SystemMessage(rv));
		return rv;
	}

	String_String_Map key_map;

	for (int i = 0; i < (int) num_sub_keys; ++i) { 
		DWORD sub_key_len;

		sub_key_len = max_sub_key_len;

		ZeroMemory(sub_key_name, sub_key_len);

		rv = RegEnumKeyEx(
			hSubKey, 
			i, 
			sub_key_name, 
			&sub_key_len,
			NULL, 
			NULL, 
			NULL, 
			NULL); 

		if (rv == ERROR_NO_MORE_ITEMS) {
			rv = ERROR_SUCCESS;
			break;
		}

		if (rv != ERROR_SUCCESS) {
			_ftprintf(stderr, _T("Cannot enum key %s\\%s: Error %d: %s"), hkey_name[hKey], root ? root : empty_string, rv, SystemMessage(rv));
#ifdef _DEBUG
			_ftprintf(stderr, _T("i=%u, sub_key_len=%u, sub_key_name=%s\n"), i, sub_key_len, sub_key_name);
#endif
			break;
		} 

		if (sub_key_len <= MAX_KEY_NAME_LENGTH)
			sub_key_name[sub_key_len] = '\0';

		tstring sort_key;
		if (unsorted) {
			static TCHAR s[16];
			_stprintf(s, _T("%09d"), i);
			sort_key = s;
		} else {
			sort_key = sub_key_name;
		}

		key_map[sort_key] = sub_key_name;
	}

	delete [] sub_key_name;
	sub_key_name = NULL;

	if (rv != ERROR_SUCCESS) {
		return rv;
	} 

	for (String_String_Map::const_iterator it = key_map.begin(); it != key_map.end(); ++it) {
		tstring sub_key_name = (tstring) it->second;

		tstring new_root;

		if (root) {
			new_root = root;
			new_root += _T("\\");
			new_root += sub_key_name;
		} else {
			new_root = sub_key_name;
		}

		const TCHAR *gnu_root = new_root.c_str();

		rv = writeValues(hKey, gnu_root);
	}

	return rv;
};
