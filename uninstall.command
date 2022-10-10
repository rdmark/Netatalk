#!/bin/bash
# Script for uninstalling netatalk binaries on macOS

# Set the path for the executable to current folder
cd "$(dirname $0)"

# Unload launch daemon and uninstall
sudo launchctl unload -w /Library/LaunchDaemons/com.netatalk.daemon.plist
sudo ninja -C build uninstall

# Exit gracefully
osascript -e 'tell application "Terminal" to close (every window whose name contains ".command")' &
exit
