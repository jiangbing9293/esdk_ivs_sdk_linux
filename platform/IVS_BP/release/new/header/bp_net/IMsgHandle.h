/********************************************************************
filename    :    IMsgHandle.h
author      :    j00213308
created     :    2012/04/06
description :    Э��ջ��Ϣ����ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/06 ��ʼ�汾
*********************************************************************/
#ifndef IMSG_HANDLE_H
#define IMSG_HANDLE_H

#include "bp_def.h"
#include "ace_header.h"

NAMESPACE_BP_BEGIN

// ǰ������
class CCmd;

// Э��ջ��Ϣ����ӿ�
class IMsgHandle
{
public:
    IMsgHandle(void){};
    virtual ~IMsgHandle(void){};

    /**************************************************************************
    * name       : HandlePkg
    * description: �������ݰ�
    * input      : strTransId     : ����ID
    *              netElementType : ���յ����ݵ���Ԫ����
    *              strLinkId         : ���յ���������Id
    *              pPkgData       : ָ������ָ��
    *               uiDataLen     : ���ݴ�С
    * output     : NA
    * return     : NA
    * remark     : ��Ϣ�����߱����Լ������ڴ汣�����ݰ�
    **************************************************************************/
    virtual void HandlePkg(const string& strTransId, int netElementType, const std::string& strLinkId, const char* pPkgData, unsigned int uiDataLen) = 0;

	virtual bool OnLoginServer(const string& /*strTransId*/, int /*netElementType*/, 
	    const std::string& /*strLinkId*/, const char* /*pPkgData*/, unsigned int /*uiDataLen*/)
	{
		return true;
	}
};

NAMESPACE_BP_END

#endif    // IMSG_HANDLE_H
