#!/bin/sh

TEST_NAME=`head -1 $1`
TESTS=`tail -n +2 $1`
echo python test/stf.py $TEST_NAME $TESTS