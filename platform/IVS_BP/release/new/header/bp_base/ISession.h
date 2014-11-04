/********************************************************************
filename    :    ISession.h 
author      :    b00209723
created     :    2012/04/12
description :    ý��Ự�ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/04/12 ��ʼ�汾
*********************************************************************/
#ifndef  BP_I_SESSION_H
#define  BP_I_SESSION_H
//#include "cmd.h"
#include "bp_stdint.h"
#include <time.h>
#include "bp_namespace.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN
class CCmd;

//�Ự״̬��-1����ʼ��״̬��0������״̬������״̬��ҵ����
const int SessionStatus_Init = -1;
const int SessionStatus_Complete = 0;

//Ĭ�ϻỰ��ʱʱ��
const int SessionTimeoutDef = 30;

class ISession
{
public:
    ISession()
        : m_status(SessionStatus_Init)
        , m_lastCmdTime(ACE_OS::time())
        , m_iTimeout(SessionTimeoutDef)
        , m_uiServiceID(0)
    {

    }

    virtual ~ISession(){};

    /**********************************************************************
    * name           : GetStatus
    * description    : ��ȡ״̬
    * input          : 
    * return         : �Ự״ֵ̬
    * remark         : ��ȡ�Ự״̬
    *********************************************************************/
    int GetStatus() const {return m_status;}
	void SetStatus(int iStatus){m_status = iStatus;}
	void UpdateLastCmdTime(time_t tLastCmdTime){m_lastCmdTime = tLastCmdTime;}

	bool IsComplete(){return (SessionStatus_Complete == m_status);}

    /**********************************************************************
    * name           : HandleCmd
    * description    : �����������
    * input          : CCmd* �������
    * output         : NA
    * return         : IVS_SUCCEED �ɹ� IVS_FAIL ʧ��
    * remark         : ���麯����������ʵ����δ�������
    *********************************************************************/
    virtual int HandleCmd(CCmd *) = 0;

    /**********************************************************************
    * name           : Routing
    * description    : ���д���
    * input          : NA
    * return         : NA
    * remark         : ���麯����������������д�����
    *********************************************************************/
	virtual void Routing() = 0;
    
    /**********************************************************************
    * name           : IsTimeout
    * description    : �жϻỰ�Ƿ�ʱ
    * input          : time_t ��ǰʱ��
    * return         : True �Ự��ʱ��False �Ự����
    *********************************************************************/
	bool IsTimeout(time_t tCurrentTime)
	{
	    return ((tCurrentTime - m_lastCmdTime) >= m_iTimeout);
	}

	void SetTimeout(int iSessionTimeout)
	{
	    m_iTimeout = iSessionTimeout;
	}

    void SetServiceID(uint32_t uiServiceID) { m_uiServiceID = uiServiceID; }

    uint32_t GetServiceID() const { return(m_uiServiceID); }

    /**********************************************************************
    * name           : Timeout
    * description    : ��ʱ��Ĵ������� 
    * input          : NA
    * return         : NA   
    * remark         : ���麯����������ʵ�ֳ�ʱ��������
    *********************************************************************/
    virtual void Timeout() = 0;

private:
    int       m_status;         // ��¼�Ự�ڲ�״̬
    time_t    m_lastCmdTime;    // ǰ��ҵ����ʱ�䣬�ж��Ƿ�ʱ

	int       m_iTimeout;
    uint32_t  m_uiServiceID;
};

NAMESPACE_BP_END   // namespace end
#endif 
