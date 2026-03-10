#!/bin/sh
SRCFILE="include/rx/regex.hpp"
DSTFILE="single-include/rx/regex.hpp"
TMPFILE=$(mktemp)
quom -I "include/" $SRCFILE $TMPFILE
printf '// THIS FILE IS AUTOMATICALLY AMALGAMATED FROM "%s"\n' $SRCFILE > $DSTFILE
awk '/^\/\/ Copyright \(C\)'/ $TMPFILE | sort | uniq >> $DSTFILE
printf '\n\n' >> $DSTFILE
awk '{print "// " $0}' LICENSE >> $DSTFILE
printf '\n\n#pragma once\n\n' >> $DSTFILE
awk '/^#include <.*+>/' $TMPFILE | sort | uniq >> $DSTFILE
printf '\n' >> $DSTFILE
awk '!/^(#pragma once)$|^(#include <.*+>)$/' $TMPFILE | awk -F'//' '{if ($1 !~ /^\s*$/) print $1; else print "" }' | cat --squeeze-blank >> $DSTFILE
rm $TMPFILE
echo "Successfully generated ${DSTFILE}"
