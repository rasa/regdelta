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
