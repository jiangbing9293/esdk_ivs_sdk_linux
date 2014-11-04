/********************************************************************
filename    :    db_pgsql.h
author       :    z00193167
created      :    2012/04/16
description :    
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history       :    2012/04/16 ��ʼ�汾
*********************************************************************/
#ifndef BP_PGSQL_H
#define BP_PGSQL_H

#include "bp_def.h"
#include "DBInterface.h"
#include "db_exception_alarm.h"


// lob�����buffer��С
#define LobSize  (8*1024*1024)
// ��ʱʱ�䣬������ODBC����λΪ��
#define TimeOut  15
// ��־�洢·��
#define LOGPATH  "./"

class otl_connect;
class otl_nocommit_stream;

class CPgsqlResultSet : public IDBResultSet
{
public:
    CPgsqlResultSet();
    ~CPgsqlResultSet();

public:
    // �رս����
    void Close(void);

    // �Ƿ�������β
    bool HasNext(void);

    // ���������һ��int
    int GetInt(void);

    // ���������һ��float
    float GetFloat(void);

    // ���������һ��double
    double GetBignum(void);

    // ���������һ��string
    std::string GetString(void);

    // ���������һ��lob
    std::string GetLob(void);

private:
    //����sql�����������һ����
    void Open(int iBufferSize,const char *psql,otl_connect* pConnection, CDBParam* pParam, int iParamSize);

    // д��־
    static void WriteLog(const char* strLog);

protected:
    otl_nocommit_stream* m_pStream;

    friend class CPgsqlConnect;
};

class CPgsqlConnect : public IDBConnect
{
public:
    CPgsqlConnect();
    ~CPgsqlConnect();

public:
    // ��ʼ�����ݿ⻷����ֻ����һ��
    static void InitDBContext(DBExceptionCallBack pFuncExcptionCallBack = NULL);

    // �����û�������ͷ�����ʵ������һ������
    int Connect(const char* pszDBServer, const char* pszUser, const char* pszPasswd);

    // ���½������ӣ��������·�����Դ
    int ReConnect(const char* pszDBServer, const char* pszUser, const char* pszPasswd);

    IDBResultSet* Execute(const char *pSql, CDBParam* pParam, int iParamSize);

    int SEQNextVal(const char* seqName);

    int Commit(void);

    int Rollback(void);

    int DisConnect(void);

    bool IsConnected(void);

private:
    int ConnectAgain();
    std::string ExgChar(const char* pszStr, char rep) const;
	void CkeckIfNeedToReCreateConn();

protected:
    static DBExceptionCallBack s_pFuncExcptionCallBack;  // ���ݿ�����쳣�ص�����ָ��
    static CDBExceptionAlarm   s_aDBExceptionAlarm;      // ���ݿ�澯����

private:
    otl_connect* m_connet;
    CPgsqlResultSet m_resultset;
    bool m_bIsConn;
	bool m_bIsCommited;
    std::string m_strURL;	

	int m_iExceptionCount;                               // �쳣ͳ��
};

#endif //DB_PGSQL_H
