
#ifndef __RLM_CAL_CACHE_H__
#define __RLM_CAL_CACHE_H__

#include "rtmp_type.h"

INT rlmCalCacheDone(VOID *rlmCalCache);
INT rlmCalCacheInit(VOID *pAd, VOID **ppRlmCalCache);
INT rlmCalCacheDeinit(VOID **ppRlmCalCache);
INT RlmCalCacheTxLpfInfo(VOID *rlmCalCtrl, UINT8 *Data, UINT32 Length);
INT RlmCalCacheTxIqInfo(VOID *rlmCalCtrl, UINT8 *Data, UINT32 Length);
INT RlmCalCacheTxDcInfo(VOID *rlmCalCtrl, UINT8 *Data, UINT32 Length);
INT RlmCalCacheRxFiInfo(VOID *rlmCalCtrl, UINT8 *Data, UINT32 Length);
INT RlmCalCacheRxFdInfo(VOID *rlmCalCtrl, UINT8 *Data, UINT32 Length);
VOID rlmCalCacheStatus(VOID *rlmCalCache);
VOID rlmCalCacheDump(VOID *rlmCalCache);

INT TxLpfCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr);
INT TxIqCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr);
INT TxDcCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr);
INT RxFiCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr);
INT RxFdCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr, UINT32 chGroup);
INT RlmPorCalInfoAlloc(VOID *pAd, VOID *rlmCalCache, VOID **pptr);

#endif /* __RLM_CAL_CACHE_H__ */

