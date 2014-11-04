/********************************************************************
 filename    :    SDKInterface.h
 author      :    z00201790
 created     :    2012/10/23
 description :    �����û�ʵ��,SDK���֧��64���û�ʵ����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/17 ��ʼ�汾
*********************************************************************/

#ifndef __SDK_INTERFACE_H__
#define __SDK_INTERFACE_H__

#include "vos.h"      // mutex
#include "hwsdk.h"    // type definition
#include "SDKDef.h"
#include "EventMgr.h"
#include "UserMgr.h"

class CUserMgr;
typedef std::map <int, CUserMgr*>        MAP_SESSEIONID_USERMGR;
typedef MAP_SESSEIONID_USERMGR::iterator MAP_SESSEIONID_USERMGR_ITER;

typedef struct {
    int iTimeOut;      // ͬ��������Ϣ��ʱʱ��
} ST_SDK_DEFAULT_CFG;


class CSDKInterface
{
public:
    CSDKInterface(void);
	~CSDKInterface(void);

	/******************************************************************
	 function   : Init
	 description: ��ʼ��
	 output     : NA
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 Init();

	/******************************************************************
	 function   : Cleanup
	 description: ����
	 output     : NA
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 Cleanup();

	/******************************************************************
	 function   : SetEventCallBack
	 description: ����SDK�Ļص��¼�
	 input      : IVS_INT32 iSessionID              ��Ӧ���û��������SessionID
	 input      : EventCallBack fEventCallBack      �¼��ص�����ָ��
	 input      : void * pUserData                  �û�����
	 output     : NA
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 SetEventCallBack(IVS_INT32 iSessionID, EventCallBack fEventCallBack = NULL, void *pUserData = NULL);

public:
	/******************************************************************
	 function   : GetLoginRspInfo
	 description: ��ȡ��¼��Ϣ
	 input      : IVS_INT32 iSessionID              ��Ӧ���û��������SessionID
	 input      : LOGIN_RSP_INFO * pLoginRspInfo    
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	//IVS_INT32 GetLoginRspInfo(IVS_INT32 iSessionID, LOGIN_RSP_INFO* pLoginRspInfo);

	/******************************************************************
	 function   : GetSessionIDByLoginID
	 description: ����loginID��ѯ SessionID
	 input      : const string & strLoginID  // ��¼ID
     output     : NA
	 return     : IVS_INT32 SessionIDֵ
	*******************************************************************/
	IVS_INT32 GetSessionIDByLoginID(const string &strLoginID);

	/******************************************************************
	 function   : GetSessoinID
	 description: ��ȡ�ỰID
	 output     : NA
	 return     : int �ỰID
	*******************************************************************/
	int GetSessoinID();

	/******************************************************************
	 function   : FreeSessoinID
	 description: �ͷ�SessionID
	 input      : int iSessionID    �ỰID
	 output     : NA
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 FreeSessoinID(int iSessionID);

	/******************************************************************
	 function   : AddSessoinID_SingleSvrProxyToMap
	 description: ����SessionID��map��
	 input      : int iSessionID                �ỰID
	 input      : CUserMgr * pSingleSvrProxy    �û��������;
	 output     : NA
	 return     : void
	*******************************************************************/
	void AddSessoinID_SingleSvrProxyToMap(int iSessionID, CUserMgr *pSingleSvrProxy);

	/******************************************************************
	 function   : GetUserMgr
	 description: ����SessionID��ȡ�û�����
	 input      : int iSessionID        �ỰID
	 output     : NA
	 return     : CUserMgr *            �û��������;
	*******************************************************************/
	CUserMgr *GetUserMgr(int iSessionID);

    /******************************************************************
     function   : Login
     description: �û���¼
     input      : const IVS_LOGIN_INFO * pLoginReqInfo  ��¼��Ϣ
     input      : IVS_INT32 * pSessionId                �ػ�ID
     output     : NA
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
    IVS_INT32 Login(const IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId);


	/******************************************************************
     function   : LoginByTicket
     description: �û������¼;
     input      : const IVS_LOGIN_INFO_EX * pLoginReqInfoEx  �����¼��Ϣ;
     output      : IVS_INT32 * pSessionId                �ỰID;
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��;
    *******************************************************************/
    IVS_INT32 LoginByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx, IVS_INT32* pSessionId);


    /**************************************************************************
    * name       : WinUserLogin
    * description: �û���¼
    * input      : pUserName �û���
                   pPassword ����
                   pIP       ������IP��ַ
                   iPort     �������˿�
    * output     : NA
    * return     : �ɹ�����SessionID�����ỰID��SessionID��SDK�ڲ�����������0��������
                   ��Ϊ�����ӿڵ���Σ��������ַ��ʵķ�����������¼128����������ʧ��
                   ���ظ���������
    * remark     : NA
    **************************************************************************/
    IVS_INT32 WinUserLogin(const IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId);

    // �û�ע��
    IVS_INT32 Logout(int iSessionID, bool iSendLogoutCmd = true);

    // �����ѵ�¼�û�
    int RemoveUser(const string &strUserName, const string &strIP, const string &strDomainName);

	// �����ѵ����¼���û�;
	int RemoveUserByTicketID(const string &strTicketID, const string &strIP, const string &strDomainName);

  	// �ͷ�SDK�ڲ�������ڴ�
	IVS_INT32 ReleaseBuffer(IVS_CHAR *pBuffer)const;
	
	//ͨ��rtsp�����ȡ�û����������
	CUserMgr* GetUserMgrbyRtspHandle(const long iRtspHandle);

    void AddTransIDChannelListRetriveStatusMap(std::string& strTransID, IVS_CHANNLELIST_RETRIVE_STATUS_INFO& channelListRetriveStatus);
    void RemoveChannelListRetriveStatusByTransID(std::string& strTransID);
    IVS_INT32 GetChannelListRetriveStatusByTransID(std::string& strTransID, IVS_CHANNLELIST_RETRIVE_STATUS_INFO& channelListRetriveStatus);
    
private:
    // ��ʼ��BP
    IVS_INT32 BPInit();

    // ����BP
    IVS_INT32 BPCleanup();
    
    // ���ر������ò���
    IVS_INT32 LoadLocalConfig();

protected:
	char                    m_SessionIDFlagArray[SDK_CLIENT_TOTAL]; //SessionID��־���� 0��ʾδ����  1��ʾ�ѷ���
	VOS_Mutex               *m_pMutexArrayLock;                     // m_SessionIDFlagArray��
	MAP_SESSEIONID_USERMGR  m_mapSessionIDToSingleSvrProxy;         // �洢�û�ʵ��
	VOS_Mutex               *m_pMutexMapLock;                       // m_mapSessionIDToSingleSvrProxy��
	VOS_Mutex				*m_pMutexLoginLock;						//��ͨ��¼�ӿڵĻ�����
	VOS_Mutex				*m_pMutexLoginByTktLock;			//�����¼�ӿڵĻ�����
	CUserMgr                m_UserMgrArray[SDK_CLIENT_TOTAL];       // 128���û�ʵ��
	IVS_BOOL                m_bInit;                                // �Ƿ��ʼ��
    IVS_BOOL                m_bBplogInit;                           // ��־��ʼ���������ʼ����־ʧ�ܣ����õ���BP������־����;


    std::map<std::string, IVS_CHANNLELIST_RETRIVE_STATUS_INFO> m_ChannelListRetriveStatusMap;
    VOS_Mutex               *m_pRetriveStatusMapLock;                       // m_mapSessionIDToSingleSvrProxy��


private: 
    CEventMgr               m_eventMgr;                             // �¼�������;
};
#endif  //__SDK_INTERFACE_H__
