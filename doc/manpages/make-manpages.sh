#!/usr/bin/env bash

# Generate txt files from man page sources
# First run make from the base dir, then this from doc/manpages

set -e

baseDir=$(dirname "$(readlink -f "${0}")")
manDir="$baseDir/../../man/man"

# iterate over all man pages in a numbered man dir
function makeManPages() {
	cd "$manDir$1"
	for file in *."$1"; do
		if [ -f "$file" ]; then
			man -l "$file" > "$baseDir/${man::-2}.txt"
			echo "Processed $file"
		fi
	done
}

makeManPages "1"
makeManPages "3"
makeManPages "4"
makeManPages "5"
makeManPages "8"
