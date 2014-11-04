/********************************************************************
filename    :    bp_logoutput.h
author      :    cwx148380
created     :    2012/11/22
description :    ��־��Ϣ������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/22 ��ʼ�汾
*********************************************************************/

#ifndef BP_LOG_OUTPUT_H
#define BP_LOG_OUTPUT_H

#ifdef WIN32
# ifdef BP_LOG_EXPORTS
#  define EXPORT_BP_LOG_API __declspec(dllexport)
# else
#  define EXPORT_BP_LOG_API __declspec(dllimport)
# endif
#else
#define EXPORT_BP_LOG_API
#endif

#define LOG_OUTPUT_FAIL (-1)
//lint -e526 -e628
class EXPORT_BP_LOG_API ILogOutput //lint !e129
{//lint !e129 !e10
public:
    ILogOutput() {};
    virtual ~ILogOutput() {};

   /**********************************************************************
   * name           : OperationOutput
   * description    : ������־�������
   * input          : @pUser:           �û�
                      @pIp:             IP��ַ
                      @iLevel:          ��־�ȼ�
                      @pServiceCode:    ҵ�����
                      @pModuleType:     ģ������
                      @pNodeCode:       ������
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
                                ) = 0;

   /**********************************************************************
   * name           : SecurityOutput
   * description    : ��ȫ��־�������
   * input          : @pUser:           �û�
                      @pIp:             IP��ַ
                      @iLevel:          ��־�ȼ�
                      @pServiceCode:    ҵ�����
                      @pModuleType:     ģ������
                      @pNodeCode:       ������
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
                               ) = 0;
};
//lint +e526 +e628
#endif //bp_logoutput.h
