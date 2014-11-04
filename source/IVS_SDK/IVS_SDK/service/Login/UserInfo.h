/********************************************************************
    filename	: 	UserInfo.cpp
    author		:	x00192614
    created		:	2011/11/1
    description	:	�û���Ϣ
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/5/20 ��ʼ�汾
 *********************************************************************/

#ifndef __USER_INFO_H__
#define __USER_INFO_H__

#include "hwsdk.h"
#include "vos.h"   // ������ͷ�ļ�
#include "SDKDef.h"


class CUserInfo
{
public:
	CUserInfo(void);
	virtual ~CUserInfo(void);

public:
	/******************************************************************
	 function   : Init
	 description: ��ʼ���û���Ϣ
	 input      : IVS_INT32 iSdkSessionID
	 input      : const IVS_LOGIN_INFO * pLoginReqInfo
	 input      : const ST_SRU_LOGIN_RSP & stSRULoginRsp
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 Init(IVS_INT32 iSdkSessionID, const IVS_LOGIN_INFO* pLoginReqInfo, const ST_SRU_LOGIN_RSP &stSRULoginRsp);

	/******************************************************************
	 function   : Init
	 description: ��ʼ���û���Ϣ;
	 input      : IVS_INT32 iSdkSessionID
	 input      : const IVS_LOGIN_INFO_EX * pLoginReqInfoEx
	 input      : const IVS_CHAR * pUserName
	 input      : const ST_SRU_LOGIN_RSP & stSRULoginRspEx
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 Init(IVS_INT32 iSdkSessionID, const IVS_CHAR *pUserName, const IVS_LOGIN_INFO_EX* pLoginReqInfoEx, const ST_SRU_LOGIN_RSP &stSRULoginRspEx);

	/******************************************************************
	 function   : GetServerIP
	 description: ��ȡ��¼IP
	 output     : NA
	 return     : string
	*******************************************************************/
	virtual std::string GetServerIP() {return m_strIP;}

	/******************************************************************
	 function   : GetSerSessionID
	 description:  ��ȡ��¼������SessionID
	 output     : NA
	 return     : string
	*******************************************************************/
	virtual std::string GetSerSessionID() {return m_stSRULoginRsp.szSessionID;}

	/******************************************************************
	 function   : GetLoginRspInfo
	 description: ��ȡ��¼��Ӧ��Ϣ
	 input      : ST_SRU_LOGIN_RSP & stSRULoginRsp
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetLoginRspInfo(ST_SRU_LOGIN_RSP &stSRULoginRsp) const;

	/******************************************************************
	 function   : GetServerPort
	 description: ��ȡ������SMU�˿�
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetServerPort() const{ return m_iSmuPort; }

	/******************************************************************
	 function   : GetLoginName
	 description: ��ȡ�û���
	 output     : NA
	 return     : string
	*******************************************************************/
	std::string GetLoginName() const { return m_strLoginName; }

	/******************************************************************
	 function   : GetTicketID
	 description: ��ȡ�����¼��TicketID
	 output     : NA
	 return     : string
	*******************************************************************/
	std::string GetTicketID() const { return m_strTicketID; }

	/******************************************************************
	 function   : GetPassword
	 description: ��ȡ�û�����
	 output     : NA
	 return     : string
	*******************************************************************/
	std::string GetPWD() const;

	/******************************************************************
	 function   : GetOrgCode
	 description: ��ȡ��֯���
	 output     : NA
	 return     : string
	*******************************************************************/
	std::string GetOrgCode() const { return m_stSRULoginRsp.szOrgCode; }

	/******************************************************************
	 function   : GetSdkSessionID
	 description: ��ȡSDK��SessionID
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetSdkSessionID() const { return m_iSdkSessionID; };

    /******************************************************************
     function   : GetClientType
     description: ��ȡSDK��m_iClientType
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetClientType() const { return m_iClientType; };

    /******************************************************************
     function   : GetUserLocalIp
     description: ��ȡSDK��m_strLocalIp
     output     : NA
     return     : string
    *******************************************************************/
    std::string GetUserLocalIp() const { return m_strLocalIp; };

	/******************************************************************
	 function   : GetDomainName
	 description: ��ȡSDK��m_strDomainName
	 output     : NA
	 return     : string
	*******************************************************************/
	std::string GetDomainName() const { return m_strDomainName; };

    /******************************************************************
     function   : GetMachineName
     description: ��ȡ������
     output     : NA
     return     : std::string
    *******************************************************************/
    std::string GetMachineName() const { return m_strMachineName; };

    /******************************************************************
     function   : SetUserLocalIp
     description: ����SDK��m_strLocalIp
     input      : const string & strIp
     output     : NA
     return     : void
    *******************************************************************/
    void SetUserLocalIp(const std::string& strIp) { m_strLocalIp = strIp; };

    /******************************************************************
     function   : GetLoginType
     description: ��ȡ��¼����
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetLoginType() const { return m_loginType; };
    
    /******************************************************************
     function   : GetLoginId
     description: ��ȡLoginId
     output     : NA
     return     : char*
    *******************************************************************/
    char* GetLoginId();

    /******************************************************************
     function   : SetLoginID
     description: ����LoginID
     input      : std::string & strLoginID
     output     : NA
     return     : void
    *******************************************************************/
    void SetLoginID(const std::string &strLoginID);
    
    /******************************************************************
     function   : GetUserID
     description: ��ȡUserID
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetUserID() const;

	/******************************************************************
	 function   : Clear
	 description: �����û���Ϣ
	 output     : NA
	 return     : void
	*******************************************************************/
	void Clear();

private:
	ST_SRU_LOGIN_RSP m_stSRULoginRsp;  // ��¼�û���Ӧ��Ϣ

	std::string    m_strLoginName;           // ��¼�û���
	std::string	m_strTicketID;			//�����¼��TicketID;
	std::string    m_strPWD;            // ��¼�û�����
	std::string    m_strIP;                  // ��¼SMU��ַ
	IVS_INT32 m_iSmuPort;             // ��¼SMU�˿�
	IVS_INT32 m_loginType;				// ��½���� 0-������1-windows��2-windows����
	std::string		m_strDomainName;			//����
	std::string		m_strMachineName;			//������
    IVS_INT32 m_iClientType;             //�ͻ�������
	IVS_INT32 m_iSdkSessionID;         // ���û�SessionID;
    std::string    m_strLocalIp;                      //����IP��ʵ��ҵ��ʹ��

	VOS_Mutex* m_pConfigMutex;   //������
};

#endif
