/********************************************************************
filename    :    NetElementMgr.h
author      :    j00213308
created     :    2012/04/07
description :    ��Ԫ������Ԫ���ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 ��ʼ�汾
*********************************************************************/
#ifndef NET_ELEMENT_MGR_H
#define NET_ELEMENT_MGR_H

#include "net_element.h"

NAMESPACE_BP_BEGIN        // IVS NAMESPACE

// ����Ԥ����
class CCmd;
class ILink;
class ILinkCreater;

// �����ض���
typedef std::map<int, INetElement*>              CNetElementMap;     // ��Ԫ����(int) ����Ԫ���� map
typedef CNetElementMap::iterator    NetElementMapIterator;

//static const int MAX_SELECT_WAIT_TIME  = 20*1000;            // �׽���select�ļ��ʱ�䣬��λ:����


// ��Ԫ������
class EXPORT_DLL CNetElementMgr
{
public:
    CNetElementMgr(void);
    ~CNetElementMgr(void);

    /**************************************************************************
    * name       : HandleCmd
    * description: ��Ϣ�����
    * input      : pCmd : ���������Ϣ����
    * output     : NA
    * return     : 0 - ��Ϣ�ѱ�����, -1 - ��Ϣ��δ������
    * remark     : NA
    **************************************************************************/
    int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: �г̴���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    uint32_t Routing(void);

    /**************************************************************************
    * name       : AddElement
    * description: ������Ԫ����
    * input      : iElementType: ��Ԫ����
    *            : pElement    : �������Ԫ����
    * output     : NA
    * return     : true - �ɹ�, false - ʧ��
    * remark     : 1. �̰߳�ȫ
    *              2. �����������ͬ���͵���Ԫ
    **************************************************************************/
    bool AddElement(int iElementType, INetElement* pElement);

    /**************************************************************************
    * name       : RmvElement
    * description: �Ƴ���Ԫ����
    * input      : iElementType: ��Ԫ����
    * output     : NA
    * return     : ��NULL - �ɹ�, NULL - ʧ��
    * remark     : 1. �̰߳�ȫ
    *              2. ������ָ������Ԫ���ͣ��򷵻�ʧ��
    *              3. ���ص�ָ��Ϊ���Ƴ��Ķ���ָ��
    **************************************************************************/
    INetElement* RmvElement(int iElementType);

	/**************************************************************************
    * name       : GetElement
    * description: ������Ԫ���ͻ�ȡ��Ԫ����ָ��
    * input      : iElementType: ��Ԫ����
    * output     : NA
    * return     : ��NULL - �ɹ�, NULL - ʧ��
    * remark     : NA
    **************************************************************************/
    INetElement* GetElement(int iElementType);

	/**************************************************************************
    * name       : GetEpollFD()
    * description: ������Ԫ���ͻ�ȡ��Ԫ����ָ��
    * input      : iElementType: ��Ԫ����
    * output     : NA
    * return     : ��NULL - �ɹ�, NULL - ʧ��
    * remark     : NA
    **************************************************************************/
	int GetEpollFD(void){return m_epollFD;};

private:

    /**************************************************************************
    * name       : DoRouting
    * description: ����������Ԫ����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    uint32_t DoRouting(void);

    /**************************************************************************
    * name       : WaitForEvent
    * description: �ȴ�ע����׽��־���ϵ������¼�����
    * input      : NA
    * output     : NA
    * return     : ���ر��οɶ�д���ļ��������ں���ͳ��
    * remark     : NA
    **************************************************************************/
    uint32_t WaitForEvent(void);

    /**************************************************************************
    * name       : RegisterHandle
    * description: ע��������Ԫ�����Ӷ����Լ����Ƿ��������¼�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void RegisterHandle(void);

    /**************************************************************************
    * name       : UnRegisterHandle
    * description: ע�����Ӷ���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DeregisterHandle(void);

	/**************************************************************************
    * name       : EpollEvent
    * description: ��������¼�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void EpollEvent();

private:
    ACE_Handle_Set m_rdHandleSet;            // ���Ӷ��������¼������
    ACE_Handle_Set m_wtHandleSet;
    ACE_Handle_Set m_exHandleSet;
    bool           m_bMonitorRead;           // �Ƿ�select�ɶ��¼�
    bool           m_bMonitorWrite;          // �Ƿ�select��д�¼�
    bool           m_bMonitorExp;            // �Ƿ�select�쳣�¼�

    RecursiveMutex m_lock;                   // ͬ����
    CNetElementMap m_mapElement;             // ������Ԫ
	
	int            m_epollFD;                // epoll���õľ��

#ifndef WIN32
	int            m_maxevents;
	struct epoll_event *m_events;
#endif //WIN32

};


NAMESPACE_BP_END

#endif // NET_ELEMENT_MGR_H
