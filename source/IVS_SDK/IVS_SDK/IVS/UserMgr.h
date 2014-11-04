/********************************************************************
filename	: 	IVSSDK.cpp
author		:	z00201790
created		:	2012/10/23	
description	:	���� DLL Ӧ�ó���ĵ�������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/10/23 ��ʼ�汾;
*********************************************************************/

#ifndef __USER_MGR_H__
#define __USER_MGR_H__

#include "UserInfoMgr.h"
#include "RoleMgr.h"
#include "UserGroupMgr.h"
#include "RecordPlanMgr.h"
#include "RecordBookmark.h"
#include "RealPlayBookmark.h"
#include "RecordList.h"
#include "DeviceMgr.h"
#include "AlarmMgr.h"
#include "AlarmAreaMgr.h"
#include "AlarmHelpMgr.h"
#include "AlarmLevelMgr.h"
#include "DeviceGroupMgr.h"
#include "DeviceParaConfig.h"
#include "RealPlayMgr.h"
#include "RecordService.h"
#include "PlaybackMgr.h"
#include "LogMgr.h"
#include "DeviceParaConfig.h"
#include "LoginMgr.h"
#include "SDKDef.h"
#include "IVS_SDKTVWall.h"
#include "AlarmLinkageMgr.h"
#include "IntelligenceAnalysisMgr.h"
#include "LinkRouteMgr.h"
#include "DomainRouteMgr.h"
#include "PtzControl.h"
#include "UserDataMgr.h"
#include "MotionDetection.h"
#include "AutoResume.h"
#include "CloseLinkCount.h"
#include "ReLink.h"
#ifdef WIN32
#include "TVWallMgr.h"
#include "RecordBackupMgr.h"
#endif
#include "CapturePicture.h"
#include "ClientNetworkMgr.h"
#ifdef WIN32
#include "AudioMgr.h"
#endif
#include "DownloadManger.h"
#include "TelepresenceMgr.h"
#include "ReportMgr.h"
#include "UpgradeMgr.h"
#include "ToolsHelp.h"
#include "XmlProcess.h"

#define XML XmlProcessMgr::GetInstance()

typedef enum
{
	TYPE_NSS_XML = 0,
	TYPE_NSS_NOXML
}NSS_NEED_XML;

class CSendNssMsgInfo
{
private:
	unsigned short m_usReqID;               //������ϢNSSͷ
	unsigned int m_uiNeedXml;               //�Ƿ���Ҫ����XML
	int m_iNetElemType;                    //��Ҫ���͵���Ԫ
    unsigned int m_iReqLength;             //�����ִ����ȣ������ڷ��ͽṹ�壻���಻��Ҫ
    char* m_pReqData;                      //��������ָ�� 
	std::string m_strDomainCode;           //�����
	std::string m_strCameraCode;           //��������� 
	std::string m_strReqData;              //����XML�ִ�ָ��

public:
	CSendNssMsgInfo()
		: m_usReqID(0)
		, m_uiNeedXml(0)
		, m_iNetElemType(0)
        , m_iReqLength(0)
        , m_pReqData(NULL)
	{}

	unsigned short GetReqID()const { return m_usReqID; };
	void SetReqID(unsigned short usReqID){ m_usReqID = usReqID; };
	unsigned int GetNeedXml()const { return m_uiNeedXml; };
	void SetNeedXml(unsigned int uiNeedXml){ m_uiNeedXml = uiNeedXml; };
	int GetNetElemType()const { return m_iNetElemType; };
	void SetNetElemType(int usNetElemType){ m_iNetElemType = usNetElemType; };
    unsigned int GetReqLength() const { return m_iReqLength; };
    void SetReqLength(unsigned int uiReqLength) { m_iReqLength = uiReqLength; };
    char* GetReqDataPointer() { return m_pReqData; };
    void SetReqDataPointer(char*& pReqData) { m_pReqData = pReqData; };

	std::string& GetDomainCode(){ return m_strDomainCode; };//lint !e1036 !e1536
	void SetDomainCode(const std::string& usDomainCode){ m_strDomainCode = usDomainCode; };
	std::string& GetCameraCode(){ return m_strCameraCode; };//lint !e1036 !e1536
	void SetCameraCode(const std::string& usCameraCode){ m_strCameraCode = usCameraCode; };
	std::string& GetReqData(){ return m_strReqData; };//lint !e1036 !e1536
	void SetReqData(const std::string& strReqData){ m_strReqData = strReqData; };
	
};

typedef enum EM_ADD_LINK_STATUS
{
	TYPE_ADD_CLOSECOUNT = 0,
	TYPE_ADD_RELINK
}ADD_LINK_STATUS;

class CUserMgr
{
public:
	CUserMgr(void);
	~CUserMgr(void);

    /////////////////////////////�ṩ�����������Ⱪ¶�Ľӿ�begin///////////////////////////
    CUpgradeMgr& GetUpgradeMgr(){return m_UpgradeMgr;}; //lint !e1536

    // ��ȡ��¼���������;
    CLoginMgr& GetLoginMgr(){ return m_loginObj; };//lint !e1536

    // ��ȡ�û���Ϣ���������;
    CUserInfoMgr& GetUserInfoMgr(){ return m_UserInfoMgr; };//lint !e1536

    // ��ȡ�û�����������;
    CUserGroupMgr& GetUserGroupMgr(){ return m_UserGroupMgr; };//lint !e1536

    // ��ȡ��ɫ���������;
    CRoleMgr& GetRoleMgr(){ return m_RoleMgr; };//lint !e1536

    // ��ȡ¼��ƻ����������;
    CRecordPlanMgr& GetRecordPlanMgr(){ return m_RecordPlanMgr; };//lint !e1536

    // ��ȡ¼����ǩ���������;
    CRecordBookmark& GetRecordBookmark(){ return m_RecordBookMark; };//lint !e1536

	 // ��ȡʵ����ǩ���������;
    CRealPlayBookmark& GetRealPlayBookmark(){ return m_RealPlayBookMark; };//lint !e1536

    // ��ȡ¼���б��ѯ���������;
    CRecordList& GetRecordList(){ return m_RecordList; };//lint !e1536

    // ��ȡ�豸����������;
    CDeviceGroupMgr& GetDeviceGroupMgr(){ return m_DeviceGroupMgr; };//lint !e1536

    // ��ȡ�豸�������������;
    CDeviceParaConfig& GetDeviceParaConfig(){ return m_DeviceParaConfig; };//lint !e1536

    // ��ȡ�澯���������;
    CAlarmMgr& GetAlarmMgr(){ return m_AlarmMgr; };//lint !e1536

    // ��ȡ�澯�������������;
    CAlarmAreaMgr& GetAlarmAreaMgr(){ return m_AlarmAreaMgr; };//lint !e1536

    // ��ȡ�澯�������������;
    CAlarmHelpMgr& GetAlarHelpMgr(){ return m_AlarmHelpMgr; };//lint !e1536

    // ��ȡ�澯������������;
    CAlarmLevelMgr& GetAlarmLevelMgr(){ return m_AlarmLevelMgr; };//lint !e1536

    // ��ȡʵ�����������;
    CRealPlayMgr& GetRealPlayMgr() { return m_RealPlayMgr;}; //lint !e1536

    // ��ȡ¼��ҵ�������������;
    CRecordService& GetRecordService(){ return m_RecordService; };//lint !e1536

    // ��ȡ�طŹ��������;
    CPlaybackMgr& GetPlaybackMgr(){ return m_PlaybackMgr; };//lint !e1536

    // ��ȡ������־���������;
    CLogMgr& GetLogMgr(){ return m_LogMgr; };//lint !e1536

    // ��ȡ�澯���������;
    CAlarmLinkageMgr& GetAlarmLinkageMgr(){ return m_AlarmLinkageMgr; }; //lint !e1536

    // ��ȡ���ܷ���������������;
    CIntelligenceAnalysisMgr& GetIntelligenceAnalysisMgr(){ return m_IntelligenceAnalysisMgr; };//lint !e1536

    // ��ȡ��·�ɹ��������;
    CDomainRouteMgr& GetDomainRouteMgr() { return m_DomainRouteMgr;}; //lint !e1536

    // ��ȡ��̨���������;
    CPtzControl& GetPtzControl(){ return m_PtzControl; };//lint !e1536

    // ��ȡ�û����������;
    CUserDataMgr& GetUserDataMgr(){ return m_UserDataMgr; };//lint !e1536

    // �ƶ������������;
    CMotionDetection& GetMotionDetection(){ return m_MotionDetection; };//lint !e1536

    // ��ȡ�豸���������;
    CDeviceMgr& GetDeviceMgr() { return m_DeviceMgr; };  //lint !e1536

    // ��ȡ�Զ��ָ������;
#ifdef WIN32
    CAutoResume& GetAutoResume() { return m_AutoResume;}; //lint !e1536
	
	// ��ȡ����ǽ���������;
	CTVWallMgr& GetTVWallMgr() { return m_TVWallMgr;}; //lint !e1536
#endif

	// ��ȡ¼�񱸷ݹ��������
#ifdef WIN32
	CRecordBackupMgr& GetRecordBackupMgr() {return m_RecordBackupMgr;}; //lint !e1536
#endif

	// ��ȡͼ��ץ�Ĺ��������;
	CCapturePicture& GetCapturePicture(){ return m_CapturePicture;};//lint !e1536

	// ��ȡ�ͻ������������;
	CClientNetworkMgr& GetClientNetworkMgr(){ return m_ClientNetworkMgr;};//lint !e1536

    // ��ȡ��Ƶ���������;
#ifdef WIN32
    CAudioMgr& GetAudioMgr() { return m_AudioMgr;}; //lint !e1536
#endif

	CDownloadMgr& GetDownLoadMgr() { return m_DownLoadMgr; };//lint !e1536

	// ��ȡ������������
	CTelepresenceMgr& GetTelepresenceMgr() { return m_TPMgr; };//lint !e1536

    // ��ȡ������������
    CReportMgr& GetReportMgr(){ return m_ReportMgr; };//lint !e1536
	
    /////////////////////////////�ṩ�����������Ⱪ¶�Ľӿ�end///////////////////////////

    // ����SDKInterface��ʵ������;
	void SetSingleSvrProxy(void *pSingleSvrProxy);

    void* GetSingleSvrProxy(){return m_pSingleSvrProxy; };

    // ���ø�UserMgr�󶨵�SessionID��;
	void SetSessionID(int iSessionID){ m_iSessionID = iSessionID; };

    // ��ȡ��UserMgr��Ӧ��SessionID
	int GetSessionID() const{ return m_iSessionID; };

    // ��ȡSDK������û���Ϣ����������ҵ��
    CUserInfo* GetUserInfoEx();

    //��ȡloginId
    char* GetLoginId();
    
    // ��ȡUserID
    IVS_UINT32 GetUserID() const;

    // �û���¼
    IVS_INT32 Login(const IVS_LOGIN_INFO* pLoginReqInfo);

	// �û������¼;
	IVS_INT32 LoginByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx);

    // �û�ע��
    IVS_INT32 Logout(bool bSendLogoutCmd);

    // �����¼��ص�����;
    void SetEventCallBack( EventCallBack pEventCallBack, void *pUserData)
    {
        m_fnEventCallBack = pEventCallBack;
        m_pUserParam = pUserData;
    }

    //��ȡ�쳣����
    void* GetEventUserData()
    {
        return m_pUserParam;
    }

    // ��ȡ�¼��ص�����
    EventCallBack GetEventCallBack() const { return m_fnEventCallBack;}

    // ��ȡ��½��smuʱ��linkId
    void GetSMULinkID(std::string& strSmuLinkID);

	//modify by zwx211831���ӷ�����
	// ��ȡָ�����smulinkId
	IVS_INT32 GetSMULinkID(std::string& strSmuLinkID, const std::string& strDomainCode);

    // ��ȡ��½ʱ��omu�������ӵ�linkId
    void GetOMULinkID(std::string& strOmuLinkID);

    // ��ȡ����LinkID����Ϣ���͵�����SMU��OMU�õ���
    IVS_INT32 GetLocalDomainLinkID(IVS_INT32 iNetElementType, std::string& strLocalDomainLink);

    // ��ȡ��scu����������linkId
    void GetSCULinkID(const std::string& strIP, const std::string& strPort, std::string& strScuLinkID) const;

    // �����û��������
    void SetDomainCode(const std::string &strDomainCode){ m_strDomainCode = strDomainCode; };

    // ��ȡ�û��������
    void GetDomainCode(std::string &strDomainCode)const { strDomainCode = m_strDomainCode; };

    // ����Rtsp�����ȡ���ž��
    IVS_ULONG GetPlayHandlebyRtspHandle(long iRtspHandle);

    // ���ݲ���ͨ���Ż�ȡ���ž��
    IVS_ULONG GetPlayHandlebyPlayChannel(unsigned long ulPlayChannel);

    // ����NetSource��ͨ���Ż�ȡ���ž��
    IVS_ULONG GetPlayHandlebyNet(unsigned long ulNetChannel);

	// ��ȡ����ǰ���豸����ID
	void GetDevTransID(std::string &strTransID)const { strTransID = m_strTransID; };

	// ���÷���ǰ���豸����ID
	void SetDevTransID(const std::string& strTransID){ m_strTransID = strTransID; };

	// ��ȡ����ǰ���豸����ID
	void GetDevServerCode(std::string &strServerCode)const { strServerCode = m_strServerCode; };

	// ���÷���ǰ���豸����ID
	void SetDevServerCode(const std::string& strServerCode){ m_strServerCode = strServerCode; };
	 /******************************************************************
     function   : GetSameSuperDomainCode
     description: ��ȡ����ĸ���ڵ�
     input      : strDomainCode1 ��Code1
     input      : strDomainCode2 ��Code2
     output     : strSuperDomainCode ������Ϣ
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 GetSameSuperDomainCode(const std::string& strDomainCode1,const std::string& strDomainCode2,
			std::string& strSuperDomainCode);

	 /******************************************************************
     function   : IsInDomainTree
     description: �ж��Ƿ���������¼���
     input      : strDomainCode ��Code
     output     : 
     return     : bool true�ɹ� falseʧ��
    *******************************************************************/
	bool IsInDomainTree(const std::string& strDomainCode);
    
	/******************************************************************
     function   : BuildSMUCmd
     description: ����NSS�������½��SMU,���ӽ����ɹ�֪ͨ�û�
     input      : unsigned short usReqID NSS��Ϣ����
     input      : const char * pData     ��������
     output     : NA
     return     : BP::CCmd* �ɹ�����CCmd��ʧ�ܷ���NULL
    *******************************************************************/
    BP::CCmd* BuildSMUCmd(unsigned short usReqID,const char* pData);

     /******************************************************************
     function   : SendCmd
     description: ��ָ����������������;
     input      : pNssMsgInfo     ָ��ṹ��
	 input      : uiTimeout       ��ʱʱ��
	 input      : bTimeout        �Ƿ���Ҫ���ó�ʱʱ��(0(Ĭ��):����Ҫ����;1:��Ҫ����)
     output     : strNSSRsp       ��Ϣ��Ӧָ��
	 output     : iNeedRedirect   �Ƿ���Ҫ�ض���
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
    IVS_INT32 SendCmd(CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp,IVS_INT32& iNeedRedirect, uint32_t uiTimeout = 35, IVS_BOOL bTimeout = 0);
    IVS_INT32 ProcrssEnableLinkNode(CLinkInfo& oLinkIdInfo);
	 /******************************************************************
     function   : SendRedirectCmd
     description: �����ض������
     input      : pNssMsgInfo     ָ��ṹ��
     output     : strNSSRsp       ��Ϣ��Ӧָ��
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 SendRedirectServe(CSendNssMsgInfo& sendNssMsgInfo,std::string& strNSSRsp);

	/******************************************************************
	 function   : SendSynCmd
	 description: ����ͬ������,���첢����Cmd
	 input      : const SEND_NSS_MSG_INFO * pNssMsgInfo ������Ϣ�ṹ��
	 input      : const std::string & strLinkID         ָ�����͵�����
	 input      : uiTimeout       ��ʱʱ��
	 input      : bTimeout        �Ƿ���Ҫ���ó�ʱʱ��(0(Ĭ��):����Ҫ����;1:��Ҫ����)
	 output     : std::string & strNSSRsp               ������Ӧ
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 SendSynCmd(CSendNssMsgInfo& sendNssMsgInfo, const std::string& strLinkID,std::string& strNSSRsp, uint32_t uiTimeout = 35, IVS_BOOL bTimeout = 0);

    // �첽������Ϣ
    IVS_INT32 SendAsynMsg(const IVS_CHAR* pTargetDomainCode, 
        IVS_UINT32 uiReqID, 
        const std::string& strTransID,
        CXml* pSendXm);
	/******************************************************************
	 function   : GetCameraBriefInfobyCode
	 description: ͨ�������Code��ȡ�����������Ϣ,Ĭ�ϴ���Ĳ�ѯ����strCameraCode�ǲ��������
	 input      : const std::string & strCameraCode         ���������
	 input      : bool bHasDomainCode                       ָ��������������Ƿ���������
	 output     : IVS_CAMERA_BRIEF_INFO & stCameraBriefInfo �������Ϣ
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 GetCameraBriefInfobyCode(const std::string& strCameraCode, IVS_CAMERA_BRIEF_INFO& stCameraBriefInfo, bool bHasDomainCode = false);
	
	/******************************************************************
	 function   : GetCameraCodebyCode
	 description: ��������б���ͨ�����������������룬��ȡ��������������
	 input      : const std::string & strCameraCode         ���������
	 output     : std::string & strCameraCodeWithDomainCode ƴװ�����������������
	 return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
	*******************************************************************/
	IVS_INT32 GetCameraCodebyCode(const std::string& strCameraCode, std::string& strCameraCodeWithDomainCode);

    /******************************************************************
     function   : LinkOpenSuccessNotify
     description: ���ӽ���֪ͨ
     input      : const std::string & strLinkId ����ID
     output     : NA
     return     : void
    *******************************************************************/
    void LinkOpenSuccessNotify(const std::string &strLinkId);

	/******************************************************************
	 function   : AddLinkIdToBusinessResumeList
	 description: ����LinkID��������;
	 input      : const std::string & strLinkID     LinkID
	 input      : int iNetElemType                  ��Ԫ����
	 input      : const std::string & strIP         IP��ַ
	 input      : int iPort                         �˿�
	 input      : int iAddType                      ��������
	 output     : NA
	 return     : void
	*******************************************************************/
	void AddLinkIdToBusinessResumeList(const std::string& strLinkID,int iNetElemType,const std::string& strIP,int iPort,int iAddType);

	/******************************************************************
	 function   : RemoveLinkIDfromResumeList
	 description: ���������Ƴ�LinkID
	 input      : const std::string & strLinkID LinkID
	 input      : int iAddType                  ��������
	 output     : NA
	 return     : void
	*******************************************************************/
	// void RemoveLinkIDfromResumeList(const std::string& strLinkID,int iNetElemType,const std::string& strIP,int iPort,int iAddType);

    /******************************************************************
     function   : GetServiceTypebyPlayHandle
     description: ���ݲ��ž����ȡ��������
     input      : IVS_ULONG ulPlayHandle            ���ž��
     output     : IVS_SERVICE_TYPE & enServiceType  ҵ������
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
    IVS_INT32 GetServiceTypebyPlayHandle(IVS_ULONG ulPlayHandle, IVS_SERVICE_TYPE& enServiceType);
	/******************************************************************
     function   : SetPlaySecretKeybyRtspHandle
     description: ���ݻ�ȡ��RTSP Announce��Ϣ����ȡ��Ƶ������Կ
     input      : int iRtspHandle    rtsp���
				  char* pSdp         SDP��Ϣ
 
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 SetPlaySecretKeybyRtspHandle(long iRtspHandle,const char* pSdp);

	/**************************************************************************
	* function   : GetVideoNumAndResource
	* description: ��ѯMU����ԴԤ���Ƿ�ﵽ���޼���ǰ�������Ƶ���Ƿ�����
	* input      : pCameraCode       ���������
	* output     : bReachLimit       �Ƿ�ﵽ���ޣ�0-δ�ﵽ 1-�Ѵﵽ
				   bWhetherCited     �Ƿ����ã�0-û������ 1-�б�����
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
#ifdef WIN32
	IVS_INT32 GetVideoNumAndResource(const IVS_CHAR* pCameraCode,IVS_BOOL* bReachLimit,
									 IVS_BOOL* bWhetherCited);
#endif
    /******************************************************************
     function   : NotifyStreamBufferState
     description: ֪ͨ������״̬�¼�
     input      : IVS_ULONG ulPlayHandle    ���ž��
     input      : IVS_INT32 iBufferState    ������״̬
     output     : NA
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ�� 
    *******************************************************************/
#ifdef WIN32
    IVS_INT32 NotifyStreamBufferState(IVS_ULONG ulPlayHandle, IVS_INT32 iBufferState);

	IVS_VOID StopNotifyBufEvent(IVS_ULONG ulPlayHandle);
#endif
	/******************************************************************
	 function   : RecollectCameraDomainCodes2Map
	 description: �������list����������ɸѡ���ࣨ�ѿ��Ǵ��������;
	 input      : IVS_UINT32 uiCameraNum
	 input      : const IVS_DEVICE_CODE * pCameraCodeList
	 output     : DeviceCodeMap & deviceCodeMap
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 RecollectCameraDomainCodes2Map(IVS_UINT32 uiCameraNum,const IVS_DEVICE_CODE* pCameraCodeList, DeviceCodeMap& deviceCodeMap);

	/*****************************************************************
	     function   : BuildCmd
	     description: ��ȡͨ����
	     input     : CSendNssMsgInfo & sendNssMsgInfo
	     return     : 
	*****************************************************************/
	CCmd *BuildCmd(CSendNssMsgInfo& sendNssMsgInfo);
private:
	/******************************************************************
     function   : BuildLinkAndDoBusiness
     description: �������Ӳ����Ƿ���ҵ��
     input      : const std::string & strDomainCode     �����
     input      : const std::string & strNvrCode        NVR����
     output     : IVS_INT32 &iRetCode                   NSS��Ϣ������
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
    IVS_INT32 BuildLinkAndDoBusiness(const std::string& strDomainCode, const std::string& strNvrCode,CSendNssMsgInfo& sendNssMsgInfo, std::string &strNSSRsp) ;

    /******************************************************************
     function   : SendCmdbyDomainNvrCode
     description: ����������NVRCode����LinkRouteMgr��SCU��LinkID
     input      : const std::string & strDomainCode �����
     input      : const std::string & strNvrCode    NVR����
     output     : IVS_INT32 &iRetCode               NSS��Ϣ������
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
	IVS_INT32 SendCmdbyDomainNvrCode(const std::string& strDomainCode, const std::string& strNvrCode,CSendNssMsgInfo& sendNssMsgInfo, std::string &strNSSRsp) ;

    /******************************************************************
     function   : SendCmdbyReDirect
     description: ���ض�������;
     input      : const IVS_DOMAIN_ROUTE & stDomainRoute    ����Ϣ
     input      : pNssMsgInfo                               ����ָ��ṹ��
     output     : IVS_INT32 &iRetCode                       NSS��Ϣ������
     return     : IVS_INT32 IVS_SUCCEED�ɹ� IVS_FAILʧ��
    *******************************************************************/
    IVS_INT32 SendCmdbyReDirect(const IVS_DOMAIN_ROUTE& stDomainRoute,CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp);

	/******************************************************************
	 function   : StartResumeTimer
	 description: �����Զ��ָ���ʱ��
	 output     : NA
	 return     : void
	*******************************************************************/
	void StartResumeTimer();

	/******************************************************************
	 function   : StopResumeTimer
	 description: ֹͣ��ʱ��
	 output     : NA
	 return     : void
	*******************************************************************/
	void StopResumeTimer();

    /******************************************************************
     function   : PrintLog
     description: ѭ����ӡ��־����BP��ӡ�ĵ�����־���ܳ���2048
     input      : const std::string& strLog
     output     : NA
     return     : void
    *******************************************************************/
    void PrintLog(const std::string& strLog) const;

public:
    // ����ˮӡ������Ϣ
    void SetWaterMarkInfo(IVS_ULONG ulPlayHandle, ST_WATERMARK_ALARM_INFO& stWaterMarkInfo);

    // ��ȡˮӡ������Ϣ
    void GetWaterMarkInfo(IVS_ULONG ulPlayHandle, ST_WATERMARK_ALARM_INFO &stWaterMarkInfo);

    void RemoveWaterMarkInfoFromMap(IVS_ULONG ulPlayHandle);

    bool GetRelinkFlag(){return m_bRelink;};

	/******************************************************************
     function		: SetReceiveOMUAlarm
     description	: �����Ƿ����OMU�豸��澯
     input				: bReceive trueΪ���գ�falseΪ������
     output			: NA
     return			: NA
    *******************************************************************/
	void SetReceiveOMUAlarm(IVS_BOOL bReceive);

	/******************************************************************
     function		: GetReceiveOMUAlarm
     description	: ��ȡ�Ƿ����OMU�豸��澯�ı�ʶ
     input				: NA
     output			: NA
     return			: IVS_BOOL �Ƿ����OMU�豸��澯�ı�ʶ
    *******************************************************************/
	IVS_BOOL GetReceiveOMUAlarm(void) {return m_bReceiveOMUAlarm;}

	/******************************************************************
     function		: GetSSOResumeStartTime
     description	: ��ȡ������ʼʱ��
     input				: NA
     output			: NA
     return			: time_t
    *******************************************************************/
	time_t GetResumeStartTime(void) const {return m_ResumeStartTime;}

	/******************************************************************
     function		: UpdateSSOResumeStartTime
     description	: ����������ʼʱ��
     input				: NA
     output			: NA
     return			: time_t
    *******************************************************************/
	void UpdateResumeStartTime(const time_t &ltime);

private:
	CLoginMgr           m_loginObj;             // ��¼����
	CUserInfoMgr        m_UserInfoMgr;          // �û���Ϣ������;
	CUserGroupMgr       m_UserGroupMgr;         // �û��������;
	CRoleMgr            m_RoleMgr;              // ��ɫ������;
	CRecordPlanMgr      m_RecordPlanMgr;        // ¼��ƻ�������;
	CRecordBookmark     m_RecordBookMark;       // ¼����ǩ������;
	CRealPlayBookmark   m_RealPlayBookMark;     // ʵ����ǩ������
	CRecordList         m_RecordList;           // ¼���б���;
	CDeviceGroupMgr     m_DeviceGroupMgr;       // �豸�������
	CDeviceParaConfig   m_DeviceParaConfig;     // �豸�����Ĺ���
	CAlarmMgr           m_AlarmMgr;             // �澯������
	CAlarmAreaMgr       m_AlarmAreaMgr;         // �澯����������
	CAlarmHelpMgr       m_AlarmHelpMgr;         // �澯����������
	CAlarmLevelMgr      m_AlarmLevelMgr;        // �澯���������
    CRealPlayMgr        m_RealPlayMgr;          // ʵ��
	CRecordService      m_RecordService;        // ¼�������
	CPlaybackMgr        m_PlaybackMgr;          // �طŹ�����
	CLogMgr             m_LogMgr;               // ��ѯ������־
	CAlarmLinkageMgr    m_AlarmLinkageMgr;      // �澯��������
    CIntelligenceAnalysisMgr    m_IntelligenceAnalysisMgr; // ��ѯ�������ܷ�������������б�
	CDomainRouteMgr     m_DomainRouteMgr;       // ��·�ɹ�����
    CPtzControl         m_PtzControl;           // �ƾ�������
    CUserDataMgr        m_UserDataMgr;          // �����ϴ�����
	CMotionDetection    m_MotionDetection;      // �˶����
    CDeviceMgr          m_DeviceMgr;            // �豸������
	CAutoResume         m_AutoResume;	        // �Զ��ָ���
#ifdef WIN32
	CTVWallMgr			m_TVWallMgr;			// ����ǽ������
	CRecordBackupMgr    m_RecordBackupMgr;     // ¼�񱸷ݹ�����
#endif
	CCapturePicture     m_CapturePicture;       // ץ�Ŀ�����
	CClientNetworkMgr   m_ClientNetworkMgr;     // �ͻ���������
#ifdef WIN32
    CAudioMgr           m_AudioMgr;             // �����Խ��㲥������
#endif
	CDownloadMgr        m_DownLoadMgr;  
	CTelepresenceMgr    m_TPMgr;				//����Խӹ�����
    CReportMgr          m_ReportMgr;            // ���������
    CUpgradeMgr       m_UpgradeMgr;         // ������
private:
	int                 m_iSessionID;           // �û�����¼�����Ϣ
    std::string         m_strDomainCode;        // ����Code
    void*               m_pUserParam;           // �ɻص�����������û�����;
	void*               m_pSingleSvrProxy;      // ����SDKInterfaceָ��
	CNVSTimer           m_ResumeTimerInst;      // �Զ��ָ���ʱ��
	bool                m_bAutoResume;          // �Զ��ָ�����
	CNVSTimer           m_CloseLinkIDInst;      // ������linkID�ļ�����ʱ��
	CCloseLinkCount     m_CloseLinkIDCount;     // ������ͳ����
	bool                m_bCloseLinkIDCount;    // ����ͳ�ƵĿ���
	CNVSTimer           m_RelinkInst;           // ���½������ӵļ�����
	CReLink             m_Relink;               // ���½����ӵ���
	bool                m_bRelink;              // ��������;
	IVS_BOOL m_bReceiveOMUAlarm;		//�Ƿ����OMU�豸��澯��0Ϊ�����գ���0Ϊ����
    EventCallBack       m_fnEventCallBack;      // �¼��ص�����;
	std::string         m_strTransID;           // ��ȡ��������ID
	std::string         m_strServerCode;        // ��ȡ����ServerCode
	time_t					m_ResumeStartTime;		//�����¼��ʼʱ��

    MAP_WATERMARK_ALARM_INFO m_mapWaterMarkInfo;// ˮӡ������Ϣ�봰������Ӧ�� 
    VOS_Mutex*          m_pWaterMarkInfoMutex;  // ˮӡ������Ϣ��
};
#endif
