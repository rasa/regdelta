// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#include <assert.h>
#include <tchar.h>
#include <windows.h>

#include "RegFileReader.h"

RegFileReader::RegFileReader() {
	line		= const_cast<TCHAR *>(empty_string);
	line_len	= 0;
	_isSection	= false;
	section		= const_cast<TCHAR *>(empty_string);
	section_len	= 0;
	equals_pos	= 0;
};

RegFileReader::~RegFileReader() {
};

bool RegFileReader::areValuesEqual(RegFileReader *r2) {
	if (_isSection || r2->isSection())
		return true;
	int len = length();

	if (len != r2->length())
		return false;

	for (int i = equals_pos + 1; i < len; ++i) {
		if (line[i] != r2->getChar(i))
			return false;
	}
	return true;
};

INLINE TCHAR RegFileReader::getChar(int n) {
	return line[n];
};

INLINE int RegFileReader::getEqualPos() {
	return equals_pos;
};

INLINE TCHAR * RegFileReader::getLine() {
	return _isSection ? const_cast<TCHAR *>(empty_string) : line;
};

INLINE TCHAR * RegFileReader::getSection() {
	return section;
};

INLINE bool RegFileReader::isSection() {
	return _isSection;
};

int RegFileReader::keyCompare(RegFileReader *r2) {
	if (_isSection) {
		if (r2->isSection()) {
			return 0;
		} else {
			return -1;
		}
	} else {
		if (r2->isSection()) {
			return 1;
		}
	}
	int len = length();

	int len2 = r2->length();
	for (int i = 0; i < len, i < len2; ++i) {
		if (line[i] == '=') {
			equals_pos = i;
			if (r2->getChar(i) == '=')
				return 0;
			else
				return -1;
		}
		if (line[i] == r2->getChar(i))
			continue;
		if (line[i] > r2->getChar(i))
			return 1;
		return -1;
	}
	return len > len2 ? 1 : (len < len2 ? -1 : 0);
};

TCHAR *RegFileReader::readNextLine() {
	if (eof()) {
		return const_cast<TCHAR *>(empty_string);
	}

	while (!eof()) {
		line = readline();
		line_len = length();
		if (line_len > 0)
			if (line[0] == '[' || line[0] == '@' || line[0] == '"')
				break;
	}

	equals_pos = 0;

	if (length() == 0) {
		_isSection = false;
		return const_cast<TCHAR *>(empty_string);
	}

	_isSection = line[0] == '[';
	if (_isSection) {
		section = line;
		section_len = length();
	}

	return line;
};

int RegFileReader::sectionCompare(RegFileReader *r2) {
	return _tcsicmp(section, r2->getSection());
}

bool RegFileReader::skipDelSection() {
	if (!_isSection)
		return false;

	int left_bracket_pos = 0;
	for (int i = section_len - 1; i >= 0; --i) {
		if (section[i] == ']') {
			left_bracket_pos = i;
			break;
		}
	}
	if (left_bracket_pos == 0)
		return false;

	while (!eof()) {
		line = readline();
		if (length() == 0)
			continue;
		if (line[0] == '[') {
			if (_tcsncmp(line, section, left_bracket_pos))
				break;
		}
	}

	equals_pos = 0;
	line_len = length();
	if (line_len > 0) {
		_isSection = line[0] == '[';
		if (_isSection) {
			section = line;
			section_len = length();
		}
	}

	return !eof();
};

bool RegFileReader::skipHeader() {
	while (!eof()) {
		readNextLine();
		if (length() > 0)
			if (line[0] == '[' || line[0] == '@' || line[0] == '"')
				break;
	}
	return !eof();
};

bool RegFileReader::skipToNextSection() {
	while (!eof()) {
		readNextLine();
		if (eof())
			break;
		if (_isSection)
			break;
	}
	return !eof();
};
