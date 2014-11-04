/********************************************************************
filename    :    NSSLinkCreater.h
author      :    w90004666
created     :    2012/9/20
description :	 NSSЭ��ջ������ӿ��ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/20 ��ʼ�汾
*********************************************************************/
#ifndef NSS_LINK_CREATER_H
#define NSS_LINK_CREATER_H

#include "bp_namespace.h"
#include "ILinkCreater.h"
#include "NSSMsgHandle.h"
#include "bp_def.h"

	//��������
extern "C" EXPORT_DLL BP::ILinkCreater* CreateLinkCreater();


NAMESPACE_BP_BEGIN

class CNSSLinkCreater : public ILinkCreater
{
public:
    CNSSLinkCreater(void);
    virtual ~CNSSLinkCreater(void);

protected:
    // ������Ϣ������
    virtual IMsgHandle* BuildMsgHandle(void);

	virtual IProtocol* BuildProtocol(void);
};

NAMESPACE_BP_END
#endif  // NSS_LINK_CREATER_H
