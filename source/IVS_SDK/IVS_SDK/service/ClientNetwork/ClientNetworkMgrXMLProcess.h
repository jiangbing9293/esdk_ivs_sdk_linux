/********************************************************************
filename : ClientNetworkMgrXMLProcess.h
author : pkf57481
created : 2013/01/22
description : �ͻ�������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/22 ��ʼ�汾
*********************************************************************/


#ifndef __CLIENTNETWORK_MGR_XML_PROCESS_H__
#define __CLIENTNETWORK_MGR_XML_PROCESS_H__

#include "ToolsHelp.h"
#include "XmlProcess.h"
#include <string.h>
#include "QueryFiled.h"
#include "hwsdk.h"

class CClientNetworkMgrXMLProcess
{
private:
    CClientNetworkMgrXMLProcess();
public:
    ~CClientNetworkMgrXMLProcess();

public:
	/***********************************************************************************
    * name       : GetCPUMemoryGetXML
    * description: �ͻ��˲�ѯCPU/�ڴ�����ƴ��xml
    * input      : pNodeCode:��Ԫ����; xml:����XML
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	static IVS_INT32 NodeCodeGetXML(const IVS_CHAR* pNodeCode, CXml& xml);

};

#endif

