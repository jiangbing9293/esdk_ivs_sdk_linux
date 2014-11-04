/********************************************************************
filename    :    CommuService.h
author      :    j00213308
created     :    2012/04/07
description :    ��ԪͨѶ������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/04/07 ��ʼ�汾
*********************************************************************/
#ifndef COMMU_SERVICE_H
#define COMMU_SERVICE_H

#include "IService.h"
#include "net_element.h"

NAMESPACE_BP_BEGIN

class CCmd;
class CNetElementMgr;

// ��ԪͨѶ������
class EXPORT_DLL CCommuService : public IService
{
public:
	CCommuService(void);
	virtual ~CCommuService(void);
    
	/**************************************************************************
    * name       : HandleCmd
    * description: ����������Ϣ
    * input      : pCmd : �账���������Ϣ
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	virtual int HandleCmd(CCmd* pCmd);
	    
	/**************************************************************************
    * name       : Routing
    * description: �г̴���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	virtual uint32_t Routing(void);

	/**************************************************************************
    * name       : SetElementMgr
    * description: ������Ԫ������
    * input      : pElementMgr:��Ԫ������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SetElementMgr(CNetElementMgr* pElementMgr);

	/**************************************************************************
    * name       : GetElement
    * description: ��ȡ��Ԫ
    * input      : iElementType:��Ԫ����
    * output     : NA
    * return     : INetElement*
    * remark     : NA
    **************************************************************************/
	INetElement* GetElement(int iElementType);

    /**************************************************************************
    * name       : AddNetElement()
    * description: �����Ԫ
    * input      : pNetElement --- ��Ԫ����
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
    int AddNetElement(INetElement *pNetElement);
private:
	CNetElementMgr* m_pElementMgr;
};

NAMESPACE_BP_END

#endif	// end of COMMU_SERVICE_H
