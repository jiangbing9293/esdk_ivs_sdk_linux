/********************************************************************
filename    :    IProtocol.h
author      :    j00213308
created     :    2012/04/06
description :    Э��ջ�ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 ��ʼ�汾
*********************************************************************/
#ifndef IPROTOCOL_H
#define IPROTOCOL_H

#include "bp_def.h"
#include "Cmd.h"
#include "IMsgHandle.h"
#include "ILink.h"

NAMESPACE_BP_BEGIN        // IVS NAMESPACE

// Э��ӿ�
class IProtocol
{
public:
    IProtocol(void)
        : m_pILink(NULL)
        , m_pMsgHandle(NULL)
    {}
    virtual ~IProtocol(void) { HW_DELETE(m_pMsgHandle); }
private:
    IProtocol(const IProtocol&src)
    {
    }

    IProtocol& operator=(const IProtocol&)
    {
        return *this;
    }

public:

    /**************************************************************************
    * name       : HandleCmd
    * description: ����������Ϣ
    * input      : pCmd : �账���������Ϣ
    * output     : NA
    * return     : NA
    * remark     : �����ڴ�ͨ��Э��ջ������Ϣ��
    **************************************************************************/
    virtual void HandleCmd(CCmd* pCmd) = 0;
    
    /**************************************************************************
    * name       : Routing
    * description: �г̴���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : ����Э��ջ������
    **************************************************************************/
    virtual void Routing(void) = 0;

    /**************************************************************************
    * name       : NotifyOpen
    * description: ֪ͨЭ��ջ���ӽ���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : ���ӽ���ʱ�����ص���֪ͨЭ��ջ
    **************************************************************************/
    virtual void NotifyOpen(void) = 0;

    /**************************************************************************
    * name       : NotifyClose
    * description: ֪ͨЭ��ջ���ӶϿ�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : ���ӶϿ�ʱ�����ص���֪ͨЭ��ջ
    **************************************************************************/
    virtual void NotifyClose(void) = 0;

    /**************************************************************************
    * name       : NotifyTimeout
    * description: ֪ͨЭ��ջ���ӳ�ʱ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : ���ӳ�ʱʱ�����ص���֪ͨЭ��ջ
    **************************************************************************/
    virtual void NotifyTimeout(void) = 0;
    
    /**************************************************************************
    * name       : NotifyRecv
    * description: ֪ͨЭ��ջ�����ݵ���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : �����������ݵ���ʱ�����ص���֪ͨЭ��ջ��
    *              ��ʱ��Э��ջӦ�������Ӷ���Ľ������ݺ���
    **************************************************************************/
    virtual int NotifyRecv(void) = 0;

	/**************************************************************************
	* name       : SetLink
	* description: �������Ӷ���
	* input      : pILink�����Ӷ���
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    inline void SetLink(ILink* pILink) { m_pILink = pILink; }
    
	/**************************************************************************
	* name       : SetMsgHandle
	* description: ������Ϣ�������
	* input      : pMsgHandle����Ϣ�������
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    inline void SetMsgHandle(IMsgHandle* pMsgHandle) { m_pMsgHandle = pMsgHandle; }

	/**************************************************************************
	* name       : LinkStatusReport
	* description: ����״̬�ϱ�
	* input      : linkState�� ����״̬�������д򿪣���ɣ���ʱ����
	*              pszTransID������ID
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    virtual void LinkStatusReport(enum enLinkState linkState, const char* pszTransID) = 0;

protected:
    ILink*        m_pILink;      // ���Ӷ���ָ��
    IMsgHandle*   m_pMsgHandle;  // ��Ϣ������ָ��
};

NAMESPACE_BP_END

#endif    // IPROTOCOL_H
