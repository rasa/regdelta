// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

#if _MSC_VER > 1000
#pragma once
#endif

#ifndef _REG_MAN_H
#define _REG_MAN_H

/*
#define INLINE inline
generates the following errors:
regdelta.obj : error LNK2001: unresolved external symbol "public: char * __thiscall RegFileReader::getLine(void)" (?getLine@RegFileReader@@QAEPADXZ)
regdelta.obj : error LNK2001: unresolved external symbol "public: bool __thiscall RegFile::eof(void)" (?eof@RegFile@@QAE_NXZ)
RegFileReader.obj : error LNK2001: unresolved external symbol "public: bool __thiscall RegFile::eof(void)" (?eof@RegFile@@QAE_NXZ)
RegFileReader.obj : error LNK2001: unresolved external symbol "public: unsigned long __thiscall RegFile::length(void)" (?length@RegFile@@QAEKXZ)

*/

#define INLINE

#endif /* _REG_MAN_H */
