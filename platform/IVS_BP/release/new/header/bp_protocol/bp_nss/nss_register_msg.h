/********************************************************************
filename    :     nss_register_msg.h
author      :     h00232170
created     :     2012/10/24
description :     nss-xml ��Ϣ����
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/24 ��ʼ�汾
*********************************************************************/

#ifndef NSS_REGISTER_MSG_H
#define NSS_REGISTER_MSG_H

#include <ace_header.h>
#include "bp_stdint.h"
#include "bp_def.h"
#include "nss_msg_header.h"

/////////////////NSS  TAG ���� ///////////////////////////////
const uint16_t NssTag_XMLContent = 0x0001;


USING_NAMESPACE_BP

typedef struct NssXMLMsg
{
	uint16_t usiXMLTag;
	uint32_t uiXMLLength;
	char* pszXMLValue;
}TNssXMLMsg;

//const int NssHeaderLength = sizeof(TNssMsgHeader);

class EXPORT_DLL CNssRegisterMsg: public CNssMsgBase
{
public:
	CNssRegisterMsg(void);
        virtual ~CNssRegisterMsg(void);
	CNssRegisterMsg(uint16_t usiMsgType,uint32_t uiSeqID, const char* szTransactionNo,uint32_t uiRspCode, uint32_t uiTraceID = 0);

	// ����Ϣͷ������m_header�⹹�У��������������ֽ���ת����
	virtual uint32_t decodeStream(const char* pszMsg, int iMsgLength);
	// ��m_header�⹹�е��ֶΣ��ൽpMsgBlock�У�pMsgBlock �����GetMsgLength����ͳһ�����ڴ棻
	virtual uint32_t encode(ACE_Message_Block* pMsgBlock);
	// ������Ϣ���ȣ�������Ҫ����ʵ�ָ÷������������Ϣ��ʵ�ʳ���;
	virtual uint32_t GetMsgLength(){return (sizeof(TNssMsgHeader) + DEVICE_CODE_LEN);}

	void SetDeviceCode(const char* pszDeviceCode);
	const char* GetDeviceCode(){return m_szDeviceCode;}
private:
	char m_szDeviceCode[DEVICE_CODE_LEN + 1];
};


#endif // NSS_REGISTER_MSG_H
