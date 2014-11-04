/********************************************************************
    filename    :    bp_alarm_storage.h
    author      :    yWX150774
    created     :    2013/03/23
    description :    BP�ڲ��澯�ֿ�, ����BP��δע�ᴦ����������֮ǰ�ĸ澯
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/23 ��ʼ�汾
*********************************************************************/

#ifndef BP_ALARM_STORAGE_H
#define BP_ALARM_STORAGE_H


#include "bp_def.h"
#include "Cmd.h"

#include "ace/Thread_Mutex.h"

#include <list>

class EXPORT_DLL CAlarmMsgStorage
{
public:
    static CAlarmMsgStorage & GetInstance();

public:
    void Init();
    void Release();

public:
    void StopStoreAlarmMsg();
    bool TryStoreAlarmMsg(BP::CCmd *& pCmd);
    void GetAlarmMsg(std::list<BP::CCmd *> & listAlarmMsg);
    void CleanAlarmMsg();

private:
    CAlarmMsgStorage();
    ~CAlarmMsgStorage();

private:
    CAlarmMsgStorage(const CAlarmMsgStorage &);
    CAlarmMsgStorage & operator = (const CAlarmMsgStorage &);

private:
    bool                    m_bNeedStoreAlarmMsg; // �Ƿ���Ҫ����澯��Ϣ
    std::list<BP::CCmd *>   m_listAlarmMsg;       // ����澯��Ϣ������
    ACE_Thread_Mutex        m_aThreadMutex;       // ��������������̻߳�����
};


#endif // BP_ALARM_STORAGE_H
