## Makefile for distrib/initscripts/

SUFFIXES = .tmpl .

pkgconfdir = @PKGCONFDIR@

#
# Template Generation
#

.tmpl:
	sed -e s@:BINDIR:@${bindir}@ \
	    -e s@:SBINDIR:@${sbindir}@ \
	    -e s@:ETCDIR:@${pkgconfdir}@ \
	    -e s@:PKGLIBEXECDIR:@${pkglibexecdir}@ \
	    -e s@:VERSION:@${VERSION}@ \
	    <$< >$@

GENERATED_FILES = \
	rc.a2boot.netbsd	\
	rc.afpd.netbsd		\
	rc.atalkd.netbsd	\
	rc.cnid_metad.netbsd	\
	rc.papd.netbsd		\
	rc.timelord.netbsd	\
	rc.atalk.sysv		\
	rc.atalk.gentoo		\
	a2boot.service		\
	afpd.service		\
	atalkd.service		\
	cnid.service		\
	papd.service		\
	timelord.service

TEMPLATES = \
	rc.a2boot.netbsd.tmpl		\
	rc.afpd.netbsd.tmpl		\
	rc.atalkd.netbsd.tmpl		\
	rc.cnid_metad.netbsd.tmpl	\
	rc.papd.netbsd.tmpl		\
	rc.timelord.netbsd.tmpl		\
	rc.atalk.sysv.tmpl		\
	rc.atalk.gentoo.tmpl		\
	a2boot.service.tmpl		\
	afpd.service.tmpl		\
	atalkd.service.tmpl		\
	cnid.service.tmpl		\
	papd.service.tmpl		\
	timelord.service.tmpl

CLEANFILES = $(GENERATED_FILES) $(sysv_SCRIPTS) $(service_DATA) a2boot afpd atalkd cnid_metad papd timelord
EXTRA_DIST = $(TEMPLATES)

# overwrite automake uninstall
# not beautiful, but this way we can call the OS specific init script
# tools, like chkconfig, insserv or rc-update

uninstall: uninstall-startup

#
# checking for systemd scripts:
#

if USE_SYSTEMD

servicedir = /lib/systemd/system

service_DATA = a2boot.service afpd.service atalkd.service cnid.service papd.service timelord.service

install-data-hook:
	-systemctl daemon-reload
if SYSTEMD_START
	-systemctl enable $(service_DATA)
	-systemctl start $(service_DATA)
endif

uninstall-startup:
	-systemctl disable $(service_DATA)
	rm -f $(addprefix $(DESTDIR)$(servicedir)/, $(service_DATA))
	-systemctl daemon-reload

endif

#
# checking for NetBSD init scripts
#

if USE_NETBSD

sysvdir = /etc/rc.d
sysv_SCRIPTS = afpd atalkd cnid_metad papd timelord a2boot

afpd: rc.afpd.netbsd
	cp -f $? $@
	chmod a+x $@

atalkd: rc.atalkd.netbsd
	cp -f $? $@
	chmod a+x $@

cnid_metad: rc.cnid_metad.netbsd
	cp -f $? $@
	chmod a+x $@

papd: rc.papd.netbsd
	cp -f $? $@
	chmod a+x $@

timelord: rc.timelord.netbsd
	cp -f $? $@
	chmod a+x $@
	
a2boot: rc.a2boot.netbsd
	cp -f $? $@
	chmod a+x $@

install-data-hook:

uninstall-hook:

uninstall-startup: uninstall-am

endif

#
# checking for Solaris init scripts
#

if USE_SOLARIS

sysvdir = /etc/init.d
sysv_SCRIPTS = netatalk

$(sysv_SCRIPTS): rc.atalk.sysv
	cp -f rc.atalk.sysv $@
	chmod a+x $@

install-data-hook:
	rm -f $(DESTDIR)/etc/rc2.d/S90$(sysv_SCRIPTS)
	-ln -s ../init.d/$(sysv_SCRIPTS) $(DESTDIR)/etc/rc2.d/S90$(sysv_SCRIPTS)
	rm -f $(DESTDIR)/etc/rc0.d/K04$(sysv_SCRIPTS)
	-ln -s ../init.d/$(sysv_SCRIPTS) $(DESTDIR)/etc/rc0.d/K04$(sysv_SCRIPTS)

uninstall-startup:
	rm -f $(DESTDIR)$(sysvdir)/$(sysv_SCRIPTS) \
		$(DESTDIR)/etc/rc2.d/S90$(sysv_SCRIPTS) \
		$(DESTDIR)/etc/rc0.d/K04$(sysv_SCRIPTS)

endif

#
# checking for "Gentoo" style sysv scripts:
#

if USE_GENTOO

sysvdir = /etc/init.d
sysv_SCRIPTS = netatalk

$(sysv_SCRIPTS): rc.atalk.gentoo
	cp -f rc.atalk.gentoo $(sysv_SCRIPTS)
	chmod a+x $(sysv_SCRIPTS)

install-data-hook:
#	-rc-update add $(sysv_SCRIPTS) default

uninstall-startup:
#	-rc-update del $(sysv_SCRIPTS) default
#	rm -f $(DESTDIR)$(sysvdir)/$(sysv_SCRIPTS)

endif

#
# defaults, no init scripts installed
#

if USE_UNDEF

install-data-hook:

uninstall-hook:

uninstall-startup: uninstall-am

endif

