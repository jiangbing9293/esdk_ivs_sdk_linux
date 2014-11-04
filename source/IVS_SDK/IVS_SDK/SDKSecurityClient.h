/********************************************************************
 filename    :    SDKSecurityClient.h
 author      :    x00192614
 created     :    2013/01/11
 description :    SDK��Ϣ��ȫ�ͻ���
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/11 ��ʼ�汾
*********************************************************************/
#ifndef __SDKSECURITY_CLIENT_H__
#define __SDKSECURITY_CLIENT_H__

#include "hwsdk.h"
#include "bp_security_client.h"


class CUserMgr;
class CSDKSecurityClient
{
public:
    CSDKSecurityClient(void);
    ~CSDKSecurityClient(void);
    
    /******************************************************************
     function   : Init
     description: ��ʼ���������Ϣ��ȫ�ͻ��˶����ʼ������Կ����
     input      : int iNetElemType ��Ԫ����
     input      : std::string & strLinkID ���ӱ�ʶ
     output     : NA
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 Init(int iNetElemType, const std::string& strLinkID , 
        const char * pszCameraCode = "", 
        const char * pszDomainCode = "", 
        const char * pszDestModule = "",
        const char * pszDeviceCode = "",
		const char * pszNVRCode = "",
		enum BP::DOMAIN_TYPE enDomainType = BP::DOMAIN_NONE);

    /******************************************************************
     function   : EncryptString
     description:  ��������
     input      : const char * pszInBuff ��������
     output      : std::string & strCipher ���ķ���
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 EncryptString(const char* pszInBuff, std::string& strCipher) const;

    /******************************************************************
     function   : DecryptString
     description: ��������
     input      : const char * pszInBuff ��������
     output      : std::string & strPlain ���ķ���
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 DecryptString(const char* pszInBuff, std::string& strPlain) const;

    /******************************************************************
     function   : GetSecurityDataTransID
     description: ��ȡ��Կ��������õ�����ID
     return     : const std::string& ����ID
    *******************************************************************/
    const std::string& GetSecurityDataTransID() const;

    /******************************************************************
     function   : MultiDomainExchangeKey
     description: �༶�����µ���Կ����
     input      : int iNetElemType
     input      : CUserMgr * pUserMgr
     input      : std::string & strDomainCode
     output     : std::string& strLinkID
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 MultiDomainExchangeKey(int iNetElemType, CUserMgr* pUserMgr, const std::string& strDomainCode, 
        std::string& strLinkID, const char* pszDestModule = "", const char* pszDeviceCode = "", const char* pszNVRCode = "", enum BP::DOMAIN_TYPE enDomainType = BP::DOMAIN_NONE);
private:
    BP::CSecurityClient m_oSecurityClient; // ��Ϣ��ȫ�ͻ��˶���
};
#endif


