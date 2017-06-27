/* *
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

#ifndef __CONFIG_EXPORT_H__
#define __CONFIG_EXPORT_H__

struct _RTMP_ADAPTER;

struct wpf_data {
	UCHAR idx;
	void *dev;
	void *conf;
	void *oper;
};

struct wpf_ctrl{
	struct wpf_data pf[WDEV_NUM_MAX];
};

struct phy_cfg{

};

struct ht_cfg {
	UCHAR 	ext_cha;
	UCHAR	ht_bw;
};

struct vht_cfg {
	UCHAR vht_bw;
};


/*for profile usage*/
VOID wpf_init(struct _RTMP_ADAPTER *ad);
VOID wpf_exit(struct _RTMP_ADAPTER *ad);
VOID wpf_config_exit(struct _RTMP_ADAPTER *ad);
VOID wpf_config_init(struct _RTMP_ADAPTER *ad);

/*
* Configure Get
*/
UCHAR wlan_config_get_ht_bw(struct wifi_dev *wdev);
UCHAR wlan_config_get_ext_cha(struct wifi_dev *wdev);
UCHAR wlan_config_get_vht_bw(struct wifi_dev *wdev);
UCHAR wlan_config_get_ext_cha(struct wifi_dev *wdev);

/*
* Configure Set
*/
VOID wlan_config_set_ht_bw(struct wifi_dev *wdev,UCHAR ht_bw);
VOID wlan_config_set_ht_bw_all(struct wpf_ctrl *ctrl,UCHAR ht_bw);
VOID wlan_config_set_ht_ext_cha(struct wifi_dev *wdev,UCHAR ext_cha);
VOID wlan_config_set_ht_ext_cha_all(struct wpf_ctrl *ctrl,UCHAR ext_cha);
VOID wlan_config_set_vht_bw(struct wifi_dev *wdev, UCHAR vht_bw);
VOID wlan_config_set_vht_bw_all(struct wpf_ctrl *ctrl,UCHAR vht_bw);
VOID wlan_config_set_ext_cha(struct wifi_dev *wdev, UCHAR ext_cha);


#endif
