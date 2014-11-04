/********************************************************************
    filename    :    bp_config_alarmer.h
    author      :    yWX150774
    created     :    2013/03/14
    description :    ���ø澯ʹ���ߵĻ���
    copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history     :    2013/03/14 ��ʼ�汾
*********************************************************************/

#ifndef BP_CONFIG_ALARMER_H
#define BP_CONFIG_ALARMER_H


#include "bp_def.h"
#include "bp_config_alarm.h"

#include <string>

class EXPORT_DLL CConfigAlarmer
{
protected:
    CConfigAlarmer();
    ~CConfigAlarmer();

public:
    void SetConfigCallBack(ConfigErrorCallBack pFuncCallBack);

protected:
    void SetConfigFile(const std::string & strConfigFile);

protected:
    void LoadFailAlarm();
    void FindFailAlarm(const char * pszItem, const char * pszDefault);
    void IntoFailAlarm(const char * pszItem);
    void InvalidValueAlarm(const char * pszItem, const char * pszInvalid, const char * pszDefault);

private:
    CConfigAlarmer(const CConfigAlarmer &);
    CConfigAlarmer & operator = (const CConfigAlarmer &);

private:
    CConfigAlarm           m_aConfigAlarm;  // ���ø澯����
    std::string            m_strConfigFile; // �����ļ���
};


#endif // BP_CONFIG_ALARMER_H
