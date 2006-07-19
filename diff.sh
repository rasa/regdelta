#!/bin/bash

. ./config.sh $*

rm -f HK*_${1}_*diff*.reg HK*_${1}_diff*.out

for i in $ROOT_KEYS
do
	b1=${i}_$1
	b2=${b1}_regedit
	f1=$b1.reg
	f2=$b2.reg
	
	test -f $f1 || (echo File not found: $f1 ; false) || exit 1
	test -f $f2 || (echo File not found: $f2 ; false) || exit 1
	
	d1=${b1}_diff.reg
	d2=${b1}_diff2.reg
	
	set -x
	$TIMEIT $DIR/regdelta -c -o $d1 $f1 $f2 2>&1 | tee ${b1}_diff.out
	test -s $d1 &&
		$TIMEIT $DIR/regdelta -c -o $d2 $f2 $f1 2>&1 | tee ${b1}_diff2.out
	set +x
done

exit 0

