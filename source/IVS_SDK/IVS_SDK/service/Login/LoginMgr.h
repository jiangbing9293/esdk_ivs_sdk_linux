/********************************************************************
    filename	: 	LoginMgr.h
    author		:	x00192614
    created		:	2011/11/1
    description	:	�û���¼������
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/5/20 ��ʼ�汾
 *********************************************************************/

#ifndef __LOGIN_MNG_H__
#define __LOGIN_MNG_H__

#include "ivs_error.h"
#include "hwsdk.h"
#include "UserInfo.h"
#include "bp_def.h"
// #include <map>
#include "SDKDef.h"
#include "bp_security_client.h"

class CUserMgr;

class CLoginMgr	
{
public:
	CLoginMgr(void);
	~CLoginMgr(void);

    /******************************************************************
     function   : SetUserMgr
     description: ���õ�¼ģ����û�ָ��
     input      : CUserMgr * pUserMgr �û�ָ��
     output     : NA
     return     : void
    *******************************************************************/
    void SetUserMgr(CUserMgr *pUserMgr);

    /**************************************************************************
    * name       : Login
    * description: �û���¼
    * input      : IVS_LOGIN_INFO �û���¼��Ϣ
    * output     : NA
    * return     : ������Ϣ�ɹ�����SMU�����룬ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_INT32 Login(const IVS_LOGIN_INFO* pLoginReqInfo);

	/**************************************************************************
    * name       : LoginByTicket
    * description: �û������¼;
    * input      : IVS_LOGIN_INFO_EX �û������¼��Ϣ;
    * output     : NA
    * return     : ������Ϣ�ɹ�����SMU�����룬ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_INT32 LoginByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx);

	/******************************************************************
	 function   : GetInitData
	 description:  ��ȡ��·�ɣ�NVRList��������б�
	 output     : NA
	 return     : void
	*******************************************************************/
	void GetInitData();

	/******************************************************************
	 function   : GetLoginRspInfo
	 description: ��ȡ��¼��Ӧ��Ϣ
	 input      : LOGIN_RSP_INFO * pLoginRspInfo SDK����ĵ�¼��Ӧ��Ϣ
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	//IVS_INT32 GetLoginRspInfo(LOGIN_RSP_INFO* pLoginRspInfo);

	/******************************************************************
	 function   : Logout
	 description: �û�ע��
	 input      : bool bSendLogoutCmd �Ƿ���ע������Ĭ��Ϊtrue
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 Logout(bool bSendLogoutCmd = true);

	/******************************************************************
	 function   : GetUserInfo
	 description: ��ȡ�û���Ϣ
	 output     : NA
	 return     : CUserInfo* �����û���Ϣ��ָ��
	*******************************************************************/
	CUserInfo* GetUserInfo();

    /******************************************************************
     function   : GetLoginId
     description: ��ȡ�û���¼�ɹ�����LoginID
     output     : NA
     return     : char* ��¼�ɹ����ص�LoginID
    *******************************************************************/
    char* GetLoginId();

    /******************************************************************
     function   : GetUserID
     description:  ��ȡ�û���¼�ɹ�����UserID
     output     : NA
     return     : IVS_UINT32 ��¼�ɹ�����UserID
    *******************************************************************/
    IVS_UINT32 GetUserID() const;

	/******************************************************************
     function   : GetLoginType
     description:  ��ȡ�û��ĵ�¼���ͣ������¼����ͨ��¼
     output     : NA
     return     : IVS_LOGIN_TYPE_E ��¼����
    *******************************************************************/
    IVS_LOGIN_TYPE_E GetLoginType() const;

    /******************************************************************
     function   : LoginOmuThreadEntry
     description:  ��½Omu�߳����
     input      : void * pArg
     output     : NA
     return     : long __stdcall
    *******************************************************************/
    static long __SDK_CALL LoginOmuThreadEntry(void* pArg);

    /******************************************************************
     function   : ReLogin
     description: SMU�ָ������µ�¼��SMU
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 ReLogin(std::string& strLinkID);

    IVS_INT32 ReLoginSMU(const std::string& strIP, int iPort);

    IVS_INT32 ReLoginOmu(const std::string& strIP, int iPort);

public:
    /******************************************************************
     function   : LoginSmu
     description:  �û���¼������SMU
     input      : const IVS_LOGIN_INFO * pLoginReqInfo ��¼�������
     output      : ST_SRU_LOGIN_RSP & stSRULoginRsp ��¼������Ϣ
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 LoginSmu(const IVS_LOGIN_INFO* pLoginReqInfo, ST_SRU_LOGIN_RSP &stSRULoginRsp);

	/******************************************************************
     function   : LoginSmuByTicket
     description:  �û������¼������SMU;
     input      : const IVS_LOGIN_INFO_EX * pLoginReqInfoEx �����¼�������;
     output      : ST_SRU_LOGIN_RSP & stSRULoginRspEx �����¼������Ϣ;
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 LoginSmuByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx, ST_SRU_LOGIN_RSP &stSRULoginRspEx);

    /******************************************************************
     function   : LoginScu
     description: �û���¼ָ����SCU
     input      : const string & strDomainCode �����
     input      : const string & strNVRCode NVR����
     input      : string & strLinkID ��SCU�����ӱ�ʶ
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 LoginScu(const std::string &strDomainCode,const std::string &strNVRCode, std::string &strLinkID);

    /******************************************************************
     function   : GeneralLoginSmu
     description: �û���½��SMU
     input      : const std::string & strDomainCode �����
     input      : std::string & strLinkID ��SMU�����ӱ�ʶ
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 GeneralLoginSmu(const std::string &strDomainCode, std::string &strLinkID);

    /******************************************************************
     function   : LoginOmu
     description: ��½OMU
     output     : NA
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 LoginOmu();

    /******************************************************************
     function   : LogoutSmu
     description: �û�ע��
     input      : const std::string & sSmuLinkId ����SMU�����ӱ�ʶ
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 LogoutSmu(const std::string& sSmuLinkId);

private:
    /******************************************************************
     function   : SendGeneralAuthCmd
     description: ����ͨ��ҵ����֤��Ϣ
     input      : int iNetElementType
     input      : const std::string & strLinkId ���ӱ�ʶ
     input      : CXml & xml ����xml
     input      : const std::string & strTransID ����ID
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_INT32 SendGeneralAuthCmd(int iNetElementType, const std::string& strLinkId, CXml& xml, const std::string& strTransID) const;

    /******************************************************************
     function   : SendReConnectCmd
     description: ��������SMU���󣨺���Կ������loginID���ܣ�
     input      : const std::string & strLinkID
     output      : std::string & strLoginID
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SendReConnectCmd(const std::string &strLinkID, std::string &strLoginID);

    /******************************************************************
     function   : SendLoginCmd
     description: ���·��͵�¼����
     input      : const std::string & strLinkID
     output     : std::string & strLoginID
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SendLoginCmd(const std::string &strLinkID, std::string &strLoginID);

    /******************************************************************
     function   : RefreshLoginInfo
     description: �����ɹ��󣬸���loginID��LinkID
     input      : const std::string strLoginID ���»�ȡ��LoginID
     input      : int iSessionID
     input      : const std::string strLocalDomainCode 
     input      : const string strLinkID ���µ�¼�ɹ���LinkID
     output     : NA
     return     : void
    *******************************************************************/
    void RefreshLoginInfo(const std::string strLoginID, int iSessionID, const std::string strLocalDomainCode, const std::string strLinkID );

    IVS_INT32 SendReLoginOMUCmd(const std::string& strLinkID);

public:
    bool m_bLoginOmuThreadRun; //��½OMU���߳�������

private:

	CUserInfo  m_userInfo;   // �û���Ϣ
    VOS_Thread*  m_pLoginOMUThread; // ��¼OMU�߳�ָ
    CUserMgr   *m_pUserMgr; // �û�ָ��
	IVS_LOGIN_TYPE_E m_enLoginType;	//�û���¼���ͣ�0 ������¼;1 �����¼;
};

#endif
