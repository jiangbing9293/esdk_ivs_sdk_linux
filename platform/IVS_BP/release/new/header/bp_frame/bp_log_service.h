/********************************************************************
filename    :    Thread.h
author      :    cwx148380
created     :    2012/11/23
description :    ��־������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/23 ��ʼ�汾
*********************************************************************/
#ifndef BP_LOG_SERVICE_H
#define BP_LOG_SERVICE_H

#include "IService.h"
#include "bp_memoutput.h"
#include "db_connect.h"

NAMESPACE_BP_BEGIN //lint !e129 !e10

class CLogService : public IService
{
public:
    virtual int HandleCmd(CCmd* pCmd){ return(0); }
    virtual uint32_t Routing(void){ return(0); }
};

class COperLogService : public IService
{
public:
    COperLogService ( CMemOutput& aMemOutput );
    virtual ~COperLogService(void);

public:
    /**************************************************************************
    * name       : HandleCmd
    * description: ���������������ʵ������Ҫ��������
    * input      : pCmd ����ָ��
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: ���ڼ�飬��������ʵ���ദ���ڼ������
				   ע�⣺һ��Ҫͳ�Ʊ���routing��ɵ��¼������̵߳���ѯ�����ڴ�ͳ�ƣ�����
    * input      : NA
    * output     : NA
    * return     : ���ر���rouint��ɵ��¼��������ں���ͳ��
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

private:
    CMemOutput & m_MemOutput;
    IDBConnect * m_connect;                   // ���ݿ�����
    const char * m_szDbSql;                   // ���SQL
};


class CSecLogService : public IService
{
public:
    CSecLogService ( CMemOutput& aMemOutput );
    virtual ~CSecLogService(void);

public:
    /**************************************************************************
    * name       : HandleCmd
    * description: ���������������ʵ������Ҫ��������
    * input      : pCmd ����ָ��
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: ���ڼ�飬��������ʵ���ദ���ڼ������
				   ע�⣺һ��Ҫͳ�Ʊ���routing��ɵ��¼������̵߳���ѯ�����ڴ�ͳ�ƣ�����
    * input      : NA
    * output     : NA
    * return     : ���ر���rouint��ɵ��¼��������ں���ͳ��
    * remark     : NA
    **************************************************************************/
    virtual uint32_t Routing(void);

private:
    CMemOutput & m_MemOutput;
    IDBConnect * m_connect;                   // ���ݿ�����
    const char * m_szDbSql;                   // ���SQL
};

NAMESPACE_BP_END

#endif //bp_log_service.h
