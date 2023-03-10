#ifndef AFPD_UNIX_H
#define AFPD_UNIX_H

#include "config.h"

#include <arpa/inet.h>
#include <sys/mount.h>
#include <sys/statvfs.h>

#include "volume.h"

#define f_frsize f_bsize

extern struct afp_options default_options;

extern int setdirunixmode   (const struct vol *, char *, mode_t);
extern int setdirmode       (const struct vol *, const char *, mode_t);
extern int setdirowner      (const struct vol *, const char *, const uid_t, const gid_t);
extern int setfilunixmode   (const struct vol *, struct path*, const mode_t);
extern int setfilowner      (const struct vol *, const uid_t, const gid_t, struct path*);
extern void accessmode      (const AFPObj *obj, const struct vol *, char *, struct maccess *, struct dir *, struct stat *);

#endif /* UNIX_H */
