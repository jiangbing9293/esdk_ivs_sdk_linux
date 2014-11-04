/********************************************************************
filename    :    CapabilityMgr.h
author      :    s00191067
created     :    2012/03/27
description :    ��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_CAPABILITY_MGR_H
#define BP_CAPABILITY_MGR_H

#include <vector>
#include <map>
#include <list>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"

using std::vector;

NAMESPACE_BP_BEGIN // namespace begin

typedef struct stCapabilityInfo
{
    uint32_t        ReqID;      // ����ID,��Ӧ��Ϣ����
    uint32_t        ServiceID;  // ����ID
	std::string     ServiceName; //���������
    ACE_thread_t    ThreadID;   // �߳�ID
    int     ModuleType; // ģ������

    stCapabilityInfo()
        : ReqID(0)
        , ServiceID(0)
        , ServiceName("")
        , ThreadID(0)
        , ModuleType(0)
    {
    }

    stCapabilityInfo& operator = (const stCapabilityInfo& info)
    {
        this->ReqID = info.ReqID;
        this->ServiceID = info.ServiceID;
        this->ThreadID = info.ThreadID;
		this->ServiceName = info.ServiceName;
        this->ModuleType = info.ModuleType;
        return *this;
    }

}CAPABILITY_INFO;

typedef std::list<CAPABILITY_INFO*> CCapabilityList;
typedef CCapabilityList::iterator   CapabilityListIter;

class EXPORT_DLL CCapabilityMgr
{
public:
    virtual ~CCapabilityMgr(void);

    static CCapabilityMgr& instance();

    void close();

    /**************************************************************************
    * name       : RegCapability
    * description: ע������
    * input      : enReqID ����ID 
                   enServiceID ����ID
                   threadID �߳�ID
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : ���ĳService�ܴ���10�����󣬸�Service��Ҫע��10��
    **************************************************************************/
    int RegCapability(uint32_t enReqID, uint32_t enServiceID, ACE_thread_t threadID, int moduleType = 0);

    /**************************************************************************
    * name       : GetCapability
    * description: ���ݸ��������ȡ���ŵ���������
    * input      : enReqID ����ID 
    * output     : stCapabilityInfo �������ŵ���������
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : �̵߳ĸ��ؾ���ʵ�ַ���������list���ȡ��Ϣ���г�����̵��̡߳�
    **************************************************************************/
    int GetCapabiltybyLoad(uint32_t enReqID, CAPABILITY_INFO& stCapabilityInfo);

    int GetModuleType(uint32_t enReqID, int& outModuleType);

   /**************************************************************************
    * name       : GetCapabilityAll
    * description: ��ȡ�ܴ��������������������
    * input      : enReqID ����ID 
    * output     : vecCapabilityInfo ������������
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : �̵߳ĸ��ؾ���ʵ�ַ���������list���ȡ��Ϣ���г�����̵��̡߳�
    **************************************************************************/
    int GetCapabilityAll(uint32_t enReqID, std::vector<CAPABILITY_INFO>& vecCapabilityInfo);

private:
    CCapabilityMgr(void);

    /**************************************************************************
    * name       : GetCapability
    * description: ��ȡ������
    * input      : enReqID ����ID 
    * output     : NA
    * return     : CCapabilityList* ��������ID��ȡ�ܴ�������������List��
    * remark     : �̵߳ĸ��ؾ���ʵ�ַ���������list���ȡ��Ϣ���г�����̵��̡߳�
    **************************************************************************/
    CCapabilityList* GetCapability(uint32_t enReqID);

private:
	static CCapabilityMgr m_Instance;

    typedef std::map<uint32_t, CCapabilityList*> CCapabilityMap;     //keyΪ����ID
    typedef CCapabilityMap::iterator    CapabilityMapIter;
    CCapabilityMap                      m_mapCapability;
    ACE_Thread_Mutex                    m_mutex;  // �̸߳����б���;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_CAPABILITY_MGR_H
