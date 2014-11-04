/********************************************************************
 filename    :    bp_memoutput.h
 author      :    cwx148380
 created     :    2012/11/23
 description :    ��־��Ϣ������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/23 ��ʼ�汾
*********************************************************************/
#ifndef BP_MEMOUTPUT_H
#define BP_MEMOUTPUT_H

#include "bp_namespace.h"
#include "bp_stdint.h"
#include "bp_logoutput.h"
#include <string>
#include <list>
#include "bp_mutex.h"


NAMESPACE_BP_BEGIN //lint !e129 !e10

#define MEM_OPER_ACCOUNT_LENGTH          (32)
#define MEM_OPER_IP_LENGTH               (15)
#define MEM_OPER_SERVICECODE_LENGTH      (24)
#define MEM_OPER_MODULE_LENGTH           (24)
#define MEM_OPER_MODULE_TYPE_LENGTH      (24)
#define MEM_OPER_NODE_CODE_LENGTH        (32)
#define MEM_OPER_OPERATION_CODE_LENGTH   (6)
#define MEM_OPER_OBJECT_LENGTH           (1024)
#define MEM_OPER_OCCURTIME_LENGTH        (17)
#define MEM_OPER_ERRCODE_LENGTH          (9)
#define MEM_OPER_RESULT_LENGTH           (10)
#define MEM_OPER_ADDINFO_LENGTH          (1024)


#define MEM_SEC_ACCOUNT_LENGTH           (32)
#define MEM_SEC_IP_LENGTH                (15)
#define MEM_SEC_SERVICECODE_LENGTH       (24)
#define MEM_SEC_MODULE_LENGTH            (24)
#define MEM_SEC_MODULE_TYPE_LENGTH       (24)
#define MEM_SEC_NODE_CODE_LENGTH         (32)
#define MEM_SEC_OPERATION_CODE_LENGTH    (6)
#define MEM_SEC_OBJECT_LENGTH            (1024)
#define MEM_SEC_OCCURTIME_LENGTH         (17)
#define MEM_SEC_ERRCODE_LENGTH           (9)
#define MEM_SEC_RESULT_LENGTH            (10)
#define MEM_SEC_ADDINFO_LENGTH           (1024)

#pragma  pack(push)
#pragma  pack(1)

//������־���ݽṹ
typedef struct StOperationLog
{
    char szUser[MEM_OPER_ACCOUNT_LENGTH + 1];                     //�û�
    char szIP[MEM_OPER_IP_LENGTH + 1];                            //IP��ַ
    int32_t iLevel;                                           //��־�ȼ�
    char szServiceCode[MEM_OPER_SERVICECODE_LENGTH + 1];          //ҵ����룬"IVS"
    char szModuleType[MEM_OPER_MODULE_TYPE_LENGTH + 1];           //OMU��SMU��SCU
    char szNodeCode[MEM_OPER_NODE_CODE_LENGTH + 1];
    char szOperationCode[MEM_OPER_OPERATION_CODE_LENGTH + 1];     //������
    char szObject[MEM_OPER_OBJECT_LENGTH + 1];                    //��������
    char szOccurTime[MEM_OPER_OCCURTIME_LENGTH + 1];              //����ʱ��
    long iUTCOffset;
    char szErrCode[MEM_OPER_ERRCODE_LENGTH + 1];                  //������
    char szResult[MEM_OPER_RESULT_LENGTH + 1];                    //�������
    char szAdditionalInfo[MEM_OPER_ADDINFO_LENGTH + 1];           //������Ϣ
}ST_OPERATION_LOG;

//��ȫ��־���ݽṹ
typedef struct StSecurityLog
{
    char szUser[MEM_SEC_ACCOUNT_LENGTH + 1];                  //�û�
    char szIP[MEM_SEC_IP_LENGTH + 1];                         //IP��ַ
    int32_t iLevel;                                           //��־�ȼ�
    char szServiceCode[MEM_SEC_SERVICECODE_LENGTH + 1];       //ҵ����룬"IVS"
    char szModuleType[MEM_SEC_MODULE_TYPE_LENGTH + 1];        //OMU��SMU��SCU
    char szNodeCode[MEM_SEC_NODE_CODE_LENGTH + 1];
    char szOperationCode[MEM_SEC_OPERATION_CODE_LENGTH + 1];  //������
    char szObject[MEM_OPER_OBJECT_LENGTH + 1];               //��������
    char szOccurTime[MEM_SEC_OCCURTIME_LENGTH + 1];          //����ʱ��
    long iUTCOffset;
    char szErrCode[MEM_SEC_ERRCODE_LENGTH + 1];              //������
    char szResult[MEM_SEC_RESULT_LENGTH + 1];                //�������
    char szAdditionalInfo[MEM_SEC_ADDINFO_LENGTH + 1];       //������Ϣ
}ST_SECURITY_LOG;
#pragma pack(pop)


class CMemOutput : public ILogOutput
{
public:
    CMemOutput();
    virtual ~CMemOutput();

   /**********************************************************************
   * name           : OperationOutput
   * description    : ������־�������
   * input          : @pUser:           �û�
                      @pIp:             IP��ַ
                      @iLevel:          ��־�ȼ�
                      @pServiceCode:    ҵ�����
                      @pModuleType:     ģ������
                      @pNodeCode:     ģ����
                      @pOpcode:         ������
                      @pOperation:      ������Ϣ
                      @pObject:         ��������
                      @pOccurTime:      ��־����ʱ��
                      @iUTCOffset:      UTCʱ��ƫ����
                      @pResult:         ���
                      @pErrCode:        ������
                      @pAdditionalInfo: ������Ϣ
   * return         : �ɹ������ص�ǰ������־������
                      ʧ�ܣ�����LOG_OUTPUT_FAIL
   * remark         : NA
   *********************************************************************/
    virtual int OperationOutput ( const char *pUser, const char* pIp,
                                  const int iLevel,
                                  const char *pServiceCode, const char *pModuleType, const char* pNodeCode,
                                  const char *pOpcode, /*const char *pOperation,*/ const char *pObject,
                                  const char *pOccurTime, const long iUTCOffset,
                                  const char *pResult, const char *pErrCode, const char *pAdditionalInfo
                                );

   /**********************************************************************
   * name           : SecurityOutput
   * description    : ��ȫ��־�������
   * input          : @pUser:           �û�
                      @pIp:             IP��ַ
                      @iLevel:          ��־�ȼ�
                      @pServiceCode:    ҵ�����
                      @pModuleType:     ģ������
                      @pNodeCode:     ģ����
                      @pOpcode:         ������
                      @pOperation:      ������Ϣ
                      @pOccurTime:      ��־����ʱ��
                      @iUTCOffset:      UTCʱ��ƫ����
                      @pResult:         ���
                      @pErrCode:        ������
                      @pAdditionalInfo: ������Ϣ
   * return         : �ɹ�: ���ص�ǰ������־������
                      ʧ�ܣ�����LOG_OUTPUT_FAIL
   * remark         : NA
   *********************************************************************/
    virtual int SecurityOutput ( const char *pUser, const char *pIp,
                                 const int iLevel,
                                 const char* pServiceCode,
                                 const char* pModuleType, const char* pNodeCode,
                                 const char *pOpcode, /* const char* pOperation,*/
                                 const char* pOccurTime, const long iUTCOffset,
                                 const char* pResult, const char* pErrCode, const char *pAdditionalInfo
                               );

    /**********************************************************************
    * name           : GetOperation
    * description    : ȡ��һ��������־����
    * input          : NA
    * output         : ������־�ṹ��
    * return         : BP_OK - �ɹ� B_FAIL - ʧ��
    * remark         : NA
    *********************************************************************/
    virtual int GetOperation ( ST_OPERATION_LOG& stOperLog );

    /**********************************************************************
    * name           : GetSecurity
    * description    : ȡ��һ����ȫ��־����
    * input          : NA
    * output         : ������־�ṹ��
    * return         : BP_OK - �ɹ� B_FAIL - ʧ��
    * remark         : NA
    *********************************************************************/
    virtual int GetSecurity ( ST_SECURITY_LOG& stSecLog );

private:
    BP_Thread_Mutex m_mutexOperLog;             //������־�߳���
    std::list<ST_OPERATION_LOG> m_lstOperLog;   //������־����

    BP_Thread_Mutex m_mutexSecLog;              //��ȫ��־�߳���
    std::list<ST_SECURITY_LOG> m_lstSecLog;     //��ȫ��־����
};

NAMESPACE_BP_END

#endif //bp_memoutput.h

