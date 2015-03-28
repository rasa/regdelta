#!/usr/bin/env bash

. ./config.sh

set -x

rm ?out.reg

for d in 1 2 3 4 5 6 7 8
do
	$DIR/regdelta -cro ${d}out.reg ${d}a.reg ${d}b.reg || exit
	diff -uw ${d}c.reg ${d}out.reg || exit
done

exit 0
