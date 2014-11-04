/********************************************************************
filename    :     nss_msg_header.h
author      :     h00232170
created     :     2012/10/24
description :     nss ��Ϣͷ����
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/24 ��ʼ�汾
*********************************************************************/

#ifndef NSS_MSG_HEADER_H_
#define NSS_MSG_HEADER_H_

#include <ace_header.h>
#include "bp_stdint.h"
#include "bp_def.h"
#include "bp_nss_def.h"

const int MaxTransactionNoLength = 64;
const int MaxNssReserveFieldLength = 32;

const unsigned int NssRet_Succeed = 0;
const unsigned int NssRet_InnerErr = 999;
const unsigned int NssRet_MsgLengthInvalid = 1000;
const unsigned int NssRet_MsgFieldInvalid = 1001;
const unsigned int NssRet_TagUnexpected = 1002;

USING_NAMESPACE_BP

//const int NssHeaderLength = sizeof(TNssMsgHeader);

class EXPORT_DLL CNssMsgBase
{
public:
	CNssMsgBase(void);
	CNssMsgBase(uint16_t usiMsgType, uint32_t uiSeqID, const char* szTransactionNo,uint32_t uiRspCode, uint32_t uiTraceID = 0);
    virtual ~CNssMsgBase(void);

	static bool DecodeMsgType(const char* pszMsg, int iMsgLength, uint16_t& usiMsgType);
	static bool DecodeTransactionNo(const char* pszMsg, int iMsgLength, char* pszTransactionNo);
	static bool DecodeRspCode(const char* pszMsg, int iMsgLength, uint32_t& uiRspCode);
	static bool DecodePacketLength(const char* pszMsg, int iMsgLength, uint32_t& uiPacketLength);

	// ����Ϣͷ������m_header�⹹�У��������������ֽ���ת����
	uint32_t decode(ACE_Message_Block* pMsgBlock);
	virtual uint32_t decodeStream(const char* pszMsg, int iMsgLength);
	// ��m_header�⹹�е��ֶΣ��ൽpMsgBlock�У�pMsgBlock �����GetMsgLength����ͳһ�����ڴ�,
	// �ⲿֻ��Ҫתһ��ACE_Message_Block����ָ�룬������������еĻ��棻
	virtual uint32_t encode(ACE_Message_Block* pMsgBlock);
	// ������Ϣ���ȣ�������Ҫ����ʵ�ָ÷������������Ϣ��ʵ�ʳ���;
	virtual uint32_t GetMsgLength(){return sizeof(TNssMsgHeader);}

	// ��ȡ��Ϣ����׵�ַ
	char* GetContentBuff(ACE_Message_Block* pMsgBlock);
	// ��ȡ��Ϣ�峤��;
	int GetContentLength(ACE_Message_Block* pMsgBlock);

	TNssMsgHeader& GetHeader(){return m_header;}

private:
	TNssMsgHeader m_header;
};


#endif /* OMU_GET_NODE_LIST_SESSION_H_ */
