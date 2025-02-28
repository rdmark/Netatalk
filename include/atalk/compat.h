/*
 * Copyright (c) 1996 Regents of The University of Michigan.
 * All Rights Reserved.  See COPYRIGHT.
 *
 * NOTE: SunOS 4 and ultrix are pretty much the only reason why there
 * are checks for EINTR everywhere. 
 */

#include <sys/types.h>
#include <signal.h>

#ifdef __svr4__
/*
 * SunOS 5 (solaris) has SA_RESTART, but no SA_INTERRUPT.
 */
#ifndef SA_INTERRUPT
#define SA_INTERRUPT	0
#endif
#endif /* __svr4__ */

#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

extern int flock (int, int);
extern int inet_aton (const char *, struct in_addr *);

#ifdef linux
/*
 * Linux has SA_RESTART, but no SA_INTERRUPT.  Note that the documentation
 * seems to be wrong on several counts.  First, SA_ONESHOT is not the default,
 * and second, SA_RESTART does what you'd expect (the same as svr4) and has
 * nothing to do with SA_ONESHOT.
 */
#ifndef SA_INTERRUPT
#define SA_INTERRUPT	0
#endif /* SA_INTERRUPT */
#endif /* linux */

#ifdef __NetBSD__
#ifndef SA_INTERRUPT
#define SA_INTERRUPT	0
#endif
#endif /* __NetBSD__ */

#if defined(NEED_GETUSERSHELL)
extern char *getusershell (void);
#endif

#if !defined(HAVE_SNPRINTF) || !defined(HAVE_VSNPRINTF)
#include <stdio.h>
#include <stdarg.h>
#endif

#ifndef HAVE_SNPRINTF
int snprintf (char *str,size_t count,const char *fmt,...);
#endif

#ifndef HAVE_VSNPRINTF
int vsnprintf(char *str, size_t count, const char *fmt, va_list args);
#endif

/* OpenBSD */
#if defined(__OpenBSD__) && !defined(ENOTSUP)
#define ENOTSUP EOPNOTSUPP
#endif

#if !defined(HAVE_PSELECT) || defined(__OpenBSD__)
extern int pselect(int, fd_set * restrict, fd_set * restrict,
                   fd_set * restrict, const struct timespec * restrict,
                   const sigset_t * restrict);
#endif
