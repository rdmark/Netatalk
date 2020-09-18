/*
  Copyright (c) 2009 Frank Lahm <franklahm@gmail.com>
  Copyright (c) 2010 Frank Lahm <franklahm@gmail.com>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
*/

#include "config.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include <atalk/afp.h>
#include <atalk/util.h>
#include <atalk/logger.h>
#include <atalk/errchk.h>
#include <atalk/acl.h>


#ifdef HAVE_POSIX_ACLS
/*!
 * Remove any ACL_USER, ACL_GROUP, ACL_MASK or ACL_TYPE_DEFAULT ACEs from an object
 *
 * @param name  (r) filesystem object name
 *
 * @returns AFP error code, AFP_OK (= 0) on success, AFPERR_MISC on error
 */
int remove_acl_vfs(const char *name)
{
    EC_INIT;

    struct stat st;
    acl_t acl = NULL;
    acl_entry_t e;
    acl_tag_t tag;
    int entry_id = ACL_FIRST_ENTRY;


    /* Remove default ACL if it's a dir */
    EC_ZERO_LOG_ERR(stat(name, &st), AFPERR_MISC);
    if (S_ISDIR(st.st_mode)) {
        EC_NULL_LOG_ERR(acl = acl_init(0), AFPERR_MISC);
        EC_ZERO_LOG_ERR(acl_set_file(name, ACL_TYPE_DEFAULT, acl), AFPERR_MISC);
        EC_ZERO_LOG_ERR(acl_free(acl), AFPERR_MISC);
        acl = NULL;
    }

    /* Now get ACL and remove ACL_MASK, ACL_USER or ACL_GROUP entries, then re-set
     * the ACL again. acl_calc_mask() must not be called because there is no need
     * for an ACL_MASK entry in a basic ACL. */
    EC_NULL_LOG_ERR(acl = acl_get_file(name, ACL_TYPE_ACCESS), AFPERR_MISC);
    for ( ; acl_get_entry(acl, entry_id, &e) == 1; entry_id = ACL_NEXT_ENTRY) {
        EC_ZERO_LOG_ERR(acl_get_tag_type(e, &tag), AFPERR_MISC);
        if (tag == ACL_USER || tag == ACL_GROUP || tag == ACL_MASK)
            EC_ZERO_LOG_ERR(acl_delete_entry(acl, e), AFPERR_MISC);
    }
    EC_ZERO_LOG_ERR(acl_valid(acl), AFPERR_MISC);
    EC_ZERO_LOG_ERR(acl_set_file(name, ACL_TYPE_ACCESS, acl), AFPERR_MISC);

EC_CLEANUP:
    if (acl) acl_free(acl);

    EC_EXIT;
}
#endif /* HAVE_POSIX_ACLS */
