/*
 * $Id: module.c,v 1.5 2003-02-17 02:03:12 srittau Exp $
 */

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <atalk/util.h>

#ifndef HAVE_DLFCN_H

#else /* HAVE_DLFCN_H */

#include <dlfcn.h>

#ifdef DLSYM_PREPEND_UNDERSCORE
void *mod_symbol(void *module, const char *name)
{
   void *symbol;
   char *underscore;

   if (!module)
     return NULL;

   if ((underscore = (char *) malloc(strlen(name) + 2)) == NULL)
     return NULL;

   strcpy(underscore, "_");
   strcat(underscore, name);
   symbol = dlsym(module, underscore);
   free(underscore);

   return symbol;
}
#endif /* DLSYM_PREPEND_UNDERSCORE */
#endif /* HAVE_DLFCN_H */
