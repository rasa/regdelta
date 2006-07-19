#!/bin/bash

. ./config.sh $*

rm -f HK*_16_regedit.reg HK*_16_regedit.out

for i in $LONG_ROOT_KEYS
do
	b=`basename ${i}`_16_regedit
	f=$b.reg
	set -x
	$TIMEIT regedit /e $f $i 2>&1 | tee $b.out
	set +x
done

set -x

mv -f HKEY_CURRENT_CONFIG_16_regedit.reg	HKCC_16_regedit.reg || exit 1
mv -f HKEY_CLASSES_ROOT_16_regedit.reg		HKCR_16_regedit.reg || exit 1
mv -f HKEY_CURRENT_USER_16_regedit.reg		HKCU_16_regedit.reg || exit 1
mv -f HKEY_LOCAL_MACHINE_16_regedit.reg		HKLM_16_regedit.reg || exit 1
mv -f HKEY_USERS_16_regedit.reg				HKU_16_regedit.reg	|| exit 1

exit 0
