#!/usr/bin/env bash

. ./config.sh

./init.sh

./test8.sh

./test16.sh

./winmerge8.sh || exit

./winmerge16.sh || exit
