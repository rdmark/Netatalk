/*
 * $Id: cnid_tdb_nextid.c,v 1.2 2005-04-28 20:50:02 bfernhomberg Exp $
 */

#include "config.h"

#ifdef CNID_BACKEND_TDB

#include "cnid_tdb.h"

cnid_t cnid_tdb_nextid(struct _cnid_db *cdb)
{
    return CNID_INVALID;
}

#endif 
