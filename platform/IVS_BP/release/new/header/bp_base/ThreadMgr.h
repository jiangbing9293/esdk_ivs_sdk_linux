/********************************************************************
filename    :    ThreadMgr.h
author      :    s00191067
created     :    2012/03/28
description :    �̹߳�����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/28 ��ʼ�汾
*********************************************************************/
#ifndef BP_THREAD_MGR_H
#define BP_THREAD_MGR_H
#include <map>
#include "Thread.h"
#include "bp_namespace.h"

NAMESPACE_BP_BEGIN // namespace begin
typedef std::map<ACE_thread_t, CThread*> CThreadMap;
typedef CThreadMap::iterator             ThreadMapIter;
class EXPORT_DLL CThreadMgr
{
public:
    virtual ~CThreadMgr(void);

    static CThreadMgr& instance();

    void close();

    /**************************************************************************
    * name       : AddThread
    * description: ���̹߳������������߳�
    * input      : pThread �̶߳���ָ��
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ע�⣺���̲߳��ܼ��뵽ThreadMgr�й�����ΪThreadMgr����ʱ��
                   ������ע����߳����٣������߳���Ҫ��������
    **************************************************************************/
    int AddThread(CThread* pThread);

    /**************************************************************************
    * name       : DelThread
    * description: ɾ���߳�
    * input      : threadID �߳�ID
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
	int DelThread(ACE_thread_t threadID);

    /**************************************************************************
    * name       : GetThreadMap
    * description: ��ȡ�߳�map��
    * input      : NA
    * output     : NA
    * return     : CThreadMap* �߳�map��
    * remark     : NA
    **************************************************************************/
    CThreadMap* GetThreadMap();

	/**************************************************************************
    * name       : GetThread
    * description: ��ȡ�߳�
    * input      : threadID
    * output     : NA
    * return     : CThread* �߳�
    * remark     : NA
    **************************************************************************/
    CThread* GetThread(ACE_thread_t threadID);

	/**************************************************************************
    * name       : GetThreadByName
    * description: ��ȡ�߳�
    * input      : strThreadName
    * output     : NA
    * return     : CThread* �߳�
    * remark     : NA
    **************************************************************************/
    CThread* GetThreadByName(std::string strThreadName);


    /**************************************************************************
    * name       : AddCommuThread
    * description: ע����Ԫͨ���߳�
    * input      : pThread ��Ԫ�̶߳���ָ��
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void AddCommuThread(CThread* pThread);

    /**************************************************************************
    * name       : Notify
    * description: ����ҵ���߳�ID����ҵ���߳�������������Ϣ
    * input      : threadID �߳�ID pCmd�������
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ����Ϣ���Ÿ�ҵ���߳�
    **************************************************************************/
    int Notify(ACE_thread_t threadID, CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyEx
    * description: ����ҵ���߳�ID����ҵ���߳�������������Ϣ
    * input      : threadID �߳�ID pCmd�������, lVal ˽��ֵ
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ����Ϣ���Ÿ�ҵ���߳�
    **************************************************************************/
    int NotifyEx(ACE_thread_t threadID, CCmd* pCmd, long lVal);

    /**************************************************************************
    * name       : Notify2CommuThread
    * description: ��������Ԫͨ���߳�
    * input      : cmd �������
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ����Ϣ��������Ԫ�߳�
    **************************************************************************/
    //int Notify2CommuThread(CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyCommu
    * description: ��������Ԫͨ���߳�
    * input      : cmdType   : ��������
	*              netType   : �������
	*              ulLinkId  : ���Ӻ�
    *              strTransID: ����id
	*              pPkgData  : ����
	*              iDataLen  : ���ݳ���
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ����Ϣ��������Ԫ�߳�
    **************************************************************************/
	//int NotifyCommu(enCmdType cmdType, enNetElemType netType, unsigned long ulLinkId, const std::string& strTransID, const char* pPkgData, unsigned int iDataLen);

    /**************************************************************************
    * name       : GetPayLoad
    * description: ��ȡָ���̸߳���Ȩֵ
    * input      : threadID �߳�ID
    * output     : iPayLoad �̸߳���Ȩֵ
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - �̲߳�����
    * remark     : ����Ԫ�߳�
    **************************************************************************/
    int GetPayLoad(ACE_thread_t threadID, unsigned int& iPayLoad);
	void PrintSessionStatInfo();
	void PrintCmdQueueStatInfo();
private:
    CThreadMgr(void);

private:
	static CThreadMgr m_Instance;

    CThreadMap              m_mapThread;

    ACE_Recursive_Thread_Mutex        m_mutexThread;      // �߳���
};

NAMESPACE_BP_END   // namespace end
#endif  //BP_THREAD_MGR_H
