// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _FILE_H
#define _FILE_H

#include <tchar.h>
#include <tstl.h>
#include <windows.h>

#include "regdelta.h"

#define CR _T("\r")
#define LF _T("\n")
#define CRLF CR LF
#define EOL CRLF

const DWORD MAX_KEY_NAME_LENGTH = 255;
const DWORD MAX_VALUE_LENGTH = 16383;

#ifdef _UNICODE
	#define REG_HEADER _T("\xfeff") _T("Windows Registry Editor Version 5.00")
#else
	#define REG_HEADER _T("REGEDIT4")
#endif

static const TCHAR *empty_string = _T("");

static const TCHAR reg_header[] = REG_HEADER EOL;

static const TCHAR *hex_char[] = {
	_T("0"),_T("1"),_T("2"),_T("3"),_T("4"),_T("5"),_T("6"),_T("7"),
	_T("8"),_T("9"),_T("a"),_T("b"),_T("c"),_T("d"),_T("e"),_T("f")
};

static const TCHAR hex_char2[] = {
	'0','1','2','3','4','5','6','7','8','9','a','b','c','d','e','f'
};

static const TCHAR *hex_chars[] = {
	_T("00"),_T("01"),_T("02"),_T("03"),_T("04"),_T("05"),_T("06"),_T("07"),
	_T("08"),_T("09"),_T("0a"),_T("0b"),_T("0c"),_T("0d"),_T("0e"),_T("0f"),
	_T("10"),_T("11"),_T("12"),_T("13"),_T("14"),_T("15"),_T("16"),_T("17"),
	_T("18"),_T("19"),_T("1a"),_T("1b"),_T("1c"),_T("1d"),_T("1e"),_T("1f"),
	_T("20"),_T("21"),_T("22"),_T("23"),_T("24"),_T("25"),_T("26"),_T("27"),
	_T("28"),_T("29"),_T("2a"),_T("2b"),_T("2c"),_T("2d"),_T("2e"),_T("2f"),
	_T("30"),_T("31"),_T("32"),_T("33"),_T("34"),_T("35"),_T("36"),_T("37"),
	_T("38"),_T("39"),_T("3a"),_T("3b"),_T("3c"),_T("3d"),_T("3e"),_T("3f"),
	_T("40"),_T("41"),_T("42"),_T("43"),_T("44"),_T("45"),_T("46"),_T("47"),
	_T("48"),_T("49"),_T("4a"),_T("4b"),_T("4c"),_T("4d"),_T("4e"),_T("4f"),
	_T("50"),_T("51"),_T("52"),_T("53"),_T("54"),_T("55"),_T("56"),_T("57"),
	_T("58"),_T("59"),_T("5a"),_T("5b"),_T("5c"),_T("5d"),_T("5e"),_T("5f"),
	_T("60"),_T("61"),_T("62"),_T("63"),_T("64"),_T("65"),_T("66"),_T("67"),
	_T("68"),_T("69"),_T("6a"),_T("6b"),_T("6c"),_T("6d"),_T("6e"),_T("6f"),
	_T("70"),_T("71"),_T("72"),_T("73"),_T("74"),_T("75"),_T("76"),_T("77"),
	_T("78"),_T("79"),_T("7a"),_T("7b"),_T("7c"),_T("7d"),_T("7e"),_T("7f"),
	_T("80"),_T("81"),_T("82"),_T("83"),_T("84"),_T("85"),_T("86"),_T("87"),
	_T("88"),_T("89"),_T("8a"),_T("8b"),_T("8c"),_T("8d"),_T("8e"),_T("8f"),
	_T("90"),_T("91"),_T("92"),_T("93"),_T("94"),_T("95"),_T("96"),_T("97"),
	_T("98"),_T("99"),_T("9a"),_T("9b"),_T("9c"),_T("9d"),_T("9e"),_T("9f"),
	_T("a0"),_T("a1"),_T("a2"),_T("a3"),_T("a4"),_T("a5"),_T("a6"),_T("a7"),
	_T("a8"),_T("a9"),_T("aa"),_T("ab"),_T("ac"),_T("ad"),_T("ae"),_T("af"),
	_T("b0"),_T("b1"),_T("b2"),_T("b3"),_T("b4"),_T("b5"),_T("b6"),_T("b7"),
	_T("b8"),_T("b9"),_T("ba"),_T("bb"),_T("bc"),_T("bd"),_T("be"),_T("bf"),
	_T("c0"),_T("c1"),_T("c2"),_T("c3"),_T("c4"),_T("c5"),_T("c6"),_T("c7"),
	_T("c8"),_T("c9"),_T("ca"),_T("cb"),_T("cc"),_T("cd"),_T("ce"),_T("cf"),
	_T("d0"),_T("d1"),_T("d2"),_T("d3"),_T("d4"),_T("d5"),_T("d6"),_T("d7"),
	_T("d8"),_T("d9"),_T("da"),_T("db"),_T("dc"),_T("dd"),_T("de"),_T("df"),
	_T("e0"),_T("e1"),_T("e2"),_T("e3"),_T("e4"),_T("e5"),_T("e6"),_T("e7"),
	_T("e8"),_T("e9"),_T("ea"),_T("eb"),_T("ec"),_T("ed"),_T("ee"),_T("ef"),
	_T("f0"),_T("f1"),_T("f2"),_T("f3"),_T("f4"),_T("f5"),_T("f6"),_T("f7"),
	_T("f8"),_T("f9"),_T("fa"),_T("fb"),_T("fc"),_T("fd"),_T("fe"),_T("ff")
};

class RegFile {
	protected:
		const TCHAR *filename;
		HANDLE fh;
		DWORD size;
		TCHAR* buffer;
		DWORD pos;
		DWORD _length;
		bool _eof;
		bool _opened;

		bool quirks;
		bool unsorted;
		bool verbose;
		bool wordwrap;

	protected:
		RegFile();

		~RegFile();

		int _open(const TCHAR *filename, const DWORD dwDesiredAccess, const DWORD dwCreationDisposition);

	public:
		bool getQuirks();

		void setQuirks(bool b);

		bool getUnsorted();

		void setUnsorted(bool b);

		bool getVerbose();

		void setVerbose(bool b);

		bool getWordwrap();

		void setWordwrap(bool b);

		int create(const TCHAR *filename);

		int open(const TCHAR *filename);

		int close();

		void writeHeader(bool eol);

		DWORD writeValues(const HKEY hKey, const TCHAR *root);

		int readall();

		INLINE bool eof();

		INLINE bool isOpen();
	protected:
		INLINE DWORD length();

		TCHAR *readline();

		void _write_byte(const BYTE byte);

		void _write_bytes(const BYTE *bytes, const DWORD length, DWORD output_pos);

		void _write_char(const TCHAR c);

		void _write_eol();

		void _write_file(const TCHAR *p, DWORD len);

		void _write_string(const TCHAR *s);

		DWORD _write_estring(const TCHAR *s);

		int _write_reg_value(const DWORD reg_type, const BYTE *value_data, const DWORD value_data_len, const DWORD output_pos);

		DWORD _write_subkeys(const HKEY hKey, const TCHAR *root, const DWORD num_sub_keys, const HKEY hSubKey);
};

#endif /* _FILE_H */
