/*
 */

#include "config.h"

#include "cnid_tdb.h"

void cnid_tdb_close(struct _cnid_db *cdb)
{
    struct _cnid_tdb_private *db;

    db = (struct _cnid_tdb_private *)cdb->cnid_db_private;
    tdb_close(db->tdb_cnid);
    free(cdb->cnid_db_private);
    free(cdb);
}
