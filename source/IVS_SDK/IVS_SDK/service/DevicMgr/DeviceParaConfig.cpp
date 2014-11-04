/********************************************************************
filename : DeviceParaConfig.cpp
author : gwx151720
created : 2012/12/03
description : 
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/
#include "DeviceParaConfig.h"
#include "UserMgr.h"
#include "nss_mt.h"
#include "NSSOperator.h"
#include "XmlProcess.h"
#include "DeviceParaConfigXMLProcess.h"
#include "IVS_Trace.h"
#include "devEnum.h"

CDeviceParaConfig::CDeviceParaConfig()
	:m_pUserMgr(NULL)
{
}

CDeviceParaConfig::~CDeviceParaConfig()
{
	m_pUserMgr = NULL;
}

void CDeviceParaConfig::SetUserMgr( CUserMgr *pUserMgr)
{
	m_pUserMgr = pUserMgr;
}

// ���������ı�
CDeviceParaConfig::REQUEST_NAME CDeviceParaConfig::m_reqNames[] =
{
	{ CONFIG_DEVICE_CFG,	(FnSetDevicePara)&CDeviceParaConfig::SetMainDevConfig,        (FnGetDevicePara)&CDeviceParaConfig::GetMainDevCfg},            //
	{ CONFIG_DEVICE_NET_CFG,   (FnSetDevicePara)&CDeviceParaConfig::SetDevNetConfig,         (FnGetDevicePara)&CDeviceParaConfig::GetDevNetConfig},
	{ CONFIG_CAMERA_CFG,    (FnSetDevicePara)&CDeviceParaConfig::SetCameraBriefCfg,       (FnGetDevicePara)&CDeviceParaConfig::GetCameraBriefCfg},
	{ CONFIG_CAMERA_STREAM_CFG,    (FnSetDevicePara)&CDeviceParaConfig::SetCameraStreamCfg,      (FnGetDevicePara)&CDeviceParaConfig::GetCameraStreamCfg},
	{ CONFIG_CAMERA_DISPLAY_CFG,     (FnSetDevicePara)&CDeviceParaConfig::SetCameraBasicImgCfg,    (FnGetDevicePara)&CDeviceParaConfig::GetCameraBasicImgCfg},
	{ CONFIG_ALARM_IN_CFG,     (FnSetDevicePara)&CDeviceParaConfig::SetAlarmInCfg,           (FnGetDevicePara)&CDeviceParaConfig::GetAlarmInCfg},
	{ CONFIG_ALARM_OUT_CFG,     (FnSetDevicePara)&CDeviceParaConfig::SetAlarmOutCfg,          (FnGetDevicePara)&CDeviceParaConfig::GetAlarmOutCfg},
	{ CONFIG_CAMERA_OSD_CFG,        (FnSetDevicePara)&CDeviceParaConfig::SetOsgPara,              (FnGetDevicePara)&CDeviceParaConfig::GetOSDPara},
	{ CONFIG_CAMERA_VIDEO_HIDE_ALARM_CFG,    (FnSetDevicePara)&CDeviceParaConfig::SetVideoHideAlarm,       (FnGetDevicePara)&CDeviceParaConfig::GetVideoHideAlarm},
	{ CONFIG_CAMERA_IMAGING_CFG,     (FnSetDevicePara)&CDeviceParaConfig::SetCameraImgCfg,         (FnGetDevicePara)&CDeviceParaConfig::GetCameraImgCfg},
	{ CONFIG_CAMERA_VIDEO_MASK_CFG,    (FnSetDevicePara)&CDeviceParaConfig::SetVideoMaskCfg,         (FnGetDevicePara)&CDeviceParaConfig::GetVideoMask},
	{ CONFIG_DEVICE_PTZ_CFG,       (FnSetDevicePara)&CDeviceParaConfig::SetCameraPTZCfg,         (FnGetDevicePara)&CDeviceParaConfig::GetCameraPTZCfg},
	{ CONFIG_CAMERA_AUDIO_CFG,      (FnSetDevicePara)&CDeviceParaConfig::SetAudioCfg,             (FnGetDevicePara)&CDeviceParaConfig::GetAudioCfg},
	{ CONFIG_CAMERA_MOTION_DETECTION_CFG,      (FnSetDevicePara)&CDeviceParaConfig::SetMotionDetec,          (FnGetDevicePara)&CDeviceParaConfig::GetMotionDetec},
	{ CONFIG_DEVICE_TIME_CFG,         (FnSetDevicePara)&CDeviceParaConfig::SetDevTimeConfig,        (FnGetDevicePara)&CDeviceParaConfig::GetDevTimeConfig},
	{ CONFIG_SERIAL_CHANNEL_CFG,        (FnSetDevicePara)&CDeviceParaConfig::SetSerialChannelConfig,    (FnGetDevicePara)&CDeviceParaConfig::GetSerialChannelConfig},
	{ CONFIG_CAMERA_EXTEND_CFG,         (FnSetDevicePara)&CDeviceParaConfig::SetCameraExCfg,          (FnGetDevicePara)&CDeviceParaConfig::GetCameraExCfg},
	{ CONFIG_CAMERA_SNAPSHOT_CFG,        (FnSetDevicePara)&CDeviceParaConfig::SetCameraSnapshotCfg,    (FnGetDevicePara)&CDeviceParaConfig::GetCameraSnapshotCfg},
	{ CONFIG_CAMERA_RECORD_STREAM_CFG,       (FnSetDevicePara)&CDeviceParaConfig::SetCameraRecordStreamCfg,    (FnGetDevicePara)&CDeviceParaConfig::GetCameraRecordStreamCfg},
	{ CONFIG_CAMERA_FEC_CFG,      (FnSetDevicePara)&CDeviceParaConfig::SetCameraFECCfg,         (FnGetDevicePara)&CDeviceParaConfig::GetCameraFECCfg},
	//add by zwx211831, Date:20140624,�澯����¼��ʱ����¼��ͼ��������Ŀǰû��get�ķ�������ʱʹ�þɵ�get����������û�ж��⿪�š����������ʱ�ٸĶ���
	{ CONFIG_CAMERA_STREAM_EVENTEX_CFG, (FnSetDevicePara)&CDeviceParaConfig::SetCameraStreamCfgEventEx, (FnGetDevicePara)&CDeviceParaConfig::GetCameraStreamCfg}
}; 

//����REQ_ID��ȡ������ָ��
CDeviceParaConfig::FnSetDevicePara CDeviceParaConfig::GetFnSetDevicePara(IVS_UINT32 enReqID) const
{
	FnSetDevicePara fn = NULL;
	int iSize = sizeof(m_reqNames) / sizeof(REQUEST_NAME);

	for ( int i = 0; i < iSize; i++ )
	{
		if ( m_reqNames[i].reqID == enReqID )
		{
			fn = m_reqNames[i].fnSetDevicePara;
			break;
		}
	}
	return fn;
}

//����REQ_ID��ȡ������ָ��
CDeviceParaConfig::FnGetDevicePara CDeviceParaConfig::GetFnGetDevicePara(IVS_UINT32 enReqID) const
{
	FnGetDevicePara fn = NULL;
	int iSize = sizeof(m_reqNames) / sizeof(REQUEST_NAME);

	for ( int i = 0; i < iSize; i++ )
	{
		if ( m_reqNames[i].reqID == enReqID )
		{
			fn = m_reqNames[i].fnGetDevicePara;
			break;
		}
	}
	return fn;
}

int CDeviceParaConfig::HandleSetDevicePara(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, IVS_UINT32 uiConfigType)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	int iRet = IVS_FAIL;
	// ����ȫ���Ӷȴ˴����˷ַ��������Ҫ����������������m_reqNames�����������Ϣ���ͣ��ʹ���������
	FnSetDevicePara fnSetProcess = GetFnSetDevicePara(uiConfigType);
	if (NULL == fnSetProcess)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "no process fn","iRet = %d", iRet);
		return iRet;
	}
	return (this->*fnSetProcess)(pBuffer, pDevCode, pDomainCode);
}

int CDeviceParaConfig::HandleGetDevicePara(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, IVS_UINT32 uiConfigType)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	int iRet = IVS_FAIL;
	// ����ȫ���Ӷȴ˴����˷ַ��������Ҫ����������������m_reqNames�����������Ϣ���ͣ��ʹ���������
	FnGetDevicePara fnGetProcess = GetFnGetDevicePara(uiConfigType);
	if (NULL == fnGetProcess)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "no process fn","iRet = %d", iRet);
		return iRet;
	}
	return (this->*fnGetProcess)(pBuffer, pDevCode, pDomainCode);
}

// �����豸����
IVS_INT32 CDeviceParaConfig::SetDeviceConfig(const IVS_CHAR* pDevBigCode, 
    IVS_UINT32 uiConfigType, 
    const IVS_VOID* pBuffer, 
    IVS_UINT32 uiBufSize)const
{
    CHECK_POINTER(pDevBigCode, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CHAR chDevCode[IVS_SDK_DEV_CODE_LEN+1] = {0};
	IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};

    // ����DevCode
    IVS_INT32 iRet = CXmlProcess::ParseDevCode(pDevBigCode, chDevCode, chDomaCode);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Set DeviceConfig fail", "(ParseDevCode)iRet=%d", iRet);
        return iRet;
    }

    // ���buffersize
    iRet =  CheckBufferSize(uiConfigType, uiBufSize);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Set DeviceConfig fail", "(CheckBufferSize)iRet=%d", iRet);
        return iRet;
    }

	iRet  = HandleSetDevicePara(pBuffer, chDevCode, chDomaCode, uiConfigType);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set DeviceConfig fail", "(HandleEvent)iRet=%d", iRet);
		return iRet;
	}

    return iRet;
}

// ��ȡ�豸����
IVS_INT32 CDeviceParaConfig::GetDeviceConfig(const IVS_CHAR* pDevBigCode, 
                                             IVS_UINT32 uiConfigType,
                                             IVS_VOID* pBuffer,
                                             IVS_UINT32 uiBufSize)const
{
    CHECK_POINTER(pDevBigCode, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CHAR chDevCode[IVS_SDK_DEV_CODE_LEN+1] = {0};
	IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};

    // ����DevCode
    IVS_INT32 iRet = CXmlProcess::ParseDevCode(pDevBigCode, chDevCode, chDomaCode);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get DeviceConfig fail", "(ParseDevCode)iRet=%d", iRet);
        return iRet;
    }

    //���buffersize
    iRet =  CheckBufferSize(uiConfigType, uiBufSize);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Get DeviceConfig fail", "(CheckBufferSize)iRet=%d", iRet);
        return iRet;
    }

	iRet  = HandleGetDevicePara(pBuffer, chDevCode, chDomaCode, uiConfigType);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DeviceConfig fail", "(HandleEvent)iRet=%d", iRet);
		return iRet;
	}

    return iRet;
}

// ��ȡ�������豸��ϸ��Ϣ
IVS_INT32 CDeviceParaConfig::GetAudioDeviceConfig(const IVS_CHAR* pDevBigCode, 
	const IVS_CHAR* pAudioDevCode, 
	IVS_UINT32 uiConfigType,
	IVS_VOID* pBuffer,
	IVS_UINT32 uiBufSize)const
{
	CHECK_POINTER(pDevBigCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAudioDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CHAR chDevCode[IVS_SDK_DEV_CODE_LEN+1] = {0};
	IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};

	// ����DevCode
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pDevBigCode, chDevCode, chDomaCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Audio Device Config fail", "(ParseDevCode)iRet=%d", iRet);
		return iRet;
	}

	//���buffersize
	iRet =  CheckBufferSize(uiConfigType, uiBufSize);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Audio Device Config fail", "(CheckBufferSize)iRet=%d", iRet);
		return iRet;
	}

	iRet  = GetAudioCfg(pBuffer,chDevCode,pAudioDevCode,chDomaCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Audio Device Config fail", "(GetAudioCfg)iRet=%d", iRet);
		return iRet;
	}

	return iRet;
}

// У���ڴ��С
IVS_INT32 CDeviceParaConfig::CheckBufferSize(IVS_UINT32 uiConfigType, IVS_UINT32 uiBuffSize)const
{
    IVS_DEBUG_TRACE("");
	
    IVS_UINT32 uiCheckBufferSize = 0;
    IVS_INT32 iRet = IVS_SUCCEED;

	uiCheckBufferSize = CDevEnum::GetSizeOfConfigType(uiConfigType);
	if ((uiCheckBufferSize == 0) || (uiBuffSize != uiCheckBufferSize))
	{
		iRet = IVS_OPERATE_MEMORY_ERROR;
		BP_RUN_LOG_ERR(iRet, "Check BufferSize fail", "uiConfigType fail");
	}

    return iRet;
}

// �������豸����
IVS_INT32 CDeviceParaConfig::SetMainDevConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const

{
    CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
    CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_DEVICE_CFG* pMainDevCfg = (IVS_DEVICE_CFG*)pBuffer;

	// �ж��Ǽ�Ⱥ��ѵ�
	IVS_DOMAIN_ROUTE stDomainRoute;
    memset(&stDomainRoute, 0, sizeof(stDomainRoute));

	std::string strDomainCode = pDomainCode;
	CDomainRouteMgr& oDomainRoute = m_pUserMgr->GetDomainRouteMgr();
	IVS_INT32 iRet = oDomainRoute.GetDomainRoutebyCode(strDomainCode, stDomainRoute);
	// ��Ⱥ���
	if (2 == stDomainRoute.uiDomainType)
	{
		iRet = SetMainDevConfigMsg(pMainDevCfg, pDevCode, pDomainCode, "CMU", "", BP::DOMAIN_CLUSTER);

		return iRet;
	}
	// �ѵ�ģʽ������ģʽ
	iRet = SetMainDevConfigMsg(pMainDevCfg, pDevCode, pDomainCode, "SCU", pDevCode);

	return iRet;
}

// �������豸������Ϣģʽ
IVS_INT32 CDeviceParaConfig::SetMainDevConfigMsg(const IVS_DEVICE_CFG* pMainDevCfg, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const char* pszDestModule, const char* pszDeviceCode, enum BP::DOMAIN_TYPE enDomainType)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pMainDevCfg, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	/************************************************************************
    ** ��Կ�������̣���ʼ��->��������ID->����Կ->��Կ����(���ɵ�����ID)->
    **              ��������->ʹ����Կ�������ɵ�����ID����Cmd->������Ϣ                                                                   
    ************************************************************************/
    // ��ʼ����Կ���������,�����Կ����
    std::string strLinkID;
    CSDKSecurityClient oSecurityClient;
    int iMultiExchangeKey = oSecurityClient.MultiDomainExchangeKey(NET_ELE_SMU_NSS, m_pUserMgr, pDomainCode, strLinkID, pszDestModule, pszDeviceCode, "", enDomainType);
    if (IVS_SUCCEED != iMultiExchangeKey)
    {
        BP_RUN_LOG_ERR(iMultiExchangeKey, "Set MainDevConfig", "oSecurityClient Init failed");
        return iMultiExchangeKey;
    }
    // �������ݺ�ƴװ
    std::string strDevPWDCipher;
	std::string strDevRegPWDCipher;
    char cDevPasswdTemp[IVS_PWD_LEN + 1] = {0};
	char cDevRegPWDTemp[IVS_PWD_LEN + 1] = {0};
	// �豸����
	if (0 != strcmp(pMainDevCfg->cDevPasswd, ""))
	{
		if (!CToolsHelp::Memcpy(cDevPasswdTemp, IVS_PWD_LEN, pMainDevCfg->cDevPasswd, IVS_PWD_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevPasswdTemp, IVS_PWD_LEN, pMainDevCfg->cDevPasswd, IVS_PWD_LEN)", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		if (IVS_SUCCEED != oSecurityClient.EncryptString(cDevPasswdTemp, strDevPWDCipher))
		{
			BP_RUN_LOG_ERR(IVS_DATA_ENCRYPT_ERROR, "Set MainDevConfig", "Encrypt String failed");
			return IVS_DATA_ENCRYPT_ERROR;
		}
	}
    
	// �豸ע������
	if (0 != strcmp(pMainDevCfg->cDevRegPasswd, ""))
	{
		if (!CToolsHelp::Memcpy(cDevRegPWDTemp, IVS_PWD_LEN, pMainDevCfg->cDevRegPasswd, IVS_PWD_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevRegPWDTemp, IVS_PWD_LEN, pMainDevCfg->cDevRegPasswd, IVS_PWD_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		if (IVS_SUCCEED != oSecurityClient.EncryptString(cDevRegPWDTemp, strDevRegPWDCipher))
		{
			BP_RUN_LOG_ERR(IVS_DATA_ENCRYPT_ERROR, "Set MainDevConfig", "Encrypt String failed");
			return IVS_DATA_ENCRYPT_ERROR;
		}
	}

    CXml xmlTemp;
    IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetMainParaConfigGetXml(pDomainCode,  pDevCode, pMainDevCfg, strDevPWDCipher.c_str(), strDevRegPWDCipher.c_str(), xmlTemp);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(iRet, "Set MainParaConfig GetXml fail", "IVS_SUCCEED != iRet");
        return iRet;
    }	

	IVS_UINT32 uiLen = 0;
	const IVS_CHAR* pReq = xmlTemp.GetXMLStream(uiLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
	BP_RUN_LOG_INF("SetMainDevConfigCluster", "send XML = %s", pReq);
	std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();

	//����Ҫ���͵�CMD��ƴװ��NSS��Ϣͷ
	CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_SET_MAIN_DEV_CFG_REQ, pReq, strSecurityTransID);
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);
	//������Ϣ
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);

	iRet = CNSSOperator::instance().ParseCmd2NSS(pCmdRsp);
	HW_DELETE(pCmdRsp);
    return iRet;
}

// ��ѯ�������������
IVS_INT32 CDeviceParaConfig::GetCameraBriefCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const

{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_CFG* pCameraCfg = (IVS_CAMERA_CFG*)pBuffer;
	CXml xmlReq;
	//ƴװxml��ѯ����		
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pCameraCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CameraBriefCfg GetXML fail","IVS_SUCCEED != iRet");
		return iRet;
	}
	
	//��ȡ��Ӧxml
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgrForRedirect(xmlReq,xmlRsp, m_pUserMgr, NSS_GET_CAMERA_CFG_REQ, BUILSTRING,pCameraCode, pDomainCode);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	// ��ȡ��Ӧ��Ϣ,���ṹ��
	iRet = CDeviceParaConfigXMLProcess::ParseCameraBriefInfoRsp(xmlRsp, pCameraCfg);

	return iRet;
}

//��ѯ�������������
IVS_INT32 CDeviceParaConfig::GetCameraStreamCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const

{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_STREAM_CFG* pCameraStrCfg = (IVS_CAMERA_STREAM_CFG*)pBuffer;
	CXml xmlReq;
	// ƴװxml��ѯ����		
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CameraStreamCfg fail","IVS_SUCCEED != iRet");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_CAMERA_STREAM_CFG_REQ, BUILSTRING, pDomainCode); 
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::CameraStreamCfgParseXML(pCameraStrCfg, xmlRsp);

	return iRet;

}

//��ѯ���������ͼ�����
IVS_INT32 CDeviceParaConfig::GetCameraBasicImgCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const

{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_DISPLAY_CFG* pCameraDisplayCfg = (IVS_CAMERA_DISPLAY_CFG*)pBuffer;
	// ������ѯ������ͨ������
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pCameraCode, xmlTemp, SDKCAMERA);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get CommConfig GetXML fail", "(GetCommConfigGetXML)iRet=%d", iRet);
		return iRet;
	}

	// ��������xml
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_BASIC_IMAGING_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��		
	iRet = CDeviceParaConfigXMLProcess::ParseCameraDispalyCfgRsp(xmlRsp, pCameraDisplayCfg);

	return iRet;
}

// ��ȡ��̨����
IVS_INT32 CDeviceParaConfig::GetCameraPTZCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const

{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_PTZ_CFG* pCameraPTZCfg = (IVS_PTZ_CFG*)pBuffer;
	// ������ѯ������ͨ������
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pCameraCode, xmlTemp, SDKCAMERA);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get CameraPTZCfg fail", "(GetCommConfigGetXML)iRet=%d", iRet);
		return iRet;
	}

	//��������xml
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_PTZ_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��		
	iRet = CDeviceParaConfigXMLProcess::ParseCameraPTZCfgRsp(xmlRsp, pCameraPTZCfg, pDomainCode);

	return iRet;
}

// ��ѯ��Ƶ����(�����豸ͨ��)
IVS_INT32 CDeviceParaConfig::GetAudioCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode,const IVS_CHAR* pAudioDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAudioDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_AUDIO_CFG* pAudioCfg = (IVS_AUDIO_CFG*)pBuffer;
	// ������ѯ������ͨ������
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pAudioDevCode, xmlTemp, SDKVOICE);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get AudioCfg fail", "(GetCommConfigGetXML)iRet=%d", iRet);
		return iRet;
	}

	// ��������xml
	iRet = CXmlProcess::GetCommSendMgrForRedirect(xmlTemp, m_pUserMgr, NSS_GET_VOICE_INFO_REQ, BUILSTRING,pCameraCode, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��		
	iRet = CDeviceParaConfigXMLProcess::ParseAudioCfgRsp(xmlTemp,pAudioCfg);

	return iRet;
}

// ��ѯ�������豸��ϸ��Ϣ
IVS_INT32 CDeviceParaConfig::GetAudioCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_AUDIO_CFG* pAudioCfg = (IVS_AUDIO_CFG*)pBuffer;
	// ������ѯ������ͨ������
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pCameraCode, xmlTemp, SDKVOICE);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get AudioCfg fail", "(GetCommConfigGetXML)iRet=%d", iRet);
		return iRet;
	}

	// ��������xml
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp,xmlRsp, m_pUserMgr, NSS_GET_VOICE_INFO_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��		
	iRet = CDeviceParaConfigXMLProcess::ParseAudioCfgRsp(xmlRsp,pAudioCfg);

	return iRet;
}


// �޸��������������
IVS_INT32 CDeviceParaConfig::SetCameraBriefCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_CFG* pCameraCfg = (IVS_CAMERA_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraBriefCfgGetXml(pDomainCode,  pCameraCode, pCameraCfg, xmlTemp);
	// �����豸��װxml
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraBriefCfg GetXml fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_CAMERA_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// �޸��������������
IVS_INT32 CDeviceParaConfig::SetCameraStreamCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const 
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_STREAM_CFG* pCameraStrCfg = (IVS_CAMERA_STREAM_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraStreamCfgGetXML(pCameraStrCfg, pDomainCode, pDevCode, xmlTemp);
	// �����豸��װxml
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraStreamCfg GetXML fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_CAMERA_STREAM_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}


// �޸���������������������¼�����ͼ������������Ϣ��
IVS_INT32 CDeviceParaConfig::SetCameraStreamCfgEventEx(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const 
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_STREAM_EVENTEX_CFG* pCameraStrCfgEx= (IVS_CAMERA_STREAM_EVENTEX_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraStreamCfgEventExGetXML(pCameraStrCfgEx, pDomainCode, pDevCode, xmlTemp);
	// �����豸��װxml
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "SetCameraStreamCfgEventEx fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_CAMERA_STREAM_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// �޸����������ͼ�����
IVS_INT32 CDeviceParaConfig::SetCameraBasicImgCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_DISPLAY_CFG* pCameraDisplayCfg = (IVS_CAMERA_DISPLAY_CFG*)pBuffer;
	CXml tempXml;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraBasicImgCfgGetXml(pDomainCode,  pCameraCode, pCameraDisplayCfg, tempXml);

	// ���ʧ�ܣ���ӡ������־
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraBasicImgCfg GetXml fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(tempXml, m_pUserMgr, NSS_SET_BASIC_IMAGING_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// �޸���̨����
IVS_INT32 CDeviceParaConfig::SetCameraPTZCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_PTZ_CFG* pCameraPTZCfg = (IVS_PTZ_CFG*)(pBuffer);
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraPTZCfgGetXml(pDomainCode,  pCameraCode, pCameraPTZCfg, xmlTemp);

	// ���ʧ�ܣ���ӡ������־
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraPTZCfg GetXml fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_PTZ_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}
// �����������豸����
IVS_INT32 CDeviceParaConfig::SetAudioCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_AUDIO_CFG* pAudioCfg = (IVS_AUDIO_CFG*)(pBuffer);
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetAudioCfgGetXml(pDomainCode,  pCameraCode, pAudioCfg, xmlTemp);

	// ���ʧ�ܣ���ӡ������־
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set AudioCfg fail", "IVS_SUCCEED != iRet");
		return iRet;
	}	

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_CONF_VOICE_CHAN_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ���豸����
IVS_INT32 CDeviceParaConfig::GetMainDevCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
    CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
	
	IVS_DEVICE_CFG* pMainDevCfg = (IVS_DEVICE_CFG*)pBuffer;

	// �ж��Ǽ�Ⱥ��ѵ�
	IVS_DOMAIN_ROUTE stDomainRoute;
    memset(&stDomainRoute, 0, sizeof(stDomainRoute));

	std::string strDomainCode = pDomainCode;
	CDomainRouteMgr& oDomainRoute = m_pUserMgr->GetDomainRouteMgr();
	IVS_INT32 iRet = oDomainRoute.GetDomainRoutebyCode(strDomainCode, stDomainRoute);
	// ��Ⱥ���
	if (2 == stDomainRoute.uiDomainType)
	{
		iRet = GetMainDevCfgMsg(pMainDevCfg, pDevCode, pDomainCode, BP::DOMAIN_CLUSTER);

		return iRet;
	}
	// �ѵ�ģʽ������ģʽ
	iRet = GetMainDevCfgMsg(pMainDevCfg, pDevCode, pDomainCode);

	return iRet;
}

// ��ѯ���豸������Ϣģʽ
IVS_INT32 CDeviceParaConfig::GetMainDevCfgMsg(IVS_DEVICE_CFG* pMainDevCfg, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, enum BP::DOMAIN_TYPE enDomainType)const
{
	CHECK_POINTER(pMainDevCfg, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	/************************************************************************
    ** ��Կ�������̣���ʼ��->��������ID->����Կ->��Կ����(���ɵ�����ID)->
    **              ��������->ʹ����Կ�������ɵ�����ID����Cmd->������Ϣ                                                                   
    ************************************************************************/
    // ��ʼ����Կ���������,�����Կ����
    std::string strLinkID;
	CSDKSecurityClient oSecurityClient;
    int iMultiExchangeKey = oSecurityClient.MultiDomainExchangeKey(NET_ELE_SMU_NSS, m_pUserMgr, pDomainCode, strLinkID, "SCU", pDevCode, "", enDomainType);
    if (IVS_SUCCEED != iMultiExchangeKey)
    {
        BP_RUN_LOG_ERR(iMultiExchangeKey, "Get MainDevCfg fail", "oSecurityClient Init failed");
        return iMultiExchangeKey;
    }

	// ��װ������
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlTemp, SDKDEVICE);
    if(IVS_SUCCEED != iRet)
    {		
        BP_RUN_LOG_ERR(iRet, "Get MainDevCfg fail", "(GetCommConfigGetXML)iRet=%d", iRet);
        return iRet;
    }
	IVS_UINT32 uiLen = 0;
	const IVS_CHAR* pReq = xmlTemp.GetXMLStream(uiLen);
	CHECK_POINTER(pReq, IVS_OPERATE_MEMORY_ERROR);
	std::string strSecurityTransID  = oSecurityClient.GetSecurityDataTransID();

	CCmd *pCmd = CNSSOperator::instance().BuildCmd(strLinkID, NET_ELE_SMU_NSS, NSS_GET_DEV_BASIC_INFO_REQ, pReq, strSecurityTransID);
	CHECK_POINTER(pCmd, IVS_OPERATE_MEMORY_ERROR);

	// ������Ϣ
	CCmd *pCmdRsp = CNSSOperator::instance().SendSyncCmd(pCmd);
	CHECK_POINTER(pCmdRsp, IVS_NET_RECV_TIMEOUT);
	const IVS_CHAR* pRsp = CNSSOperator::instance().ParseCmd2XML(pCmdRsp, iRet);
	HW_DELETE(pCmdRsp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevConfig CommSendMgr fail", "NA");
		return iRet;
	}
	CHECK_POINTER(pRsp, IVS_XML_INVALID);
	CXml xmlRsp;
	xmlRsp.RemoveElem();
	if (!xmlRsp.Parse(pRsp))
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "xmlTemp.Parse(pRsp) fail", "NA");
		IVS_DELETE(pRsp, MUILI);
		return IVS_OPERATE_MEMORY_ERROR;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��
	iRet = CDeviceParaConfigXMLProcess::GetMainDevConfigParseXml(pMainDevCfg, xmlRsp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get MainDevConfig ParseXml fail", "NA");
		IVS_DELETE(pRsp, MUILI);
		return iRet;
	}

	// �������ݺ�ƴװ
	std::string strDevNewPWD;
	std::string strDevNewRegPWD;
	// �豸����
	if (0 != strcmp(pMainDevCfg->cDevPasswd, ""))
	{
		if (IVS_SUCCEED != oSecurityClient.DecryptString(pMainDevCfg->cDevPasswd, strDevNewPWD))
		{
			BP_RUN_LOG_ERR(IVS_DATA_DECRYPT_ERROR, "Get MainDevCfg fail", "Decrypt PWD fail");
			IVS_DELETE(pRsp, MUILI);
			return IVS_DATA_DECRYPT_ERROR;
		}
		memset(pMainDevCfg->cDevPasswd, 0x0, IVS_PWD_LEN);

		if (!CToolsHelp::Memcpy(pMainDevCfg->cDevPasswd, IVS_PWD_LEN, strDevNewPWD.c_str(), strDevNewPWD.size()))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(pMainDevCfg->cDevPasswd, IVS_PWD_LEN, strDevNewPWD.c_str(), strDevNewPWD.size()) fail", "NA");
			IVS_DELETE(pRsp, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
	}
	
	// �豸ע������
	if (0 != strcmp(pMainDevCfg->cDevRegPasswd, ""))
	{
		if (IVS_SUCCEED != oSecurityClient.DecryptString(pMainDevCfg->cDevRegPasswd, strDevNewRegPWD))
		{
			BP_RUN_LOG_ERR(IVS_DATA_DECRYPT_ERROR, "Get MainDevCfg fail", "Decrypt PWD fail");
			IVS_DELETE(pRsp, MUILI);
			return IVS_DATA_DECRYPT_ERROR;
		}
		memset(pMainDevCfg->cDevRegPasswd, 0x0, IVS_PWD_LEN);

		if (!CToolsHelp::Memcpy(pMainDevCfg->cDevRegPasswd, IVS_PWD_LEN, strDevNewRegPWD.c_str(), strDevNewRegPWD.size()))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(pMainDevCfg->cDevRegPasswd, IVS_PWD_LEN, strDevNewRegPWD.c_str(), strDevNewRegPWD.size()) fail", "NA");
			IVS_DELETE(pRsp, MUILI);
			return IVS_ALLOC_MEMORY_ERROR;
		}
	}

	IVS_DELETE(pRsp, MUILI);
	return iRet;
}

// ��ѯOSD����
IVS_INT32 CDeviceParaConfig::GetOSDPara(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const
{
	// У�����
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_OSD_CFG* pOSDConfig = (IVS_CAMERA_OSD_CFG*)pBuffer;
	CXml xmlReq;
	// ���ṹ����װΪXML
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get OSDPara fail", "(GetOSDParaGetXML)iRet=%d", iRet);
		return iRet;
	}
	// ����װ�õ�XMLƴװ��NSS����
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_OSD_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	// ��XMLת��Ϊ�ṹ��
	iRet = CDeviceParaConfigXMLProcess::GetOSDParaParseXML(pOSDConfig,xmlRsp);

	return iRet;
}


// �޸ĸ澯�������豸
IVS_INT32 CDeviceParaConfig::SetAlarmInCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_ALARM_IN_CFG* pAlarmInCfg = (IVS_ALARM_IN_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetAlarmInCfgGetXML(pAlarmInCfg, pDomainCode, pDevCode, xmlTemp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set AlarmInCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_CONF_ALARM_IN_CHAN_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�澯�������豸
IVS_INT32 CDeviceParaConfig::GetAlarmInCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_ALARM_IN_CFG* pAlarmInCfg = (IVS_ALARM_IN_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlTemp, SDKALARMINCODE);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get AlarmInCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_ALARM_IN_INFO_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::GetAlarmInCfgParseXML(pAlarmInCfg, xmlRsp);

	return iRet;
}

// �޸ĸ澯������豸
IVS_INT32 CDeviceParaConfig::SetAlarmOutCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_ALARM_OUT_CFG* pAlarmOutCfg = (IVS_ALARM_OUT_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetAlarmOutCfgGetXML(pAlarmOutCfg, pDomainCode, pDevCode, xmlTemp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set AlarmOutCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_CONF_ALARM_OUT_CHAN_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�澯������豸
IVS_INT32 CDeviceParaConfig::GetAlarmOutCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_ALARM_OUT_CFG* pAlarmOutCfg = (IVS_ALARM_OUT_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlTemp, SDKALARMOUTCODE);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get AlarmOutCfg GetXML fail", "NA");
		return iRet;
	}
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_ALARM_OUT_INFO_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::GetAlarmOutCfgParseXML(pAlarmOutCfg, xmlRsp);

	return iRet;
}

// ��ѯ��˽����
IVS_INT32 CDeviceParaConfig::GetVideoMask(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_VIDEO_MASK* pVideoMask = (IVS_VIDEO_MASK*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlTemp, SDKCAMERA);
	if(IVS_SUCCEED != iRet)
	{		
		BP_RUN_LOG_ERR(iRet, "Get MainDevCfg fail", "(GetCommConfigGetXML)iRet=%d", iRet);
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_VIDEO_MASK_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	//��ȡ��Ӧ��Ϣ,���ṹ��
	iRet = CDeviceParaConfigXMLProcess::GetVideoMaskConfigParseXml(pVideoMask, xmlRsp);	

	return iRet;
}

// �޸���˽����
IVS_INT32 CDeviceParaConfig::SetVideoMaskCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_VIDEO_MASK* pVideoMask = (IVS_VIDEO_MASK*)(pBuffer);
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetVideoMaskCfgGetXML(pVideoMask, pDomainCode, pDevCode, xmlTemp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set AlarmOutCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_VIDEO_MASK_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}
// �޸�OSD����
IVS_INT32 CDeviceParaConfig::SetOsgPara(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_OSD_CFG* pCameraOsgCfg = (IVS_CAMERA_OSD_CFG*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraOsgCfgGetXML(pDomainCode, pDevCode, pCameraOsgCfg, xmlTemp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraOsgCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_OSD_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// �޸�ͼ���ڵ�
IVS_INT32 CDeviceParaConfig::SetVideoHideAlarm(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm = (IVS_VIDEO_HIDE_ALARM*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetVideoHideAlarmGetXML(pDomainCode, pDevCode, pVideoHideAlarm, xmlTemp);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set VideoHideAlarm GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, m_pUserMgr, NSS_SET_HIDE_ALARM_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯͼ���ڵ�
IVS_INT32 CDeviceParaConfig::GetVideoHideAlarm(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm = (IVS_VIDEO_HIDE_ALARM*)pBuffer;
	CXml xmlTemp;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlTemp, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get VideoHideAlarm GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlTemp, xmlRsp,m_pUserMgr, NSS_GET_HIDE_ALARM_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::GetVideoHideAlarmParseXML(pVideoHideAlarm, xmlRsp);

	return iRet;
}


// ����ǰ���ƶ����
IVS_INT32 CDeviceParaConfig::SetMotionDetec(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_MOTION_DETECTION* pMotionDetec = (IVS_MOTION_DETECTION*)(pBuffer);
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetMotionDetecGetXML(pDevCode, pDomainCode, pMotionDetec, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set MotionDetec GetXML is fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_MOTION_DETECTION_ALARM_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯǰ���ƶ����
IVS_INT32 CDeviceParaConfig::GetMotionDetec(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_MOTION_DETECTION* pMotionDetec = reinterpret_cast<IVS_MOTION_DETECTION*>(pBuffer);
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get MotionDetecXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_MOTION_DETECTION_ALARM_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::ParseMotionDetecXML(xmlRsp, pMotionDetec);

	return iRet;
}

// �޸����豸�������
IVS_INT32 CDeviceParaConfig::SetDevNetConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_DEVICE_NET_CFG* pDevNetCfg = (IVS_DEVICE_NET_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetDevNetCfgGetXML(pDevCode, pDomainCode, pDevNetCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set DevNetCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_DEV_NET_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ���豸�������
IVS_INT32 CDeviceParaConfig::GetDevNetConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_DEVICE_NET_CFG* pDevNetCfg = (IVS_DEVICE_NET_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKDEVICE);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevNetCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp, m_pUserMgr, NSS_GET_DEV_NET_CFG_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	iRet = CDeviceParaConfigXMLProcess::GetDevNetCfgParseXML(pDevNetCfg, xmlRsp);

	return iRet;
}

// �޸����豸ʱ�����
IVS_INT32 CDeviceParaConfig::SetDevTimeConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_DEVICE_TIME_CFG* pDevTimeCfg = (IVS_DEVICE_TIME_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetDevTimeCfgGetXML(pDevCode, pDomainCode, pDevTimeCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set DevTimeCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_DEV_TIME_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ���豸ʱ�����
IVS_INT32 CDeviceParaConfig::GetDevTimeConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_DEVICE_TIME_CFG* pDevTimeCfg = (IVS_DEVICE_TIME_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKDEVICE);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevTimeCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_DEV_TIME_CFG_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DevConfig CommSendMgr fail", "NA");
		return iRet;
	}
	iRet = CDeviceParaConfigXMLProcess::GetDevTimeCfgParseXML(pDevTimeCfg, xmlRsp);

	return iRet;
}

// ���ô������豸��ϸ��Ϣ
IVS_INT32 CDeviceParaConfig::SetSerialChannelConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg = (IVS_SERIAL_CHANNEL_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetSerialChannelCfgGetXML(pDevCode, pDomainCode, pSerialChannelCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set SerialChannelConfig fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_CONF_SERIAL_CHAN_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�������豸��ϸ��Ϣ
IVS_INT32 CDeviceParaConfig::GetSerialChannelConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pSerialChannelCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pSerialChannelCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg = (IVS_SERIAL_CHANNEL_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pSerialChannelCode, xmlReq, SDKSERIAL);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get SerialChannelConfig fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_SERIAL_INFO_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get SerialChannelConfig fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::GetSerialChannelCfgParseXML(pSerialChannelCfg, xmlRsp);

	return iRet;
}

// �����������չͼ�����
IVS_INT32 CDeviceParaConfig::SetCameraImgCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_IMAGING_CFG* pCameraImgCfg = (IVS_CAMERA_IMAGING_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraImgCfgGetXML(pDevCode, pDomainCode, pCameraImgCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraImgCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_EXTENDED_IMAGING_PARA_REQ, BUILDRET, pDomainCode); 

	return iRet;
}

// ��ѯ�������չͼ�����
IVS_INT32 CDeviceParaConfig::GetCameraImgCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_IMAGING_CFG* pCameraImgCfg = (IVS_CAMERA_IMAGING_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CameraImgCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_EXTENDED_IMAGING_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	iRet = CDeviceParaConfigXMLProcess::GetCameraImgCfgParseXML(pCameraImgCfg, xmlRsp);

	return iRet;
}

// �����������չ����
IVS_INT32 CDeviceParaConfig::SetCameraExCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_EXTEND_CFG* pCameraExCfg = (IVS_CAMERA_EXTEND_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraExCfgGetXML(pDevCode, pDomainCode, pCameraExCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set CameraExCfg GetXML fail", "NA");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_CAMERA_EXTEND_CFG_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�������չ����
IVS_INT32 CDeviceParaConfig::GetCameraExCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_EXTEND_CFG* pCameraExCfg = (IVS_CAMERA_EXTEND_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CameraExCfg GetXML fail", "NA");
		return iRet;
	}
	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_CAMERA_EXTEND_CFG_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	iRet = CDeviceParaConfigXMLProcess::GetCameraExCfgParseXML(pCameraExCfg, xmlRsp);

	return iRet;
}

// �����ץ�Ĳ�������
IVS_INT32 CDeviceParaConfig::SetCameraSnapshotCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg = (IVS_CAMERA_SNAPSHOT_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraSnapshotCfgGetXML(pDevCode, pDomainCode, pCameraSnapshotCfg, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Camera Snapshot Cfg", "Get xml failed");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_SNAP_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�����ץ�Ĳ�������
IVS_INT32 CDeviceParaConfig::GetCameraSnapshotCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg = (IVS_CAMERA_SNAPSHOT_CFG*)pBuffer;
	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CameraImgCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_SNAP_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}
	iRet = CDeviceParaConfigXMLProcess::GetCameraSnapshotCfgParseXML(pCameraSnapshotCfg, xmlRsp);

	return iRet;
}

// ���������ǰ��¼����������
IVS_INT32 CDeviceParaConfig::SetCameraRecordStreamCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
    // edit by wanglei 00165153:2013.10.14 DTS2013101404169 ��IVS_UINT32 - > IVS_CAMERA_RECORD_STREAM_CFG��
    // CONFIG_CAMERA_RECORD_STREAM_CFG ͨ��SDK�ӿڵ��ò�ѯǰ��¼�������������᷵��109100005�����ڴ�ʧ��
	// IVS_UINT32* pCameraRecordStream = (IVS_UINT32*)pBuffer;
    IVS_CAMERA_RECORD_STREAM_CFG* pCameraRecordStream = (IVS_CAMERA_RECORD_STREAM_CFG*) pBuffer;

	CXml xmlReq;

    // edit by wanglei 00165153:2013.10.14 DTS2013101404169��pCameraRecordStream ->  pCameraRecordStream->uiStreamType��
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraRecordStreamCfgGetXML(pDevCode, pDomainCode, pCameraRecordStream->uiStreamType, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Camera Record Stream Cfg", "Get xml failed");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_RECORD_STREAM_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�����ǰ��¼����������
IVS_INT32 CDeviceParaConfig::GetCameraRecordStreamCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    // edit by wanglei 00165153:2013.10.14 DTS2013101404169 ��IVS_UINT32 - > IVS_CAMERA_RECORD_STREAM_CFG��
    // CONFIG_CAMERA_RECORD_STREAM_CFG ͨ��SDK�ӿڵ��ò�ѯǰ��¼�������������᷵��109100005�����ڴ�ʧ��
    // IVS_UINT32* pCameraRecordStream = (IVS_UINT32*)pBuffer;
    IVS_CAMERA_RECORD_STREAM_CFG* pCameraRecordStream = (IVS_CAMERA_RECORD_STREAM_CFG*) pBuffer;

	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Camera Record StreamCfg GetXML fail", "NA");
		return iRet;
	}

	CXml xmlRsp;
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, xmlRsp,m_pUserMgr, NSS_GET_RECORD_STREAM_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

    // edit by wanglei 00165153:2013.10.14 DTS2013101404169��pCameraRecordStream ->  pCameraRecordStream->uiStreamType��
	iRet = CDeviceParaConfigXMLProcess::GetCameraRecordStreamCfgParseXML(&(pCameraRecordStream->uiStreamType), xmlRsp);

	return iRet;
}


// ���������FEC����
IVS_INT32 CDeviceParaConfig::SetCameraFECCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_FEC_CFG* pCameraFEC = (IVS_CAMERA_FEC_CFG*) pBuffer;

	CXml xmlReq;
	IVS_INT32 iRet = CDeviceParaConfigXMLProcess::SetCameraFECCfgGetXML(pDevCode, pDomainCode, pCameraFEC, xmlReq);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Set Camera FEC Cfg", "Get xml failed");
		return iRet;
	}

	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_SET_FEC_PARA_REQ, BUILDRET, pDomainCode);

	return iRet;
}

// ��ѯ�����FEC����
IVS_INT32 CDeviceParaConfig::GetCameraFECCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pBuffer, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(m_pUserMgr, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_CAMERA_FEC_CFG* pCameraFEC = (IVS_CAMERA_FEC_CFG*) pBuffer;

	CXml xmlReq;
	IVS_INT32 iRet = CXmlProcess::GetCommConfigGetXML(pDomainCode, pDevCode, xmlReq, SDKCAMERA);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get Camera Record StreamCfg GetXML fail", "NA");
		return iRet;
	}
	iRet = CXmlProcess::GetCommSendMgr(xmlReq, m_pUserMgr, NSS_GET_FEC_PARA_REQ, BUILSTRING, pDomainCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get CommBuildRet fail", "NA");
		return iRet;
	}

	iRet = CDeviceParaConfigXMLProcess::GetCameraFECCfgParseXML(pCameraFEC, xmlReq);

	return iRet;
}

