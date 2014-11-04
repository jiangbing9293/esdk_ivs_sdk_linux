/********************************************************************
filename    :    MKTransID.h
author      :    h00211123
created     :    2012/05/09
description :    
copyright   :    Copyright HUAWEI (C) 2011-2015
history     :    2012/05/09 ��ʼ�汾
*********************************************************************/
#ifndef MK_TRANSID_H
#define MK_TRANSID_H

#include <string>
#include "ace_header.h"
#include "bp_namespace.h"
#include "bp_def.h"


NAMESPACE_BP_BEGIN

typedef ACE_Recursive_Thread_Mutex	MutexThread;

class EXPORT_DLL CMKTransID
{
public:
    virtual ~CMKTransID(void){};

    static CMKTransID& Instance();

	/**************************************************************************
    * name       : SetTransParam
    * description: ����transid�Ĳ���
				   ע�⣺�ڵ���GenerateTransID֮ǰһ��Ҫ���ñ��ӿڽ������ã������ָ���ģ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : ���ڸ���ģ����������id
    **************************************************************************/
    void SetTransParam(const std::string& strModName, const std::string& strDevId);

	/**************************************************************************
    * name       : GenerateTransID
    * description: ����һ���µ�transid
    * input      : NA
    * output     : NA
    * return     : �µ�transid
    * remark     : ���ڸ���ģ����������id
    **************************************************************************/
	std::string GenerateTransID();

private:
    void FixString(std::string& str, unsigned int uiLen, char c);

private:
    CMKTransID(void);
    MutexThread  m_mutex;

	unsigned long m_ulTransIDNumFeed;
    std::string   m_strModuleName;
    std::string   m_strDevId;

};

NAMESPACE_BP_END   // namespace end
#endif //MK_TRANSID_H