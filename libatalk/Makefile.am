# Makefile.am for libatalk/

SUBDIRS = acl adouble bstring compat cnid dsi iniparser talloc util unicode vfs

lib_LTLIBRARIES = libatalk.la

libatalk_la_SOURCES =

libatalk_la_CFLAGS = \
	@PTHREAD_CFLAGS@

libatalk_la_LIBADD  = \
	@PTHREAD_LIBS@ @MYSQL_LIBS@ \
	acl/libacl.la \
	adouble/libadouble.la \
	bstring/libbstring.la \
	cnid/libcnid.la \
	dsi/libdsi.la \
	iniparser/libiniparser.la \
	unicode/libunicode.la \
	util/libutil.la \
	vfs/libvfs.la

libatalk_la_DEPENDENCIES = \
	acl/libacl.la \
	adouble/libadouble.la \
	bstring/libbstring.la \
	cnid/libcnid.la \
	dsi/libdsi.la \
	iniparser/libiniparser.la \
	unicode/libunicode.la \
	util/libutil.la \
	vfs/libvfs.la

if USE_BUILTIN_TDB
SUBDIRS += tdb
libatalk_la_LIBADD += tdb/libtdb.la
libatalk_la_DEPENDENCIES += tdb/libtdb.la
endif
