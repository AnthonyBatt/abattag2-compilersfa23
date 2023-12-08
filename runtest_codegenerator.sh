#!/bin/sh

for testfile in tests/codegen/good*.bminor
do
	if ./bminor --codegen $testfile $testfile.s &> $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done
