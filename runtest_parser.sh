#!/bin/sh

for testfile in tests/parse/good*.bminor
do
	if ./bminor --parse $testfile &> $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in tests/parse/bad*.bminor
do
	if ./bminor --parse $testfile &> $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done
