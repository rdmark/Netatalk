/* -*- Mode: C; tab-width: 2; indent-tabs-mode: t; c-basic-offset: 2 -*- */
/*
 * Author:  Daniel S. Haischt <me@daniel.stefan.haischt.name>
 * Purpose: Zeroconf facade, that abstracts access to a
 *          particular Zeroconf implementation
 * Doc:     http://www.dns-sd.org/
 *
 */

#include <config.h>

#include "afp_zeroconf.h"
#include "afp_mdns.h"

/*
 * Functions (actually they are just facades)
 */
void zeroconf_register(const AFPObj *configs)
{
  LOG(log_debug, logtype_afpd, "Attempting to register with mDNS using mDNSResponder");

	md_zeroconf_register(configs);
}

void zeroconf_deregister(void)
{
  LOG(log_debug, logtype_afpd, "Attempting to de-register mDNS using mDNSResponder");
	md_zeroconf_unregister();
}
