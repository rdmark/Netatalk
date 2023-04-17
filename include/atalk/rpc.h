/*
 * Copyright (c) 2011 Frank Lahm
 * All Rights Reserved.  See COPYRIGHT.
 */

#ifndef ATALK_RPC_H
#define ATALK_RPC_H

#include <config.h>

#include <inttypes.h>
#include <sys/types.h>

#include <atalk/lockrpc.gen.h>

struct adouble;
extern int rpc_init(const char *addr, unsigned short port);
extern int rpc_lock(struct adouble *, uint32_t eid, int type, off_t off, off_t len, int user);
extern void rpc_unlock(struct adouble *, int user);
extern int rpc_tmplock(struct adouble *, uint32_t eid, int type, off_t off, off_t len, int user);

#endif  /* ATALK_RPC_H */
