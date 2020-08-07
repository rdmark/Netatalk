/*
   Copyright (c) 2008,2009 Frank Lahm <franklahm@gmail.com>

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
 
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 */

#ifndef ACL_MAPPINGS
#define ACL_MAPPINGS


#include "acls.h"

/* 
 * Stuff for mapping between ACL implementations
 */

/* Solaris 10u8 still hasn't got ACE_INHERITED_ACE */
#ifndef ACE_INHERITED_ACE
#define ACE_INHERITED_ACE 0x0080
#endif

struct ace_rights_map {
    u_int32_t from;
    u_int32_t to;
};


#endif	/* ACL_MAPPINGS */
