/********************************************************************
 filename    :    EventMgr.h
 author      :    s00191067
 created     :    2012/11/20
 description :    �쳣��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/20 ��ʼ�汾
*********************************************************************/
#ifndef EVENT_MGR_H
#define EVENT_MGR_H

// #include <queue>
#include "vos.h"
#include "NetSourceCommon.h"
#include "BaseJob.h"
#include "rtsp_client_api.h"
#include "IVS_Player.h"
#include "SDKDef.h"

typedef struct
{
	int iSessionID;
	int iEventType;
	int iBufSize;
	void* pEventData;
}EVENT_CALL_BACK_DATA;

class _BaseThread;
class CEventMgr
{
public:
    CEventMgr(void);

    ~CEventMgr(void);

    int SetEventCallBack( int iSessionID, EventCallBack fEventCallBack = NULL, void *pUserData = NULL );

    int Init();

    void UnInit();

    int OnEventCallBack(int iSessionID, int iEventType, void* pEventData, int iBufSize);

    PLAYER_EXCEPTION_CALLBACK GetPlayerExceptionCallBack() const;

    NET_EXCEPTION_CALLBACK GetNetExceptionCallBack() const;

    fExceptionCallBack GetRtspExceptionCallBack() const;

private:
    void HandleStreamMsg(int iSessionID, int iEventType, void* pEventData, int iBufSize);

private:
    //��Ϊ�б��ػ��¼���sessionidΪ-1������Ҫ��1;
    EventCallBack  m_EventCallBack[SDK_CLIENT_TOTAL + 1];
    void*          m_EventUserData[SDK_CLIENT_TOTAL + 1];
	VOS_Mutex*     m_pMutexEventCB;				// �¼��ص���;
    _BaseThread     *m_pEventCallbackThread;    //�¼��쳣�ص�;

};
#endif //EVENT_MGR_H
