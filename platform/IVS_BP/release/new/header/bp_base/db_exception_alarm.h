/********************************************************************
    filename    :    db_exception_alarm.h
    author      :    yWX150774
    created     :    2013/03/09
    description :    ���ݿ��쳣�澯
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/09 ��ʼ�汾
*********************************************************************/

#ifndef DB_EXCEPTION_ALARM_H
#define DB_EXCEPTION_ALARM_H


#include "bp_def.h"
#include "DBInterface.h"

#include "ace/Thread_Mutex.h"

#include <string>

class EXPORT_DLL CDBExceptionAlarm
{
public:
    CDBExceptionAlarm();
    ~CDBExceptionAlarm();

public:
    void SetUser(const std::string & strUser);
    void SetServer(const std::string & strServer);
    void SetExceptionCallBack(DBExceptionCallBack pFuncCallBack);

public:
    void DBConnectFailAlarm();
    void DBConnectResumeAlarm();

    /**
     * ִ��SQL����쳣ʱ�澯�����ݿ������ΪiErrorCode��
     * bAlarmΪ��ʱ���Ǹ澯������iErrorCode�Ǻ�ֵ�����Ĳ���Ϊ������oracle, pgsql������, �ɸ������ݿ��ڲ��ж��Ƿ�澯��
     */
    void DBExceptionFailAlarm(int iErrorCode, bool bAlarm = false);
    void DBExceptionResumeAlarm();

private:
    enum enLastDBAlarmState  // ���ݿ�澯״̬
    {
        ALARM_STATE_UNKNOWN, // ���������, δ֪��һ�θ澯״̬
        ALARM_STATE_ALARMED, // ��һ��Ϊ�澯
        ALARM_STATE_RESUMED  // ��һ��Ϊ�ָ�
    };

private:
    DBExceptionCallBack m_pFuncCallBack;       // ���ݿ�����쳣�ص�����ָ��

    std::string m_strUser;                     // �û���
    std::string m_strServer;                   // ������
    enLastDBAlarmState m_enConnectAlarmState;  // �������ݿ�ĸ澯״̬
    enLastDBAlarmState m_enAccessAlarmState;   // �������ݿ�ĸ澯״̬

    ACE_Thread_Mutex m_aThreadMutex;           // ��������������̻߳�����
};


#endif // DB_EXCEPTION_ALARM_H
