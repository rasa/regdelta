// Copyright (c) 2005-2015 Ross Smith II. See Mit LICENSE in /LICENSE

#ifndef WIN32_LEAN_AND_MEAN
# define WIN32_LEAN_AND_MEAN 1
#endif

#include <tchar.h>
#include <windows.h>
#include <stdio.h>

#include "RegValue.h"

RegValue::RegValue(DWORD value_type, const TCHAR *value_name, DWORD value_len, const BYTE *value_data, DWORD _value_data_len) {
		this->value_type = value_type;
		this->value_len = value_len;
		this->value_data_len = _value_data_len;
		this->value_name = new TCHAR[value_len + 1];
		memcpy(this->value_name, value_name, value_len * sizeof(TCHAR));
		this->value_name[value_len] = '\0';
		this->value_data = new BYTE[_value_data_len + sizeof(TCHAR)];
		memcpy(this->value_data, value_data, _value_data_len);
		this->value_data[_value_data_len] = '\0';
#ifdef _UNICODE
		this->value_data[_value_data_len + 1] = '\0';
#endif
};

RegValue::~RegValue() {
		delete [] value_name;
		delete [] value_data;
		value_name = NULL;
		value_data = NULL;
};
