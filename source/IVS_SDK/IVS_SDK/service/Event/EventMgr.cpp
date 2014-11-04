#include "EventMgr.h"
#include "ivs_error.h"
#include "_BaseThread.h"
#include "IVSCommon.h"
#include "EventCallBackJob.h"
#include "SDKInterface.h"
#ifdef WIN32
#include "RealPlay.h"
#include "AutoResume.h"
#endif
#include "IVS_SDK.h"
#include "IVS_Trace.h"
#include "ToolHelp.h"
//lint -e526
extern CSDKInterface *g_pNvs ;

//add by c00206592 ������յ�ˮӡ���ĸ澯ID
//key:���������  value:�澯ID
typedef std::map<std::string, std::string> WATER_ALARM_MAP;
typedef WATER_ALARM_MAP::iterator WATER_ALARM_MAP_ITER;
//�澯��ʼʱ��MAP��  key:�澯ID,Value:��ʼʱ��
typedef std::map<std::string ,std::string >ALARM_STARTIME_MAP;
typedef ALARM_STARTIME_MAP::iterator ALARM_STARTIME_MAP_ITER;

static  WATER_ALARM_MAP m_WaterAlarmEventIdMap;
static  ALARM_STARTIME_MAP m_AlarmTimeMap;

#define MAX_WATER_ALARM_MAP_LEN 128
//end add by c00206592 

static void GetWaterAlarmXml(const std::string& strCameraCode,const std::string& strUserIdDomainCode,
	const std::string& strDomianCode,const std::string& strStartTime,std::string& strReq)
{
	IVS_DEBUG_TRACE("");
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");

	(void)xml.AddChildElem("LoginInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("UserIDDomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strUserIdDomainCode.c_str());
	xml.OutOfElem();

	(void)xml.AddElem("CameraCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strCameraCode.c_str());
	(void)xml.AddElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strDomianCode.c_str());
	(void)xml.AddElem("AlarmTypeID");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolHelp::IntToStr(0).c_str());
	(void)xml.AddElem("OccurStartTime");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strStartTime.c_str());
	xml.OutOfElem();
	unsigned int xmlLen = 0;
	const IVS_CHAR* pReq = xml.GetXMLStream(xmlLen);

    if (NULL != pReq)
    {
        strReq = pReq;
    }

}

static void PraseResponseData(const std::string& strCameraCode, const std::string& strStartTime, 
                              const std::string& strpRsp, std::string& strWaterAlarmId)
{
	//��������
	CXml xmlRsp;
	(void)xmlRsp.Parse(strpRsp.c_str());
	IVS_DEBUG_TRACE("");

	if (!xmlRsp.FindElemEx("Content/AlarmEventID"))
	{
		return;
	}
	const char* AttriValue = xmlRsp.GetElemValue(); //��ʱ�洢�����ڵ�ֵ
	if (NULL == AttriValue)
	{
		return;
	}
	strWaterAlarmId = AttriValue;

	if (MAX_WATER_ALARM_MAP_LEN <= m_WaterAlarmEventIdMap.size())
	{
		WATER_ALARM_MAP_ITER  firstIter = m_WaterAlarmEventIdMap.begin();
		m_WaterAlarmEventIdMap.erase(firstIter);
	}
	if (MAX_WATER_ALARM_MAP_LEN <= m_AlarmTimeMap.size())
	{
		ALARM_STARTIME_MAP_ITER  firstIter = m_AlarmTimeMap.begin();
		m_AlarmTimeMap.erase(firstIter);
	}

    WATER_ALARM_MAP_ITER waterAlarmMapIter = m_WaterAlarmEventIdMap.find(strCameraCode);
    if (waterAlarmMapIter != m_WaterAlarmEventIdMap.end())
    {
        m_WaterAlarmEventIdMap.erase(waterAlarmMapIter);
    }

	m_WaterAlarmEventIdMap.insert(make_pair(strCameraCode, strWaterAlarmId));//lint !e534

    ALARM_STARTIME_MAP_ITER alarmTimeIter = m_AlarmTimeMap.find(strWaterAlarmId);
    if (alarmTimeIter != m_AlarmTimeMap.end())
    {
        m_AlarmTimeMap.erase(alarmTimeIter);
    }

	m_AlarmTimeMap.insert(make_pair(strWaterAlarmId, strStartTime));//lint !e534

}
static int GetUpdateWaterAlarmXml(const std::string& strCameraCode,const std::string& strUserIdDomainCode,const std::string& strDomianCode,const std::string& strStopTime,
									std::string& strReq,std::string& strStartTime,std::string& strAlarmEventID)
{
	IVS_DEBUG_TRACE("");
	WATER_ALARM_MAP_ITER waterAlarmMapIter = m_WaterAlarmEventIdMap.find(strCameraCode);
	WATER_ALARM_MAP_ITER waterAlarmMapIterEnd = m_WaterAlarmEventIdMap.end();
	if (waterAlarmMapIter == waterAlarmMapIterEnd)
	{
		return IVS_FAIL;
	}
	ALARM_STARTIME_MAP_ITER alarmTimeIter = m_AlarmTimeMap.find(waterAlarmMapIter->second);
	ALARM_STARTIME_MAP_ITER alarmTimeIterEnd = m_AlarmTimeMap.end();
	if (alarmTimeIter != alarmTimeIterEnd)
	{
		strStartTime = alarmTimeIter->second;
		m_AlarmTimeMap.erase(alarmTimeIter);
	}
	CXml xml;
	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");

	(void)xml.AddChildElem("LoginInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("UserIDDomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strUserIdDomainCode.c_str());
	xml.OutOfElem();

	(void)xml.AddElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strDomianCode.c_str());
	(void)xml.AddElem("AlarmEventID");
	(void)xml.IntoElem();
	strAlarmEventID = waterAlarmMapIter->second;
	(void)xml.SetElemValue(strAlarmEventID.c_str());
	(void)xml.AddElem("OccurStopTime");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(strStopTime.c_str());
	xml.OutOfElem();
	unsigned int xmlLen = 0;
	const IVS_CHAR* pReq = xml.GetXMLStream(xmlLen);
    if (NULL != pReq)
    {
        strReq = pReq;
    }
	
	m_WaterAlarmEventIdMap.erase(waterAlarmMapIter);
	return IVS_SUCCEED;
}

//�¼��ص�����
void __SDK_CALL EventCallBackFun(int iEventType, void *pEventBuf, int iBufSize, void *pUserData)
{
    int iSessionID = (long)pUserData;
    (void)CEventCallBackJob::instance().PostEvent(iSessionID, iEventType, pEventBuf, iBufSize);
} //lint !e818

//lint +e429

// rtsp_client�쳣�ص�;
static void __SDK_CALL RtspExceptionCallBack(long          handle,
                                             unsigned int ulMsgType,
                                             unsigned int ulErrCode,           // �ص����������룬���_enRtspClientRetCode
                                             void *       pParameter,
                                             void *       pUser)    // ע���������ΪNULL
{
	BP_RUN_LOG_INF("RtspExceptionCallBack", "rtsp handle[%d] MsgType[%u] ErrCode[%u]", handle, ulMsgType, ulErrCode);

	CHECK_POINTER_VOID(g_pNvs);
	unsigned long ulPlayHandle = 0;
	int iSessionID = reinterpret_cast<long>(pUser);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	if (NULL == pUserMgr)
	{
        BP_RUN_LOG_WAR("RtspExceptionCallBack", "Can not get usermgr by sessionid:%d", iSessionID);
	}
	else
	{
		ulPlayHandle = (unsigned long)pUserMgr->GetPlayHandlebyRtspHandle(handle);
	}
	//add by wangguangyan start
	if (0 == ulPlayHandle)
	{
		pUserMgr = g_pNvs->GetUserMgrbyRtspHandle(handle);
		if (NULL == pUserMgr)
		{
			BP_RUN_LOG_ERR(IVS_PARA_INVALID,"RtspExceptionCallBack", "Can not get usermgr by handle:%d", handle);
			return;
		}
		else
		{
			ulPlayHandle = (unsigned long)pUserMgr->GetPlayHandlebyRtspHandle(handle);
			iSessionID = reinterpret_cast<long>(pUser);
		}
	}
	//add by wangguangyan end
	if (0 == ulPlayHandle)
	{
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "RtspExceptionCallBack", "Get playhandle by rtsphandle failed, rtsp handle:%d", handle);
		return;
	}

    STREAM_EXCEPTION_INFO exceptionInfo;
    exceptionInfo.ulHandle = ulPlayHandle;
    exceptionInfo.iSessionID = iSessionID;
    exceptionInfo.iExceptionCode = 0;
	IVS_INT32 iRet = IVS_FAIL;
    // ����MU�������쳣ֹͣ�������;
    if (RTSP_METHOD_ANNOUNCE == ulMsgType)
    {
		//��ȡ����SDP��Ϣ
		if (IVS_SUCCEED == ulErrCode && NULL != pParameter)
		{
			//������ԿЭ�̹��̣�ʧ������ʾ��ԿЭ��ʧ��;
			iRet = pUserMgr->SetPlaySecretKeybyRtspHandle(handle,(char*)pParameter);
			if (IVS_SUCCEED == iRet)
			{
				BP_DBG_LOG("Set Play Secret Key by Rtsp Handle succeed, rtsp handle:%d", handle);
				return;
			}

            BP_DBG_LOG("Set Play Secret Key by Rtsp Handle failed, rtsp handle:%d", handle);
            return;
            //exceptionInfo.iExceptionCode = IVS_SDK_EXCHANGE_KEY_ERR_GET_KEY_FAIL;
		}
        // ����������ֻ��ص�
        if(RTSP_ERR_NORMAL_STOP == ulErrCode)
        {
            exceptionInfo.iExceptionCode = IVS_PLAYER_RET_RTSP_NORMAL_STOP;
        }
        // �쳣����
        else if(RTSP_ERR_ABNORMAL_STOP == ulErrCode)
        {
            exceptionInfo.iExceptionCode = IVS_PLAYER_RET_RTSP_ABNORMAL_STOP;
        }
    }
	// �쳣�Ͽ�����
	if (RTSP_ERR_DISCONNECT == ulErrCode)
	{
		//IVS_LOG(LOG_INFO, "IVS handle Rtsp connect abnormal begin");
		exceptionInfo.iExceptionCode = IVS_PLAYER_RET_RTSP_DISCONNECT;
	}
	else if (RTSP_ERR_TIMEOUT == ulErrCode)
	{
		exceptionInfo.iExceptionCode = IVS_PLAYER_RET_RECV_DATA_TIMEOUT;
	}
	
    // Rtsp�¼�����;
	ulMsgType = IVS_EVENT_RTSP_MSG;

	if (RTSP_ERR_TIMEOUT != ulErrCode)
	{
#ifdef WIN32
		pUserMgr->StopNotifyBufEvent(ulPlayHandle);
#endif
	}

	// ������Ϣ���ͺʹ����룬��pParameterת��Ϊ������sizeof��ô�С;
	int iBufSize = sizeof(STREAM_EXCEPTION_INFO);
	exceptionInfo.iReseved1 = handle;
	BP_RUN_LOG_INF("RtspExceptionCallBack", "Rtsp Error");

    EventCallBackFun(static_cast<IVS_INT32>(ulMsgType), &exceptionInfo, iBufSize, pUser);
}//lint !e818 ԭ�п�ĺ������壬���޸�

// net_source�쳣�ص�;
static void __SDK_CALL NetSourceExceptionCallBack(unsigned long ulChannel, int iMsgType, void*  /*pParam*/, void* pUser)
{
	BP_DBG_LOG("NetSource ExceptionCallBack Channel[%lu] MsgType[%d]", ulChannel, iMsgType);
	CHECK_POINTER_VOID(g_pNvs);
	//CHECK_POINTER_VOID(pUser); pUser��iSessionID��ֵ�����ǵ�ַ�����ܵ�ָ����;
    int iSessionID = reinterpret_cast <long>(pUser);

    // ������Ϣ���ͺʹ����룬��pParameterת��Ϊ������sizeof��ô�С;
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	if (NULL == pUserMgr)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "NetSourc ExceptionCallBack", "Can not get usermgr by sessionid:%d", iSessionID);
		return;
	}
#ifdef WIN32
	unsigned long playHandle = (unsigned long)pUserMgr->GetPlayHandlebyNet(ulChannel);
#else
	unsigned long playHandle = 0;
#endif
	if (0 == playHandle)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "NetSourc ExceptionCallBack", "Get playhandle by rtsphandle failed, rtsp handle:%d", playHandle);
		return;
	}

	if (IVS_PLAYER_RET_RECV_DATA_TIMEOUT != iMsgType)
	{
#ifdef WIN32
		pUserMgr->StopNotifyBufEvent(playHandle);
#endif
	}

	STREAM_EXCEPTION_INFO exceptionInfo;

	exceptionInfo.ulHandle = playHandle;
	exceptionInfo.iSessionID = iSessionID;
	//��NetSource�з��صĴ�������Ϣ��ΪiMsgType
	exceptionInfo.iExceptionCode = iMsgType;
	exceptionInfo.iReseved1 = (IVS_INT32)ulChannel;

	// ������Ϣ���ͺʹ����룬��pParameterת��Ϊ������sizeof��ô�С;
	int iBufSize = sizeof(STREAM_EXCEPTION_INFO);
	BP_DBG_LOG("NetSourceExceptionCallBack NetSource Error");
    EventCallBackFun(IVS_EVENT_NETSOURCE_MSG, &exceptionInfo, iBufSize, pUser);
}

// ivs_player���ſ��쳣�ص�;
static void __SDK_CALL PlayerExceptionCallBack(unsigned long ulChannel, int iMsgType, void*  pParam, void*  pUser)
{
	//���ſⷵ�صĴ�������Ϣ�Ѿ���װ��STREAM_EXCEPTION_INFO���ݽṹ���أ����а����˴�������Ϣ
	
    int iSessionID = reinterpret_cast <long>(pUser);
	BP_DBG_LOG("Player ExceptionCallBack Channel[%lu] MsgType[%d] SessionID[%d]", ulChannel, iMsgType, iSessionID);
    // ������Ϣ���ͺʹ����룬��pParameterת��Ϊ������sizeof��ô�С;
    //int iBufSize = sizeof(pParam);

    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	if (NULL == pUserMgr)
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "RtspExceptionCallBack", "Can not get usermgr by sessionid:%d", iSessionID);
		return;
	}

	unsigned long playHandle = pUserMgr->GetPlayHandlebyPlayChannel(ulChannel);
	if ((0 == playHandle) && (SERVICE_TYPE_AUDIO_BROADCAST != iMsgType))
	{
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "RtspExceptionCallBack", "Get playhandle by rtsphandle failed, rtsp handle:%d", playHandle);
		return;
	}
	
	switch (iMsgType)
	{
	case IVS_EVENT_LOCAL_RECORD_SUCCESS:
	case IVS_EVENT_DOWNLOAD_SUCCESS:
		{
			char Buf[sizeof(IVS_LOCAL_RECORD_SUCCESS_INFO) + sizeof(unsigned long)];
			IVS_LOCAL_RECORD_SUCCESS_INFO *recordInfo = (IVS_LOCAL_RECORD_SUCCESS_INFO *)(Buf);
			unsigned long *pHandle = (unsigned long *)(recordInfo + 1);//lint !e826
			if (!CToolsHelp::Memcpy(recordInfo, sizeof(IVS_LOCAL_RECORD_SUCCESS_INFO), pParam, sizeof(IVS_LOCAL_RECORD_SUCCESS_INFO)))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
				return;
			}
			*pHandle = playHandle;
			EventCallBackFun(iMsgType, Buf, sizeof(Buf), (void*)iSessionID);
		}
		break;

	case IVS_EVENT_LOCAL_PLAYBACK_FAILED:
		{	
			STREAM_EXCEPTION_INFO  info;
			info.iExceptionCode = 0;
			info.iSessionID     = iSessionID;
			info.ulHandle       = playHandle;
			info.iReseved1      = 0;
			EventCallBackFun(iMsgType, &info, sizeof(info), (void*)iSessionID);
		}
		break;

		//ˮӡ�۸ĸ澯
#ifdef WIN32
	case IVS_EVENT_PLAYER_WATER_MARK_EXCEPTION:
		{
			WATER_MARK_EXCEPTION_DATA* pExceptionData;
			pExceptionData = (WATER_MARK_EXCEPTION_DATA*)pParam;
			CMediaBase * pMediaBase = (CMediaBase *)playHandle;
			if (NULL == pMediaBase)
			{
				BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR,"ivs player ret water mark error", "MediaBase is null.");
				return ;
			}
			std::string strCameraID;
			pMediaBase->GetCameraID(strCameraID);
			std::string strUserDomainCode; //�û�����DomainCode
			//�ֽ��ȡCameraCode��DomainCode
			std::string strCameraCode;
			std::string strDomainCode;
			IVS_INT32 iRet = CToolsHelp::SplitCameraDomainCode(strCameraID, strCameraCode, strDomainCode);
			if (IVS_SUCCEED != iRet)
			{
				BP_RUN_LOG_ERR(iRet,"DO Player Exception", "SplitCameraCode failed, RetCode=%d.", iRet);
				return ;
			}
			//��ȡ�û��������
			pUserMgr->GetDomainCode(strUserDomainCode);
			//����XML����SMU�������
			CSendNssMsgInfo sendNssMsgInfo;	
			sendNssMsgInfo.SetNeedXml(TYPE_NSS_XML);
			sendNssMsgInfo.SetNetElemType(NET_ELE_SMU_NSS);
			sendNssMsgInfo.SetCameraCode(strCameraCode);
			sendNssMsgInfo.SetDomainCode(strDomainCode);
			std::string strReq;
			//ʱ���ʽΪ���뼶�𣬳���1000ת��Ϊ��
			time_t stTime_t = pExceptionData->ullTimeStamp / 1000; //lint !e713
			std::string strTime;
			//��ȡ��ʽ����string Time
			CMediaBase::UTCTime2Str(stTime_t, strTime, IVS_DATE_TIME_FORMAT);
			//����XML
			std::string strpRsp;
			IVS_INT32 iNeedRedirect = IVS_FAIL;
			IVS_ALARM_WATERMARK_INFO stWaterAlarmInfo;
			memset(&stWaterAlarmInfo,0,sizeof(IVS_ALARM_WATERMARK_INFO));
			memcpy(stWaterAlarmInfo.cCameraCode,strCameraID.c_str(),IVS_DEV_CODE_LEN);
			//�쳣�ص����ݣ�β��ָ������ݱ��浱ǰ���ŵ�handle
			char Buf[sizeof(IVS_ALARM_WATERMARK_INFO) + sizeof(unsigned long)];
			IVS_ALARM_WATERMARK_INFO *recordInfo = (IVS_ALARM_WATERMARK_INFO *)(Buf);
			unsigned long *pHandle = (unsigned long *)(recordInfo + 1);//lint !e826
			*pHandle = playHandle;

            std::string strWaterAlarmId("");

			//����ˮӡУ���쳣
			if (IVS_PLAYER_RET_WATER_MARK_ERROR == pExceptionData->ulExceptionCode)
			{
				memcpy(stWaterAlarmInfo.cStartTime, strTime.c_str(), strTime.size());
				sendNssMsgInfo.SetReqID(NSS_WATERMARK_ALARM_NOTIFY_REQ);

				GetWaterAlarmXml(strCameraCode, strUserDomainCode, strDomainCode, strTime, strReq);
				sendNssMsgInfo.SetReqData(strReq);

                if (0 == strReq.length())
                {
                    BP_DBG_LOG("", "NSS_WATERMARK_ALARM_NOTIFY_REQ xml is null, no need send msg to smu.");
                    return;
                }

				//���Ͳ���ʧ�ܣ�ֱ�ӷ���
				iRet = pUserMgr->SendCmd(sendNssMsgInfo, strpRsp, iNeedRedirect);
				if (IVS_SUCCEED == iRet)
				{   
                    //������������ֵ
					PraseResponseData(strCameraCode, strTime, strpRsp, strWaterAlarmId);
					if (!CToolsHelp::Memcpy(recordInfo, sizeof(IVS_ALARM_WATERMARK_INFO), &stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO)))
					{
						BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
						return;
					}
					//����ˮӡ�۸���ʾ
					//OnEventCallBack(iSessionID, IVS_EVENT_REPORT_WATERMARK_TIP, &stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO));
					EventCallBackFun(IVS_EVENT_REPORT_WATERMARK_TIP, Buf, sizeof(Buf), (void*)iSessionID);

                    stWaterAlarmInfo.ullAlarmEventID = static_cast<IVS_UINT64>(CToolHelp::StrToInt(strWaterAlarmId.c_str()));//lint !e571
                    BP_DBG_LOG("PlayerExceptionCallBack, ullAlarmEventID:%d, code:%d", stWaterAlarmInfo.ullAlarmEventID, IVS_PLAYER_RET_WATER_MARK_ERROR);

                    //����ˮӡ������Ϣ
                    ST_WATERMARK_ALARM_INFO stWaterMarkAlarmInfo;
                    memset(&stWaterMarkAlarmInfo, 0, sizeof(stWaterMarkAlarmInfo));
                    memcpy(&stWaterMarkAlarmInfo.stWaterMarkInfo, &stWaterAlarmInfo, sizeof(stWaterMarkAlarmInfo.stWaterMarkInfo));
                    stWaterMarkAlarmInfo.bIsNeedReport = true;
                    stWaterMarkAlarmInfo.bIsOpenedWater = true;

                    pUserMgr->SetWaterMarkInfo(playHandle, stWaterMarkAlarmInfo);
                    BP_DBG_LOG("PlayerExceptionCallBack, cCameraCode:%s, cStartTime:%s, ullAlarmEventID:%d",
                               stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, 
                               stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID);

//!!!ר�Ÿ����Ե��쳣����������ɾ���˶δ���
                    //std::string strStartTime;
                    //std::string strAlarmEventID;
                    //sendNssMsgInfo.SetReqID(NSS_WATERMARK_ALARM_UPDATE_REQ);
                    //(void)GetUpdateWaterAlarmXml(strCameraCode,strUserDomainCode,strDomainCode,strTime,strReq,strStartTime,strAlarmEventID);

                    //memcpy(stWaterAlarmInfo.cStartTime,strStartTime.c_str(),strStartTime.size());  //��ʼʱ��
                    //memcpy(stWaterAlarmInfo.cEndTime,strTime.c_str(),strTime.size());				//����ʱ��
                    //stWaterAlarmInfo.ullAlarmEventID =(unsigned long long) CToolsHelp::StrToInt(strAlarmEventID);//lint !e571
                    //sendNssMsgInfo.SetReqData(strReq);
                    //iRet = pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
                    ////����ͨ����Ӧ������Ҫ����
                    ////ֻ�з���ˮӡ�澯�ָ���ʱ�����CU�����¼�
                    //if (IVS_SUCCEED == iRet)
                    //{
                    //    if (!CToolsHelp::Memcpy(recordInfo, sizeof(IVS_ALARM_WATERMARK_INFO), &stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO)))
                    //    {
                    //        BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
                    //        return;
                    //    }
                    //    // ˮӡ�澯��¼����֪ͨ���д۸Ŀ�ʼʱ���ֹͣʱ�䣩
                    //    //OnEventCallBack(iSessionID, IVS_EVENT_REPORT_WATERMARK_ALARM,&stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO));
                    //    EventCallBackFun(IVS_EVENT_REPORT_WATERMARK_ALARM, Buf, sizeof(Buf), (void*)iSessionID);
                    //}
//!!!ר�Ÿ����Ե��쳣����������ɾ���˶δ���
				}
                else
                {
                    //����NSS_WATERMARK_ALARM_NOTIFY_REQ��Ϣ��SMUʧ��
                    BP_RUN_LOG_ERR(iRet, "", "Send NSS_WATERMARK_ALARM_NOTIFY_REQ(0x0C31) Msg To SMU Fail. ErrCode:%d.", iRet);
                }
			}
			//ˮӡ�澯�ָ�
			if(IVS_PLAYER_RET_WATER_MARK_OK == pExceptionData->ulExceptionCode)
			{
				std::string strStartTime;
				std::string strAlarmEventID;
				sendNssMsgInfo.SetReqID(NSS_WATERMARK_ALARM_UPDATE_REQ);
				(void)GetUpdateWaterAlarmXml(strCameraCode,strUserDomainCode,strDomainCode,strTime,strReq,strStartTime,strAlarmEventID);
                
                if (0 == strReq.length())
                {
                    BP_DBG_LOG("", "NSS_WATERMARK_ALARM_UPDATE_REQ xml is null, no need send msg to smu.");
                    return;
                }

				memcpy(stWaterAlarmInfo.cStartTime,strStartTime.c_str(),strStartTime.size());  //��ʼʱ��
				memcpy(stWaterAlarmInfo.cEndTime,strTime.c_str(),strTime.size());				//����ʱ��
				stWaterAlarmInfo.ullAlarmEventID =(unsigned long long) CToolsHelp::StrToInt(strAlarmEventID);//lint !e571
				sendNssMsgInfo.SetReqData(strReq);
				iRet = pUserMgr->SendCmd(sendNssMsgInfo,strpRsp,iNeedRedirect);
				//����ͨ����Ӧ������Ҫ����
				//ֻ�з���ˮӡ�澯�ָ���ʱ�����CU�����¼�
				if (IVS_SUCCEED == iRet)
				{
					if (!CToolsHelp::Memcpy(recordInfo, sizeof(IVS_ALARM_WATERMARK_INFO), &stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO)))
					{
						BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
						return;
					}
					// ˮӡ�澯��¼����֪ͨ���д۸Ŀ�ʼʱ���ֹͣʱ�䣩
					//OnEventCallBack(iSessionID, IVS_EVENT_REPORT_WATERMARK_ALARM,&stWaterAlarmInfo, sizeof(IVS_ALARM_WATERMARK_INFO));
					EventCallBackFun(IVS_EVENT_REPORT_WATERMARK_ALARM, Buf, sizeof(Buf), (void*)iSessionID);
                    
                    //����ˮӡ������Ϣ
                    ST_WATERMARK_ALARM_INFO stWaterMarkAlarmInfo;
                    memset(&stWaterMarkAlarmInfo, 0, sizeof(stWaterMarkAlarmInfo));
                    memcpy(&stWaterMarkAlarmInfo.stWaterMarkInfo, &stWaterAlarmInfo, sizeof(stWaterMarkAlarmInfo.stWaterMarkInfo));
                    stWaterMarkAlarmInfo.bIsNeedReport = false;
                    stWaterMarkAlarmInfo.bIsOpenedWater = true;

                    pUserMgr->SetWaterMarkInfo(playHandle, stWaterMarkAlarmInfo);
                    //pUserMgr->SetWaterExceptionFlag(playHandle, true);
                    BP_DBG_LOG("WaterMark PlayerExceptionCallBack, cCameraCode:%s, cStartTime:%s, cEndTime:%s, ullAlarmEventID:%d",
                                stWaterMarkAlarmInfo.stWaterMarkInfo.cCameraCode, stWaterMarkAlarmInfo.stWaterMarkInfo.cStartTime, 
                                stWaterMarkAlarmInfo.stWaterMarkInfo.cEndTime, stWaterMarkAlarmInfo.stWaterMarkInfo.ullAlarmEventID);
                }
                else
                {
                    //����NSS_WATERMARK_ALARM_UPDATE_REQ��Ϣ��SMUʧ��
                    BP_RUN_LOG_ERR(iRet, "", "Send NSS_WATERMARK_ALARM_UPDATE_REQ(0x0C32) Msg To SMU Fail. ErrCode:%d.", iRet);
                }
			}
		}
		break;
#endif
        //�����ļ��㲥�������ϱ���OCX����OCX�ر��ļ��㲥
    case SERVICE_TYPE_AUDIO_BROADCAST:   
        STREAM_EXCEPTION_INFO  info;
        info.iExceptionCode = IVS_PLAYER_RET_BROADCAST_FILE_IS_END;
        info.iSessionID     = iSessionID;
        info.ulHandle       = playHandle;
        info.iReseved1      = 0;
        EventCallBackFun(IVS_EVENT_FILE_BROADCAST_END, &info, sizeof(info), (void*)iSessionID);
        break;
	case IVS_EVENT_LOCAL_RECORD_EGENERIC:
	case IVS_EVENT_DOWNLOAD_FAILED:
	default:
		char Buf[sizeof(unsigned int) + sizeof(unsigned long)];
		unsigned long *pCode   = (unsigned long *)(Buf);
		unsigned long *pHandle = (unsigned long *)(pCode + 1);//lint !e826

		*pCode   = (unsigned long)pParam;
		*pHandle = (unsigned long)playHandle;
		EventCallBackFun(iMsgType, Buf, sizeof(Buf), (void*)iSessionID);
		break;
	}
	

    //EventCallBackFun(iMsgType, Buf, sizeof(Buf), (void*)iSessionID);
}

CEventMgr::CEventMgr(void)
{
    for (int i = 0; i < SDK_CLIENT_TOTAL + 1; i++)
    {
        m_EventUserData[i] = NULL;
        m_EventCallBack[i] = NULL;
    }

    m_pEventCallbackThread = NULL;
	m_pMutexEventCB = VOS_CreateMutex();
}

CEventMgr::~CEventMgr(void)
{
    // ֹͣ�¼��ص��߳�;
    IVS_DELETE(m_pEventCallbackThread);//lint !e1551

	try
	{
		if (NULL != m_pMutexEventCB)
		{
			VOS_DestroyMutex(m_pMutexEventCB);
			m_pMutexEventCB = NULL;
		}
	}
	catch(...)
	{ //lint !e1775

	}
}

int CEventMgr::Init()
{
    int iRet = IVS_SUCCEED;

    //�����¼��ص��߳�;
    try
    {
		CEventCallBackJob::instance().SetEventMgr(this);
        m_pEventCallbackThread = new _BaseThread(&CEventCallBackJob::instance(), 0);
    }
    catch (...)
    {
		BP_RUN_LOG_ERR(IVS_PLAYER_RET_ALLOC_MEM_ERROR, "new thread", "NA");
        m_pEventCallbackThread = NULL;
        iRet = IVS_PLAYER_RET_ALLOC_MEM_ERROR;
    }
    return iRet;
}

void CEventMgr::UnInit()
{
    // ֹͣ�¼��ص��߳�;
    IVS_DELETE(m_pEventCallbackThread);
}

int CEventMgr::SetEventCallBack( int iSessionID, EventCallBack fEventCallBack /*= NULL*/, void *pUserData /*= NULL*/ )
{
	CLockGuard lock(m_pMutexEventCB);
    if (iSessionID == -1)
    {
        //��¼�ص�����;
        m_EventCallBack[iSessionID + 1] = fEventCallBack;
        m_EventUserData[iSessionID + 1] = pUserData;

        //SetLocalFilePlayBackEventCallBack(EventCallBackFun, (void*)iSessionID);
        return IVS_SUCCEED;
    }

    if ((SDK_CLIENT_STARTNO > iSessionID) || (SDK_CLIENT_TOTAL <= iSessionID))
    {
		BP_RUN_LOG_ERR(IVS_PARA_INVALID, "iSessionID invalid", "NA");
        return IVS_PARA_INVALID;
    }

    //��¼�ص�����;
    m_EventCallBack[iSessionID + 1] = fEventCallBack;
    m_EventUserData[iSessionID + 1] = pUserData;

    //pUserMgr->SetEventCallBack(EventCallBackFun, (void*)iSessionID);
    //SetLocalFilePlayBackEventCallBack(EventCallBackFun, (void*)iSessionID);

    return IVS_SUCCEED;
}

//�첽�Ļص�
int CEventMgr::OnEventCallBack(int iSessionID, int iEventType, void* pEventData, int iBufSize)
{
    switch(iEventType)
    {
    case IVS_EVENT_RTSP_MSG:
    case IVS_EVENT_NETSOURCE_MSG:
        {
            BP_DBG_LOG("OnEventCallBack, iEventType:%d", iEventType);
            HandleStreamMsg(iSessionID, iEventType, pEventData, iBufSize);
            return IVS_SUCCEED;
        }
		//add by zwx211831, 20140509, �����Ƿ���Ӧ10026�¼������ô���
	case IVS_EVENT_OMU_REPORT_ALARM:
		{
			CHECK_POINTER(g_pNvs, IVS_OPERATE_MEMORY_ERROR);
			CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
			CHECK_POINTER(pUserMgr, IVS_SDK_RET_INVALID_SESSION_ID);
			if (!pUserMgr->GetReceiveOMUAlarm())
			{
				BP_DBG_LOG("CEventMgr::OnEventCallBack", "Receive OMU Alarm, EventType: %d. Ignore", iEventType);
				return IVS_SUCCEED;
			}
			break;
		}
    default:
        break;
    }
    
	CLockGuard lock(m_pMutexEventCB);
    EventCallBack fEventCallBack = m_EventCallBack[iSessionID + 1];
    if (NULL != fEventCallBack)
    {
        fEventCallBack(iEventType, pEventData, static_cast<IVS_UINT32>(iBufSize), m_EventUserData[iSessionID + 1]);
		BP_DBG_LOG("fEventCallBack End, EventType: %d, SessionID: %d, EventData: %p, BufSize: %d", 
			iEventType, iSessionID, pEventData, iBufSize);
    }
	else
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "OnEventCallBack", "fEventCallBack is null.");
	}
    return IVS_SUCCEED;
}//lint !e1762


fExceptionCallBack CEventMgr::GetRtspExceptionCallBack() const
{
    return RtspExceptionCallBack;
}

NET_EXCEPTION_CALLBACK CEventMgr::GetNetExceptionCallBack() const
{
    return NetSourceExceptionCallBack;
}

PLAYER_EXCEPTION_CALLBACK CEventMgr::GetPlayerExceptionCallBack() const
{
    return PlayerExceptionCallBack;
}

void CEventMgr::HandleStreamMsg(int iSessionID, int iEventType, void* pEventData, int iBufSize)
{
	IVS_INFO_TRACE("");
	CHECK_POINTER_VOID(g_pNvs);
    CUserMgr *pUserMgr = g_pNvs->GetUserMgr(iSessionID);
	CHECK_POINTER_VOID(pUserMgr);

    STREAM_EXCEPTION_INFO* pExceptionInfo = static_cast<STREAM_EXCEPTION_INFO*>(pEventData);
	CHECK_POINTER_VOID(pExceptionInfo);

    IVS_SERVICE_TYPE enServiceType = SERVICE_TYPE_REALVIDEO;
    int iRet = pUserMgr->GetServiceTypebyPlayHandle(pExceptionInfo->ulHandle, enServiceType);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID, "HandleStreamMsg", "GetServiceTypebyPlayHandle failed. ulPlayHandle(%d) is not realvideo or not platform playback.");
        return;
    }
    BP_RUN_LOG_INF("NA"," ServiceType = %d",enServiceType);

    BP_DBG_LOG("HandleStreamMsg: enServiceType = %d, iExceptionCode = %d.", enServiceType, pExceptionInfo->iExceptionCode);

    // ʵ����Ϊ��������ʱ�������Ҫ����;
    if (SERVICE_TYPE_REALVIDEO == enServiceType)
    {
	    
    	//CRealPlay* pRealPlay = dynamic_cast<CRealPlay *>(pUserMgr->GetRealPlayMgr().GetMediaBase(pExceptionInfo->ulHandle));
    	CRealPlay *pRealPlay = pUserMgr->GetRealPlayMgr().GetRealPlayEx(pExceptionInfo->ulHandle);
	
		//����ǽʹ�õ���ͬ�����͵�rtsp
		if(NULL == pRealPlay)
		{	
#ifdef WIN32	
			CTVWallMgr& mTVwallMgr = pUserMgr->GetTVWallMgr();				
			CDecoderChannel* pChannel = mTVwallMgr.GetChannel(pExceptionInfo->ulHandle);
			CHECK_POINTER_VOID(pChannel);		

			CUMW_NOTIFY_INFO info = {0};
			CUMW_NOTIFY_TYPE_DECODER_ERROR_INFO stErrorInfo = {0};

			const IVS_CHAR* pCode = pChannel->GetDecoderID();
			CHECK_POINTER_VOID(pCode);
			(void)CToolsHelp::Strncpy(stErrorInfo.szDecoderID,CUMW_DECODER_ID_LEN+1,pCode,strlen(pCode));//lint !e831
			stErrorInfo.szDecoderID[CUMW_DECODER_ID_LEN] = 0;

			stErrorInfo.usChannelNo = pChannel->GetChannel();//lint !e734
            if (IVS_PLAYER_RET_RTSP_NORMAL_STOP == pExceptionInfo->iExceptionCode)
            {
                stErrorInfo.stErrorCode.ulErrorCode = (unsigned long)pExceptionInfo->iExceptionCode;
            }

            if (NULL != pCode)//lint !e831 !e774
            {
                BP_DBG_LOG("HandleStreamMsg: DecoderCode = %s, usChannelNo = %d.", pCode, stErrorInfo.usChannelNo);
            }
            else
            {
                BP_DBG_LOG("HandleStreamMsg: DecoderCode is NULL.");
            }
            
			info.ulNotifyType = CUMW_NOTIFY_TYPE_RTSP_EXCEPTION;
			info.pNotifyInfo = (void*)(&stErrorInfo);
			info.ulNotifyInfoLen = sizeof(info);

			mTVwallMgr.DeleteChannel(pChannel->GetHandle());
			mTVwallMgr.NotifyEvent(&info);			
			mTVwallMgr.AddAutoResume(pChannel, CUMW_BREAKDOWN_REASON_NO_VIDEO);

			BP_RUN_LOG_INF("TVWall rtsp exception.","Ready to AutoResume.");
#endif
			return;
		}
		// ��ȡ��������Ҫ�Ĳ���
        CAMERA_PLAY_INFO cameraPlayInfo;
        memset(&cameraPlayInfo, 0 ,sizeof(CAMERA_PLAY_INFO));
        pRealPlay->GetCameraPlayInfo(cameraPlayInfo);

		BP_DBG_LOG("CameraPlayInfo: StreamType[%d], Protocal[%d]", cameraPlayInfo.stMediaPara.StreamType, cameraPlayInfo.stMediaPara.ProtocolType);

		// ֹͣ����
        (void)pRealPlay->StopStream();

				
        OnEventCallBack(iSessionID, IVS_EVENT_REALPLAY_FAILED, pEventData, 1);
#ifdef WIN32
        // ʵ���ĶϿ����Ӻ�������ʱ��Ҫ��������;
        if (IVS_PLAYER_RET_RTSP_DISCONNECT == pExceptionInfo->iExceptionCode 
            || IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode
			|| IVS_PLAYER_RET_RTSP_ABNORMAL_STOP == pExceptionInfo->iExceptionCode)
			//|| IVS_PLAYER_RET_RTSP_NORMAL_STOP == pExceptionInfo->iExceptionCode//���������ֹͣ�������������̡�
        {
			BP_RUN_LOG_INF("rtsp exception, add to autoresume list","NA");
            CAutoResume& autoResume = pUserMgr->GetAutoResume();
            autoResume.AddCameraPlayInfo2List(cameraPlayInfo);
			pRealPlay->GetRef();
        }
		
		pUserMgr->GetRealPlayMgr().ReleaseRealPlay(pRealPlay);
#endif
    }
    else if (SERVICE_TYPE_PLAYBACK == enServiceType)
    {
		CPlayback* pPlayBack = pUserMgr->GetPlaybackMgr().GetPlayBack(pExceptionInfo->ulHandle);
		if (NULL != pPlayBack)
		{
			// �ж�����ǻ������������Ƿ񲥷���ϣ����û�в��ţ��ٴβ��뵽���У��ȴ��´μ��;
			unsigned int uiRemainCount = 0;
			(void)pPlayBack->GetSourceBufferRemain(uiRemainCount);
			if (uiRemainCount > 0 
				&& IVS_EVENT_RTSP_MSG == iEventType 
				&& pExceptionInfo->iExceptionCode == IVS_PLAYER_RET_RTSP_NORMAL_STOP)
			{
#ifdef WIN32
				Sleep(100);
#else
				VOS_Sleep(100);
#endif
				EventCallBackFun(IVS_EVENT_RTSP_MSG, pEventData, iBufSize, (void*)iSessionID);
				//OnEventCallBack(iSessionID, IVS_EVENT_RTSP_MSG, pEventData, iBufSize);
				BP_RUN_LOG_INF("Handle Rtsp Msg", "Error Code[%u] RemainCount[%u]", pExceptionInfo->iExceptionCode, uiRemainCount);
				
				return;
			}

			if ((pPlayBack->IsStepPlay() || pPlayBack->IsPaused() || pPlayBack->IsControlPaused()) && 
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Timeout Msg on Pause State");
				return;
			}

			if ((pPlayBack->IsStepPlay()|| pPlayBack->IsPaused()) && pPlayBack->IsStepPlayLastFrame() && IVS_PLAYER_RET_RTSP_NORMAL_STOP == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "StepPlay Is Last Frame Msg on Pause State");
				return;
			}

			if (uiRemainCount > 0 && 
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Decode buffer remain count[%u], do not handle timeout exception", uiRemainCount);
				return;
			}

			BP_RUN_LOG_INF("HandleStreamMsg", "Notify to OCX, Error Code[%u] RemainCount[%u]", pExceptionInfo->iExceptionCode, uiRemainCount);
			// �ж�״̬
			//(void)IVS_SDK_StopPlatformPlayBack(pExceptionInfo->iSessionID, pExceptionInfo->ulHandle);
			OnEventCallBack(iSessionID, IVS_EVENT_REMOTE_PLAYBACK_FAILED, pEventData, iBufSize);
		}
    }
#ifdef WIN32
	else if (SERVICE_TYPE_BACKUPRECORD_PLAYBACK == enServiceType)
	{
		CPlayback* pPlayBack = pUserMgr->GetPlaybackMgr().GetPlayBack(pExceptionInfo->ulHandle);
		if (NULL != pPlayBack)
		{
			if ((pPlayBack->IsStepPlay() || pPlayBack->IsPaused()) &&
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Timeout Msg on Pause State");
				return;
			}

			// �ж�״̬
			(void)IVS_SDK_StopBackupPlayBack(pExceptionInfo->iSessionID, pExceptionInfo->ulHandle);
			OnEventCallBack(iSessionID, IVS_EVENT_REMOTE_PLAYBACK_FAILED, pEventData, iBufSize);
		}
	}
	else if (SERVICE_TYPE_DISASTEBACKUP_PLAYBACK == enServiceType)
	{
		CPlayback* pPlayBack = pUserMgr->GetPlaybackMgr().GetPlayBack(pExceptionInfo->ulHandle);
		if (NULL != pPlayBack)
		{
			if ((pPlayBack->IsStepPlay() || pPlayBack->IsPaused()) && 
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Timeout Msg on Pause State");
				return;
			}

			// �ж�״̬
			(void)IVS_SDK_StopDisasterRecoveryPlayBack(pExceptionInfo->iSessionID, pExceptionInfo->ulHandle);
			OnEventCallBack(iSessionID, IVS_EVENT_REMOTE_PLAYBACK_FAILED, pEventData, iBufSize);
		}
	}
    else if (SERVICE_TYPE_PU_PLAYBACK == enServiceType)
    {
		CPlayback* pPlayBack = pUserMgr->GetPlaybackMgr().GetPlayBack(pExceptionInfo->ulHandle);
		if (NULL != pPlayBack)
		{
			if (pPlayBack->IsPaused() && 
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Pu Play Back Timeout Msg on Pause State");
				return;
			}

			(void)IVS_SDK_StopPUPlayBack(pExceptionInfo->iSessionID, pExceptionInfo->ulHandle);
			OnEventCallBack(iSessionID, IVS_EVENT_REMOTE_PLAYBACK_FAILED, pEventData, iBufSize);
		}
    }
#endif
	else if (SERVICE_TYPE_DOWNLOAD == enServiceType || 
		SERVICE_TYPE_BACKUPRECORD_DOWNLOAD == enServiceType ||
		SERVICE_TYPE_DISASTEBACKUP_DOWNLOAD == enServiceType ||
		SERVICE_TYPE_PU_DOWNLOAD == enServiceType)
	{
		CDownload* pDownload = pUserMgr->GetDownLoadMgr().GetDownLoad(pExceptionInfo->ulHandle);
		if (NULL != pDownload)
		{
			if (pDownload->IsPaused() && 
				IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
			{
				BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Download Timeout Msg on Pause State");
				return;
			}
			OnEventCallBack(iSessionID, IVS_EVENT_DOWNLOAD_FAILED, pEventData, iBufSize);
		}

		//(void)IVS_SDK_StopPlatformDownload(pExceptionInfo->iSessionID, pExceptionInfo->ulHandle);
		//OnEventCallBack(iSessionID, IVS_EVENT_DOWNLOAD_FAILED, pEventData, iBufSize);
	}
#ifdef WIN32
	else if (SERVICE_TYPE_AUDIO_BROADCAST == enServiceType)
	{
        // Modify by wanglei 00165153:2013.6.27
        // �㲥ҵ����Ҫ���������쳣����
        // DTS2013062701806 
        if (IVS_PLAYER_RET_RECV_DATA_TIMEOUT == pExceptionInfo->iExceptionCode)
        {
            BP_RUN_LOG_INF("HandleStreamMsg", "Ignore Broadcast Timeout Msg on Pause State");
            return;
        }

        IVS_ULONG ulConnHandle = (IVS_ULONG)pExceptionInfo->iReseved1;
        std::string strDeviceCode;
        iRet = pUserMgr->GetAudioMgr().GetBroadCastDeviceCodeByConnHandle(ulConnHandle, strDeviceCode);
		if (IVS_SUCCEED == iRet)
		{
            // ���ӽṹ�巵���쳣�������㲥Code���OCX��OCX��װ��xml���ظ�CU;
            memset(pExceptionInfo->szDeviceCode, 0, IVS_DEV_CODE_LEN);
            strncpy(pExceptionInfo->szDeviceCode, strDeviceCode.c_str(), IVS_DEV_CODE_LEN - 1);
            OnEventCallBack(iSessionID, IVS_EVENT_BROADCAST_FAILED, pEventData, iBufSize);
		}
	}
    else if (SERVICE_TYPE_AUDIO_CALL == enServiceType)
    {
        IVS_ULONG ulConnHandle = (IVS_ULONG)pExceptionInfo->iReseved1;
        std::string strDeviceCode;
        iRet = pUserMgr->GetAudioMgr().GetTalkBackDeviceCodeByConnHandle(ulConnHandle, strDeviceCode);
        if (IVS_SUCCEED == iRet)
        {
            // ���ӽṹ�巵���쳣�������㲥Code���OCX��OCX��װ��xml���ظ�CU;
            memset(pExceptionInfo->szDeviceCode, 0, sizeof(pExceptionInfo->szDeviceCode));
            strncpy(pExceptionInfo->szDeviceCode, strDeviceCode.c_str(), IVS_DEV_CODE_LEN - 1);
            OnEventCallBack(iSessionID, IVS_EVENT_TALKBACK_FAILED, pEventData, iBufSize);
        }
    }
#endif

}
