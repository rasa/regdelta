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

#ifndef _REG_VALUE_H
#define _REG_VALUE_H

#include <tchar.h>

class RegValue {
	public:
		DWORD	 value_type;
		TCHAR	*value_name;
		DWORD	 value_len;
		BYTE	*value_data;
		DWORD	 value_data_len;

	RegValue(DWORD value_type, const TCHAR *value_name, DWORD value_len, const BYTE *value_data, DWORD _value_data_len);

	~RegValue();
};

#endif /* _REG_VALUE_H */
