#!/bin/bash

# Set the path for the executable to current folder
cd "$(dirname $0)"

# Install netatalk launch daemon
if [ ! -f /Library/LaunchDaemons/com.netatalk.daemon.plist]; then
  cp com.netatalk.daemon.plist /Library/LaunchDaemons && \
  launchctl load -w /Library/LaunchDaemons/com.netatalk.daemon.plist
fi
