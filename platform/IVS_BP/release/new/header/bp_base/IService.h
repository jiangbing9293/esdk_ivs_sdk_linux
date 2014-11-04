/********************************************************************
filename    :    IService.h
author      :    s00191067
created     :    2012/03/27
description :    ����ӿ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_ISERVICE_H
#define BP_ISERVICE_H

#include <map>
#include "ace_header.h"
#include "bp_def.h"
#include "bp_stdint.h"


NAMESPACE_BP_BEGIN // namespace begin
class CCmd;
class EXPORT_DLL IService
{
public:
    IService(void);
    virtual ~IService(void);

public:
    virtual int Init();

    virtual int Close();

    /**************************************************************************
    * name       : HandleCmd
    * description: ���������������ʵ������Ҫ��������
    * input      : pCmd ����ָ��
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd) = 0;

    /**************************************************************************
    * name       : Routing
    * description: ���ڼ�飬��������ʵ���ദ���ڼ������
				   ע�⣺һ��Ҫͳ�Ʊ���routing��ɵ��¼������̵߳���ѯ�����ڴ�ͳ�ƣ�����
    * input      : NA
    * output     : NA
    * return     : ���ر���rouint��ɵ��¼��������ں���ͳ��
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void) = 0;

    /**************************************************************************
    * name       : GetServicePayload
    * description: ��ȡService��ǰ����Ȩ��
    * input      : NA
    * output     : NA
    * return     : ����Ȩ��
    * remark     : Ĭ�������Service Ȩ��Ϊ0, MeidaService �迼��session Ȩ��ֵ
    **************************************************************************/
    virtual unsigned int GetServicePayload();
   

    void SetServiceID(uint32_t uiServiceID);

    uint32_t GetServiceID() const;
	virtual uint32_t GetSessionCount(){return 0;}

    void SetResendIntervalOffset(uint32_t uiResendIntervalOffset);

    uint32_t GetResendIntervalOffset() const;

    void SetResendTimes(uint32_t uiResendTimes);

    uint32_t GetResendTimes() const;

protected:
    uint32_t m_uiResendIntervalOffset;
    uint32_t m_uiResendTimes;

private:
    uint32_t m_uiServiceID;
};

class EXPORT_DLL CServiceSet
{
public:
    CServiceSet();
    ~CServiceSet();

    /**************************************************************************
    * name       : GetServiceNum
    * description: ��ȡService����
    * input      : NA
    * output     : NA
    * return     : Service����
    * remark     : NA
    **************************************************************************/
    unsigned int GetServiceNum(void);

    /**************************************************************************
    * name       : AddService
    * description: ����Service��Service����
    * input      : pService Service����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int AddService(IService* pService);

    /**************************************************************************
    * name       : DelService
    * description: ɾ��Service����
    * input      : pService Service����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int DelService(IService*& pService);

    /**************************************************************************
    * name       : FindService
    * description: ����ServiceID����Service����
    * input      : enServiceID ServiceID
    * output     : NA
    * return     : Service����ָ��
    * remark     : NA
    **************************************************************************/
    IService* FindService(uint32_t enServiceID);

    /**************************************************************************
    * name       : RoutingAll
    * description: ��������Service��Routing��ִ��Set��������Service��Routing
    * input      : NA
    * output     : NA
    * return     : ���ر��ӿ�����ɵ����飬���ں���ͳ��
    * remark     : NA
    **************************************************************************/
    uint32_t RoutingAll();

    /**************************************************************************
    * name       : DelAllService
    * description: ��������Service
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DelAllService(void);


    /**************************************************************************
    * name       : GetTotalPayload
    * description: ��ȡservice��������serverice�ĸ���Ȩ�غ�
    * input      : NA
    * output     : NA
    * return     : ����service����Ȩ�غ�
    * remark     : NA
    **************************************************************************/
    unsigned int GetTotalPayload();
	uint32_t GetSessionCount();

private:
    typedef std::map<uint32_t, IService*>   CServiceMap;
    typedef CServiceMap::iterator           ServiceIter;
    CServiceMap                             m_mapService;
    ACE_Thread_Mutex                        m_mutex;              // �̸߳����б���;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_ISERVICE_H
