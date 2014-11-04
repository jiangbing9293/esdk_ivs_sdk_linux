/********************************************************************
filename	: 	IVS_SDKNetKeyBoard.h 
author		:	x00192614
created		:	2012/12/21	
description	:	SDK�����ṩ���������ؽӿڶ���
                ��Ϊ������̽ӿڲ����⿪�ţ�ֻ��OCXʹ�ã����Բ�����IVS_SDK.h��
copyright	:	Copyright (C) 2011-2015
history		:	2012/12/21 ��ʼ�汾;
*********************************************************************/
#ifndef __IVS_SDK_NETKEYBOARD_H__
#define __IVS_SDK_NETKEYBOARD_H__

#include "hwsdk.h"
#include "vos.h"
#include "IVSSDKNetKeyBoardDef.h"

#ifdef WIN32
#ifdef  IVS_SDK_EXPORTS
#define IVS_SDK_API __declspec(dllexport)
#else
#define IVS_SDK_API __declspec(dllimport)
#endif
#else
//#define IVS_SDK_API __attribute__((visibility))
#define IVS_SDK_API __attribute__((visibility("default")))
#endif

#ifdef __cplusplus
extern "C"
{
#endif
    /*********************************************************************************************
    * function       : SetListenAddr
    * description: ���÷������˼���IP�Ͷ˿ں�
    * input      : iPort�������˼����˿ں�
    * output     : NA
    * return     : NA
    * remark     : NA
    *********************************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetNetKeyBoardPort(ULONG ulPort);

    /*********************************************************************************************
    * name       : StartListenThread
    * description: ���������߳�
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : g_hThread
    * remark     : NA
    *********************************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartNetKeyBoard(ULONG ulNetKeyBoardVendor, ULONG ulPort, LPCallBack lpCallBack);

    /*********************************************************************************************
    * name       : ReleaseThread
    * description: �ͷ��߳�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    *********************************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopNetKeyBoard();
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__IVS_SDK_NETKEYBOARD_H__
