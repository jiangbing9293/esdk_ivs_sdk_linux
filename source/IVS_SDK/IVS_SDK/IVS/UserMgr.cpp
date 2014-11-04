#include "UserMgr.h"
#include "Cmd.h"
#include "NSSOperator.h"
#include "LoginXMLProcess.h"
#ifdef WIN32
#include "MediaXMLProcess.h"
#endif
#include "MotionDetection.h"
#include "IVS_Trace.h"
#include "ToolsHelp.h"
#include "XmlProcess.h"
#include "nss_xml_msg.h"

CUserMgr::CUserMgr(void)
{
	m_UserInfoMgr.SetUserMgr(this);
	m_RealPlayMgr.SetUserMgr(this);
	m_RecordService.SetUserMgr(this);
	m_PlaybackMgr.SetUserMgr(this);
    m_DeviceMgr.SetUserMgr(this);
	m_LogMgr.SetUserMgr(this);
    m_loginObj.SetUserMgr(this);
	m_AlarmLevelMgr.SetUserMgr(this);
	m_AlarmHelpMgr.SetUserMgr(this);
    m_DeviceGroupMgr.SetUserMgr(this);
    m_RecordPlanMgr.SetUserMgr(this);
	m_DeviceParaConfig.SetUserMgr(this);
	m_RecordList.SetUserMgr(this);
	m_AlarmMgr.SetUserMgr(this);
	m_AlarmAreaMgr.SetUserMgr(this);
	m_UserGroupMgr.SetUserMgr(this);
    m_RoleMgr.SetUserMgr(this);
	m_AlarmLinkageMgr.SetUserMgr(this);
	m_DomainRouteMgr.SetUserMgr(this);
	m_PtzControl.SetUserMgr(this);
    m_RecordBookMark.SetUserMgr(this);
	m_RealPlayBookMark.SetUserMgr(this);
    m_UserDataMgr.SetUserMgr(this);
	m_MotionDetection.SetUserMgr(this);
	m_CloseLinkIDCount.SetUserMgr(this);
	m_Relink.SetUserMgr(this);
#ifdef WIN32
	m_TVWallMgr.SetUserMgr(this);
	m_RecordBackupMgr.SetUserMgr(this);
#endif
	m_CapturePicture.SetUserMgr(this);
	m_ClientNetworkMgr.SetUserMgr(this);
	m_DownLoadMgr.SetUserMgr(this);
#ifdef WIN32
	m_AudioMgr.SetUserMgr(this);
#endif
	m_TPMgr.SetUserMgr(this);
    m_ReportMgr.SetUserMgr(this);
    m_IntelligenceAnalysisMgr.SetUserMgr(this);
    m_UpgradeMgr.SetUserMgr(this);

	m_fnEventCallBack = NULL;
	m_pUserParam = NULL;
	m_pSingleSvrProxy = NULL;
    m_strDomainCode = LOCAL_DOMAIN_CODE_INIT;
	m_bAutoResume = false;
	m_bCloseLinkIDCount = false;
	m_bRelink = false;
	m_bReceiveOMUAlarm = 1;
	m_ResumeStartTime = 0;

    m_mapWaterMarkInfo.clear();
    m_pWaterMarkInfoMutex = VOS_CreateMutex();
}

CUserMgr::~CUserMgr(void)
{
    m_pUserParam = NULL;
    m_pSingleSvrProxy = NULL;

    if (NULL != m_pWaterMarkInfoMutex)
    {
        (void)VOS_DestroyMutex(m_pWaterMarkInfoMutex);
        m_pWaterMarkInfoMutex = NULL;
    }

	if (m_bAutoResume)
	{
		try
		{
			StopResumeTimer();
		}
		catch (...)
		{
            BP_RUN_LOG_ERR(IVS_FAIL,"Stop Resume Timer ","throw exception.");//lint !e1551
		}
	}
}

// ����SDKInterface��ʵ������;
void CUserMgr::SetSingleSvrProxy(void *pSingleSvrProxy)
{
	if(NULL != pSingleSvrProxy)
	{
		m_pSingleSvrProxy = pSingleSvrProxy;
	}
}

// �û���¼
IVS_INT32 CUserMgr::Login(const IVS_LOGIN_INFO* pLoginReqInfo)
{	
    IVS_DEBUG_TRACE("");
    if (NULL == pLoginReqInfo)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Login", "pLoginReqInfo = NULL");
        return IVS_PARA_INVALID;
    }
	int iRet = m_loginObj.Login(pLoginReqInfo);

	if (IVS_SUCCEED == iRet || IVS_SMU_USER_FIRST_LOGIN == iRet  || IVS_SMU_USER_PWD_NEED_MOD == iRet)
	{
		// ����澯����
		//m_alarmMgr.StartAlarmReporter();
		// �����Զ��ָ���ʱ��
		StartResumeTimer();

		//modify by zwx211831:ͨ��GetDomainRoute�����򣬸���LinkID��Map
		IVS_UINT32 uiBufSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + 127 * sizeof(IVS_DOMAIN_ROUTE);
		IVS_DOMAIN_ROUTE_LIST * pDomainRouteList = (IVS_DOMAIN_ROUTE_LIST *)(new IVS_CHAR[uiBufSize]);
		CHECK_POINTER(pDomainRouteList, IVS_OPERATE_MEMORY_ERROR);

		memset(pDomainRouteList, 0, uiBufSize);
		IVS_INT32 iReturn = GetDomainRouteMgr().GetDomainRoute(pDomainRouteList, uiBufSize);
		delete []pDomainRouteList;
		pDomainRouteList = NULL;
		if (IVS_SUCCEED != iReturn)
		{
			return iReturn;
		}
		//end
	}

	return iRet;
}


//�û������¼;
IVS_INT32 CUserMgr::LoginByTicket(const IVS_LOGIN_INFO_EX* pLoginReqInfoEx)
{
	IVS_DEBUG_TRACE("");
	if (NULL == pLoginReqInfoEx)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID,"Login", "pLoginReqInfo = NULL");
		return IVS_PARA_INVALID;
	}
	int iRet = m_loginObj.LoginByTicket(pLoginReqInfoEx);

	if (IVS_SUCCEED == iRet || IVS_SMU_USER_FIRST_LOGIN == iRet  || IVS_SMU_USER_PWD_NEED_MOD == iRet)
	{
		// �����Զ��ָ���ʱ��;
		StartResumeTimer();

		//ͨ��GetDomainRoute�����򣬸���LinkID��Map;
		IVS_UINT32 uiBufSize = sizeof(IVS_DOMAIN_ROUTE_LIST) + 127 * sizeof(IVS_DOMAIN_ROUTE);
		IVS_DOMAIN_ROUTE_LIST * pDomainRouteList = (IVS_DOMAIN_ROUTE_LIST *)(new IVS_CHAR[uiBufSize]);
		CHECK_POINTER(pDomainRouteList, IVS_OPERATE_MEMORY_ERROR);

		memset(pDomainRouteList, 0, uiBufSize);
		IVS_INT32 iReturn = GetDomainRouteMgr().GetDomainRoute(pDomainRouteList, uiBufSize);
		delete []pDomainRouteList;
		pDomainRouteList = NULL;
		if (IVS_SUCCEED != iReturn)
		{
			return iReturn;
		}
	}

	return iRet;
}

// ��ȡ��¼�ɹ����ص�loginId
char* CUserMgr::GetLoginId()
{
    return m_loginObj.GetLoginId();
}

// ��ȡUserID
IVS_UINT32 CUserMgr::GetUserID() const
{
    return m_loginObj.GetUserID();
}

// ���ӽ����ɹ�֪ͨ�û�
void CUserMgr::LinkOpenSuccessNotify(const std::string &strLinkId)
{
    IVS_DEBUG_TRACE("LinkId:%s", strLinkId.c_str());
    CLinkInfo oLinkInfoTemp;
    if (!CLinkRouteMgr::instance().FindLinkIDInfo(strLinkId, oLinkInfoTemp))
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"Find linkID info from link route mgr ","failed");
        return;
    }

    //�½������ӣ�post�ź�����״̬��Ϊ������
    CLinkRouteMgr::instance().SetLinkIDStatus(strLinkId, LINKID_STATUS_OPENED);
}//lint !e1762 �ú�����������Ϊconst����ΪSetLinkIDStatus�ı�ֵ.

// ����NSS�������½��SMU,���ӽ����ɹ�֪ͨ�û�
BP::CCmd* CUserMgr::BuildSMUCmd(unsigned short usReqID,const char* pData)
{
    CHECK_POINTER(pData, NULL);
    IVS_DEBUG_TRACE("ReqID:%d, Data:%s", (int)usReqID, pData);
    std::string strSmuLinkID = "";
    GetSMULinkID(strSmuLinkID);
    return CNSSOperator::instance().BuildSMUCmd(usReqID, pData, strSmuLinkID);
}

// ��ȡ��½��smuʱ��linkId
void CUserMgr::GetSMULinkID( std::string& strSmuLinkID)
{
    IVS_DEBUG_TRACE("SmuLinkID:%s", strSmuLinkID.c_str());

    //��ȡ��½SMU��linkID
    CLinkInfo oLinkInfoResult;
    bool bGetLinkRet = CLinkRouteMgr::instance().GetLinkIDInfoByDomainCode(m_iSessionID, NET_ELE_SMU_NSS, m_strDomainCode, oLinkInfoResult);
    if (!bGetLinkRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Get LinkID Info By Domain Code ", "failed.");
        return;
    }

    //�ж�����״̬��TODO ��Ϊö�١�2Ϊ���� 3Ϊ�Ͽ�
    if (LINKID_STATUS_OPENED == oLinkInfoResult.GetLinkStatus())
    {
        strSmuLinkID = oLinkInfoResult.GetStrLinkId();
    }

    return;
}

// ��ȡָ�����smulinkId
IVS_INT32 CUserMgr::GetSMULinkID(std::string& strSmuLinkID, const std::string& strDomainCode)
{
	IVS_DEBUG_TRACE("DomainCode:%s", strDomainCode.c_str());

	//add by zwx211831���Ӷ��������Ч�Ե��ж�
	IVS_INT32 iRet = CLinkRouteMgr::instance().DecideDomainCode(m_iSessionID, NET_ELE_SMU_NSS, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Start Search Device failed", "DomainCode is invalid");
		return iRet;
	}

	//��ȡָ����SMU��linkID
	CLinkInfo oLinkInfoResult;
	bool bGetLinkRet = CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_iSessionID, strDomainCode, oLinkInfoResult);
	if (!bGetLinkRet)
	{
		// ��������;
		std::string strGeneralSmuLinkID = "";
		iRet = m_loginObj.GeneralLoginSmu(strDomainCode, strGeneralSmuLinkID);
		if (IVS_SUCCEED != iRet && IVS_SMU_USER_FIRST_LOGIN != iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Get LinkID Info By Domain Code","LoginSmu failed");
			return iRet;
		}
		strSmuLinkID = strGeneralSmuLinkID;
		if (0 == strSmuLinkID.length())
		{
			BP_RUN_LOG_ERR(IVS_FAIL, "Get SMU LinkID failed", "LinkID is NULL");
			return IVS_FAIL;
		}
		return iRet;
	}

	//add by zwx211831	���״̬Ϊ���õ�δ���ӣ�������
	if (LINKID_STATUS_ENABLED == oLinkInfoResult.GetLinkStatus())
	{
		ProcrssEnableLinkNode(oLinkInfoResult);
	}

	//�ж�����״̬��TODO ��Ϊö�١�1Ϊ���� 2Ϊ�Ͽ�
	if (LINKID_STATUS_OPENED == oLinkInfoResult.GetLinkStatus())
	{
		strSmuLinkID = oLinkInfoResult.GetStrLinkId();
	}

	if (0 == strSmuLinkID.length())
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Get SMU LinkID failed", "LinkID is NULL");
		return IVS_FAIL;
	}

	return iRet;
}

// ������Ԫ���ͻ�ȡ����linkID(SMU��OMUʹ��)
IVS_INT32 CUserMgr::GetLocalDomainLinkID(IVS_INT32 iNetElementType, std::string& strLocalDomainLink)
{
    IVS_DEBUG_TRACE("iNetElementType:%d", iNetElementType);
    //��ȡ��¼���linkID
    CLinkInfo oLinkInfoResult;
    bool bGetLinkRet = CLinkRouteMgr::instance().GetLinkIDInfoByDomainCode(m_iSessionID, iNetElementType, m_strDomainCode, oLinkInfoResult);
    if (!bGetLinkRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Get LinkID Info By Domain Code ", "failed."); // �����߼��������˷�֧�����ǵ�¼��ȡ��·��ʧ��
        return IVS_FAIL;
    }

    //�ж�����״̬
    if (LINKID_STATUS_OPENED != oLinkInfoResult.GetLinkStatus())
    {
        BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "Get LocalDomain LinkID ", "LinkStatus isn't open"); // �����߼��������˷�֧�����ǵ�¼��ȡ��·��ʧ��
        return IVS_NET_CONNECT_ERROR;
    }
    strLocalDomainLink = oLinkInfoResult.GetStrLinkId();
    BP_DBG_LOG("Get LocalDomain LinkID, strLocalDomainLink: %s", strLocalDomainLink.c_str());
    return IVS_SUCCEED;
}

// �û�ע��
IVS_INT32 CUserMgr::Logout(bool bSendLogoutCmd)
{
	IVS_DEBUG_TRACE("SendLogoutCmd:%d",(int)bSendLogoutCmd);
	int iRet = IVS_FAIL;

	// ȥ����ʱ��
	if (m_bAutoResume)
	{
		StopResumeTimer();
	}

	// ��·������
    m_DomainRouteMgr.ClearDomainRouteMap();
    m_DeviceMgr.Clear();
	iRet = m_loginObj.Logout(bSendLogoutCmd);
    
	// �����û���link·�ɴ�·�ɱ���ɾ��;
	CLinkRouteMgr::instance().DeleteLinkList(m_iSessionID);
    
    m_strDomainCode = LOCAL_DOMAIN_CODE_INIT;    // �������������ֹ֮ǰҵ����ʹ��m_strDomainCode
    m_iSessionID = SDK_SESSIONID_INIT;
    m_strTransID = "";
    m_strServerCode = "";
	return iRet;
}

// ��ȡSDK������û���Ϣ����������ҵ��
CUserInfo* CUserMgr::GetUserInfoEx()
{
	return m_loginObj.GetUserInfo();
}

// ����Rtsp�����ȡ���ž��
IVS_ULONG CUserMgr::GetPlayHandlebyRtspHandle(long iRtspHandle)
{
    IVS_DEBUG_TRACE("RtspHandle:%d", iRtspHandle);
    IVS_ULONG ulPlayHandle = m_RealPlayMgr.GetPlayHandlebyRtspHandle(iRtspHandle);
    if (0 != ulPlayHandle)
    {
        BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from real play mgr, playhandle=%u", ulPlayHandle);
        return ulPlayHandle;
    }

    ulPlayHandle = m_PlaybackMgr.GetPlayHandlebyRtspHandle(iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from play back mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}

	ulPlayHandle = m_DownLoadMgr.GetDownloadHandlebyRtspHandle(iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from DownLoad mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}

	ulPlayHandle = m_TPMgr.GetPlayHandlebyRtspHandle(iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from tp mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}
#ifdef WIN32
	ulPlayHandle = m_TVWallMgr.GetTVWallHandlebyRtspHandle((IVS_ULONG)iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from TVWall mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}

	ulPlayHandle = m_AudioMgr.GetTalkBackPlayHandleByRtspHandle((IVS_ULONG)iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from Audio mgr TalkBack, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}

	ulPlayHandle = m_AudioMgr.GetBroadCastPlayHandleByRtspHandle((IVS_ULONG)iRtspHandle);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from Audio mgr BroadCast, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}
#endif
    //BP_DBG_LOG("Get Play Handle by Rtsp Handle succeed from play back mgr, playhandle=%u", ulPlayHandle);
    return ulPlayHandle;
}

// ���ݲ���ͨ���Ż�ȡ���ž��
IVS_ULONG CUserMgr::GetPlayHandlebyPlayChannel(unsigned long ulPlayChannel)
{
    IVS_DEBUG_TRACE("PlayChannel:%lu", ulPlayChannel);
	IVS_ULONG ulPlayHandle = m_RealPlayMgr.GetPlayHandleByPlayChannel(ulPlayChannel);
    if (0 != ulPlayHandle)
    {
        BP_DBG_LOG("Get Play Handle by Play Channel succeed from real play mgr, playhandle=%u", ulPlayHandle);
        return ulPlayHandle;
    }

    ulPlayHandle = m_PlaybackMgr.GetPlayHandleByPlayChannel(ulPlayChannel);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Play Channel succeed from real play back mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}

	ulPlayHandle = m_DownLoadMgr.GetDownloadHandleByPlayChannel(ulPlayChannel);
	if (0 != ulPlayHandle)
	{
		BP_DBG_LOG("Get Play Handle by Play Channel succeed from real DownLoad mgr, playhandle=%u", ulPlayHandle);
		return ulPlayHandle;
	}
	
    //BP_DBG_LOG("Get Play Handle by Play Channel succeed from play back mgr, playhandle=%u", ulPlayHandle);
    return ulPlayHandle;
}

// ����NetSource��ͨ���Ż�ȡ���ž��
IVS_ULONG CUserMgr::GetPlayHandlebyNet(unsigned long ulNetChannel)
{
    IVS_DEBUG_TRACE("NetChannel:%lu", ulNetChannel);
	IVS_ULONG ulPlayHandle = m_RealPlayMgr.GetPlayHandleByNetSourceChannel(ulNetChannel);
    if (0 != ulPlayHandle)
    {
        BP_DBG_LOG("Get Play Handle by Net Source Channel succeed from real play mgr, playhandle=%u", ulPlayHandle);
        return ulPlayHandle;
    }

    ulPlayHandle = m_PlaybackMgr.GetPlayHandleByNetSourceChannel(ulNetChannel);
    BP_DBG_LOG("Get Play Handle by Net Source Channel succeed from play back mgr, playhandle=%u", ulPlayHandle);
#ifdef WIN32
    ulPlayHandle = m_AudioMgr.GetTalkBackPlayHandleByNetHandle((IVS_ULONG)ulNetChannel);
    if (0 != ulPlayHandle)
    {
        BP_DBG_LOG("Get Play Handle by net Handle succeed from Audio mgr TalkBack, playhandle=%u", ulPlayHandle);
        return ulPlayHandle;
    }

    ulPlayHandle = m_AudioMgr.GetBroadCastPlayHandleByNetHandle((IVS_ULONG)ulNetChannel);
    if (0 != ulPlayHandle)
    {
        BP_DBG_LOG("Get Play Handle by net Handle succeed from Audio mgr BroadCast, playhandle=%u", ulPlayHandle);
        return ulPlayHandle;
    }
#endif
    return ulPlayHandle;
}

// ���ݲ��ž����ȡ��������
IVS_INT32 CUserMgr::GetServiceTypebyPlayHandle(IVS_ULONG ulPlayHandle, IVS_SERVICE_TYPE& enServiceType)
{
    IVS_DEBUG_TRACE("PlayHandle:%lu", ulPlayHandle);
    IVS_INT32 iRet = IVS_SUCCEED;
    CMediaBase* pRealPlay = m_RealPlayMgr.GetMediaBase(ulPlayHandle);
    if (NULL != pRealPlay)
    {
        enServiceType = SERVICE_TYPE_REALVIDEO;
        BP_DBG_LOG("Get Service Type by PlayHandle succeed, ServiceType[%d]",enServiceType);
        return iRet;
    }

    CPlayback* pPlayBack = m_PlaybackMgr.GetPlayBack(ulPlayHandle);
    if (NULL != pPlayBack)
    {
        enServiceType = pPlayBack->GetServiceType();
        BP_DBG_LOG("Get Service Type by PlayHandle succeed, ServiceType[%d]",enServiceType);
        return iRet;
    }

	// add by w00210470
	CDownload *pDownload = m_DownLoadMgr.GetDownLoad(ulPlayHandle);
	if (NULL != pDownload)
	{
		enServiceType = pDownload->GetServiceType();
		BP_DBG_LOG("Get Service Type by PlayHandle succeed, ServiceType[%d]", enServiceType);
        return IVS_SUCCEED;
	}
#ifdef WIN32
	CDecoderChannel* pChannel = m_TVWallMgr.GetChannel(ulPlayHandle);
	if (NULL != pChannel)
	{
		enServiceType = SERVICE_TYPE_REALVIDEO;
		BP_DBG_LOG("Get Service Type by PlayHandle succeed In TVWall, ServiceType[%d]",enServiceType);
		return iRet;
	}

	CAudioObj *pAudioObj = m_AudioMgr.GetAudioObj(ulPlayHandle);
	if (NULL != pAudioObj)
	{
		enServiceType = pAudioObj->GetServiceType();
		BP_DBG_LOG("Get Service Type by PlayHandle succeed, ServiceType[%d]", enServiceType);
		return IVS_SUCCEED;
	}
	
	bool bIsBroadCast = m_AudioMgr.IsBroadCast(ulPlayHandle);
	if (bIsBroadCast)
	{
		enServiceType = SERVICE_TYPE_AUDIO_BROADCAST;
		BP_DBG_LOG("Get Service Type by PlayHandle succeed, ServiceType[%d]", enServiceType);
		return IVS_SUCCEED;
	}
#endif

    BP_RUN_LOG_ERR(iRet, "Get Service Type by Play Handle", "Play handle error, ulPlayHandle = %lu", ulPlayHandle);
    return IVS_FAIL;
}

//����SDP��Ϣ����ȡ�����Ƿ���ü��ܵ��㷨
IVS_INT32 CUserMgr::SetPlaySecretKeybyRtspHandle(long iRtspHandle,const char* pSdp)
{
	IVS_DEBUG_TRACE("");
	IVS_INT32 iRet = IVS_FAIL;
	if (NULL == pSdp)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"Get Secret Key ","SDP Info is NULL");
		return iRet;
	}
	//����SDP��Ϣ
	bool bUseSecretKey = false;		//�Ƿ�ʹ������Ƶ����
	MEDIA_INFO_SDP stMediaInfo;
	STORAGESECRET_INFO stStorageSecretInfo;
	MEDIA_KEY_INFO stMediaKeyInfo;
	memset(&stMediaInfo, 0x0, sizeof(MEDIA_INFO_SDP));
	memset(&stStorageSecretInfo,0x0,sizeof(STORAGESECRET_INFO));
	memset(&stMediaKeyInfo,0x0,sizeof(MEDIA_KEY_INFO));
	stMediaInfo.ManuType = 101;

	/*
	 * init payload type, add by w00210470
	 */
	stMediaKeyInfo.uPTAudio = PAY_LOAD_TYPE_PCMA;
	stMediaKeyInfo.uPTVideo = PAY_LOAD_TYPE_H264;
	
	stMediaKeyInfo.iAudioKeyType = HW_NO_DEC_TYPE;
	stMediaKeyInfo.iVideoKeyType = HW_NO_DEC_TYPE;
	stMediaKeyInfo.enVideoDecoderType = VIDEO_DEC_H264;//Ĭ�ϳ�ʼ����Ƶ����
	stMediaKeyInfo.enAudioDecoderType = AUDIO_DEC_G711U;//Ĭ�ϳ�ʼ����Ƶ��ʽ
	iRet = parseSdp(pSdp, &stMediaKeyInfo, &stMediaInfo, &stStorageSecretInfo);
	if(IVS_SUCCEED == iRet)
	{
		//Ŀǰ����SDPֻ���֪�Ƿ�ʹ���˼����㷨����������Կ
		if ((HW_NO_DEC_TYPE != stMediaKeyInfo.iAudioKeyType && HW_NO_DEC_TYPE != stMediaKeyInfo.iVideoKeyType)&& 
			stMediaKeyInfo.iAudioKeyType == stMediaKeyInfo.iVideoKeyType)
		{
            BP_DBG_LOG("Need Encry. stMediaKeyInfo.iVideoKeyType : %d", stMediaKeyInfo.iVideoKeyType);
			bUseSecretKey = true;
		}
        else
        {
            BP_DBG_LOG("No Need Encry. iVideoKeyType:%d, iAudioKeyType:%d", 
                       stMediaKeyInfo.iVideoKeyType, stMediaKeyInfo.iAudioKeyType);
        }
	}
	else
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"Parse Sdp Info","Parse Sdp Info Fail");
		return IVS_FAIL;
	}
	//��ȡ��ǰ�������ͣ������ü�����Ϣ
	IVS_ULONG ulMediaHandle = 0;
	CMediaBase* pMediaBase = NULL;
	unsigned int uiPort = 0;
	//ͨ��RtspHandle��ȡMediaBase���࣬����ʵ��
	ulMediaHandle = m_RealPlayMgr.GetPlayHandlebyRtspHandle(iRtspHandle);
	if (0 != ulMediaHandle)
	{
		pMediaBase = m_RealPlayMgr.GetMediaBase(ulMediaHandle);
		uiPort = m_RealPlayMgr.GetPlayerChannelByHandle(ulMediaHandle);

        if (NULL == pMediaBase)
        {
            BP_RUN_LOG_ERR(IVS_FAIL,"Set Secret Key ","Set Secret Key Error,media Info is NULL");
            return IVS_FAIL;
        }

        //���÷��͸�ǰ���豸��SessionInfo
        BP_DBG_LOG("SessionUrl:%s, SessionId:%s", stMediaInfo.SessionUrl, stMediaInfo.SessionId);
        pMediaBase->SetSessionInfo(stMediaInfo.SessionUrl, stMediaInfo.SessionId);
	}
	//�ط�
	ulMediaHandle = m_PlaybackMgr.GetPlayHandlebyRtspHandle(iRtspHandle);
	if (0 != ulMediaHandle)
	{
		pMediaBase = m_PlaybackMgr.GetMediaBase(ulMediaHandle);
		uiPort = m_PlaybackMgr.GetPlayerChannelByHandle(ulMediaHandle);
	}
	//����
	ulMediaHandle = m_DownLoadMgr.GetDownloadHandlebyRtspHandle(iRtspHandle);
	if (0 != ulMediaHandle)
	{
		pMediaBase = m_DownLoadMgr.GetDownLoad(ulMediaHandle);
        if (NULL == pMediaBase)
        {
            BP_RUN_LOG_ERR(IVS_FAIL,"Set Secret Key ","download mgr,media base is NULL");
            return IVS_FAIL;
        }
		uiPort = pMediaBase->GetPlayerChannel();
	}

	if (NULL == pMediaBase)
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"Set Secret Key ","Set Secret Key Error,media Info is NULL");
		return IVS_FAIL;
	}

    /*
	* set rtp payload type, add by w00210470
	*/
	MEDIA_ATTR AttrAudio = {0};//lint !e522 !e10
	AttrAudio.rtpmap = stMediaKeyInfo.uPTAudio;//lint !e1013 !e10 !e63 !e734
	AttrAudio.type   = stMediaKeyInfo.enAudioDecoderType;//lint !e1013 !e10 !e63
	AttrAudio.samplerate = 8000;//lint !e1013 !e10 !e63
	MEDIA_ATTR AttrVideo = {0};//lint !e522 !e10
	AttrVideo.rtpmap = stMediaKeyInfo.uPTVideo;//lint !e1013 !e10 !e63 !e734
	AttrVideo.type   = stMediaKeyInfo.enVideoDecoderType;//lint !e1013 !e10 !e63
	AttrVideo.samplerate = 90000;//lint !e1013 !e10 !e63
	pMediaBase->SetMeidaAttribute(AttrVideo, AttrAudio);

	iRet = IVS_SUCCEED;
	//�ڲ�ʵ����Կ�Ľ����Լ���ȡ��������Կ
	if(bUseSecretKey)
	{
		//BP_DBG_LOG("Need Secret Key.");
		iRet = pMediaBase->SetSecretKey(uiPort, stMediaKeyInfo);
        if (IVS_SUCCEED != iRet)
        {
            BP_DBG_LOG("Set Secret Key Is Fail.");
            pMediaBase->SetSecretFlag(false);
            return iRet;
        }
        BP_DBG_LOG("Set Secret Key Is Success.");
        pMediaBase->SetSecretFlag(true);
	}
    else
    {
        BP_DBG_LOG("No Need Secret Key.");
        iRet = pMediaBase->SetNoNeedSecretKey(uiPort, stMediaKeyInfo);
        if (IVS_SUCCEED != iRet)
        {
            BP_DBG_LOG("Set No Need Secret Key Is Fail.");
            pMediaBase->SetSecretFlag(false);
            return iRet;
        }
        BP_DBG_LOG("Set Secret Key Is Success.");
        pMediaBase->SetSecretFlag(true);
    }

    //�����Կ����ʧ�ܲ���ֱ�ӵ���StopPlay������ɱ���
	//if(IVS_SUCCEED != iRet)
	//{
    //  BP_RUN_LOG_ERR(IVS_FAIL,"Set Secret Key ","Set Secret Key Fail");
	//	pMediaBase->StopPlay();		
	//}
	return iRet;
}

IVS_INT32 CUserMgr::GetSameSuperDomainCode(const std::string& strDomainCode1,const std::string& strDomainCode2,
																std::string& strSuperDomainCode)
{
	return m_DomainRouteMgr.GetSameSuperDomainCode(strDomainCode1,strDomainCode2,strSuperDomainCode);
}

bool CUserMgr::IsInDomainTree(const std::string& strDomainCode)
{
	return m_DomainRouteMgr.IsInDomainTree(strDomainCode);
}


// ��ָ����������������
IVS_INT32 CUserMgr::SendCmd(CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp,IVS_INT32& iNeedRedirect, uint32_t uiTimeout, IVS_BOOL bTimeout)
{
    IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    // ��������Ϊ�գ�ֱ�ӷ��ͣ������Ƕ༶�����ض����
	std::string strDomainCode = sendNssMsgInfo.GetDomainCode();
	//std::string strCameraCode = sendNssMsgInfo.GetCameraCode();
	CLinkInfo oLinkIdInfo;
	/**************************************
	**��������Ϊ�գ���ʾ��ǰ����Ҫ�ض������
	***************************************/
	if (strDomainCode.empty())
	{
		if (!CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_iSessionID, m_strDomainCode, oLinkIdInfo))
		{
			BP_RUN_LOG_ERR(IVS_FAIL,"Send Cmd","strDomainCode is empty and GetSmuLinkIDInfoByDomainCode failed");
		    return IVS_FAIL;
		}
        // �ж�����״̬��������ڹر�״̬��ֱ�ӷ���ʧ��
        if (LINKID_STATUS_OPENED != oLinkIdInfo.GetLinkStatus())
        {
            BP_RUN_LOG_ERR(iRet,"Send Cmd","Link is closed");
            return IVS_NET_CONNECT_ERROR;
        }
		iRet = SendSynCmd(sendNssMsgInfo,oLinkIdInfo.GetStrLinkId(),strNSSRsp, uiTimeout, bTimeout);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(iRet,"Send Cmd","SendSynCmd failed");
		}
		return iRet;
	}
	/***************************************
	**���·�����Ҫ������������CameraCode��Ϊ��
	****************************************/

    //������������Ŀ����ʹ�����ӳ������Ҵ�����
    std::string strSendDomainCode;
    bool bRet = m_DomainRouteMgr.FindProxyDomainCode(strDomainCode, strSendDomainCode);
    // ���δ�ҵ�������ʹ��Ŀ�������;
    if (!bRet)
    {
        strSendDomainCode = strDomainCode;
    }
    // ������������LinkRouteMgr��SMU��LinkID
    bool bGetLinkRet = CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_iSessionID, strSendDomainCode, oLinkIdInfo);// �˴����ò���LinkRouteMgr�ĺ����� 
    if (!bGetLinkRet)
    {
        BP_RUN_LOG_ERR(bGetLinkRet, "send cmd,", "domain code:[%s] is not exist in route list cache.", strSendDomainCode.c_str());
    }

    if (LINKID_STATUS_ENABLED == oLinkIdInfo.GetLinkStatus())   // �������ɹ���oLinkIdInfo�����ó� LINKID_STATUS_OPENED
    {
        iRet = ProcrssEnableLinkNode(oLinkIdInfo);
    }

    if (LINKID_STATUS_OPENED == oLinkIdInfo.GetLinkStatus())  // ���õ������Ѵ��ڣ�����ֱ�ӷ�����Ϣ
    { 
        // ��������; 
		iRet = SendSynCmd(sendNssMsgInfo,oLinkIdInfo.GetStrLinkId(),strNSSRsp, uiTimeout, bTimeout);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(iRet,"Send Cmd","ON LinkStatus Send Cmd Error");
		}
		return iRet;
    }
    else
    {
        iRet = IVS_NET_CONNECT_ERROR;
    }

	iNeedRedirect = IVS_SUCCEED;
	return iRet;
}

IVS_INT32 CUserMgr::ProcrssEnableLinkNode(CLinkInfo& oLinkIdInfo)
{
    std::string strGeneralSmuLinkID;
    int iRet = m_loginObj.GeneralLoginSmu(oLinkIdInfo.GetStrDomainCode(), strGeneralSmuLinkID);
    if (IVS_SUCCEED == iRet || IVS_SMU_USER_FIRST_LOGIN == iRet)
    {
        // �ɹ���½������RoutLink״̬ΪOpen
        oLinkIdInfo.SetLinkStatus(LINKID_STATUS_OPENED);
        oLinkIdInfo.SetStrLinkId(strGeneralSmuLinkID);
        CLinkRouteMgr::instance().UpdateLinkInfo(m_iSessionID, oLinkIdInfo);
    }
    else
    {
        // ��½ʧ�ܣ��ŵ����������У�������RouteLink״̬ΪClosed
        oLinkIdInfo.SetLinkStatus(LINKID_STATUS_CLOSED);
        (void)CLinkRouteMgr::instance().UpdateLinkInfo(m_iSessionID, oLinkIdInfo);

        IVS_DOMAIN_ROUTE stDomainRouteInfo;
        memset(&stDomainRouteInfo, 0, sizeof(IVS_DOMAIN_ROUTE));
        iRet = GetDomainRouteMgr().GetDomainRoutebyCode(oLinkIdInfo.GetStrDomainCode(), stDomainRouteInfo);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(IVS_SDK_ERR_DOMAINCODE_NOT_EXIST, "General Login Smu", " failed");
            return IVS_SDK_ERR_DOMAINCODE_NOT_EXIST;
        }

        AddLinkIdToBusinessResumeList(strGeneralSmuLinkID, NET_ELE_SMU_NSS, stDomainRouteInfo.stIP.cIP, DEFAULT_SMU_PORT, TYPE_ADD_RELINK);

        iRet = IVS_NET_CONNECT_ERROR;
    }
    return iRet;
}

IVS_INT32 CUserMgr::SendRedirectServe(CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp)
{
	IVS_DEBUG_TRACE("");
	IVS_INT32 iRet = IVS_FAIL;
	// ��������Ϊ�գ�ֱ�ӷ��ͣ������Ƕ༶�����ض����
	std::string strDomainCode = sendNssMsgInfo.GetDomainCode();
	std::string strCameraCode = sendNssMsgInfo.GetCameraCode();

	//������������Ŀ����ʹ�����ӳ������Ҵ�����
	std::string strSendDomainCode;
	bool bRet = m_DomainRouteMgr.FindProxyDomainCode(strDomainCode, strSendDomainCode);
	// ���δ�ҵ�������ʹ��Ŀ�������;
	if (!bRet)
	{
		strSendDomainCode = strDomainCode;
	}
	//���¹���׼�������ض���
	IVS_DOMAIN_ROUTE stDomainRoute;
	memset(&stDomainRoute, 0, sizeof(IVS_DOMAIN_ROUTE));
	//��ȡ��·����Ϣ
	iRet = m_DomainRouteMgr.GetDomainRoutebyCode(strSendDomainCode, stDomainRoute);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Send Cmd", "Get DoMain Code Error");
		return iRet;
	}
	// ��������Ǵ���ֱ�ӷ���ʧ��;
	if (stDomainRoute.bIsAgent)
	{
		BP_RUN_LOG_ERR(iRet, "Send Cmd", "This Route is ProxyRoute");
		return IVS_FAIL;
	}
	// ��������ID��������ӳ��� �жϸ������Ƿ���Ҫ�ض���
	bRet = CToolsHelp::IsNeedReDirect(sendNssMsgInfo.GetReqID());
	if (!bRet)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Send Cmd", "Current ReqID with no need for ReDirect");
		return IVS_FAIL;
	}
	// ��������������ҵ���Ӧ��NvrCode;
	if (strCameraCode.empty())
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"Send Cmd","CameraCode Empty");
		return IVS_FAIL;
	}
	IVS_CAMERA_BRIEF_INFO stCameraBriefInfo;
	memset(&stCameraBriefInfo, 0, sizeof(IVS_CAMERA_BRIEF_INFO));
	//��ȡ���������Ӧ��nvr code
	iRet = m_DeviceMgr.GetCameraBriefInfobyCode(strCameraCode, stCameraBriefInfo);
	if(IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Send Cmd", "Can not find Camera Info,strCameraCode = %s", strCameraCode.c_str());
		return iRet;
	}

    IVS_CHAR szNvrCode[IVS_NVR_CODE_LEN + 1] = {0};
	if(!CToolsHelp::Memcpy(szNvrCode,IVS_NVR_CODE_LEN,stCameraBriefInfo.cNvrCode,IVS_NVR_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Send Cmd", "cNvrCode Memcpy error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	std::string strNvrCode = szNvrCode;

	// ����������NVRCode����LinkRouteMgr��SCU��LinkID
	//����ɹ���˵���Ѿ��������˷���
	iRet = SendCmdbyDomainNvrCode(strDomainCode, strNvrCode, sendNssMsgInfo, strNSSRsp);
	// ������ͳɹ���˵��SMU���ˣ������δ����Ǩ�ƣ����ڵ�¼ʱ����̨NVR��;
	if (IVS_SUCCEED == iRet)
	{
		BP_DBG_LOG("Send Cmd by Domain Nvr Code succeed. smu going down, camera do not transfer to other nvr.");
		return iRet;
	}
	// TODO������Ϊ1������ʧ�� 2������ʧ�� 3��IPC���ڸ�NVR�ϣ��ٷ����ض�������;
// 	if (IVS_NET_CONNECT_ERROR == iRet || IVS_NET_RECV_TIMEOUT == iRet 
// 		|| IVS_SCU_REDIRECTION_ERROR == iRet || IVS_SCU_DEV_NOT_IN_WHITE_LIST == iRet)
// 	{
// 		//��������ض���Ĺ��ܣ�����Ⱥ���߶ѵ�ģʽ�µ��ض���
// 		iRet = SendCmdbyReDirect(stDomainRoute, sendNssMsgInfo, strNSSRsp);
// 	}
	// ֻҪʧ������¶��������ض���,����������SCU��Ȩ��ͨ���ĵ�����������������ʧ���ˣ��ض�����ܻ�����ԭ���������ϣ�����
	// ���ܷ�������һ�ε�������������Ҳ�Ǳ������ mod by z00193167
	//��������ض���Ĺ��ܣ�����Ⱥ���߶ѵ�ģʽ�µ��ض���
	iRet = SendCmdbyReDirect(stDomainRoute, sendNssMsgInfo, strNSSRsp);

	return iRet;
}

// �����첽��Ϣ
IVS_INT32 CUserMgr::SendAsynMsg(const IVS_CHAR* pTargetDomainCode, 
                                                                IVS_UINT32 uiReqID, 
                                                                const std::string& strTransID,
                                                                CXml* pSendXm)
{
    CHECK_POINTER(pTargetDomainCode, IVS_PARA_INVALID);

    BP_RUN_LOG_INF("send asyn message", "target domain code:%s, reqid:%u", pTargetDomainCode, uiReqID);

    // ��⵱ǰ�Ƿ����Ŀ���������
    CLinkInfo linkInfo;
    bool bGetLinkRet = CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_iSessionID, pTargetDomainCode, linkInfo);
    if (!bGetLinkRet)
    {
        // ֻ�����򣬲Ŵ���CLinkRouteMgr��û��¼�ĳ���
        BP_RUN_LOG_INF("send asyn message", "fail to find domain code:%s in link route manager", pTargetDomainCode);
        //std::string strGeneralSmuLinkID;
        //int iCreateLinkRet = m_loginObj.GeneralLoginSmu(pTargetDomainCode, strGeneralSmuLinkID);
        //if (IVS_SUCCEED != iCreateLinkRet &&IVS_SMU_USER_FIRST_LOGIN != iCreateLinkRet)
        //{
        //    BP_RUN_LOG_ERR(iCreateLinkRet,"Send Cmd","LoginSmu failed");
        //    linkInfo.SetLinkStatus(LINKID_STATUS_CLOSED);
        //    IVS_DOMAIN_ROUTE stDomainRouteInfo;
        //    memset(&stDomainRouteInfo, 0, sizeof(IVS_DOMAIN_ROUTE));
        //    if (IVS_SUCCEED == GetDomainRouteMgr().GetDomainRoutebyCode(pTargetDomainCode, stDomainRouteInfo))
        //    {
        //        (void)AddLinkIdToBusinessResumeList(strGeneralSmuLinkID,NET_ELE_SMU_NSS,stDomainRouteInfo.stIP.cIP,DEFAULT_SMU_PORT,TYPE_ADD_RELINK);
        //    }
        //    return iCreateLinkRet;
        //}
        //linkInfo.SetLinkStatus(LINKID_STATUS_OPENED);
        //linkInfo.SetStrLinkId(strGeneralSmuLinkID);
    }

    if (LINKID_STATUS_ENABLED == linkInfo.GetLinkStatus())
    {
        (void)ProcrssEnableLinkNode(linkInfo);
    }

    // �����������״̬������ҵ��;
    if (LINKID_STATUS_OPENED != linkInfo.GetLinkStatus())
    {
        BP_RUN_LOG_ERR(IVS_NET_CONNECT_ERROR, "fail to send asyn message", "connect is closed");
        return IVS_NET_CONNECT_ERROR;
    }

    const IVS_CHAR* pXmlData = "";
    IVS_UINT32 uiLen = 0;
    if (NULL != pSendXm)
    {

        pXmlData = pSendXm->GetXMLStream(uiLen);
        if (NULL == pXmlData)
        {
            BP_RUN_LOG_ERR(IVS_PARA_INVALID, "send asyn message", "request xml is null");
            return IVS_PARA_INVALID;
        }
    }

    CNssXMLMsg nssXMLMsg(uiReqID, 0, strTransID.c_str(), 0);


    nssXMLMsg.SetXML(pXmlData, uiLen);

    ACE_Message_Block* pMsgBlock = NULL;
    HW_NEW(pMsgBlock,ACE_Message_Block); //lint !e774 !e831
    CHECK_POINTER(pMsgBlock, IVS_NULL_POINTER);
    IVS_INT32 iRet = nssXMLMsg.encode(pMsgBlock);
    if (IVS_SUCCEED != iRet && NULL != pMsgBlock)
    {
        (void)pMsgBlock->release();
        pMsgBlock = NULL;
        BP_RUN_LOG_ERR(iRet, "SendMsg", "Encode nssXMLMsg fail.");
        return iRet;
    }
    
    CMainCtrl* pMainCtrl = CMainCtrl::GetMainCtrl();
    CHECK_POINTER(pMainCtrl, IVS_NULL_POINTER);

    iRet = CDispatcher::instance().dispatcherOutWithoutTransaction(NET_ELE_SMU_NSS, linkInfo.GetStrLinkId().c_str(), strTransID.c_str(), uiReqID, pMsgBlock);
    if (IVS_SUCCEED != iRet && NULL != pMsgBlock)
    {
        BP_RUN_LOG_ERR(iRet, "SendMsg", "DispatcherMsgOut failed.");
        (void)pMsgBlock->release();
        pMsgBlock = NULL;
        return iRet;
    }
    return IVS_SUCCEED;
}

// ���첢����Cmd��strLinkIDָ�����͵�����
IVS_INT32 CUserMgr::SendSynCmd(CSendNssMsgInfo& sendNssMsgInfo,const std::string& strLinkID, std::string& strNSSRsp, uint32_t uiTimeout, IVS_BOOL bTimeout)
{
    IVS_DEBUG_TRACE("");

    // ��ӡ������;
    BP_DBG_LOG("request info NetElemType:%d, ReqID:%x, LinkID:%s", 
                sendNssMsgInfo.GetNetElemType(), 
                sendNssMsgInfo.GetReqID(),
                strLinkID.c_str());

    std::string strReqData = sendNssMsgInfo.GetReqData();
    PrintLog(strReqData);

	CCmd *pCmd = NULL;
    // ����Cmd�����ͽṹ��ʱ����ָ�����ȣ�����NSS��Ϣ����Ҫ
    if (0 != sendNssMsgInfo.GetReqLength() && NULL != sendNssMsgInfo.GetReqDataPointer())
    {
        pCmd = CNSSOperator::instance().BuildCmd(strLinkID, sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqDataPointer(), (int)sendNssMsgInfo.GetReqLength());
    }
    else
    {
        pCmd = CNSSOperator::instance().BuildCmd(strLinkID, sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqData().c_str());
    }
	CHECK_POINTER(pCmd,IVS_SDK_RET_BP_CMD_REQ_INVALID);

	//����nssЭ��
	IVS_INT32 iRet = IVS_FAIL;
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd, uiTimeout, bTimeout);
	CHECK_POINTER(pCmdRsp,IVS_NET_RECV_TIMEOUT);
	//const IVS_CHAR* pRspת��ΪXML��ת��Ϊ���ݽṹ
	if (TYPE_NSS_NOXML != sendNssMsgInfo.GetNeedXml())
	{
		BP_DBG_LOG("CUserMgr::SendSynCmd TYPE_NSS_XML OK");
        // ����������ֵiRet �� ������Ϣ�ķ�����iRetCode ��ֿ� modify by xiongfeng 00192614 2012-12-30
		IVS_CHAR* pRsp = CNSSOperator::instance().ParseCmd2XML(pCmdRsp,iRet);  
		//CHECK_POINTER(pRsp,iRetCode);
        if (NULL != pRsp)
        {
            strNSSRsp = pRsp;
            IVS_DELETE(pRsp,MUILI);

            // ��ӡ��Ӧ����;
            PrintLog(strNSSRsp);
        }
	}
	else
	{
		BP_DBG_LOG("CUserMgr::SendSynCmd TYPE_NSS_NOXML Ok");
		iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	}

	HW_DELETE(pCmdRsp);
	return iRet;
}//lint !e1746 !e1762

// �������Ӳ����Ƿ���ҵ��
IVS_INT32 CUserMgr::BuildLinkAndDoBusiness(const std::string& strDomainCode, const std::string& strNvrCode,
                                           CSendNssMsgInfo& sendNssMsgInfo, std::string &strNSSRsp)
{
	IVS_DEBUG_TRACE("");
	if (strDomainCode.empty() || strNvrCode.empty())
	{
		BP_RUN_LOG_ERR(IVS_FAIL,"Create Link and Send Cmd ","DomainCode or NvrCode is empty");
		return IVS_FAIL;
	}
	IVS_INT32 iRet = IVS_FAIL;
	//��scu��������
	// �������ӳɹ���linkID����LinkRouteMgrͳһ��LoginMgr����
	std::string strLinkID;
	iRet = m_loginObj.LoginScu(strDomainCode,strNvrCode, strLinkID);
	//���Ӳ��ɹ�
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"Create Link and Send Cmd ","Build Link Error");
		return iRet;
	}
	//����ҵ���������ýӿ�
	CSendNssMsgInfo sendNssInfo;
	sendNssInfo.SetReqID(sendNssMsgInfo.GetReqID());
	sendNssInfo.SetNeedXml(sendNssMsgInfo.GetNeedXml());
	sendNssInfo.SetNetElemType(NET_ELE_SCU_NSS);
	sendNssInfo.SetDomainCode(strDomainCode);
	sendNssInfo.SetCameraCode(sendNssMsgInfo.GetCameraCode());
	sendNssInfo.SetReqData(sendNssMsgInfo.GetReqData());
	//int iRetCode = IVS_FAIL;
	iRet = SendSynCmd(sendNssInfo,strLinkID,strNSSRsp);
	return iRet;
}

// ����������NVRCode����LinkRouteMgr��SCU��LinkID
IVS_INT32 CUserMgr::SendCmdbyDomainNvrCode(const std::string& strDomainCode, const std::string& strNvrCode,
										   CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp) 
{
	IVS_DEBUG_TRACE("");
    IVS_INT32 iRet = IVS_FAIL;
    CLinkInfo oLinkIdInfo;
    //memset(&stLinkIdInfo, 0, sizeof(LINKID_INFO));
    bool bRet = false;
    //��ȡscu��link��Ϣ
    bRet = CLinkRouteMgr::instance().GetScuLinkByDomainCodeAndNVRCode(m_iSessionID, strDomainCode, strNvrCode,
                                                                      oLinkIdInfo);
    // ���û���ҵ�linkID,��Ҫ��������
    if (!bRet)
    {
		BP_RUN_LOG_INF("Send Cmd by NVR","Can not find Link Info");
		//�������ӣ�������ҵ��
		iRet = BuildLinkAndDoBusiness(strDomainCode,strNvrCode,sendNssMsgInfo,strNSSRsp);
		return iRet;
    }
    // ����ҵ���linkID���жϵ�ǰLinkID�Ƿ�Ϊ����״̬
    if ( LINKID_STATUS_OPENED == oLinkIdInfo.GetLinkStatus() )//���ɺ�,
    {
        // ��������;����ҵ������
		CSendNssMsgInfo sendNssInfo;
		sendNssInfo.SetReqID(sendNssMsgInfo.GetReqID());
		sendNssInfo.SetNeedXml(sendNssMsgInfo.GetNeedXml());
		sendNssInfo.SetNetElemType(NET_ELE_SCU_NSS);
		sendNssInfo.SetDomainCode(strDomainCode);
		sendNssInfo.SetCameraCode(sendNssMsgInfo.GetCameraCode());
		sendNssInfo.SetReqData(sendNssMsgInfo.GetReqData());
		iRet = SendSynCmd(sendNssInfo,oLinkIdInfo.GetStrLinkId(),strNSSRsp);

	}
    return iRet;
}

// ���ض�������
IVS_INT32 CUserMgr::SendCmdbyReDirect(const IVS_DOMAIN_ROUTE& stDomainRoute, CSendNssMsgInfo& sendNssMsgInfo,std::string &strNSSRsp)
{
	IVS_DEBUG_TRACE("");
    int iRet = IVS_FAIL;
    IVS_DOMAIN_TYPE enDomainType = static_cast<IVS_DOMAIN_TYPE>(stDomainRoute.uiDomainType);
    // ������Ǽ�Ⱥ��ֱ�ӷ���ʧ��
    if (DOMAIN_TYPE_CLUSTER != enDomainType)
    {
		BP_RUN_LOG_ERR(IVS_SDK_NOT_CLUSTER_MODE, "Send Cmd by ReDirect","Current Mode is STACK");
        return IVS_SDK_NOT_CLUSTER_MODE;
    }
	
	IVS_CHAR szDomainCode[IVS_DOMAIN_CODE_LEN + 1];
	memset(szDomainCode, 0, IVS_DOMAIN_CODE_LEN + 1);
	if(!CToolsHelp::Memcpy(szDomainCode,IVS_DOMAIN_CODE_LEN  + 1,stDomainRoute.cDomainCode,IVS_DOMAIN_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Send Cmd by ReDirect", "DomainCode Memcpy error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    std::string strDomainCode = szDomainCode;

	IVS_DEVICE_BASIC_INFO stDeviceBasicInfo;
	memset(&stDeviceBasicInfo, 0, sizeof(IVS_DEVICE_BASIC_INFO));
    unsigned int uiNVRSize = m_DeviceMgr.GetNVRSize(strDomainCode);
	std::string strLinkID;
	std::string strNvrCode;

    for (unsigned int i = 0; i < uiNVRSize; i++)
    {
        iRet = m_DeviceMgr.GetFirstNVR(strDomainCode, stDeviceBasicInfo);
        if (IVS_SUCCEED != iRet)
        {
			BP_RUN_LOG_ERR(iRet, "Send Cmd by ReDirect","Get first NVR Error");
            return iRet;
        }
		//��ȡ�豸NVR Code
		strNvrCode = stDeviceBasicInfo.cCode;
		iRet = m_loginObj.LoginScu(strDomainCode,strNvrCode,strLinkID);
		//�����ǰ����û�гɹ���������������NVR��
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(iRet, "Send Cmd by ReDirect","Login Scu Error");
            continue;
        }
		//���ɻ�ȡ�������λָ��XML
		CXml xml;
		iRet = CLoginXMLProcess::GetFineMainDevXML(sendNssMsgInfo.GetCameraCode(),xml);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(iRet, "Send Cmd by ReDirect","Build Cmd Error");
			break;
		}
		unsigned int uiLen = 0;
		const char* pszData = xml.GetXMLStream(uiLen);
		CHECK_POINTER(pszData, IVS_OPERATE_MEMORY_ERROR);
		char *pReqData = IVS_NEW(pReqData,uiLen);
		if (NULL == pReqData)
		{
			BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "Send Cmd by ReDirect","Alloc ReqData Error");
			break;
		}
		if(!CToolsHelp::Memcpy(pReqData,uiLen,pszData,uiLen))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Send Cmd by ReDirect", "ReqData Memcpy error.");
			return IVS_ALLOC_MEMORY_ERROR;
		}

		//�����ض�λָ�� sendSysCmd();���ִ��ʧ��Ҳcontinue ����ɹ���ʾ�ҵ�����IPC���Ǹ�NVR��
		//�����������λ��Ϣָ��
		CSendNssMsgInfo stSendNssInfo;
		stSendNssInfo.SetReqID(NSS_CLUSTER_FIND_MAIN_DEV_REQ);
		stSendNssInfo.SetNeedXml(TYPE_NSS_XML);
		stSendNssInfo.SetNetElemType(NET_ELE_SCU_NSS);
		stSendNssInfo.SetDomainCode(strDomainCode);
		stSendNssInfo.SetCameraCode(sendNssMsgInfo.GetCameraCode());
		stSendNssInfo.SetReqData(pReqData);
		iRet = SendSynCmd(stSendNssInfo,strLinkID,strNSSRsp);
		if (IVS_SUCCEED != iRet)
		{
			//��ǰ��λʧ��
			BP_RUN_LOG_ERR(iRet, "Send Cmd by ReDirect","Find Camera Error");
			continue;
		}
		//�����ض�λ����������������ҵ��������Ϣ
		//���ɽ��XML
		CXml retXml;
		if (!retXml.Parse(strNSSRsp.c_str()))
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "Send Cmd by ReDirect","Parse XML Error");
			continue;
		}
		char szNvrCode[IVS_NVR_CODE_LEN + 1];
		memset(szNvrCode,0,IVS_NVR_CODE_LEN + 1);
		iRet = CLoginXMLProcess::ParseNvrCodeXML(retXml,szNvrCode);
		if (IVS_SUCCEED != iRet)
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "Send Cmd by ReDirect","Get NVRCode Error");
			break;
		}
		//��ȡ���µ�nvrCode,�����µ�����
		std::string strNewNvrCode = szNvrCode;
        iRet = BuildLinkAndDoBusiness(strDomainCode,strNewNvrCode,sendNssMsgInfo,strNSSRsp);
        if (IVS_SUCCEED == iRet )
        {
            IVS_CAMERA_BRIEF_INFO stCameraBriefInfo;
            memset(&stCameraBriefInfo, 0, sizeof(IVS_CAMERA_BRIEF_INFO));
            IVS_INT32 iRetCamera = m_DeviceMgr.GetCameraBriefInfobyCode(sendNssMsgInfo.GetCameraCode(), stCameraBriefInfo);
            if (IVS_SUCCEED == iRetCamera)
            {
                BP_RUN_LOG_INF("Send Cmd by ReDirect","Modify Camera Info");
                m_DeviceMgr.ModifyDevStatusByDomainCode(strDomainCode,sendNssMsgInfo.GetCameraCode(),stCameraBriefInfo.uiStatus, strNewNvrCode);
            }
        }
		//ִ�е������ʾ�����Ƿ���ҵ��ɹ�������
		break;
	}
    return iRet;
}
#ifdef WIN32
IVS_INT32 CUserMgr::GetVideoNumAndResource(const IVS_CHAR* pCameraCode,IVS_BOOL* bReachLimit,
								 IVS_BOOL* bWhetherCited)
{
	IVS_INT32 iRet = IVS_FAIL;
	std::string strCameraCode;
	std::string strDomainCode;
	iRet = CToolsHelp::SplitCameraDomainCode(pCameraCode, strCameraCode, strDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get ReqNumber And ReqRSP XML", "SplitCameraCode failed, RetCode=%d.", iRet);
		return iRet;
	}
	CXml xmlReq;
	iRet = CMediaXMLProcess::VideoNumAndResourceGetXML(strCameraCode, xmlReq); //lint !e838
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get ReqNumber And ReqRSP XML", "get xml false");
		return IVS_FAIL;
	}
	unsigned int xmlLen  = 0;
	const IVS_CHAR* pReq = xmlReq.GetXMLStream(xmlLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
	BP_DBG_LOG("Get Video ReqNumber And ReqRSP xml = %s.", pReq);
	char cLocalDomainCode[IVS_DOMAIN_CODE_LEN + 1];
	memset(cLocalDomainCode, 0, IVS_DOMAIN_CODE_LEN + 1);
	bool bRet = CToolsHelp::Memcpy(cLocalDomainCode, IVS_DOMAIN_CODE_LEN + 1, strDomainCode.c_str(),
		IVS_DOMAIN_CODE_LEN);
	if (false == bRet)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Get ReqNumber And ReqRSP XML", "Memcpy error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	CSendNssMsgInfo sendNssMsgInfo;
	sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
	sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
	sendNssMsgInfo.SetReqID(NSS_GET_VIDEO_NUM_AND_RESOURCE_REQ);
	sendNssMsgInfo.SetReqData(pReq);
	sendNssMsgInfo.SetCameraCode(strCameraCode);
	sendNssMsgInfo.SetDomainCode(strDomainCode);
	iRet = IVS_FAIL;
	std::string strpRsp;
	IVS_INT32 iNeedRedirect = IVS_FAIL;
	// ���Ͳ���ʧ�ܣ�ֱ�ӷ���
	iRet = SendCmd(sendNssMsgInfo, strpRsp,iNeedRedirect);
	// NSS��Ϣ��������󣬲��������ݣ�ֱ�ӷ��ش�����
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get URL", "SendCmd operation succeed,rspCode = %d", iRet);
		return iRet;
	}
	//��������
	CXml xmlRsp;
	(void)xmlRsp.Parse(strpRsp.c_str());
	IVS_INT32 iReachLimit = 0;
	IVS_INT32 iWhetherCited = 0;
	iRet = CMediaXMLProcess::ParseVideoNumAndResource(xmlRsp, iReachLimit, iWhetherCited);
	*bReachLimit = (IVS_BOOL)iReachLimit;
	*bWhetherCited = (IVS_BOOL)iWhetherCited;
	return iRet;
}//lint !e818 !e1762
#endif
// �����Զ��ָ���ʱ��
void CUserMgr::StartResumeTimer()
{
	IVS_INFO_TRACE("");
	m_ResumeTimerInst.init(100);
	m_ResumeTimerInst.run();
	m_bAutoResume = true;
	try
	{
		// 		if ( !m_AutoResume.IsStart() )
		// 		{
		//������ʱ�� 15S
		//m_AutoResume.SetStart();
		m_ResumeTimerInst.registerTimer(&m_AutoResume, NULL, 150, enRepeated);
		//	}
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "AutoResume registerTimer failed", "NA");
		return;
	}

	m_CloseLinkIDInst.init(100);
	m_CloseLinkIDInst.run();
	m_bCloseLinkIDCount = true;
	try
	{
		//ÿ1����һ��
		m_CloseLinkIDInst.registerTimer(&m_CloseLinkIDCount, NULL, 10, enRepeated);
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "CloseLinkIDCount registerTimer failed", "NA");
		return;
	}

	m_RelinkInst.init(100);
	m_RelinkInst.run();
	m_bRelink = true;
	try
	{
		//ÿ30����һ��
		m_RelinkInst.registerTimer(&m_Relink, NULL, 100, enRepeated);

	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "Relink registerTimer failed", "NA");
		return;
	}
}

// ֹͣ��ʱ��
void CUserMgr::StopResumeTimer()
{
	m_AutoResume.clearCamerInfoList();
	m_bAutoResume = false;
	m_ResumeTimerInst.cancelTimer(&m_AutoResume);
	m_ResumeTimerInst.clearTimer();
	m_ResumeTimerInst.exit();
	
	if (m_bCloseLinkIDCount)
	{
		m_bCloseLinkIDCount = false;
		m_CloseLinkIDInst.cancelTimer(&m_CloseLinkIDCount);
		m_CloseLinkIDInst.clearTimer();
		m_CloseLinkIDInst.exit();
	}
	
	if (m_bRelink)
	{
		m_bRelink = false;
		m_RelinkInst.cancelTimer(&m_Relink);
		m_RelinkInst.clearTimer();
		m_RelinkInst.exit();
	}
}

// ����LinkID��������
void CUserMgr::AddLinkIdToBusinessResumeList(const std::string& strLinkID,int iNetElemType,const std::string& strIP,int iPort,int iAddType)
{
	IVS_DEBUG_TRACE("iNetElemType: %d, iAddType: %d, strLinkID: %s, strIP: %s", 
		iNetElemType, iAddType, strLinkID.c_str(), strIP.c_str());
    // ��¼��SMU���ӽ�����������
	//modify by zwx2118311, Date:20140618, ����ָ���пղ��� 
	CUserInfo *pUserInfo = m_loginObj.GetUserInfo();
	if (NULL == pUserInfo)
	{
		BP_RUN_LOG_WAR("GetServerIP Failed.", "pUserInfo is NULL");
		return;
	}
	std::string strSerIP = pUserInfo->GetServerIP();
	//end

    if (NET_ELE_SMU_NSS == iNetElemType)
    {
        if (TYPE_ADD_CLOSECOUNT == iAddType)
        {
            m_CloseLinkIDCount.AddLinkIdToBusinessList(strLinkID,iNetElemType,strIP,iPort);
        }
        else
        {
            // ɾ�����ӣ���ֹͨ�Ų�����
            if (!strLinkID.empty())
            {
                CNetFactory::Instance().DeleteClientLink(strLinkID, iNetElemType);
            }
            
			// ������һ����Ϣ��ȥ
			if (0 == strSerIP.compare(strIP))
			{
				// ��һ������ʧ����Ϣ,��ӵ��ص�¼����
				if(NULL!=m_fnEventCallBack)
				{
					m_fnEventCallBack(IVS_EVENT_KEEP_ALIVE_FAILED, NULL, 1, m_pUserParam);
					BP_RUN_LOG_INF("Keep Alive Failed CallBack End.", "SessionID: %d, IP: %s, Port: %d", m_iSessionID, strIP.c_str(), iPort);
				}
				else
				{
					BP_RUN_LOG_WAR("Keep Alive Failed CallBack Failed.", "m_fnEventCallBack is NULL");
				}
			}
            if (IVS_SUCCEED == m_Relink.AddLinkIdToBusinessList(strLinkID,iNetElemType,strIP,iPort))
            {
                BP_RUN_LOG_INF("resume change notify,", "add resume item, element type:[%d], target ip:[%s:%u].", iNetElemType, strIP.c_str(), iPort);
            }
        }
    }
    // ���OMU������������
    else if (NET_ELE_OMU_NSS == iNetElemType)
    {
        if (!strLinkID.empty())
        {
            CNetFactory::Instance().DeleteClientLink(strLinkID, iNetElemType);
        }
        if (IVS_SUCCEED == m_Relink.AddLinkIdToBusinessList(strLinkID,iNetElemType,strIP,iPort))
        {
            BP_RUN_LOG_INF("resume change notify,", "add resume item, element type:[%d], target ip:[%s:%u].", iNetElemType, strIP.c_str(), iPort);
        }
    }

	return;
}

// ���������Ƴ�LinkID
//void CUserMgr::RemoveLinkIDfromResumeList(const std::string& strLinkID,int iNetElemType,const std::string& strIP,int iPort,int iAddType)
//{
//    std::string strSerIP = m_loginObj.GetUserInfo()->GetServerIP();
//    if (NET_ELE_SMU_NSS == iNetElemType && 0 == strSerIP.compare(strIP))
//    {
//	    if (TYPE_ADD_CLOSECOUNT == iAddType)
//	    {
//		    m_CloseLinkIDCount.RemoveLinkIdFromBusinessList(strLinkID);
//	    }
//	    else
//	    {
//		    m_Relink.RemoveLinkIdFromBusinessList(strLinkID);
//	    }
//    }
//}

// ѭ����ӡ��־����BP��ӡ�ĵ�����־���ܳ���2048
void CUserMgr::PrintLog(const std::string& strLog) const
{
    // ʹ��BP��ӡ�����ĺͽ�������,BP�ĵ�����־����Ϊ2048����ȥ�ļ��������ڵ���Ϣ��ʣ��1930���ҵĳ���;
    static const unsigned int s_uiLogLen = 1930;
    std::string strLogData = strLog;
    while (strLogData.length() > s_uiLogLen)
    {
        std::string strOutputStr = strLogData.substr(0, s_uiLogLen);
        // ��ӡ������;
        BP_DBG_LOG("%s", strOutputStr.c_str());

        strLogData = strLogData.substr(s_uiLogLen, strLogData.length() - 1);
    }

    // ��ӡʣ���������;
    BP_DBG_LOG("%s", strLogData.c_str());
}

// ֪ͨ������״̬�¼�
#ifdef WIN32
IVS_INT32 CUserMgr::NotifyStreamBufferState(IVS_ULONG ulPlayHandle, IVS_INT32 iBufferState)
{
    IVS_DEBUG_TRACE("");
    IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_INVALID;
    IVS_INT32 iRet = GetServiceTypebyPlayHandle(ulPlayHandle, enServiceType);

    // ��Ҫ���ڿͻ������أ������غͻط�ҵ���漰����������
    switch (enServiceType)
    {
    case SERVICE_TYPE_DOWNLOAD:
        break;
    case SERVICE_TYPE_PLAYBACK:
	case SERVICE_TYPE_BACKUPRECORD_PLAYBACK:
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK:
	case SERVICE_TYPE_PU_PLAYBACK:
        iRet = m_PlaybackMgr.NotifyStreamBufferState(ulPlayHandle, iBufferState);
        break;
    case SERVICE_TYPE_PU_DOWNLOAD:
        break;
    case SERVICE_TYPE_REALVIDEO:
    case SERVICE_TYPE_RECORD:
    case SERVICE_TYPE_AUDIO_CALL:
    case SERVICE_TYPE_AUDIO_BROADCAST:
    default:
        BP_DBG_LOG("Service Type %d, PlayHandle %d", enServiceType, ulPlayHandle);
        break;
    }
    return iRet;
}

IVS_VOID CUserMgr::StopNotifyBufEvent(IVS_ULONG ulPlayHandle)
{
	IVS_DEBUG_TRACE("");
	IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_INVALID;
	(void)GetServiceTypebyPlayHandle(ulPlayHandle, enServiceType);

	// ��Ҫ���ڿͻ������أ������غͻط�ҵ���漰����������
	switch (enServiceType)
	{
	case SERVICE_TYPE_DOWNLOAD:
		break;
	case SERVICE_TYPE_PLAYBACK:
	case SERVICE_TYPE_BACKUPRECORD_PLAYBACK:
	case SERVICE_TYPE_DISASTEBACKUP_PLAYBACK:
	case SERVICE_TYPE_PU_PLAYBACK:
		m_PlaybackMgr.StopNotifyBufEvent(ulPlayHandle);
		break;
	case SERVICE_TYPE_PU_DOWNLOAD:
		break;
	case SERVICE_TYPE_REALVIDEO:
	case SERVICE_TYPE_RECORD:
	case SERVICE_TYPE_AUDIO_CALL:
	case SERVICE_TYPE_AUDIO_BROADCAST:
	default:
		BP_DBG_LOG("Service Type %d, PlayHandle %d", enServiceType, ulPlayHandle);
		break;
	}
}
#endif
IVS_INT32 CUserMgr::RecollectCameraDomainCodes2Map(IVS_UINT32 uiCameraNum,const IVS_DEVICE_CODE* pCameraCodeList, DeviceCodeMap& deviceCodeMap)
{
	IVS_INT32 iRet = IVS_SUCCEED;
	deviceCodeMap.clear();
	//1����ȡ����룬�жϴ�����
	for (IVS_UINT32 i = 0; i < uiCameraNum; i++)
	{
		IVS_CHAR chDevCode[IVS_DEV_CODE_LEN+1] = {0};
		IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};
		iRet += CXmlProcess::ParseDevCode(pCameraCodeList->cDevCode, chDevCode, chDomaCode);

		std::string strDomainCode = chDomaCode;
		//������������Ŀ����ʹ�����ӳ������Ҵ�����;
		std::string strSendDomainCode;
		bool bRet = m_DomainRouteMgr.FindProxyDomainCode(strDomainCode, strSendDomainCode);
		// ���δ�ҵ�������ʹ��Ŀ�������;
		if (!bRet)
		{
			strSendDomainCode = strDomainCode;
		}

		// �ж�strSendDomainCode�Ƿ��Ѿ���map��;
		DeviceCodeMap::iterator iter = deviceCodeMap.find(strSendDomainCode);
		if (iter == deviceCodeMap.end())
		{
			DeviceCodeList deviceCodeList;

			deviceCodeList.push_back(pCameraCodeList);
			(void)deviceCodeMap.insert(std::make_pair(strSendDomainCode, deviceCodeList));
		}
		else
		{
			DeviceCodeList deviceCodeList = static_cast<DeviceCodeList>(iter->second); //lint !e612 !e69
			deviceCodeList.push_back(pCameraCodeList);
			deviceCodeMap.erase(iter);
			(void)deviceCodeMap.insert(std::make_pair(strSendDomainCode, deviceCodeList));
		}

		pCameraCodeList++;
	}

	if (IVS_SUCCEED != iRet)
	{
		iRet = IVS_FAIL;
	}
	return iRet;
}

CCmd * CUserMgr::BuildCmd(CSendNssMsgInfo& sendNssMsgInfo)
{
	IVS_DEBUG_TRACE("");
	// IVS_INT32 iRet = IVS_FAIL;
	// ��������Ϊ�գ�ֱ�ӷ��ͣ������Ƕ༶�����ض����
	std::string strDomainCode = sendNssMsgInfo.GetDomainCode();
	//std::string strCameraCode = sendNssMsgInfo.GetCameraCode();
	CLinkInfo oLinkIdInfo;

	//������������Ŀ����ʹ�����ӳ������Ҵ�����
	std::string strSendDomainCode;
	bool bRet = m_DomainRouteMgr.FindProxyDomainCode(strDomainCode, strSendDomainCode);
	// ���δ�ҵ�������ʹ��Ŀ�������;
	if (!bRet)
	{
		strSendDomainCode = strDomainCode;
	}
	// ������������LinkRouteMgr��SMU��LinkID
	bool bGetLinkRet = CLinkRouteMgr::instance().GetSmuLinkIDInfoByDomainCode(m_iSessionID, strSendDomainCode, oLinkIdInfo);// �˴����ò���LinkRouteMgr�ĺ����� 
	if (!bGetLinkRet)
	{
        BP_RUN_LOG_ERR(bGetLinkRet, "Build cmd,", "domain code:[%s] is not exist in route list cache.", strSendDomainCode.c_str());
		// ��������;
		//std::string strGeneralSmuLinkID;
		//int iCreateLinkRet = m_loginObj.GeneralLoginSmu(strSendDomainCode, strGeneralSmuLinkID);
		//// BP�������ܷ���-1�Ĵ����룬���º������ͳ�ʱ�����س�ʱ�����룻���ÿ��ǵ�һ�ε�¼������
 	//	if (IVS_SUCCEED != iCreateLinkRet && IVS_SMU_USER_FIRST_LOGIN != iCreateLinkRet)
 	//	{
 	//		BP_RUN_LOG_ERR(iCreateLinkRet,"Build Cmd","LoginSmu failed");
  //          IVS_DOMAIN_ROUTE stDomainRouteInfo;
  //          memset(&stDomainRouteInfo, 0, sizeof(IVS_DOMAIN_ROUTE));
  //          if (IVS_SUCCEED == GetDomainRouteMgr().GetDomainRoutebyCode(strDomainCode, stDomainRouteInfo))
  //          {
  //              (void)AddLinkIdToBusinessResumeList(strGeneralSmuLinkID,NET_ELE_SMU_NSS,stDomainRouteInfo.stIP.cIP,stDomainRouteInfo.uiPort,TYPE_ADD_RELINK);
  //          }

 	//		return NULL;
 	//	}
		//// ��������;
		//if (0 != sendNssMsgInfo.GetReqLength() && NULL != sendNssMsgInfo.GetReqDataPointer())
		//{
		//	return CNSSOperator::instance().BuildCmd(oLinkIdInfo.GetStrLinkId(), sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqDataPointer(), (int)sendNssMsgInfo.GetReqLength());
		//}
		//else
		//{
		//	return CNSSOperator::instance().BuildCmd(oLinkIdInfo.GetStrLinkId(), sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqData().c_str());
		//}
	}

    if (LINKID_STATUS_ENABLED == oLinkIdInfo.GetLinkStatus())
    {
        (void)ProcrssEnableLinkNode(oLinkIdInfo);
    }

	if (LINKID_STATUS_OPENED == oLinkIdInfo.GetLinkStatus())  	// �����������״̬��ֱ�ӷ���ҵ��;
	{
		// ����Cmd�����ͽṹ��ʱ����ָ�����ȣ�����NSS��Ϣ����Ҫ
		if (0 != sendNssMsgInfo.GetReqLength() && NULL != sendNssMsgInfo.GetReqDataPointer())
		{
			return CNSSOperator::instance().BuildCmd(oLinkIdInfo.GetStrLinkId(), sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqDataPointer(), (int)sendNssMsgInfo.GetReqLength());
		}
		else
		{
			return CNSSOperator::instance().BuildCmd(oLinkIdInfo.GetStrLinkId(), sendNssMsgInfo.GetNetElemType(), sendNssMsgInfo.GetReqID(), sendNssMsgInfo.GetReqData().c_str());
		}
	}

	BP_RUN_LOG_ERR(IVS_FAIL,"Build Cmd","Build Cmd Failed. link status close");
	return NULL;
}

// ����ˮӡ������Ϣ
void CUserMgr::SetWaterMarkInfo(IVS_ULONG ulPlayHandle, ST_WATERMARK_ALARM_INFO& stWaterMarkAlarmInfo)
{
    BP_DBG_LOG("CUserMgr::SetWaterMarkInfo, ulPlayHandle:%lu.", ulPlayHandle);

    CLockGuard lock(m_pWaterMarkInfoMutex);
    MAP_WATERMARK_ALARM_INFO_ITOR itor = m_mapWaterMarkInfo.find(ulPlayHandle);
    if (itor != m_mapWaterMarkInfo.end())
    {
        m_mapWaterMarkInfo.erase(itor++);
    }

    (void)m_mapWaterMarkInfo.insert(std::make_pair(ulPlayHandle, stWaterMarkAlarmInfo));

    BP_DBG_LOG("CUserMgr::SetWaterMarkInfo, ulPlayHandle:%d, cCameraCode:%s, cStartTime:%s, ullAlarmEventID:%d",
               ulPlayHandle, stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, 
               stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID);
}

// ��ȡˮӡ������Ϣ
void CUserMgr::GetWaterMarkInfo(IVS_ULONG ulPlayHandle, ST_WATERMARK_ALARM_INFO &stWaterMarkAlarmInfo)
{ 
    BP_DBG_LOG("CUserMgr::GetWaterMarkInfo, ulPlayHandle:%lu.", ulPlayHandle);

    CLockGuard lock(m_pWaterMarkInfoMutex);
    MAP_WATERMARK_ALARM_INFO_ITOR itor = m_mapWaterMarkInfo.find(ulPlayHandle);
    if (itor != m_mapWaterMarkInfo.end())
    {
        memcpy(&stWaterMarkAlarmInfo, &(itor->second)/*&(itor->second.stWaterMarkInfo)*/, sizeof(stWaterMarkAlarmInfo));//lint !e826
    }

    BP_DBG_LOG("CUserMgr::GetWaterMarkInfo, cCameraCode:%s, cStartTime:%s, ullAlarmEventID:%d",
        stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, 
        stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID);
}

void CUserMgr::RemoveWaterMarkInfoFromMap(IVS_ULONG ulPlayHandle)
{
    CLockGuard lock(m_pWaterMarkInfoMutex);
    MAP_WATERMARK_ALARM_INFO_ITOR itor = m_mapWaterMarkInfo.find(ulPlayHandle);
    if (itor != m_mapWaterMarkInfo.end())
    {
        m_mapWaterMarkInfo.erase(itor++);
    }
}


//�����Ƿ����OMU�豸��澯
void CUserMgr::SetReceiveOMUAlarm(IVS_BOOL bReceive)
{
	IVS_DEBUG_TRACE("SetReceiveOMUAlarm bReceive:%d", bReceive);

	if (!bReceive)
	{
		m_bReceiveOMUAlarm = 0;
		BP_RUN_LOG_INF("SetReceiveOMUAlarm False", "bReceive: %d", bReceive);
	}
	else
	{
		m_bReceiveOMUAlarm = 1;
	}
}


//����������ʼʱ��
void CUserMgr::UpdateResumeStartTime(const time_t &ltime)
{
	IVS_DEBUG_TRACE("UpdateSSOResumeStartTime, ltime: %d", ltime);

	if (m_ResumeStartTime <= (time_t)0 || (time_t)0 == ltime)
	{
		m_ResumeStartTime = ltime;
		BP_RUN_LOG_INF("Update SSO Resume Start Time", "SSOResumeStartTime is: %d", m_ResumeStartTime);
	}

	return;
}