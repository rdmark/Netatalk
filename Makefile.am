# Makefile.am for top level of netatalk package

SUBDIRS = include libatalk bin config etc contrib distrib doc man macros test

EXTRA_DIST = CONTRIBUTORS COPYRIGHT COPYING NEWS VERSION

ACLOCAL_AMFLAGS = -I macros
AUTOMAKE_OPTIONS = foreign

if DEVELOPER
abi-clean:
	@echo "Removing ABI file libatalk/libatalk-$(NETATALK_VERSION).abi"
	@rm -f libatalk/libatalk-$(NETATALK_VERSION).abi
endif
