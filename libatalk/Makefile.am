
# Makefile.am for libatalk/

SUBDIRS = acl adouble asp atp bstring compat cnid dsi nbp netddp util unicode vfs

lib_LTLIBRARIES = libatalk.la

libatalk_la_SOURCES = dummy.c

libatalk_la_LIBADD  = \
	asp/libasp.la \
	acl/libacl.la \
	atp/libatp.la \
	adouble/libadouble.la	\
	bstring/libbstring.la \
	compat/libcompat.la	\
	dsi/libdsi.la		\
	nbp/libnbp.la		\
	netddp/libnetddp.la \
	util/libutil.la		\
	unicode/libunicode.la \
	vfs/libvfs.la

libatalk_la_DEPENDENCIES = \
	asp/libasp.la \
	acl/libacl.la \
	atp/libatp.la \
	adouble/libadouble.la	\
	bstring/libbstring.la \
	compat/libcompat.la	\
	dsi/libdsi.la		\
	nbp/libnbp.la		\
	netddp/libnetddp.la \
	util/libutil.la		\
	unicode/libunicode.la \
	vfs/libvfs.la

libatalk_la_LDFLAGS = -static
