#!/bin/bash
# Script for compiling netatalk binaries on macOS

# Set the path for the executable to current folder
cd "$(dirname $0)"

# Global variables
cores=$(getconf _NPROCESSORS_ONLN)
title="Compile Netatalk for macOS"
prompt="Pick an option:"
options=("binaries" "documentation")

# Build and install
build_binaries() {
  git pull
  ./bootstrap
  make -j "$cores"
  sudo make install -j "$cores"
  glibtool --finish /usr/local/lib/netatalk/
  git clean -d -x -f
}

build_documentation() {
  git pull
  ./bootstrap
  make html -j "$cores"
  make install -j "$cores"
  git clean -d -x -f
}

echo "$title"
PS3="$prompt"
select opt in "${options[@]}" "Quit"; do
  case "$REPLY" in
  1)
    build_binaries
    ;;
  2)
    build_documentation
    ;;
  $((${#options[@]} + 1)))
    echo "Goodbye!"
    break
    ;;
  esac
  REPLY=
done

# Exit gracefully
osascript -e 'tell application "Terminal" to close (every window whose name contains ".command")' &
exit
