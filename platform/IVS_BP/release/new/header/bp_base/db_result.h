#ifndef _db_result_h
#define _db_result_h

#include "db_parameter.h"

class DLLEXPORT  IDBResultSet
{
public:
    IDBResultSet(){};
    virtual ~IDBResultSet(){};

public:
    /**********************************************************************
    * name           : Close
    * description    : �ر����ݿ���
    * input          : NA
    * return         : NA
    * remark         : NA
    *********************************************************************/
    virtual void Close(void) = 0;

    /**********************************************************************
    * name           : HasNext
    * description    : �Ƿ���������β
    * input          : NA
    * return         : bool
    * remark         : NA
    *********************************************************************/
    virtual bool HasNext(void) = 0;

    /**********************************************************************
    * name           : GetInt
    * description    : ���������һ��intֵ
    * input          : NA
    * return         : int
    * remark         : NA
    *********************************************************************/
    virtual int GetInt(void) = 0;

    /**********************************************************************
    * name           : GetFloat
    * description    : ���������һ��floatֵ
    * input          : NA
    * return         : float
    * remark         : NA
    *********************************************************************/
    virtual float GetFloat(void) = 0;

    /**********************************************************************
    * name           : GetBignum
    * description    : ���������һ��doubleֵ
    * input          : NA
    * return         : double
    * remark         : NA
    *********************************************************************/
    virtual double GetBignum(void) = 0;

    /**********************************************************************
    * name           : GetString
    * description    : ���������һ��stringֵ
    * input          : NA
    * return         : string
    * remark         : NA
    *********************************************************************/
    virtual std::string GetString(void) = 0;
    
    /**********************************************************************
    * name           : GetLob
    * description    : ���������һ��lobֵ
    * input          : NA
    * return         : string
    * remark         : NA
    *********************************************************************/
    virtual std::string GetLob(void) = 0;

    /**********************************************************************
    * remark         : ���ݽӿڣ�����MAUʹ�á�����ģ����ʹ��Get***
    **********************************************************************/
    int AsInt(void){return GetInt();}
    std::string AsString(void){return GetString();}
    std::string AsLongString(void){return GetLob();}
};

#endif
