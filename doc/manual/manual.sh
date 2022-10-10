#!/bin/bash

# Set the path for the executable to current folder
cd "$(dirname $0)"

# Install netatalk manual pages
mkdir -p /usr/local/share/doc/netatalk
cp ../../build/doc/manual/*.html /usr/local/share/doc/netatalk
