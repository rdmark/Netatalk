## Makefile for distrib/initscript/

pkgconfdir = @PKGCONFDIR@

# overwrite automake uninstall
# not beautiful, but this way we can call the OS specific init script
# tools, like launchctl

uninstall: uninstall-startup

#
# checking for macOS init script
#

if USE_MACOS_LAUNCHD

bin_SCRIPTS = netatalkd
EXTRA_DIST = netatalkd
launchd_PLIST = com.netatalk.daemon.plist

install-data-hook:
	(if [ ! -f $(INIT_DIR)/$(launchd_PLIST) ] ; then \
		cp $(launchd_PLIST) $(INIT_DIR) && \
		launchctl load -w $(INIT_DIR)/$(launchd_PLIST); \
	fi)

uninstall-startup:
	$(bin_SCRIPTS) stop
	launchctl unload -w $(INIT_DIR)/$(launchd_PLIST)
	rm -f $(DESTDIR)$(bindir)/$(bin_SCRIPTS)
	rm -f $(INIT_DIR)/$(launchd_PLIST)

endif
