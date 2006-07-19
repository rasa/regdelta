#!/bin/bash

. ./config.sh $*

rm -f HK*_$1.cmp

for i in $ROOT_KEYS
do
	b1=${i}_$1
	b2=${b1}_regedit
	f1=$b1.reg
	f2=$b2.reg
	
	test -f $f1 || (echo File not found: $f1 ; false) || exit 1
	test -f $f2 || (echo File not found: $f2 ; false) || exit 1

	set -x
	cmp -b $f1 $f2 | tee $b1.cmp
	test -s $b1.cmp && "$WINMERGE" $f1 $f2
	set +x
done

exit 0
