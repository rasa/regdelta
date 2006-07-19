#!/bin/bash

. ./config.sh $*

rm -f HK*_8*.diff

for i in $ROOT_KEYS
do
	b1=${i}_8
	b2=${b1}_regedit
	f1=$b1.reg
	f2=$b2.reg
	
	test -f $f1 || (echo File not found: $f1 ; false) || exit 1
	test -f $f2 || (echo File not found: $f2 ; false) || exit 1

	set -x
	diff -uw $f1 $f2 >$b1.diff
	diff -W 160 -wy $f1 $f2 | egrep "[[:space:]]\|[[:space:]]" >${b1}_2.diff
	set +x
done

ls -ltSr HK*_8*.diff

exit 0
