/*
 ***************************************************************************
 * MediaTek Inc.
 *
 * All rights reserved. source code is an unpublished work and the
 * use of a copyright notice does not imply otherwise. This source code
 * contains confidential trade secret material of MediaTek. Any attemp
 * or participation in deciphering, decoding, reverse engineering or in any
 * way altering the source code is stricitly prohibited, unless the prior
 * written consent of MediaTek, Inc. is obtained.
 ***************************************************************************

	Module Name:
	ap_band_steering.c
*/

#ifdef BAND_STEERING
#include "rt_config.h"

extern BNDSTRG_OPS D_BndStrgOps;

/* ioctl */
INT Show_BndStrg_Info(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->ShowTableInfo(table);

	return TRUE;	
}


INT Show_BndStrg_List(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;
	
	if (table->Ops)
		table->Ops->ShowTableEntries(P_BND_STRG_TABLE);

	return TRUE;	
}


INT Set_BndStrg_Enable(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;
	BOOLEAN enable = (BOOLEAN) simple_strtol(arg, 0, 10);

	if (table->Ops)
		table->Ops->SetEnable(table, enable);

	return TRUE;
}


INT Set_BndStrg_RssiDiff(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	CHAR RssiDiff = (CHAR) simple_strtol(arg, 0, 10);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetRssiDiff(table, RssiDiff);

	table->RssiDiff = RssiDiff;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): RssiCheck = %u\n", __FUNCTION__, table->RssiDiff));

	return TRUE;
}


INT Set_BndStrg_RssiLow(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	CHAR RssiLow = (CHAR) simple_strtol(arg, 0, 10);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetRssiLow(table, RssiLow);

	table->RssiLow = RssiLow;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): RssiLow = %d\n", __FUNCTION__, table->RssiLow));

	return TRUE;
}


INT Set_BndStrg_Age(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UINT32 AgeTime = (UINT32) simple_strtol(arg, 0, 10);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetAgeTime(table, AgeTime);

	return TRUE;
}


INT Set_BndStrg_HoldTime(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UINT32 HoldTime = (UINT32) simple_strtol(arg, 0, 10);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetHoldTime(table, HoldTime);

	return TRUE;
}


INT Set_BndStrg_CheckTime5G(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UINT32 CheckTime = (UINT32) simple_strtol(arg, 0, 10);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetCheckTime(table, CheckTime);

	return TRUE;
}


INT Set_BndStrg_FrmChkFlag(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UINT32 FrmChkFlag = (UINT32) simple_strtol(arg, 0, 16);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetFrmChkFlag(table, FrmChkFlag);

	return TRUE;
}


INT Set_BndStrg_CndChkFlag(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UINT32 CndChkFlag = (UINT32) simple_strtol(arg, 0, 16);
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops)
		table->Ops->SetCndChkFlag(table, CndChkFlag);

	return TRUE;
}

#ifdef BND_STRG_DBG
INT Set_BndStrg_MonitorAddr(
	PRTMP_ADAPTER	pAd,
	RTMP_STRING		*arg)
{
	UCHAR					MonitorAddr[MAC_ADDR_LEN];
	RTMP_STRING				*value;
	INT						i;
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;


	if(strlen(arg) != 17)  /*Mac address acceptable format 01:02:03:04:05:06 length 17 */
		return FALSE;

	for (i=0, value = rstrtok(arg,":"); value; value = rstrtok(NULL,":")) 
	{
		if((strlen(value) != 2) || (!isxdigit(*value)) || (!isxdigit(*(value+1))) ) 
			return FALSE;  /*Invalid */

		AtoH(value, (UCHAR *)&MonitorAddr[i++], 1);
	}

	if (table->Ops)
		table->Ops->SetMntAddr(table, MonitorAddr);

	return TRUE;
}
#endif /* BND_STRG_DBG */
/**** end of ioctl ****/

INT BndStrg_Init(PRTMP_ADAPTER pAd)
{
	INT ret_val = BND_STRG_SUCCESS;
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, (YLW("%s()\n"), __FUNCTION__));

	ret_val = BndStrg_TableInit(pAd, table);


	if (ret_val != BND_STRG_SUCCESS)
	{
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
					("Error in %s(), error code = %d!\n", __FUNCTION__, ret_val));
	}

	return ret_val;
}


INT BndStrg_TableInit(PRTMP_ADAPTER pAd, PBND_STRG_CLI_TABLE table)
{
	INT ret_val = BND_STRG_SUCCESS;
	struct wifi_dev *wdev = NULL;

	ASSERT(pAd != NULL);

	if (table->bInitialized == TRUE)
		return BND_STRG_SUCCESS;

	wdev = &pAd->ApCfg.MBSSID[BSS0].wdev;
	NdisZeroMemory(table, sizeof(BND_STRG_CLI_TABLE));
	OS_NdisAllocateSpinLock(&table->Lock);


	BndStrg_SetInfFlags(pAd, table, TRUE);

	table->Ops = &D_BndStrgOps;
	table->DaemonPid = 0xffffffff;
	table->RssiDiff = BND_STRG_RSSI_DIFF;
	table->RssiLow = BND_STRG_RSSI_LOW;
	table->AgeTime = BND_STRG_AGE_TIME;
	table->HoldTime = BND_STRG_HOLD_TIME;
	table->CheckTime_5G = BND_STRG_CHECK_TIME_5G;
	table->AutoOnOffThrd = BND_STRG_AUTO_ONOFF_THRD;
	table->AlgCtrl.ConditionCheck = fBND_STRG_CND_5G_RSSI;
	table->AlgCtrl.FrameCheck =  fBND_STRG_FRM_CHK_PRB_REQ | \
								fBND_STRG_FRM_CHK_ATH_REQ;
	table->priv = (VOID *) pAd;
	table->bInitialized = TRUE;

	if (ret_val != BND_STRG_SUCCESS)
	{
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
					("Error in %s(), error code = %d!\n", __FUNCTION__, ret_val));
	}

	return BND_STRG_SUCCESS;
}

INT BndStrg_Release(PRTMP_ADAPTER pAd)
{
	INT ret_val = BND_STRG_SUCCESS;
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, (YLW("%s()\n"), __FUNCTION__));
	BndStrg_SetInfFlags(pAd, table, FALSE);

	if (table->bEnabled == TRUE)
		ret_val = BndStrg_Enable(table, 0);

	if ((table->b2GInfReady == FALSE && table->b5GInfReady == FALSE))
		ret_val = BndStrg_TableRelease(table);
	if (ret_val != BND_STRG_SUCCESS)
	{
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
					("Error in %s(), error code = %d!\n", __FUNCTION__, ret_val));
	}

	return ret_val;
}


INT BndStrg_TableRelease(PBND_STRG_CLI_TABLE table)
{
	INT ret_val = BND_STRG_SUCCESS;

	if (table->bInitialized == FALSE)
		return BND_STRG_NOT_INITIALIZED;
	
	OS_NdisFreeSpinLock(&table->Lock);
	table->bInitialized = FALSE;

	if (ret_val != BND_STRG_SUCCESS)
	{
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
					("Error in %s(), error code = %d!\n", __FUNCTION__, ret_val));
	}

	return ret_val;
}


INT BndStrg_InsertEntry(
	PBND_STRG_CLI_TABLE table,
	PUCHAR pAddr,
	PBND_STRG_CLI_ENTRY *entry_out)
{
	INT i;
	UCHAR HashIdx;
	PBND_STRG_CLI_ENTRY entry = NULL, this_entry = NULL;
	INT ret_val = BND_STRG_SUCCESS;

	if (table->Size >= BND_STRG_MAX_TABLE_SIZE) {
		MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("%s(): Table is full!\n", __FUNCTION__));
		return BND_STRG_TABLE_FULL;
	}

	NdisAcquireSpinLock(&table->Lock);
	for (i = 0; i< BND_STRG_MAX_TABLE_SIZE; i++)
	{
		entry = &table->Entry[i];

		/* pick up the first available vacancy*/
		if (entry->bValid == FALSE)	{
			NdisZeroMemory(entry, sizeof(BND_STRG_CLI_ENTRY));
			/* Fill Entry */
			RTMP_GetCurrentSystemTick(&entry->jiffies);
			COPY_MAC_ADDR(entry->Addr, pAddr);
			entry->bValid = TRUE;
			break;
		}
	}

	if (entry) {
		/* add this MAC entry into HASH table */
		HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
		if (table->Hash[HashIdx] == NULL) {
			table->Hash[HashIdx] = entry;
		} else {
			this_entry = table->Hash[HashIdx];
			while (this_entry->pNext != NULL) {
				this_entry = this_entry->pNext;
			}
			this_entry->pNext = entry;
		}
		
		*entry_out = entry;
		table->Size++;
	}
	NdisReleaseSpinLock(&table->Lock);
	return ret_val;
}


INT BndStrg_DeleteEntry(PBND_STRG_CLI_TABLE table, PUCHAR pAddr, UINT32 Index)
{
	USHORT HashIdx;
	PBND_STRG_CLI_ENTRY entry, pre_entry, this_entry;
	INT ret_val = BND_STRG_SUCCESS;


	NdisAcquireSpinLock(&table->Lock);
	HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
	if (Index >= BND_STRG_MAX_TABLE_SIZE)
	{
		entry = table->Hash[HashIdx];
		while (entry) {
			if (MAC_ADDR_EQUAL(pAddr, entry->Addr)) {
				/* this is the entry we're looking for */
				break;
			} else {
				entry = entry->pNext;
			}
		}

		if (entry == NULL)
		{
			BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_INFO,
				("%s(): Index=%u, %02x:%02x:%02x:%02x:%02x:%02x, "
				"Entry not found.\n",
				__FUNCTION__, Index, PRINT_MAC(pAddr)));
			NdisReleaseSpinLock(&table->Lock);
			return BND_STRG_INVALID_ARG;
		}
	}
	else	
		entry = &table->Entry[Index];
	
	if (entry && entry->bValid) 
	{
		{
			pre_entry = NULL;
			this_entry = table->Hash[HashIdx];
			ASSERT(this_entry);
			if (this_entry != NULL)
			{
				/* update Hash list*/
				do
				{
					if (this_entry == entry)
					{
						if (pre_entry == NULL)
							table->Hash[HashIdx] = entry->pNext;
						else
							pre_entry->pNext = entry->pNext;
						break;
					}

					pre_entry = this_entry;
					this_entry = this_entry->pNext;
				} while (this_entry);
			}

			/* not found !!!*/
			ASSERT(this_entry != NULL);

			NdisZeroMemory(entry->Addr, MAC_ADDR_LEN);
			entry->pNext = NULL;
			entry->bValid = FALSE;
			table->Size--;
		}
	}
	NdisReleaseSpinLock(&table->Lock);

	return ret_val;
}


PBND_STRG_CLI_ENTRY BndStrg_TableLookup(PBND_STRG_CLI_TABLE table, PUCHAR pAddr)
{
	ULONG HashIdx;
	BND_STRG_CLI_ENTRY *entry = NULL;
	
	HashIdx = MAC_ADDR_HASH_INDEX(pAddr);
	entry = table->Hash[HashIdx];

	while (entry && entry->bValid)
	{
		if (MAC_ADDR_EQUAL(entry->Addr, pAddr))
			break;
		else
			entry = entry->pNext;
	}

	return entry;
}

BOOLEAN BndStrg_CheckConnectionReq(
		PRTMP_ADAPTER	pAd,
		struct wifi_dev *wdev,
		PUCHAR pSrcAddr,
		UINT8 FrameType,
		PCHAR Rssi,
		BOOLEAN bAllowStaConnectInHt)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->Ops && (table->bEnabled == TRUE))
	{
		return table->Ops->CheckConnectionReq(
										pAd,
										wdev,
										pSrcAddr,
										FrameType,
										Rssi,
										bAllowStaConnectInHt);
	}
	
	return TRUE;
}


INT BndStrg_Enable(PBND_STRG_CLI_TABLE table, BOOLEAN enable)
{
	BNDSTRG_MSG msg;
	PRTMP_ADAPTER pAd = NULL;

	if (table == NULL)
		return BND_STRG_TABLE_IS_NULL;

	if (table->bInitialized == FALSE)
		return BND_STRG_NOT_INITIALIZED;
	if (!(table->bEnabled ^ enable))
	{
		/* Already enabled/disabled */
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, /* TRACE */
				(GRN("%s(): Band steering is already %s.\n"),
				__FUNCTION__, (enable ? "enabled" : "disabled")));
		return BND_STRG_SUCCESS;
	}

	if (enable)
	{
		table->bEnabled = TRUE;
	}
	else
	{

		table->bEnabled = FALSE;
	}

		pAd = (PRTMP_ADAPTER) table->priv;
		msg.Action = BNDSTRG_ONOFF;
		msg.OnOff = table->bEnabled;
		msg.Band = table->Band;
		RtmpOSWrielessEventSend(
			pAd->net_dev,
			RT_WLAN_EVENT_CUSTOM,
			OID_BNDSTRG_MSG,
			NULL,
			(UCHAR *)&msg,
			sizeof(msg));

	BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
				(GRN("%s(): Band steering %s running.\n"),
				__FUNCTION__, (enable ? "start" : "stop")));

	return BND_STRG_SUCCESS;
}

static INT D_BndStrgSendMsg(
	PRTMP_ADAPTER pAd,
	BNDSTRG_MSG *msg)
{
	return	RtmpOSWrielessEventSend(
				pAd->net_dev,
				RT_WLAN_EVENT_CUSTOM,
				OID_BNDSTRG_MSG,
				NULL,
				(UCHAR *) msg,
				sizeof(BNDSTRG_MSG));
}

INT BndStrg_SetInfFlags(PRTMP_ADAPTER pAd, PBND_STRG_CLI_TABLE table, BOOLEAN bInfReady)
{
	INT ret_val = BND_STRG_SUCCESS;
	struct wifi_dev *wdev = NULL;
	BNDSTRG_MSG msg;
	INT max_mbss_check_num;
	ASSERT(pAd != NULL);

	if (pAd->CommonCfg.dbdc_mode == TRUE) {
		max_mbss_check_num = pAd->ApCfg.BssidNum;
	} else {
		max_mbss_check_num = 1;
	}
	
	if (pAd->CommonCfg.dbdc_mode == TRUE) {
		INT apidx;
		for (apidx = 0; apidx < max_mbss_check_num; apidx++) {
			wdev = &pAd->ApCfg.MBSSID[apidx].wdev;
			if (WMODE_CAP_5G(wdev->PhyMode) &&
					(table->b5GInfReady ^ bInfReady))
			{
				table->b5GInfReady = bInfReady;
				if (bInfReady)
					table->Band |= BAND_5G;
				else
					table->Band &= ~BAND_5G;
				if (wdev->if_dev)
					strcpy(table->uc5GIfName,wdev->if_dev->name);
				table->u5GIdx = apidx;
				msg.Action = INF_STATUS_RSP_5G;
				msg.b5GInfReady = table->b5GInfReady;
				if (wdev->if_dev)
					strcpy(msg.uc5GIfName,wdev->if_dev->name);
				D_BndStrgSendMsg(pAd, &msg);
				BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
								  (BLUE("%s(): BSS (%02x:%02x:%02x:%02x:%02x:%02x)")
								   BLUE(" set 5G Inf %s %s.\n")
								   , __FUNCTION__, PRINT_MAC(wdev->bssid),(wdev->if_dev== NULL) ? "null":wdev->if_dev->name,
								   bInfReady ? "ready" : "not ready"));
			}
			else if (table->b2GInfReady ^ bInfReady)
			{
				table->b2GInfReady = bInfReady;
				if (bInfReady)
					table->Band |= BAND_24G;
				else
					table->Band &= ~BAND_24G;
				if (wdev->if_dev)
					strcpy(table->uc2GIfName,wdev->if_dev->name);
				table->u2GIdx = apidx;
				msg.Action = INF_STATUS_RSP_2G;
				msg.b2GInfReady = table->b2GInfReady;
				if (wdev->if_dev)
					strcpy(msg.uc2GIfName,wdev->if_dev->name);
				D_BndStrgSendMsg(pAd, &msg);
				BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
								  (BLUE("%s(): BSS (%02x:%02x:%02x:%02x:%02x:%02x)")
								   BLUE(" set 2G Inf %s %s.\n")
								   , __FUNCTION__, PRINT_MAC(wdev->bssid),(wdev->if_dev== NULL) ? "null":wdev->if_dev->name,
								   bInfReady ? "ready" : "not ready"));
			}
		}
	}

	return ret_val;
}


BOOLEAN BndStrg_IsClientStay(
			PRTMP_ADAPTER pAd,
			PMAC_TABLE_ENTRY pEntry)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;
	CHAR Rssi = RTMPAvgRssi(pAd, &pEntry->RssiSample);
	//printk("%d,%d,%d,%d   %d\n\r",pEntry->RssiSample.AvgRssi[0],
	//	  pEntry->RssiSample.AvgRssi[1],
	//	 pEntry->RssiSample.AvgRssi[2],
	//	pEntry->RssiSample.AvgRssi[3],Rssi);
	if ((pAd->CommonCfg.dbdc_mode == TRUE) && (HcGetBandByWdev(pEntry->wdev) == 0)) //5G
	{
		return TRUE;
	}
	if ((table->AlgCtrl.ConditionCheck & fBND_STRG_CND_5G_RSSI) &&
		((table->Band & BAND_5G) == BAND_5G) &&
		(Rssi < (table->RssiLow)))
	{
		BNDSTRG_MSG msg;

		msg.Action = CLI_DEL;
		COPY_MAC_ADDR(msg.Addr, pEntry->Addr);
		 /* we don't know the index, daemon should look it up */
		msg.TalbeIndex = BND_STRG_MAX_TABLE_SIZE;

		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_TRACE,
				(YLW("%s(): Kick client (%02x:%02x:%02x:%02x:%02x:%02x)")
				 YLW(" due to low Rssi(%d).\n")
				 , __FUNCTION__, PRINT_MAC(pEntry->Addr), Rssi));

		RtmpOSWrielessEventSend(
			pAd->net_dev,
			RT_WLAN_EVENT_CUSTOM,
			OID_BNDSTRG_MSG,
			NULL,
			(UCHAR *) &msg,
			sizeof(BNDSTRG_MSG));
		table->Ops->TableEntryDel(table, pEntry->Addr, BND_STRG_MAX_TABLE_SIZE);

		return FALSE;
	}

	return TRUE;
}


INT BndStrg_MsgHandle(PRTMP_ADAPTER pAd, RTMP_IOCTL_INPUT_STRUCT *wrq)
{
	BNDSTRG_MSG msg;
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;

	if (table->bInitialized == FALSE)
		return BND_STRG_NOT_INITIALIZED;
	
	if (wrq->u.data.length != sizeof(BNDSTRG_MSG))
		return BND_STRG_INVALID_ARG;
	else
	{
		copy_from_user(&msg, wrq->u.data.pointer, wrq->u.data.length);
		if (table->Ops->MsgHandle)
			table->Ops->MsgHandle(pAd, &msg);
	}

	return BND_STRG_SUCCESS;
}

static VOID D_ShowTableInfo(PBND_STRG_CLI_TABLE table)
{
	BNDSTRG_MSG msg;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	
	msg.Action = TABLE_INFO;

	D_BndStrgSendMsg(pAd, &msg);
}

static VOID D_ShowTableEntries(PBND_STRG_CLI_TABLE table)
{
	BNDSTRG_MSG msg;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	CHAR	band_str[4][10]={"","5G","2.4G","2.4G/5G"};
	INT i;
	BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
						("\t%s Accessible Clients:\n",band_str[table->Band]));

	for (i = 0; i < BND_STRG_MAX_TABLE_SIZE; i++)
	{
		if (table->Entry[i].bValid)
		{
			BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
							("\t%d: %02x:%02x:%02x:%02x:%02x:%02x\n",
							 i, PRINT_MAC(table->Entry[i].Addr)));
		}
	}

	BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
						("\tBndStrg Table Entries:\n"));
	msg.Action = ENTRY_LIST;
	D_BndStrgSendMsg(pAd, &msg);
}


static BOOLEAN D_CheckConnectionReq(
			PRTMP_ADAPTER pAd,
			struct wifi_dev *wdev,
			PUCHAR pSrcAddr,
			UINT8 FrameType,
			PCHAR Rssi,
			BOOLEAN bAllowStaConnectInHt)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;
	BNDSTRG_MSG msg;
	UINT32 frame_type_to_frame_check_flags[] = { \
								fBND_STRG_FRM_CHK_PRB_REQ,
								0,
								fBND_STRG_FRM_CHK_ASS_REQ,
								fBND_STRG_FRM_CHK_ATH_REQ};
	UINT32 frame_check_flags = 0;
	CHAR i, rssi_max;
	BOOLEAN IsInf2G = FALSE;

	/* Send to daemon */
	rssi_max = pAd->CommonCfg.RxStream;
	memset(msg.Rssi,0x80,sizeof(msg.Rssi));
	for ( i = 0; i < rssi_max; i++)
	{
		msg.Rssi[i] = Rssi[i];
	}

	msg.Action= CONNECTION_REQ;
	if (WMODE_CAP_2G(wdev->PhyMode) &&
			wdev->channel <= 14) {
		msg.Band = BAND_24G;
		IsInf2G = TRUE;
	}
	if (WMODE_CAP_5G(wdev->PhyMode) &&
			wdev->channel > 14) {
		msg.Band = BAND_5G;
	}
	msg.FrameType = FrameType;
	msg.bAllowStaConnectInHt = bAllowStaConnectInHt;
	COPY_MAC_ADDR(msg.Addr, pSrcAddr);
	D_BndStrgSendMsg(pAd, &msg);

	if (FrameType < (sizeof(frame_type_to_frame_check_flags)/sizeof(UINT32)))
		frame_check_flags = frame_type_to_frame_check_flags[FrameType];
	else
		{/* invalid frame type */}

	if (table->bEnabled == TRUE &&
		frame_check_flags & table->AlgCtrl.FrameCheck)
	{
		PBND_STRG_CLI_ENTRY entry = NULL;

		if (table->Ops)
			entry = table->Ops->TableLookup(table, pSrcAddr);

		if (entry/* || table->Band == BAND_5G*/) {
			if (pAd->CommonCfg.dbdc_mode == TRUE) { // have to check entry connection flags for processing or not
				if ((IsInf2G == TRUE) && (frame_check_flags == fBND_STRG_FRM_CHK_ATH_REQ)) {
					if (entry && (entry->Control_Flags & fBND_STRG_CLIENT_ALLOW_TO_CONNET_2G)) {
						return TRUE;
					} else {
#ifdef BND_STRG_QA
						BND_STRG_PRINTQAMSG(table, pSrcAddr,
											(RED("[%d]%s: check %s request failed. client's (%02x:%02x:%02x:%02x:%02x:%02x)"
												 " request is ignored. \n"), __LINE__, (table->Band == BAND_24G ? "2.4G" : "5G"),
											 FrameType == 0 ? ("probe") : (FrameType == 3 ? "auth" : "unknow"),
											 PRINT_MAC(pSrcAddr)));
#endif
						return FALSE;
					}
				}
				if ((IsInf2G == TRUE) && (frame_check_flags == fBND_STRG_FRM_CHK_PRB_REQ)) {
					if (entry && (entry->Control_Flags & fBND_STRG_CLIENT_ALLOW_TO_CONNET_2G)) {
						return TRUE;
					} else {
#ifdef BND_STRG_QA
						BND_STRG_PRINTQAMSG(table, pSrcAddr,
											(RED("[%d]%s: check %s request failed. client's (%02x:%02x:%02x:%02x:%02x:%02x)"
												 " request is ignored. \n"), __LINE__, (table->Band == BAND_24G ? "2.4G" : "5G"),
											 FrameType == 0 ? ("probe") : (FrameType == 3 ? "auth" : "unknow"),
											 PRINT_MAC(pSrcAddr)));
#endif
						return FALSE;
					}
				}
				if ((IsInf2G ==FALSE) && ((frame_check_flags == fBND_STRG_FRM_CHK_PRB_REQ) || (frame_check_flags == fBND_STRG_FRM_CHK_ATH_REQ))) {
					if (entry && (entry->Control_Flags & fBND_STRG_CLIENT_ALLOW_TO_CONNET_5G)) {
						return TRUE;
					} else {
#ifdef BND_STRG_QA
						BND_STRG_PRINTQAMSG(table, pSrcAddr,
											(RED("[%d]%s: check %s request failed. client's (%02x:%02x:%02x:%02x:%02x:%02x)"
												 " request is ignored. \n"), __LINE__, (table->Band == BAND_24G ? "2.4G" : "5G"),
											 FrameType == 0 ? ("probe") : (FrameType == 3 ? "auth" : "unknow"),
											 PRINT_MAC(pSrcAddr)));
#endif
						return FALSE;
					}
				}
			}
			return TRUE;
		} else {
#ifdef BND_STRG_QA
			BND_STRG_PRINTQAMSG(table, pSrcAddr,
			(RED("%s: check %s request failed. client's (%02x:%02x:%02x:%02x:%02x:%02x)"
			" request is ignored. \n"), (table->Band == BAND_24G ? "2.4G" : "5G"),
			FrameType == 0 ? ("probe") : (FrameType == 3 ? "auth" : "unknow"),
			PRINT_MAC(pSrcAddr)));
#endif
			return FALSE;
		}			

	}
	
	return TRUE;
}


static VOID D_InfStatusRsp(PBND_STRG_CLI_TABLE table, BNDSTRG_MSG *msg)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG Rsp;

	/* If user config is OFF, don't respond to this query */
	if (pAd->ApCfg.BandSteering == 0)
		return;
	else if (pAd->ApCfg.BandSteering == 2 /* Auto OnOff */
		&& pAd->CommonCfg.Channel <= 14) 
	{
		// TODO: Use Avg False CCA
		if (pAd->RalinkCounters.OneSecFalseCCACnt > table->AutoOnOffThrd &&
			table->bEnabled == FALSE) {
			table->Ops->SetEnable(table, 1);
			return;
		} else if (pAd->RalinkCounters.OneSecFalseCCACnt < table->AutoOnOffThrd &&
			table->bEnabled == TRUE){
			table->Ops->SetEnable(table, 0);
			return;
		}
	}
	
	/*INF_STATUS*/
	if (table->bInitialized == TRUE)
	{
		PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
		struct wifi_dev *wdev = NULL;
		/* Send to daemon */
		if (table->Band == BAND_24G) {
			wdev = &pAd->ApCfg.MBSSID[table->u2GIdx].wdev;
			if (wdev->if_dev)
				strcpy(table->uc2GIfName,wdev->if_dev->name);
			Rsp.Action = INF_STATUS_RSP_2G;
			Rsp.b2GInfReady = table->b2GInfReady;
			strcpy(Rsp.uc2GIfName,table->uc2GIfName);
		} else if (table->Band == BAND_5G) {
			wdev = &pAd->ApCfg.MBSSID[table->u5GIdx].wdev;
			if (wdev->if_dev)
				strcpy(table->uc5GIfName,wdev->if_dev->name);
			Rsp.Action = INF_STATUS_RSP_5G;
			Rsp.b5GInfReady = table->b5GInfReady;
			strcpy(Rsp.uc5GIfName,table->uc5GIfName);
		} else if (table->Band == BAND_BOTH) {
			wdev = &pAd->ApCfg.MBSSID[table->u2GIdx].wdev;
			if (wdev->if_dev)
				strcpy(table->uc2GIfName,wdev->if_dev->name);
			wdev = &pAd->ApCfg.MBSSID[table->u5GIdx].wdev;
			if (wdev->if_dev)
				strcpy(table->uc5GIfName,wdev->if_dev->name);
			Rsp.Action = INF_STATUS_RSP_DBDC;
			Rsp.b2GInfReady = table->b2GInfReady;
			strcpy(Rsp.uc2GIfName,table->uc2GIfName);
			Rsp.b5GInfReady = table->b5GInfReady;
			strcpy(Rsp.uc5GIfName,table->uc5GIfName);
		} else {
			return;
		}
		D_BndStrgSendMsg(pAd, &Rsp);

	}
}

/* For IOCTL */
static INT D_SetEnable(
			PBND_STRG_CLI_TABLE table,
			BOOLEAN enable)
{
	INT ret_val = BND_STRG_SUCCESS;
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): enable = %u\n", __FUNCTION__,  enable));

	if (!(table->bEnabled ^ enable))
	{
		/* Already enabled/disabled */
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, /* TRACE */
				(GRN("%s(): Band steering is already %s.\n"),
				__FUNCTION__, (enable ? "enabled" : "disabled")));
		return BND_STRG_SUCCESS;
	}

	pAd->ApCfg.BandSteering = enable;

	if (enable)
		ret_val = BndStrg_Init(pAd);
	else
		ret_val = BndStrg_Release(pAd);

	if (ret_val != BND_STRG_SUCCESS)
	{
		BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_ERROR,
					("Error in %s(), error code = %d!\n", __FUNCTION__, ret_val));
	}

	return TRUE;
}


static INT D_SetRssiDiff(
			PBND_STRG_CLI_TABLE table,
			CHAR RssiDiff)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;

	table->RssiDiff = RssiDiff;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): RssiCheck = %u\n", __FUNCTION__, table->RssiDiff));

	msg.Action = SET_RSSI_DIFF;
	msg.RssiDiff = RssiDiff;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}


static INT D_SetRssiLow(
			PBND_STRG_CLI_TABLE table,
			CHAR RssiLow)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;

	table->RssiLow = RssiLow;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): RssiLow = %d\n", __FUNCTION__, table->RssiLow));

	msg.Action = SET_RSSI_LOW;
	msg.RssiLow = RssiLow;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}


static INT D_SetAgeTime(
			PBND_STRG_CLI_TABLE table,
			UINT32	Time)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;

	table->AgeTime = Time;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): AgeTime = %u\n", __FUNCTION__, table->AgeTime));

	msg.Action = SET_AGE_TIME;
	msg.Time = table->AgeTime;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}


static INT D_SetHoldTime(
			PBND_STRG_CLI_TABLE table,
			UINT32	Time)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;
	
	table->HoldTime= Time;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): HoldTime = %u\n", __FUNCTION__, table->HoldTime));

	msg.Action = SET_HOLD_TIME;
	msg.Time = table->HoldTime;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}


static INT D_SetCheckTime(
			PBND_STRG_CLI_TABLE table,
			UINT32	Time)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;

	table->CheckTime_5G = Time;
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): CheckTime_5G = %u\n", __FUNCTION__, table->CheckTime_5G));

	msg.Action = SET_CHECK_TIME;
	msg.Time = table->CheckTime_5G;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}


static INT D_SetFrmChkFlag(
			PBND_STRG_CLI_TABLE table,
			UINT32	FrmChkFlag)
{
	table->AlgCtrl.FrameCheck = FrmChkFlag;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
			("%s(): FrameCheck = 0x%x\n", __FUNCTION__, table->AlgCtrl.FrameCheck));

	return TRUE;
}


static INT D_SetCndChkFlag(
			PBND_STRG_CLI_TABLE table,
			UINT32	CndChkFlag)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;

	table->AlgCtrl.ConditionCheck = CndChkFlag;

	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF,
			("%s(): CndChkFlag = 0x%x\n", __FUNCTION__, table->AlgCtrl.ConditionCheck));

	msg.Action = SET_CHEK_CONDITIONS;
	msg.ConditionCheck = table->AlgCtrl.ConditionCheck;
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}

#ifdef BND_STRG_DBG
static INT D_SetMntAddr(
			PBND_STRG_CLI_TABLE table,
			PUCHAR Addr)
{
	PRTMP_ADAPTER pAd = (PRTMP_ADAPTER) table->priv;
	BNDSTRG_MSG msg;
	
	COPY_MAC_ADDR(table->MonitorAddr, Addr);
	MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_OFF, 
			("%s(): %02x:%02x:%02x:%02x:%02x:%02x\n",
			__FUNCTION__, PRINT_MAC(table->MonitorAddr)));

	msg.Action = SET_MNT_ADDR;
	COPY_MAC_ADDR(msg.Addr, table->MonitorAddr);
	D_BndStrgSendMsg(pAd, &msg);

	return TRUE;
}
#endif /* BND_STRG_DBG */

static VOID D_MsgHandle(
			PRTMP_ADAPTER	pAd,
			BNDSTRG_MSG *msg)
{
	PBND_STRG_CLI_TABLE table = P_BND_STRG_TABLE;
	PBND_STRG_CLI_ENTRY entry = NULL;

	BND_STRG_MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_INFO, ("%s: action code (%d)\n",__FUNCTION__, msg->Action));

	if (!table)
		return;

	if ((table->DaemonPid != 0xffffffff) && (table->DaemonPid != current->pid))
		return;
	switch (msg->Action)
	{
		case CLI_ADD:
			if (!table->Ops->TableLookup(table, msg->Addr))
				table->Ops->TableEntryAdd(table, msg->Addr, &entry);
			break;

		case CLI_UPDATE:
			entry = table->Ops->TableLookup(table, msg->Addr);
			if (entry != NULL) {
				entry->Control_Flags = msg->Control_Flags;
				entry->elapsed_time = msg->elapsed_time;
			}
			break;

		case CLI_DEL:
			table->Ops->TableEntryDel(table, msg->Addr, 0xFF);
			break;

		case CLI_AGING_REQ:
			msg->Action = CLI_AGING_RSP;
			msg->Band = table->Band;

			if (MacTableLookup(pAd, msg->Addr) == NULL)
			{
				/* we can aging the entry if it is not in the mac table */
				msg->ReturnCode = BND_STRG_SUCCESS;
				table->Ops->TableEntryDel(table, msg->Addr, 0xFF);
			}
			else
			{
				msg->ReturnCode = BND_STRG_STA_IS_CONNECTED;
			}

			D_BndStrgSendMsg(pAd, msg);

			break;

		case INF_STATUS_QUERY:
			D_InfStatusRsp(table, msg);
			break;

		case BNDSTRG_ONOFF:
			BndStrg_Enable(table, msg->OnOff);
			if (msg->OnOff) 
				table->DaemonPid = current->pid;
			else {
				UINT32 i;
				PBND_STRG_CLI_ENTRY entry = NULL;
				for (i=0;i<BND_STRG_MAX_TABLE_SIZE;i++) {
					entry = &table->Entry[i];
					if (entry->bValid == TRUE) {
						BndStrg_DeleteEntry(table, entry->Addr, i);
					}
				}
				table->DaemonPid = 0xffffffff;
			}
			break;

		default:
			MTWF_LOG(DBG_CAT_ALL, DBG_SUBCAT_ALL, DBG_LVL_WARN, ("%s: unknown action code. (%d)\n",__FUNCTION__, msg->Action));
			break;
	}
}

BNDSTRG_OPS D_BndStrgOps = {
	.ShowTableInfo = D_ShowTableInfo,
	.ShowTableEntries = D_ShowTableEntries,
	.TableEntryAdd = BndStrg_InsertEntry,
	.TableEntryDel = BndStrg_DeleteEntry,
	.TableLookup = BndStrg_TableLookup,
	.CheckConnectionReq = D_CheckConnectionReq,
	.SetEnable = D_SetEnable,
	.SetRssiDiff = D_SetRssiDiff,
	.SetRssiLow = D_SetRssiLow,
	.SetAgeTime = D_SetAgeTime,
	.SetHoldTime = D_SetHoldTime,
	.SetCheckTime = D_SetCheckTime,
	.SetFrmChkFlag = D_SetFrmChkFlag,
	.SetCndChkFlag = D_SetCndChkFlag,
#ifdef BND_STRG_DBG
	.SetMntAddr = D_SetMntAddr,
#endif /* BND_STRG_DBG */
	.MsgHandle= D_MsgHandle,
};
#endif /* BAND_STEERING */

