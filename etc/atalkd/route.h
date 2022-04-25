/*
 * $Id: route.h,v 1.3 2009-10-13 22:55:37 didg Exp $
 */

#ifndef ATALKD_ROUTE_H
#define ATALKD_ROUTE_H 1

#include <sys/types.h>

#ifndef __NetBSD__
int route ( int, struct sockaddr *, struct sockaddr *, int );
#else /* __NetBSD__ */
int route ( int, struct sockaddr_at *, struct sockaddr_at *, int);
#endif /* __NetBSD__ */

#endif /* ATALKD_ROUTE_H */
