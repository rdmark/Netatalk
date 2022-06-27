# Makefile.am for test/afpd/

pkgconfdir = @PKGCONFDIR@

TESTS = test.sh test

check_PROGRAMS = test
noinst_HEADERS = test.h subtests.h afpfunc_helpers.h
EXTRA_DIST = test.sh
CLEANFILES = test.default test.conf

test_SOURCES =  test.c subtests.c afpfunc_helpers.c \
				$(top_srcdir)/etc/afpd/afp_config.c \
				$(top_srcdir)/etc/afpd/afp_dsi.c \
				$(top_srcdir)/etc/afpd/afp_options.c \
				$(top_srcdir)/etc/afpd/afp_util.c \
				$(top_srcdir)/etc/afpd/afprun.c \
				$(top_srcdir)/etc/afpd/appl.c \
				$(top_srcdir)/etc/afpd/auth.c \
				$(top_srcdir)/etc/afpd/catsearch.c \
				$(top_srcdir)/etc/afpd/desktop.c \
				$(top_srcdir)/etc/afpd/dircache.c \
				$(top_srcdir)/etc/afpd/directory.c \
				$(top_srcdir)/etc/afpd/enumerate.c \
				$(top_srcdir)/etc/afpd/extattrs.c \
				$(top_srcdir)/etc/afpd/fce_api.c \
				$(top_srcdir)/etc/afpd/fce_util.c \
				$(top_srcdir)/etc/afpd/file.c \
				$(top_srcdir)/etc/afpd/filedir.c \
				$(top_srcdir)/etc/afpd/fork.c \
				$(top_srcdir)/etc/afpd/hash.c \
				$(top_srcdir)/etc/afpd/mangle.c \
				$(top_srcdir)/etc/afpd/messages.c \
				$(top_srcdir)/etc/afpd/nfsquota.c \
				$(top_srcdir)/etc/afpd/ofork.c \
				$(top_srcdir)/etc/afpd/quota.c \
				$(top_srcdir)/etc/afpd/status.c \
				$(top_srcdir)/etc/afpd/spotlight_marshalling.c \
				$(top_srcdir)/etc/afpd/switch.c \
				$(top_srcdir)/etc/afpd/uam.c \
				$(top_srcdir)/etc/afpd/unix.c \
				$(top_srcdir)/etc/afpd/volume.c

test_CFLAGS = \
	-I$(top_srcdir)/etc/afpd \
	-I$(top_srcdir)/include \
	-I$(top_srcdir)/sys \
	@GSSAPI_CFLAGS@ @KRB5_CFLAGS@\
	-DAPPLCNAME \
	-DSERVERTEXT=\"$(SERVERTEXT)/\" \
	-D_PATH_AFPDPWFILE=\"$(pkgconfdir)/afppasswd\" \
	-D_PATH_AFPDUAMPATH=\"$(UAMS_PATH)/\" \
	-D_PATH_CONFDIR=\"$(pkgconfdir)/\" \
	-D_PATH_STATEDIR='"$(localstatedir)/netatalk/"'

test_LDADD = \
	$(top_builddir)/libatalk/libatalk.la \
	@LIBGCRYPT_LIBS@ @GSSAPI_LIBS@ @KRB5_LIBS@ @MYSQL_LIBS@

test_LDFLAGS = -export-dynamic
