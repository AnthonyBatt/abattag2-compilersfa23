#!/bin/sh

for testfile in tests/resolve/good*.bminor
do
	if ./bminor --resolve $testfile &> $testfile.out
	then
		echo "$testfile success (as expected)"
	else
		echo "$testfile failure (INCORRECT)"
	fi
done

for testfile in tests/resolve/bad*.bminor
do
	if ./bminor --resolve $testfile &> $testfile.out
	then
		echo "$testfile success (INCORRECT)"
	else
		echo "$testfile failure (as expected)"
	fi
done
