dnl Autoconf macro to check for Berkeley DB library

AC_DEFUN([AC_NETATALK_PATH_BDB], [
	AC_ARG_WITH(bdb-dir, [  --with-bdb-dir=PATH     specify path to Berkeley DB installation (must contain
                          lib and include dirs)],
		[
			if test "x$withval" = "xno"; then
				trybdb=no
			elif test "x$withval" = "xyes"; then
				trybdb=yes
				trybdbdir=
			else
				dnl FIXME: should only try in $withval
				trybdb=yes
				trybdbdir="$withval"
			fi
		], [trybdb=yes]
	)

	BDB_CFLAGS=""
	BDB_LIBS=""
	saved_LIBS=$LIBS
	saved_CFLAGS=$CFLAGS
	neta_cv_have_bdb=no

	if test "$trybdb" = "yes"; then
		AC_MSG_CHECKING([for BDB])
		for bdbdir in "" $trybdbdir $(brew --prefix berkeley-db) ; do
			if test -f "$bdbdir/include/db.h" ; then
				BDB_CFLAGS="$BDB_CFLAGS -I$bdbdir/include"
				BDB_LIBS="$BDB_LIBS -L$bdbdir/lib -L$bdbdir -ldb"
					if test "x$enable_dtags" = "xyes"; then
						BDB_LIBS="$BDB_LIBS -Wl,--enable-new-dtags"
					fi
				AC_MSG_RESULT([$bdbdir (Berkeley DB present)])
				CFLAGS="$CFLAGS $BDB_CFLAGS"
				LIBS="$LIBS $BDB_LIBS"
				neta_cv_have_bdb=yes
				CFLAGS=$saved_CFLAGS
				LIBS=$saved_LIBS
				break
			fi
		done
		if test "x$neta_cv_have_bdb" = "xno"; then
			AC_MSG_RESULT([no])
			AC_MSG_ERROR([Berkeley DB library required but not found, please install using Homebrew!])
		fi
	fi
	CFLAGS_REMOVE_USR_INCLUDE(BDB_CFLAGS)
	LIB_REMOVE_USR_LIB(BDB_LIBS)
	AC_SUBST(BDB_CFLAGS)
	AC_SUBST(BDB_LIBS)
	LIBS=$saved_LIBS
])
