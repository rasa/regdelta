#!/usr/bin/env bash

. ./config.sh

./exportd16.sh || exit

./exportr16.sh || exit

./diff16.sh || exit
