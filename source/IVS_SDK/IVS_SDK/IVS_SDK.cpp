/********************************************************************
filename	: 	IVSSDK.cpp
author		:	z00201790
created		:	2012/10/23	
description	:	���� DLL Ӧ�ó���ĵ�������;
copyright	:	Copyright (C) 2011-2015 Huawei Tech.Co.,Ltd
history		:	2012/10/23 ��ʼ�汾;
*********************************************************************/

/*!< Ԥ����ͷ�ļ� */
#ifdef WIN32
#include "stdafx.h"
#endif

#include <stdlib.h> 

#include "IVS_SDK.h"
#ifdef WIN32
#include "IVS_SDKTVWall.h"
#include "IVS_SDKNetKeyBoard.h"
#include "NetKeyBoardService.h"
#endif
//#include "IVS/SDKInterface.h"
#include "SDKInterface.h"
//#include "IVS/Sys/CriticalSectionEx.h"
#include "CriticalSectionEx.h"
#include "bp_log.h"
#ifdef WIN32
#include "IVS_Player.h"
#endif
#include "Aes256Operator.h"
#include "UserMgr.h"
#include "IVS_Trace.h"
#ifdef WIN32
#include "AudioMgr.h"
#endif
#include "ivs_password.h"
#include "EventCallBackJob.h"
#include "ToolHelp.h"
#include "SDKConfig.h"

#ifndef DEBUG
#include "Rename2Java.h"
#endif

CSDKInterface *g_pNvs = NULL;
CCRITICAL_SECTIONEX g_InitLock; //���ڱ���g_pNvs��g_InitCount��֧�ֶ��̵߳���;
int g_InitCount = 0;
//�Խ�
VOS_Mutex* m_pMutexTalkbackFlagLock;
bool g_bIsVoiceTalkbackOpened = false;
int g_voiceTalkbackHandle = SDK_INVALID_HANDLE;

#ifdef WIN32
static CMediaBase* GetMediaBaseAndPortInfo(IVS_INT32 iSessionID,IVS_ULONG ulHandle,IVS_UINT32& uiPort)
{
	IVS_INFO_TRACE("SessionID: %d,Handle: %lu",iSessionID,ulHandle);
	if (NULL == g_pNvs)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"Get Media And Port Info", "Get Media And Port Info Error");
		return NULL;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	if (NULL == pUserMgr)
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"Get Media And Port Info", "Session ID Error");
		return NULL;
	}
	IVS_INT32 iRet = IVS_FAIL;
	IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_REALVIDEO;
	iRet = pUserMgr->GetServiceTypebyPlayHandle(ulHandle, enServiceType);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"Get Media And Port Info", "Get Service Type failed. enServiceType = %d", (int)enServiceType);
		return NULL;
	}
	CMediaBase* mediaBase = NULL;
	switch(enServiceType)
	{
	case SERVICE_TYPE_REALVIDEO:
		mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulHandle);
		uiPort= pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulHandle);
		break;
	case SERVICE_TYPE_PLAYBACK:				//¼��ط�
    case SERVICE_TYPE_PU_PLAYBACK:			//ǰ��¼��ط�
	case SERVICE_TYPE_BACKUPRECORD_PLAYBACK://¼�񱸷ݻط�
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK://����¼��ط�
		mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulHandle);
		uiPort= pUserMgr->GetPlaybackMgr().GetPlayerChannelByHandle(ulHandle);
		break;
	default:
		break;
	}
	if (NULL == mediaBase)
	{
		BP_RUN_LOG_ERR(iRet,"Get Media And Port Info", "Get Media Info Error");
		return NULL;
	}
	return mediaBase;
}
#endif
//��ʼ��SDK
IVS_INT32 __SDK_CALL IVS_SDK_Init()
{
//	IVS_INFO_TRACE("");

	g_InitLock.EnterCriticalSectionEx();
	
	//����Ѿ���ȡ��ȫ��CSDKInterfaceʵ������ôֻ���Ӽ���;
	if (g_pNvs != NULL)
	{
		g_InitCount ++;
		g_InitLock.LeaveCriticalSectionEx();
		BP_RUN_LOG_INF("IVS SDK Init Success", "g_pNvs exist");
		return IVS_SUCCEED;
	}

	try
	{
		g_pNvs = new CSDKInterface();
		if(NULL == g_pNvs)
		{
			g_InitLock.LeaveCriticalSectionEx();
			BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"IVS SDK Init Failed.","Reason:NEW SDK Interface Failed");
			return IVS_OPERATE_MEMORY_ERROR;
		}

		g_InitCount ++;
		g_InitLock.LeaveCriticalSectionEx();

              IVS_INT32 iRet = g_pNvs->Init();
              if (IVS_SUCCEED != iRet)
              {
                    g_InitLock.EnterCriticalSectionEx();
                    g_InitCount--;
                    delete g_pNvs;
                    g_pNvs = NULL;
                    g_InitLock.LeaveCriticalSectionEx();
              }
              return iRet;
	}
	catch (...)
	{
		g_InitLock.LeaveCriticalSectionEx();
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"IVS SDK Init Failed.", "Reason:Try NEW SDK Interface Failed.");
		return IVS_OPERATE_MEMORY_ERROR;
	}
}

//�ͷ�SDK
IVS_INT32 __SDK_CALL IVS_SDK_Cleanup()
{
	IVS_INFO_TRACE("");

	int iRet = IVS_SUCCEED;
	g_InitLock.EnterCriticalSectionEx();
	if(g_InitCount)
	{
		g_InitCount--;
	}

	if (g_InitCount <= 0)
	{
		if(NULL == g_pNvs)
		{
			g_InitLock.LeaveCriticalSectionEx();
			BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"IVS SDK Clean up Failed.", "Reason:g_pNvs is Null");
			return IVS_OPERATE_MEMORY_ERROR;
		}
		iRet = g_pNvs->Cleanup();
		if (g_pNvs)
		{
			delete g_pNvs;
			g_pNvs = NULL;
		}
	}

	g_InitLock.LeaveCriticalSectionEx();
	return iRet;  
}

//��ȡSDK�汾�ţ���ǰ�汾��Ϊ2.1.0.1
IVS_INT32 __SDK_CALL IVS_SDK_GetVersion()
{
	IVS_INFO_TRACE("");
#ifdef WIN32
	WORD nlVersion = MAKEWORD(IVS_SDK_RESERVATION_VERSION,IVS_SDK_REVISE_VERSION);//lint !e835 !e845
	WORD nhVersion = MAKEWORD(IVS_SDK_SUB_VERSION,IVS_SDK_MAINE_VERSION);
	DWORD ulVersion;
	ulVersion = MAKELONG(nlVersion, nhVersion);
#else
	unsigned short nlVersion = ((unsigned short)(IVS_SDK_RESERVATION_VERSION))|((unsigned short)(IVS_SDK_REVISE_VERSION)<<8);
	unsigned short nhVersion = ((unsigned short)(IVS_SDK_SUB_VERSION))|((unsigned short)(IVS_SDK_MAINE_VERSION)<<8);
	unsigned long ulVersion = ((unsigned long)(nlVersion))|((unsigned long)(nhVersion)<<16);;
#endif
	return ulVersion;//lint !e713
}



IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCUVersion (IVS_INT32 iSessionID, const IVS_CU_INFO* pCUInfo, IVS_UPGRADE_INFO* pUpgradeInfo)
{
    IVS_INFO_TRACE("SessionID:%d", iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Cu Version Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUpgradeMgr().GetCUVersion(pCUInfo, pUpgradeInfo);
}

//�����¼��ص�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetEventCallBack(IVS_INT32 iSessionID, EventCallBack fEventCallBack, void *pUserData)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	return g_pNvs->SetEventCallBack(iSessionID, fEventCallBack, pUserData);
}

//ɾ���豸��
IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceGroup( IVS_INT32 iSessionID,
                                                const IVS_CHAR* pDomainCode,
                                                IVS_UINT32 uiDevGroupID)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR); 
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,DevGroupID: %u",iSessionID,pDomainCode,uiDevGroupID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_OPERATE_MEMORY_ERROR);
    return pUserMgr->GetDeviceGroupMgr().DeleteDeviceGroup(pDomainCode, uiDevGroupID);
}


// ����û�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo, IVS_UINT32* puiUserID)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserInfo, IVS_PARA_INVALID);
    CHECK_POINTER(puiUserID, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().AddUser(pUserInfo, puiUserID);
}

// �޸��û�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserInfo, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify User failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().ModifyUser(pUserInfo);
}

// ɾ���û�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().DeleteUser(pDomainCode, uiUserID);

}

// ��ȡ�û��б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_USER_INFO_LIST* pUserList, IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d,BufferSize: %u",iSessionID,uiBufferSize);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUnifiedQuery, IVS_PARA_INVALID);
    CHECK_POINTER(pUserList, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get User List Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().GetUserList(pDomainCode, pUnifiedQuery, pUserList, uiBufferSize);
}

// ��ȡ�����û��б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ONLINE_USER_LIST* pOnlineUserList, IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d, BufferSize: %u",iSessionID,uiBufferSize);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUnifiedQuery, IVS_PARA_INVALID);
    CHECK_POINTER(pOnlineUserList, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Online User Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);    
    return pUserMgr->GetUserInfoMgr().GetOnlineUser(pDomainCode, pUnifiedQuery, pOnlineUserList, uiBufferSize);
}

// ��ȡ�û���Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_INFO* pUserInfo)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserInfo, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get User Info Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);    
    return pUserMgr->GetUserInfoMgr().GetUserInfo(pDomainCode, uiUserID, pUserInfo);
}

// ����/�����û�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_BOOL bLock)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Lock User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);    
    return pUserMgr->GetUserInfoMgr().LockUser(pDomainCode, (IVS_INT32)uiUserID, uiUserID, bLock);
}

// �����û��豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserDeviceList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize)
{
	BP_RUN_LOG_INF("IVS_SDK_SetUserDeviceList", "");
    IVS_INFO_TRACE("SessionID: %d,UserID: %u,BufferSize: %u",iSessionID,uiUserID,uiBufferSize);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserDevList, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Set User Device List Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	// TQE
	if (uiBufferSize < sizeof(IVS_USER_DEVICE_INFO_LIST))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"buffer size error", "NA");
		return IVS_PARA_INVALID;
	}

	// �������� TODO
	IVS_USER_DEVICE_INFO_LIST* pUserDevListIn = (IVS_USER_DEVICE_INFO_LIST*)IVS_NEW((IVS_CHAR* &)pUserDevListIn,uiBufferSize);
	(void)CToolsHelp::Memcpy(pUserDevListIn, uiBufferSize, pUserDevList, uiBufferSize);
	IVS_UINT32 uiNum = pUserDevListIn->uiTotal;
	for (IVS_UINT32 i = 0; i < uiNum; i++)
	{
		IVS_CHAR szDevGroupCode[IVS_DEVICE_GROUP_LEN + 1] = {0};
		IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1] = {0};
		(void)CXmlProcess::ParseDevCode(pUserDevListIn->stUserDeviceInfo[i].cDevGroupCode, szDevGroupCode, szDomainCode);
		(void)CToolsHelp::Memcpy(pUserDevListIn->stUserDeviceInfo[i].cDevGroupCode, IVS_DEVICE_GROUP_LEN, szDevGroupCode, IVS_DEVICE_GROUP_LEN);
	}
	IVS_INT32 iRet = pUserMgr->GetUserInfoMgr().SetUserDeviceList(pDomainCode, uiUserID, pUserDevList, uiBufferSize);
	IVS_DELETE(pUserDevListIn);
	return iRet;
}

// ��ѯ�û��豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserDeviceList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u,BufferSize: %u",iSessionID,uiUserID,uiBufferSize);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
    CHECK_POINTER(pUserDevList, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get User Device List Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().GetUserDeviceList(pDomainCode, uiUserID, pIndexRange, pUserDevList, uiBufferSize);
}

// �޸�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ChangePWD(IVS_INT32 iSessionID, const IVS_CHAR* pOldPWD, const IVS_CHAR* pNewPWD)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pOldPWD, IVS_PARA_INVALID);
    CHECK_POINTER(pNewPWD, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Change PWD Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);    
    return pUserMgr->GetUserInfoMgr().ChangePWD(pOldPWD, pNewPWD);
}

// ��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID, const IVS_CHAR* pNewPWD)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pNewPWD, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Reset PWD Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().ResetPWD(pDomainCode, uiUserID, pNewPWD);
}

// У�������Ƿ������˺Ź���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckPWDByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, const IVS_CHAR* pPWD)
{	
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pLoginName, IVS_PARA_INVALID);
    CHECK_POINTER(pPWD, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,RoleID: %u,LoginName: %s",iSessionID,uiRoleID,pLoginName);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Check PWD By Role Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().CheckPWDByRole(pDomainCode, uiRoleID, pLoginName, pPWD);
}

// ��ȡ����ǿ��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDStrength(const IVS_CHAR* pPWD, IVS_UINT32* uiPWDStrength)
{	
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pPWD, IVS_PARA_INVALID);
    IVS_INFO_TRACE("uiPWDStrength: %u",uiPWDStrength);

    *uiPWDStrength = GetPasswordReliability(pPWD);
    return IVS_SUCCEED;
}

// ��ȡ���������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDExpireInfo(IVS_INT32 iSessionID, IVS_PWD_EXPIRE_INFO* pPWDExpireInfo)
{	
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pPWDExpireInfo, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Reset PWD Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().GetPWDExpireInfo(pPWDExpireInfo);
}

// �����û�IDǩ���û������лỰ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Sign Out User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);     
    return pUserMgr->GetUserInfoMgr().SignOutUser(pDomainCode, uiUserID);
}

// ���ݽ�ɫIDǩ���û������лỰ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUserByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID)
{
    IVS_INFO_TRACE("SessionID: %d,RoleID: %u",iSessionID,uiRoleID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Sign Out User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);     
    return pUserMgr->GetUserInfoMgr().SignOutUserByRole(pDomainCode, uiRoleID);
}

// ǩ�������û�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserOnlineID)
{
    IVS_INFO_TRACE("SessionID: %d,UserOnlineID: %u",iSessionID,uiUserOnlineID);

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Sign Out Online User Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);     
    return pUserMgr->GetUserInfoMgr().SignOutOnlineUser(pDomainCode, uiUserOnlineID);
}

// ����û���init
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup, IVS_UINT32* puiUserGroupID)
{

    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserGroup, IVS_PARA_INVALID);
    CHECK_POINTER(puiUserGroupID, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserGroupMgr().AddUserGroup(pUserGroup, puiUserGroupID);
}

// ɾ���û���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserGroupID)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID,uiUserGroupID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserGroupMgr().DeleteUserGroup(pDomainCode, uiUserGroupID);
}

// �޸��û���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserGroup, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserGroupMgr().ModifyUserGroup(pUserGroup);
}

// �ƶ��û���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiTargetGroupID, const IVS_USER_GROUP_LIST* pUserGroupList)
{
    IVS_INFO_TRACE("SessionID: %d,TargetGroupID: %u",iSessionID,uiTargetGroupID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUserGroupList, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Move User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);


    return pUserMgr->GetUserGroupMgr().MoveUserGroup(pDomainCode, uiTargetGroupID, pUserGroupList);
}

// ��ȡ�û����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserGroupList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_GROUP_LIST* pUserGroupList, IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d,BufferSize: %u",iSessionID,uiBufferSize);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
    CHECK_POINTER(pUserGroupList, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get User Group List Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserGroupMgr().GetUserGroupList(pDomainCode, pIndexRange, pUserGroupList, uiBufferSize);
}

// ��ӽ�ɫ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);
    CHECK_POINTER(puiRoleID, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add Role Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);   

    return pUserMgr->GetRoleMgr().AddRole(pRoleInfo, puiRoleID);
}

// ɾ����ɫ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID)
{
    IVS_INFO_TRACE("SessionID: %d,RoleID: %u",iSessionID,uiRoleID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete Role Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetRoleMgr().DeleteRole(pDomainCode, uiRoleID);
}

// �޸Ľ�ɫ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify Role Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);     
    return pUserMgr->GetRoleMgr().ModifyRole(pRoleInfo);
}

// ��ȡ��ɫ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d,uiBufferSize: %u",iSessionID,uiBufferSize);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
    CHECK_POINTER(pRoleInfoList, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Role List Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRoleMgr().GetRoleList(pDomainCode, pIndexRange, pRoleInfoList, uiBufferSize);
}

// ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByRoleID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,RoleID: %u",iSessionID,uiRoleID); 

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get RoleInfo By RoleID Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRoleMgr().GetRoleInfoByRoleID(pDomainCode, uiRoleID, pRoleInfo);
}

//���ݽ�ɫ��ȡ�����˺Ź�������� 
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAutoCreatePWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, IVS_CHAR* pNewPWD, IVS_UINT32 uiPWDLen)
{	
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pLoginName, IVS_PARA_INVALID);
    CHECK_POINTER(pNewPWD, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,RoleID: %u,LoginName: %s",iSessionID,uiRoleID,pLoginName);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get AutoCreate PWD Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserInfoMgr().GetAutoCreatePWD(pDomainCode, uiRoleID, pLoginName, pNewPWD, uiPWDLen);
}

// �����û�ID��ȡ��ɫ��ϸ��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByUserID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo)
{
    IVS_INFO_TRACE("SessionID: %d,UserID: %u",iSessionID,uiUserID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleInfo, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get RoleInfo By UserID Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetRoleMgr().GetRoleInfoByUserID(pDomainCode, uiUserID, pRoleInfo);
}

// ��ȡĬ���˺Ź���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDefaultRoleAccountRule(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule)
{
    IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRoleAccountRule, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Default RoleAccount Rule Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetRoleMgr().GetDefaultRoleAccountRule(pDomainCode, pRoleAccountRule);
}

// ��ȡ�û�Ȩ����Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationRightGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList,IVS_UINT32 uiBufferSize)
{
    IVS_INFO_TRACE("SessionID: %d,BufferSize: %u",iSessionID,uiBufferSize);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRightGroupList, IVS_PARA_INVALID);

    //У��sessionId
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Operation Right Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    //��ȡ�����û�����
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRoleMgr().GetOperationRightGroup(pDomainCode, pRightGroupList);
}

//����ƽ̨¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordPlan(IVS_INT32 iSessionID,
	                                                   IVS_UINT32 uiDeviceNum,
	                                                   const IVS_DEVICE_CODE* pDeviceList,
	                                                   const IVS_RECORD_PLAN* pRecordPlan,
	                                                   IVS_RESULT_LIST* pResultList,
	                                                   IVS_UINT32 uiResultBufferSize)
{	
	IVS_INFO_TRACE("Add Record Plan SessionID: %d,DeviceNum: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiResultBufferSize);
	CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
	CHECK_POINTER(pResultList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add Record Plan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    CRecordPlanMgr& recordPlanMgr = pUserMgr->GetRecordPlanMgr();
    recordPlanMgr.SetUserMgr(pUserMgr); 
    return recordPlanMgr.AddRecordPlan(uiDeviceNum,pDeviceList, pRecordPlan ,pResultList, uiResultBufferSize);
}

//�޸�ƽ̨¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
                                                          const IVS_DEVICE_CODE* pDeviceList,
	                                                      const IVS_RECORD_PLAN* pRecordPlan,
                                                          IVS_RESULT_LIST* pResultList,
                                                          IVS_UINT32 uiResultBufferSize)
{
	IVS_INFO_TRACE("Modify Record Plan SessionID: %d,DeviceNum: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiResultBufferSize);
	CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
	CHECK_POINTER(pResultList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify RecordPlan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordPlanMgr().ModifyRecordPlan(uiDeviceNum,pDeviceList, pRecordPlan ,pResultList, uiResultBufferSize);
}

//ɾ��ƽ̨¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordPlan(IVS_INT32 iSessionID,
	                                                      IVS_UINT32 uiDeviceNum,
	                                                      const IVS_DEVICE_CODE* pDeviceList,
	                                                      IVS_UINT32 uiRecordMethod,
	                                                      IVS_RESULT_LIST* pResultList,
	                                                      IVS_UINT32 uiResultBufferSize)
{
	IVS_INFO_TRACE("SessionID: %d,DeviceNum: %u,RecordMethod: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiRecordMethod,uiResultBufferSize);
	CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
	CHECK_POINTER(pResultList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete RecordPlan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordPlanMgr().DeleteRecordPlan(uiDeviceNum, pDeviceList, uiRecordMethod,pResultList, uiResultBufferSize);
}

//��ѯƽ̨¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPlan(IVS_INT32 iSessionID,
	                                                   const char* pCameraCode,
	                                                   IVS_UINT32 uiRecordMethod,
	                                                   IVS_RECORD_PLAN* pRecordPlan)
{	
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode :%s,RecordMethod: %u",iSessionID,pCameraCode,uiRecordMethod);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get RecordPlan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordPlanMgr().GetRecordPlan(pCameraCode, uiRecordMethod, pRecordPlan);
}

// ��ѯ������ƻ�������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraPlanInfo(IVS_INT32 iSessionID, IVS_UINT32 uiPlanType, IVS_CHAR** pRspXml)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("SessionID: %d,uiPlanType :%d",iSessionID,uiPlanType);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get RecordPlan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordPlanMgr().GetCameraPlanInfo(uiPlanType, pRspXml);
}

//����¼����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBookmark(IVS_INT32 iSessionID,
	const IVS_CHAR* pCameraCode,
	const IVS_CHAR* pNVRCode,
	const IVS_CHAR* pBookmarkName,
	const IVS_CHAR* pBookmarkTime,
	IVS_UINT32* uiBookmarkID)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(pBookmarkTime, IVS_PARA_INVALID);
	CHECK_POINTER(uiBookmarkID, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,NVRCode: %s,BookmarkName :%s,BookmarkTime :%s,AlarmLevelNum: %u",iSessionID,pCameraCode,pNVRCode,pBookmarkName,pBookmarkTime,*uiBookmarkID);


	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add Bookmark Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
	return pUserMgr->GetRecordBookmark().AddBookmark(RECORD_METHOD_PLATFORM,strDomainCode.c_str(),pNVRCode,pCameraCode, pBookmarkName, pBookmarkTime,uiBookmarkID);
}

//�޸�¼����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName)
{
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pNewBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,NVRCode: %s,CameraCode: %s,BookmarkID: %u,NewBookmarkName: %s",iSessionID,pNVRCode,pCameraCode,uiBookmarkID,pNewBookmarkName);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify Bookmark Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
    return pUserMgr->GetRecordBookmark().ModifyBookmark(RECORD_METHOD_PLATFORM,strDomainCode.c_str(),pNVRCode,pCameraCode,uiBookmarkID,pNewBookmarkName);

}

//ɾ��¼����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID)
{	
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,NVRCode: %s,CameraCode: %s,BookmarkID: %u",iSessionID,pNVRCode,pCameraCode,uiBookmarkID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete Bookmark Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
    return pUserMgr->GetRecordBookmark().DeleteBookmark(RECORD_METHOD_PLATFORM,strDomainCode.c_str(),pNVRCode,pCameraCode, uiBookmarkID);
}

//����ʵ����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRealPlayBookmark(IVS_INT32 iSessionID,
	                                                const IVS_CHAR* pCameraCode,	                                                 
	                                                const IVS_CHAR* pBookmarkName,
	                                                const IVS_CHAR* pBookmarkTime,
													IVS_UINT32* uiBookmarkID)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(pBookmarkTime, IVS_PARA_INVALID);
	CHECK_POINTER(uiBookmarkID, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,BookmarkName :%s,BookmarkTime :%s,AlarmLevelNum: %u",iSessionID,pCameraCode,pBookmarkName,pBookmarkTime,*uiBookmarkID);


    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add Bookmark Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
    return pUserMgr->GetRealPlayBookmark().AddBookmark(strDomainCode.c_str(),pCameraCode, pBookmarkName, pBookmarkTime,uiBookmarkID);
}

//�޸�ʵ����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRealPlayBookmark(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pNewBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,BookmarkID: %u,NewBookmarkName: %s",iSessionID,pCameraCode,uiBookmarkID,pNewBookmarkName);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify Bookmark Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
    return pUserMgr->GetRealPlayBookmark().ModifyBookmark(strDomainCode.c_str(),pCameraCode,uiBookmarkID,pNewBookmarkName);

}

//ɾ��ʵ����ǩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRealPlayBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID)
{	
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,BookmarkID: %u",iSessionID,pCameraCode,uiBookmarkID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete Bookmark Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	std::string strDomainCode = "NULL";
    return pUserMgr->GetRealPlayBookmark().DeleteBookmark(strDomainCode.c_str(),pCameraCode, uiBookmarkID);
}


//��ѯ¼����ǩ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetBookmarkList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
	IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize)
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u",iSessionID,uiBufSize);
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBookmarkList, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get BookMark List Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordBookmark().GetBookmarkList(RECORD_METHOD_PLATFORM,pUnifiedQuery,pBookmarkList,uiBufSize);
}

//��ȡ¼���б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryRecordList(IVS_INT32 iSessionID, 
                                                         const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
	                                                     IVS_RECORD_INFO_LIST* pRecordInfoList,
                                                         IVS_UINT32 uiBufferSize)
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u",iSessionID,uiBufferSize);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Query RecordList Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordList().QueryRecordList(pUnifiedQuery,pRecordInfoList,uiBufferSize);
}

//��ѯ¼���б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordList(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordMethod,const IVS_TIME_SPAN* pTimeSpan,
	const IVS_INDEX_RANGE* pIndexRange,IVS_RECORD_INFO_LIST* pRecordList,IVS_UINT32 uiBufSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pTimeSpan, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pIndexRange, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRecordList, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode; %s,RecordMethod: %d,BufSize: %u",iSessionID,pCameraCode,iRecordMethod,uiBufSize);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get RecordList Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordList().GetRecordList(pCameraCode,iRecordMethod,pTimeSpan,pIndexRange,pRecordList,uiBufSize);
}

//��ѯƽ̨¼�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_POLICY_TIME* pRecordPolicy)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordPolicy, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Record Policy By Time Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().GetRecordPolicy(pCameraCode,pRecordPolicy);
}

//����¼��ռ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetRecordPolicyByTime(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,const IVS_RECORD_POLICY_TIME* pRecordSpaceInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordSpaceInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Set RecordPolicy By Time Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().SetRecordPolicy(pCameraCode,pRecordSpaceInfo);
}

//�鿴�澯�����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarmQuery(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{	
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s",iSessionID,pReqXml);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmMgr().SubscribeQuery(pReqXml, pRspXml);
}

//����,�޸�,ȡ���澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s",iSessionID,pReqXml);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmMgr().SubscribeAlarm(pReqXml);
}

//�����澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddHelpInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *pHelpInfo)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs,IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pHelpInfo, IVS_PARA_INVALID);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarHelpMgr().AddHelpInfo(pDomainCode, pHelpInfo);
}

//�޸ĸ澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyHelpInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *pHelpInfo)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pHelpInfo, IVS_PARA_INVALID);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarHelpMgr().ModifyHelpInfo(pDomainCode, pHelpInfo);
}

//ɾ���澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteHelpInfo(IVS_INT32 iSessionID, IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId, const IVS_CHAR* pDomainCode)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,AlarmHelpSourceType: %u,AlarmHelpSourceId: %u, DomainCode: %s ",iSessionID,uiAlarmHelpSourceType,uiAlarmHelpSourceId, pDomainCode);
	

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarHelpMgr().DeleteHelpInfo(uiAlarmHelpSourceId, uiAlarmHelpSourceType, pDomainCode);
}

//��ѯ�澯��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryHelpInfo(IVS_INT32 iSessionID, IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId,const IVS_CHAR* pDomainCode, IVS_HELP_INFO* pHelpInfoResult)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmHelpSourceType: %u,AlarmHelpSourceId: %u, DomainCode: %s",iSessionID,uiAlarmHelpSourceType,uiAlarmHelpSourceId, pDomainCode);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pHelpInfoResult,IVS_PARA_INVALID);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarHelpMgr().QueryHelpInfo(uiAlarmHelpSourceType,uiAlarmHelpSourceId,pDomainCode, pHelpInfoResult);
}

// ��ѯ������Ϣ����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetHelpInfoText(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmInCode, const IVS_CHAR* pAlarmType, IVS_CHAR* pHelpInfo, IVS_UINT32 uiBufferSize)
{
    CHECK_POINTER(pAlarmInCode,IVS_PARA_INVALID);
    CHECK_POINTER(pAlarmType,IVS_PARA_INVALID);
    CHECK_POINTER(pHelpInfo,IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,pAlarmInCode: %s,pAlarmType: %s",iSessionID,pAlarmInCode,pAlarmType);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Record Policy By Time Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarHelpMgr().QueryHelpInfoText(pAlarmInCode,pAlarmType,pHelpInfo,uiBufferSize);

}
//��ѯ�Զ���澯�����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmTypeList(IVS_INT32 iSessionID, 
                                                          const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
                                                          IVS_ALARM_TYPE_LIST* pAlarmTypeList, 
                                                          IVS_UINT32 uiBufferSize)
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u",iSessionID,uiBufferSize);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUnifiedQuery,IVS_PARA_INVALID);
    CHECK_POINTER(pAlarmTypeList,IVS_PARA_INVALID);
    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmMgr().GetAlarmTypeList(pUnifiedQuery, pAlarmTypeList, uiBufferSize);
}

//���ø澯���ͼ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetAlarmTypeLevel(IVS_INT32 iSessionID,  
                                                           const IVS_ALARM_TYPE_LIST* pAlarmTypeList)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    if (NULL == pAlarmTypeList)
    {
        return IVS_PARA_INVALID;
    }

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmMgr().SetAlarmTypeLevel(pAlarmTypeList);
}

//�����澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLevel(IVS_INT32 iSessionID, const IVS_ALARM_LEVEL *pAlarmLevel, IVS_UINT32* pAlarmLevelID)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmLevel, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmLevelMgr().AddAlarmLevel(pAlarmLevel,pAlarmLevelID);	
}

//�޸ĸ澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLevel(IVS_INT32 iSessionID, const IVS_ALARM_LEVEL *pAlarmLevel)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmLevel, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmLevelMgr().ModifyAlarmLevel(pAlarmLevel);

}

//ɾ���澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLevel(IVS_INT32 iSessionID, const IVS_UINT32 uiAlarmLevelId)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmLevelId: %u",iSessionID,uiAlarmLevelId);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmLevelMgr().DeleteAlarmLevel(uiAlarmLevelId);
}

//��ѯ�澯�����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevelList(IVS_INT32 iSessionID,IVS_UINT32 uiMaxAlarmLevelNum,IVS_UINT32* puiAlarmLevelNum,IVS_ALARM_LEVEL* pAlarmLevel,IVS_UINT32 uiBufferSize)
{	
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmLevel, IVS_PARA_INVALID);
	CHECK_POINTER(puiAlarmLevelNum, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,MaxAlarmLevelNum: %u,AlarmLevelNum: %u,BufferSize: %u",iSessionID,uiMaxAlarmLevelNum,*puiAlarmLevelNum,uiBufferSize);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    return pUserMgr->GetAlarmLevelMgr().GetAlarmLevelList(uiMaxAlarmLevelNum, puiAlarmLevelNum, pAlarmLevel, uiBufferSize);
}

// ��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmArea(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)
{	
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s",iSessionID,pReqXml);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    if (NULL == pUserMgr)
    {
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
     
    return pUserMgr->GetAlarmAreaMgr().AddAlarmArea(pReqXml, pRspXml);
}

//�޸ķ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs,  IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);	

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetAlarmAreaMgr().ModifyAlarmArea(pReqXml);
}

//ɾ������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmArea(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId)
{
	IVS_INFO_TRACE("SessionID: %d,uiAlarmAreaId: %u", iSessionID, uiAlarmAreaId);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().DeleteAlarmArea(pDomainCode, uiAlarmAreaId);
}

//�鿴�����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetAlarmAreaMgr().GetAlarmAreaList(pReqXml, pRspXml);
}

// �鿴��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaInfo (IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId,IVS_CHAR** pAlarmArea)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s, AlarmAreaId:%u", iSessionID, pDomainCode, uiAlarmAreaId);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetAlarmAreaMgr().GetAlarmAreaInfo(pDomainCode, uiAlarmAreaId, pAlarmArea);
}

// �ֶ�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s, AlarmAreaId:%u", iSessionID, pDomainCode, uiAlarmAreaId);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().StartAlarmAreaGuard(pDomainCode, uiAlarmAreaId);
}

// �ֶ�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s, AlarmAreaId:%u", iSessionID, pDomainCode, uiAlarmAreaId);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().StopAlarmAreaGuard(pDomainCode, uiAlarmAreaId);
}

// ���������ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AllocAreaGuardPlan (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().AllocAreaGuardPlan(pReqXml);
}

// �޸Ĳ����ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAreaGuardPlan(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);	

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetAlarmAreaMgr().ModifyAreaGuardPlan(pReqXml);
}

// ɾ�������ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAreaGuardPlan(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s, uiAlarmAreaId:%u", iSessionID, pDomainCode, uiAlarmAreaId);
	
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().DeleteAreaGuardPlan(pDomainCode, uiAlarmAreaId);
}

// �鿴�����ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAreaGuardPlan (IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId, IVS_CHAR** pRspXml)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmAreaID:%u", iSessionID, uiAlarmAreaId);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmAreaMgr().GetAreaGuardPlan(pDomainCode, uiAlarmAreaId, pRspXml);
}

// �����澯��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLinkage(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs,  IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().AddAlarmLinkage(pReqXml, pRspXml);
}

// �޸ĸ澯��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().ModifyAlarmLinkage(pReqXml);
}

// ɾ���澯��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLinkage(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID, pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().DeleteAlarmLinkage(pReqXml);
}

//�鿴�澯���������б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmlinkageList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s", iSessionID,pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().GetAlarmlinkageList(pReqXml, pRspXml);
}

//��ѯ�澯��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkage (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)

{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml :%s", iSessionID,pReqXml);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().GetAlarmLinkage(pReqXml, pRspXml);
}

//�澯ȷ��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmConfirm(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo)
{	
	IVS_INFO_TRACE("SessionID: %d,AlarmEventID: %llu", iSessionID,ullAlarmEventID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmOperateInfo, IVS_PARA_INVALID);
	CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);
	
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ConfirmAlarm(ullAlarmEventID, pAlarmInCode, pAlarmOperateInfo);
}

//�澯����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCancel(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmEventID: %llu", iSessionID,ullAlarmEventID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmOperateInfo, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().CancelAlarm(ullAlarmEventID, pAlarmInCode, pAlarmOperateInfo);
}

//�澯��Ȩ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCommission (IVS_INT32 iSessionID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission)
{
	IVS_INFO_TRACE("SessionID: %d", iSessionID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmCommission, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAlarmMgr().AlarmCommission(pAlarmInCode, pAlarmCommission);
}

//��Ȩ�澯�û���֤
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UserAuthentication(IVS_INT32 iSessionID, const IVS_CHAR *pPWD)
{
	IVS_INFO_TRACE("SessionID: %d, ", iSessionID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pPWD, IVS_PARA_INVALID);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().UserAlarmAuthorization(pPWD);
}

// �澯��Ϣ�ϱ����澯���ͣ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmReport(IVS_INT32 iSessionID, const IVS_ALARM_REPORT* pAlarmReport)
{
	IVS_INFO_TRACE("SessionID: %d", iSessionID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmReport, IVS_PARA_INVALID);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().AlarmReport(pAlarmReport);
}

// �������澯�ϱ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ManualAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReq, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,Req: %s", iSessionID,pReq);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ManualAlarm(pReq, pRsp);
}

//��������������澯
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmOutCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,AlarmOutCode: %s", iSessionID,pAlarmOutCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().StartAlarmOut(pAlarmOutCode);
}

//ֹͣ����������澯
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmOutCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,AlarmOutCode: %s", iSessionID,pAlarmOutCode);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().StopAlarmOut(pAlarmOutCode);
}

//��ѯ�澯��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventInfo(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmEventID: %llu",iSessionID,ullAlarmEventID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmInCode, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pAlarmEvent, IVS_OPERATE_MEMORY_ERROR);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetAlarmInfo(ullAlarmEventID, pAlarmInCode, pAlarmEvent);
}
// ��ѯ��ʷ�澯��Ϣ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize)
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u", iSessionID,uiBufferSize);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmEventList, IVS_OPERATE_MEMORY_ERROR);
	
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetAlarmList( pUnifiedQuery, pAlarmEventList, uiBufferSize);
}

//�û���¼
IVS_INT32 __SDK_CALL IVS_SDK_Login(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId)
{
	IVS_INFO_TRACE("");
    CHECK_POINTER(pSessionId, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pLoginReqInfo, IVS_OPERATE_MEMORY_ERROR);
    
	*pSessionId = SDK_SESSIONID_INIT; // ��ʼ��sessionId
    return g_pNvs->Login(pLoginReqInfo, pSessionId);
}//lint !e818


//�û������¼;
IVS_INT32 __SDK_CALL IVS_SDK_LoginByTicket(IVS_LOGIN_INFO_EX* pLoginReqInfoEx, IVS_INT32* pSessionId)
{
	IVS_DEBUG_TRACE("TicketID: %s, SerIP: [%s : %d], DomainName: %s, MachineName: %s, ClientType: %d, LoginType: %d", 
		pLoginReqInfoEx->cTicketID, pLoginReqInfoEx->stIP.cIP, pLoginReqInfoEx->uiPort, pLoginReqInfoEx->cDomainName, 
		pLoginReqInfoEx->cMachineName, pLoginReqInfoEx->uiClientType, pLoginReqInfoEx->uiLoginType);
	CHECK_POINTER(pSessionId, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLoginReqInfoEx, IVS_OPERATE_MEMORY_ERROR);

	*pSessionId = SDK_SESSIONID_INIT; // ��ʼ��sessionId;
	return g_pNvs->LoginByTicket(pLoginReqInfoEx, pSessionId);
}


//�û���¼
IVS_INT32 __SDK_CALL IVS_SDK_WinUserLogin(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId)
{
	IVS_INFO_TRACE("");

	CHECK_POINTER(pSessionId, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLoginReqInfo, IVS_OPERATE_MEMORY_ERROR);
	*pSessionId = SDK_SESSIONID_INIT; // ��ʼ��sessionId
	return  g_pNvs->WinUserLogin(pLoginReqInfo, pSessionId);
}//lint !e818

//�����û�����
IVS_INT32 __SDK_CALL IVS_SDK_BackupUserData(IVS_INT32 iSessionID, const IVS_CHAR* pUserDataFile)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
    CHECK_POINTER(pUserDataFile, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetUserDataMgr().BackupUserData(pUserDataFile);
}

//��ȡ��¼��Ӧ��Ϣ
// IVS_INT32 __SDK_CALL IVS_SDK_GetLoginRspInfo(IVS_INT32 iSessionID, LOGIN_RSP_INFO* pLoginRspInfo)
// {
// 	IVS_INFO_TRACE("SessionID: %d",iSessionID);
// 	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pLoginRspInfo, IVS_OPERATE_MEMORY_ERROR);
// 	int iRet = g_pNvs->GetLoginRspInfo(iSessionID, pLoginRspInfo);
// 	
// 	return iRet;
// }

//��ȡ��½�û����û�ID
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserID(IVS_INT32 iSessionID, IVS_UINT32* puiUserID)
{
  	IVS_INFO_TRACE("SessionID: %d", iSessionID);    
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(puiUserID, IVS_OPERATE_MEMORY_ERROR);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    IVS_UINT32 uiUserID = pUserMgr->GetLoginMgr().GetUserID();
    *puiUserID = uiUserID;
    return IVS_SUCCEED;
}

//��ȡ��¼��Ӧ��Ϣ
IVS_INT32 __SDK_CALL IVS_SDK_Logout( IVS_INT32 iSessionID )
{
	IVS_INFO_TRACE("SessionID: %d", iSessionID);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	int iRet = g_pNvs->Logout(iSessionID);
	return iRet;
}

// ɾ���豸��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceFromGroup( IVS_INT32 iSessionID, 
                                                                const IVS_CHAR* pTargetDomainCode,
                                                                IVS_UINT32 uiDeviceNum,
                                                                const IVS_DEV_GROUP_CAMERA* pGroupCameraList)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pGroupCameraList, IVS_PARA_INVALID);
	CHECK_POINTER(pTargetDomainCode, IVS_PARA_INVALID);

 	IVS_INFO_TRACE("SessionID: %d,TargetDomainCode: %s,DeviceNum: %u",iSessionID,pTargetDomainCode,uiDeviceNum);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().DeleteDeviceFromGroup(pTargetDomainCode, uiDeviceNum, pGroupCameraList);
}

//�����豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDevice(IVS_INT32 iSessionID, 
                                                   const IVS_CHAR* pDomainCode,
                                                   IVS_UINT32 uiDeviceNum,
                                                   const IVS_DEVICE_OPER_INFO* pDevList,
                                                   IVS_DEVICE_OPER_RESULT_LIST* pResultList,
                                                   IVS_UINT32 uiResultBufferSize)
{
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDevList, IVS_PARA_INVALID);
	CHECK_POINTER(pResultList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);

	IVS_INFO_TRACE("SessionID: %d,TargetDomainCode: %s,DeviceNum: %u,ResultBufSize: %u",
		           iSessionID,pDomainCode,uiDeviceNum,uiResultBufferSize);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().AddDevice(pDomainCode, uiDeviceNum, pDevList, pResultList, uiResultBufferSize);
}

//�������豸�����豸����
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceConfig(IVS_INT32 iSessionID,
                                                          const IVS_CHAR* pDevCode,
                                                          IVS_UINT32 uiConfigType,
                                                          IVS_VOID* pBuffer,
                                                          IVS_UINT32 uiBufSize)
{
    CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,DevCode: %s,ConfigType: %u,BufSize: %u", iSessionID,pDevCode,uiConfigType,uiBufSize);
    CHECK_POINTER(pBuffer, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceParaConfig().SetDeviceConfig(pDevCode, uiConfigType, pBuffer, uiBufSize);
}//lint !e818

//�������豸�����豸����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig(IVS_INT32 iSessionID,
                                                        const IVS_CHAR* pDevCode, 
                                                        IVS_UINT32 uiConfigType,
                                                        IVS_VOID* pBuffer,
                                                        IVS_UINT32 uiBufSize)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevCode: %s,ConfigType: %u,BufSize: %u",iSessionID,pDevCode,uiConfigType,uiBufSize);

 	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceParaConfig().GetDeviceConfig(pDevCode, uiConfigType, pBuffer, uiBufSize);
}

//ɾ���豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDevice(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiDeviceNum,
	const IVS_DEVICE_CODE* pDevCodeList, IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize)	
{
	CHECK_POINTER(g_pNvs,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCodeList,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pResultList,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode,IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,DeviceNum: %u,BufSize: %u", iSessionID,pDomainCode,uiDeviceNum,uiResultBufferSize);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().DeleteDevice(pDomainCode,uiDeviceNum,pDevCodeList,pResultList,uiResultBufferSize);
}

//��ѯ�豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryDeviceList(IVS_INT32 iSessionID, 
	const IVS_CHAR* pDomainCode,
	IVS_UINT32 uiDeviceType, 
	const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
	IVS_VOID* pBuffer, 
	IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pQueryFormat, IVS_PARA_INVALID);
    CHECK_POINTER(pBuffer, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
    CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,DeviceType: %u,BufSize: %u", iSessionID,pDomainCode,uiDeviceType,uiBufferSize);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().QueryDeviceList(uiDeviceType, pDomainCode,pQueryFormat, pBuffer, uiBufferSize);
}
//��ȡ��ȡ�豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceList(IVS_INT32 iSessionID,
	                                                   IVS_UINT32 uiDeviceType,
	                                                   const IVS_INDEX_RANGE* pIndexRange,
	                                                   IVS_VOID* pDeviceList,
	                                                   IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
	CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevType: %u,BufSize: %u",iSessionID,uiDeviceType,uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetDeviceList(uiDeviceType, pIndexRange, pDeviceList, uiBufferSize);
}

//��ѯ���豸��ϸ��Ϣ	
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceInfo(IVS_INT32 iSessionID, 
                                                       const IVS_CHAR* pDomainCode,
                                                       const IVS_CHAR* pDevCode, 
                                                       IVS_DEVICE_INFO* pDeviceInfo)
{
    CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
    CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
    CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,DevCode: %s", iSessionID,pDomainCode,pDevCode);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetDeviceInfo(pDomainCode, pDevCode, pDeviceInfo);
}//lint !e818

//�����豸��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceGroup (IVS_INT32 iSessionID, 
	                                                     const IVS_DEV_GROUP* pDevGroup,
  	                                                     IVS_UINT32* puiDevGroupID)
{
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
    CHECK_POINTER(pDevGroup, IVS_PARA_INVALID);
    CHECK_POINTER(puiDevGroupID, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevGroupID: %u", iSessionID,*puiDevGroupID);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetDeviceGroupMgr().AddDeviceGroup(pDevGroup, puiDevGroupID);
}


//��ѯ�豸���б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceGroup(IVS_INT32 iSessionID,
                                                        const IVS_CHAR* pDomainCode,
                                                        const IVS_CHAR* pDevGroupCode,
                                                        IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
                                                        IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDevGroupCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDeviceGroupList, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,GroupID: %s,BufSize: %u",iSessionID,pDomainCode,pDevGroupCode,uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().GetDeviceGroup(pDomainCode, pDevGroupCode, pDeviceGroupList, uiBufferSize);
}


//�ƶ��豸�����豸 
// IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveDeviceToGroup(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp)
// {
// 	IVS_INFO_TRACE("SessionID: %d",iSessionID);
// 	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pRsp, IVS_OPERATE_MEMORY_ERROR);
// 	int iRet = g_pNvs->GetDeviceGroup(iSessionID, pReq, pRsp);
// 	return iRet;
// }

//ɾ���豸��
// IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceGroup(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp)
// {
//	IVS_INFO_TRACE("SessionID: %d",iSessionID);
// 	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pRsp, IVS_OPERATE_MEMORY_ERROR);
// 	int iRet = g_pNvs->GetDeviceGroup(iSessionID, pReq, pRsp);
// 	return iRet;
// }

//��ȡ�豸�Ļ�������
// IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig( IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp )
// {
//	IVS_INFO_TRACE("SessionID: %d",iSessionID);
// 	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pRsp, IVS_OPERATE_MEMORY_ERROR);
// 	int iRet = g_pNvs->GetDeviceConfig(iSessionID, pReq, pRsp);
// 	return iRet;
// }

#ifdef WIN32
//��ʼʵʱ���
IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlay( IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, HWND hWnd, IVS_ULONG* pHandle )
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRealplayPara, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
    
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRealPlayMgr().StartRealPlay(pRealplayPara, pCameraCode, hWnd, pHandle);
}
#endif


//��ʼʵʱ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBRaw(IVS_INT32 iSessionID, 
	                                                        IVS_REALPLAY_PARAM* pRealplayPara, 
	                                                        const IVS_CHAR* pCameraCode, 
	                                                        RealPlayCallBackRaw fRealPlayCallBackRaw, 
	                                                        void* pUserData, 
	                                                        IVS_ULONG* pHandle )
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pRealplayPara, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pUserData, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s", iSessionID, pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRealPlayMgr().StartRealPlayCBRaw(pRealplayPara, pCameraCode, fRealPlayCallBackRaw, pUserData, pHandle);
}
#ifdef WIN32
//��ʼʵʱ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBFrame( IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData, IVS_ULONG* pHandle )
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRealplayPara, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pUserData, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID, pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRealPlayMgr().StartRealPlayCBFrame(pRealplayPara, pCameraCode, fRealPlayCallBackFrame, pUserData, pHandle);
}
#endif
//ֹͣʵʱ���
IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlay( IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle )
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRealPlayMgr().StopRealPlay(ulPlayHandle);
}

#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPlayWnd(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	CRealPlay *pRealPlay = dynamic_cast<CRealPlay *>(pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pRealPlay)
	{
		BP_RUN_LOG_INF("real play zoom","NA");
		return pUserMgr->GetRealPlayMgr().AddPlayWindow(ulPlayHandle, hWnd);
	}
	CPlayback *pPlayback = dynamic_cast<CPlayback *>(pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pPlayback)
	{
		BP_RUN_LOG_INF("paly back zoom","NA");
		return pUserMgr->GetPlaybackMgr().AddPlayWindow(ulPlayHandle, hWnd);
	}
	return IVS_FAIL;
}
#endif
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPlayWnd(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	CRealPlay *pRealPlay = dynamic_cast<CRealPlay *>(pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pRealPlay)
	{
		BP_RUN_LOG_INF("real play zoom","NA");
		return pUserMgr->GetRealPlayMgr().DelPlayWindow(ulPlayHandle, hWnd);
	}
	CPlayback *pPlayback = dynamic_cast<CPlayback *>(pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pPlayback)
	{
		BP_RUN_LOG_INF("paly back zoom","NA");
		return pUserMgr->GetPlaybackMgr().DelPlayWindow(ulPlayHandle, hWnd);
	}
	return IVS_FAIL;
}
#endif
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPlayWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	CRealPlay *pRealPlay = dynamic_cast<CRealPlay *>(pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pRealPlay)
	{
		BP_RUN_LOG_INF("real play zoom","NA");
		return pUserMgr->GetRealPlayMgr().AddPlayWindowPartial(ulPlayHandle, hWnd, pPartial);
	}
	CPlayback *pPlayback = dynamic_cast<CPlayback *>(pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pPlayback)
	{
		BP_RUN_LOG_INF("paly back zoom","NA");
		return pUserMgr->GetPlaybackMgr().AddPlayWindowPartial(ulPlayHandle, hWnd, pPartial);
	}
	return IVS_FAIL;
}
#endif
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UpdateWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	CRealPlay *pRealPlay = dynamic_cast<CRealPlay *>(pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pRealPlay)
	{
		BP_RUN_LOG_INF("real play zoom","NA");
		return pUserMgr->GetRealPlayMgr().UpdateWindowPartial(ulPlayHandle, hWnd, pPartial);
	}
	CPlayback *pPlayback = dynamic_cast<CPlayback *>(pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle));
	if (NULL != pPlayback)
	{
		BP_RUN_LOG_INF("paly back zoom","NA");
		return pUserMgr->GetPlaybackMgr().UpdateWindowPartial(ulPlayHandle, hWnd, pPartial);
	}
	return IVS_FAIL;
}
#endif
#ifdef WIN32
//��ȡ�Ѳ���ʱ��
IVS_INT32 __SDK_CALL IVS_SDK_GetPlayedTime(IVS_INT32 iSessionID, IVS_ULONG ulHandle, IVS_UINT32 *puPlayedTime)
{
	IVS_INFO_TRACE("SessionID: %d,Handle: %lu",iSessionID,ulHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(puPlayedTime, IVS_PARA_INVALID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    IVS_INT32 iRet  = IVS_FAIL;
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulHandle);
	if(NULL != mediaBase)
	{
		unsigned int port = pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulHandle);
		iRet = mediaBase->GetPlayedTime(port,puPlayedTime);
	}
	return iRet;
}
#endif
//����ʵ������ģʽ������������
IVS_INT32 __SDK_CALL IVS_SDK_SetPlayQualityMode(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iPlayQualityMode)
{
	IVS_INFO_TRACE("SessionID: %d,Handle: %lu,PlayQualityMode: %d",iSessionID,ulHandle,iPlayQualityMode);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    IVS_INT32  iRet = IVS_FAIL;
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	
	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulHandle);
	if(NULL != mediaBase)
	{
		unsigned int port = pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulHandle);
		iRet = mediaBase->SetPlayQualityMode(port,iPlayQualityMode);
	}
	
	return iRet;
}

//��ѯMU����ԴԤ���Ƿ�ﵽ���޼���ǰ�������Ƶ���Ƿ�����
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoNumAndResource (IVS_INT32 iSessionID, 
																 const IVS_CHAR* pCameraCode,
																 IVS_BOOL* bReachLimit,
																 IVS_BOOL* bWhetherCited)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(bReachLimit, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(bWhetherCited, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetVideoNumAndResource(pCameraCode,bReachLimit,bWhetherCited);
}
#endif

static std::string GetPlayBackCurrentTime(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
{
    //ʱ���ʽΪ���뼶�𣬳���1000ת��Ϊ��
    IVS_UINT64 tickNow = 0;
    (void)IVS_SDK_GetCurrentFrameTick(iSessionID, ulHandle, &tickNow);
    time_t stTime_t = tickNow / 1000; //lint !e713

    std::string strCurrentTime("");
    //��ȡ��ʽ����string Time
    CMediaBase::UTCTime2Str(stTime_t, strCurrentTime, IVS_DATE_TIME_FORMAT);
    BP_DBG_LOG("IVS_SDK_SetWaterMarkVerify, strCurrentTime:%s.", strCurrentTime.c_str());
    return strCurrentTime;
}
static void BuildUpdateWaterAlarmXml(const std::string strUserIdDomainCode, const std::string strDomianCode,
                                     IVS_ALARM_WATERMARK_INFO &stWaterMarkInfo, std::string& strReq)
{
    CXml xml;
    (void)xml.AddDeclaration("1.0","UTF-8","");
    (void)xml.AddElem("Content");

    (void)xml.AddChildElem("LoginInfo");
    (void)xml.IntoElem();
    (void)xml.AddChildElem("UserIDDomainCode");
    (void)xml.IntoElem();
    (void)xml.SetElemValue(strUserIdDomainCode.c_str());
    xml.OutOfElem();

    (void)xml.AddElem("DomainCode");
    (void)xml.IntoElem();
    (void)xml.SetElemValue(strDomianCode.c_str());
    (void)xml.AddElem("AlarmEventID");
    (void)xml.IntoElem();
    
    (void)xml.SetElemValue(CToolHelp::IntToStr(stWaterMarkInfo.ullAlarmEventID).c_str());
    (void)xml.AddElem("OccurStopTime");
    (void)xml.IntoElem();
    (void)xml.SetElemValue(stWaterMarkInfo.cEndTime);
    xml.OutOfElem();
    unsigned int xmlLen = 0;
    const IVS_CHAR* pReq = xml.GetXMLStream(xmlLen);
    if (NULL != pReq)
    {
        strReq = pReq;
    }
    
    return;
}

static IVS_INT32 SendWaterAlarmToSmu(IVS_INT32 iSessionID, IVS_ULONG ulHandle, ST_WATERMARK_ALARM_INFO &stWaterMarkAlarmInfo, std::string& strRsp)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_PARA_INVALID);

    //������ȡCameraCode��DomainCode
    std::string strCameraID("");   
    std::string strCameraCode("");
    std::string strDomainCode("");
    CMediaBase* pMediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulHandle);
    if (NULL != pMediaBase)
    {
        pMediaBase->GetCameraID(strCameraID);

        IVS_INT32 iRet = CToolsHelp::SplitCameraDomainCode(strCameraID, strCameraCode, strDomainCode);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(iRet,"DO Player Exception", "SplitCameraCode failed, RetCode=%d.", iRet);
            return IVS_FAIL;
        }
    }
    
    //��ȡ�û�������룬�û�����DomainCode
    std::string strUserDomainCode(""); 
    pUserMgr->GetDomainCode(strUserDomainCode);
    
    //����XML����SMU�������
    CSendNssMsgInfo sendNssMsgInfo;	
    sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
    sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
    sendNssMsgInfo.SetCameraCode(strCameraCode);
    sendNssMsgInfo.SetDomainCode(strDomainCode);

    sendNssMsgInfo.SetReqID(NSS_WATERMARK_ALARM_UPDATE_REQ);
    std::string strReq("");
    //��װ����SMU��XML
    BuildUpdateWaterAlarmXml(strUserDomainCode, strDomainCode, stWaterMarkAlarmInfo.stWaterMarkInfo, strReq);
    BP_DBG_LOG("BuildUpdateWaterAlarmXml, strReq��%s", strReq.c_str());

    sendNssMsgInfo.SetReqData(strReq);
    IVS_INT32 iNeedRedirect = IVS_FAIL;
    return pUserMgr->SendCmd(sendNssMsgInfo, strRsp, iNeedRedirect);
}


//���ÿ���/ֹͣУ��ˮӡ
IVS_INT32 __SDK_CALL IVS_SDK_SetWaterMarkVerify(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_BOOL bIsStart)
{
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu, IsStart: %d",iSessionID, ulHandle, (int)bIsStart);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    IVS_INT32  iRet = IVS_FAIL;
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	
	//CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulHandle);
	//if(NULL != mediaBase)
	//{
	//	unsigned int port = pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulHandle);
	//	iRet = mediaBase->SetWaterMarkVerify(port,bIsStart);
	//}

	//ֻ�Իط���ˮӡУ��
	CMediaBase* mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulHandle);
	if (NULL != mediaBase)
	{
		unsigned int port = pUserMgr->GetPlaybackMgr().GetPlayerChannelByHandle(ulHandle);
		iRet = mediaBase->SetWaterMarkVerify(port, bIsStart);
        BP_DBG_LOG("SetWaterMarkVerify, iRet:%d", iRet);
	}
	
    if ((IVS_SUCCEED == iRet) && (1 == bIsStart))
    {
        ST_WATERMARK_ALARM_INFO stWaterMarkAlarmInfo;
        memset(&stWaterMarkAlarmInfo, 0, sizeof(stWaterMarkAlarmInfo));  

        //��ȡˮӡ������Ϣ
        pUserMgr->GetWaterMarkInfo(ulHandle, stWaterMarkAlarmInfo);
        BP_DBG_LOG("IVS_SDK_SetWaterMarkVerify, GetWaterMarkInfo, ulHandle:%d, cCameraCode:%s, cStartTime:%s, ullAlarmEventID:%d",
            ulHandle, stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, 
            stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID);

        //��ȡ��ǰ¼�񲥷�ʱ��
        std::string strTime = GetPlayBackCurrentTime(iSessionID, ulHandle);
        BP_DBG_LOG("IVS_SDK_SetWaterMarkVerify, strTime:%s.", strTime.c_str());

        std::string strRsp("");
        //�ж����ֹͣˮӡУ�鲢����Ҫ�ϱ�ˮӡ�澯�Ľ���ʱ�䣬���͸澯��Ϣ��SMU���ϱ���OCX->CU
        if (/*(1 == bIsStart) && */(stWaterMarkAlarmInfo.bIsNeedReport))
        {
			(void)CToolsHelp::Memcpy(stWaterMarkAlarmInfo.stWaterMarkInfo.cEndTime, IVS_TIME_LEN, strTime.c_str(), strTime.length());
            //��װ���͸�SMU��ˮӡ�澯��Ϣ      
            iRet = SendWaterAlarmToSmu(iSessionID, ulHandle, stWaterMarkAlarmInfo, strRsp);
            //���SMU��Ӧ�ɹ����ϱ���OCX
            if (IVS_SUCCEED == iRet)
            {
                pUserMgr->RemoveWaterMarkInfoFromMap(ulHandle);

                IVS_ALARM_WATERMARK_INFO stWaterMarkInfo;
                memset(&stWaterMarkInfo, 0, sizeof(stWaterMarkInfo));

                memcpy(stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, sizeof(stWaterMarkInfo.cCameraCode));
                memcpy(stWaterMarkInfo.cStartTime, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, sizeof(stWaterMarkInfo.cStartTime));
				//memcpy(stWaterMarkInfo.cEndTime, strTime.c_str(), sizeof(stWaterMarkInfo.cEndTime));
                //strncpy(stWaterMarkInfo.cEndTime, strTime.c_str(), sizeof(stWaterMarkInfo.cEndTime));
				(void)CToolsHelp::Strncpy(stWaterMarkInfo.cEndTime, sizeof(stWaterMarkInfo.cEndTime), strTime.c_str(), strTime.length());
                stWaterMarkInfo.ullAlarmEventID = stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID;

                BP_DBG_LOG("IVS_SDK_SetWaterMarkVerify>>>>>>, strCameraCode:%s, cStartTime:%s, strTime:%s, ullAlarmEventID:%d", 
                    stWaterMarkInfo.cCameraCode, stWaterMarkInfo.cStartTime, stWaterMarkInfo.cEndTime,
                    stWaterMarkInfo.ullAlarmEventID);

                char Buf[sizeof(IVS_ALARM_WATERMARK_INFO) + sizeof(unsigned long)];
                IVS_ALARM_WATERMARK_INFO *recordInfo = (IVS_ALARM_WATERMARK_INFO *)(Buf);
                unsigned long *pHandle = (unsigned long *)(recordInfo + 1);//lint !e826
                *pHandle = ulHandle;

                (void)CToolsHelp::Memcpy(recordInfo, sizeof(IVS_ALARM_WATERMARK_INFO), &stWaterMarkInfo, sizeof(IVS_ALARM_WATERMARK_INFO));

                BP_DBG_LOG("IVS_SDK_SetWaterMarkVerify, pHandle:%d, ulHandle:%d",*pHandle, ulHandle);

                (void)CEventCallBackJob::instance().PostEvent(iSessionID, IVS_EVENT_REPORT_WATERMARK_ALARM, Buf, sizeof(Buf)+1);
            }
            else
            {
                //��¼��־
                BP_RUN_LOG_ERR(IVS_FAIL, "Send WaterAlarm To SMU Fail.", "EventType:[%d]", IVS_EVENT_REPORT_WATERMARK_ALARM);
            }

        }
    }
    
	return iRet;
}

//ˮӡ�澯ȷ��
IVS_INT32 __SDK_CALL IVS_SDK_WatermarkAlarmConfirm (IVS_INT32 iSessionID, const IVS_WATERMARK_ALARM_CONFIRM* pConfirmInfo)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pConfirmInfo, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ConfirmWaterAlarm(pConfirmInfo);
}

//ˮӡ�澯��ѯ
IVS_INT32 __SDK_CALL IVS_SDK_QueryWatermarkAlarmList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
	IVS_VOID* pBuffer,IVS_UINT32 uiBufferSize)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryFormat, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().QueryWatermarkAlarmList(pQueryFormat,pBuffer,uiBufferSize);
}

//ˮӡ�澯����
IVS_INT32 __SDK_CALL IVS_SDK_WatermarkAlarmCancel(IVS_INT32 iSessionID, const IVS_WATERMARK_ALARM_CANCEL* pCancelInfo)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCancelInfo, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().CancelWaterAlarm(pCancelInfo);
}



//���ÿ�����ʾ
#ifdef WIN32
IVS_INT32 __SDK_CALL IVS_SDK_SetDisplayScale(IVS_INT32 iSessionID,IVS_ULONG ulHandle,IVS_INT32 iDisplayScale)
{
	IVS_INFO_TRACE("SessionID: %d,Handle: %lu,DisplayScale: %d",iSessionID,ulHandle,iDisplayScale);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	unsigned int uiPort = 0;
	CMediaBase* mediaBase = GetMediaBaseAndPortInfo(iSessionID,ulHandle,uiPort);
	IVS_INT32 iRet = IVS_FAIL;
	if (NULL != mediaBase)
	{
		iRet = mediaBase->SetDisplayScale(uiPort,static_cast<IVS_UINT32>(iDisplayScale));
	}
	return iRet;
}
#endif
//��ѯ������־��
 IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLog(IVS_INT32 iSessionID,
	                                         const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
	                                         IVS_OPERATION_LOG_LIST* pResOperLogList,
	                                         IVS_UINT32 uiBufSize)
 {
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u",iSessionID,uiBufSize);
    CHECK_POINTER(pUnifiedQuery, IVS_PARA_INVALID);
	CHECK_POINTER(pResOperLogList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs,IVS_OPERATE_MEMORY_ERROR );
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetLogMgr().GetOperationLog(pUnifiedQuery,pResOperLogList,uiBufSize);
 }

#ifdef WIN32
 //��չ��ѯ������־
 IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLogEx(IVS_INT32 iSessionID,
	 const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
	 const IVS_QUERY_OPTLOG_EX *pOptLogQueryEx,
	 IVS_OPERATION_LOG_LIST* pResOperLogList,
	 const IVS_UINT32 uiBufSize)
 {
	 CHECK_POINTER(pUnifiedQuery, IVS_PARA_INVALID);
	 CHECK_POINTER(pResOperLogList, IVS_PARA_INVALID);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pOptLogQueryEx, IVS_PARA_INVALID);
	 IVS_INFO_TRACE("SessionID: %d, NVRCode: %s, ModuleName: %s", 
		 iSessionID, pOptLogQueryEx->cNVRCode, pOptLogQueryEx->cModuleName);

	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	 return pUserMgr->GetLogMgr().GetOperationLogEx(pUnifiedQuery, pOptLogQueryEx, pResOperLogList, uiBufSize);
 }
#endif

#ifdef WIN32
IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBack(IVS_INT32 iSessionID,
                                                                const IVS_CHAR* pCameraCode,
                                                                const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                HWND hWnd,
                                                                IVS_ULONG* pHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	IVS_INFO_TRACE("Start Platform PlayBack SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().StartPlatformPlayBack(pCameraCode, pPlaybackParam, hWnd, pHandle);
}
#endif
//ֹͣƽ̨¼��ط�
IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

    ST_WATERMARK_ALARM_INFO stWaterMarkInfo;
    memset(&stWaterMarkInfo, 0, sizeof(stWaterMarkInfo));
    pUserMgr->GetWaterMarkInfo(ulPlayHandle, stWaterMarkInfo);

    //���ˮӡУ�鿪���У���ر�ˮӡУ��
    if (stWaterMarkInfo.bIsOpenedWater)
    {
        IVS_INT32 iRet = IVS_SDK_SetWaterMarkVerify(iSessionID, ulPlayHandle, 1);
        BP_DBG_LOG("Stop Platform PlayBack, Stop WaterMark Verify. Retcode:%d", iRet);
        //ͣ100ms����ֹ���ڵĲ��ž���Ѿ���ע��
		VOS_Sleep(100);
    }

	return pUserMgr->GetPlaybackMgr().StopPlatformPlayBack(ulPlayHandle);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackByIP(IVS_INT32 iSessionID,
																const IVS_CHAR* pCameraCode,
																const IVS_PLAYBACK_PARAM* pPlaybackParam,
																const IVS_MEDIA_ADDR* pMediaAddrDst,
																IVS_MEDIA_ADDR* pMediaAddrSrc,
																IVS_ULONG* pHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrDst, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrSrc, IVS_PARA_INVALID);
	CHECK_POINTER(pHandle, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, CameraCode: %s, IP: %s, VideoPort: %u, AudioPort: %u",
		iSessionID, pCameraCode, pMediaAddrDst->stIP.cIP, pMediaAddrDst->uiVideoPort, pMediaAddrDst->uiAudioPort);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	ULONG ulHandle = *pHandle;
	IVS_MEDIA_ADDR stMediaAddr = {0};
	IVS_INT32 iRet = pUserMgr->GetTelepresenceMgr().StartPlatformPlayBackByIP(pCameraCode, *pPlaybackParam, *pMediaAddrDst, stMediaAddr, ulHandle);
	*pHandle = ulHandle;
	bool bSucc = CToolsHelp::Memcpy(pMediaAddrSrc, sizeof(IVS_MEDIA_ADDR), &stMediaAddr, sizeof(IVS_MEDIA_ADDR));
	if(!bSucc)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Start Platform PlayBack ByIP", "Memcpy failed.");
	}
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackByIP(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu",iSessionID, ulPlayHandle);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetTelepresenceMgr().StopPlayByIP(ulPlayHandle);
}

//��ͣƽ̨¼��ط�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackPauseByIP(IVS_INT32 iSessionID,
	IVS_ULONG ulPlayHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu",iSessionID, ulPlayHandle);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetTelepresenceMgr().PlatformPlayBackPauseByIP(ulPlayHandle);
}


//�ָ�ƽ̨¼��ط�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackResumeByIP(IVS_INT32 iSessionID,
	IVS_ULONG ulPlayHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu",iSessionID, ulPlayHandle);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetTelepresenceMgr().PlatformPlayBackResumeByIP(ulPlayHandle);
}


//����ƽ̨¼�������ط��ٶ�
IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeedByIP(IVS_INT32 iSessionID, 
	IVS_ULONG ulPlayHandle, 
	IVS_FLOAT fSpeed)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu Speed:%f", iSessionID, ulPlayHandle, fSpeed);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetTelepresenceMgr().SetPlayBackSpeedByIP(ulPlayHandle, fSpeed);
}


//����ƽ̨¼�������ط�ʱ��
IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTimeByIP(IVS_INT32 iSessionID, 
	IVS_ULONG ulPlayHandle, 
	IVS_UINT32 uiTime)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu Time:%d", iSessionID, ulPlayHandle, uiTime);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetTelepresenceMgr().SetPlayBackTimeByIP(ulPlayHandle, uiTime);
}

// ��ʼ����¼��ط�
#ifdef WIN32
IVS_INT32 __SDK_CALL IVS_SDK_StartDisasterRecoveryPlayBack (
    IVS_INT32 iSessionID,
    const IVS_CHAR* pCameraCode,
    const IVS_PLAYBACK_PARAM* pPlaybackParam,
    HWND hWnd,
    IVS_ULONG* pHandle)

{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
    IVS_INFO_TRACE("Start Disaster Recovery PlayBack SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetPlaybackMgr().StartDisasterRecoveryPlayBack(pCameraCode, pPlaybackParam, hWnd, pHandle);
}
#endif
// ֹͣ����¼��ط�
#ifdef WIN32
IVS_SDK_API IVS_INT32  __SDK_CALL  IVS_SDK_StopDisasterRecoveryPlayBack(
    IVS_INT32 iSessionID,
    IVS_ULONG ulPlayHandle)
{
    IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);	
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetPlaybackMgr().StopDisasterRecoveryPlayBack(ulPlayHandle);
}
#endif
//��ʼƽ̨¼��طţ��Իص���ʽ���ƴ֡�����������
IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBRaw(IVS_INT32 iSessionID,
														const IVS_CHAR* pCameraCode,
														const IVS_PLAYBACK_PARAM* pPlaybackParam,
														PlayBackCallBackRaw fPlayBackCallBackRaw, 
														void* pUserData,
														IVS_ULONG* pHandle)
{	
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	CHECK_POINTER(fPlayBackCallBackRaw, IVS_PARA_INVALID);
	IVS_INFO_TRACE("Start Platform PlayBack CBRaw SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().StartPlatformPlayBackCB(pCameraCode, pPlaybackParam, fPlayBackCallBackRaw, pUserData, pHandle);
}//lint !e818
//��ʼƽ̨¼��طţ��Իص���ʽ���YUV����
#ifdef WIN32
IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBFrame(IVS_INT32 iSessionID,
	 													  const IVS_CHAR* pCameraCode,
														  const IVS_PLAYBACK_PARAM* pPlaybackParam,
														  PlayBackCallBackFrame fPlayBackCallBackFrame, 
														  void* pUserData,
														  IVS_ULONG* pHandle)
{
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	 CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	 CHECK_POINTER(fPlayBackCallBackFrame, IVS_PARA_INVALID);
	 IVS_INFO_TRACE("Start Platform PlayBack CBFrame SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPlaybackMgr().StartPlatformPlayBackCBFrame(pCameraCode, pPlaybackParam, fPlayBackCallBackFrame, pUserData, pHandle);
}//lint !e818
#endif


// ��ʼ����¼��ط�
#ifdef WIN32
IVS_INT32 IVS_SDK_StartLocalPlayBack (IVS_INT32 iSessionID, const IVS_CHAR* pFileName, 
	           const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam, HWND hWnd, IVS_ULONG* pHandle)
{
	IVS_INFO_TRACE("SessionID: %d, pFileName: %s",iSessionID, pFileName);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	
	return pUserMgr->GetPlaybackMgr().StartLocalPlayBack(pFileName, pPlaybackParam, hWnd, pHandle);
}
#endif
 // ֹͣ����¼��ط�
#ifdef WIN32
 IVS_INT32 IVS_SDK_StopLocalPlayBack(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	 return pUserMgr->GetPlaybackMgr().StopLocalPlayBack(ulPlayHandle);
 }


 IVS_INT32 __SDK_CALL IVS_SDK_PlayBackPause(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetPlaybackMgr().PlayBackPause(ulPlayHandle);
	return iRet;
 }

 IVS_INT32 __SDK_CALL IVS_SDK_PlayBackResume(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().PlayBackResume(ulPlayHandle);
 }

 IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackStatus(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_BOOL* pbPause)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 IVS_BOOL bPause = 0;//��ʼ��ֵΪ��ͣ;
	 int iRet = pUserMgr->GetPlaybackMgr().GetPlayBackStatus(ulPlayHandle, bPause);
	 *pbPause = bPause;
	 BP_RUN_LOG_INF("Get PlayBack Status", "Status[%d] Ret[%d]", bPause, iRet);
	 return iRet;
 }

 IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepForward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID, ulPlayHandle);
     CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPlaybackMgr().PlayBackFrameStepForward(ulPlayHandle);
 }


 IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepBackward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID, ulPlayHandle);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPlaybackMgr().PlayBackFrameStepBackward(ulPlayHandle);
 }
 

 IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT fSpeed)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu Speed:%f", iSessionID, ulPlayHandle, fSpeed);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPlaybackMgr().SetPlayBackSpeed(ulPlayHandle, fSpeed);
 }

 IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT* pfSpeed)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu", iSessionID, ulPlayHandle);

	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 float fSpeed = 0.0;
	 int iRet = pUserMgr->GetPlaybackMgr().GetPlayBackSpeed(ulPlayHandle, fSpeed);
	 *pfSpeed = fSpeed;
	 BP_RUN_LOG_INF("Get PlayBack Speed", "Speed[%f] Ret[%d]", fSpeed, iRet);
	 return iRet;
 }

 IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uiTime)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu,Time: %u",iSessionID,ulPlayHandle,uiTime);
	
    // ֧���϶�����ʼʱ��,0 == uiTime������Ϸ�
    /*if (0 == uiTime)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"IVS SDK SetPlayBackTime Failed.", "Reason:Param invalid, uiTime = 0");
		return IVS_PARA_INVALID;
	}*/
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().SetPlayBackTime(ulPlayHandle, uiTime);
 }

 IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_PLAYBACK_TIME* pPlayBackTime)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().GetPlayBackTime(ulPlayHandle, pPlayBackTime);
 }
 #endif
#ifdef WIN32
//��ʼǰ��¼��ط�
IVS_INT32 __SDK_CALL IVS_SDK_StartPUPlayBack(IVS_INT32 iSessionID,
                                    const IVS_CHAR* pCameraCode,
                                    const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                    HWND hWnd,
                                    IVS_ULONG* pHandle)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
    IVS_INFO_TRACE("Start PU Play Back SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetPlaybackMgr().StartPUPlayBack(pCameraCode, pPlaybackParam, hWnd, pHandle);
}

//��ʼǰ��¼��ط�
IVS_INT32 __SDK_CALL IVS_SDK_StopPUPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle)
{
    IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);	
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetPlaybackMgr().StopPUPlayBack(ulPlayHandle);
}
 #endif
 IVS_INT32 __SDK_CALL IVS_SDK_GetCurrentFrameTick(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT64* pTick)
 {
	 IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPlaybackMgr().GetCurrentFrameTick(ulPlayHandle, pTick);
 }

 /*
  * IVS_SDK_StartPlatformDownload
  * ����ƽ̨¼������
  */
IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
	                     const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle)
{
	CHECK_POINTER(pCameraCode,    IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDownloadParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pulHandle,      IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, CamCode: %s",iSessionID, pCameraCode);

	// ��������
    if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
		pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
	{
        BP_RUN_LOG_INF("Start Platform Record Download", "Name Rule=[%u]", pDownloadParam->uiNameRule);
		return IVS_PARA_INVALID;
	}

	// �ļ���ʽ
    if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
		pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
	{
        BP_RUN_LOG_INF("Start Platform Record Download", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
		return IVS_PARA_INVALID;
	}

	// �ļ��ָ�����
	if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
		pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	{
		BP_RUN_LOG_INF("Start Platform Record Download", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
		return IVS_PARA_INVALID;
	}
    
	// �ļ��ָ�����ֵ
    if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Platform Record Download", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}
	else
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Platform Record Download", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}


	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StartPlatformDownLoad(NULL,NULL,pCameraCode, pDownloadParam, pulHandle);
}
IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformDownloadCB(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
	const IVS_DOWNLOAD_PARAM* pDownloadParam,DownloadCallBackRaw pFPlatformDownLoadRawCB,void* pUserData,IVS_ULONG* pulHandle)
{
	CHECK_POINTER(pCameraCode,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDownloadParam,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pulHandle,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pFPlatformDownLoadRawCB,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUserData,      IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, CamCode: %s",iSessionID, pCameraCode);

	// ��������
	if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
		pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
	{
		BP_RUN_LOG_INF("Start Platform Record Download CB", "Name Rule=[%u]", pDownloadParam->uiNameRule);
		return IVS_PARA_INVALID;
	}

	// �ļ���ʽ
	if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
		pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
	{
		BP_RUN_LOG_INF("Start Platform Record Download CB", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
		return IVS_PARA_INVALID;
	}

	// �ļ��ָ�����
	if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
		pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	{
		BP_RUN_LOG_INF("Start Platform Record Download CB", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
		return IVS_PARA_INVALID;
	}

	// �ļ��ָ�����ֵ
	if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Platform Record Download CB", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}
	else
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Platform Record Download CB", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}


	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StartPlatformDownLoadRawCB(NULL,NULL,pCameraCode, pDownloadParam,pFPlatformDownLoadRawCB,pUserData,pulHandle);
}
/*
  * IVS_SDK_StopPlatformDownload
  * ֹͣƽ̨¼������
  */
 IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
 {
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu", iSessionID, ulHandle);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StopPlatformDownLoad(ulHandle);
 }
 #ifdef WIN32
 IVS_INT32 __SDK_CALL IVS_SDK_StartDisasterRecoveryDownload (
     IVS_INT32 iSessionID,
     const IVS_CHAR* pCameraCode,
     const IVS_DOWNLOAD_PARAM* pDownloadParam,
     IVS_ULONG* pulHandle)
 {
     CHECK_POINTER(pCameraCode,    IVS_OPERATE_MEMORY_ERROR);
     CHECK_POINTER(pDownloadParam, IVS_OPERATE_MEMORY_ERROR);
     CHECK_POINTER(pulHandle,      IVS_OPERATE_MEMORY_ERROR);
     IVS_INFO_TRACE("SessionID: %d, CamCode: %s",iSessionID, pCameraCode);

     // ��������
     if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
         pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
     {
         BP_RUN_LOG_INF("Start Disaster Recovery Download", "Name Rule=[%u]", pDownloadParam->uiNameRule);
         return IVS_PARA_INVALID;
     }

     // �ļ���ʽ
     if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
         pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
     {
         BP_RUN_LOG_INF("Start Disaster Recovery Download", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
         return IVS_PARA_INVALID;
     }

     // �ļ��ָ�����
     if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
         pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
     {
         BP_RUN_LOG_INF("Start Disaster Recovery Download", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
         return IVS_PARA_INVALID;
     }

     // �ļ��ָ�����ֵ
     if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
     {
         if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
             pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
         {
             BP_RUN_LOG_INF("Start Disaster Recovery Download", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
             return IVS_PARA_INVALID;
         }
     }
     else
     {
         if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
             pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
         {
             BP_RUN_LOG_INF("Start Disaster Recovery Download", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
             return IVS_PARA_INVALID;
         }
     }


     CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
     CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
     CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
     return pUserMgr->GetDownLoadMgr().StartDisasterRecoveryDownload(pCameraCode, pDownloadParam, pulHandle);
 }

 IVS_INT32 __SDK_CALL  IVS_SDK_StopDisasterRecoveryDownload (
     IVS_INT32 iSessionID,
     IVS_ULONG ulHandle)
 {
     IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu", iSessionID, ulHandle);

     CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
     CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
     CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
     return pUserMgr->GetDownLoadMgr().StopDisasterRecoveryDownload(ulHandle);
 }


 /*
  * IVS_SDK_StartPUDownload
  * ����¼������
  */
 IVS_INT32 __SDK_CALL IVS_SDK_StartPUDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
	           const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle)
 {
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDownloadParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pulHandle, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, CamID: %s", iSessionID, pCameraCode);

		// ����У��

	// ��������
    if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
		pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
	{
        BP_RUN_LOG_INF("Start PU Record Download", "Name Rule=[%u]", pDownloadParam->uiNameRule);
		return IVS_PARA_INVALID;
	}


	// �ļ���ʽ
    if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
		pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
	{
        BP_RUN_LOG_INF("Start PU Record Download", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
		return IVS_PARA_INVALID;
	}


	// �ļ��ָ�����
	if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
		pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	{
		BP_RUN_LOG_INF("Start PU Record Download", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
		return IVS_PARA_INVALID;
	}
    
	// �ļ��ָ�����ֵ
    if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start PU Record Download", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}
	else
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start PU Record Download", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StartPUDownLoad(pCameraCode, pDownloadParam, pulHandle);
 }

 IVS_INT32 __SDK_CALL IVS_SDK_StartPUDownloadCB(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
	 const IVS_DOWNLOAD_PARAM* pDownloadParam,DownloadCallBackRaw pFPUDownLoadRawCB,void* pUserData,IVS_ULONG* pulHandle)
 {
	 CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pDownloadParam, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pulHandle, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pFPUDownLoadRawCB, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pUserData, IVS_OPERATE_MEMORY_ERROR);
	 IVS_INFO_TRACE("SessionID: %d, CamID: %s", iSessionID, pCameraCode);

	 // ����У��

	 // ��������
	 if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
		 pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
	 {
		 BP_RUN_LOG_INF("Start PU Record Download CB", "Name Rule=[%u]", pDownloadParam->uiNameRule);
		 return IVS_PARA_INVALID;
	 }


	 // �ļ���ʽ
	 if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
		 pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
	 {
		 BP_RUN_LOG_INF("Start PU Record Download CB", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
		 return IVS_PARA_INVALID;
	 }


	 // �ļ��ָ�����
	 if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
		 pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	 {
		 BP_RUN_LOG_INF("Start PU Record Download CB", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
		 return IVS_PARA_INVALID;
	 }

	 // �ļ��ָ�����ֵ
	 if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
	 {
		 if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			 pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		 {
			 BP_RUN_LOG_INF("Start PU Record Download CB", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
			 return IVS_PARA_INVALID;
		 }
	 }
	 else
	 {
		 if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			 pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		 {
			 BP_RUN_LOG_INF("Start PU Record Download CB", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
			 return IVS_PARA_INVALID;
		 }
	 }

	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetDownLoadMgr().StartPUDownLoadRawCB(pCameraCode, pDownloadParam, pFPUDownLoadRawCB,pUserData,pulHandle);
 }

/*
 * IVS_SDK_StopPUDownload
 * ֹͣǰ��¼������
 */
IVS_INT32 __SDK_CALL IVS_SDK_StopPUDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
{
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu",iSessionID, ulHandle);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StopPUDownLoad(ulHandle);
}

/*
 * IVS_SDK_DownloadPause
 */
IVS_INT32 __SDK_CALL IVS_SDK_DownloadPause(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
{
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu",iSessionID, ulHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().DownLoadPause(ulHandle);
}

/*
 * IVS_SDK_DownloadResume
 */
IVS_INT32 __SDK_CALL IVS_SDK_DownloadResume(IVS_INT32 iSessionID,IVS_ULONG ulHandle)
{
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu",iSessionID, ulHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().DownLoadResume(ulHandle);
}


/*
 * IVS_SDK_GetDownloadInfo
 */
IVS_INT32 __SDK_CALL IVS_SDK_GetDownloadInfo(IVS_INT32 iSessionID, 
		              IVS_ULONG ulHandle,IVS_DOWNLOAD_INFO* pDownloadInfo)
{
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu",iSessionID, ulHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().GetDownLoadInfo(ulHandle, pDownloadInfo);
}
#endif


//��ȡý����Ϣ
#ifdef WIN32
 IVS_INT32 __SDK_CALL IVS_SDK_GetMediaInfo(IVS_INT32 iSessionID, IVS_ULONG ulHandle, IVS_MEDIA_INFO* pMediaInfo)
 {
	IVS_INFO_TRACE("SessionID: %d,Handle: %lu",iSessionID,ulHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pMediaInfo, IVS_OPERATE_MEMORY_ERROR);
	unsigned int uiPort = 0;
	CMediaBase* mediaBase = GetMediaBaseAndPortInfo(iSessionID,ulHandle,uiPort);
	CHECK_POINTER(mediaBase, IVS_OPERATE_MEMORY_ERROR);
	//add by c0020659 ��ȡý��˵�ַ
	std::string strMediaSourceIP;
	mediaBase->GetMediaSourceIP(strMediaSourceIP);
	if (strMediaSourceIP.size() <= IVS_IP_LEN)
	{
		memcpy(pMediaInfo->stMediaSourceIP.cIP,strMediaSourceIP.c_str(),strMediaSourceIP.size());
	}
	//add end c0020659 ��ȡý��˵�ַ
	return mediaBase->GetMediaInfo(uiPort,pMediaInfo);
 }
#endif

//��ѯ��Ԫ�б�
IVS_INT32 __SDK_CALL IVS_SDK_GetNVRList(IVS_INT32 iSessionID, 
		                                const IVS_CHAR* pDomainCode,
		                                IVS_UINT32 uiServerType,
		                                const IVS_INDEX_RANGE* pIndexRange,
		                                IVS_DEVICE_BRIEF_INFO_LIST* pNvrList,
		                                IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,ServiceType: %u,BufSize: %u",iSessionID,pDomainCode,uiServerType,uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetDeviceMgr().GetNVRList(pDomainCode,uiServerType,pIndexRange,pNvrList,uiBufferSize);
}

//��ȡ��������
#ifdef WIN32
IVS_SDK_API IVS_BOOL __SDK_CALL IVS_SDK_GetGPUDecodeAbility()
{
	IVS_INFO_TRACE("");
    VIDEO_DECODE_MODE  emDecodeMode = IVS_PLAY_GetDecodeAbility();
    if ((DECODE_HW_INTEL == emDecodeMode)
        ||(DECODE_HW_NVDIA == emDecodeMode))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

//����/�ر�GPU����
IVS_SDK_API IVS_VOID __SDK_CALL IVS_SDK_EnableGPU(IVS_BOOL bEnable)
{
	IVS_INFO_TRACE("Enable: %d",(int)bEnable);
    IVS_PLAY_SetHardwareDec(bEnable);
}
#endif
//�ƶ��豸�����豸
IVS_SDK_API IVS_INT32  __SDK_CALL IVS_SDK_ModifyDeviceGroupName(IVS_INT32 iSessionID, 
                                                const IVS_CHAR* pDomainCode,
                                                IVS_UINT32 uiDevGroupID,
                                                const IVS_CHAR* pNewDevGroupName)
 {
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNewDevGroupName, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,DevGroupID: %u,NewDevGroupName: %s",iSessionID,pDomainCode,uiDevGroupID,pNewDevGroupName);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().ModifyDeviceGroupName(pDomainCode, uiDevGroupID, pNewDevGroupName);
 }


//�ƶ��豸�����豸
 IVS_SDK_API IVS_INT32  __SDK_CALL IVS_SDK_MoveDeviceGroup(IVS_INT32 iSessionID, 
                                              const IVS_CHAR* pDomainCode,
                                              IVS_UINT32 uiNewParentDevGroupID,
                                              IVS_UINT32 uiDevGroupID)
 {
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s,NewParentDevGroupID: %u,DevGroupID: %u",
		            iSessionID,pDomainCode,uiNewParentDevGroupID,uiDevGroupID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().MoveDeviceGroup(pDomainCode, uiNewParentDevGroupID, uiDevGroupID);
 }

//��ѯ�澯��������
IVS_SDK_API IVS_INT32  __SDK_CALL IVS_SDK_GetAlarmLevel(IVS_INT32 iSessionID,
	                                                     IVS_UINT32 uiAlarmLevelID,
	                                                     IVS_ALARM_LEVEL* pAlarmLevel)
{
	IVS_INFO_TRACE("SessionID: %d,AlarmLevelID: %u",iSessionID,uiAlarmLevelID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmLevel, IVS_PARA_INVALID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLevelMgr().GetAlarmLevel(uiAlarmLevelID,pAlarmLevel);
}
 
//AES256����
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Aes256CbcEncode(const IVS_CHAR* pszPlain, IVS_CHAR* pszCipher, IVS_UINT64 ullLenCipher)
 {	
    CHECK_POINTER(pszPlain, IVS_PARA_INVALID);
    CHECK_POINTER(pszCipher, IVS_PARA_INVALID);
	IVS_INFO_TRACE("");

    IVS_UINT32 iRet = Aes256Operator::instance().Aes256CbcEncode(pszPlain, pszCipher, ullLenCipher);

    return (IVS_INT32)iRet;
 }

//AES256����
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Aes256CbcDecode(const IVS_CHAR* pszCipher, IVS_CHAR* pszPlain, IVS_UINT64 ullLenPlain)
 {
	 CHECK_POINTER(pszCipher, IVS_PARA_INVALID);
	 CHECK_POINTER(pszPlain, IVS_PARA_INVALID);
	 IVS_INFO_TRACE("");

	 IVS_UINT32 iRet = Aes256Operator::instance().Aes256CbcDecode(pszCipher, pszPlain, ullLenPlain);

	 return (IVS_INT32)iRet;
 }
//��ʼƽ̨¼��
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformRecord(IVS_INT32 iSessionID,
	                                                          const IVS_CHAR* pCameraCode,
	                                                          IVS_INT32 iRecordTime)
{	
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,RecordTime: %d",iSessionID,pCameraCode,iRecordTime);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordService().StartPlatformRecord(pCameraCode,iRecordTime);
 }

//ֹͣƽ̨¼��
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode)
 {
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);

	 IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
	 
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetRecordService().StopPlatformRecord(pCameraCode);
 }

//��ʼǰ��¼��
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime)
 {
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	 IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,RecordTime: %d",iSessionID,pCameraCode,iRecordTime);

	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetRecordService().StartPURecord(pCameraCode,iRecordTime);
 }
//ֹͣǰ��¼��
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode)
 {
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordService().StopPURecord(pCameraCode);
 }


//��ʼ����¼��
 #ifdef WIN32
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartLocalRecord(IVS_INT32 iSessionID,
	                                                       const IVS_LOCAL_RECORD_PARAM* pRecordParam, 
                                                           IVS_ULONG ulPlayHandle, 
                                                           const IVS_CHAR* pSaveFileName)
 {
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// ******pSaveFileName ����Ϊ��**************
// 	CHECK_POINTER(pSaveFileName, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordParam, IVS_PARA_INVALID);

	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu,SaveFileName: %s",
		    iSessionID, ulPlayHandle, pSaveFileName != NULL ? pSaveFileName : "NULL");
	
	// ����У��

	// ��������
    if (pRecordParam->uiNameRule < RULE_NAME_NUM_TIME ||
		pRecordParam->uiNameRule > RULE_TIME_NAME_NUM)
	{
        BP_RUN_LOG_INF("Start Local Record", "Name Rule=[%u]", pRecordParam->uiNameRule);
		return IVS_PARA_INVALID;
	}

	// �ļ���ʽ
    if (pRecordParam->uiRecordFormat < IVS_FILE_MP4 || 
		pRecordParam->uiRecordFormat > IVS_FILE_AVI)
	{
        BP_RUN_LOG_INF("Start Local Record", "Record Format=[%u]", pRecordParam->uiRecordFormat);
		return IVS_PARA_INVALID;
	}

	// ¼��ʱ��
	if (pRecordParam->uiRecordTime < IVS_MIN_RECORD_TIME ||
		pRecordParam->uiRecordTime > IVS_MAX_RECORD_TIME) 
	{
		BP_RUN_LOG_INF("Start Local Record", "Record Time=[%u]", pRecordParam->uiRecordTime);
		return IVS_PARA_INVALID;
	}

	// �ļ��ָ�����
	if (pRecordParam->uiSplitterType < RECORD_SPLITE_TIME ||
		pRecordParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	{
		BP_RUN_LOG_INF("Start Local Record", "Splitter Type=[%u]", pRecordParam->uiSplitterType);
		return IVS_PARA_INVALID;
	}
    
	// �ļ��ָ�����ֵ
    if (RECORD_SPLITE_TIME == pRecordParam->uiSplitterType)
	{
		if (pRecordParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			pRecordParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Local Record", "Splitter Time=[%u]", pRecordParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}
	else
	{
		if (pRecordParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			pRecordParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Local Record", "Splitter FileSize=[%u]", pRecordParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}

	if (NULL != pSaveFileName && strlen(pSaveFileName) > 256)
	{
		BP_RUN_LOG_INF("Start Local Record", "pSaveFileName too long [%s]", pSaveFileName);
		return IVS_PARA_INVALID;
	}

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	IVS_INT32 iRet = IVS_FAIL;
    CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
    CHECK_POINTER(mediaBase,IVS_FAIL);
	//unsigned int port = pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulPlayHandle);
	iRet = mediaBase->StartLocalRecord(pRecordParam, pSaveFileName);
	
	return iRet;
 }

//ֹͣ����¼��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopLocalRecord(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, PlayHandle: %lu",iSessionID,ulPlayHandle);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    IVS_INT32 iRet = IVS_FAIL;
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	
	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if(NULL != mediaBase)
	{
		//unsigned int port = pUserMgr->GetRealPlayMgr().GetPlayerChannelByHandle(ulPlayHandle);
		iRet = mediaBase->StopLocalRecord();
	}
	return iRet;
}
#endif

//��ѯ��·����Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainRoute( IVS_INT32 iSessionID, IVS_DOMAIN_ROUTE_LIST* pDomainRouteList, IVS_UINT32 uiBufferSize )
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %lu",iSessionID,uiBufferSize);

	CHECK_POINTER(pDomainRouteList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDomainRouteMgr().GetDomainRoute(pDomainRouteList, uiBufferSize);
}

//�ͷ�SDK�ڲ�������ڴ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ReleaseBuffer(IVS_CHAR *pBuffer)
{
	IVS_INFO_TRACE("");
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	return g_pNvs->ReleaseBuffer(pBuffer);
}


 // ���ܷ���ͳһ͸���ӿ�
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_IA(IVS_INT32 iSessionID, IVS_UINT32 uiInterfaceType, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
 {
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
//	***********pReqXml����Ϊ��*********************
//  CHECK_POINTER(pReqXml, NULL);
	IVS_INFO_TRACE("SessionID: %d, InterfaceType: %u, ReqXml: %s", iSessionID, uiInterfaceType, NULL != pReqXml ? pReqXml : "NULL");

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetIntelligenceAnalysisMgr().IntelligenceAnalysis(uiInterfaceType, NULL, 0, pReqXml, pRspXml);
 }

 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_IA_EX(IVS_INT32 iSessionID, IVS_UINT32 uiInterfaceType, IVS_CHAR* pTransID, 
												IVS_UINT32 uiTransIDLen, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
 {
	 CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	 CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	 CHECK_POINTER(pTransID, IVS_PARA_INVALID);
//	 ***********pReqXml����Ϊ��*********************
//   CHECK_POINTER(pReqXml, NULL);
	 IVS_INFO_TRACE("SessionID: %d, InterfaceType: %u, TransID: %s, TransIDLen: %u, ReqXml: %s", 
		 iSessionID, uiInterfaceType, pTransID, uiTransIDLen, NULL != pReqXml ? pReqXml : "NULL");

	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	 return pUserMgr->GetIntelligenceAnalysisMgr().IntelligenceAnalysis(uiInterfaceType, pTransID, uiTransIDLen, pReqXml, pRspXml);
 }


 #ifdef WIN32
 // ���û�ͼ�ص�
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDrawCallBack(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, DRAW_PROC callback, void *pUserData)
 {
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	// callback��pUserData����Ϊ��
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	
	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if (NULL == mediaBase)
	{
		mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
	}

	if (NULL != mediaBase)
	{
		return mediaBase->SetDrawCallBack(callback, pUserData);
	}
	return IVS_FAIL;
 }

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDrawCallBackEx(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, DRAW_PROC callback, void *pUserData)
{

	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	 // callback��pUserData����Ϊ��

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if (NULL == mediaBase)
	{
	    mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
	}

	if (NULL != mediaBase)
	{
		return mediaBase->SetDrawCallBackEx(hWnd, callback, pUserData);
	}
	return IVS_FAIL;
}
#endif
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDelayFrameNum(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uDelayFrameNum)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu, DelayFrameNum: %u",iSessionID,ulPlayHandle, uDelayFrameNum);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if (NULL == mediaBase)
	{
		mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
	}

	if (NULL != mediaBase)
	{
		return mediaBase->SetDelayFrameNum(uDelayFrameNum);
	}
	return IVS_FAIL;

}

#endif
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceToGroup( IVS_INT32 iSessionID, 
                                     const IVS_CHAR* pTargetDomainCode,
                                     IVS_UINT32 uiTargetDevGroupID, 
                                     IVS_UINT32 uiDeviceNum,
                                     const IVS_DEV_GROUP_CAMERA* pGroupCameraList )
 {
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pGroupCameraList, IVS_PARA_INVALID);
	CHECK_POINTER(pTargetDomainCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,TargetDomainCode: %s,TargetDevGroupID: %u,DeviceNum: %u",iSessionID,pTargetDomainCode,uiTargetDevGroupID,uiDeviceNum);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().AddDeviceToGroup(pTargetDomainCode, uiTargetDevGroupID, uiDeviceNum, pGroupCameraList);
 }

//�������豸��ѯ���豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetChannelList(IVS_INT32 iSessionID, 
                                                        const IVS_CHAR* pDevCode,
                                                        IVS_DEV_CHANNEL_BRIEF_INFO_LIST* pChannelList,
                                                        IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pChannelList, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevCode: %s,BufSize: %u",iSessionID,pDevCode,uiBufferSize);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetChannelList(pDevCode, pChannelList, uiBufferSize);
}

//��ȡ��̨λ����Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZAbsPosition(IVS_INT32 iSessionID,
	const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, IVS_PTZ_ABSPOSITION *pPtzAbsPosition)
{
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPtzAbsPosition, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s", iSessionID, pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	IVS_INT32 iRet = pUserMgr->GetPtzControl().GetPtzAbsPosition(pCameraCode, pPtzAbsPosition);

	return iRet;
}

//�ƾ�����
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControl(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode, IVS_INT32 iControlCode,const IVS_CHAR* pControlPara1,const IVS_CHAR* pControlPara2, IVS_UINT32* pLockStatus)
 {
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	 CHECK_POINTER(pControlPara1, IVS_PARA_INVALID);
	 CHECK_POINTER(pControlPara2, IVS_PARA_INVALID);
	 CHECK_POINTER(pLockStatus, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,ControlCode: %d,ControlPara1: %s,ControlPara2: %s",
		            iSessionID,pCameraCode,iControlCode,pControlPara1,pControlPara2);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_PTZ_CONTROL_INFO PtzControlInfo = {0};
	IVS_INT32 iRet = pUserMgr->GetPtzControl().PtzControl(pCameraCode, iControlCode, pControlPara1,pControlPara2, &PtzControlInfo);
	*pLockStatus = PtzControlInfo.uiLockStatus;
	return iRet;
 }

 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControlWithLockerInfo(IVS_INT32 iSessionID,
															 const IVS_CHAR* pCameraCode, 
															 IVS_INT32 iControlCode,
															 const IVS_CHAR* pControlPara1,
															 const IVS_CHAR* pControlPara2, 
															 IVS_PTZ_CONTROL_INFO* pPtzControlInfo)
 {
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	 CHECK_POINTER(pControlPara1, IVS_PARA_INVALID);
	 CHECK_POINTER(pControlPara2, IVS_PARA_INVALID);
	 CHECK_POINTER(pPtzControlInfo, IVS_PARA_INVALID);
	 IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,ControlCode: %d,ControlPara1: %s,ControlPara2: %s",
		 iSessionID,pCameraCode,iControlCode,pControlPara1,pControlPara2);

	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	 CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetPtzControl().PtzControl(pCameraCode, iControlCode, pControlPara1,pControlPara2, pPtzControlInfo);
 }

 //��ѯ¼��״̬
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordStatus(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordState)
 {
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,RecordMethod: %u",iSessionID,pCameraCode,uiRecordMethod);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordList().GetRecordStatus(pCameraCode, uiRecordMethod, pRecordState);
 }
 //�޸����豸����
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMainDeviceName(IVS_INT32 iSessionID,const IVS_CHAR* pDevCode,const IVS_CHAR* pNewDevName)
 {
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNewDevName, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevCode: %s,NewDevName: %s",iSessionID,pDevCode,pNewDevName);
	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().SetMainDeviceName(pDevCode, pNewDevName);
 }
 //�޸����������
 IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraName(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_CHAR* pNewCameraName)
 {
	 CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	 CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	 CHECK_POINTER(pNewCameraName, IVS_PARA_INVALID);

	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s,NewCameraName: %s",iSessionID,pCameraCode,pNewCameraName);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().SetCameraName(pCameraCode,pNewCameraName);
 } 

#ifdef WIN32
// ����BT��Infursion���⣬��ȡ�ظ�����
static IVS_INT32 DealAudioSound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle, IVS_INT32 iPlayFlag)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_REALVIDEO;
    IVS_INT32 iRet = pUserMgr->GetServiceTypebyPlayHandle(uiPlayHandle, enServiceType);//lint !e1013 !e516
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"IVS SDK Play Sound Failed.", "Reason:Get ServiceType by PlayHandle Failed. enServiceType = %d", (int)enServiceType);
        return iRet;
    }

    switch (enServiceType)
    {
    case SERVICE_TYPE_REALVIDEO:
        if (0 == iPlayFlag)// 0Ϊ����
        {
            iRet = pUserMgr->GetRealPlayMgr().PlayAudioSound(uiPlayHandle);
        }
        else// 1 Ϊֹͣ
        {
            iRet = pUserMgr->GetRealPlayMgr().StopAudioSound(uiPlayHandle);
        }
        break;
    case SERVICE_TYPE_PLAYBACK:
    case SERVICE_TYPE_BACKUPRECORD_PLAYBACK:
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK:
    case SERVICE_TYPE_PU_PLAYBACK:
        if (0 == iPlayFlag)// 0Ϊ����
        {
            iRet = pUserMgr->GetPlaybackMgr().PlayAudioSound(uiPlayHandle);
        }
        else// 1 Ϊֹͣ
        {
            iRet = pUserMgr->GetPlaybackMgr().StopAudioSound(uiPlayHandle);
        }
        break;
    default:
        break;
    }
    return iRet;
}

//������·����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlaySound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle)
{
    IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu", iSessionID, uiPlayHandle);
    IVS_INT32 iRet = DealAudioSound(iSessionID, uiPlayHandle, 0);// 0Ϊ����
    return iRet;
}

//ֹͣ������·����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle)
{
    IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu", iSessionID, uiPlayHandle);
    IVS_INT32 iRet = DealAudioSound(iSessionID, uiPlayHandle, 1);// 1Ϊֹͣ����
    return iRet;
}

//��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVolume(IVS_INT32 iSessionID,
                                                   IVS_UINT32 uiPlayHandle, 
                                                   IVS_UINT32 uiVolumeValue)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %u,VolumeValue: %u",iSessionID,uiPlayHandle,uiVolumeValue);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_REALVIDEO;
	IVS_INT32 iRet = pUserMgr->GetServiceTypebyPlayHandle(uiPlayHandle, enServiceType);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"IVS SDK Set Volume Failed.", "Reason:Get ServiceType by PlayHandle Failed. enServiceType = %d", (int)enServiceType);
		return iRet;
	}

	switch (enServiceType)
	{
	case SERVICE_TYPE_REALVIDEO:
		iRet = pUserMgr->GetRealPlayMgr().SetVolume(uiPlayHandle,uiVolumeValue);
		break;
	case SERVICE_TYPE_PLAYBACK:
	case SERVICE_TYPE_BACKUPRECORD_PLAYBACK:
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK:
    case SERVICE_TYPE_PU_PLAYBACK:
		iRet = pUserMgr->GetPlaybackMgr().SetVolume(uiPlayHandle,uiVolumeValue);
		break;
    case SERVICE_TYPE_AUDIO_CALL:
        {
            iRet = pUserMgr->GetAudioMgr().SetVolume(uiPlayHandle,uiVolumeValue);
            break;
        }
	default:
		break;
	}

    return iRet;
}


//��ȡ����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVolume(IVS_INT32 iSessionID,
                                                   IVS_UINT32 uiPlayHandle, 
                                                   IVS_UINT32* puiVolumeValue)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %u",iSessionID,uiPlayHandle);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_REALVIDEO;
	IVS_INT32 iRet = pUserMgr->GetServiceTypebyPlayHandle(uiPlayHandle, enServiceType);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"IVS SDK Get Volume Failed.", "Reason:Get ServiceType by PlayHandle Failed. enServiceType = %d", (int)enServiceType);
		return iRet;
	}

	switch (enServiceType)
	{
	case SERVICE_TYPE_REALVIDEO:
		iRet = pUserMgr->GetRealPlayMgr().GetVolume(uiPlayHandle,puiVolumeValue);
		break;
	case SERVICE_TYPE_PLAYBACK:
	case SERVICE_TYPE_BACKUPRECORD_PLAYBACK:
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK:
    case SERVICE_TYPE_PU_PLAYBACK:
		iRet = pUserMgr->GetPlaybackMgr().GetVolume(uiPlayHandle,puiVolumeValue);
		break;
    case SERVICE_TYPE_AUDIO_CALL:
        {
            iRet = pUserMgr->GetAudioMgr().GetVolume(uiPlayHandle, puiVolumeValue);
            break;
        }
	default:
		break;
	}

    return iRet;
}
#endif

#ifdef WIN32
//��ʼ������ǽ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_InitTVWall(IVS_INT32 iSessionID, PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData)
{
	CHECK_POINTER(pFunNotifyCallBack, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUserData, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("pSDK: 0x%p, SessionID: %d",g_pNvs, iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().InitTVWall(pFunNotifyCallBack, pUserData);
	return iRet;
}

//�ͷŵ���ǽ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CleanupTVWall(IVS_INT32 iSessionID)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().CleanupTVWall();
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDecoderList(IVS_INT32 iSessionID,
														const IVS_INDEX_RANGE* pIndexRange,
														CUMW_NOTIFY_REMOTE_DECODER_STATUS_LIST* pDecoderList,
														IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
	CHECK_POINTER(pDecoderList, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("pSDK: 0x%p, SessionID: %d, BufferSize: %u",g_pNvs, iSessionID, uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().GetDecoderList(pIndexRange, pDecoderList, uiBufferSize);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayTVWall(IVS_INT32 iSessionID,
															const IVS_CHAR* pCameraCode,
															const IVS_REALPLAY_PARAM* pRealplayParam,
															const IVS_TVWALL_PARAM* pTVWallParam)
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID, pCameraCode);
	CHECK_POINTER(pRealplayParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StartRealPlayTVWall(pCameraCode, pRealplayParam, pTVWallParam);
	return iRet;

}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayTVWall(IVS_INT32 iSessionID,
															const IVS_TVWALL_PARAM* pTVWallParam)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StopRealPlayTVWall(pTVWallParam);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_CHAR* pCameraCode,
                                                                        const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	CHECK_POINTER(pTVWallParam, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("pSDK: 0x%p,SessionID: %d,CameraCode: %s",g_pNvs, iSessionID, pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StartPlayBackTVWall(pCameraCode, pPlaybackParam, pTVWallParam, REPLAY_TYPE_PLATFORM);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackTVWall(IVS_INT32 iSessionID,
                                                                    const IVS_TVWALL_PARAM* pTVWallParam)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StopPlayBackTVWall(pTVWallParam);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUPlayBackTVWall (IVS_INT32 iSessionID,
																const IVS_CHAR* pCameraCode,
																const IVS_PLAYBACK_PARAM* pPlaybackParam,
																const IVS_TVWALL_PARAM* pTVWallParam)
{
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("Start PU PlayBack TVWallSessionID: %d,CameraCode: %s",iSessionID, pCameraCode);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	CHECK_POINTER(pTVWallParam, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StartPlayBackTVWall(pCameraCode, pPlaybackParam, pTVWallParam, REPLAY_TYPE_PU);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUPlayBackTVWall(IVS_INT32 iSessionID,
																const IVS_TVWALL_PARAM* pTVWallParam)
{
	return IVS_SDK_StopPlatformPlayBackTVWall(iSessionID, pTVWallParam);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartBackupPlayBackTVWall (IVS_INT32 iSessionID,
																	const IVS_CHAR* pDomainCode,
																	const IVS_CHAR* pNVRCode,
																	const IVS_CHAR* pCameraCode,
																	const IVS_PLAYBACK_PARAM* pPlaybackParam,
																	const IVS_TVWALL_PARAM* pTVWallParam)
{
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	CHECK_POINTER(pTVWallParam, IVS_PARA_INVALID);
	IVS_INFO_TRACE("Start Backup PlayBack TVWallSessionID: %d,DomainCode: %s,NVRCode:%s,CameraCode: %s, PlaybackParam: %s",
					iSessionID, pDomainCode, pNVRCode, pCameraCode, pPlaybackParam);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().StartPlayBackTVWall(pCameraCode, pPlaybackParam, pTVWallParam, REPLAY_TYPE_BACKUP, pDomainCode, pNVRCode);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopBackupPlayBackTVWall(IVS_INT32 iSessionID,
	const IVS_TVWALL_PARAM* pTVWallParam)
{
	return IVS_SDK_StopPlatformPlayBackTVWall(iSessionID, pTVWallParam);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackPauseTVWall(IVS_INT32 iSessionID,
															const IVS_TVWALL_PARAM* pTVWallParam)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().PlayBackPauseTVWall(*pTVWallParam);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackResumeTVWall(IVS_INT32 iSessionID,
															const IVS_TVWALL_PARAM* pTVWallParam)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().PlayBackResumeTVWall(*pTVWallParam);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlaybackFrameStepForwardTVWall(IVS_INT32 iSessionID,
																		const IVS_TVWALL_PARAM* pTVWallParam)
{
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CHECK_POINTER(pTVWallParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().PlaybackFrameStepForwardTVWall(*pTVWallParam);
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddCallBackTVWall(IVS_INT32 iSessionID, PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, 
															void* pUserData)
{
	CHECK_POINTER(pFunNotifyCallBack, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pUserData, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("pSDK: 0x%p, SessionID: %d",g_pNvs, iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().AddCallBack(pFunNotifyCallBack, pUserData);
	return iRet;

}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RemoveCallBackTVWall(IVS_INT32 iSessionID, void* pUserData)
{
	CHECK_POINTER(pUserData, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("pSDK: 0x%p, SessionID: %d",g_pNvs, iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_INT32 iRet = pUserMgr->GetTVWallMgr().RemoveCallBack(pUserData);
	return iRet;
}
#endif
//�����û�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserData(IVS_INT32 iSessionID, const IVS_CHAR* pFileName)
{
    CHECK_POINTER(pFileName, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,FileName: %s",iSessionID,pFileName);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
  	return pUserMgr->GetUserDataMgr().BackupUserData(pFileName);
}

//�ָ��û�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserData(IVS_INT32 iSessionID, const IVS_CHAR* pFileName)
{
    CHECK_POINTER(pFileName, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,FileName: %s",iSessionID,pFileName);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetUserDataMgr().RestoreUserData(pFileName);
}

// ��ȡӵ��ָ���豸Ȩ�޵��û��б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
	CHECK_POINTER(pDevUserList, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DomainCode: %s, DevCode: %s,BufSize: %u",iSessionID,pDomainCode, pDevCode,uiBufferSize);
	
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetDeviceUserList(pDomainCode, pDevCode, uiIsUserOnline, pIndexRange, pDevUserList, uiBufferSize);
}

// ����ǰ���豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RebootDevice (IVS_INT32 iSessionID, const IVS_CHAR* pDeviceCode, IVS_UINT32 uiRebootType)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDeviceCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,DevCode: %s",iSessionID,pDeviceCode);	

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetDeviceMgr().RestartDevice(pDeviceCode, uiRebootType);
}

// ��ȡƽ̨������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetConfigInfo (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,ReqXml: %s",iSessionID,pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetConfigInfo(pReqXml, pRspXml);
}

// ��ѯƽ̨��������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAbilityInfo (IVS_INT32 iSessionID,IVS_UINT32* uiAbility)
{
	CHECK_POINTER(uiAbility, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,uiAbility: %d",iSessionID,*uiAbility);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_CHAR* pRsqXml = NULL;
	return pUserMgr->GetAlarmMgr().GetAbilityInfo(uiAbility,&pRsqXml);
}

// ����ץ��
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LocalSnapshot(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, 
														IVS_UINT32 uiPictureFormat, const IVS_CHAR* pFileName)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pFileName, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, PlayHandle: %lu, PictureFormat: %lu, FileName: %s",
		iSessionID, ulPlayHandle, uiPictureFormat, pFileName);
	if (IVS_FILE_NAME_LEN < strlen(pFileName))
	{
		BP_RUN_LOG_INF("Local Snapshot", "File Name too long");
		return IVS_NAME_INVALID;
	}
	if (CAPTURE_FORMAT_NONE == uiPictureFormat || CAPTURE_FORMAT_OTHERS <= uiPictureFormat)
	{
		BP_RUN_LOG_ERR(IVS_FILE_FORMAT_ERROR, "Local Snapshot", "Picture Format Invalid");
		return IVS_FILE_FORMAT_ERROR;
	}

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if (NULL == mediaBase)
	{
		mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
	}
 	CHECK_POINTER(mediaBase, IVS_SDK_RET_INVALID_SESSION_ID);

	return mediaBase->LocalSnapshot(pFileName, uiPictureFormat);
}

// �ֲ��Ŵ󱾵�ץ�ĵ���ͼƬ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LocalSnapshotRect(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, 
															IVS_UINT32 uiPictureFormat, const IVS_CHAR* pFileName, 
															const IVS_RECT* pRect)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pFileName, IVS_PARA_INVALID);
	CHECK_POINTER(pRect, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, PlayHandle: %lu, PictureFormat: %lu, FileName: %s",
		iSessionID, ulPlayHandle, uiPictureFormat, pFileName);
	if (IVS_FILE_NAME_LEN < strlen(pFileName))
	{
		BP_RUN_LOG_INF("Local Snapshot Rect", "File Name too long");
		return IVS_PARA_INVALID;
	}
	if (CAPTURE_FORMAT_OTHERS <= uiPictureFormat)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "Local Snapshot Rect", "Picture Format Invalid");
		return IVS_PARA_INVALID;
	}

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	CMediaBase* pMediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
	if (NULL == pMediaBase)
	{
		pMediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
	}
	CHECK_POINTER(pMediaBase, IVS_SDK_RET_INVALID_SESSION_ID);

	return pMediaBase->LocalSnapshotRect(pFileName, uiPictureFormat, pRect);
}
#endif
// IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraName(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_ULONG ulNameLen, IVS_CHAR *pCamName)
// {
// 	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
// 	CHECK_POINTER(pCamName, IVS_PARA_INVALID);
// 	IVS_INFO_TRACE("SessionID: %d, PlayHandle: %lu", iSessionID, ulPlayHandle);
// 
// 	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
// 	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
// 	CMediaBase* mediaBase = pUserMgr->GetRealPlayMgr().GetMediaBase(ulPlayHandle);
// 	if (NULL == mediaBase)
// 	{
// 		mediaBase = pUserMgr->GetPlaybackMgr().GetMediaBase(ulPlayHandle);
// 	}
// 	CHECK_POINTER(mediaBase, IVS_SDK_RET_INVALID_SESSION_ID);
// 
// 	return mediaBase->GetCameraName(ulNameLen, pCamName);
// }

//Logo��֤
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetLogo (IVS_INT32 iSessionID, IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoFile)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLogoFile, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetUserDataMgr().GetLogo(uiLogoType, pLogoFile);
}

//�ϴ�Logo��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UploadLogo(IVS_INT32 iSessionID, IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoFile)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLogoFile, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetUserDataMgr().UploadLogo(uiLogoType, pLogoFile);
}

//��ȡ������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStatement(IVS_INT32 iSessionID,const IVS_CHAR* pLanguageType,IVS_CHAR** pStatement)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLanguageType, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetUserDataMgr().GetStatement(pLanguageType, pStatement);
}

//�ϴ�������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UploadStatement(IVS_INT32 iSessionID,const IVS_CHAR* pLanguageType,const IVS_CHAR* pStatementFile)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLanguageType, IVS_PARA_INVALID);
	CHECK_POINTER(pStatementFile, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetUserDataMgr().UploadStatement(pLanguageType, pStatementFile);
}


// ����Ԥ��λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName, IVS_UINT32* uiPresetIndex)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPresetName, IVS_PARA_INVALID);
	CHECK_POINTER(uiPresetIndex, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().AddPTZPreset(pCameraCode, pPresetName, uiPresetIndex);
}

// ɾ��Ԥ��λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s,uiPresetIndex: %u",iSessionID,pCameraCode, uiPresetIndex);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().DelPTZPreset(pCameraCode, uiPresetIndex);
}

// �޸�Ԥ��λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPTZPreset, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().ModPTZPreset(pCameraCode, pPTZPreset);
}

// ��ѯԤ��λ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZPresetList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiPTZPresetNum)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPTZPresetList, IVS_PARA_INVALID);
	CHECK_POINTER(uiPTZPresetNum, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s,uiBufferSize: %u",iSessionID,pCameraCode, uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().GetPTZPresetList(pCameraCode, pPTZPresetList, uiBufferSize, uiPTZPresetNum);
}

// ���ÿ���λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_GUARD_POS_INFO* pGuardPosInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pGuardPosInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().SetGuardPos(pCameraCode, pGuardPosInfo);
}

//ɾ������λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().DelGuardPos(pCameraCode, uiPresetIndex);
}


// ��ȡ����λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_GUARD_POS_INFO* pGuardPosInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pGuardPosInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().GetGuardPos(pCameraCode, pGuardPosInfo);
}

// ����Ѳ���켣
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, IVS_UINT32* uiTrackIndex)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCuriseTrackInfo, IVS_PARA_INVALID);
	CHECK_POINTER(uiTrackIndex, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s, uiTrackIndex: %u", iSessionID, pCameraCode, *uiTrackIndex);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().AddCruiseTrack(pCameraCode, pCuriseTrackInfo, uiTrackIndex);
}

// ɾ��Ѳ���켣
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiTrackIndex, IVS_UINT32 uiCruiseType)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s, uiTrackIndex: %u, uiCruiseType: %u", iSessionID, pCameraCode, uiTrackIndex, uiCruiseType);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().DelCruiseTrack(pCameraCode, uiTrackIndex, uiCruiseType);
}

// �޸�Ѳ���켣
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCuriseTrackInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().ModCruiseTrack(pCameraCode, pCuriseTrackInfo);
}

// ��ѯѲ���켣�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrackList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_TRACK* pCruiseTrackList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiCruiseTrackNum)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCruiseTrackList, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s, uiBufferSize:%u",iSessionID,pCameraCode,uiBufferSize);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().GetCruiseTrackList(pCameraCode, pCruiseTrackList, uiBufferSize, uiCruiseTrackNum);
}

// ��ѯѲ���켣��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_INT32 uiTrackIndex, IVS_CRUISE_TRACK* pCruiseTrackInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCruiseTrackInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s, uiTrackIndex:%u",iSessionID,pCameraCode, uiTrackIndex);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().GetCruiseTrack(pCameraCode, uiTrackIndex, pCruiseTrackInfo);
}

// ����Ѳ���ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_PLAN* pCruisePlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCruisePlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().SetCruisePlan(pCameraCode, pCruisePlan);
}

// ��ѯѲ���ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_PLAN* pCruisePlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCruisePlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPtzControl().GetCruisePlan(pCameraCode, pCruisePlan);
}

// ������������ݲ���
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraBackupConfig(IVS_INT32 iSessionID, const IVS_CHAR* pBackupXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBackupXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pBackupXml: %s",iSessionID,pBackupXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().SetCameraBackupCfg(pBackupXml);
}

// ��ѯ��������ݲ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraBackupConfig(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CHAR** pBackConfigXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().GetCameraBackupCfg(pCameraCode, pBackConfigXml);
}
#endif
// ��ѯOMU�澯�¼��б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceAlarmList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_DEVICE_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAlarmEventList, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetDeviceAlarmList(pUnifiedQuery, pAlarmEventList, uiBufferSize);
}
//ƽ̨ץ�� 
IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_PlatformSnapshot(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().PlatformSnapshot(pCameraCode);
}

//����ƽ̨ץ�ļƻ� 
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetSnapshotPlan(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_SNAPSHOT_PLAN* pSnapShotPlan)

{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pSnapShotPlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().SetSnapshotPlan(pCameraCode,pSnapShotPlan);
}

//��ѯƽ̨ץ�ļƻ� 
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetSnapshotPlan(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_SNAPSHOT_PLAN * pSnapShotPlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().GetSnapshotPlan(pCameraCode,pSnapShotPlan);
}

//��ѯ�����б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainList (IVS_INT32 iSessionID, const IVS_CHAR* pQueryInfo, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDomainRouteMgr().GetExDomainList(pQueryInfo, pRspXml);
}

//Ŀ¼����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShareCatalog (IVS_INT32 iSessionID,
													   const IVS_CHAR* pShareInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pShareInfo, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, ShareCatalog: %s", iSessionID, pShareInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDomainRouteMgr().SetShareCatalog(pShareInfo);
}


//��ѯĿ¼������Ϣ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetShareCatalogList (IVS_INT32 iSessionID,
															  const IVS_CHAR* pQueryInfo,
															  IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDomainRouteMgr().GetShareCatalogList(pQueryInfo,pRspXml);
}


// ��������ǰ���豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pSearchInfo, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, DomainCode:%s, pSearchInfo: %s", iSessionID, pDomainCode, pSearchInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().StartSearchDevice(pDomainCode, pSearchInfo);
}

// ֹͣ����ǰ���豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pSearchInfo, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, DomainCode:%s, pSearchInfo: %s", iSessionID, pDomainCode, pSearchInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().StopSearchDevice(pDomainCode, pSearchInfo);
}

// ��֤ǰ���豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_VerifyDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pVerifyInfo, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pVerifyInfo, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, DomainCode:%s, pVerifyInfo: %s", iSessionID, pDomainCode, pVerifyInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().VerifyDevice(pDomainCode, pVerifyInfo, pRspXml);
}


// ��ѯǰ����־
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDevLog(IVS_INT32 iSessionID, const IVS_CHAR* pQueryXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, pVerifyInfo: %s", iSessionID, pQueryXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetDevLog(pQueryXml, pRspXml);
}

// ��ȡƽ̨ץ��ͼƬ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetSnapshotList(IVS_INT32 iSessionID,
	const IVS_CHAR* pCameraCode,
	const IVS_QUERY_SNAPSHOT_PARAM* pQueryParam,
	IVS_SNAPSHOT_INFO_LIST* pSnapshotList,
	IVS_UINT32 uiBufSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryParam, IVS_PARA_INVALID);
	CHECK_POINTER(pSnapshotList, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().GetSnapshotList(pCameraCode, pQueryParam, pSnapshotList, uiBufSize);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainList(IVS_INT32 iSessionID, IVS_CHAR** pDomainList)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetDomainList(pDomainList);
}

// CPU/�ڴ����ܲ�ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pNodeCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPerformance, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetCPUMemory(pNodeCode, pPerformance);
}

// ��ʼ�����Խ�
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartVoiceTalkback(
                                IVS_INT32 iSessionID,
                                const IVS_VOICE_TALKBACK_PARAM* pTalkbackParam,
                                const IVS_CHAR* pCameraCode,
                                IVS_ULONG* pHandle)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pTalkbackParam, IVS_PARA_INVALID);
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d, VoiceCode: %s", iSessionID, pCameraCode);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StartVoiceTalkback(pTalkbackParam, pCameraCode, pHandle);
}


// ֹͣ�����Խ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopVoiceTalkback(
                                                        IVS_INT32 iSessionID,
                                                        IVS_ULONG ulHandle)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("SessionID: %d,Handle: %lu",iSessionID,ulHandle);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StopVoiceTalkback(ulHandle);
}

// ������˷�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMicVolume(IVS_UINT32 uiVolumeValue)
{
    IVS_INFO_TRACE("VolumeValue: %u", uiVolumeValue);
    return IVS_PLAY_SetMicVolume(uiVolumeValue);
}

// ��ȡ��˷�����
IVS_SDK_API IVS_UINT32 __SDK_CALL IVS_SDK_GetMicVolume()
{
    IVS_UINT32 uVolumeValue = 0;
    if (IVS_SUCCEED != IVS_PLAY_GetMicVolume(&uVolumeValue))
    {
        return 0;
    }
    
    return uVolumeValue;
}

// ��ʼʵʱ�����㲥
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealBroadcast(IVS_INT32 iSessionID)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StartRealBroadcast();
}

// ֹͣʵʱ�����㲥
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealBroadcast(IVS_INT32 iSessionID)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StopRealBroadcast();
}

// ��ʼ�ļ������㲥
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartFileBroadcast(
                                                IVS_INT32 iSessionID,
                                                const IVS_CHAR* pFileName,
                                                IVS_BOOL bCycle)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pFileName, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,FileName: %s, Cycle %d",
                   iSessionID, pFileName, bCycle);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StartFileBroadcast(pFileName, bCycle);
}

// ֹͣ�ļ������㲥
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopFileBroadcast(IVS_INT32 iSessionID)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("SessionID: %d",iSessionID);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().StopFileBroadcast();
}

// ��ӹ㲥�豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBroadcastDevice(
                                                IVS_INT32 iSessionID,
                                                const IVS_CHAR* pCameraCode)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,VoiceCode: %s",iSessionID, pCameraCode);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().AddBroadcastDevice(pCameraCode);
}

// ɾ���㲥�豸
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBroadcastDevice(
                                                IVS_INT32 iSessionID,
                                                const IVS_CHAR* pCameraCode)
{
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    IVS_INFO_TRACE("SessionID: %d,VoiceCode: %s",iSessionID, pCameraCode);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetAudioMgr().DeleteBroadcastDevice(pCameraCode);
}

// ɾ��ƽ̨ץ�Ļ���
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelSnapshotPlan(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pCameraCode: %s",iSessionID, pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().DelSnapshotPlan(pCameraCode);
}
#endif
// ���¼�񱸷ݼƻ�
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBackupPlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pBackupPlan: %s",iSessionID, pBackupPlan);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().AddRecordBackupPlan(pBackupPlan);
}


// �޸�¼�񱸷ݼƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBackupPlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pBackupPlan: %s",iSessionID, pBackupPlan);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().ModifyRecordBackupPlan(pBackupPlan);
}

// ɾ������¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBackupPlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pBackupPlan: %s",iSessionID, pBackupPlan);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().DeleteRecordBackupPlan(pBackupPlan);
}

// ��ѯ����¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pQueryInfo, IVS_CHAR** pBackupPlan)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_PARA_INVALID);
	CHECK_POINTER(pBackupPlan, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pQueryInfo: %s",iSessionID, pQueryInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().GetRecordBackupPlan(pQueryInfo, pBackupPlan);
}
#endif
// ���ܷ���CPU/�ڴ����ܲ�ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetIACPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pNodeCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pPerformance, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetIACPUMemory(pNodeCode, pPerformance);
}

// ��Ԫ���ܲ�ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryPerformance(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pNodeCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pPerformance, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().QueryPerformance(pNodeCode, pPerformance);
}

// ��ѯ��ǰ�û����ֶ�¼������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordTask(IVS_INT32 iSessionID, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pRspXml: %s",iSessionID, pRspXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetRecordTask(pUserMgr->GetUserID(),pUserMgr->GetDeviceMgr(),pRspXml);
}

// ��ѯ¼�񱸷������б�
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordBackupTaskList (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_PARA_INVALID);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pQueryInfo: %s",iSessionID, pQueryInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().GetRecordBackupTaskList(pQueryInfo, pRspXml);
}

// ����/����¼�񱸷�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pQueryInfo: %s",iSessionID, pQueryInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().StartRecordBackupTask(pQueryInfo);
}

// ��ͣ/ֹͣ¼�񱸷�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pQueryInfo: %s",iSessionID, pQueryInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().StopRecordBackupTask(pQueryInfo);
}

// ɾ��¼�񱸷�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pQueryInfo, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pQueryInfo: %s",iSessionID, pQueryInfo);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().DeleteRecordBackupTask(pQueryInfo);
}
// ����¼�񱸷�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordBackupTask(IVS_INT32 iSessionID, const IVS_CHAR* pBackupTask)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBackupTask, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pBackupTask: %s",iSessionID, pBackupTask);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBackupMgr().AddRecordBackupTask(pBackupTask);
}
#endif
// ��ѯ�澯������������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkageAction(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmLinkageMgr().GetAlarmLinkageAction(pReqXml, pRsqXml);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIP(IVS_INT32 iSessionID, 
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrDst, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrSrc, IVS_PARA_INVALID);
	CHECK_POINTER(pHandle, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, CameraCode: %s, IP: %s, VideoPort: %u, AudioPort: %u",
					iSessionID, pCameraCode, 
					pMediaAddrDst->stIP.cIP, pMediaAddrDst->uiVideoPort, pMediaAddrDst->uiAudioPort);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	ULONG ulHandle = *pHandle;
	IVS_MEDIA_ADDR stMediaAddr = {0};
	IVS_INT32 iRet = pUserMgr->GetTelepresenceMgr().StartRealPlayByIP(pCameraCode, *pMediaAddrDst, stMediaAddr, ulHandle);
	*pHandle = ulHandle;
	bool bSucc = CToolsHelp::Memcpy(pMediaAddrSrc, sizeof(IVS_MEDIA_ADDR), &stMediaAddr, sizeof(IVS_MEDIA_ADDR));
	if(!bSucc)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "TP Start Play", "Memcpy failed.");
	}
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIP(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu",iSessionID, ulHandle);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetTelepresenceMgr().StopPlayByIP(ulHandle);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIPEx(IVS_INT32 iSessionID, 
															const IVS_REALPLAY_PARAM  *pRealplayParam,
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRealplayParam, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrDst, IVS_PARA_INVALID);
	CHECK_POINTER(pMediaAddrSrc, IVS_PARA_INVALID);
	CHECK_POINTER(pHandle, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, CameraCode: %s, IP: %s, VideoPort: %u, AudioPort: %u",
		iSessionID, pCameraCode, pMediaAddrDst->stIP.cIP, pMediaAddrDst->uiVideoPort, pMediaAddrDst->uiAudioPort);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	ULONG ulHandle = *pHandle;
	IVS_MEDIA_ADDR stMediaAddr = {0};
	IVS_INT32 iRet = pUserMgr->GetTelepresenceMgr().StartRealPlayByIPEx(pCameraCode, *pRealplayParam, *pMediaAddrDst, stMediaAddr, ulHandle);
	*pHandle = ulHandle;
	bool bSucc = CToolsHelp::Memcpy(pMediaAddrSrc, sizeof(IVS_MEDIA_ADDR), &stMediaAddr, sizeof(IVS_MEDIA_ADDR));
	if(!bSucc)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "TP Start Play", "Memcpy failed.");
	}
	return iRet;
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIPEx(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, Handle: %lu",iSessionID, ulHandle);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetTelepresenceMgr().StopPlayByIP(ulHandle);
}

// ����¼��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Lock Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().LockRecord(RECORD_METHOD_PLATFORM,NULL,NULL,pCameraCode, pRecordLockInfo);
}

// �޸�¼��������Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModLockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Mod Lock Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().ModLockRecord(RECORD_METHOD_PLATFORM,NULL,NULL,pCameraCode, pRecordLockInfo);
}

// ����¼��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UnLockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK UnLock Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().UnLockRecord(RECORD_METHOD_PLATFORM,NULL,NULL,pCameraCode, pRecordLockInfo);
}

// ����¼��(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,pNVRCode: %s,pCameraCode: %s",iSessionID,pDomainCode,pNVRCode,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Lock Backup Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().LockRecord(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode, pRecordLockInfo);
}

// ����¼��(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UnlockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,pNVRCode: %s,pCameraCode: %s",iSessionID,pDomainCode,pNVRCode,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK UnLock Backup Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().UnLockRecord(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode, pRecordLockInfo);
}

// �޸�¼��������Ϣ(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModLockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo)
{
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,pNVRCode: %s,pCameraCode: %s",iSessionID,pDomainCode,pNVRCode,pCameraCode);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRecordLockInfo, IVS_PARA_INVALID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Mod Lock Backup Record Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return  pUserMgr->GetRecordList().ModLockRecord(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode, pRecordLockInfo);
}

// ��ѯƽ̨�˶�����������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetMotionRangeDataPlatform(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_TIME_SPAN* pTimeSpan,const IVS_INDEX_RANGE* pIndexRange,
	IVS_MOTION_RANGE_DATA_LIST* pMotionRangeData,IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pTimeSpan, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pIndexRange, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pMotionRangeData, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,CameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetMotionDetection().GetMotionRangeDataPlatform(pCameraCode, pTimeSpan,pIndexRange,pMotionRangeData,uiBufferSize);
}

//����¼����ǩ(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBookmarkForBackup(IVS_INT32 iSessionID,
	const IVS_CHAR* pDomainCode,
	const IVS_CHAR* pNVRCode,
	const IVS_CHAR* pCameraCode,	                                                 
	const IVS_CHAR* pBookmarkName,
	const IVS_CHAR* pBookmarkTime)
{
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(pBookmarkTime, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,NVRCode: %s,CameraCode: %s,BookmarkName :%s,BookmarkTime :%s",iSessionID,pDomainCode,pNVRCode,pCameraCode,pBookmarkName,pBookmarkTime);


	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add Bookmark For Backup Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	IVS_UINT32 uiBookmarkID = 0;
	return pUserMgr->GetRecordBookmark().AddBookmark(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode, pBookmarkName, pBookmarkTime,&uiBookmarkID);
}

//�޸�¼����ǩ(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyBookmarkForBackup(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName)
{
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	//CHECK_POINTER(pNewBookmarkName, IVS_PARA_INVALID);//��SEȷ�ϣ���ǩ���ƿ���Ϊ��
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,NVRCode: %s,CameraCode: %s,BookmarkID: %u,NewBookmarkName: %s",iSessionID,pDomainCode,pNVRCode,pCameraCode,uiBookmarkID,pNewBookmarkName);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Modify Bookmark For Backup Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	 return pUserMgr->GetRecordBookmark().ModifyBookmark(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode,uiBookmarkID,pNewBookmarkName);

}

//ɾ��¼����ǩ(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBookmarkForBackup(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID)
{	
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pDomainCode: %s,NVRCode: %s,CameraCode: %s,BookmarkID: %u",iSessionID,pDomainCode,pNVRCode,pCameraCode,uiBookmarkID);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete Bookmark For Backup Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordBookmark().DeleteBookmark(RECORD_METHOD_MBU,pDomainCode,pNVRCode,pCameraCode, uiBookmarkID);
}

//��ѯ¼����ǩ�б�(���ݷ�����)
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetBookmarkListForBackup(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
	IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize)
{
	IVS_INFO_TRACE("SessionID: %d,BufSize: %u",iSessionID,uiBufSize);
	CHECK_POINTER(pUnifiedQuery, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBookmarkList, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get BookMark List For Backup Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordBookmark().GetBookmarkList(RECORD_METHOD_MBU,pUnifiedQuery,pBookmarkList,uiBufSize);
}

// ʵʱ�����ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDevReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryDevReport(pReqXml, pRspXml);
}

// ʵʱ���������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDevReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryDevReportDetail(pReqXml, pRspXml);
}

// ʵʱ�����ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRealtimeReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRealtimeReport(pReqXml, pRspXml);
}

// ʵʱ���������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRealtimeReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRealtimeReportDetail(pReqXml, pRspXml);
}

// ��Ƶ������ϲ�ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryQDReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryQDReport(pReqXml, pRspXml);
}

// ��Ƶ������������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryQDReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryQDReportDetail(pReqXml, pRspXml);
}

// ¼��ִ�мƻ������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordPlanReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRecordPlanReport(pReqXml, pRspXml);
}

// ¼��ִ�мƻ����������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordPlanReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRecordPlanReportDetail(pReqXml, pRspXml);
}

// ¼�������Ա����ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordIntegralityReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRecordIntegralityReport(pReqXml, pRspXml);
}

// ¼�������Ա��������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordIntegralityReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryRecordIntegralityReportDetail(pReqXml, pRspXml);
}

// �����ʱ����ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryPacketLossRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryPacketLossRateReport(pReqXml, pRspXml);
}

// �����ʱ��������ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryPacketLossRateReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryPacketLossRateReportDetail(pReqXml, pRspXml);
}

// ���̴洢ռ���ʱ����ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
    IVS_INFO_TRACE("SessionID: %d,UserGroupID: %u",iSessionID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pReqXml, IVS_PARA_INVALID);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Delete User Group Failed.", "Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetReportMgr().OMUQueryDiskUsageRateReport(pReqXml, pRspXml);
}


// ��ѯ������Ϣ�ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskProperty(IVS_INT32 iSessionID, 
	IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetReportMgr().OMUQueryDiskProperty(pRspXml);
}

// ��ѯ����Ԥ¼��������б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPrerecordCameraList(IVS_INT32 iSessionID, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d",iSessionID);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetRecordService().GetPrerecordCameraList(pRspXml);
}

// �����豸�鱸ע��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevGroupRemark(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pDevGroupRemark)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDevGroupRemark, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, pDomainCode: %s, uiDevGroupID: %u, pDevGroupRemark: %s",iSessionID, pDomainCode, uiDevGroupID, pDevGroupRemark);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().SetDevGroupRemark(pDomainCode, uiDevGroupID, pDevGroupRemark);
}

// ��ѯ�豸�鱸ע��Ϣ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDevGroupRemark(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, IVS_CHAR* pDevGroupRemark, IVS_UINT32 uiRemarkLen)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDevGroupRemark, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, pDomainCode: %s, uiDevGroupID: %u, pDevGroupRemark: %s, uiRemarkLen: %u",iSessionID, pDomainCode, uiDevGroupID, pDevGroupRemark, uiRemarkLen);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceGroupMgr().GetDevGroupRemark(pDomainCode, uiDevGroupID, pDevGroupRemark, uiRemarkLen);
}


// �����澯֪ͨ����ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUAddAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().AddAlarmLinkageRule(pReqXml, pRspXml);
}

// �޸ĸ澯֪ͨ����ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUModAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().ModAlarmLinkageRule(pReqXml);
}

// ɾ���澯֪ͨ����ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUDelAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().DelAlarmLinkageRule(pReqXml);
}

// ��ȡ�澯֪ͨ�����б�ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetAlarmLinkageRuleList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetAlarmLinkageRuleList(pReqXml, pRspXml);
}

// ��ȡ�澯֪ͨ��������ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetAlarmLinkageRuleDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetAlarmLinkageRuleDetail(pReqXml, pRspXml);
}

// ������Ԫ��ȡ�豸���ͽӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetDevTypeByNodeCode(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().GetDevTypeByNodeCode(pReqXml, pRspXml);
}

// ɾ��ƽ̨ץ��ͼƬ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteSnapshot(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPictureID)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetCapturePicture().DeleteSnapshot(pCameraCode, uiPictureID);
}

//��ѯ�������������¼�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainRecordPolicy(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get ExDomain Record Policy Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetRecordService().GetExDomainRecordPolicy(pReqXml,pRspXml);
}

//�����������������¼�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetExDomainRecordPolicy(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Set ExDomain Record Policy Failed.", "Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetRecordService().SetExDomainRecordPolicy(pReqXml);
}

// �����������������¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddExDomainRecordPlan(
    IVS_INT32 iSessionID, 
    IVS_UINT32 uiDeviceNum,
    const IVS_DEVICE_CODE* pDeviceList,
    const IVS_RECORD_PLAN* pRecordPlan,
    IVS_RESULT_LIST* pResultList,
    IVS_UINT32 uiResultBufferSize)
{
    IVS_INFO_TRACE("Add ExDomain Record Plan,SessionID: %d,DeviceNum: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiResultBufferSize);

    CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
    CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
    CHECK_POINTER(pResultList, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Add ExDomain Record Plan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);

    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().AddExDomainRecordPlan(uiDeviceNum,pDeviceList, pRecordPlan ,pResultList, uiResultBufferSize);
}

// �޸��������������¼��ƻ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModExDomainRecordPlan(
    IVS_INT32 iSessionID, 
    IVS_UINT32 uiDeviceNum,
    const IVS_DEVICE_CODE* pDeviceList,
    const IVS_RECORD_PLAN* pRecordPlan,
    IVS_RESULT_LIST* pResultList,
    IVS_UINT32 uiResultBufferSize)
{
    IVS_INFO_TRACE("Mod ExDomain Record Plan SessionID: %d,DeviceNum: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiResultBufferSize);
    CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
    CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
    CHECK_POINTER(pResultList, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Mod ExDomain Record Plan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().ModExDomainRecordPlan(uiDeviceNum,pDeviceList, pRecordPlan ,pResultList, uiResultBufferSize);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelExDomainRecordPlan(
    IVS_INT32 iSessionID,
    IVS_UINT32 uiDeviceNum,
    const IVS_DEVICE_CODE* pDeviceList,
    IVS_UINT32 uiRecordMethod,
    IVS_RESULT_LIST* pResultList,
    IVS_UINT32 uiResultBufferSize)
{
    IVS_INFO_TRACE("Del ExDomain Record Plan SessionID: %d,DeviceNum: %u,RecordMethod: %u,ResultBufferSize: %u",iSessionID,uiDeviceNum,uiRecordMethod,uiResultBufferSize);
    CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);
    CHECK_POINTER(pResultList, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Del ExDomain Record Plan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().DelExDomainRecordPlan(uiDeviceNum, pDeviceList, uiRecordMethod,pResultList, uiResultBufferSize);
    //return pUserMgr->GetRecordPlanMgr().DeleteRecordPlan(uiDeviceNum, pDeviceList, uiRecordMethod,pResultList, uiResultBufferSize);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainRecordPlan(
    IVS_INT32 iSessionID,
    const IVS_CHAR* pCameraCode,
    IVS_UINT32 uiRecordMethod,
    IVS_RECORD_PLAN* pRecordPlan)
{
    CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
    CHECK_POINTER(pRecordPlan, IVS_PARA_INVALID);
    CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
    IVS_INFO_TRACE("Get ExDomain Record Plan SessionID: %d,CameraCode :%s,RecordMethod: %u",iSessionID,pCameraCode,uiRecordMethod);

    if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get ExDomain Record Plan Failed.","Reason:SessionID is invalid");
        return IVS_SDK_RET_INVALID_SESSION_ID;
    }
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
    return pUserMgr->GetRecordService().GetExDomainRecordPlan(pCameraCode, uiRecordMethod, pRecordPlan);
    //return pUserMgr->GetRecordPlanMgr().GetRecordPlan(pCameraCode, uiRecordMethod, pRecordPlan);
}
// ��ѯ�������������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceCapability(IVS_INT32 iSessionID, IVS_UINT32 uiCapabilityType, const IVS_CHAR* pDevCode, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, uiCapabilityType: %u, pDevCode: %s",iSessionID, uiCapabilityType, pDevCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetDeviceCapability(uiCapabilityType, pDevCode, pRspXml);
}

// �ͻ����������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckNewVersion(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s",iSessionID, pReqXml);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().CheckNewVersion(pReqXml, pRspXml);
}

// �豸������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceGuard(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s", iSessionID, pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().SetDeviceGuard(pReqXml);
}
// ������־��·��
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetLogPath(const IVS_CHAR* pLogPath)
{
    CHECK_POINTER(pLogPath, IVS_PARA_INVALID);
    CSDKConfig::instance().SetLogPath(pLogPath);
    return IVS_SUCCEED;
}
#ifdef WIN32
// ���÷������˼���IP�Ͷ˿ں�
IVS_INT32 IVS_SDK_API __SDK_CALL IVS_SDK_SetNetKeyBoardPort(IVS_ULONG ulPort)
{

    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER(pNetKeyBoardService,IVS_FAIL);

    pNetKeyBoardService->SetNetKeyBoardListenAddr((unsigned short )ulPort);

    return IVS_SUCCEED;
}

// ���������߳�
IVS_INT32 IVS_SDK_API __SDK_CALL IVS_SDK_StartNetKeyBoard(IVS_ULONG ulNetKeyBoardVendor, IVS_ULONG ulPort, LPCallBack lpCallBack)
{

    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER(pNetKeyBoardService,IVS_FAIL);

    ULONG ulRet = (ULONG)pNetKeyBoardService->StartNetKeyBoard(ulNetKeyBoardVendor, ulPort, lpCallBack);

    return ulRet;
}

// �ͷ��߳�
IVS_INT32 IVS_SDK_API __SDK_CALL IVS_SDK_StopNetKeyBoard()
{

    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER(pNetKeyBoardService,IVS_FAIL);

    ULONG ulRet = (ULONG)pNetKeyBoardService->ReleaseThread();

    return ulRet;
}
#endif
//ֹͣ����¼��ط�
#ifdef WIN32
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopBackupPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle)
{
	IVS_INFO_TRACE("SessionID: %d,PlayHandle: %lu",iSessionID,ulPlayHandle);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);	
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().StopBackupPlayBack(ulPlayHandle);
}
//��ʼ����¼��ط�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartBackupPlayBack(IVS_INT32 iSessionID,
	const IVS_CHAR* pDomainCode,
	const IVS_CHAR* pNVRCode,
	const IVS_CHAR* pCameraCode,
	const IVS_PLAYBACK_PARAM* pPlaybackParam,
	HWND hWnd,
	IVS_ULONG* pHandle)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pNVRCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pPlaybackParam, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pNVRCode: %s,CameraCode: %s",iSessionID,pNVRCode,pCameraCode);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetPlaybackMgr().StartBackupPlayBack(pDomainCode,pNVRCode,pCameraCode, pPlaybackParam, hWnd, pHandle);
}
#endif

// ��ȡ�����Ԥ��ͼ�ӿ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraThumbnail(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode, IVS_CHAR* pThumbnailURL,IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, ReqXml: %s %d", iSessionID, NULL != pCameraCode ? pCameraCode : "NULL",uiBufferSize);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetCameraThumbnail(pCameraCode, pThumbnailURL,uiBufferSize);
}

#ifdef WIN32
 /*
  * IVS_SDK_StartBackupDownload
  * ��������¼������
  */
IVS_INT32 __SDK_CALL IVS_SDK_StartBackupDownload(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode, const IVS_CHAR* pCameraCode,
	                     const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle)
{
	CHECK_POINTER(pDomainCode,    IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pNVRCode,    IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode,    IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDownloadParam, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pulHandle,      IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d, CamCode: %s",iSessionID, pCameraCode);

	// ��������
    if (pDownloadParam->uiNameRule < RULE_NAME_NUM_TIME ||
		pDownloadParam->uiNameRule > RULE_TIME_NAME_NUM)
	{
        BP_RUN_LOG_INF("Start Backup Record Download", "Name Rule=[%u]", pDownloadParam->uiNameRule);
		return IVS_PARA_INVALID;
	}

	// �ļ���ʽ
    if (pDownloadParam->uiRecordFormat < IVS_FILE_MP4 || 
		pDownloadParam->uiRecordFormat > IVS_FILE_AVI)
	{
        BP_RUN_LOG_INF("Start Backup Record Download", "Record Format=[%u]", pDownloadParam->uiRecordFormat);
		return IVS_PARA_INVALID;
	}

	// �ļ��ָ�����
	if (pDownloadParam->uiSplitterType < RECORD_SPLITE_TIME ||
		pDownloadParam->uiSplitterType > RECORD_SPLITE_CAPACITY)
	{
		BP_RUN_LOG_INF("Start Backup Record Download", "Splitter Type=[%u]", pDownloadParam->uiSplitterType);
		return IVS_PARA_INVALID;
	}
    
	// �ļ��ָ�����ֵ
    if (RECORD_SPLITE_TIME == pDownloadParam->uiSplitterType)
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_TIME_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_TIME_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Backup Record Download", "Splitter Time=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}
	else
	{
		if (pDownloadParam->uiSplitterValue < IVS_MIN_SPACE_SPLITTER_VALUE || 
			pDownloadParam->uiSplitterValue > IVS_MAX_SPACE_SPLITTER_VALUE)
		{
			BP_RUN_LOG_INF("Start Backup Record Download", "Splitter FileSize=[%u]", pDownloadParam->uiSplitterValue);
			return IVS_PARA_INVALID;
		}
	}


	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StartPlatformDownLoad(pDomainCode,pNVRCode,pCameraCode, pDownloadParam, pulHandle);
}

 /*
  * IVS_SDK_StopBackupDownload
  * ֹͣ����¼������
  */
 IVS_INT32 __SDK_CALL IVS_SDK_StopBackupDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle)
 {
	IVS_INFO_TRACE("SessionID: %d, ulHandle: %lu", iSessionID, ulHandle);

	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDownLoadMgr().StopPlatformDownLoad(ulHandle);
 }
#endif
// ����ǰ���豸����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pDevUserName, const IVS_CHAR* pDevPWD, const IVS_CHAR* pDevRegPWD)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDevUserName, IVS_PARA_INVALID);
	CHECK_POINTER(pDevPWD, IVS_PARA_INVALID);
	CHECK_POINTER(pDevRegPWD, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pDevCode: %s,pDevUserName: %s",iSessionID,pDevCode,pDevUserName);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().SetDevPWD(pDevCode, pDevUserName, pDevPWD, pDevRegPWD);
}

// ����ǰ�������ļ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ImportDevCfgFile(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pCFGFile)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCFGFile, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pDevCode: %s,pDevUserName: %s",iSessionID,pDevCode,pCFGFile);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().ImportDevCfgFile(pDevCode, pCFGFile);
}

// ����ǰ�������ļ�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExportDevCfgFile(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pCFGFilePath)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCFGFilePath, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pDevCode: %s,pDevUserName: %s",iSessionID,pDevCode,pCFGFilePath);
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().ExportDevCfgFile(pDevCode, pCFGFilePath);
}

// ������λ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ResetAlarm(pReqXml);
}


// ��ȡ�澯ץ��ͼƬ�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmSnapshotList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml)
{
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s", iSessionID, pReqXml);

	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetAlarmSnapshotList(pReqXml, pRsqXml);
}
// �ⲿ��澯�¼�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExEventSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ExEventSubscribe(pReqXml);
}

// ��ѯ�ⲿ��澯�¼�����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExEventSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetExEventSubscribe(pReqXml, pRspXml);
}

// �ⲿ���豸Ŀ¼����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExCatalogSubscribe(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().ExCatalogSubscribe(pReqXml);
}

// ��ѯ�ⲿ���豸Ŀ¼����
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExCatalogSubscribeList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);
	CUserMgr* pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetAlarmMgr().GetExCatalogSubscribeList(pReqXml, pRspXml);
}
// Rename2Java.h �����޴�����


//�豸״̬��ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExGetDeviceStatus(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get ExDomain Device Status Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().ExGetDeviceStatus(pReqXml,pRspXml);
}

//�豸��Ϣ��ѯ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExGetDeviceInfo(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get ExDomain Device Info Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().ExGetDeviceInfo(pReqXml);
}

//ˢ��������б�ͬ���¼���������б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RefreshCameraList(IVS_INT32 iSessionID)
{
    CHECK_POINTER(g_pNvs, IVS_PARA_INVALID);
    IVS_INFO_TRACE("IVS_SDK_RefreshCameraList SessionID: %d", iSessionID);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
    CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

    return pUserMgr->GetDeviceMgr().RefreshCameraList(iSessionID);
}

//Onvif�豸��ǰ�˻�ȡ����������Χ
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceEncodeCapability(IVS_INT32 iSessionID,const IVS_CHAR* pDevCode,
	IVS_STREAM_INFO_LIST* pStreamInfoList)
{
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pCameraCode: %s",iSessionID,pDevCode);

	if((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
	{
		BP_RUN_LOG_ERR(IVS_SDK_RET_INVALID_SESSION_ID,"IVS SDK Get Camera Encode Capabilities Failed.","Reason:SessionID is invalid");
		return IVS_SDK_RET_INVALID_SESSION_ID;
	}
	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetDeviceMgr().GetDeviceEncodeCapability(pDevCode,pStreamInfoList);
}

IVS_SDK_API void __SDK_CALL IVS_SDK_SetLogLevel(IVS_ULONG ulLogLevel)
{
    //��ȡ��ǰUTCʱ����Ϊ��ʼʱ�䣬����ʱ��Ϊ10���
    std::string strBeginUtcTime("");
    std::string strEndUtcTime("");
    (void)CSDKConfig::instance().GetUtcTimeSpan(strBeginUtcTime, strEndUtcTime, "yyyymmddHHMMss", LOG_TIME_SPAN);
    
	BP_RUN_LOG_INF("IVS_SDK_SetLogLevel", "LogLevel[%d], BeginUtcTime[%s], EndUtcTime[%s]", 
		           ulLogLevel, strBeginUtcTime.c_str(), strEndUtcTime.c_str());

    //ͨ���ӿ�����SDK��־�����BP�ײ�
    int iRet = IVS_SetLogConf(TYPE_RUN, (LOG_LEVEL)ulLogLevel, strBeginUtcTime.c_str(), strEndUtcTime.c_str());
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_SDK_RET_SET_LOG_LEVEL_ERROR, "IVS SDK Set Log Level To BP Failed.", "");
    }

    return;
}
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroup(IVS_INT32 iSessionID, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d", iSessionID);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().QueryRaidGroup("<Content></Content>", pRspXml);
}

IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroupDisk(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml)
{
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,pReqXml: %s",iSessionID,pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	return pUserMgr->GetClientNetworkMgr().QueryRaidGroupDisk(pReqXml, pRspXml);
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetReceiveOMUAlarm(
	IVS_INT32 iSessionID,
	IVS_BOOL bReceive)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	IVS_INFO_TRACE("SessionID: %d,bReceive: %d", iSessionID, bReceive);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr,IVS_SDK_RET_INVALID_SESSION_ID);
	pUserMgr->SetReceiveOMUAlarm(bReceive);

	return IVS_SUCCEED;
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoDiagnose(IVS_INT32 iSessionID,
	const IVS_CHAR* pDomainCode,
	const IVS_CHAR* pCameraCode,
	IVS_CHAR** pRspXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pRspXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, DomainCode: %s, CameraCode: %s", iSessionID, pDomainCode, pCameraCode);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetDeviceMgr().GetVideoDiagnose(pDomainCode, pCameraCode, pRspXml);
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoDiagnose(IVS_INT32 iSessionID, 
	const IVS_CHAR* pReqXml)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pReqXml, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pReqXml: %s", iSessionID, pReqXml);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetDeviceMgr().SetVideoDiagnose(pReqXml);
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByCam(IVS_INT32 iSessionID, 
	const IVS_CHAR* pDomainCode,
	const IVS_CHAR* pCameraCode,
	IVS_UINT32 uiBufSize,
	IVS_STREAM_LIST_BY_CAM* pStreamListByCam)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	CHECK_POINTER(pStreamListByCam, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pDomainCode: %s, pCameraCode: %s, uiBufSize: %d", 
		iSessionID, pDomainCode, pCameraCode, uiBufSize);

	if (sizeof(IVS_STREAM_LIST_BY_CAM) + (IVS_MAX_STREAM_USER_INFO_NUM -1) * sizeof(IVS_STREAM_USER_INFO)
		!= uiBufSize)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"IVS_SDK_GetStreamListByCam", "uiBufSize should equal %d",
			sizeof(IVS_STREAM_LIST_BY_CAM) + (IVS_MAX_STREAM_USER_INFO_NUM -1) * sizeof(IVS_STREAM_USER_INFO));
		return IVS_PARA_INVALID;
	}

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetUserInfoMgr().GetStreamListByCam(pDomainCode, pCameraCode, uiBufSize, pStreamListByCam);
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByUser(IVS_INT32 iSessionID, 
	const IVS_CHAR* pDomainCode,
	IVS_UINT32 uiUserID,
	IVS_UINT32 uiBufSize,
	IVS_STREAM_LIST_BY_USER* pStreamListByUser)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pStreamListByUser, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("SessionID: %d, pDomainCode: %s, uiUserID: %d, uiBufSize: %d", 
		iSessionID, pDomainCode, uiUserID, uiBufSize);

	if (sizeof(IVS_STREAM_LIST_BY_USER) + (IVS_MAX_STREAM_CAM_INFO_NUM -1) * sizeof(IVS_STREAM_CAM_INFO)
		!= uiBufSize)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"IVS_SDK_GetStreamListByCam", "uiBufSize should equal %d",
			sizeof(IVS_STREAM_LIST_BY_USER) + (IVS_MAX_STREAM_CAM_INFO_NUM -1) * sizeof(IVS_STREAM_CAM_INFO));
		return IVS_PARA_INVALID;
	}

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetUserInfoMgr().GetStreamListByUser(pDomainCode, uiUserID, uiBufSize, pStreamListByUser);
}


IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShutdownStreamByUser(IVS_INT32 iSessionID, 
	const IVS_CHAR* pDomainCode,
	IVS_UINT32 uiUserID,  
	const IVS_CHAR* pCameraCode,
	IVS_INT32 iStreamType,
	IVS_INT32 iDuration)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_PARA_INVALID);
	IVS_INFO_TRACE("SessionID: %d, pDomainCode: %s, uiUserID: %d, pCameraCode: %s, iStreamType: %d, iDuration: %d", 
		iSessionID, pDomainCode, uiUserID, pCameraCode, iStreamType, iDuration);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetUserInfoMgr().ShutdownStreamByUser(pDomainCode, uiUserID, pCameraCode, iStreamType, iDuration);
}


// �û�����������֤
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PasswdValidation(IVS_INT32 iSessionID,
	const IVS_CHAR* pLoginName,
	const IVS_CHAR *pPasswd)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pLoginName, IVS_PARA_INVALID);
	CHECK_POINTER(pPasswd, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetUserInfoMgr().PasswdValidation(pLoginName, pPasswd);
}

#ifdef WIN32
// ¼����Ƶ��������
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoEffect(IVS_INT32 iSessionID,
	IVS_ULONG ulPlayHandle, IVS_LONG lBrightValue,
	IVS_LONG lContrastValue, IVS_LONG lSaturationValue,
	IVS_LONG lHueValue)
{
	IVS_INFO_TRACE("Playhandle: %lu, BrightValue: %d, ContrastValue: %d, SaturationValue: %d, HueValue: %d",
		ulPlayHandle, lBrightValue, lContrastValue, lSaturationValue, lHueValue);
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetPlaybackMgr().SetVideoEffect(ulPlayHandle, lBrightValue, lContrastValue, lSaturationValue, lHueValue);
}
#endif

// ��ȡָ�������������豸�б�
IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainDeviceList(IVS_INT32 iSessionID,
	const IVS_CHAR* pDomainCode,
	IVS_UINT32 uiDeviceType,
	const IVS_INDEX_RANGE* pIndexRange,
	IVS_VOID* pDeviceList,
	IVS_UINT32 uiBufferSize)
{
	CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pIndexRange, IVS_PARA_INVALID);
	CHECK_POINTER(pDeviceList, IVS_PARA_INVALID);

	IVS_INFO_TRACE("iSessionID: %d, pDomainCode: %s, uiDeviceType: %u, pIndexRange: [%u - %u], uiBufferSize: %u",
		iSessionID, pDomainCode, uiDeviceType, pIndexRange->uiFromIndex, pIndexRange->uiToIndex, uiBufferSize);

	CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);

	return pUserMgr->GetDeviceMgr().GetDomainDeviceList(pDomainCode, uiDeviceType, pIndexRange, pDeviceList, uiBufferSize);
}