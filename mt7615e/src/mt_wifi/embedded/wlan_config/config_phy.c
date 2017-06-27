 /***************************************************************************
 * MediaTek Inc.
 * 4F, No. 2 Technology 5th Rd.
 * Science-based Industrial Park
 * Hsin-chu, Taiwan, R.O.C.
 *
 * (c) Copyright 1997-2012, MediaTek, Inc.
 *
 * All rights reserved. MediaTek source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek Technology, Inc. is obtained.
 ***************************************************************************

*/

#include "rt_config.h"
#include "wlan_config/config_internal.h"


/*
* define  constructor & deconstructor & method
*/
/*
*basic phy related
*/
VOID phy_cfg_init(struct phy_cfg *obj)
{
}

VOID phy_cfg_exit(struct phy_cfg *obj)
{
	os_zero_mem(obj,sizeof(struct phy_cfg));
}

/*
* Operater loader
*/


/*
* export function
*/
/*
* configure functio
*/


