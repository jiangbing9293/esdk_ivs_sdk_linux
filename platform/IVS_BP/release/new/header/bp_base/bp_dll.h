/********************************************************************
filename    :    bp_dll.h
author      :    w90004666
created     :    2012/9/20
description :	 ���ض�̬����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef BP_DLL_H
#define BP_DLL_H

#include "ace/DLL.h"

NAMESPACE_BP_BEGIN // namespace begin


class CDllLoad
{
public:
	CDllLoad(const std::string& strName, bool bService);
	virtual ~CDllLoad() throw();

	/**************************************************************************
	 * name       : Load
	 * description: ����ƽ̨�����ض�̬��
	 * output     : NA
	 * return     : int
	 * remark     :               
	 **************************************************************************/
	int Load(void);

	/**************************************************************************
	 * name       : GetFunctionPtr
	 * description: �Ӷ�̬���л�ȡ����ָ��
	 * input      : const std::string & strFunctionName
	 * output     : NA
	 * return     : void*
	 * remark     :               
	 **************************************************************************/
	void* GetFunctionPtr(const std::string& strFunctionName);

private:
	CDllLoad();

	void Init(void);


private:
	bool m_bService;		//�Ƿ���Serivce�����
	std::string m_strName;	//��̬�����ƣ���������չ��
	std::string m_strPath;	//��̬��ȫ·����������չ��

	ACE_DLL m_DLL;			//��̬�����

	bool m_bOpenFlag;		//��̬���Ƿ��Ѽ���
};

NAMESPACE_BP_END   // namespace end
#endif //BP_DLL_H
