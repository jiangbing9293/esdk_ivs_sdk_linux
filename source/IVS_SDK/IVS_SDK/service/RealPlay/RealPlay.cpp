#include "RealPlay.h"
#include "bp_log.h"
#include "IVS_Player.h"
#include "LockGuard.h"
#include "IVS_Trace.h"
#ifdef WIN32
#include "AutoResume.h"
#endif
#include "UserMgr.h"

//TODO �쳣����Ҫ�����ȥ����Ϊrtspclient���쳣�ص�����ֻ��һ���������������쳣����Ҳֻ��һ����
// �����Ƿ�����rtsp��������������ÿ��ͨ����һ���쳣�ص�����;

// RTSP �쳣�ص�����
//static void __SDK_CALL g_fRtspExceptionCallback(int handle, unsigned int ulMsgType,unsigned int ulErrCode, const void *pParameter, void *pUser)//lint !e830  !e528
//{
//    if ((RTSP_METHOD_ANNOUNCE == ulMsgType) && (0 == ulErrCode))
//    {
//        // 1������SDP��Ϣ;
//        //char szVideoSecretKey[KEY_MAX_LEN] = {0,};
//        //char szAudioSecretKey[KEY_MAX_LEN] = {0,};
//        //ULONG ulVideoKeyType = 0;
//        //ULONG ulAudioKeyType = 0;
//        //MLIB_MEDIA_INFO stMediaInfo;
//        //memset(&stMediaInfo, 0x0, sizeof(MLIB_MEDIA_INFO));
//        //stMediaInfo.ManuType = 101;
//        //ULONG ulRet = parseSdp((char*)pParameter, 
//        //                        &ulVideoKeyType, szVideoSecretKey, 
//        //                        &ulAudioKeyType, szAudioSecretKey,
//        //                        &stMediaInfo);        //lint !e732
//        
//        // 2��
//
//    }
//
//}//lint !e818 

// Rtspý�����ݻص�
static void  __SDK_CALL g_fRtspDataCallBack(long handle, 
                                            char *pBuffer,
                                            unsigned int ulBufSize,
                                            void *pUser)//lint !e528 
{
    //�����Ч�Լ��
    if ((NULL == pBuffer) 
        || (0 == ulBufSize)
        || (NULL == pUser))
    {
        return;
    }


    if (*pBuffer != '$')
    {
        return;
    }
    if (1 == *(pBuffer+1) || 3 == *(pBuffer+1))
    {
        // RTCP����������ַ�����ȥ
        //(void)IVS_InputData(ulPort, NULL, 0);
        return;
    }

    //��ȡ�ص������������;
    CMediaBase* pMediaBase = (CMediaBase*)pUser;
    unsigned long uiPlayerChannel = pMediaBase->GetPlayerChannel();
    if (0 == uiPlayerChannel)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"g_fRtsp Data Call Back", "pMediaBase's uiPlayerChannel = 0.");
        return;
    }

    pBuffer   += 4;
    ulBufSize -= 4;
    IVS_PLAY_InputRtpData(uiPlayerChannel, pBuffer, ulBufSize);
    return;
} //lint !e818

static void __SDK_CALL g_fNetDataCallBack( char *pBuf, unsigned int uiSize, void *pUser)//lint !e818 !e830
{
    //��ȡ�ص������������;
    CMediaBase* pMediaBase = reinterpret_cast<CMediaBase*>(pUser);
    if (NULL == pMediaBase)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"g_fNet Data Call Back", "transform pUser to MediaBase pointer failed.");
        return;
    }
    unsigned long uiPlayerChannel = pMediaBase->GetPlayerChannel();
    if (0 == uiPlayerChannel)
    {
        BP_RUN_LOG_ERR(IVS_FAIL,"g_fNet Data Call Back", "pMediaBase's uiPlayerChannel = 0.");
        return;
    }

    (void)IVS_PLAY_InputRtpData(uiPlayerChannel, pBuf, uiSize);
    return;
}//lint !e818

// �������ص�;
static IVS_VOID __SDK_CALLBACK g_fReassmCallBack(IVS_RAW_FRAME_INFO* pstRawFrameInfo,
                                           IVS_CHAR*           pFrame,
                                           IVS_UINT32          uFrameSize,
                                           REC_PACKET_INFO*    /*pstRecPacketInfo*/,
                                           IVS_VOID*       pUser)
{
    CRealPlay* pRealPlay = static_cast<CRealPlay*>(pUser);
    if (NULL == pRealPlay)
    {
        return;
    }

    pRealPlay->DoRawFrameCallBack(pstRawFrameInfo, pFrame, uFrameSize);
}

// ����ص������ؽ��������;
static IVS_VOID __SDK_CALLBACK g_fDecoderCallBack(IVS_INT32   iStreamType,
                                            IVS_VOID*   pFrameInfo,
                                            IVS_VOID*   pUser)
{
    CRealPlay* pRealPlay = static_cast<CRealPlay*>(pUser);
    if (NULL == pRealPlay)
    {
        return;
    }

    pRealPlay->DoFrameCallBack(iStreamType, pFrameInfo);
}

CRealPlay::CRealPlay(void)
{
    m_pRealPlayRawCBMutex = VOS_CreateMutex();
    m_pRealPlayFrameDecoderCBMutex = VOS_CreateMutex();

    CMediaBase::RetSet();
    RetSet();//lint !e1506

	m_RefCounter.counter = 0;
	m_nStatus = STATUS_READY;
}


CRealPlay::~CRealPlay(void)
{
    try
    {
        VOS_DestroyMutex(m_pRealPlayRawCBMutex);
        m_pRealPlayRawCBMutex = NULL;
        VOS_DestroyMutex(m_pRealPlayFrameDecoderCBMutex);
        m_pRealPlayFrameDecoderCBMutex = NULL;
	    RetSet(); //lint !e1506�Ѿ�ʵ�����麯��;
    }
    catch(...)
    {

    }
}


void CRealPlay::RetSet()
{
	IVS_DEBUG_TRACE("");
	CMediaBase::RetSet();
#ifdef WIN32
	m_WndList.clear();
#endif
    m_fRealPlayCallBackRaw = NULL;
    m_pRealPlayUserDataRaw = NULL;
    m_fRealPlayCallBackFrame = NULL;
    m_pRealPlayUserDataFrame = NULL;
    memset(&m_CameraPlayInfo, 0, sizeof(CAMERA_PLAY_INFO));
}

int CRealPlay::ReStartRealPlay(const IVS_MEDIA_PARA* pMediaPara, const IVS_CHAR* pCameraCode)
{
	IVS_DEBUG_TRACE("");

    CLockGuard lock(m_pMediaMutex);

    /*
	 * EvenetMgr ���ƿ��������߳�stop֮��
	 * ��CRealPlay���������������
	 * �����ж����߳��Ƿ��Ѿ��رձ�ͨ��
	 */
	if (STATUS_FINISHED == m_nStatus || STATUS_PLAYING == m_nStatus)
	{
		return IVS_PARA_INVALID;
	}

	IVS_PLAY_ResetData(m_ulPlayerChannel); 
	IVS_PLAY_SetResetFlag(m_ulPlayerChannel, false);

	START_PLAY_PARAM stParam;
	stParam.enServiceType = SERVICE_TYPE_REALVIDEO;
	stParam.cbRtspDataCallBack = g_fRtspDataCallBack;
	stParam.cbNetDataCallBack = g_fNetDataCallBack;
	//��ʼʵ��;
	int iRet = ReStartStream(pMediaPara, pCameraCode, stParam);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"ReStart Real Play", "Start play error.");
	}

	return iRet;
}
#ifdef WIN32
int CRealPlay::StartRealPlay(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode,HWND hWnd)
{
	IVS_DEBUG_TRACE("");
#if 0
	if(m_iStartType == PLAY_TYPE_HWND && NULL == hWnd)
	{
        BP_RUN_LOG_ERR(IVS_SDK_ERR_FINDVIDEOHANDLE_FAILED,"Start Real Play", "hwnd invalid");
        return IVS_PARA_INVALID;
	}
#endif 
    CLockGuard lock(m_pMediaMutex);

    START_PLAY_PARAM stParam;
    stParam.enServiceType = SERVICE_TYPE_REALVIDEO;
    stParam.cbRtspDataCallBack = g_fRtspDataCallBack;
    stParam.cbNetDataCallBack = g_fNetDataCallBack;
	//��ʼʵ��;
	int iRet = StartPlay(pRealplayPara, pCameraCode, stParam);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start Real Play", "Start play error.");
        return iRet;
    }

    iRet = IVS_PLAY_SetPlayWindow(m_ulPlayerChannel, hWnd);
    // �����ڲ��뵽�����һ��;
    if (IVS_SUCCEED == iRet)
    {
		// ����ʵ�����������ڻָ�, add by zwb
		memset(&m_CameraPlayInfo, 0, sizeof(CAMERA_PLAY_INFO));
		bool bRet = CToolsHelp::Memcpy((IVS_CHAR*)&m_CameraPlayInfo,IVS_DEV_CODE_LEN, pCameraCode, strlen(pCameraCode));
		if(false == bRet)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start Real Play", "Memcpy error.");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		bRet = CToolsHelp::Memcpy((IVS_CHAR*)&m_CameraPlayInfo + IVS_DEV_CODE_LEN,
                                  sizeof(IVS_MEDIA_PARA)+IVS_DEV_CODE_LEN,
                                  &m_stuMediaParam, sizeof(IVS_MEDIA_PARA));

		if(false == bRet)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start Real Play", "Memcpy error.");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		m_CameraPlayInfo.uPlayHandle = (ULONG)this;
		m_CameraPlayInfo.pUserData = m_pUserMgr; 
		m_CameraPlayInfo.pPlayObj  = this;
    }

	return iRet;
}
#endif
int CRealPlay::ReStartStream(const IVS_MEDIA_PARA* pMediaPara, const IVS_CHAR* pCameraCode, const START_PLAY_PARAM& stParam)
{
	//У�����
	if (NULL == pCameraCode || strlen(pCameraCode) > (CODE_LEN-1) || strlen(pCameraCode) == 0)
	{
		BP_RUN_LOG_ERR(IVS_SMU_CAMERA_CODE_NOT_EXIST,"ReStart Stream", "camera code invalid");
		return IVS_PARA_INVALID;
	}
	IVS_DEBUG_TRACE("");

	//���ò���	
	bool bRet = CToolsHelp::Strncpy(m_szCameraID, sizeof(m_szCameraID), pCameraCode, strlen(pCameraCode));//lint !e534
	if(false == bRet)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"ReStart Stream", "Strncpy error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

	int iRet = SetMediaParam(pMediaPara);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"StartRealPlay", "set media param error");
		return iRet;
	}

	//��SCU��ȡý��URL���������ݱ����ڻ���.
	iRet = DoGetUrlRealPlay();
	if (IVS_SUCCEED != iRet) 
	{
		BP_RUN_LOG_ERR(iRet,"ReStart Stream", "get url real play error, stream default");
		return iRet;
	}

	// ���û�����������;
	iRet = CMediaBase::ReStartStream(stParam);
	if (iRet != IVS_SUCCEED)
	{
		BP_RUN_LOG_ERR(iRet,"ReStart Stream", "call base class failed.");
		return IVS_FAIL;
	}

	m_nStatus = STATUS_PLAYING;

	return iRet;
}

//lint -e1775 
int CRealPlay::StartPlay(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, const START_PLAY_PARAM& stParam)
{
    //У�����
    if (NULL == pCameraCode || strlen(pCameraCode) > (CODE_LEN-1) || strlen(pCameraCode) == 0)
    {
        BP_RUN_LOG_ERR(IVS_SMU_CAMERA_CODE_NOT_EXIST,"Start RealPlay", "camera code invalid");
        return IVS_PARA_INVALID;
    }
	CHECK_POINTER(pRealplayPara, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

    //���ò���
    bool bRet = CToolsHelp::Strncpy(m_szCameraID, sizeof(m_szCameraID), pCameraCode, strlen(pCameraCode));//lint !e534
	if(false == bRet)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start RealPlay", "Strncpy error.");
		return IVS_ALLOC_MEMORY_ERROR;
	}

    IVS_MEDIA_PARA stMediaPara;
    memset(&stMediaPara, 0, sizeof(IVS_MEDIA_PARA));
    stMediaPara.ProtocolType = (IVS_PROTOCOL_TYPE)pRealplayPara->uiProtocolType;
    stMediaPara.StreamType = (IVS_STREAM_TYPE)pRealplayPara->uiStreamType;
    stMediaPara.TransMode = (IVS_MEDIA_TRANS_MODE)pRealplayPara->bDirectFirst;
    stMediaPara.BroadCastType = (IVS_BROADCAST_TYPE)pRealplayPara->bMultiCast;
    stMediaPara.VideoDecType = VIDEO_DEC_H264;
    // m_stuMediaParam.VideoDecType = AUDIO_DEC_G711U;
    m_stuMediaParam.AudioDecType = AUDIO_DEC_G711U;
    int iRet = SetMediaParam(&stMediaPara);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay", "set media param error");
        return iRet;
    }

    //��SCU��ȡý��URL���������ݱ����ڻ���.
    iRet = DoGetUrlRealPlay();
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay", "get url real play error, stream default");
        return iRet;
    }

    pRealplayPara->uiStreamType = m_stuMediaParam.StreamType;

    // ���û�����������;
    iRet = CMediaBase::StartPlay(stParam);
    if (iRet != IVS_SUCCEED)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay", "call base class failed.");
        return iRet;
    }
	// �ر�ʵ���������¼�
	try
	{
		IVS_PLAY_StopNotifyBuffEvent(m_ulPlayerChannel);
	}
	catch (...)
	{//lint !e1775
		BP_RUN_LOG_INF("StopNotifyBuffEvent", " exception"); 
	}
	
	m_nStatus = STATUS_PLAYING;
	
	return iRet;
}

int CRealPlay::DoGetUrlRealPlay()
{
	IVS_DEBUG_TRACE("");
    int iRet = IVS_FAIL;

    //������Ϣ;
    memset(&m_stUrlMediaInfo, 0, sizeof(ST_URL_MEDIA_INFO));

    //TODO : szDstPort��ʵ�����ã���һ���̶�ֵ�����Э������szDstPP��νṹ��δ����
    m_stUrlMediaInfo.ServiceType = SERVICE_TYPE_REALVIDEO;

    std::string strCameraDomainCode = m_szCameraID;
    std::string strCameraCode;
    std::string strDomainCode;
    iRet = CToolsHelp::SplitCameraDomainCode(m_szCameraID, strCameraCode, strDomainCode);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "CToolsHelp::SplitCameraCode failed, RetCode=%d.", iRet);
        return iRet;
    }
	
    (void)CToolsHelp::Strncpy(m_stUrlMediaInfo.szSrcCode, sizeof(m_stUrlMediaInfo.szSrcCode), strCameraCode.c_str(), strCameraCode.size());//lint !e534
	(void)CToolsHelp::Strncpy(m_stUrlMediaInfo.cDomainCode,IVS_DOMAIN_CODE_LEN+1,  strDomainCode.c_str(), strDomainCode.size());//lint !e534
	(void)CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstCode,sizeof(m_stUrlMediaInfo.szDstCode), "01",strlen("01"));//lint !e534
	m_stUrlMediaInfo.MediaPara.StreamType = m_stuMediaParam.StreamType;
    m_stUrlMediaInfo.MediaPara.ProtocolType = m_stuMediaParam.ProtocolType;
	m_stUrlMediaInfo.MediaPara.VideoDecType = m_stuMediaParam.VideoDecType;
	m_stUrlMediaInfo.MediaPara.AudioDecType = m_stuMediaParam.AudioDecType;
	m_stUrlMediaInfo.MediaPara.TransMode = m_stuMediaParam.TransMode;
    m_stUrlMediaInfo.PackProtocolType = PACK_PROTOCOL_ES;
    m_stUrlMediaInfo.MediaPara.BroadCastType = m_stuMediaParam.BroadCastType;
    (void)CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstIP,sizeof(m_stUrlMediaInfo.szDstIP), "",strlen(""));//lint !e534
	(void)CToolsHelp::Strncpy(m_stUrlMediaInfo.szDstPort,sizeof(m_stUrlMediaInfo.szDstPort), "35016",strlen("35016"));//lint !e534

    //��ȡý��URL
    iRet = GetURL(CToolsHelp::GetSCCHead().c_str(), m_pUserMgr, m_stUrlMediaInfo, m_stMediaRsp, m_bAssociatedAudio);//lint !e838
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "real play SCC get URL failed!");
        return iRet;
    }

	iRet = ParseIPAddr(m_stMediaRsp.szURL,m_strMuIpAddr);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "parse IP Addr failed from url: %s",
			m_stMediaRsp.szURL);
		return iRet;
	}
    //����URL,�õ�ServiceType��������鲥ʵ����ֱ��������ôЭ������תΪudp
    std::string strServiceType;
    iRet = ParseURL(m_stMediaRsp.szURL, URL_SERVICE_TYPE_FLAG, strServiceType);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "parse service type failed from url: %s",
            m_stMediaRsp.szURL);
        return iRet;
    }
    int iServiceType = CToolsHelp::StrToInt(strServiceType);

    std::string strMediaTransMode;
    iRet = ParseURL(m_stMediaRsp.szURL, URL_MEDIA_TRANS_MODE, strMediaTransMode);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "parse media trans mode failed from url");
        return iRet;
    }
    int iMediaTransMode = CToolsHelp::StrToInt(strMediaTransMode);

    std::string strBroadcastType;
    iRet = ParseURL(m_stMediaRsp.szURL, URL_BROADCAST_TYPE, strBroadcastType);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "parse broadcast type failed from url");
        return iRet;
    }
    int iBroadcastType = CToolsHelp::StrToInt(strBroadcastType);
    
    m_enServiceType = (IVS_SERVICE_TYPE)iServiceType;
    m_stuMediaParam.TransMode = (IVS_MEDIA_TRANS_MODE)iMediaTransMode;
    m_stuMediaParam.BroadCastType = (IVS_BROADCAST_TYPE)iBroadcastType;

    // ֱ���鲥ʵ������Ҫ��Э������ת��ΪUDP;
	if (SERVICE_TYPE_REALVIDEO == m_enServiceType 
        && MEDIA_DIRECT == m_stuMediaParam.TransMode
        && BROADCAST_MULTICAST == m_stuMediaParam.BroadCastType)
	{
		m_stuMediaParam.ProtocolType = PROTOCOL_RTP_OVER_UDP;
	}

    // add for ������ [12/17/2012 z90003203]
    std::string strStreamType;
    iRet = ParseURL(m_stMediaRsp.szURL, URL_STREAM_TYPE, strStreamType);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Do Get Url RealPlay", "parse stream type failed from url");
        return iRet;
    }
    int iStreamType = CToolsHelp::StrToInt(strStreamType);
    m_stuMediaParam.StreamType = (IVS_STREAM_TYPE)iStreamType;

    if (STREAM_TYPE_UNSPECIFIED == m_stuMediaParam.StreamType)
    {
        m_stuMediaParam.StreamType = STREAM_TYPE_MAIN;
    }
    // end add
	return iRet;
}//lint !e529

//ֹͣʵ��
int CRealPlay::StopRealPlay()
{
	IVS_DEBUG_TRACE("");

    CLockGuard lock(m_pMediaMutex);

    int iRet = CMediaBase::StopPlay();

	//���ò���
	RetSet();

	m_nStatus = STATUS_FINISHED;

    if (iRet != IVS_SUCCEED)
    {
        iRet = IVS_FAIL;
    }
	return iRet;
}
#ifdef WIN32
int CRealPlay::AddPlayWindow(HWND hWnd)
{
	return IVS_PLAY_SetPlayWindow(m_ulPlayerChannel, hWnd);
}

int CRealPlay::DelPlayWindow(HWND hWnd)
{
	return IVS_PLAY_DelPlayWindow(m_ulPlayerChannel, hWnd);
}

int CRealPlay::AddPlayWindowPartial(HWND hWnd, void* pPartial)
{
	return IVS_PLAY_AddPlayWindowPartial(m_ulPlayerChannel, hWnd, (LAYOUT_DRAW_PARTIAL*)pPartial);
}

int CRealPlay::UpdateWindowPartial(HWND hWnd, void* pPartial)
{
	return IVS_PLAY_UpdateWindowPartial(m_ulPlayerChannel, hWnd, (LAYOUT_DRAW_PARTIAL*)pPartial);
}

//�жϴ��ھ���Ƿ����;
bool CRealPlay::IsExistHWnd(const HWND& hwnd)
{
    bool bRet = false;
    WndListIter iter = m_WndList.begin();
    WndListIter iterEnd = m_WndList.end();
    for (; iter != iterEnd; iter++)
    {
        if (hwnd == *iter)
        {
            bRet = true;
            break;
        }
    }
    return bRet;
}

//ɾ��WndList�е�Wnd;
int CRealPlay::DelHWndfromList(const HWND& hwnd)
{
    bool bRet = IVS_FAIL;
    WndListIter iter = m_WndList.begin();
    WndListIter iterEnd = m_WndList.end();
    for (; iter != iterEnd; iter++)
    {
        if (hwnd == *iter)
        {
            m_WndList.erase(iter);
            bRet = IVS_SUCCEED;
            break;
        }
    }
    return bRet;
}
#endif

//��ȡCameraCode
char* CRealPlay::GetCameraCode()
{
	return m_szCameraID;//lint !e1536
}//lint !e1762

void CRealPlay::SetHandle(unsigned long ulHandle)
{
	m_ulHandle = ulHandle;
}//lint !e1762

unsigned long CRealPlay::GetHandle()
{
	return m_ulHandle;
}//lint !e1762

//��ʼʵ��(������)
int CRealPlay::StartRealPlayCBRaw(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackRaw fRealPlayCallBackRaw, void* pUserData )
{
	IVS_DEBUG_TRACE("");
    CLockGuard lock(m_pMediaMutex);
    START_PLAY_PARAM stParam;
    //stParam.enServiceType = SERVICE_TYPE_REALVIDEO;
    stParam.cbRtspDataCallBack = g_fRtspDataCallBack;
    stParam.cbNetDataCallBack = g_fNetDataCallBack;
    int iRet = StartPlay(pRealplayPara, pCameraCode, stParam);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay CBRaw", "start play failed.");
        return iRet;
    }
	
    m_iStartType = PLAY_TYPE_CB;
    (void)VOS_MutexLock(m_pRealPlayRawCBMutex);
    m_fRealPlayCallBackRaw = fRealPlayCallBackRaw;
    m_pRealPlayUserDataRaw = pUserData;
    (void)VOS_MutexUnlock(m_pRealPlayRawCBMutex);
    iRet = IVS_PLAY_SetRawFrameCallBack(m_ulPlayerChannel, g_fReassmCallBack, this);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay CBRaw", "Set Raw Frame CallBack failed.");
        return iRet;
    }

    // ����ʵ�����������ڻָ�, add by zwb
    memset(&m_CameraPlayInfo, 0, sizeof(CAMERA_PLAY_INFO));
    bool bRet = CToolsHelp::Memcpy((IVS_CHAR*)&m_CameraPlayInfo, IVS_DEV_CODE_LEN, pCameraCode, strlen(pCameraCode));
    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start Real Play", "Memcpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }

    bRet = CToolsHelp::Memcpy((IVS_CHAR*)&m_CameraPlayInfo + IVS_DEV_CODE_LEN,
                               sizeof(IVS_MEDIA_PARA)+IVS_DEV_CODE_LEN,
                               &m_stuMediaParam, sizeof(IVS_MEDIA_PARA));

    if(false == bRet)
    {
        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Start Real Play", "Memcpy error.");
        return IVS_ALLOC_MEMORY_ERROR;
    }
    m_CameraPlayInfo.uPlayHandle = (ULONG)this;
    m_CameraPlayInfo.pUserData = m_pUserMgr;

	return iRet; 
}

void CRealPlay::DoRawFrameCallBack(IVS_RAW_FRAME_INFO* pstRawFrameInfo, IVS_CHAR* pFrame, IVS_UINT32 uFrameSize)
{
    (void)VOS_MutexLock(m_pRealPlayRawCBMutex);
    if (NULL != m_fRealPlayCallBackRaw)
    {
        m_fRealPlayCallBackRaw(m_ulHandle, pstRawFrameInfo, pFrame, uFrameSize, m_pRealPlayUserDataRaw);
    }
    (void)VOS_MutexUnlock(m_pRealPlayRawCBMutex);
}

void CRealPlay::DoFrameCallBack(IVS_INT32 iStreamType, IVS_VOID* pFrameInfo)
{
    (void)VOS_MutexLock(m_pRealPlayFrameDecoderCBMutex);
    if (NULL != m_fRealPlayCallBackFrame)
    {
        m_fRealPlayCallBackFrame(m_ulHandle, (unsigned int)iStreamType, pFrameInfo, m_pRealPlayUserDataFrame);
    }
    (void)VOS_MutexUnlock(m_pRealPlayFrameDecoderCBMutex);
}

//��ʼʵ��(YUV��)
int CRealPlay::StartRealPlayCBFrame(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData )
{
	IVS_DEBUG_TRACE("");
    CLockGuard lock(m_pMediaMutex);
    START_PLAY_PARAM stPlayParam;
    //stParam.enServiceType = SERVICE_TYPE_REALVIDEO;
    stPlayParam.cbRtspDataCallBack = g_fRtspDataCallBack;
    stPlayParam.cbNetDataCallBack = g_fNetDataCallBack;
    int iRet = StartPlay(pRealplayPara, pCameraCode, stPlayParam);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet,"Start RealPlay CB Frame", "start play failed.");
        return iRet;
    }

    m_iStartType = PLAY_TYPE_YUV;
    (void)VOS_MutexLock(m_pRealPlayFrameDecoderCBMutex);
    m_fRealPlayCallBackFrame = fRealPlayCallBackFrame;
    m_pRealPlayUserDataFrame = pUserData;
    (void)VOS_MutexUnlock(m_pRealPlayFrameDecoderCBMutex);
    iRet = IVS_SUCCEED;//IVS_PLAY_SetFrameCallBack(m_ulPlayerChannel, g_fDecoderCallBack, this);
	return iRet;
}
#ifdef WIN32
int CRealPlay::ReuseStream(HWND hWnd)
{
	IVS_DEBUG_TRACE("");
    LAYOUT_DRAW_NORMAL Layout;
    memset(&Layout, 0, sizeof(LAYOUT_DRAW_NORMAL));
    int iRet = IVS_PLAY_AddPlayWindow(m_ulPlayerChannel, hWnd, DRAW_NORMAL, &Layout);
    // ������óɹ������ڲ��뵽list���;
    if (IVS_SUCCEED == iRet)
    {
        m_WndList.push_back(hWnd);
    }
    return iRet;
}
#endif
bool CRealPlay::IsExistStream(const char* pszCameraID, IVS_STREAM_TYPE eStreamType) const
{
    if (NULL == pszCameraID)
    {
        return false;
    }

    std::string strCameraID = pszCameraID;
    if ((0 == strCameraID.compare(m_szCameraID))
        && ((STREAM_TYPE_UNSPECIFIED == eStreamType)
        || (eStreamType == m_stuMediaParam.StreamType)))
    {
        return true;
    }

    return false;
}

// ������·����
int CRealPlay::PlaySound()
{
    return CMediaBase::PlaySound();
}

//ֹͣ������·����
int CRealPlay::StopSound()
{
    return CMediaBase::StopSound();
}

//��������
int CRealPlay::SetVolume(unsigned int uiVolumeValue)
{
	IVS_DEBUG_TRACE("");
    return CMediaBase::SetVolume(uiVolumeValue);
}

//��ȡ����
int CRealPlay::GetVolume(unsigned int* puiVolumeValue)
{
	IVS_DEBUG_TRACE("");
    return CMediaBase::GetVolume(puiVolumeValue);
}

// ��ȡ������Ϣ
void CRealPlay::GetCameraPlayInfo(CAMERA_PLAY_INFO& cameraPlayInfo)const
{
	memset(&cameraPlayInfo, 0, sizeof(CAMERA_PLAY_INFO));
	bool bRet = CToolsHelp::Memcpy(&cameraPlayInfo, sizeof(CAMERA_PLAY_INFO), &m_CameraPlayInfo, sizeof(CAMERA_PLAY_INFO));
	if(false == bRet)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR,"Get Camera PlayInfo", "Memcpy error.");
		//return IVS_ALLOC_MEMORY_ERROR;
	}
}

// ��ȡ���Ŵ���
#ifdef WIN32
int CRealPlay::GetPlayWindows(HWND *pBuf, int size)
{
	IVS_DEBUG_TRACE("");
     WndListIter iter = m_WndList.begin();
	 WndListIter end  = m_WndList.end();
	 int count = 0;
	 for(; iter != end && count < size; iter++)
	 {
		 pBuf[count++] = *iter;
	 }
	 return count;
}
#endif

int CRealPlay::StopStream()
{
	IVS_DEBUG_TRACE("");
	CLockGuard lock(m_pMediaMutex);

	if (STATUS_FINISHED == m_nStatus)
	{
		return IVS_SUCCEED;
	}

	(void)CMediaBase::StopStream();

	m_nStatus = STATUS_STOPPED;

	return IVS_SUCCEED;
}

long CRealPlay::GetRef()
{
	return atomic_inc(&m_RefCounter);
}

long CRealPlay::PutRef()
{
	return atomic_dec(&m_RefCounter);
}

