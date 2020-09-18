/*
  Copyright (c) 2010 Frank Lahm <franklahm@gmail.com>
  Copyright (c) 2011 Laura Mueller <laura-mueller@uni-duesseldorf.de>

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

#ifdef HAVE_ACLS

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <sys/acl.h>

#include <atalk/logger.h>
#include <atalk/afp.h>
#include <atalk/util.h>
#include <atalk/acl.h>
#include <atalk/unix.h>


#ifdef HAVE_POSIX_ACLS

/* This is a workaround for chmod() on filestystems supporting Posix 1003.1e draft 17
 * compliant ACLs. For objects with extented ACLs, eg objects with an ACL_MASK entry,
 * chmod() manipulates ACL_MASK instead of ACL_GROUP_OBJ. As OS X isn't aware of
 * this behavior calling FPSetFileDirParms may lead to unpredictable results. For
 * more information see section 23.1.2 of Posix 1003.1e draft 17.
 *
 * posix_chmod() accepts the same arguments as chmod() and returns 0 in case of
 * success or -1 in case something went wrong.
 */

#define SEARCH_GROUP_OBJ 0x01
#define SEARCH_MASK 0x02

int posix_chmod(const char *name, mode_t mode)
{
	int ret = 0;
	int entry_id = ACL_FIRST_ENTRY;
	acl_entry_t entry;
	acl_entry_t group_entry;
	acl_tag_t tag;
	acl_t acl;
	u_char not_found = (SEARCH_GROUP_OBJ | SEARCH_MASK);	/* used as flags */

	LOG(log_maxdebug, logtype_afpd, "posix_chmod: %s mode: 0x%08x",
	    name, mode);

	/* Call chmod() first because there might be some special bits to be set which
	 * aren't related to access control.
	 */
	ret = chmod(name, mode);

	if (ret)
		goto done;

	/* Check if the underlying filesystem supports ACLs. */
	acl = acl_get_file(name, ACL_TYPE_ACCESS);

	if (acl) {
		/* There is no need to keep iterating once we have found ACL_GROUP_OBJ and ACL_MASK. */
		while ((acl_get_entry(acl, entry_id, &entry) == 1)
		       && not_found) {
			entry_id = ACL_NEXT_ENTRY;

			ret = acl_get_tag_type(entry, &tag);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_chmod: Failed to get tag type.");
				goto cleanup;
			}

			switch (tag) {
			case ACL_GROUP_OBJ:
				group_entry = entry;
				not_found &= ~SEARCH_GROUP_OBJ;
				break;

			case ACL_MASK:
				not_found &= ~SEARCH_MASK;
				break;

			default:
				break;
			}
		}
		if (!not_found) {
			/* The filesystem object has extented ACLs. We have to update ACL_GROUP_OBJ
			 * with the group permissions.
			 */
			acl_permset_t permset;
			acl_perm_t perm = 0;

			ret = acl_get_permset(group_entry, &permset);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_chmod: Can't get permset.");
				goto cleanup;
			}
			ret = acl_clear_perms(permset);

			if (ret)
				goto cleanup;

			if (mode & S_IXGRP)
				perm |= ACL_EXECUTE;

			if (mode & S_IWGRP)
				perm |= ACL_WRITE;

			if (mode & S_IRGRP)
				perm |= ACL_READ;

			ret = acl_add_perm(permset, perm);

			if (ret)
				goto cleanup;

			ret = acl_set_permset(group_entry, permset);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_chmod: Can't set permset.");
				goto cleanup;
			}
			/* also update ACL_MASK */
			ret = acl_calc_mask(&acl);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_chmod: acl_calc_mask failed.");
				goto cleanup;
			}
			ret = acl_set_file(name, ACL_TYPE_ACCESS, acl);
		}
	      cleanup:
		acl_free(acl);
	}
      done:
	LOG(log_maxdebug, logtype_afpd, "posix_chmod: %d", ret);
	return ret;
}

/*
 * posix_fchmod() accepts the same arguments as fchmod() and returns 0 in case of
 * success or -1 in case something went wrong.
 */
int posix_fchmod(int fd, mode_t mode)
{
	int ret = 0;
	int entry_id = ACL_FIRST_ENTRY;
	acl_entry_t entry;
	acl_entry_t group_entry;
	acl_tag_t tag;
	acl_t acl;
	u_char not_found = (SEARCH_GROUP_OBJ | SEARCH_MASK);	/* used as flags */

	/* Call chmod() first because there might be some special bits to be set which
	 * aren't related to access control.
	 */
	ret = fchmod(fd, mode);

	if (ret)
		goto done;

	/* Check if the underlying filesystem supports ACLs. */
	acl = acl_get_fd(fd);

	if (acl) {
		/* There is no need to keep iterating once we have found ACL_GROUP_OBJ and ACL_MASK. */
		while ((acl_get_entry(acl, entry_id, &entry) == 1)
		       && not_found) {
			entry_id = ACL_NEXT_ENTRY;

			ret = acl_get_tag_type(entry, &tag);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_fchmod: Failed to get tag type.");
				goto cleanup;
			}

			switch (tag) {
			case ACL_GROUP_OBJ:
				group_entry = entry;
				not_found &= ~SEARCH_GROUP_OBJ;
				break;

			case ACL_MASK:
				not_found &= ~SEARCH_MASK;
				break;

			default:
				break;
			}
		}
		if (!not_found) {
			/* The filesystem object has extented ACLs. We have to update ACL_GROUP_OBJ
			 * with the group permissions.
			 */
			acl_permset_t permset;
			acl_perm_t perm = 0;

			ret = acl_get_permset(group_entry, &permset);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_fchmod: Can't get permset.");
				goto cleanup;
			}
			ret = acl_clear_perms(permset);

			if (ret)
				goto cleanup;

			if (mode & S_IXGRP)
				perm |= ACL_EXECUTE;

			if (mode & S_IWGRP)
				perm |= ACL_WRITE;

			if (mode & S_IRGRP)
				perm |= ACL_READ;

			ret = acl_add_perm(permset, perm);

			if (ret)
				goto cleanup;

			ret = acl_set_permset(group_entry, permset);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_fchmod: Can't set permset.");
				goto cleanup;
			}
			/* also update ACL_MASK */
			ret = acl_calc_mask(&acl);

			if (ret) {
				LOG(log_error, logtype_afpd,
				    "posix_fchmod: acl_calc_mask failed.");
				goto cleanup;
			}
			ret = acl_set_fd(fd, acl);
		}
	      cleanup:
		acl_free(acl);
	}
      done:
	return ret;
}

#endif				/* HAVE_POSIX_ACLS */

#endif				/* HAVE_ACLS */
