/********************************************************************
filename    :    NetSource.h
author      :    s00191067
created     :    2012/11/05
description :    ��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/
#ifndef NET_SOURCE_H
#define NET_SOURCE_H

#include "NetSourceCommon.h"
#include "BaseJob.h"
#include "Lock.h"

typedef enum tagRecvStreamState
{
    START_RECV_STREAM = 0,  // ��ʼ����
    STOP_RECV_STREAM        // ֹͣ����
}RECV_STREAM_STATE;

#ifndef WIN32
unsigned long GetTickCount();
#endif

class CChannelInfo;
class _BaseThread;
class CNetSource : public BaseJob
{
public:
    CNetSource(void);
    virtual ~CNetSource(void);

    /******************************************************************
     function   : Init
     description: ��ʼ��;
     input      : CChannelInfo * pChannelInfo ͨ����Ϣ�ֿ���ָ��;
     output     : NA
     return     : int �ɹ�:IVS_SUCCEED ʧ��:IVS_FAIL;
    *******************************************************************/
    virtual int Init(CChannelInfo* pChannelInfo);

    /******************************************************************
     function   : Release
     description: �ͷ�����;
     output     : NA
     return     : void
    *******************************************************************/
    virtual void Release();

    /******************************************************************
     function   : DoExceptionCallBack
     description: �����쳣�ص�;
     input      : int iMsgType  �쳣��Ϣ����;
     input      : void * pParam �쳣���ݣ�������Ϣ���Ͷ�Ӧ��ͬ�Ľṹ��;
     output     : NA
     return     : void
    *******************************************************************/
    void DoExceptionCallBack(int iMsgType, void*  pParam);

    /******************************************************************
     function   : MainLoop
     description: ��ѭ��
     output     : NA
     return     : void
    *******************************************************************/
    virtual void MainLoop();

    /******************************************************************
     function   : kill
     description: ֹͣ����
     output     : NA
     return     : void
    *******************************************************************/
    virtual void kill();

    /******************************************************************
     function   : Start
     description: ��������
     output     : NA
     return     : int
    *******************************************************************/
    int Start();

    /******************************************************************
     function   : Stop
     description: ֹͣ����
     output     : NA
     return     : void
    *******************************************************************/
    void Stop();

protected:
    /******************************************************************
     function   : ProcessRecevData
     description: ����
     output     : NA
     return     : void
    *******************************************************************/
    virtual void ProcessRecevData() = 0;

	/******************************************************************
     function   : CheckNatSend
     description: ����Ƿ�����NAT��Խ����,ÿ��30s�����������nat��Ϣ;
     output     : NA
     return     : int IVS_FAIL��ʾ������������ IVS_SUCCEED��ʾ��������;
    *******************************************************************/
    virtual int CheckNatSend();

	/******************************************************************
     function   : ProcMediaPacket
     description: �ַ�ý�屨��
     output     : NA
     return     : void
    *******************************************************************/
    void ProcMediaPacket();

    /******************************************************************
     function   : CheckTimeOut
     description: ����Ƿ�ʱ
     output     : NA
     return     : int
    *******************************************************************/
    virtual int CheckTimeOut();

protected:
    CChannelInfo*       m_pChannelInfo;				// ͨ����Ϣ����;
	_BaseThread*		m_pSourceThread;			// �����߳�;
    char*               m_pRecvBuf;                 // ����Ƶ������������;
    unsigned long       m_ulRecvBufLen;             // ����Ƶ�����������ݳ���;
    unsigned long       m_ulLastSendNatTick;        // �ϴη���Nat��Խ����ʱ��;
    unsigned long       m_ulLastCheckTimeoutTick;   // �ϴο�ʼ���㳬ʱ��ʱ��;
	bool                m_bRun;                     // �߳����б�־; 
	CIVSMutex			m_RunFlagMutex;				// ���б�־����;
};
#endif //NET_SOURCE_H
