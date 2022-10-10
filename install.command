#!/bin/bash
# Script for compiling netatalk binaries on macOS

# Set the path for the executable to current folder
cd "$(dirname $0)"

# Clean repo
git clean -dxf

# Update repo
git pull

#Build & install
meson build
ninja -C build
sudo ninja -C build install

# Exit gracefully
osascript -e 'tell application "Terminal" to close (every window whose name contains ".command")' &
exit
