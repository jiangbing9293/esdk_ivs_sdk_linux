/********************************************************************
	filename	: 	DownloadMgr.cpp
	author		:	z00201790
	created		:	2012/12/06	
	description	:	����¼�����ع�����;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/

#include "DownloadManger.h"
#include "LockGuard.h"
#include "UserMgr.h"
#include "MediaBase.h"
#include "IVS_Trace.h"

CDownloadMgr::CDownloadMgr(void)
{
	m_pUserMgr = NULL;
	m_pHandleDownLoadMutex = VOS_CreateMutex();
}

CDownloadMgr::~CDownloadMgr(void)
{
	m_pUserMgr = NULL;
	try
	{
		HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
		while (m_HandleDownLoadMap.end() != iter)
		{
			CDownload* pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
			if (NULL != pDownLoad)
			{
				IVS_DELETE(pDownLoad);
			}
			++iter;
		}
		m_HandleDownLoadMap.clear();

		if (NULL != m_pHandleDownLoadMutex)
		{
			(void)VOS_DestroyMutex(m_pHandleDownLoadMutex);
			m_pHandleDownLoadMutex = NULL;
		}
	}
	catch (...)
	{
	}
}

void CDownloadMgr::SetUserMgr(CUserMgr* pUserMgr)
{
	m_pUserMgr = pUserMgr;
}


CDownload* CDownloadMgr::GetFreeDownLoad(IVS_ULONG& ulHandle)
{
	CLockGuard lock(m_pHandleDownLoadMutex);
	CDownload *pDownLoad = NULL;

	// ���ҿ��ж���;
	HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
	HandleDownLoadIter end  = m_HandleDownLoadMap.end();
	for (; iter != end; iter++)
	{
		pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
		if (NULL != pDownLoad)
		{
			if (pDownLoad->GetStatus() == MEDIA_STATUS_FREE)
			{
				break;
			}
		}
		pDownLoad = NULL;
	}

	if (NULL != pDownLoad)
	{
		m_HandleDownLoadMap.erase(iter);
	}
	else
	{
		pDownLoad = IVS_NEW((CDownload*&)pDownLoad);
		if (NULL == pDownLoad)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, 
				"Get Free DownLoad", "create DownLoad error");
			return NULL;
		}
	}

	//��ʼ��realplay����;
	pDownLoad->SetUserMgr(m_pUserMgr);
	pDownLoad->SetStatus(MEDIA_STATUS_BUSY);

	//�����б�;
	ulHandle = reinterpret_cast<unsigned long>(pDownLoad);
	(void)m_HandleDownLoadMap.insert(std::make_pair(ulHandle, pDownLoad));
	pDownLoad->SetHandle(ulHandle);
	return pDownLoad;
}


CDownload* CDownloadMgr::GetDownLoad(IVS_ULONG ulHandle)
{
	CLockGuard lock(m_pHandleDownLoadMutex);
    CDownload *pDownLoad = NULL;
    HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
    HandleDownLoadIter end  = m_HandleDownLoadMap.end();

    //���ҿ��ж���;
    for (; iter != end; iter++)
    {
        if (iter->first == ulHandle)
        {
            pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
            break;
        }
    }
    return pDownLoad;
}

// ����Rtsp�����ȡ���ؾ��
IVS_ULONG CDownloadMgr::GetDownloadHandlebyRtspHandle(int iRtspHandle)
{
	CLockGuard lock(m_pHandleDownLoadMutex);
	IVS_ULONG ulHandle = 0;
    HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
    HandleDownLoadIter end  = m_HandleDownLoadMap.end();
    
	for (; iter != end; iter++)
    {
        CDownload* pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
        if (NULL == pDownLoad)
        {
            continue;
        }

        int iRtspHandleTemp = pDownLoad->GetRtspHandle();
        if (iRtspHandleTemp == iRtspHandle)
        {
            ulHandle = iter->first;
            break;
        }
    }
    return ulHandle;
}


IVS_ULONG CDownloadMgr::GetDownloadHandleByNetSourceChannel(unsigned long ulNetSourceChannel)
{
	CLockGuard lock(m_pHandleDownLoadMutex);
	IVS_ULONG ulHandle = 0;
    HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
    HandleDownLoadIter end  = m_HandleDownLoadMap.end();
    for (; iter != end; iter++)
    {
        CDownload* pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
        if (NULL == pDownLoad)
        {
            continue;
        }
        unsigned long ulNetSourceHandleTemp = pDownLoad->GetNetSourceHandle();
        if (ulNetSourceHandleTemp == ulNetSourceChannel)
        {
            ulHandle = iter->first;
            break;
        }
    }
    return ulHandle;
}


IVS_ULONG CDownloadMgr::GetDownloadHandleByPlayChannel(unsigned long ulPlayChannel)
{
	CLockGuard lock(m_pHandleDownLoadMutex);
	IVS_ULONG ulHandle = 0;
    HandleDownLoadIter iter = m_HandleDownLoadMap.begin();
    HandleDownLoadIter end  = m_HandleDownLoadMap.end();
    for (; iter != end; iter++)
    {
        CDownload* pDownLoad = dynamic_cast<CDownload*>(iter->second);//lint !e611
        if (NULL == pDownLoad)
        {
            continue;
        }
        unsigned long ulNetSourceHandleTemp = pDownLoad->GetPlayerChannel();
        if (ulNetSourceHandleTemp == ulPlayChannel)
        {
            ulHandle = iter->first;
            break;
        }
    }
    return ulHandle;
}


// ����ƽ̨(�����ݷ�����)¼������
int CDownloadMgr::StartPlatformDownLoad(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const char* pCameraCode, 
	    const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pHandle)
{
	IVS_DEBUG_TRACE("");
    //���ݷ�����ʱpDomainCode��pNVRCode����ΪNULL��ƽ̨ʱ��ΪNULL����������У��
	if (NULL == pCameraCode || NULL == pDownloadParam || NULL == pHandle)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Platform DownLoad", "CheckParam failed");
        return IVS_PARA_INVALID;
    }

    IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
  	IVS_INT32 iRet     = IVS_FAIL;

    CDownload *pDownload = GetFreeDownLoad(ulHandle);
    if (NULL != pDownload)
    {
      	iRet = pDownload->StartPlatformDownLoad(pDomainCode,pNVRCode,pCameraCode, pDownloadParam);
        if (IVS_SUCCEED != iRet)
        {
            pDownload->RetSet();
        }
        else
        {
            *pHandle = ulHandle;
        }
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
    }
	return iRet;
}

int CDownloadMgr::StartPlatformDownLoadRawCB(const IVS_CHAR* pDomainCode,
	const IVS_CHAR* pNVRCode,
	const char* pCameraCode,
	const IVS_DOWNLOAD_PARAM* pDownloadParam,
	DownloadCallBackRaw pFPlatformDownLoadRawCB,
	void* pUserData,
	IVS_ULONG* pHandle)
{
	IVS_DEBUG_TRACE("");
	//���ݷ�����ʱpDomainCode��pNVRCode����ΪNULL��ƽ̨ʱ��ΪNULL����������У��
	if (NULL == pCameraCode || NULL == pDownloadParam || NULL == pHandle)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Platform DownLoad CB", "CheckParam failed");
		return IVS_PARA_INVALID;
	}

	IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
	IVS_INT32 iRet     = IVS_FAIL;

	CDownload *pDownload = GetFreeDownLoad(ulHandle);
	if (NULL != pDownload)
	{
		iRet = pDownload->StartPlatformDownLoadRawCB(pDomainCode,pNVRCode,pCameraCode, pDownloadParam,pFPlatformDownLoadRawCB,pUserData);
		if (IVS_SUCCEED != iRet)
		{
			pDownload->RetSet();
		}
		else
		{
			*pHandle = ulHandle;
		}
	}
	else
	{
		iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
	}
	return iRet;
}
// ֹͣƽ̨¼������
int CDownloadMgr::StopPlatformDownLoad(IVS_ULONG ulHandle)
{
	IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->StopDownLoad();
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}

// ��������¼������
int CDownloadMgr::StartDisasterRecoveryDownload(const char* pCameraCode, 
    const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pHandle)
{
    IVS_DEBUG_TRACE("");

    if (NULL == pCameraCode || NULL == pDownloadParam || NULL == pHandle)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Start Platform DownLoad", "CheckParam failed");
        return IVS_PARA_INVALID;
    }

    IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
    IVS_INT32 iRet     = IVS_FAIL;

    CDownload *pDownload = GetFreeDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        iRet = pDownload->StartDisasterRecoveryDownload(pCameraCode, pDownloadParam);
        if (IVS_SUCCEED != iRet)
        {
            pDownload->RetSet();
        }
        else
        {
            *pHandle = ulHandle;
        }
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
    }
    return iRet;
}

// ֹͣ����¼������
int CDownloadMgr::StopDisasterRecoveryDownload(IVS_ULONG ulHandle)
{
    IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->StopDisasterRecoveryDownload();
    }
    return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}

// ��ͣ����
int CDownloadMgr::DownLoadPause(IVS_ULONG ulHandle)
{
	IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->DownLoadPause();
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;

}

// �ָ�����
int CDownloadMgr::DownLoadResume(IVS_ULONG ulHandle)
{
	IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->DownLoadResume();
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}

// ���������ٶ�
int CDownloadMgr::SetDownLoadSpeed(IVS_ULONG ulHandle, float fSpeed)
{
	IVS_DEBUG_TRACE("");
    //IVS_INT32 iRet = IVS_FAIL;

    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->SetDownLoadSpeed(fSpeed);
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}


// ��ȡ������Ϣ
int CDownloadMgr::GetDownLoadInfo(IVS_ULONG ulHandle, IVS_DOWNLOAD_INFO* pInfo)
{
	IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->GetDownLoadInfo(pInfo);
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}

// ����ǰ��¼������
int CDownloadMgr::StartPUDownLoad(const char* pCameraCode,
	     const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG *pHandle)
{
    if (NULL == pCameraCode || NULL == pDownloadParam || NULL == pHandle)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Start PU DownLoad", "CheckParam failed.");
        return IVS_PARA_INVALID;
    }

    IVS_RECORD_INFO stRecordInfo;
    memset(&stRecordInfo, 0, sizeof(stRecordInfo));
    IVS_INT32 iRet = GetPUDownLoadFileName(pCameraCode, &pDownloadParam->stTimeSpan,stRecordInfo);
    if (IVS_SUCCEED != iRet)
    {
        return iRet;
    }

    IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
    CDownload *pDownload = GetFreeDownLoad(ulHandle);
    if (NULL != pDownload)
    {
         iRet = pDownload->StartPUDownLoad(pCameraCode,
                                           pDownloadParam,
                                           stRecordInfo.cRecordFileName);
        if (IVS_SUCCEED != iRet)
        {
            pDownload->RetSet();
        }
        else
        {
            *pHandle = ulHandle;
        }
    }
    else
    {
        iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
    }

    return iRet;

}

int CDownloadMgr::StartPUDownLoadRawCB(const char* pCameraCode,
	const IVS_DOWNLOAD_PARAM* pDownloadParam,
	DownloadCallBackRaw pFPUDownLoadRawCB,
	void* pUserData,
	IVS_ULONG  *pHandle)
{
	if (NULL == pCameraCode || NULL == pDownloadParam || NULL == pHandle)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Start PU DownLoad RawCB", "CheckParam failed.");
		return IVS_PARA_INVALID;
	}

	IVS_RECORD_INFO stRecordInfo;
	memset(&stRecordInfo, 0, sizeof(stRecordInfo));
	IVS_INT32 iRet = GetPUDownLoadFileName(pCameraCode, &pDownloadParam->stTimeSpan,stRecordInfo);
	if (IVS_SUCCEED != iRet)
	{
		return iRet;
	}

	IVS_ULONG ulHandle = SDK_INVALID_HANDLE;
	CDownload *pDownload = GetFreeDownLoad(ulHandle);
	if (NULL != pDownload)
	{
		iRet = pDownload->StartPUDownLoadRawCB(pCameraCode,pDownloadParam,stRecordInfo.cRecordFileName,pFPUDownLoadRawCB,pUserData);
		if (IVS_SUCCEED != iRet)
		{
			pDownload->RetSet();
		}
		else
		{
			*pHandle = ulHandle;
		}
	}
	else
	{
		iRet = IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
	}

	return iRet;
}

// ֹͣǰ��¼������
int CDownloadMgr::StopPUDownLoad(IVS_ULONG ulHandle)
{
	IVS_DEBUG_TRACE("");
    CDownload *pDownload  = GetDownLoad(ulHandle);
    if (NULL != pDownload)
    {
        return pDownload->StopDownLoad();
    }
	return IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED;
}


// ��ȡǰ��¼���ļ���
int CDownloadMgr::GetPUDownLoadFileName(const char* pCameraCode,
                                              const IVS_TIME_SPAN* pTimeSpan,
                                              IVS_RECORD_INFO &stRecordInfo)
{
    if (NULL == pCameraCode || NULL == pTimeSpan)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Get PU DownLoad FileName", "CheckParam failed.");
        return IVS_PARA_INVALID;
    }

    if (NULL == m_pUserMgr)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Get PU DownLoad FileName", "CheckParam failed.");
        return IVS_PARA_INVALID;
    }

    memset(&stRecordInfo, 0, sizeof(stRecordInfo));

    IVS_INDEX_RANGE stIndexRange;
    stIndexRange.uiFromIndex = 1;
    stIndexRange.uiToIndex = 1;
    int iSize = sizeof(IVS_RECORD_INFO_LIST) + (stIndexRange.uiToIndex-stIndexRange.uiFromIndex) * sizeof(IVS_RECORD_INFO);   //lint !e737 !e713

	char* pBuff = IVS_NEW((char*&)pBuff,iSize);//lint !e732
	CHECK_POINTER(pBuff,IVS_ALLOC_MEMORY_ERROR);
    memset(pBuff, 0, iSize);   //lint !e732

    int iRet = m_pUserMgr->GetRecordList().GetRecordList(pCameraCode, 
                                                RECORD_TYPE_PU,
                                                pTimeSpan,
                                                &stIndexRange,
                                                (IVS_RECORD_INFO_LIST*)pBuff,
                                                (IVS_UINT32)iSize);   //lint !e826


    if (IVS_SUCCEED == iRet)
    {
        IVS_RECORD_INFO_LIST *pRecordList = (IVS_RECORD_INFO_LIST*)pBuff;   //lint !e826
        (void)CToolsHelp::Memcpy(&stRecordInfo, sizeof(stRecordInfo),
                           pRecordList->stRecordInfo, sizeof(IVS_RECORD_INFO));
    }
	IVS_DELETE(pBuff,MUILI);

    return iRet;
}


