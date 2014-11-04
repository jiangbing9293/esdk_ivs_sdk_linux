/********************************************************************
    filename	: 	CumwCommon.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	����ͷ�ļ�����
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/

#ifndef CUMW_COMMON_H
#define CUMW_COMMON_H

//ȥ��map�澯
#pragma warning(disable : 4786 4503)

//��׼��ͷ�ļ�
#include <string>
#include <sstream>

//VOSͷ�ļ�
#include "vos.h"            //��ƽ̨���Ͷ���
#include "SyncQueue.h"     //��ƽ̨��Ϣ����
#include "ConnMgr.h"       //��ƽ̨ͨ��ƽ̨
#include "NVSTimer.h"      //��ƽ̨��ʱ��
#include "LockGuard.h"      //�Խ���
#include "LockableObject.h" //��������
#include "bp_log.h"
//CUMW��ͷ�ļ�
#include "hwsdk.h"
#include "ivs_error.h"
#include "IVS_SDKTVWall.h"
#include "IVSCommon.h"
#include "CumwInterface.h"      //����ӿ�
#include "CumwTypes.h"          //�ڲ����Ͷ���
#include "CumwStatusClasses.h"  //�ص�֪ͨ
#include "Business.h"          //ҵ�������

//#include <windows.h>
#define IVS_RUN_LOG_EMR(fmt, ...) \
	(void)IVS_LogRun(IVSLOG_EMERG, "TVWallDecoder", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define IVS_RUN_LOG_CRI(fmt, ...) \
	(void)IVS_LogRun(IVSLOG_CRIT, "TVWallDecoder", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define IVS_RUN_LOG_ERR(fmt, ...)\
	(void)IVS_LogRun(IVSLOG_ERR, "TVWallDecoder", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define IVS_RUN_LOG_WAR(fmt, ...)\
	(void)IVS_LogRun(IVSLOG_WARNING, "TVWallDecoder", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define IVS_RUN_LOG_INF(fmt, ...)\
	(void)IVS_LogRun(IVSLOG_INFO, "TVWallDecoder", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__);
#define IVS_DBG_LOG(fmt, ...) \
	(void)IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif //CUMW_COMMON_H
