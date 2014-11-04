/********************************************************************
	filename	: 	IVSSDK.cpp
	author		:	s00191067
	created		:	2011/10/29	
	description	:	���� DLL Ӧ�ó���ĵ���������
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/29 ��ʼ�汾
*********************************************************************/
/*!< Ԥ����ͷ�ļ� */
#include "stdafx.h"
#include <WinSock2.h>   //Socktͨ��ʹ��

/*!< ����ͷ�ļ� */
#include "IVSSDKNetKeyBoard.h"
#include "NetKeyBoardService.h"
#include "IVS_Trace.h"

//��¼Ȩ�ޱ�ʶ
enum enNetKeyBoardVendor
{
    NET_KEYBOARD_DAHUA = 1,
    NET_KEYBOARD_TDWY,
};

static HANDLE g_hThread = NULL;                  //�߳̾��

/**************************************************************************
* name       : SetListenAddr
* description: ���÷������˼���IP�Ͷ˿ں�
* input      : iPort�������˼����˿ں�
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void __stdcall IVS_SDK_KB_SetNetKeyBoardListenAddr(u_short iPort)
{
    IVS_INFO_TRACE("NetKeyBoard ");
    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER_VOID(pNetKeyBoardService);
    if (true)
    {
        pNetKeyBoardService->m_UDPSocketService.SetServerAddr(iPort);
    }
    else
    {
        pNetKeyBoardService->m_SocketService.SetServerAddr(iPort);
    }
}

/**************************************************************************
* name       : StartListenThread
* description: ���������߳�
* input      : lpCallBack �ص�����
* output     : NA
* return     : g_hThread
* remark     : NA
**************************************************************************/
int __stdcall IVS_SDK_KB_StartNetKeyBoardListenThread(u_short ulNetKeyBoardVendor, u_short ulPort, LPCallBack lpCallBack)
{
    IVS_INFO_TRACE("NetKeyBoard Start NetKeyBoard Listen Thread: ulNetKeyBoardVendor = %d, ulPort = %d.", ulNetKeyBoardVendor, ulPort);
    
    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER(pNetKeyBoardService,IVS_FAIL);

    // �����
    if (NET_KEYBOARD_DAHUA == ulNetKeyBoardVendor)
    {
        pNetKeyBoardService->m_SocketService.SetServerAddr(ulPort);

        int iRet = pNetKeyBoardService->IniTcpServer();
        if (IVS_SUCCEED != iRet)
        {
            BP_DBG_LOG("IVS_SDK_KB_StartNetKeyBoardListenThread: Init TCP Server Failed.");
            return iRet;
        }

        g_hThread = CreateThread(NULL,0,pNetKeyBoardService->ListenSocket,(LPVOID)lpCallBack,0,NULL);
    }
    // �������ΰҵ
    else
    {
        pNetKeyBoardService->m_UDPSocketService.SetServerAddr(ulPort);

        int iRet = pNetKeyBoardService->IniUdpServer();
        if (IVS_SUCCEED != iRet)
        {
            BP_DBG_LOG("IVS_SDK_KB_StartNetKeyBoardListenThread: Init UDP Server Failed.");
            return iRet;
        }

        g_hThread = CreateThread(NULL,0,pNetKeyBoardService->ListenUDPSocket,(LPVOID)lpCallBack,0,NULL);
    }

    if (NULL == g_hThread)
    {
        int iRet;
        iRet = GetLastError();
        BP_RUN_LOG_ERR(IVS_FAIL, "Start Listen Thread Failed! ", "Error Code: %d",iRet);

        pNetKeyBoardService->ReleaseListenServer();
        return iRet;
    }

    return IVS_SUCCEED;
}

/**************************************************************************
* name       : ReleaseThread
* description: �ͷ��߳�
* input      : NA
* output     : NA
* return     : NA
* remark     : NA
**************************************************************************/
void __stdcall IVS_SDK_KB_ReleaseThread()
{
    IVS_INFO_TRACE("NetKeyBoard ");
    CNetKeyBoardService *pNetKeyBoardService = CNetKeyBoardService::GetInstance();
    CHECK_POINTER_VOID(pNetKeyBoardService);
    pNetKeyBoardService->ReleaseListenThread(g_hThread);
}

