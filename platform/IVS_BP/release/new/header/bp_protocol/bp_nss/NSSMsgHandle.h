/********************************************************************
filename    :    NSSMsgHandle.h
author      :    w90004666
created     :    2012/9/20
description :	 NSSЭ��ص�������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef NSS_MSG_HANDLE_H
#define NSS_MSG_HANDLE_H

#include "bp_namespace.h"
#include "IMsgHandle.h"

NAMESPACE_BP_BEGIN

using namespace std;

class CNSSMsgHandle: public IMsgHandle
{
private:
    CNSSMsgHandle(const CNSSMsgHandle& obj);
    CNSSMsgHandle& operator=(const CNSSMsgHandle& obj);
public:
    CNSSMsgHandle(void);
    virtual ~CNSSMsgHandle(void);
    
    // �������ݰ�
	virtual void HandlePkg(const string& strTransId, int netElementType, const std::string& strLinkId, const char* pPkgData, unsigned int uiDataLen);
};

NAMESPACE_BP_END
#endif //NSS_MSG_HANDLE_H

