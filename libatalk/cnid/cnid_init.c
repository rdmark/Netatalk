
/* 
 *
 * Copyright (c) 2003 the Netatalk Team
 * Copyright (c) 2003 Rafal Lewczuk <rlewczuk@pronet.pl>
 * 
 * This program is free software; you can redistribute and/or modify
 * it under the terms of the GNU General Public License as published
 * by the Free Software Foundation version 2 of the License or later
 * version if explicitly stated by any of above copyright holders.
 *
 */

/*
 * This file contains initialization stuff for CNID backends.
 * Currently it only employs static bindings. 
 * No plans for dynamically loaded CNID backends here (temporary). 
 * Maybe somewhere in the future.
 */

#include "config.h"

#include <atalk/cnid.h>

#ifdef CNID_BACKEND_LAST
extern struct _cnid_module cnid_last_module;
#endif

#ifdef CNID_BACKEND_DBD
extern struct _cnid_module cnid_dbd_module;
#endif

#ifdef CNID_BACKEND_MYSQL
extern struct _cnid_module cnid_mysql_module;
#endif

void cnid_init(void)
{
#ifdef CNID_BACKEND_LAST
    cnid_register(&cnid_last_module);
#endif

#ifdef CNID_BACKEND_DBD
    cnid_register(&cnid_dbd_module);
#endif

#ifdef CNID_BACKEND_MYSQL
    cnid_register(&cnid_mysql_module);
#endif
}
