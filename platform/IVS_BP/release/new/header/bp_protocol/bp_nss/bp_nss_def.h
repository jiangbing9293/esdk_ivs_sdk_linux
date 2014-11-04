/********************************************************************
filename    :    bp_nss_def.h
author      :    h00232170
created     :    2012/9/19
description :	 NSSЭ��ͷ�ļ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/19 ��ʼ�汾
*********************************************************************/
#ifndef BP_NSSP_DEF_H
#define BP_NSSP_DEF_H

#include "nss_mt.h"
#include "bp_def.h"
#include "bp_stdint.h"

// �Ƿ��������Ŀ���
#define _CHECK_HEARTBEAT_

// ��¼״̬
typedef enum enNSSP_STAT_FIAG
{
    STAT_UNDEF,
	LOGIN_INIT,
	LOGIN_ING,
	LOGIN_ERROR,
	LOGIN_OK,
	LOGIN_TIMEOUT,
	HEARTBEAT_ING,
	HEARTBEAT_OK,
	HEARTBEAT_TIMEOUT
}NSSP_STAT_FIAG;


//TODO���Ժ�ŵ���һ��ר����Ϣ����ͷ�ļ���
#define NSS_MSG_PROTOCOL_VERSION   0x0100  // NSS��Ϣ�汾��


//////////////// ͨ����Ϣ�ṹ�嶨�� /////////////////////////
/*
 * ��Ϣͷ�ṹ
 */
//����Ϊ���ֽڶ���
#pragma pack(push)
#pragma pack(1)

typedef struct NssMsgHeader
{
	uint16_t usiProtocolVersion;             // Э��汾��Ϣ����ǰ�汾��Ϊ0x0100 
	uint16_t usiMsgType;                     // ��Ϣ����,�μ� NSS_MSG_TYPE DELETE_SPACE_TYPE
	uint32_t uiSeqID;                        // ��Ϣ��ţ�Ҫ��Ӧ����Ϣ��������Ϣһ��
	uint32_t uiTraceID;                      // ��Ϣ����ID
	char szTransactionNo[TRANSACTIONNO_LEN]; // ������
	uint32_t uiRspCode;                      // ��Ӧ�롣0���ɹ������������������
	char szReserve[RESERVE_LEN];             // Ԥ���ֶ� 
	uint32_t uiPacketLength;                 // ��Ϣ�峤�ȣ��ó��Ȱ�����Ϣͷ���ȣ�
}TNssMsgHeader;

const int NssMsgTypePos = sizeof(uint16_t);
const int NssTransactionNoPos = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(uint32_t) + sizeof(uint32_t);
const int NssRspCodePos = NssTransactionNoPos + TRANSACTIONNO_LEN;
const int NssPacketLengthPos = NssRspCodePos + sizeof(uint32_t) + RESERVE_LEN;


//ע��������Ϣ
typedef struct _NSS_REGISTER_REQ_MSG
{
	TNssMsgHeader Header;
	char           ClientDeviceCode[DEVICE_CODE_LEN];
}NSS_REGISTER_REQ_MSG, *PNSS_REGISTER_REQ_MSG;

//ע����Ӧ��Ϣ
typedef struct _NSS_REGISTER_RESP_MSG
{
	TNssMsgHeader Header;
	char           ServerDeviceCode[DEVICE_CODE_LEN];
}NSS_REGISTER_RESP_MSG, *PNSS_REGISTER_RESP_MSG;

//����������Ϣ
typedef struct _NSS_KEEPALIVE_REQ_MSG
{
	TNssMsgHeader Header;
}NSS_KEEPALIVE_REQ_MSG, *PNSS_KEEPALIVE_REQ_MSG;

typedef struct _NSS_TEST_REQ_MSG
{
	TNssMsgHeader Header;
}NSS_TEST_REQ_MSG, *PNSS_TEST_REQ_MSG;

typedef struct _NSS_TEST_RESP_MSG
{
	TNssMsgHeader Header;
}NSS_TEST_RESP_MSG, *PNSS_TEST_RESP_MSG;

#pragma pack(pop)

#endif
