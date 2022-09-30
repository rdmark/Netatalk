# Kitchen sink for configuration macros.

# AX_CHECK_DOCBOOK
# ----------------
# Check for docbook.

AC_DEFUN([AX_CHECK_DOCBOOK], [
  # It's just rude to go over the net to build
  XSLTPROC_FLAGS=--nonet
  DOCBOOK_ROOT=$(brew --prefix docbook-xsl)/docbook-xsl
  XSLTPROC_WORKS=no

  if test -n "$DOCBOOK_ROOT" ; then
    AC_CHECK_PROG(XSLTPROC,xsltproc,xsltproc,)
    if test -n "$XSLTPROC"; then
      AC_MSG_CHECKING([whether xsltproc works])
      DB_FILE="$DOCBOOK_ROOT/html/docbook.xsl"
      $XSLTPROC $XSLTPROC_FLAGS $DB_FILE >/dev/null 2>&1 << END
<?xml version="1.0" encoding='ISO-8859-1'?>
<!DOCTYPE book PUBLIC "-//OASIS//DTD DocBook XML V4.1.2//EN" "http://www.oasis-open.org/docbook/xml/4.1.2/docbookx.dtd">
<book id="test">
</book>
END
      if test "$?" = 0; then
        XSLTPROC_WORKS=yes
      fi
      AC_MSG_RESULT($XSLTPROC_WORKS)
    fi
  fi

  AC_MSG_CHECKING([whether to build Docbook documentation])
  AC_MSG_RESULT($XSLTPROC_WORKS)

  AM_CONDITIONAL(HAVE_XSLTPROC, test x"$XSLTPROC_WORKS" = x"yes")
  AC_SUBST(XSLTPROC_FLAGS)
  AC_SUBST(DOCBOOK_ROOT)
  AC_SUBST(XSLTPROC)
])

# AC_NETATALK_DBUS_GLIB
# ---------------------
# Check for dbus-glib, for AFP stats.

AC_DEFUN([AC_NETATALK_DBUS_GLIB], [
  atalk_cv_with_dbus=no

  PKG_CHECK_MODULES(DBUS, dbus-1 >= 1.1, have_dbus=yes, have_dbus=no)
  PKG_CHECK_MODULES(DBUS_GLIB, dbus-glib-1, have_dbus_glib=yes, have_dbus_glib=no)
  PKG_CHECK_MODULES(DBUS_GTHREAD, gthread-2.0, have_dbus_gthread=yes, have_dbus_gthread=no)
  if test x$have_dbus_glib = xyes -a x$have_dbus = xyes -a x$have_dbus_gthread = xyes ; then
    saved_CFLAGS=$CFLAGS
    saved_LIBS=$LIBS
    CFLAGS="$CFLAGS $DBUS_GLIB_CFLAGS"
    LIBS="$LIBS $DBUS_GLIB_LIBS"
    AC_CHECK_FUNC([dbus_g_bus_get_private], [atalk_cv_with_dbus=yes], [atalk_cv_with_dbus=no])
    CFLAGS="$saved_CFLAGS"
    LIBS="$saved_LIBS"
  fi

  if test x"$withval" = x"yes" -a x"$atalk_cv_with_dbus" = x"no"; then
    AC_MSG_ERROR([afpstats requested but dbus-glib not found])
  fi

  ac_cv_dbus_sysdir=$(brew --prefix)/etc/dbus-1/system.d

  DBUS_SYS_DIR=""
  if test x$atalk_cv_with_dbus = xyes ; then
      AC_DEFINE(HAVE_DBUS_GLIB, 1, [Define if support for dbus-glib was found])
      DBUS_SYS_DIR="$ac_cv_dbus_sysdir"
  fi

  AC_SUBST(DBUS_SYS_DIR)
  AC_SUBST(DBUS_CFLAGS)
  AC_SUBST(DBUS_LIBS)
  AC_SUBST(DBUS_GLIB_CFLAGS)
  AC_SUBST(DBUS_GLIB_LIBS)
  AC_SUBST(DBUS_GTHREAD_CFLAGS)
  AC_SUBST(DBUS_GTHREAD_LIBS)
  AM_CONDITIONAL(HAVE_DBUS_GLIB, test x$atalk_cv_with_dbus = xyes)
])

# AC_DEVELOPER
# ------------
# Whether to enable developer build.

AC_DEFUN([AC_DEVELOPER], [
    AC_MSG_CHECKING([whether to enable developer build])
    AC_ARG_ENABLE(
        developer,
        AS_HELP_STRING([--enable-developer], [whether to enable developer build (ABI checking)]),
        enable_dev=$enableval,
        enable_dev=no
    )
    AC_MSG_RESULT([$enable_dev])
    AM_CONDITIONAL(DEVELOPER, test x"$enable_dev" = x"yes")
])

# AC_NETATALK_LIBEVENT
# --------------------
# Check for libevent.

AC_DEFUN([AC_NETATALK_LIBEVENT], [
    PKG_CHECK_MODULES(LIBEVENT, libevent, , [AC_MSG_ERROR([couldn't find libevent with pkg-config])])
    AC_SUBST(LIBEVENT_CFLAGS)
    AC_SUBST(LIBEVENT_LDFLAGS)
])

# AC_NETATALK_TDB
# --------------------
# Whether to disable bundled tdb.

AC_DEFUN([AC_NETATALK_TDB], [
    AC_ARG_WITH(
        tdb,
        [AS_HELP_STRING([--with-tdb],[whether to use the bundled tdb (default: yes)])],
        use_bundled_tdb=$withval,
        use_bundled_tdb=yes
    )
    AC_MSG_CHECKING([whether to use bundled tdb])
    AC_MSG_RESULT([$use_bundled_tdb])

    if test x"$use_bundled_tdb" = x"yes" ; then
        AC_DEFINE(USE_BUILTIN_TDB, 1, [Use internal tbd])
    else
        if test -z "$TDB_LIBS" ; then
            PKG_CHECK_MODULES(TDB, tdb, , [AC_MSG_ERROR([couldn't find tdb with pkg-config])])
        fi
        use_bundled_tdb=no
    fi

    AC_SUBST(TDB_CFLAGS)
    AC_SUBST(TDB_LIBS)
    AM_CONDITIONAL(USE_BUILTIN_TDB, test x"$use_bundled_tdb" = x"yes")
])

# AC_NETATALK_LOCKFILE
# --------------------
# Netatalk lockfile path.

AC_DEFUN([AC_NETATALK_LOCKFILE], [
    AC_MSG_CHECKING([netatalk lockfile path])
    AC_ARG_WITH(
        lockfile,
        [AS_HELP_STRING([--with-lockfile=PATH],[Path of netatalk lockfile])],
        ac_cv_netatalk_lock=$withval,
        ac_cv_netatalk_lock=""
    )
    if test -z "$ac_cv_netatalk_lock" ; then
        ac_cv_netatalk_lock=/var/run/netatalk.pid
    fi
    AC_DEFINE_UNQUOTED(PATH_NETATALK_LOCK, ["$ac_cv_netatalk_lock"], [netatalk lockfile path])
    AC_SUBST(PATH_NETATALK_LOCK, ["$ac_cv_netatalk_lock"])
    AC_MSG_RESULT([$ac_cv_netatalk_lock])
])

# AC_NETATALK_DEBUG
# -----------------
# Check whether to enable debug code.

AC_DEFUN([AC_NETATALK_DEBUG], [
AC_MSG_CHECKING([whether to enable verbose debug code])
AC_ARG_ENABLE(debug,
	[  --enable-debug          enable verbose debug code],[
	if test "$enableval" != "no"; then
		if test "$enableval" = "yes"; then
			AC_DEFINE(DEBUG, 1, [Define if verbose debugging information should be included])
		else
			AC_DEFINE_UNQUOTED(DEBUG, $enableval, [Define if verbose debugging information should be included])
		fi
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
        AC_DEFINE(NDEBUG, 1, [Disable assertions])
	fi
	],[
		AC_MSG_RESULT([no])
        AC_DEFINE(NDEBUG, 1, [Disable assertions])
	]
)
])

# AC_NETATALK_DEBUGGING
# ---------------------
# Check whether to disable tickle SIGALARM stuff, which eases debugging.

AC_DEFUN([AC_NETATALK_DEBUGGING], [
AC_MSG_CHECKING([whether to enable debugging with debuggers])
AC_ARG_ENABLE(debugging,
	[  --enable-debugging      disable SIGALRM timers and DSI tickles (eg for debugging with gdb/dbx/...)],[
	if test "$enableval" != "no"; then
		if test "$enableval" = "yes"; then
			AC_DEFINE(DEBUGGING, 1, [Define if you want to disable SIGALRM timers and DSI tickles])
		else
			AC_DEFINE_UNQUOTED(DEBUGGING, $enableval, [Define if you want to disable SIGALRM timers and DSI tickles])
		fi
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi
	],[
		AC_MSG_RESULT([no])
	]
)

])

# AC_NETATALK_INIT_STYLE
# ----------------------
# Check for initscript install.

AC_DEFUN([AC_NETATALK_INIT_STYLE], [
    init_style=macos-launchd
    case "$init_style" in
    "macos-launchd")
	    AC_MSG_RESULT([enabling macOS-style launchd initscript support])
	    ac_cv_init_dir="/Library/LaunchDaemons"
	    ;;
    esac
    AM_CONDITIONAL(USE_MACOS_LAUNCHD, test x$init_style = xmacos-launchd)
    AM_CONDITIONAL(USE_UNDEF, test x$init_style = xnone)
    ac_cv_init_dir="$ac_cv_init_dir"
    INIT_DIR="$ac_cv_init_dir"
    AC_SUBST(INIT_DIR, ["$ac_cv_init_dir"])
])

# AC_NETATALK_OS_SPECIFIC
# -----------------------
# OS specific configuration.

AC_DEFUN([AC_NETATALK_OS_SPECIFIC], [
case "$host_os" in
	*darwin*)			this_os=macosx ;;
esac
])

# AC_NETATALK_SET_RPATH
# ---------------------
# Check whether to enable rpath.

AC_DEFUN([AC_NETATALK_SET_RPATH], [
	AC_ARG_ENABLE(rpath,
		AS_HELP_STRING([--enable-rpath],
			[enable RPATH/RUNPATH (default: $default_rpath)]),
		AS_CASE("$enableval",
			[yes], [enable_rpath=yes],
			[no], [enable_rpath=no],
			[AC_MSG_ERROR([bad value $enableval for --enable-rpath])]),
		[enable_rpath=$default_rpath])
	AC_MSG_RESULT([$enable_rpath])
])

# AC_NETATALK_KRB5_UAM
# --------------------
# Check for building Kerberos V UAM module

AC_DEFUN([AC_NETATALK_KRB5_UAM], [
netatalk_cv_build_krb5_uam=no
AC_ARG_ENABLE(krbV-uam,
	[  --enable-krbV-uam       enable build of Kerberos V UAM module],
	[
		if test x"$enableval" = x"yes"; then
			NETATALK_GSSAPI_CHECK([
				netatalk_cv_build_krb5_uam=yes
			],[
				AC_MSG_ERROR([need GSSAPI to build Kerberos V UAM])
			])
		fi
	]

)

AC_MSG_CHECKING([whether Kerberos V UAM should be built])
if test x"$netatalk_cv_build_krb5_uam" = x"yes"; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi
AM_CONDITIONAL(USE_GSSAPI, test x"$netatalk_cv_build_krb5_uam" = x"yes")
])

# AC_NETATALK_KERBEROS
# --------------------
# Check if we can directly use Kerberos 5 API, used for reading keytabs
# and automatically construction DirectoryService names from that, instead
# of requiring special configuration in afp.conf

AC_DEFUN([AC_NETATALK_KERBEROS], [
AC_MSG_CHECKING([for Kerberos 5 (necessary for GetSrvrInfo:DirectoryNames support)])
AC_ARG_WITH([kerberos],
    [AS_HELP_STRING([--with-kerberos], [Kerberos 5 support (default=auto)])],
    [],
    [with_kerberos=auto])
AC_MSG_RESULT($with_kerberos)

if test x"$with_kerberos" != x"no"; then
   have_krb5_header="no"
   AC_CHECK_HEADERS([krb5/krb5.h krb5.h kerberosv5/krb5.h], [have_krb5_header="yes"; break])
   if test x"$have_krb5_header" = x"no" && test x"$with_kerberos" != x"auto"; then
      AC_MSG_FAILURE([--with-kerberos was given, but no headers found])
   fi

   AC_PATH_PROG([KRB5_CONFIG], [krb5-config])
   AC_MSG_CHECKING([for krb5-config])
   if test -x "$KRB5_CONFIG"; then
      AC_MSG_RESULT([$KRB5_CONFIG])
      KRB5_CFLAGS="`$KRB5_CONFIG --cflags krb5`"
      KRB5_LIBS="`$KRB5_CONFIG --libs krb5`"
      AC_SUBST(KRB5_CFLAGS)
      AC_SUBST(KRB5_LIBS)
      with_kerberos="yes"
   else
      AC_MSG_RESULT([not found])
      if test x"$with_kerberos" != x"auto"; then
         AC_MSG_FAILURE([--with-kerberos was given, but krb5-config could not be found])
      fi
   fi
fi

if test x"$with_kerberos" = x"yes"; then
   AC_DEFINE([HAVE_KERBEROS], [1], [Define if Kerberos 5 is available])
fi

# Check for krb5_free_unparsed_name and krb5_free_error_message
save_CFLAGS="$CFLAGS"
save_LIBS="$LIBS"
CFLAGS="$KRB5_CFLAGS"
LIBS="$KRB5_LIBS"
AC_CHECK_FUNCS([krb5_free_unparsed_name krb5_free_error_message krb5_free_keytab_entry_contents krb5_kt_free_entry])
CFLAGS="$save_CFLAGS"
LIBS="$save_LIBS"
])

# AC_NETATALK_OVERWRITE_CONFIG
# ----------------------------
# Check whether to overwrite the config files or not.

AC_DEFUN([AC_NETATALK_OVERWRITE_CONFIG], [
AC_MSG_CHECKING([whether configuration files should be overwritten])
AC_ARG_ENABLE(overwrite,
	[  --enable-overwrite      overwrite configuration files during installation],
	[OVERWRITE_CONFIG="${enable_overwrite}"],
	[OVERWRITE_CONFIG="no"]
)
AC_MSG_RESULT([$OVERWRITE_CONFIG])
AC_SUBST(OVERWRITE_CONFIG)
])

# AC_NETATALK_LDAP
# ----------------
# Check for LDAP support, for client-side ACL visibility.

AC_DEFUN([AC_NETATALK_LDAP], [
AC_MSG_CHECKING(for LDAP (necessary for client-side ACL visibility))
AC_ARG_WITH(ldap,
    [AS_HELP_STRING([--with-ldap[[=PATH]]],
        [LDAP support (default=auto)])],
        netatalk_cv_ldap=$withval,
        netatalk_cv_ldap=auto
        )
AC_MSG_RESULT($netatalk_cv_ldap)

save_CFLAGS="$CFLAGS"
save_LDFLAGS="$LDFLAGS"
save_LIBS="$LIBS"
CFLAGS=""
LDFLAGS=""
LIBS=""
LDAP_CFLAGS=""
LDAP_LDFLAGS=""
LDAP_LIBS=""

if test x"$netatalk_cv_ldap" != x"no" ; then
   if test x"$netatalk_cv_ldap" != x"yes" -a x"$netatalk_cv_ldap" != x"auto"; then
       CFLAGS="-I$netatalk_cv_ldap/include"
       LDFLAGS="-L$netatalk_cv_ldap/lib"
   fi
   	AC_CHECK_HEADER([ldap.h], netatalk_cv_ldap=yes,
        [ if test x"$netatalk_cv_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP headers])
        fi
		netatalk_cv_ldap=no
        ])
	AC_CHECK_LIB(ldap, ldap_init, netatalk_cv_ldap=yes,
        [ if test x"$netatalk_cv_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP library])
        fi
		netatalk_cv_ldap=no
        ])
fi

if test x"$netatalk_cv_ldap" = x"yes"; then
    LDAP_CFLAGS="$CFLAGS"
    LDAP_LDFLAGS="$LDFLAGS"
    LDAP_LIBS="-lldap"
	AC_DEFINE(HAVE_LDAP,1,[Whether LDAP is available])
fi

AC_SUBST(LDAP_CFLAGS)
AC_SUBST(LDAP_LDFLAGS)
AC_SUBST(LDAP_LIBS)
CFLAGS="$save_CFLAGS"
LDFLAGS="$save_LDFLAGS"
LIBS="$save_LIBS"
])

# AC_NETATALK_EXTENDED_ATTRIBUTES
# -------------------------------
# Check for Extended Attributes support.

AC_DEFUN([AC_NETATALK_EXTENDED_ATTRIBUTES], [
neta_cv_eas="ad"
neta_cv_eas_sys_found=no
neta_cv_eas_sys_not_found=no

AC_CHECK_HEADERS(sys/xattr.h sys/uio.h)

if test "x$neta_cv_eas_sys_found" != "xyes" ; then
  AC_CHECK_FUNCS([getxattr fgetxattr listxattr],
                 [neta_cv_eas_sys_found=yes],
                 [neta_cv_eas_sys_not_found=yes])
  AC_CHECK_FUNCS([flistxattr removexattr fremovexattr],,
                 [neta_cv_eas_sys_not_found=yes])
  AC_CHECK_FUNCS([setxattr fsetxattr],,
                 [neta_cv_eas_sys_not_found=yes])
fi

# Do xattr functions take additional options like on Darwin?
if test x"$ac_cv_func_getxattr" = x"yes" ; then
	AC_CACHE_CHECK([whether xattr interface takes additional options], smb_attr_cv_xattr_add_opt, [
		old_LIBS=$LIBS
		LIBS="$LIBS"
		AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
			#include <sys/types.h>
			#include <sys/xattr.h>
		]], [[
			getxattr(0, 0, 0, 0, 0, 0);
		]])],[smb_attr_cv_xattr_add_opt=yes],[smb_attr_cv_xattr_add_opt=no;LIBS=$old_LIBS])
	])
	if test x"$smb_attr_cv_xattr_add_opt" = x"yes"; then
		AC_DEFINE(XATTR_ADD_OPT, 1, [xattr functions have additional options])
	fi
fi

if test "x$neta_cv_eas_sys_found" = "xyes" ; then
   if test "x$neta_cv_eas_sys_not_found" != "xyes" ; then
      neta_cv_eas="$neta_cv_eas | sys"
   fi
fi
AC_DEFINE_UNQUOTED(EA_MODULES,["$neta_cv_eas"],[Available Extended Attributes modules])
])

# AC_NETATALK_SENDFILE
# --------------------
# Check for sendfile().

AC_DEFUN([AC_NETATALK_SENDFILE], [
netatalk_cv_search_sendfile=yes
AC_ARG_ENABLE(sendfile,
    [  --disable-sendfile       disable sendfile syscall],
    [if test x"$enableval" = x"no"; then
            netatalk_cv_search_sendfile=no
        fi]
)

if test x"$netatalk_cv_search_sendfile" = x"yes"; then
   case "$host_os" in
   *linux*)
        AC_DEFINE(SENDFILE_FLAVOR_LINUX,1,[Whether linux sendfile() API is available])
        AC_CHECK_FUNC([sendfile], [netatalk_cv_HAVE_SENDFILE=yes])
        ;;

    *solaris*)
        AC_DEFINE(SENDFILE_FLAVOR_SOLARIS, 1, [Solaris sendfile()])
        AC_SEARCH_LIBS(sendfile, sendfile)
        AC_CHECK_FUNC([sendfile], [netatalk_cv_HAVE_SENDFILE=yes])
        AC_CHECK_FUNCS([sendfilev])
        ;;

    *freebsd*)
        AC_DEFINE(SENDFILE_FLAVOR_BSD, 1, [Define if the sendfile() function uses BSD semantics])
        AC_CHECK_FUNC([sendfile], [netatalk_cv_HAVE_SENDFILE=yes])
        ;;

    *)
        ;;

    esac

    if test x"$netatalk_cv_HAVE_SENDFILE" = x"yes"; then
        AC_DEFINE(WITH_SENDFILE,1,[Whether sendfile() should be used])
    fi
fi
])

# AC_NETATALK_RECVFILE
# --------------------
# Check for recvfile().

AC_DEFUN([AC_NETATALK_RECVFILE], [
case "$host_os" in
*linux*)
    AC_CHECK_FUNCS([splice], [atalk_cv_use_recvfile=yes])
    ;;

*)
    ;;

esac

if test x"$atalk_cv_use_recvfile" = x"yes"; then
    AC_DEFINE(WITH_RECVFILE, 1, [Whether recvfile should be used])
fi
])

# AC_NETATALK_REALPATH
# --------------------
# Check if realpath() takes NULL.

AC_DEFUN([AC_NETATALK_REALPATH], [
AC_CACHE_CHECK([if the realpath function allows a NULL argument],
    neta_cv_REALPATH_TAKES_NULL, [
        AC_RUN_IFELSE([AC_LANG_SOURCE([[
	    #include <stdlib.h>
            #include <stdio.h>
            #include <limits.h>
            #include <signal.h>

            void exit_on_core(int ignored) {
                 exit(1);
            }

            int main(void) {
                char *newpath;
                signal(SIGSEGV, exit_on_core);
                newpath = realpath("/tmp", NULL);
                exit((newpath != NULL) ? 0 : 1);
            }]])],[neta_cv_REALPATH_TAKES_NULL=yes],[neta_cv_REALPATH_TAKES_NULL=no],[neta_cv_REALPATH_TAKES_NULL=cross
        ])
    ]
)

if test x"$neta_cv_REALPATH_TAKES_NULL" = x"yes"; then
    AC_DEFINE(REALPATH_TAKES_NULL,1,[Whether the realpath function allows NULL])
fi
])
