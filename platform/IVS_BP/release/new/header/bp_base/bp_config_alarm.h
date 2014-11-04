/********************************************************************
    filename    :    bp_config_alarm.h
    author      :    yWX150774
    created     :    2013/03/11
    description :    BP�ڲ����ø澯
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/11 ��ʼ�汾
*********************************************************************/

#ifndef BP_CONFIG_ALARM_H
#define BP_CONFIG_ALARM_H


#include "bp_def.h"

typedef void (*ConfigErrorCallBack)(int, const char *, const char *, const char *, const char *);

const unsigned int CONFIG_ALARM_PATH_NAME_LENGTH = 256;
const unsigned int CONFIG_ALARM_FILE_NAME_LENGTH = 32;
const unsigned int CONFIG_ALARM_ITEM_NAME_LENGTH = 128;
const unsigned int CONFIG_ALARM_INVALID_VALUE_LENGTH = 64;
const unsigned int CONFIG_ALARM_DEFAULT_VALUE_LENGTH = 64;

typedef struct ConfigAlarmInfo
{
    uint32_t      uiErrorCode;                                       // ������
    char          szPath[CONFIG_ALARM_PATH_NAME_LENGTH];             // �����ļ���
    char          szFile[CONFIG_ALARM_FILE_NAME_LENGTH];             // �����ļ���
    char          szItem[CONFIG_ALARM_ITEM_NAME_LENGTH];             // ����������
    char          szInvalidValue[CONFIG_ALARM_INVALID_VALUE_LENGTH]; // ���������ֵ
    char          szDefaultValue[CONFIG_ALARM_DEFAULT_VALUE_LENGTH]; // ������Ĭ��ֵ
} CONFIG_ALARM_INFO;

enum
{
    CONFIG_FAIL_ALARM_FOR_CANNOT_FIND_FILE = 700, // �Ҳ��������ļ�
    CONFIG_FAIL_ALARM_FOR_INVALID_VALUE           // �ڵ��ֵ��Ч
};

class EXPORT_DLL CConfigAlarm
{
public:
    CConfigAlarm();
    ~CConfigAlarm();

public:
    void SetConfigCallBack(ConfigErrorCallBack pFuncCallBack);

public:
    void Alarm(int iErrCode, const char * pszFile, const char * pszItem, 
               const char * pszInvalidValue, const char * pszDefaultValue) const;

private:
    CConfigAlarm(const CConfigAlarm &);
    CConfigAlarm & operator = (const CConfigAlarm &);

private:
    ConfigErrorCallBack m_pFuncCallBack;  // ���ݿ�����쳣�ص�����ָ��
};


#endif // BP_CONFIG_ALARM_H
