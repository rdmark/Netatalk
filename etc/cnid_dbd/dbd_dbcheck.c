/*
 *
 * Copyright (C) Joerg Lenneis 2003
 * All Rights Reserved.  See COPYING.
 */

#include "config.h"

#include <string.h>

#include <atalk/logger.h>

#include "dbd.h"
#include "dbif.h"

int dbd_check_indexes(DBD *dbd, char *dbdir)
{
    u_int32_t c_didname = 0, c_devino = 0, c_cnid = 0;

    LOG(log_note, logtype_cnid, "CNID database at `%s' is being checked (quick)", dbdir);

    if (dbif_count(dbd, DBIF_CNID, &c_cnid)) 
        return -1;

    if (dbif_count(dbd, DBIF_IDX_DEVINO, &c_devino))
        return -1;

    /* bailout after the first error */
    if ( c_cnid != c_devino) {
        LOG(log_error, logtype_cnid, "CNID database at `%s' corrupted (%u/%u)", dbdir, c_cnid, c_devino);
        return 1;
    }

    if (dbif_count(dbd, DBIF_IDX_DIDNAME, &c_didname)) 
        return -1;
    
    if ( c_cnid != c_didname) {
        LOG(log_error, logtype_cnid, "CNID database at `%s' corrupted (%u/%u)", dbdir, c_cnid, c_didname);
        return 1;
    }

    LOG(log_note, logtype_cnid, "CNID database at `%s' seems ok, %u entries.", dbdir, c_cnid);
    return 0;  
}


