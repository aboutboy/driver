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
*ht phy info related
*/
VOID ht_cfg_init(struct ht_cfg * obj)
{
	/*initial ht_phy_info value*/
	obj->ht_bw = HT_BW_20;
	obj->ext_cha = EXTCHA_NOASSIGN;
}

VOID ht_cfg_exit(struct ht_cfg * obj)
{
	os_zero_mem(obj,sizeof(struct ht_cfg));
}

/*
* ht operate related
*/
VOID ht_op_status_init(struct ht_op_status *obj)
{	

}

VOID ht_op_status_exit(struct ht_op_status *obj)
{
	os_zero_mem(obj,sizeof(struct ht_op_status));
}

/*
*  export configure function
*/
/*
*Set
*/
VOID wlan_config_set_ht_bw(struct wifi_dev *wdev,UCHAR ht_bw)
{
	struct wlan_config *cfg = (struct wlan_config*)wdev->wpf_cfg;
	cfg->ht_conf.ht_bw = ht_bw;
	/*configure loader*/
	config_loader_ht_bw((void*)wdev,cfg);
}

VOID wlan_config_set_ht_bw_all(struct wpf_ctrl *ctrl,UCHAR ht_bw)
{
	struct wlan_config *cfg;
	struct wlan_operate *op;
	unsigned int i;
	for(i=0;i<WDEV_NUM_MAX;i++){
		cfg = (struct wlan_config*)ctrl->pf[i].conf;
		cfg->ht_conf.ht_bw = ht_bw;
		op = (struct wlan_operate*)ctrl->pf[i].oper;
		op->ht_oper.ht_bw = ht_bw;
		operate_loader_bw(op);
	}
}

VOID wlan_config_set_ext_cha(struct wifi_dev *wdev, UCHAR ext_cha)
{
	struct wlan_config *cfg = (struct wlan_config*)wdev->wpf_cfg;
	cfg->ht_conf.ext_cha = ext_cha;
	config_loader_ext_cha(wdev,cfg);
}

/*
*Get
*/
UCHAR wlan_config_get_ht_bw(struct wifi_dev *wdev)
{
	struct wlan_config *cfg = (struct wlan_config*)wdev->wpf_cfg;
	return cfg->ht_conf.ht_bw;
}

UCHAR wlan_config_get_ext_cha(struct wifi_dev *wdev)
{
	struct wlan_config *cfg = (struct wlan_config*)wdev->wpf_cfg;
	return cfg->ht_conf.ext_cha;
}
