dnl AM_PATH_LIBGCRYPT([MINIMUM-VERSION,
dnl                   [ACTION-IF-FOUND [, ACTION-IF-NOT-FOUND ]]])
dnl Test for libgcrypt and define LIBGCRYPT_CFLAGS and LIBGCRYPT_LIBS.
dnl MINIMUM-VERSION is a string with the version number optionally prefixed
dnl with the API version to also check the API compatibility. Example:
dnl a MINIMUM-VERSION of 1:1.2.5 won't pass the test unless the installed
dnl version of libgcrypt is at least 1.2.5 *and* the API number is 1.  Using
dnl this feature allows to prevent build against newer versions of libgcrypt
dnl with a changed API.
dnl
dnl If a prefix option is not used, the config script is first
dnl searched in $SYSROOT/bin and then along $PATH.  If the used
dnl config script does not match the host specification the script
dnl is added to the gpg_config_script_warn variable.
dnl
AC_DEFUN([AC_NETATALK_PATH_LIBGCRYPT],
[ AC_REQUIRE([AC_CANONICAL_HOST])
  AC_ARG_WITH(libgcrypt-dir,
            AS_HELP_STRING([--with-libgcrypt-dir=PATH],
                           [path where LIBGCRYPT is installed (optional)]),
     libgcrypt_config_prefix="$withval", libgcrypt_config_prefix="")
  if test x"${LIBGCRYPT_CONFIG}" = x ; then
     if test x"${libgcrypt_config_prefix}" != x ; then
        LIBGCRYPT_CONFIG="${libgcrypt_config_prefix}/bin/libgcrypt-config"
     fi
  fi

  use_gpgrt_config=""
  if test x"${LIBGCRYPT_CONFIG}" = x -a x"$GPGRT_CONFIG" != x -a "$GPGRT_CONFIG" != "no"; then
    if $GPGRT_CONFIG libgcrypt --exists; then
      LIBGCRYPT_CONFIG="$GPGRT_CONFIG libgcrypt"
      AC_MSG_NOTICE([Use gpgrt-config as libgcrypt-config])
      use_gpgrt_config=yes
    fi
  fi
  if test -z "$use_gpgrt_config"; then
    if test x"${LIBGCRYPT_CONFIG}" = x ; then
      case "${SYSROOT}" in
         /*)
           if test -x "${SYSROOT}/bin/libgcrypt-config" ; then
             LIBGCRYPT_CONFIG="${SYSROOT}/bin/libgcrypt-config"
           fi
           ;;
         '')
           ;;
          *)
           AC_MSG_WARN([Ignoring \$SYSROOT as it is not an absolute path.])
           ;;
      esac
    fi
    AC_PATH_PROG(LIBGCRYPT_CONFIG, libgcrypt-config, no)
  fi

  tmp=ifelse([$1], ,1:1.2.0,$1)
  if echo "$tmp" | grep ':' >/dev/null 2>/dev/null ; then
     req_libgcrypt_api=`echo "$tmp"     | sed 's/\(.*\):\(.*\)/\1/'`
     min_libgcrypt_version=`echo "$tmp" | sed 's/\(.*\):\(.*\)/\2/'`
  else
     req_libgcrypt_api=0
     min_libgcrypt_version="$tmp"
  fi

  AC_MSG_CHECKING(for LIBGCRYPT - version >= $min_libgcrypt_version)
  ok=no
  if test "$LIBGCRYPT_CONFIG" != "no" ; then
    req_major=`echo $min_libgcrypt_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\)/\1/'`
    req_minor=`echo $min_libgcrypt_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\)/\2/'`
    req_micro=`echo $min_libgcrypt_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\)/\3/'`
    if test -z "$use_gpgrt_config"; then
      libgcrypt_config_version=`$LIBGCRYPT_CONFIG --version`
    else
      libgcrypt_config_version=`$LIBGCRYPT_CONFIG --modversion`
    fi
    major=`echo $libgcrypt_config_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\1/'`
    minor=`echo $libgcrypt_config_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\2/'`
    micro=`echo $libgcrypt_config_version | \
               sed 's/\([[0-9]]*\)\.\([[0-9]]*\)\.\([[0-9]]*\).*/\3/'`
    if test "$major" -gt "$req_major"; then
        ok=yes
    else
        if test "$major" -eq "$req_major"; then
            if test "$minor" -gt "$req_minor"; then
               ok=yes
            else
               if test "$minor" -eq "$req_minor"; then
                   if test "$micro" -ge "$req_micro"; then
                     ok=yes
                   fi
               fi
            fi
        fi
    fi
  fi
  if test $ok = yes; then
    AC_MSG_RESULT([yes ($libgcrypt_config_version)])
  else
    AC_MSG_RESULT(no)
  fi
  if test $ok = yes; then
     # If we have a recent libgcrypt, we should also check that the
     # API is compatible
     if test "$req_libgcrypt_api" -gt 0 ; then
        if test -z "$use_gpgrt_config"; then
           tmp=`$LIBGCRYPT_CONFIG --api-version 2>/dev/null || echo 0`
	else
           tmp=`$LIBGCRYPT_CONFIG --variable=api_version 2>/dev/null || echo 0`
	fi
        if test "$tmp" -gt 0 ; then
           AC_MSG_CHECKING([LIBGCRYPT API version])
           if test "$req_libgcrypt_api" -eq "$tmp" ; then
             AC_MSG_RESULT([okay])
           else
             ok=no
             AC_MSG_RESULT([does not match. want=$req_libgcrypt_api got=$tmp])
           fi
        fi
     fi
  fi
  if test $ok = yes; then
    LIBGCRYPT_CFLAGS=`$LIBGCRYPT_CONFIG --cflags`
    LIBGCRYPT_LIBS=`$LIBGCRYPT_CONFIG --libs`
    neta_cv_compile_dhx2=yes
    neta_cv_have_libgcrypt=yes
    AC_MSG_NOTICE([Enabling DHX2 UAM])
    AC_DEFINE(HAVE_LIBGCRYPT, 1, [Define if the DHX2 modules should be built with libgcrypt])
    AC_DEFINE(UAM_DHX2, 1, [Define if the DHX2 UAM modules should be compiled])
    ifelse([$2], , :, [$2])
    if test -z "$use_gpgrt_config"; then
      libgcrypt_config_host=`$LIBGCRYPT_CONFIG --host 2>/dev/null || echo none`
    else
      libgcrypt_config_host=`$LIBGCRYPT_CONFIG --variable=host 2>/dev/null || echo none`
    fi
  else
    LIBGCRYPT_CFLAGS=""
    LIBGCRYPT_LIBS=""
    ifelse([$3], , :, [$3])
  fi
  AC_SUBST(LIBGCRYPT_CFLAGS)
  AC_SUBST(LIBGCRYPT_LIBS)
])
