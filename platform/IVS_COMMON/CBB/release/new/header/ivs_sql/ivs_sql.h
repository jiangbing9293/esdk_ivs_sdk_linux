/********************************************************************
filename    :    ivs_sql.h
author      :    yWX150774
created     :    2013/03/07
description :    SQL�﷨������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/07 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SQL_H
#define CBB_SQL_H


#include "ivs_uncopy.hpp"

#include <string>
#include <map>

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * SQL�﷨������, ��һ�����ɿ�������
 *
 ************************************************************/
class EXPORT_DLL CSQLExporter : private CUnCopy
{
public:
    CSQLExporter();
    ~CSQLExporter();

public:
    /*********************************************************
     * ��ʼ������, �����ظ�����
     ********************************************************/
    bool Init(const char * pszConfigFileName);

    /*********************************************************
     * ��ȡSQL���ʽ, �ɹ����� IVS_SUCCEED
     ********************************************************/
    int GetSQL(const char * pszKey, char * pszSQLBuffer, 
        int iSQLBufferSize, va_list vArgs) const;

    /*********************************************************
     * ��ȡSQL���ʽ, �ɹ����� IVS_SUCCEED
     ********************************************************/
    int GetSQL(const char * pszKey, 
        char * pszSQLBuffer, int iSQLBufferSize, ...) const;

    /*********************************************************
     * ������, �Ǳ�����ýӿ�
     ********************************************************/
    void Clean();

private:
    bool                                m_bInit;            // �Ƿ��Ѿ��ɹ���ʼ��
    std::map<std::string, std::string>  m_mapSQLExpression; // ����SQL���ʽ��ӳ���
};

NAMESPACE_CBB_END


#endif // CBB_SQL_H
