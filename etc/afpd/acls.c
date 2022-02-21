/*
  Copyright (c) 2008, 2009, 2010 Frank Lahm <franklahm@gmail.com>
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

#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <stdlib.h>
#include <grp.h>
#include <pwd.h>
#include <errno.h>
#ifdef HAVE_ACL_LIBACL_H
#include <acl/libacl.h>
#endif

#include <atalk/errchk.h>
#include <atalk/adouble.h>
#include <atalk/vfs.h>
#include <atalk/afp.h>
#include <atalk/util.h>
#include <atalk/cnid.h>
#include <atalk/logger.h>
#include <atalk/uuid.h>
#include <atalk/acl.h>
#include <atalk/bstrlib.h>
#include <atalk/bstradd.h>

#include "directory.h"
#include "desktop.h"
#include "volume.h"
#include "fork.h"
#include "unix.h"
#include "acls.h"
#include "acl_mappings.h"
#include "auth.h"

/* for map_acl() */
#define SOLARIS_2_DARWIN       1
#define DARWIN_2_SOLARIS       2
#define POSIX_DEFAULT_2_DARWIN 3
#define POSIX_ACCESS_2_DARWIN  4
#define DARWIN_2_POSIX_DEFAULT 5
#define DARWIN_2_POSIX_ACCESS  6

#define MAP_MASK               31
#define IS_DIR                 32

/* bit flags for set_acl() and map_aces_darwin_to_posix() */
#define HAS_DEFAULT_ACL 0x01
#define HAS_EXT_DEFAULT_ACL 0x02

/********************************************************
 * Solaris funcs
 ********************************************************/


/********************************************************
 * POSIX 1e funcs
 ********************************************************/


/*
 * Multiplex ACL mapping (SOLARIS_2_DARWIN, DARWIN_2_SOLARIS, POSIX_2_DARWIN, DARWIN_2_POSIX).
 * Reads from 'aces' buffer, writes to 'rbuf' buffer.
 * Caller must provide buffer.
 * Darwin ACEs are read and written in network byte order.
 * Needs to know how many ACEs are in the ACL (ace_count) for Solaris ACLs.
 * Ignores trivial ACEs.
 * Return no of mapped ACEs or -1 on error.
 */
static int map_acl(int type, void *acl, darwin_ace_t * buf, int ace_count)
{
	int mapped_aces;

	LOG(log_debug9, logtype_afpd, "map_acl: BEGIN");

	switch (type & MAP_MASK) {



	default:
		mapped_aces = -1;
		break;
	}

	LOG(log_debug9, logtype_afpd, "map_acl: END");
	return mapped_aces;
}

/* Get ACL from object omitting trivial ACEs. Map to Darwin ACL style and
   store Darwin ACL at rbuf. Add length of ACL written to rbuf to *rbuflen.
   Returns 0 on success, -1 on error. */
static int get_and_map_acl(char *name, char *rbuf, size_t *rbuflen)
{
    EC_INIT;
    int mapped_aces = 0;
    int dirflag;
    uint32_t *darwin_ace_count = (u_int32_t *)rbuf;
	LOG(log_debug9, logtype_afpd, "get_and_map_acl: BEGIN");

	/* Skip length and flags */
	rbuf += 4;
	*rbuf = 0;
	rbuf += 4;

    LOG(log_debug, logtype_afpd, "get_and_map_acl: mapped %d ACEs", mapped_aces);

    *rbuflen += sizeof(darwin_acl_header_t) + (mapped_aces * sizeof(darwin_ace_t));
    mapped_aces = htonl(mapped_aces);
    memcpy(darwin_ace_count, &mapped_aces, sizeof(uint32_t));

    EC_STATUS(0);

      EC_CLEANUP:
	LOG(log_debug9, logtype_afpd, "get_and_map_acl: END");

	EC_EXIT;
}

/* Removes all non-trivial ACLs from object. Returns full AFPERR code. */
static int remove_acl(const struct vol *vol, const char *path, int dir)
{
	int ret = AFP_OK;

	return ret;
}

/*
  Set ACL. Subtleties:
  - the client sends a complete list of ACEs, not only new ones. So we dont need to do
  any combination business (one exception being 'kFileSec_Inherit': see next)
  - client might request that we add inherited ACEs via 'kFileSec_Inherit'.
  We will store inherited ACEs first, which is Darwins canonical order.
  - returns AFPerror code
*/


/*!
 * Checks if a given UUID has requested_rights(type darwin_ace_rights) for path.
 *
 * Note: this gets called frequently and is a good place for optimizations !
 *
 * @param vol              (r) volume
 * @param dir              (rw) directory
 * @param path             (r) path to filesystem object
 * @param uuid             (r) UUID of user
 * @param requested_rights (r) requested Darwin ACE
 *
 * @returns                    AFP result code
*/
static int check_acl_access(const struct vol *vol,
			    struct dir *dir,
			    const char *path,
			    const uuidp_t uuid, uint32_t requested_rights)
{
	int ret;
	uint32_t allowed_rights = 0;
	char *username = NULL;
	uuidtype_t uuidtype;
	struct stat st;
	bstring parent = NULL;
	int is_dir;

	LOG(log_maxdebug, logtype_afpd,
	    "check_acl_access(dir: \"%s\", path: \"%s\", curdir: \"%s\", 0x%08x)",
	    cfrombstr(dir->d_fullpath), path, getcwdpath(),
	    requested_rights);

	EC_ZERO_LOG_ERR(ostat(path, &st, vol_syml_opt(vol)), AFPERR_PARAM);

	is_dir = !strcmp(path, ".");

	if (is_dir && (curdir->d_rights_cache != 0xffffffff)) {
		/* its a dir and the cache value is valid */
		allowed_rights = curdir->d_rights_cache;
		LOG(log_debug, logtype_afpd,
		    "check_access: allowed rights from dircache: 0x%08x",
		    allowed_rights);
	} else {
		/*
		 * The DARWIN_ACE_DELETE right might implicitly result from write acces to the parent
		 * directory. As it seems the 10.6 AFP client is puzzled when this right is not
		 * allowed where a delete would succeed because the parent dir gives write perms.
		 * So we check the parent dir for write access and set the right accordingly.
		 * Currentyl acl2ownermode calls us with dir = NULL, because it doesn't make sense
		 * there to do this extra check -- afaict.
		 */
		if (vol && dir && (requested_rights & DARWIN_ACE_DELETE)) {
			int i;
			uint32_t parent_rights = 0;

			if (curdir->d_did == DIRDID_ROOT_PARENT) {
				/* use volume path */
				EC_NULL_LOG_ERR(parent =
						bfromcstr(vol->v_path),
						AFPERR_MISC);
			} else {
				/* build path for parent */
				EC_NULL_LOG_ERR(parent =
						bstrcpy(curdir->
							d_fullpath),
						AFPERR_MISC);
				EC_ZERO_LOG_ERR(bconchar(parent, '/'),
						AFPERR_MISC);
				EC_ZERO_LOG_ERR(bcatcstr(parent, path),
						AFPERR_MISC);
				EC_NEG1_LOG_ERR(i =
						bstrrchr(parent, '/'),
						AFPERR_MISC);
				EC_ZERO_LOG_ERR(binsertch(parent, i, 1, 0),
						AFPERR_MISC);
			}

			LOG(log_debug, logtype_afpd, "parent: %s",
			    cfrombstr(parent));
			EC_ZERO_LOG_ERR(lstat(cfrombstr(parent), &st),
					AFPERR_MISC);

			if (parent_rights &
			    (DARWIN_ACE_WRITE_DATA |
			     DARWIN_ACE_DELETE_CHILD))
				allowed_rights |= DARWIN_ACE_DELETE;	/* man, that was a lot of work! */
		}

		if (is_dir) {
			/* Without DARWIN_ACE_DELETE set OS X 10.6 refuses to rename subdirectories in a
			 * directory.
			 */
			if (allowed_rights & DARWIN_ACE_ADD_SUBDIRECTORY)
				allowed_rights |= DARWIN_ACE_DELETE;

			curdir->d_rights_cache = allowed_rights;
		}
		LOG(log_debug, logtype_afpd, "allowed rights: 0x%08x",
		    allowed_rights);
	}

	if ((requested_rights & allowed_rights) != requested_rights) {
		LOG(log_debug, logtype_afpd,
		    "some requested right wasn't allowed: 0x%08x / 0x%08x",
		    requested_rights, allowed_rights);
		EC_STATUS(AFPERR_ACCESS);
	} else {
		LOG(log_debug, logtype_afpd,
		    "all requested rights are allowed: 0x%08x",
		    requested_rights);
		EC_STATUS(AFP_OK);
	}

      EC_CLEANUP:
	if (username)
		free(username);
	if (parent)
		bdestroy(parent);

	EC_EXIT;
}

/********************************************************
 * Interface
 ********************************************************/

int afp_access(AFPObj * obj, char *ibuf, size_t ibuflen _U_,
	       char *rbuf _U_, size_t *rbuflen)
{
	int ret;
	struct vol *vol;
	struct dir *dir;
	uint32_t did, darwin_ace_rights;
	uint16_t vid;
	struct path *s_path;
	uuidp_t uuid;

	*rbuflen = 0;
	ibuf += 2;

	memcpy(&vid, ibuf, sizeof(vid));
	ibuf += sizeof(vid);
	if (NULL == (vol = getvolbyvid(vid))) {
		LOG(log_error, logtype_afpd,
		    "afp_access: error getting volid:%d", vid);
		return AFPERR_NOOBJ;
	}

	memcpy(&did, ibuf, sizeof(did));
	ibuf += sizeof(did);
	if (NULL == (dir = dirlookup(vol, did))) {
		LOG(log_error, logtype_afpd,
		    "afp_access: error getting did:%d", did);
		return afp_errno;
	}

	/* Skip bitmap */
	ibuf += 2;

	/* Store UUID address */
	uuid = (uuidp_t) ibuf;
	ibuf += UUID_BINSIZE;

	/* Store ACE rights */
	memcpy(&darwin_ace_rights, ibuf, 4);
	darwin_ace_rights = ntohl(darwin_ace_rights);
	ibuf += 4;

	/* get full path and handle file/dir subtleties in netatalk code */
	if (NULL == (s_path = cname(vol, dir, &ibuf))) {
		LOG(log_error, logtype_afpd,
		    "afp_getacl: cname got an error!");
		return AFPERR_NOOBJ;
	}
	if (!s_path->st_valid)
		of_statdir(vol, s_path);
	if (s_path->st_errno != 0) {
		LOG(log_error, logtype_afpd, "afp_getacl: cant stat");
		return AFPERR_NOOBJ;
	}

	ret =
	    check_acl_access(vol, dir, s_path->u_name, uuid,
			     darwin_ace_rights);

	return ret;
}

int afp_getacl(AFPObj * obj, char *ibuf, size_t ibuflen _U_,
	       char *rbuf _U_, size_t *rbuflen)
{
	struct vol *vol;
	struct dir *dir;
	int ret;
	uint32_t did;
	uint16_t vid, bitmap;
	struct path *s_path;
	struct passwd *pw;
	struct group *gr;

	LOG(log_debug9, logtype_afpd, "afp_getacl: BEGIN");
	*rbuflen = 0;
	ibuf += 2;

	memcpy(&vid, ibuf, sizeof(vid));
	ibuf += sizeof(vid);
	if (NULL == (vol = getvolbyvid(vid))) {
		LOG(log_error, logtype_afpd,
		    "afp_getacl: error getting volid:%d", vid);
		return AFPERR_NOOBJ;
	}

	memcpy(&did, ibuf, sizeof(did));
	ibuf += sizeof(did);
	if (NULL == (dir = dirlookup(vol, did))) {
		LOG(log_error, logtype_afpd,
		    "afp_getacl: error getting did:%d", did);
		return afp_errno;
	}

	memcpy(&bitmap, ibuf, sizeof(bitmap));
	memcpy(rbuf, ibuf, sizeof(bitmap));
	bitmap = ntohs(bitmap);
	ibuf += sizeof(bitmap);
	rbuf += sizeof(bitmap);
	*rbuflen += sizeof(bitmap);

	/* skip maxreplysize */
	ibuf += 4;

	/* get full path and handle file/dir subtleties in netatalk code */
	if (NULL == (s_path = cname(vol, dir, &ibuf))) {
		LOG(log_error, logtype_afpd,
		    "afp_getacl: cname got an error!");
		return AFPERR_NOOBJ;
	}
	if (!s_path->st_valid)
		of_statdir(vol, s_path);
	if (s_path->st_errno != 0) {
		LOG(log_error, logtype_afpd, "afp_getacl: cant stat");
		return AFPERR_NOOBJ;
	}

	/* Shall we return owner UUID ? */
	if (bitmap & kFileSec_UUID) {
		LOG(log_debug, logtype_afpd,
		    "afp_getacl: client requested files owner user UUID");
		if (NULL == (pw = getpwuid(s_path->st.st_uid))) {
			LOG(log_debug, logtype_afpd,
			    "afp_getacl: local uid: %u",
			    s_path->st.st_uid);
			localuuid_from_id(rbuf, UUID_USER,
					  s_path->st.st_uid);
		} else {
			LOG(log_debug, logtype_afpd,
			    "afp_getacl: got uid: %d, name: %s",
			    s_path->st.st_uid, pw->pw_name);
			if ((ret =
			     getuuidfromname(pw->pw_name, UUID_USER,
					     rbuf)) != 0)
				return AFPERR_MISC;
		}
		rbuf += UUID_BINSIZE;
		*rbuflen += UUID_BINSIZE;
	}

	/* Shall we return group UUID ? */
	if (bitmap & kFileSec_GRPUUID) {
		LOG(log_debug, logtype_afpd,
		    "afp_getacl: client requested files owner group UUID");
		if (NULL == (gr = getgrgid(s_path->st.st_gid))) {
			LOG(log_debug, logtype_afpd,
			    "afp_getacl: local gid: %u",
			    s_path->st.st_gid);
			localuuid_from_id(rbuf, UUID_GROUP,
					  s_path->st.st_gid);
		} else {
			LOG(log_debug, logtype_afpd,
			    "afp_getacl: got gid: %d, name: %s",
			    s_path->st.st_gid, gr->gr_name);
			if ((ret =
			     getuuidfromname(gr->gr_name, UUID_GROUP,
					     rbuf)) != 0)
				return AFPERR_MISC;
		}
		rbuf += UUID_BINSIZE;
		*rbuflen += UUID_BINSIZE;
	}

	/* Shall we return ACL ? */
	if (bitmap & kFileSec_ACL) {
		LOG(log_debug, logtype_afpd,
		    "afp_getacl: client requested files ACL");
		if (get_and_map_acl(s_path->u_name, rbuf, rbuflen) != 0) {
			LOG(log_error, logtype_afpd,
			    "afp_getacl(\"%s/%s\"): mapping error",
			    getcwdpath(), s_path->u_name);
			return AFPERR_MISC;
		}
	}

	LOG(log_debug9, logtype_afpd, "afp_getacl: END");
	return AFP_OK;
}

int afp_setacl(AFPObj * obj, char *ibuf, size_t ibuflen _U_,
	       char *rbuf _U_, size_t *rbuflen)
{
	struct vol *vol;
	struct dir *dir;
	int ret;
	uint32_t did;
	uint16_t vid, bitmap;
	struct path *s_path;

	LOG(log_debug9, logtype_afpd, "afp_setacl: BEGIN");
	*rbuflen = 0;
	ibuf += 2;

	memcpy(&vid, ibuf, sizeof(vid));
	ibuf += sizeof(vid);
	if (NULL == (vol = getvolbyvid(vid))) {
		LOG(log_error, logtype_afpd,
		    "afp_setacl: error getting volid:%d", vid);
		return AFPERR_NOOBJ;
	}

	memcpy(&did, ibuf, sizeof(did));
	ibuf += sizeof(did);
	if (NULL == (dir = dirlookup(vol, did))) {
		LOG(log_error, logtype_afpd,
		    "afp_setacl: error getting did:%d", did);
		return afp_errno;
	}

	memcpy(&bitmap, ibuf, sizeof(bitmap));
	bitmap = ntohs(bitmap);
	ibuf += sizeof(bitmap);

	/* get full path and handle file/dir subtleties in netatalk code */
	if (NULL == (s_path = cname(vol, dir, &ibuf))) {
		LOG(log_error, logtype_afpd,
		    "afp_setacl: cname got an error!");
		return AFPERR_NOOBJ;
	}
	if (!s_path->st_valid)
		of_statdir(vol, s_path);
	if (s_path->st_errno != 0) {
		LOG(log_error, logtype_afpd, "afp_setacl: cant stat");
		return AFPERR_NOOBJ;
	}
	LOG(log_debug, logtype_afpd, "afp_setacl: unixname: %s",
	    s_path->u_name);

	/* Padding? */
	if ((unsigned long) ibuf & 1)
		ibuf++;

	/* Start processing request */

	/* Change owner: dont even try */
	if (bitmap & kFileSec_UUID) {
		LOG(log_note, logtype_afpd,
		    "afp_setacl: change owner request, discarded");
		ret = AFPERR_ACCESS;
		ibuf += UUID_BINSIZE;
	}

	/* Change group: certain changes might be allowed, so try it. FIXME: not implemented yet. */
	if (bitmap & kFileSec_UUID) {
		LOG(log_note, logtype_afpd,
		    "afp_setacl: change group request, not supported");
		ret = AFPERR_PARAM;
		ibuf += UUID_BINSIZE;
	}

	/* Remove ACL ? */
	if (bitmap & kFileSec_REMOVEACL) {
		LOG(log_debug, logtype_afpd,
		    "afp_setacl: Remove ACL request.");
		if ((ret =
		     remove_acl(vol, s_path->u_name,
				S_ISDIR(s_path->st.st_mode))) != AFP_OK)
			LOG(log_error, logtype_afpd,
			    "afp_setacl: error from remove_acl");
	}

	/* Change ACL ? */
	if (bitmap & kFileSec_ACL) {
		LOG(log_debug, logtype_afpd,
		    "afp_setacl: Change ACL request.");
		/*  Get no of ACEs the client put on the wire */
		uint32_t ace_count;
		memcpy(&ace_count, ibuf, sizeof(uint32_t));
		ace_count = htonl(ace_count);
		ibuf += 8;	/* skip ACL flags (see acls.h) */

		ret = set_acl(vol,
			      s_path->u_name,
			      (bitmap & kFileSec_Inherit),
			      (darwin_ace_t *) ibuf, ace_count);
		if (ret == 0)
			ret = AFP_OK;
		else {
			LOG(log_warning, logtype_afpd,
			    "afp_setacl(\"%s/%s\"): error", getcwdpath(),
			    s_path->u_name);
			ret = AFPERR_MISC;
		}
	}

	LOG(log_debug9, logtype_afpd, "afp_setacl: END");
	return ret;
}

/********************************************************************
 * ACL funcs interfacing with other parts
 ********************************************************************/

/*!
 * map ACL to user maccess
 *
 * This is the magic function that makes ACLs usable by calculating
 * the access granted by ACEs to the logged in user.
 */
int acltoownermode(const struct vol *vol, char *path, struct stat *st,
		   struct maccess *ma)
{
	EC_INIT;
	uint32_t rights = 0;

	if (!(AFPobj->options.flags & OPTION_ACL2MACCESS)
	    || !(vol->v_flags & AFPVOL_ACLS))
		return 0;

	LOG(log_maxdebug, logtype_afpd,
	    "acltoownermode(\"%s/%s\", 0x%02x)", getcwdpath(), path,
	    ma->ma_user);



	LOG(log_maxdebug, logtype_afpd,
	    "resulting user maccess: 0x%02x group maccess: 0x%02x",
	    ma->ma_user, ma->ma_group);

      EC_CLEANUP:
	EC_EXIT;
}

/*!
 * Check whether a volume supports ACLs
 *
 * @param vol  (r) volume
 *
 * @returns        0 if not, 1 if yes
 */
int check_vol_acl_support(const struct vol *vol)
{
	int ret = 0;



	LOG(log_debug, logtype_afpd, "Volume \"%s\" ACL support: %s",
	    vol->v_path, ret ? "yes" : "no");
	return ret;
}
