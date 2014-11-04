/********************************************************************
filename    :     msg_resend_record.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     ��Ϣ�ظ���Ϣ
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 ��ʼ�汾
*********************************************************************/

#ifndef BP_MSG_RESEND_RECORD_H
#define BP_MSG_RESEND_RECORD_H

#include "bp_namespace.h"
#include "bp_def.h"

#include "ace_header.h"

#include <map>
#include <list>

NAMESPACE_BP_BEGIN

const int RESEND_BAD_SEQUENCE                        = -1;    // ��Ч�ı��

enum enMsgResendState
{
    RESEND_STATE_INIT,      // ��ʼ��
    RESEND_STATE_SAVE,      // ����
    RESEND_STATE_SENDED,    // �ѷ���
    RESEND_STATE_FAILURE,   // ʧ��
    RESEND_STATE_SUCCESS    // �ɹ�
};

struct MsgResendInfo
{
    MsgResendInfo();

    int                 iRecordSequence;                    // ����Ϣ�ط��ļ������;
    uint32_t            uiMsgType;                          // �ط�����Ϣ����;
    int                 iNetType;                           // �ط���Ŀ����Ԫ����;
    char                szLinkID[DEVICE_CODE_LEN];          // �ط���Ŀ����ԪID;
    char                szTransactionNo[TRANSACTIONNO_LEN]; // �ط���Ϣ��TransID;
    int                 iCurrentResendTimes;                // �Ѿ��ط�����;
    int                 iResendInterval;                    // �ط�ʱ����
    int                 iMaxResendTimes;                    // ����ط�����
    time_t              tLastSendMsgTime;                   // �ϴ��ط�ʱ��;
    enMsgResendState    enResendState;                      // �ط�״̬;
};

struct MsgResendRecord : public MsgResendInfo
{
    MsgResendRecord();

    bool HaveDataToSend() const;
    bool LastSendFailed() const;
    bool ResendOverload() const;
    bool ResendTimeCome(time_t tCurrentTime) const;

    ACE_Message_Block  * m_pMsgBlock;
};

typedef std::map<int, MsgResendRecord>  CMsgResendRecordMap;
typedef std::list<int>                  CRecordSequenceList;

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_RECORD_H
