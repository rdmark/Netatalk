dnl configure.in for netatalk

AC_INIT([netatalk],[m4_esyscmd([echo -n `cat VERSION`])])

AC_CANONICAL_TARGET
AM_INIT_AUTOMAKE([subdir-objects])
AC_CONFIG_HEADERS(config.h)
AC_CONFIG_MACRO_DIRS([macros])
AM_MAINTAINER_MODE([enable])

dnl Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
_LT_SET_OPTION([LT_INIT],[dlopen])
LT_INIT
AC_PROG_GREP
AC_PROG_PS

AM_PROG_CC_C_O

dnl Checks for header files.
AC_HEADER_DIRENT
AC_HEADER_SYS_WAIT
AC_CHECK_HEADERS(fcntl.h limits.h stdint.h strings.h time.h sys/param.h fcntl.h sys/file.h sys/ioctl.h sys/time.h sys/mnttab.h sys/statvfs.h sys/stat.h sys/vfs.h mntent.h syslog.h unistd.h termios.h termios.h netdb.h sgtty.h ufs/quota.h mount.h statfs.h sys/types.h dlfcn.h errno.h sys/uio.h langinfo.h locale.h sys/filio.h)
AC_CHECK_HEADERS([sys/mount.h], , , 
[#ifdef HAVE_SYS_PARAM_H
#include <sys/param.h>
#endif
])

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_UID_T
AC_C_INLINE
AC_TYPE_MODE_T
AC_TYPE_OFF_T
AC_TYPE_PID_T
AC_TYPE_SIZE_T

AC_STRUCT_TM

dnl --------------------------------------------------------------------------
dnl check if dlsym needs to add an underscore, uses libtool macros 
dnl --------------------------------------------------------------------------
LT_LIB_DLLOAD
AC_CHECK_FUNCS(dlopen dlsym dlclose)
LT_FUNC_DLSYM_USCORE
if test x"$libltdl_cv_need_uscore" = xyes; then
    AC_DEFINE(DLSYM_PREPEND_UNDERSCORE, 1, [BSD compatibility macro])
fi

dnl Checks for library functions.
AC_TYPE_GETGROUPS
AC_PROG_GCC_TRADITIONAL
AC_FUNC_MEMCMP
AC_HEADER_MAJOR
AC_FUNC_MMAP
AC_FUNC_UTIME_NULL
AC_CHECK_FUNCS(getcwd gethostname gettimeofday getusershell mkdir rmdir select socket strdup strcasestr strstr strtoul strchr memcpy)
AC_CHECK_FUNCS(backtrace_symbols setlocale nl_langinfo strlcpy strlcat setlinebuf dirfd pselect access pread pwrite)
AC_CHECK_FUNCS(waitpid getcwd strdup strndup strnlen strtoul strerror chown fchown chmod fchmod chroot link mknod mknod64)
ac_neta_haveatfuncs=yes
AC_CHECK_FUNCS(openat renameat fstatat unlinkat, , ac_neta_haveatfuncs=no)
if test x"$ac_neta_haveatfuncs" = x"yes" ; then
   AC_DEFINE([_ATFILE_SOURCE], 1, AT file source)
   AC_DEFINE([HAVE_ATFUNCS], 1, whether at funcs are available)
fi

AC_CHECK_MEMBERS(struct tm.tm_gmtoff,,, [#include <time.h>])

AC_CHECK_FUNC(gethostbyname,,[AC_CHECK_LIB(nsl,gethostbyname)])
AC_CHECK_FUNC(connect,,[AC_CHECK_LIB(socket,connect)])
dnl search for necessary libs for libpthread stuff
AC_SEARCH_LIBS(pthread_sigmask, pthread,, 
               [AC_MSG_ERROR([cannot find pthread_sigmask in libc or libpthread])])
if test x"$ac_cv_search_pthread_sigmask" != x"none required" ; then
   PTHREAD_LIBS=$ac_cv_search_pthread_sigmask
fi
AC_SUBST(PTHREAD_LIBS)
AC_CACHE_SAVE

dnl Checks for (v)snprintf
NETATALK_SNPRINTF_CHECK

dnl --------------------------------------------------------------------------
dnl 64bit platform check
dnl --------------------------------------------------------------------------

AC_MSG_CHECKING([whether to check for 64bit libraries])
# Test if the compiler is in 64bit mode
echo 'int i;' > conftest.$ac_ext
atalk_cv_cc_64bit_output=no
if AC_TRY_EVAL(ac_compile); then
    case `/usr/bin/file conftest.$ac_objext` in
    *"ELF 64"*)
      atalk_cv_cc_64bit_output=yes
      ;;
    esac
fi
rm -rf conftest*

case $host_cpu:$atalk_cv_cc_64bit_output in
powerpc64:yes | s390x:yes | sparc*:yes | x86_64:yes | i386:yes)
    case $target_os in
    solaris2*)
        AC_MSG_RESULT([yes])
        atalk_libname="lib/64"
        ;;
    *bsd* | dragonfly*)
        AC_MSG_RESULT([no])
        atalk_libname="lib"
        ;;
    *)
        AC_MSG_RESULT([yes])
        atalk_libname="lib64"
        ;;
    esac
    ;;
*:*)
    AC_MSG_RESULT([no])
    atalk_libname="lib"
    ;;
esac

dnl --------------------------------------------------------------------------
dnl specific configuration comes in here:
dnl --------------------------------------------------------------------------


netatalk_cv_systemd_enable=no
AC_MSG_CHECKING([for enabling and starting systemd services with make install])
AC_ARG_ENABLE(systemd-start,
 	[  --enable-systemd-start   make install also enables and starts systemd services],[
        if test x"$enableval" = x"yes"; then
		AC_DEFINE(SYSTEMD_START, 1, [Define if make install enables systemd services])
		netatalk_systemd_start=yes
		AC_MSG_RESULT([no])
	else
		AC_DEFINE(SYSTEMD_START, 0, [Define if make install enables systemd services])
		AC_MSG_RESULT([yes])
        fi],[
		AC_DEFINE(SYSTEMD_START, 0, [Define if make install enables systemd services])
		AC_MSG_RESULT([yes])
	]
)

netatalk_cv_admin_group=yes
AC_MSG_CHECKING([for administrative group support])
AC_ARG_ENABLE(admin-group,
 	[  --disable-admin-group   disable admin group],[
        if test x"$enableval" = x"no"; then
		AC_DEFINE(ADMIN_GRP, 0, [Define if the admin group should be enabled])
		netatalk_cv_admin_group=no
		AC_MSG_RESULT([no])
	else
		AC_DEFINE(ADMIN_GRP, 1, [Define if the admin group should be enabled])
		AC_MSG_RESULT([yes])
        fi],[
		AC_DEFINE(ADMIN_GRP, 1, [Define if the admin group should be enabled])
		AC_MSG_RESULT([yes])
	]
)

NETATALK_CONFIG_DIRS

netatalk_cv_with_cracklib=no
AC_ARG_WITH(cracklib,
	[  --with-cracklib=DICT    enable/set location of cracklib dictionary],[
	if test "x$withval" != "xno" ; then
		cracklib="$withval"
		AC_CHECK_LIB(crack, main, [
			AC_DEFINE(USE_CRACKLIB, 1, [Define if cracklib should be used])
			LIBS="$LIBS -lcrack"
			if test "$cracklib" = "yes"; then
				cracklib="/usr/$atalk_libname/cracklib_dict"
			fi
			AC_DEFINE_UNQUOTED(_PATH_CRACKLIB, "$cracklib",
				[path to cracklib dictionary])
			AC_MSG_RESULT([setting cracklib dictionary to $cracklib])
			netatalk_cv_with_cracklib=yes
			],[
			AC_MSG_ERROR([cracklib not found!])
			]
		)
	fi
	]
)
AC_MSG_CHECKING([for cracklib support])
AC_MSG_RESULT([$netatalk_cv_with_cracklib])

netatalk_cv_ddp_enabled=yes

AC_SYS_LARGEFILE([], AC_MSG_ERROR([AFP 3.x support requires Large File Support.]))
AC_CHECK_ICONV

dnl Check for CNID database backends
bdb_required=no
AC_NETATALK_CNID([bdb_required=yes],[bdb_required=no])

dnl Check for quota support
AC_CHECK_QUOTA

dnl Check for optional server location protocol support (used by MacOS X)
NETATALK_SRVLOC

dnl Check for optional Zeroconf support
NETATALK_ZEROCONF

dnl Check for PAM libs
netatalk_cv_use_pam=no
AC_PATH_PAM([
	use_pam_so=yes
	compile_pam=yes
	netatalk_cv_use_pam=yes
	AC_DEFINE(USE_PAM, 1, [Define to enable PAM support])
])

netatalk_cv_use_shadowpw=no
AC_ARG_WITH(shadow,
	[  --with-shadow           enable shadow password support [[auto]]],
	[netatalk_cv_use_shadowpw="$withval"],
	[netatalk_cv_use_shadowpw=auto]
)

if test "x$netatalk_cv_use_shadowpw" != "xno"; then
    AC_CHECK_HEADER([shadow.h])
    if test x"$ac_cv_header_shadow_h" = x"yes"; then
	netatalk_cv_use_shadowpw=yes
	AC_DEFINE(SHADOWPW, 1, [Define if shadow passwords should be used])
    else 
      if test "x$shadowpw" = "xyes"; then
        AC_MSG_ERROR([shadow support not available])
      else
       	netatalk_cv_use_shadowpw=no
      fi
    fi 
fi

AC_MSG_CHECKING([whether shadow support should be enabled])
if test "x$netatalk_cv_use_shadowpw" = "xyes"; then
	AC_MSG_RESULT([yes])
else
	AC_MSG_RESULT([no])
fi
	
netatalk_cv_use_shellcheck=yes
AC_MSG_CHECKING([whether checking for a valid shell should be enabled])
AC_ARG_ENABLE(shell-check,
	[  --disable-shell-check   disable checking for a valid shell],[
	if test "$enableval" = "no"; then 
		AC_DEFINE(DISABLE_SHELLCHECK, 1, [Define if shell check should be disabled])
		AC_MSG_RESULT([no])
		netatalk_cv_use_shellcheck=no
	else
		AC_MSG_RESULT([yes])
	fi
	],[
		AC_MSG_RESULT([yes])
	]
)

NETATALK_TCP_WRAPPERS

AC_MSG_CHECKING([whether system (fcntl) locking should be disabled])
AC_ARG_ENABLE(locking,
        [  --disable-locking       disable system locking],[
        	if test "$enableval" = "no"; then
                	AC_DEFINE(DISABLE_LOCKING, 1, [Define if system (fcntl) locking should be disabled])
	                AC_MSG_RESULT([yes])
		else
	                AC_MSG_RESULT([no])
        	fi
		
	],[
		AC_MSG_RESULT([no])
	]

)

AC_ARG_ENABLE(gentoo,
	[  --enable-gentoo         use gentoo-style sysv configuration ],[
	if test "$enableval" = "yes"; then
		sysv_style=gentoo
	fi
	AC_MSG_RESULT([enabling gentoo-style sysv support])
	]
)

AC_ARG_ENABLE(netbsd,
	[  --enable-netbsd         use NetBSD-style rc.d configuration ],
	if test "x$enableval" = "xyes"; then
		sysv_style=netbsd
	fi
	AC_MSG_RESULT([enabling NetBSD-style rc.d support])
)

AC_ARG_ENABLE(debian,
	[  --enable-debian         use debian-style sysv configuration ],[
	if test "$enableval" = "yes"; then
		sysv_style=debian
	fi
	AC_MSG_RESULT([enabling debian-style sysv support])
	]
)

AC_ARG_ENABLE(systemd,
	[  --enable-systemd        use general systemd configuration],[
	if test "$enableval" = "yes"; then
		sysv_style=systemd
	fi
	AC_MSG_RESULT([enabling general systemd support])
	]
)

dnl ----- timelord compilation (disabled by default)
AC_MSG_CHECKING([whether timelord should be compiled])
compile_timelord=yes
AC_ARG_ENABLE(timelord,
	[  --enable-timelord       enable compilation of timelord server],
	[compile_timelord="$enableval"],
	[compile_timelord="yes"]
)
AC_MSG_RESULT([$compile_timelord])

dnl ----- a2boot compilation (enabled by default)
AC_MSG_CHECKING([whether a2boot should be compiled])
compile_a2boot=yes
AC_ARG_ENABLE(a2boot,
	[  --enable-a2boot         enable compilation of Apple2 boot server],
	[compile_a2boot="$enableval"],
	[compile_a2boot="yes"]
)
AC_MSG_RESULT([$compile_a2boot])

AC_ARG_WITH(uams-path,
	[  --with-uams-path=PATH   path to UAMs [[PKGCONF/uams]]],[
		uams_path="$withval"
	],[
		uams_path="${PKGCONFDIR}/uams"
	]
)

NETATALK_AC_CUPS

dnl check if we can use attribute unused (gcc only) from ethereal
AC_MSG_CHECKING(to see if we can add '__attribute__((unused))' to CFLAGS)
if test x$GCC != x ; then
  CFLAGS="-D_U_=\"__attribute__((unused))\" $CFLAGS"
  AC_MSG_RESULT(yes)
else
  CFLAGS="-D_U_=\"\" $CFLAGS"
  AC_MSG_RESULT(no)
fi

dnl --------------------------------------------------------------------------
dnl FHS stuff has to be done last because it overrides other defaults
dnl --------------------------------------------------------------------------

AC_MSG_CHECKING([whether to use Filesystem Hierarchy Standard (FHS) compatibility])
AC_ARG_ENABLE(fhs,
	[  --enable-fhs            use Filesystem Hierarchy Standard (FHS) compatibility],[
	if test "$enableval" = "yes"; then
		uams_path="${libdir}/netatalk"
		sysconfdir="/etc"
		PKGCONFDIR=${sysconfdir}/netatalk
		SERVERTEXT="${PKGCONFDIR}/msg"
		use_pam_so=yes
		mandir="/usr/share/man"
		AC_DEFINE(FHS_COMPATIBILITY, 1, [Define if you want compatibily with the FHS])
		AC_MSG_RESULT([yes])
	else
		AC_MSG_RESULT([no])
	fi
	],[
		AC_MSG_RESULT([no])
	]
)

dnl --------------------------------------------------------------------------
dnl post-FHS substitutions, etc
dnl --------------------------------------------------------------------------

dnl ***** UAMS_PATH
dnl AC_DEFINE_UNQUOTED(UAMS_PATH, "${uams_path}",
dnl 	[path to UAMs [default=PKGCONF/uams]])
UAMS_PATH="${uams_path}"
AC_SUBST(UAMS_PATH)

dnl --------------------------------------------------------------------------
dnl drop in includes for top level directory structures here...
dnl --------------------------------------------------------------------------

dnl Note: $(top_srcdir)/include should be added before all other includes
dnl       so that includes from that directory a preferred to includes from
dnl       /usr/include or similar places.
LIBS="$LIBS -L\$(top_srcdir)/libatalk"
CFLAGS="-I\$(top_srcdir)/include $CFLAGS -I\$(top_srcdir)/sys"

AC_DEFINE(OPEN_NOFOLLOW_ERRNO, ELOOP, errno returned by open with O_NOFOLLOW)

dnl --------------------------------------------------------------------------
dnl specific configuration comes in here:
dnl --------------------------------------------------------------------------

dnl ac_cv_target_os is now host_os, ac_cv_target_cpu is now host_cpu

dnl --------------------- determine operating system from "target"
case "$host_os" in
	*linux*)   			this_os=linux ;;
	*netbsd*) 			this_os=netbsd; CFLAGS="${CFLAGS} -I/usr/pkg/include"; LIBS="${LIBS} -L/usr/pkg/lib" ;;
	*solaris*) 			this_os=solaris ;;
esac

case "$host_cpu" in
	i386|i486|i586|i686|k7)		this_cpu=x86 ;;
	alpha)				this_cpu=alpha ;;
	mips)				this_cpu=mips ;;
	powerpc|ppc)			this_cpu=ppc ;;
esac

dnl --------------------- GNU source
case "$this_os" in
	linux)	AC_DEFINE(_GNU_SOURCE, 1, [Whether to use GNU libc extensions])
        ;;
esac

dnl --------------------- operating system specific flags (port from sys/*)

dnl ----- Linux specific -----
if test x"$this_os" = "xlinux"; then 
	AC_MSG_RESULT([ * Linux specific configuration])
	
	dnl ----- kernel 2.6 changed struct at_addr to atalk_addr
	AC_MSG_CHECKING([for struct atalk_addr])
dnl	AC_COMPILE_IFELSE([
	AC_COMPILE_IFELSE([AC_LANG_PROGRAM([[
#include <sys/socket.h>
#include <asm/types.h>
#include <linux/atalk.h>

	struct atalk_addr foo;
]], [[ ]])],[
		ac_have_atalk_addr=yes
		AC_MSG_RESULT([yes])
	],[
		AC_MSG_RESULT([no])
	])

if test "x$ac_have_atalk_addr" = "xyes"; then
	AC_DEFINE(HAVE_ATALK_ADDR, 1, [set if struct at_addr is called atalk_addr])
fi

dnl ----- check if we need the quotactl wrapper
#	AC_CHECK_HEADERS(sys/quota.h linux/quota.h)
#	AC_CHECK_FUNC(quotactl,,
#		AC_DEFINE(NEED_QUOTACTL_WRAPPER, 1, [Define if the quotactl wrapper is needed])
#		AC_MSG_RESULT([enabling quotactl wrapper])
#	)
	AC_DEFINE(NO_QUOTA_SUPPORT, 1, [Define if Quota support should be disabled])

fi

dnl ----- NetBSD specific -----
if test x"$this_os" = "xnetbsd"; then 
	AC_MSG_RESULT([ * NetBSD specific configuration])
	AC_DEFINE(BSD4_4, 1, [BSD compatiblity macro])
	AC_DEFINE(NETBSD, 1, [Define if OS is NetBSD])
	AC_DEFINE(OPEN_NOFOLLOW_ERRNO, EFTYPE, errno returned by open with O_NOFOLLOW)

	need_dash_r=yes 
fi

dnl ----- Solaris specific -----
if test x"$this_os" = "xsolaris"; then 
	AC_MSG_RESULT([ * Solaris specific configuration])
	AC_DEFINE(__svr4__, 1, [Solaris compatibility macro])
	AC_DEFINE(_ISOC9X_SOURCE, 1, [Compatibility macro])
	AC_DEFINE(NO_STRUCT_TM_GMTOFF, 1, [Define if the gmtoff member of struct tm is not available])
	AC_DEFINE(SOLARIS, 1, [Solaris compatibility macro])
    AC_DEFINE(_XOPEN_SOURCE, 600, [Solaris compilation environment])
    AC_DEFINE(__EXTENSIONS__,  1, [Solaris compilation environment])
	CFLAGS="-I\$(top_srcdir)/sys/generic $CFLAGS"
	need_dash_r=yes
	sysv_style=solaris

	solaris_module=no
	AC_MSG_CHECKING([if we can build Solaris kernel module])
	if test -x /usr/ccs/bin/ld && test x"$netatalk_cv_ddp_enabled" = x"yes" ; then
		solaris_module=yes
	fi
	AC_MSG_RESULT([$solaris_module])

	COMPILE_64BIT_KMODULE=no
	KCFLAGS=""
	KLDFLAGS=""
	COMPILE_KERNEL_GCC=no

	if test "$solaris_module" = "yes"; then
	   dnl Solaris kernel module stuff
           AC_MSG_CHECKING([if we have to build a 64bit kernel module])

	   # check for isainfo, if not found it has to be a 32 bit kernel (<=2.6)	
	   if test -x /usr/bin/isainfo; then
		# check for 64 bit platform
		if isainfo -kv | grep '^64-bit'; then
			COMPILE_64BIT_KMODULE=yes
		fi
	   fi

	   AC_MSG_RESULT([$COMPILE_64BIT_KMODULE])

	   if test "${GCC}" = yes; then
		COMPILE_KERNEL_GCC=yes
		if test "$COMPILE_64BIT_KMODULE" = yes; then
  	        
                        AC_MSG_CHECKING([if we can build a 64bit kernel module])
		        
                        case `$CC --version 2>/dev/null` in
			[[12]].* | 3.0.*)
				COMPILE_64BIT_KMODULE=no
				COMPILE_KERNEL_GCC=no	
				solaris_module=no;;
			*)
			       	# use for 64 bit
				KCFLAGS="-m64"
				#KLDFLAGS="-melf64_sparc"
				KLDFLAGS="-64";;
			esac	
			
			AC_MSG_RESULT([$COMPILE_64BIT_KMODULE])
			
		else
			KCFLAGS=""
			KLDFLAGS=""
		fi
		KCFLAGS="$KCFLAGS -D_KERNEL -Wall -Wstrict-prototypes"
           else
		if test "$COMPILE_64BIT_KMODULE" = yes; then
                # use Sun CC (for a 64-bit kernel, uncomment " -xarch=v9 -xregs=no%appl ")
 			KCFLAGS="-xarch=v9 -xregs=no%appl"
			KLDFLAGS="-64"
		else
 			KCFLAGS=""
			KLDFLAGS=""
		fi
		KCFLAGS="-D_KERNEL $KCFLAGS -mno-app-regs -munaligned-doubles -fpcc-struct-return"
	   fi

           AC_CACHE_CHECK([for timeout_id_t],netatalk_cv_HAVE_TIMEOUT_ID_T,[
           AC_LINK_IFELSE([AC_LANG_PROGRAM([[\
#include <sys/stream.h>
#include <sys/ddi.h>]], [[\
timeout_id_t dummy;
]])],[netatalk_cv_HAVE_TIMEOUT_ID_T=yes],[netatalk_cv_HAVE_TIMEOUT_ID_T=no])])

	   AC_DEFINE(HAVE_TIMEOUT_ID_T, test x"$netatalk_cv_HAVE_TIMEOUT_ID" = x"yes", [define for timeout_id_t])
	fi

	AC_SUBST(COMPILE_KERNEL_GCC)
	AC_SUBST(COMPILE_64BIT_KMODULE)
	AC_SUBST(KCFLAGS)
	AC_SUBST(KLDFLAGS)
fi

dnl ------ Check for sendfile() --------
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
        ;;

    *)
        ;;

    esac

    if test x"$netatalk_cv_HAVE_SENDFILE" = x"yes"; then
        AC_DEFINE(WITH_SENDFILE,1,[Whether sendfile() should be used])
    fi
fi

dnl -- check for libgcrypt, if found enables DHX UAM
AM_PATH_LIBGCRYPT([1:1.2.3],[neta_cv_compile_dhx2=yes
			neta_cv_have_libgcrypt=yes
			AC_MSG_NOTICE([Enabling DHX2 UAM])
			AC_DEFINE(HAVE_LIBGCRYPT, 1, [Define if the DHX2 modules should be built with libgcrypt])
			AC_DEFINE(UAM_DHX2, 1, [Define if the DHX2 UAM modules should be compiled])
			])

dnl -- look for openssl, if found enables DHX UAM and Randnum UAM
AC_PATH_SSL

dnl Check for Berkeley DB library
if test "x$bdb_required" = "xyes"; then
	AC_PATH_BDB(, [
		AC_MSG_RESULT([])
		AC_MSG_RESULT([Make sure you have the required Berkeley DB libraries AND headers installed.])
		AC_MSG_RESULT([You can download the latest version from http://www.sleepycat.com.])
		AC_MSG_RESULT([If you have installed BDB in a non standard location use the])
		AC_MSG_RESULT([--with-bdb=/path/to/bdb configure option and make sure])
		AC_MSG_RESULT([your linker is configured to check for libraries there.])
		AC_MSG_ERROR([Berkeley DB library required but not found!])
	])
fi

dnl -- check for crypt
AC_CRYPT

dnl --------------------- overwrite the config files . . . or not.
AC_MSG_CHECKING([whether configuration files should be overwritten])
AC_ARG_ENABLE(overwrite,
	[  --enable-overwrite      overwrite configuration files during installation],
	[OVERWRITE_CONFIG="${enable_overwrite}"],
	[OVERWRITE_CONFIG="no"]
)
AC_MSG_RESULT([$OVERWRITE_CONFIG])

dnl --------------------- check for LDAP support
AC_MSG_CHECKING(for LDAP)
AC_ARG_WITH(ldap,
    [AS_HELP_STRING([--with-ldap],
        [LDAP support (default=auto)])],
    [ case "$withval" in
      yes|no)
          with_ldap="$withval"
		  ;;
      *)
          with_ldap=auto
          ;;
      esac ])
AC_MSG_RESULT($with_ldap)

if test x"$with_ldap" != x"no" ; then
   	AC_CHECK_HEADER([ldap.h], with_ldap=yes,
        [ if test x"$with_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP headers])
        fi
		with_ldap=no
        ])
	AC_CHECK_LIB(ldap, ldap_init, with_ldap=yes,
        [ if test x"$with_ldap" = x"yes" ; then
            AC_MSG_ERROR([Missing LDAP library])
        fi
		with_ldap=no
        ])
fi

if test x"$with_ldap" = x"yes"; then
	AC_DEFINE(HAVE_LDAP,1,[Whether LDAP is available])
fi

dnl --------------------- check for Extended Attributes support
neta_cv_eas="ad"
neta_cv_eas_sys_found=no
neta_cv_eas_sys_not_found=no

AC_CHECK_HEADERS(sys/attributes.h attr/xattr.h sys/xattr.h sys/extattr.h sys/uio.h sys/ea.h)

case "$this_os" in

  *solaris*)
	AC_CHECK_FUNCS([attropen],
                   [neta_cv_eas_sys_found=yes],
                   [neta_cv_eas_sys_not_found=yes])
  ;;

  *)
	AC_SEARCH_LIBS(getxattr, [attr])

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
       AC_CHECK_FUNCS([getxattr lgetxattr fgetxattr listxattr llistxattr],
                      [neta_cv_eas_sys_found=yes],
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([flistxattr removexattr lremovexattr fremovexattr],,
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([setxattr lsetxattr fsetxattr],,
                      [neta_cv_eas_sys_not_found=yes])
    fi

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
	   AC_CHECK_FUNCS([getea fgetea lgetea listea flistea llistea],
                      [neta_cv_eas_sys_found=yes],
                      [neta_cv_eas_sys_not_found=yes])
	   AC_CHECK_FUNCS([removeea fremoveea lremoveea setea fsetea lsetea],,
                      [neta_cv_eas_sys_not_found=yes])
    fi

    if test "x$neta_cv_eas_sys_found" != "xyes" ; then
	   AC_CHECK_FUNCS([attr_get attr_list attr_set attr_remove],,
                      [neta_cv_eas_sys_not_found=yes])
       AC_CHECK_FUNCS([attr_getf attr_listf attr_setf attr_removef],,
                      [neta_cv_eas_sys_not_found=yes])
    fi
  ;;
esac

if test "x$neta_cv_eas_sys_found" = "xyes" ; then
   if test "x$neta_cv_eas_sys_not_found" != "xyes" ; then
      neta_cv_eas="$neta_cv_eas | sys"
   fi
fi
AC_DEFINE_UNQUOTED(EA_MODULES,["$neta_cv_eas"],[Available Extended Attributes modules])

dnl --------------------- Check if realpath() takes NULL
AC_CACHE_CHECK([if the realpath function allows a NULL argument],
    neta_cv_REALPATH_TAKES_NULL, [
        AC_RUN_IFELSE([AC_LANG_SOURCE([[
            #include <stdio.h>
            #include <limits.h>
            #include <signal.h>

            void exit_on_core(int ignored) {
                 exit(1);
            }

            main() {
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

dnl --------------------- last minute substitutions

dnl Comment/change this to deal with gcc "helpful" warnings

CFLAGS="-Wall -Werror $CFLAGS"

AC_SUBST(LIBS)
AC_SUBST(CFLAGS)
AC_SUBST(OVERWRITE_CONFIG)

AM_CONDITIONAL(SOLARIS_MODULE, test x$solaris_module = xyes)
AM_CONDITIONAL(HAVE_LIBGCRYPT, test x$neta_cv_have_libgcrypt = xyes)
AM_CONDITIONAL(HAVE_OPENSSL, test x$neta_cv_have_openssl = xyes)
AM_CONDITIONAL(HAVE_LDAP, test x"$with_ldap" = x"yes")
AM_CONDITIONAL(USE_DHX, test x$neta_cv_compile_dhx = xyes)
AM_CONDITIONAL(USE_DHX2, test x$neta_cv_compile_dhx2 = xyes)
AM_CONDITIONAL(USE_RANDNUM, test x$neta_cv_have_openssl = xyes)
AM_CONDITIONAL(USE_PAM_SO, test x$use_pam_so = xyes)
AM_CONDITIONAL(USE_PAM, test x$netatalk_cv_install_pam = xyes)
AM_CONDITIONAL(BUILD_PAM, test x$compile_pam = xyes)
AM_CONDITIONAL(DEFAULT_HOOK, test x$neta_cv_have_libgcrypt != xyes && test x$neta_cv_have_openssl != xyes)
AM_CONDITIONAL(USE_NETBSD, test x$sysv_style = xnetbsd)
AM_CONDITIONAL(USE_REDHAT_SYSV, test x$sysv_style = xredhat-sysv)
AM_CONDITIONAL(USE_SUSE_SYSV, test x$sysv_style = xsuse-sysv)
AM_CONDITIONAL(USE_SHADOWPW, test x$shadowpw = xyes)
AM_CONDITIONAL(USE_SOLARIS, test x$sysv_style = xsolaris)
AM_CONDITIONAL(USE_GENTOO, test x$sysv_style = xgentoo)
AM_CONDITIONAL(USE_DEBIAN, test x$sysv_style = xdebian)
AM_CONDITIONAL(USE_SYSTEMD, test x$sysv_style = xsystemd || test x$sysv_style = xredhat-systemd || test x$sysv_style = xsuse-systemd)
AM_CONDITIONAL(USE_UNDEF, test x$sysv_style = x)
AM_CONDITIONAL(USE_BDB, test x$bdb_required = xyes)
AM_CONDITIONAL(SYSTEMD_START, test x$netatalk_systemd_start = xyes)

dnl --------------------- generate files

AC_CONFIG_FILES([Makefile
	bin/Makefile
	bin/ad/Makefile
	bin/adv1tov2/Makefile
	bin/aecho/Makefile
	bin/afppasswd/Makefile
	bin/cnid/Makefile
	bin/cnid/cnid2_create
	bin/getzones/Makefile
	bin/megatron/Makefile
	bin/misc/Makefile
	bin/nbp/Makefile
	bin/pap/Makefile
	bin/psorder/Makefile
	bin/uniconv/Makefile
	config/Makefile
	config/pam/Makefile
	contrib/Makefile
	contrib/macusers/Makefile
	contrib/macusers/macusers
	contrib/misc/Makefile
	contrib/printing/Makefile
	contrib/shell_utils/Makefile
	contrib/shell_utils/apple_dump
	contrib/shell_utils/asip-status.pl
	contrib/timelord/Makefile
	contrib/a2boot/Makefile
	distrib/Makefile
	distrib/config/Makefile
	distrib/config/netatalk-config
	distrib/initscripts/Makefile
	distrib/m4/Makefile
	doc/Makefile
	etc/Makefile
	etc/afpd/Makefile
	etc/atalkd/Makefile
	etc/cnid_dbd/Makefile
	etc/uams/Makefile
	etc/papd/Makefile
	etc/psf/Makefile
	include/Makefile
	include/atalk/Makefile
	libatalk/Makefile
	libatalk/acl/Makefile
	libatalk/adouble/Makefile
	libatalk/asp/Makefile
	libatalk/atp/Makefile
	libatalk/bstring/Makefile
	libatalk/cnid/Makefile
	libatalk/cnid/last/Makefile
	libatalk/cnid/dbd/Makefile
	libatalk/compat/Makefile
	libatalk/dsi/Makefile
	libatalk/nbp/Makefile
	libatalk/netddp/Makefile
	libatalk/util/Makefile
	libatalk/unicode/Makefile
	libatalk/unicode/charsets/Makefile
	libatalk/vfs/Makefile
	macros/Makefile
	man/Makefile
	man/man1/Makefile
	man/man3/Makefile
	man/man4/Makefile
	man/man5/Makefile
	man/man8/Makefile
	sys/Makefile
	sys/netatalk/Makefile
	sys/solaris/Makefile
	test/Makefile
	test/afpd/Makefile
	])
AC_CONFIG_COMMANDS([default],[chmod a+x distrib/config/netatalk-config contrib/shell_utils/apple_*
],[])
AC_OUTPUT

AC_NETATALK_LIBS_SUMMARY
AC_NETATALK_CONFIG_SUMMARY
