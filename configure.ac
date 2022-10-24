# configure.ac for netatalk

AC_INIT([netatalk],m4_normalize(m4_include([VERSION])))
AC_CONFIG_SRCDIR([etc/afpd/main.c])

NETATALK_VERSION=`cat $srcdir/VERSION`
AC_SUBST(NETATALK_VERSION)

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIRS([macros])
AC_CONFIG_AUX_DIR([build-aux])

AC_CANONICAL_HOST

AM_INIT_AUTOMAKE([subdir-objects])
AM_MAINTAINER_MODE([enable])

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
LT_INIT([dlopen])
AM_PROG_CC_C_O


# Check if we can use attribute unused (gcc only) from ethereal
AC_MSG_CHECKING(to see if we can add '__attribute__((unused))' to CFLAGS)
if test x$GCC != x ; then
  CFLAGS="-D_U_=\"__attribute__((unused))\" $CFLAGS"
  AC_MSG_RESULT(yes)
else
  CFLAGS="-D_U_=\"\" $CFLAGS"
  AC_MSG_RESULT(no)
fi

# Checks for header files
AC_CHECK_HEADERS(unistd.h termios.h netdb.h sgtty.h statfs.h dlfcn.h langinfo.h locale.h)
AC_CHECK_HEADERS(sys/param.h sys/fcntl.h sys/termios.h sys/statvfs.h sys/stat.h sys/mount.h)

AC_CHECK_TYPES([fshare_t], [], [], [[#include <fcntl.h>]])

AC_SYS_LARGEFILE([], AC_MSG_ERROR([AFP 3.x support requires Large File Support.]))

# Check if dlsym needs to add an underscore, uses libtool macros
LT_LIB_DLLOAD
AC_CHECK_FUNCS(dlopen dlsym dlclose)
LT_FUNC_DLSYM_USCORE
if test x"$libltdl_cv_need_uscore" = xyes; then
    AC_DEFINE(DLSYM_PREPEND_UNDERSCORE, 1, [BSD compatibility macro])
fi

# Special checks
ac_neta_haveatfuncs=yes
AC_CHECK_FUNCS(openat renameat fstatat unlinkat, , ac_neta_haveatfuncs=no)
if test x"$ac_neta_haveatfuncs" = x"yes" ; then
   AC_DEFINE([_ATFILE_SOURCE], 1, AT file source)
   AC_DEFINE([HAVE_ATFUNCS], 1, whether at funcs are available)
fi
AC_CHECK_MEMBERS(struct tm.tm_gmtoff,,, [#include <time.h>])

AC_CHECK_FUNCS(backtrace_symbols dirfd getusershell pread pwrite pselect)
AC_CHECK_FUNCS(setlinebuf strlcat strlcpy strnlen vasprintf asprintf)

# Search for necessary libraries
AC_SEARCH_LIBS(gethostbyname, nsl)
AC_SEARCH_LIBS(connect, socket)
AC_CHECK_FUNCS(getifaddrs) # comes after gethostbyname and connect so it picks up the libs

AC_DEFINE(OPEN_NOFOLLOW_ERRNO, ELOOP, errno returned by open with O_NOFOLLOW)

# Pthread support
AX_PTHREAD(, [AC_MSG_ERROR([missing pthread_sigmask])])

# OS specific checks
AC_NETATALK_OS_SPECIFIC

# Whether to set rpath
AC_NETATALK_SET_RPATH

# --with-pkgconfdir check to change configuration directory location
AC_NETATALK_CONFIG_DIRS

# Check whether to enable debug code
AC_NETATALK_DEBUG

# Check whether to disable tickle SIGALARM stuff, which eases debugging
AC_NETATALK_DEBUGGING

#	Check for libiconv support
AC_NETATALK_CHECK_ICONV

# Check for CNID database backends
AC_NETATALK_CNID([bdb_required=yes],[bdb_required=no])

# Check for optional Zeroconf support
AC_NETATALK_ZEROCONF

# Check for PAM libs
AC_NETATALK_PATH_PAM

# Check for optional initscript install
AC_NETATALK_INIT_STYLE

# Path where UAM modules shall be installed
AC_ARG_WITH(uams-path, [  --with-uams-path=PATH   path to UAMs [[$libdir/netatalk]]], [uams_path="$withval"], [uams_path="$libdir/netatalk"])

# Check for libgcrypt, if found enables DHX2 UAM
AC_NETATALK_PATH_LIBGCRYPT([1:1.2.3])

# Check for openssl, if found enables DHX UAM and Randnum UAM
AC_NETATALK_PATH_SSL

# Check for Berkeley DB library
AC_NETATALK_PATH_BDB

# Check for building Kerberos V UAM module
AC_NETATALK_KRB5_UAM

# Check for Kerberos 5 support
AC_NETATALK_KERBEROS

# Check for overwrite the config files or not
AC_NETATALK_OVERWRITE_CONFIG

# Check for LDAP support, for client-side ACL visibility
AC_NETATALK_LDAP

# Check for Extended Attributes support
AC_NETATALK_EXTENDED_ATTRIBUTES

# Check if realpath() takes NULL
AC_NETATALK_REALPATH

# Check for sendfile()
AC_NETATALK_SENDFILE
AC_NETATALK_RECVFILE

# Check for libevent
AC_NETATALK_LIBEVENT

# Check whether bundled tdb shall be used
AC_NETATALK_TDB

# Check for dbus-glib, for AFP stats on dbus
AC_NETATALK_DBUS_GLIB

# netatalk lockfile path, must come after AC_NETATALK_FHS
AC_NETATALK_LOCKFILE

# Check for Docbook and build documentation if found
AX_CHECK_DOCBOOK

CPPFLAGS="-I\$(top_srcdir)/include -I\$(top_builddir)/include -I\$(top_srcdir) $CPPFLAGS"
UAMS_PATH="${uams_path}"

AC_SUBST(LIBS)
AC_SUBST(CFLAGS)
AC_SUBST(UAMS_PATH)

AM_CONDITIONAL(HAVE_LIBGCRYPT, test x$neta_cv_have_libgcrypt = xyes)
AM_CONDITIONAL(HAVE_OPENSSL, test x$neta_cv_have_openssl = xyes)
AM_CONDITIONAL(HAVE_LDAP, test x"$netatalk_cv_ldap" = x"yes")
AM_CONDITIONAL(USE_DHX, test x$neta_cv_compile_dhx = xyes)
AM_CONDITIONAL(USE_DHX2, test x$neta_cv_compile_dhx2 = xyes)
AM_CONDITIONAL(USE_PAM_SO, test x$use_pam_so = xyes)
AM_CONDITIONAL(USE_PAM, test x$netatalk_cv_install_pam = xyes)
AM_CONDITIONAL(BUILD_PAM, test x$compile_pam = xyes)
AM_CONDITIONAL(DEFAULT_HOOK, test x$neta_cv_have_libgcrypt != xyes && test x$neta_cv_have_openssl != xyes)
AM_CONDITIONAL(USE_BDB, test x$bdb_required = xyes)
AM_CONDITIONAL(HAVE_ATFUNCS, test x"$ac_neta_haveatfuncs" = x"yes")

# Enable silent Automake rules if present
m4_ifdef([AM_SILENT_RULES],[AM_SILENT_RULES([yes])])

# Generate files
AC_CONFIG_FILES([
  Makefile
  bin/Makefile
  bin/ad/Makefile
  bin/cnid/Makefile
  bin/cnid/cnid2_create
  bin/misc/Makefile
  config/Makefile
  contrib/Makefile
  contrib/macusers/Makefile
  contrib/macusers/macusers
  contrib/shell_utils/Makefile
  contrib/shell_utils/apple_dump
  contrib/shell_utils/asip-status.pl
  distrib/Makefile
  distrib/config/Makefile
  distrib/config/netatalk-config
  distrib/initscript/Makefile
  distrib/m4/Makefile
  doc/Makefile
  doc/html.xsl
  doc/man.xsl
  doc/manual/Makefile
  doc/manual/manual.xml
  doc/manpages/Makefile
  doc/manpages/man1/Makefile
  doc/manpages/man5/Makefile
  doc/manpages/man8/Makefile
  etc/Makefile
  etc/afpd/Makefile
  etc/cnid_dbd/Makefile
  etc/netatalk/Makefile
  etc/uams/Makefile
  include/Makefile
  include/atalk/Makefile
  libatalk/Makefile
  libatalk/acl/Makefile
  libatalk/adouble/Makefile
  libatalk/bstring/Makefile
  libatalk/cnid/Makefile
  libatalk/cnid/last/Makefile
  libatalk/cnid/dbd/Makefile
  libatalk/cnid/tdb/Makefile
  libatalk/cnid/mysql/Makefile
  libatalk/dsi/Makefile
  libatalk/iniparser/Makefile
  libatalk/tdb/Makefile
  libatalk/unicode/Makefile
  libatalk/unicode/charsets/Makefile
  libatalk/util/Makefile
  libatalk/vfs/Makefile
  macros/Makefile
  man/Makefile
  man/man1/Makefile
  man/man1/ad.1
  man/man1/afpldaptest.1
  man/man1/afppasswd.1
  man/man1/afpstats.1
  man/man1/apple_dump.1
  man/man1/asip-status.pl.1
  man/man1/dbd.1
  man/man1/macusers.1
  man/man1/megatron.1
  man/man1/netatalk-config.1
  man/man1/uniconv.1
  man/man5/Makefile
  man/man5/afp.conf.5
  man/man5/afp_signature.conf.5
  man/man5/afp_voluuid.conf.5
  man/man5/extmap.conf.5
  man/man8/Makefile
  man/man8/afpd.8
  man/man8/cnid_dbd.8
  man/man8/cnid_metad.8
  man/man8/netatalk.8
  test/Makefile
  test/afpd/Makefile
])

AC_CONFIG_COMMANDS([default],
  [chmod a+x distrib/config/netatalk-config contrib/shell_utils/apple_*],
  []
)

AC_OUTPUT

AC_NETATALK_LIBS_SUMMARY
AC_NETATALK_CONFIG_SUMMARY
