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

#ifndef _REG_FILE_READER_H
#define _REG_FILE_READER_H

#include "RegFile.h"

class RegFileReader : public RegFile {
	private:
		TCHAR	*line;
		DWORD	line_len;
		bool	_isSection;
		TCHAR	*section;
		DWORD	section_len;
		DWORD	equals_pos;

	public:
		RegFileReader();

		~RegFileReader();

		bool areValuesEqual(RegFileReader *r2);

		INLINE TCHAR getChar(int n);

		INLINE int getEqualPos();

		INLINE TCHAR * getLine();

		INLINE TCHAR * getSection();

		INLINE bool isSection();

		int keyCompare(RegFileReader *r2);
		
		TCHAR *readNextLine();

		int sectionCompare(RegFileReader *r2);

		bool skipDelSection();

		bool skipHeader();

		bool skipToNextSection();
};

#endif /* _REG_FILE_READER_H */