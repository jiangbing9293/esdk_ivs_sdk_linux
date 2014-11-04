/********************************************************************
filename    :    bp_load_db_connection_config.h
author      :    cwx148380
created     :    2012/11/08
description :    ��ȡ���ݿ�����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/08 ��ʼ�汾
*********************************************************************/
#ifndef BP_LOAD_DB_CONNETION_CONFIG_H
#define BP_LOAD_DB_CONNETION_CONFIG_H

#include "ivs_xml.h"
#include "bp_db_config.h"
#include "bp_config_alarmer.h"
#include <string>

NAMESPACE_BP_BEGIN //lint !e129 !e10

class CLoadDatabaseConnectionConfig : private CConfigAlarmer
{
public:
    CLoadDatabaseConnectionConfig ( void );
    virtual ~CLoadDatabaseConnectionConfig (void );

    //��ȡXML�����ļ�
    int LoadConfigFile(const std::string& strConfigPath);
    const CDatabaseConnectionConfig& GetDatabaseConnetionConfig ( void ) const;

public:
    //��ȡDatabase�ڵ�������Ϣ
    int LoadDatabaseNodeConfig( const std::string& strConfigPath );

    bool GetDatabaseTypeElement ( std::string & strDatabaseType );

    bool GetDatabaseNameElement ( std::string & strDatabaseName );

    bool GetServiceIpElement ( std::string & strServiceIp );

    bool GetServicePortElement ( std::string & strServicePort );

    bool GetUserNameElement ( std::string & strUserName );

    bool GetUserPasswordElement ( std::string & strUserPassword );

    int  GetDatabasePoolSizeElement ( void );

private:
    CBB::CXml                   m_DbConnectionXml;    // �����ļ�����
    CDatabaseConnectionConfig   m_DbConnectionConfig; // ���ݿ����Ӳ���ʵ��
};

NAMESPACE_BP_END   //

#endif //BP_LOAD_DB_CONNETION_CONFIG_H
