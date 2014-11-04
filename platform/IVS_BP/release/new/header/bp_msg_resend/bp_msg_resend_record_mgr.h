/********************************************************************
filename    :     bp_msg_resend_record_mgr.h
author      :     h00232170, yWX150774
created     :     2012/10/26
description :     ��Ϣ�ط��ļ�
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/10/26 ��ʼ�汾
*********************************************************************/

#ifndef BP_MSG_RESEND_RECORD_MGR_H
#define BP_MSG_RESEND_RECORD_MGR_H

#include "bp_namespace.h"
#include "bp_msg_resend_record.h"

#include "ace/Thread_Mutex.h"

NAMESPACE_BP_BEGIN

class CMsgResendRecordMgr
{
public:
    CMsgResendRecordMgr();
    ~CMsgResendRecordMgr();

public:
    bool AddRecord(MsgResendRecord & rMsgResendRecord);
    bool RemoveRecord(const MsgResendRecord & rMsgResendRecord);
    bool ModifyRecord(const MsgResendRecord & rMsgResendRecord);
    int GetRecord(MsgResendRecord * pMsgResendRecord, int iMaxRecordCount);

    int GetRecordCount();

private:
    int ApplySequence();
    bool GetNextRecord(MsgResendRecord & rMsgResendRecord);

private:
    CMsgResendRecordMgr(const CMsgResendRecordMgr &);
    CMsgResendRecordMgr & operator = (const CMsgResendRecordMgr &);

private:
    CMsgResendRecordMap            m_mapMsgResendInfo;     // �ط���Ϣ�ڴ滺��
    CMsgResendRecordMap::iterator  m_iterLastHandle;       // ��ǰ����ĵ�����GetNextRecord��ʹ��
    int                            m_iRecordCount;         // ��Ч��¼��
    int                            m_iMaxRecordSequence;   // ��ǰ�ļ�����¼��, ��������Ŀ��м�¼;
    CRecordSequenceList            m_listIdleSequence;     // ���м�¼������б�
    ACE_Thread_Mutex               m_aThreadMutex;         // ��������������̻߳�����
};

NAMESPACE_BP_END

#endif // BP_MSG_RESEND_RECORD_MGR_H
