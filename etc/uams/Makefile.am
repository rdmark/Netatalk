# Makefile.am for etc/uams/

#
# conditionally build some modules
#

uams_LINKS = uams_dhx.so
dhx_exec_hook = echo
dhx2_exec_hook = echo

# these are complex: check if DHX and DHX2 module should be build

if USE_DHX
if HAVE_OPENSSL
if BUILD_PAM
UAMS_DHX_PAM = uams_dhx_pam.la
endif
if USE_PAM_SO
dhx_exec_hook += && $(LN_S) uams_dhx_pam.so uams_dhx.so
endif
endif
endif

if USE_DHX2
if HAVE_LIBGCRYPT
uams_LINKS += uams_dhx2.so
if BUILD_PAM
UAMS_DHX2_PAM = uams_dhx2_pam.la
endif
if USE_PAM_SO
dhx2_exec_hook += && $(LN_S) uams_dhx2_pam.so uams_dhx2.so
endif
endif
endif

# these are simple, though some the last three depend on OpenSSL

if BUILD_PAM
UAMS_PAM = uams_pam.la
endif

if USE_GSSAPI
UAMS_GSSAPI = uams_gss.la
endif

# links

if USE_PAM_SO
UAMS_CLRTXT_LINK = uams_pam.so
else
UAMS_CLRTXT_LINK = uams_passwd.so
endif

#
# source files
#

uams_pam_la_SOURCES        = uams_pam.c
uams_dhx_pam_la_SOURCES    = uams_dhx_pam.c
uams_dhx2_pam_la_SOURCES	= uams_dhx2_pam.c
uams_gss_la_SOURCES   = uams_gss.c

noinst_HEADERS = openssl_compat.h

#
# flags
#

# these should be sorted out, applying both to AM_CFLAGS is senseless
AM_CFLAGS = @SSL_CFLAGS@ @LIBGCRYPT_CFLAGS@

uams_pam_la_CFLAGS         = @PAM_CFLAGS@
uams_dhx_pam_la_CFLAGS     = @SSL_CFLAGS@ @PAM_CFLAGS@
uams_dhx2_pam_la_CFLAGS    = @LIBGCRYPT_CFLAGS@ @PAM_CFLAGS@
uams_gss_la_CFLAGS   	   = @GSSAPI_CFLAGS@ @KRB5_CFLAGS@

uams_pam_la_LDFLAGS        = -module -avoid-version @PAM_LIBS@
uams_dhx_pam_la_LDFLAGS		= -module -avoid-version @SSL_LIBS@ @PAM_LIBS@
uams_dhx2_pam_la_LDFLAGS	= -module -avoid-version @LIBGCRYPT_LIBS@ @PAM_LIBS@
uams_gss_la_LDFLAGS   	   = -module -avoid-version @GSSAPI_LIBS@ @KRB5_LIBS@

#
# module compilation
#

uamsdir = @UAMS_PATH@
uams_LTLIBRARIES =		\
	$(UAMS_PAM)		\
	$(UAMS_DHX)		\
	$(UAMS_DHX_PAM)		\
	$(UAMS_DHX2)		\
	$(UAMS_DHX2_PAM)	\
	$(UAMS_GSSAPI)

#
# link creation
#

install-data-hook:
	(if [ -d $(DESTDIR)$(uamsdir) ] ; then \
		cd $(DESTDIR)$(uamsdir)					&& \
		rm -f $(uams_LINKS)  					&& \
		$(dhx_exec_hook)						&& \
		$(dhx2_exec_hook) ; 					\
	fi)



uninstall-hook:
	(if [ -d $(DESTDIR)$(uamsdir) ] ; then \
		cd $(DESTDIR)$(uamsdir)			&& \
		rm -f $(uams_LINKS)	;		   	\
	fi)
