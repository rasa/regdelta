// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

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
