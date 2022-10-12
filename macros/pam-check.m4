# AC_NETATALK_PATH_PAM
# --------------------
# PAM finding macro.

AC_DEFUN([AC_NETATALK_PATH_PAM], [
    netatalk_cv_use_pam=no
	AC_ARG_WITH(pam, [  --with-pam[[=PATH]]       specify path to PAM installation [[auto]]],
		[
			require_pam="yes"
			if test "x$withval" = "xno"; then
				PAMDIR="NONE"
				require_pam="never"
			elif test "x$withval" = "xyes"; then
				PAMDIR="NONE"
			else
				PAMDIR="$withval"
			fi
		],
		[PAMDIR="NONE";require_pam="no"]
	)

	AC_MSG_CHECKING([for PAM installation directory])

	if test "x$PAMDIR" = "xNONE" -a "x$require_pam" != "xnever"; then
		 dnl Test for PAM
		 pam_paths="/ /usr/ /usr/local/"
		 for path in $pam_paths; do
		if test -d "${path}etc/pam.d"; then
			PAMDIR="$path"
			break
		fi
		 done
	fi

	if test "x$PAMDIR" != "xNONE"; then
		AC_MSG_RESULT([yes (path: ${PAMDIR}etc/pam.d)])
	else
		AC_MSG_RESULT([no])
	fi

	pam_found="no"
	if test "x$require_pam" != "xnever"; then

		savedCFLAGS="$CFLAGS"
		savedLDFLAGS="$LDFLAGS"
		savedLIBS="$LIBS"

		if test "x$PAMDIR" != "xNONE" -a "x$PAMDIR" != "x/"; then
			PAM_CFLAGS="-I${PAMDIR}include"
			PAM_LDFLAGS="-L${PAMDIR}lib"
			LDFLAGS="$LDFLAGS $PAM_LDFLAGS"
			CFLAGS="$CFLAGS $PAM_CFLAGS"
		fi

		AC_CHECK_HEADERS(security/pam_appl.h)

		if test x"$ac_cv_header_security_pam_appl_h" = x"no"; then
			pam_found=no
		else
			AC_CHECK_LIB(pam, pam_set_item, [
				PAM_LIBS="$PAM_LDFLAGS -lpam"
				pam_found="yes"
			])
		fi
		CFLAGS="$savedCFLAGS"
		LDFLAGS="$savedLDFLAGS"
		LIBS="$savedLIBS"
	fi

	netatalk_cv_install_pam=yes

	AC_MSG_CHECKING([whether to enable PAM support])
	if test "x$pam_found" = "xno"; then
		netatalk_cv_install_pam=no
		if test "x$require_pam" = "xyes"; then
			AC_MSG_ERROR([PAM support missing])
		else
			AC_MSG_RESULT([no])
		fi
		ifelse([$2], , :, [$2])
	else
		AC_MSG_RESULT([yes])
		ifelse([$1], , :, [$1])
        use_pam_so=yes
	    compile_pam=yes
	    netatalk_cv_use_pam=yes
	    AC_DEFINE(USE_PAM, 1, [Define to enable PAM support])
	fi

    AC_ARG_WITH(
        pam-confdir,
        [AS_HELP_STRING([--with-pam-confdir=PATH],[Path to PAM config dir (default: /etc/pam.d)])],
        ac_cv_pamdir=$withval,
        ac_cv_pamdir='/etc/pam.d'
    )

    PAMDIR="$ac_cv_pamdir"

    LIB_REMOVE_USR_LIB(PAM_LIBS)
    CFLAGS_REMOVE_USR_INCLUDE(PAM_CFLAGS)
	AC_SUBST(PAMDIR)
	AC_SUBST(PAM_CFLAGS)
	AC_SUBST(PAM_LIBS)
    AC_SUBST(PAM_DIRECTIVE)
    AC_SUBST(PAM_AUTH)
    AC_SUBST(PAM_ACCOUNT)
    AC_SUBST(PAM_PASSWORD)
    AC_SUBST(PAM_SESSION)
])
