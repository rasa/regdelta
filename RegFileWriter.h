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

#ifndef _REG_FILE_WRITER_H
#define _REG_FILE_WRITER_H

#include "RegFile.h"

class RegFileWriter : public RegFile {
	private:
		const TCHAR	*last_section;
		int last_section_len;

		const TCHAR	*last_line;
		int last_line_len;

	public:
		RegFileWriter();

		~RegFileWriter();

		void writeComment(const TCHAR *line);

		void writeDelLine(const TCHAR *line);

		void writeDelSection(const TCHAR *section);

		void writeLine(const TCHAR *line);

		void writeSection(const TCHAR *section);
};

#endif /* _REG_FILE_WRITER_H */