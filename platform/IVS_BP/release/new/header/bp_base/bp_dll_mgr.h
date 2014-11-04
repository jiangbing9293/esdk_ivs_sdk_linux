/********************************************************************
filename    :    bp_dll_mgr.h
author      :    w90004666
created     :    2012/9/20
description :	 ���ض�̬�������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_DLL_MGR_H
#define BP_DLL_MGR_H

#include <map>
#include "bp_namespace.h"
#include "bp_dll.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN // namespace begin


typedef std::map<std::string, CDllLoad*> CDllMap; 

class EXPORT_DLL CDllLoadMgr
{
public:
	virtual ~CDllLoadMgr() throw();

	/**************************************************************************
	 * name       : Instance
	 * description: ��������
	 * input      : void
	 * output     : NA
	 * return     : CDllLoadMgr&
	 * remark     :               
	 **************************************************************************/
	static CDllLoadMgr& Instance(void);

	/**************************************************************************
	 * name       : LoadDll
	 * description: ���ض�̬�⣬���������
	 * input      : const std::string & strName����̬�����ƣ�������չ��
	 * input      : bool bService���Ƿ���Service��
	 * output     : 
	 * return     : int
	 * remark     : ��ȡ���Ķ�̬�������Ҫ�ⲿ�ͷţ���Mgrͳһ�ͷ�              
	 **************************************************************************/
	int LoadDll(const std::string& strName, bool bService = true);

	/**************************************************************************
	 * name       : GetFunctionPtr
	 * description: ��ȡ����ָ��
	 * input      : const std::string & strDllName
	 * input      : const std::string & strFunctionName
	 * output     : NA
	 * return     : void*
	 * remark     :               
	 **************************************************************************/
	void* GetFunctionPtr(const std::string& strDllName, const std::string& strFunctionName);

	/**************************************************************************
	 * name       : UnloadAllDlls
	 * description: ж������dll
	 * input      : NA
	 * output     : NA
	 * return     : 
	 * remark     :               
	 **************************************************************************/
	void UnloadAllDlls();

private:
	CDllLoadMgr(void);

	//ͨ����̬�����ƻ�ȡ��Ӧ��̬��
	CDllLoad* GetDll(const std::string& strName);

private:
	static CDllLoadMgr m_Instance;

	CDllMap m_mapDll;
};

NAMESPACE_BP_END   // namespace end
#endif //BP_DLL_MGR_H
