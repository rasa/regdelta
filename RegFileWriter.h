// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

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