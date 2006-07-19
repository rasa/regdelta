#!/bin/bash

. ./config.sh $*

rm -f *_8_regedit.reg

set -x
./auto_regedit.exe
set +x

for i in $ROOT_KEYS
do
	f=${i}_8_regedit.reg
	test -f $f || (echo File not found: $f ; false) || exit 1
done

grep -q "\[HKEY_CURRENT_CONFIG\]"	HKCC_8_regedit.reg	|| (echo HKCC_8_regedit.reg	missing HKEY_CURRENT_CONFIG	; false) || exit 1
grep -q "\[HKEY_CLASSES_ROOT\]"		HKCR_8_regedit.reg	|| (echo HKCR_8_regedit.reg	missing HKEY_CLASSES_ROOT	; false) || exit 1
grep -q "\[HKEY_CURRENT_USER\]"		HKCU_8_regedit.reg	|| (echo HKCU_8_regedit.reg	missing HKEY_CURRENT_USER	; false) || exit 1
grep -q "\[HKEY_LOCAL_MACHINE\]"	HKLM_8_regedit.reg	|| (echo HKLM_8_regedit.reg	missing HKEY_LOCAL_MACHINE	; false) || exit 1
grep -q "\[HKEY_USERS\]"			HKU_8_regedit.reg	|| (echo HKU_8_regedit.reg	missing HKEY_USERS			; false) || exit 1

exit 0
