#!/bin/bash

. ./config.sh $*

rm -f HK*_$1.reg rm HK*_$1.out

for i in $ROOT_KEYS
do
	b=`basename ${i}`_$1
	f=$b.reg
	set -x
	$TIMEIT $DIR/regdelta $EXPORT_OPTS $i $f 2>&1 | tee $b.out
	set +x
done

exit 0
