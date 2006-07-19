#!/bin/bash

. ./config.sh

./exportd8.sh || exit

./exportr8.sh || exit

./diff8.sh || exit

./diff8b.sh || exit
