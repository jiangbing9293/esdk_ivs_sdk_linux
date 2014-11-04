/********************************************************************
filename : DeviceParaConfigXMLProcess.cpp
author : gwx151720
created : 2012/12/03
description : 
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/
#include "DeviceParaConfigXMLProcess.h"
#include "ToolsHelp.h"
#include "devEnum.h"
#include "XmlProcess.h"
#include "IVS_Trace.h"

// ��ѯ���豸��������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetMainDevConfigParseXml(IVS_DEVICE_CFG* pMainDevCfg, CXml& xml)
{
    CHECK_POINTER(pMainDevCfg,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

    if (!xml.FindElemEx("Content/DevInfo"))
    {
        return IVS_XML_INVALID;
    }
    const IVS_INT32 maxLen = 120;
    IVS_CHAR chTemp[maxLen];
    const char* szElemValue = NULL;
    IVS_INT32 iTemp = 0;
    GET_ELEM_VALUE_CHAR("DevCode", szElemValue, pMainDevCfg->stDeviceInfo.cCode, IVS_DEV_CODE_LEN, xml);
    GET_ELEM_VALUE_CHAR("DevName", szElemValue, pMainDevCfg->stDeviceInfo.cName, IVS_DEVICE_NAME_LEN, xml);
    memset(chTemp, 0, maxLen);
    GET_ELEM_VALUE_CHAR("DevProtocolType", szElemValue, pMainDevCfg->cProtocolType, IVS_MAX_PROTOCOLTYPE_NAME_LEN, xml);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("DevLoginType", szElemValue, iTemp, xml);
    pMainDevCfg->iLoginType = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    memset(chTemp, 0, maxLen);
    GET_ELEM_VALUE_CHAR("DevVendorType", szElemValue, pMainDevCfg->stDeviceInfo.cVendorType, IVS_MAX_VENDOR_TYPE_NAME_LEN, xml);
    
    memset(chTemp, 0, maxLen);
    GET_ELEM_VALUE_CHAR("DevFormType", szElemValue, chTemp, maxLen, xml);
    pMainDevCfg->stDeviceInfo.uiType = CDevEnum::GetDevTypeEnum(chTemp);
    GET_ELEM_VALUE_CHAR("DevModelType", szElemValue, pMainDevCfg->stDeviceInfo.cModel, IVS_DEV_MODEL_LEN, xml);

	GET_ELEM_VALUE_CHAR("DevSN", szElemValue, pMainDevCfg->stDeviceInfo.cSerialNumber, IVS_DEV_SERIALNO_LEN, xml);

    memset(chTemp, 0, maxLen);
	GET_ELEM_VALUE_NUM("DevAlarmEnable", szElemValue, iTemp, xml);
	pMainDevCfg->bEnableAlarm = iTemp;
	iTemp = 0;
    GET_ELEM_VALUE_CHAR("VideoFormat", szElemValue, chTemp, maxLen, xml);
    pMainDevCfg->uiVideoFormat = (IVS_UINT32)CDevEnum::GetVideoFormatEnum(chTemp);

    GET_ELEM_VALUE_NUM("DirectConnectFirst", szElemValue, iTemp, xml);
    pMainDevCfg->uiDirectConnectFirst = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;

    GET_ELEM_VALUE_CHAR("NVRCode", szElemValue, pMainDevCfg->cNvrCode, IVS_DEV_CODE_LEN, xml);
    GET_ELEM_VALUE_CHAR("DCGCode", szElemValue, pMainDevCfg->cTasCode, IVS_DEV_CODE_LEN, xml);

    GET_ELEM_VALUE_CHAR("DevUser", szElemValue, pMainDevCfg->cDevUser, IVS_NAME_LEN, xml);
    GET_ELEM_VALUE_CHAR("DevPWD", szElemValue, pMainDevCfg->cDevPasswd, IVS_PWD_LEN, xml);
    GET_ELEM_VALUE_CHAR("DevRegPWD", szElemValue, pMainDevCfg->cDevRegPasswd, IVS_PWD_LEN, xml);

    GET_ELEM_VALUE_NUM("BlanceSupport", szElemValue, pMainDevCfg->bEnableSchedule, xml);
    GET_ELEM_VALUE_NUM("MaxVideoNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxVideoNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("MaxAudioInNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxAudioInNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("MaxAudioOutNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxAudioOutNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("MaxAlarmInNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxAlarmInNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("MaxAlarmOutNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxAlarmOutNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
    GET_ELEM_VALUE_NUM("MaxSerialNum", szElemValue, iTemp, xml);
    pMainDevCfg->stDeviceChannel.uiMaxSerialNum = static_cast<IVS_UINT32>(iTemp);
    iTemp = 0;
	// modify by limingjie lWX175483 2013-7-30 ����һ���ֶ� DTS2013072504738
	GET_ELEM_VALUE_CHAR("ParentCode", szElemValue, pMainDevCfg->stDeviceInfo.cParentCode, IVS_DEV_CODE_LEN, xml);

    xml.OutOfElem();
    xml.OutOfElem();

    return IVS_SUCCEED;
}

// ��������ѡ�����������������Ӧ
IVS_INT32 CDeviceParaConfigXMLProcess::ParseCameraBriefInfoRsp(CXml& xml,IVS_CAMERA_CFG* pCameraCfg)
{
	CHECK_POINTER(pCameraCfg,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_INT32 iRet = IVS_FAIL;

	if (!xml.FindElemEx("Content/VideoInChanInfo"))
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "!xml.FindElemEx(Content/VideoInChanInfo)", "NA");
		return iRet;
	}   

	// ����VideoInChanInfoԪ��
	(void)xml.IntoElem();
	const char* szElemValue = NULL;

	// �������������ParentCode
	GET_ELEM_VALUE_CHAR("CameraCode",szElemValue,pCameraCfg->cCameraCode,IVS_DEV_CODE_LEN,xml);
	// �������豸����ParentCode
	GET_ELEM_VALUE_CHAR("ParentCode",szElemValue,pCameraCfg->cParentCode,IVS_DEV_CODE_LEN,xml);
	// ������Ƶͨ����������VideoInIndex
	GET_ELEM_VALUE_NUM_FOR_UINT("VideoInIndex",szElemValue,pCameraCfg->uiVideoChannel,xml);
	//CameraType
	const int tempLength = 10;
	IVS_CHAR tempChar[tempLength]={0};
	GET_ELEM_VALUE_CHAR("CameraType",szElemValue,tempChar,tempLength,xml);
	tempChar[tempLength-1] = 0;
	if(0 == strcmp(tempChar,"FIX"))
	{
		pCameraCfg->uiCameraType = CAMERA_FIX;
	}
	else if(0 == strcmp(tempChar,"PTZ"))
	{
		pCameraCfg->uiCameraType = CAMERA_PTZ;
	}
	else if(0 == strcmp(tempChar,"DOME"))
	{
		pCameraCfg->uiCameraType = CAMERA_DOME;
	}
	else if(0 == strcmp(tempChar,"FIX_DOME"))
	{
		pCameraCfg->uiCameraType = CAMERA_FIX_DOME;
	}
	else 
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse CameraBriefInfoRsp fail","Error CameraType:[%s]",tempChar);
		return iRet; 
	}

	// IsBroadcast
	GET_ELEM_VALUE_NUM("IsBroadcast",szElemValue,pCameraCfg->bSupportBroadcast,xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("BroadcastIPType", szElemValue, pCameraCfg->stBroadcast.stIP.uiIPType, xml);
	// BroadcastIP
	GET_ELEM_VALUE_CHAR("BroadcastIP",szElemValue,pCameraCfg->stBroadcast.stIP.cIP,IVS_IP_LEN,xml);
	// BroadcastPort
	GET_ELEM_VALUE_NUM_FOR_UINT("BroadcastPort",szElemValue,pCameraCfg->stBroadcast.uiPort,xml);
	// BroadcastTTL
	GET_ELEM_VALUE_NUM_FOR_UINT("BroadcastTTL",szElemValue,pCameraCfg->stBroadcast.uiTTL,xml);
	// Longitude
	GET_ELEM_VALUE_NUM_FOR_DOUBLE("Longitude", szElemValue, pCameraCfg->stPosition.dLongitude, xml);

	//GET_ELEM_VALUE_NUM_FOR_UINT("Longitude",szElemValue,pCameraCfg->stPosition.uiLongitude,xml);
	// Latitude
	GET_ELEM_VALUE_NUM_FOR_DOUBLE("Latitude", szElemValue, pCameraCfg->stPosition.dLatitude, xml);
	//GET_ELEM_VALUE_NUM_FOR_UINT("Latitude",szElemValue,pCameraCfg->stPosition.uiLatitude,xml);
	// CameraLocation
	GET_ELEM_VALUE_CHAR("CameraLocation",szElemValue,pCameraCfg->stPosition.cCameraLocation,IVS_DESCRIBE_LEN,xml);
	// Height
	//GET_ELEM_VALUE_NUM_FOR_UINT("Height",szElemValue,pCameraCfg->stPosition.uiHeight,xml);
	GET_ELEM_VALUE_NUM_FOR_DOUBLE("Height", szElemValue, pCameraCfg->stPosition.dHeight, xml);
	// CameraName
	GET_ELEM_VALUE_CHAR("CameraName",szElemValue,pCameraCfg->cCameraName,IVS_CAMERA_NAME_LEN,xml);
	// VoiceInEnable
	IVS_CHAR tempChar2[tempLength]={0};
	GET_ELEM_VALUE_CHAR("VoiceInEnable",szElemValue,tempChar2,tempLength,xml);
	tempChar2[tempLength-1]=0;
	// ��·��Ƶ�Ƿ�����	0-ͣ�� 1-����
	if (0 == strcmp(tempChar2,"1"))
	{
		pCameraCfg->bEnableVoice = 1;
	}
	else if (0 == strcmp(tempChar2,"0"))
	{
		pCameraCfg->bEnableVoice = 0;
	}
	else
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse CameraBriefInfoRsp fail","Error VoiceInEnable:[%s]",tempChar2);
		return iRet; 
	}
	// VoiceOutCode
	GET_ELEM_VALUE_CHAR("VoiceOutCode", szElemValue, pCameraCfg->cVoiceOutCode, IVS_DEV_CODE_LEN, xml);
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ������̨������Ӧxml,�ŵ��ṹ��
IVS_INT32 CDeviceParaConfigXMLProcess::ParseCameraPTZCfgRsp(CXml& xml,IVS_PTZ_CFG* pCameraPTZCfg, const IVS_CHAR* pDomainCode)
{
	CHECK_POINTER(pCameraPTZCfg,IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_INT32 iRet = IVS_FAIL;

	if (!xml.FindElemEx("Content/PTZInfo"))
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse CameraPTZCfgRsp fail", "!xml.FindElemEx(Content/PTZInfo)");
		return iRet;
	}   

	//����PTZInfoԪ��
	(void)xml.IntoElem();
	const char* szElemValue = NULL;
	//��ȡDecoderType
	GET_ELEM_VALUE_NUM_FOR_UINT("DecoderType",szElemValue,pCameraPTZCfg->uiDecoderType,xml);
	//��ȡDecoderAddress
	GET_ELEM_VALUE_NUM_FOR_UINT("DecoderAddress",szElemValue,pCameraPTZCfg->uiDecoderAddress,xml);
	//��ȡPosNegDirectControl
	const int tempLength = 10;
	IVS_CHAR tempChar[tempLength]={0};
	GET_ELEM_VALUE_CHAR("PosNegDirectControl",szElemValue,tempChar,tempLength,xml);
	tempChar[tempLength-1] = 0;
	if(0 == strcmp(tempChar,"FORWARD"))
	{
		pCameraPTZCfg->bPosNegDirectControl = 0;
	}
	else if(0 == strcmp(tempChar,"BACK"))
	{
		pCameraPTZCfg->bPosNegDirectControl = 1;
	}
	else
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse CameraPTZCfgRsp fail","Error PosNegDirectControl:[%s]",tempChar);
		return iRet; 
	}
	
	IVS_CHAR cSerialCode[IVS_DEV_SERIALNO_LEN + 1] = {0};
	//��ȡSerialCode
	GET_ELEM_VALUE_CHAR("SerialCode",szElemValue, cSerialCode,IVS_DEV_SERIALNO_LEN,xml);
	std::string strSerialCode = cSerialCode;
	std::string strDomainCode = pDomainCode;
	strSerialCode.append("#").append(strDomainCode);
	
	(void)CToolsHelp::Memcpy(pCameraPTZCfg->cSerialCode, IVS_DEV_SERIALNO_LEN, strSerialCode.c_str(), IVS_DEV_SERIALNO_LEN);

	//����ֵΪĬ��
	memset(pCameraPTZCfg->cReserve,0,32);

	iRet = IVS_SUCCEED;

	return iRet;
}

// ��������������Ӧxml,�ŵ��ṹ��
IVS_INT32 CDeviceParaConfigXMLProcess::ParseAudioCfgRsp(CXml& xml,IVS_AUDIO_CFG* pAudioCfg)
{
	CHECK_POINTER(pAudioCfg,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_INT32 iRet = IVS_FAIL;

	if (!xml.FindElemEx("Content/VoiceChanInfo"))
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse AudioCfgRsp fail", "!xml.FindElemEx(Content/VoiceChanInfo)");
		return iRet;
	}   

	// ����VoiceChanInfoԪ��
	(void)xml.IntoElem();
	const char* szElemValue = NULL;

	// ������VoiceName
	GET_ELEM_VALUE_CHAR("VoiceName",szElemValue,pAudioCfg->cAudioName,IVS_AUDIO_NAME_LEN,xml);

	// ������ChannelType
	{
		const int tempLength = 10;
		IVS_CHAR tempChar[tempLength] = {0};
		GET_ELEM_VALUE_CHAR("ChannelType",szElemValue,tempChar,tempLength,xml);
		tempChar[tempLength-1]=0;
		// ͨ������ 0-INPUT 1-OUTPUT
		if(0 == strcmp(tempChar,"INPUT"))
		{
			pAudioCfg->uiChannelType = 0;
		}
		else if(0 == strcmp(tempChar,"OUTPUT"))
		{
			pAudioCfg->uiChannelType = 1;
		}
		else
		{
			iRet = IVS_XML_INVALID;
			BP_RUN_LOG_ERR(iRet, "Parse CameraBriefInfoRsp fail","Error DataFormat:[%s]",tempChar);
			return iRet;
		}		
	}

	// ������VoiceIndex
	GET_ELEM_VALUE_NUM_FOR_UINT("VoiceIndex",szElemValue,pAudioCfg->uiAudioIndex,xml);


	// ������VoiceParam
	if (!xml.FindElemEx("Content/VoiceChanInfo/VoiceParam"))
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse AudioCfgRsp fail", "!xml.FindElemEx(Content/VoiceChanInfo/VoiceParam)");
		return iRet;
	}
	//����VoiceParamԪ��
	(void)xml.IntoElem();	
	{
		// ������Pitch
		GET_ELEM_VALUE_NUM_FOR_UINT("Pitch",szElemValue,pAudioCfg->stAudioParam.uiPitch,xml);
		// ������Volume
		GET_ELEM_VALUE_NUM_FOR_UINT("Volume",szElemValue,pAudioCfg->stAudioParam.uiVolume,xml);
		// ������InputType
		GET_ELEM_VALUE_NUM_FOR_UINT("InputType",szElemValue,pAudioCfg->stAudioParam.uiInputType,xml);

		// ������DataFormat G711A		G711U			G726			AAC
		const int tempLength = 10;
		IVS_CHAR tempChar[tempLength] = {0};
		GET_ELEM_VALUE_CHAR("DataFormat",szElemValue,tempChar,tempLength,xml);
		pAudioCfg->stAudioParam.uiDataFormat = static_cast<IVS_UINT32>(CDevEnum::GetDataFormatINT(tempChar));

		// ������SampleIngrate
		GET_ELEM_VALUE_NUM_FOR_UINT("SampleIngrate",szElemValue,pAudioCfg->stAudioParam.uiSampleRate,xml);
		// ������Channel
		GET_ELEM_VALUE_NUM_FOR_UINT("Channel",szElemValue,pAudioCfg->stAudioParam.uiChannel,xml);
		// ������Gain
		GET_ELEM_VALUE_NUM_FOR_UINT("Gain",szElemValue,pAudioCfg->stAudioParam.uiGain,xml);
		// ������SampleBits
		GET_ELEM_VALUE_NUM_FOR_UINT("SampleBits",szElemValue,pAudioCfg->stAudioParam.uiSampleBits,xml);
	}


	
	// ����ֵΪĬ��
	memset(pAudioCfg->cReserve,0,32);
	memset(pAudioCfg->stAudioParam.cReserve,0,32);

	iRet = IVS_SUCCEED;
	
	return iRet;
}

// �������豸����ƴ��XML
IVS_INT32 CDeviceParaConfigXMLProcess::SetMainParaConfigGetXml(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_DEVICE_CFG* pDevCfg, const IVS_CHAR* pDevPWD, const IVS_CHAR* pDevRegPWD, CXml& xmlTemp)
{
    CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);  
    CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

 	(void)xmlTemp.AddDeclaration("1.0","UTF-8","");
    (void)xmlTemp.AddElem("Content");
    
    (void)xmlTemp.AddChildElem("DomainCode"); 
	(void)xmlTemp.IntoElem();
    (void)xmlTemp.SetElemValue(pDomainCode);  
    (void)xmlTemp.AddElem("DevInfo");
    // ���豸����
    (void)xmlTemp.AddChildElem("DevCode");
    (void)xmlTemp.IntoElem();
    (void)xmlTemp.SetElemValue(pDevCode);   
    // ���豸�豸����
    (void)xmlTemp.AddElem("DevName");    
	IVS_CHAR cDevName[IVS_DEVICE_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevName, IVS_DEVICE_NAME_LEN, pDevCfg->stDeviceInfo.cName, IVS_DEVICE_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevName, IVS_DEVICE_NAME_LEN, pDevCfg->stDeviceInfo.cName, IVS_DEVICE_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDevName);   
    // ���豸Э������
    (void)xmlTemp.AddElem("DevProtocolType");
	IVS_CHAR cDevProtocolType[IVS_MAX_PROTOCOLTYPE_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevProtocolType, IVS_MAX_PROTOCOLTYPE_NAME_LEN, pDevCfg->cProtocolType, IVS_MAX_PROTOCOLTYPE_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevProtocolType, IVS_MAX_PROTOCOLTYPE_NAME_LEN, pDevCfg->cProtocolType, IVS_MAX_PROTOCOLTYPE_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDevProtocolType);   
    // ��֤����
    (void)xmlTemp.AddElem("DevLoginType");    
    (void)xmlTemp.SetElemValue(CToolsHelp::Int2Str((IVS_INT32)pDevCfg->iLoginType).c_str());   
    // ���豸�ṩ������
    (void)xmlTemp.AddElem("DevVendorType");   
	IVS_CHAR cDevVendorType[IVS_MAX_VENDOR_TYPE_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevVendorType, IVS_MAX_VENDOR_TYPE_NAME_LEN, pDevCfg->stDeviceInfo.cVendorType, IVS_MAX_VENDOR_TYPE_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevVendorType, IVS_MAX_VENDOR_TYPE_NAME_LEN, pDevCfg->stDeviceInfo.cVendorType, IVS_MAX_VENDOR_TYPE_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDevVendorType);   
    // ���豸��̬����
    (void)xmlTemp.AddElem("DevFormType");    
    (void)xmlTemp.SetElemValue(CDevEnum::GetDevType((IVS_INT32)pDevCfg->stDeviceInfo.uiType));   
    // ���豸�ͺ�
    (void)xmlTemp.AddElem("DevModelType");    
	IVS_CHAR cDevModelType[IVS_DEV_MODEL_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevModelType, IVS_DEV_MODEL_LEN, pDevCfg->stDeviceInfo.cModel, IVS_DEV_MODEL_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevModelType, IVS_DEV_MODEL_LEN, pDevCfg->stDeviceInfo.cModel, IVS_DEV_MODEL_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDevModelType);   
	// ���豸SN��
	(void)xmlTemp.AddElem("DevSN");
	IVS_CHAR cDevSN[IVS_DEV_SERIALNO_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevSN, IVS_DEV_SERIALNO_LEN, pDevCfg->stDeviceInfo.cSerialNumber, IVS_DEV_SERIALNO_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevSN, IVS_DEV_SERIALNO_LEN, pDevCfg->stDeviceInfo.cSerialNumber, IVS_DEV_SERIALNO_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cDevSN);
    // �豸�澯ʹ��
    (void)xmlTemp.AddElem("DevAlarmEnable");    
    (void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(pDevCfg->bEnableAlarm).c_str());   
    // ��Ƶ��ʽ
	if (0 != pDevCfg->uiVideoFormat)
	{
		(void)xmlTemp.AddElem("VideoFormat");    
		(void)xmlTemp.SetElemValue(CDevEnum::GetVideoFormatType((IVS_INT32)pDevCfg->uiVideoFormat)); 
	}
    // �Ƿ�ֱ������
    (void)xmlTemp.AddElem("DirectConnectFirst");    
    (void)xmlTemp.SetElemValue(CToolsHelp::Int2Str((IVS_INT32)pDevCfg->uiDirectConnectFirst).c_str());   
    // �豸����NVR����
    (void)xmlTemp.AddElem("NVRCode");  
	IVS_CHAR cNVRCode[IVS_NVR_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cNVRCode, IVS_NVR_CODE_LEN, pDevCfg->cNvrCode, IVS_NVR_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cNVRCode, IVS_NVR_CODE_LEN, pDevCfg->cNvrCode, IVS_NVR_CODE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cNVRCode); 
    // �豸����TAS����
    (void)xmlTemp.AddElem("DCGCode");  
	IVS_CHAR cDCGCode[IVS_TAS_CODE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDCGCode, IVS_TAS_CODE_LEN, pDevCfg->cTasCode, IVS_TAS_CODE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDCGCode, IVS_TAS_CODE_LEN, pDevCfg->cTasCode, IVS_TAS_CODE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDCGCode);   
    // �豸��¼�û�
    (void)xmlTemp.AddElem("DevUser");  
	IVS_CHAR cDevUser[IVS_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cDevUser, IVS_NAME_LEN, pDevCfg->cDevUser, IVS_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevUser, IVS_NAME_LEN, pDevCfg->cDevUser, IVS_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
    (void)xmlTemp.SetElemValue(cDevUser);   
    // �豸��¼����
    (void)xmlTemp.AddElem("DevPWD");    
    (void)xmlTemp.SetElemValue(pDevPWD);   
    // �豸��ƽ̨����
    (void)xmlTemp.AddElem("DevRegPWD");
    (void)xmlTemp.SetElemValue(pDevRegPWD); 
    // �Ƿ�֧�ּ�Ⱥ����
    (void)xmlTemp.AddElem("BlanceSupport");    
    (void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(pDevCfg->bEnableSchedule).c_str()); 
    xmlTemp.OutOfElem();

    return IVS_SUCCEED;
}

// ���޸��������������������xml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraBriefCfgGetXml( 
	const IVS_CHAR* pDomainCode, 
	const IVS_CHAR* pCameraCode, 
	const IVS_CAMERA_CFG* pCameraCfg,
	CXml& xmlTemp )
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);  
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xmlTemp.AddDeclaration("1.0","UTF-8","");
	(void)xmlTemp.AddElem("Content");
		
	(void)xmlTemp.AddChildElem("DomainCode");   
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pDomainCode);  

	// ����VideoInChanInfo
	(void)xmlTemp.AddElem("VideoInChanInfo");
		
	(void)xmlTemp.AddChildElem("CameraCode");	
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pCameraCode);

	(void)xmlTemp.AddElem("CameraType");
	(void)xmlTemp.IntoElem();
	switch (pCameraCfg->uiCameraType)
	{
	case CAMERA_FIX:
		{
			(void)xmlTemp.SetElemValue("FIX");
			break;
		}
	case CAMERA_PTZ:
		{
			(void)xmlTemp.SetElemValue("PTZ");
			break;
		}
	case CAMERA_DOME:
		{
			(void)xmlTemp.SetElemValue("DOME");
			break;
		}
	case CAMERA_FIX_DOME:
		{
			(void)xmlTemp.SetElemValue("FIX_DOME");
			break;
		}
	default:
		{
			(void)xmlTemp.SetElemValue("FIX");
		}
	}

	(void)xmlTemp.AddElem("IsBroadcast");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pCameraCfg->bSupportBroadcast?"1":"0");

	(void)xmlTemp.AddElem("BroadcastIPType");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pCameraCfg->stBroadcast.stIP.uiIPType?"1":"0");

	(void)xmlTemp.AddElem("BroadcastIP");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pCameraCfg->stBroadcast.stIP.cIP);

	(void)xmlTemp.AddElem("BroadcastPort");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraCfg->stBroadcast.uiPort)).c_str());

	(void)xmlTemp.AddElem("BroadcastTTL");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraCfg->stBroadcast.uiTTL)).c_str());

	(void)xmlTemp.AddElem("Longitude");
	(void)xmlTemp.IntoElem();
	IVS_CHAR cLongitude[STR_LEN+1] = {0};
	if (!CToolsHelp::Snprintf(cLongitude, STR_LEN, "%.12f", pCameraCfg->stPosition.dLongitude))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLongitude, STR_LEN, '%.12f', pCameraCfg->stPosition.dLongitude) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cLongitude);
	//(void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraCfg->stPosition.uiLongitude)).c_str());

	(void)xmlTemp.AddElem("Latitude");
	(void)xmlTemp.IntoElem();
	IVS_CHAR cLatitude[STR_LEN+1] = {0};
	if (!CToolsHelp::Snprintf(cLatitude, STR_LEN, "%.12f", pCameraCfg->stPosition.dLatitude))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLatitude, STR_LEN, '%.12f', pCameraCfg->stPosition.dLatitude) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cLatitude);
	//(void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraCfg->stPosition.uiLatitude)).c_str());

	(void)xmlTemp.AddElem("CameraLocation");
	(void)xmlTemp.IntoElem();
	IVS_CHAR cCameraLocation[IVS_DESCRIBE_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cCameraLocation, IVS_DESCRIBE_LEN, pCameraCfg->stPosition.cCameraLocation, IVS_DESCRIBE_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cCameraLocation, IVS_DESCRIBE_LEN, pCameraCfg->stPosition.cCameraLocation, IVS_DESCRIBE_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cCameraLocation);

	(void)xmlTemp.AddElem("Height");
	(void)xmlTemp.IntoElem();
	IVS_CHAR cHeight[STR_LEN] = {0};
	if (!CToolsHelp::Snprintf(cHeight, STR_LEN, "%.2f", pCameraCfg->stPosition.dHeight))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cHeight, STR_LEN, '%.2f', pCameraCfg->stPosition.dHeight) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cHeight);
	//(void)xmlTemp.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraCfg->stPosition.uiHeight)).c_str());

	(void)xmlTemp.AddElem("CameraName");
	(void)xmlTemp.IntoElem();
	IVS_CHAR cCameraName[IVS_CAMERA_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cCameraName, IVS_CAMERA_NAME_LEN, pCameraCfg->cCameraName, IVS_CAMERA_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cCameraName, IVS_CAMERA_NAME_LEN, pCameraCfg->cCameraName, IVS_CAMERA_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xmlTemp.SetElemValue(cCameraName);
		
	(void)xmlTemp.AddElem("VoiceInEnable");
	(void)xmlTemp.IntoElem();
	(void)xmlTemp.SetElemValue(pCameraCfg->bEnableVoice?"1":"0");
	
	(void)xmlTemp.AddElem("VoiceOutCode");
	(void)xmlTemp.IntoElem();
	IVS_CHAR chDevCode[IVS_SDK_DEV_CODE_LEN+1] = {0};
	IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};
	// ����DevCode
	IVS_INT32 iRet = CXmlProcess::ParseDevCode(pCameraCfg->cVoiceOutCode, chDevCode, chDomaCode);
	if (IVS_SUCCEED != iRet)
	{
		BP_RUN_LOG_ERR(iRet, "Get DeviceConfig fail", "(ParseDevCode)iRet=%d", iRet);
		return iRet;
	}
	(void)xmlTemp.SetElemValue(chDevCode);
	
	xmlTemp.OutOfElem();

	return IVS_SUCCEED;
}

// ��������ѡ���������ͼ���������Ӧ
IVS_INT32 CDeviceParaConfigXMLProcess::ParseCameraDispalyCfgRsp(CXml& xml,IVS_CAMERA_DISPLAY_CFG* pCameraBriefInfo)
{
	CHECK_POINTER(pCameraBriefInfo,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	IVS_INT32 iRet = IVS_FAIL;

	if (!xml.FindElemEx("Content/ImagingInfo/BasicInfo"))
	{
		iRet = IVS_XML_INVALID;
		BP_RUN_LOG_ERR(iRet, "Parse CameraDispalyCfgRsp fail", "!xml.FindElemEx(Content/ImagingInfo/BasicInfo)");
		return iRet;
	}   

	// ����Ԫ��Content/ImagingInfo/BasicInfo
	(void)xml.IntoElem();
	const char* szElemValue = NULL;
	IVS_INT32 iTemp = 0;
	// �Աȶ�
	GET_ELEM_VALUE_NUM("Contrast", szElemValue,iTemp, xml);
	pCameraBriefInfo->uiContrast = static_cast<IVS_UINT32>(iTemp);

	iTemp = 0;
	// ����
	GET_ELEM_VALUE_NUM("Bright", szElemValue,iTemp, xml);
	pCameraBriefInfo->uiBrightness = static_cast<IVS_UINT32>(iTemp);

	iTemp = 0;
	// ɫ��
	GET_ELEM_VALUE_NUM("Hue", szElemValue,iTemp, xml);
	pCameraBriefInfo->uiHue = static_cast<IVS_UINT32>(iTemp);

	iTemp = 0;
	// ���Ͷ�
	GET_ELEM_VALUE_NUM("Saturation", szElemValue,iTemp, xml);
	pCameraBriefInfo->uiSaturation = static_cast<IVS_UINT32>(iTemp);

	iRet = IVS_SUCCEED;

	return iRet;
}

//��װ�޸����������ͼ�����xml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraBasicImgCfgGetXml( 
	const IVS_CHAR* pDomainCode, 
	const IVS_CHAR* pCameraCode, 
	const IVS_CAMERA_DISPLAY_CFG* pCameraBasicImgCfg,
	CXml& xml )
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraBasicImgCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pCameraCode);

	(void)xml.AddElem("ImagingInfo");
	{	
		(void)xml.IntoElem();
		(void)xml.AddChildElem("BasicInfo");
		{		
			(void)xml.IntoElem();
			(void)xml.AddChildElem("Contrast");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiContrast)).c_str());

			(void)xml.AddElem("Bright");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiBrightness)).c_str());			

			(void)xml.AddElem("Hue");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiHue)).c_str());

			(void)xml.AddElem("Saturation");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiSaturation)).c_str());
			xml.OutOfElem();
		}
		xml.OutOfElem();
	}	
	xml.OutOfElem();	

	return IVS_SUCCEED;
}

//��װ�޸���̨����xml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraPTZCfgGetXml( 
	const IVS_CHAR* pDomainCode, 
	const IVS_CHAR* pCameraCode, 
	const IVS_PTZ_CFG* pCameraBasicImgCfg,
	CXml& reqXml )
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraBasicImgCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)reqXml.AddDeclaration("1.0","UTF-8","");
	(void)reqXml.AddElem("Content");
	(void)reqXml.IntoElem();

	(void)reqXml.AddChildElem("DomainCode");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pDomainCode);

	(void)reqXml.AddElem("CameraCode");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pCameraCode);

	(void)reqXml.AddElem("PTZInfo");
	{
		(void)reqXml.IntoElem();
		(void)reqXml.AddChildElem("DecoderType");
		(void)reqXml.IntoElem();
		(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiDecoderType)).c_str());

		(void)reqXml.AddElem("DecoderAddress");
		(void)reqXml.IntoElem();
		(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraBasicImgCfg->uiDecoderAddress)).c_str());

		(void)reqXml.AddElem("PosNegDirectControl");
		(void)reqXml.IntoElem();
		(void)reqXml.SetElemValue(pCameraBasicImgCfg->bPosNegDirectControl?"BACK":"FORWARD");

		(void)reqXml.AddElem("SerialCode");
		(void)reqXml.IntoElem();

		IVS_CHAR chDevCode[IVS_SDK_DEV_CODE_LEN+1] = {0};
		IVS_CHAR chDomaCode[IVS_DOMAIN_CODE_LEN+1] = {0};
		// ����DevCode
		(void)CXmlProcess::ParseDevCode(pCameraBasicImgCfg->cSerialCode, chDevCode, chDomaCode);
		
		(void)reqXml.SetElemValue(chDevCode);

		reqXml.OutOfElem();
	}
	reqXml.OutOfElem();

	return IVS_SUCCEED;
}

// ��װ�޸��������豸����xml
IVS_INT32 CDeviceParaConfigXMLProcess::SetAudioCfgGetXml( 
	const IVS_CHAR* pDomainCode, 
	const IVS_CHAR* pVoiceCode, 
	const IVS_AUDIO_CFG* pAudioCfg,
	CXml& reqXml )
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pVoiceCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pAudioCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)reqXml.AddDeclaration("1.0","UTF-8","");
	(void)reqXml.AddElem("Content");
	(void)reqXml.IntoElem();

	(void)reqXml.AddChildElem("DomainCode");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pDomainCode);

	(void)reqXml.AddElem("VoiceCode");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(pVoiceCode);

	// VoiceChanInfo
	(void)reqXml.AddElem("VoiceChanInfo");
	(void)reqXml.IntoElem();
			
	// VoiceName
	(void)reqXml.AddChildElem("VoiceName");
	(void)reqXml.IntoElem();
	IVS_CHAR cVoiceName[IVS_AUDIO_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cVoiceName, IVS_AUDIO_NAME_LEN, pAudioCfg->cAudioName, IVS_AUDIO_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cVoiceName, IVS_AUDIO_NAME_LEN, pAudioCfg->cAudioName, IVS_AUDIO_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)reqXml.SetElemValue(cVoiceName);

	// VoiceParam
	(void)reqXml.AddElem("VoiceParam");
	(void)reqXml.IntoElem();
	
	// Pitch
	(void)reqXml.AddChildElem("Pitch");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiPitch)).c_str());

	// Volume
	(void)reqXml.AddElem("Volume");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiVolume)).c_str());
			
	// InputType
	(void)reqXml.AddElem("InputType");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiInputType)).c_str());
			
	// DataFormat
	(void)reqXml.AddElem("DataFormat");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CDevEnum::GetDataFormatCHAR(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiDataFormat)));
		
	// SampleIngrate
	(void)reqXml.AddElem("SampleIngrate");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiSampleRate)).c_str());
			
	// Channel
	(void)reqXml.AddElem("Channel");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiChannel)).c_str());
			
	// Gain
	(void)reqXml.AddElem("Gain");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiGain)).c_str());

	// SampleBits
	(void)reqXml.AddElem("SampleBits");
	(void)reqXml.IntoElem();
	(void)reqXml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAudioCfg->stAudioParam.uiSampleBits)).c_str());
	
	reqXml.OutOfElem();
	
	reqXml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯ�����������������XML
IVS_INT32 CDeviceParaConfigXMLProcess::CameraStreamCfgParseXML(IVS_CAMERA_STREAM_CFG* pCameraStrCfg, CXml &xml)
{
	CHECK_POINTER(pCameraStrCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	const IVS_INT32 maxTempLen = 120;
	if (!xml.FindElemEx("Content/VideoInChanInfo"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "CameraStreamCfg ParseXML fail", "xml.FindElemEx(Content/VideoInChanInfo) fail");
		return IVS_SMU_XML_INVALID;
	}
	(void)xml.IntoElem();
	const char* pTemp = NULL;
	IVS_INT32 iTemp = 0;
	GET_ELEM_VALUE_NUM("StreamNum", pTemp, iTemp, xml);
	pCameraStrCfg->uiStreamInfoNum = static_cast<IVS_UINT32>(iTemp);

	if (!xml.FindElem("StreamList"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "CameraStreamCfg ParseXML fail", "xml.FindElem(StreamList) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	if (!xml.FindElem("StreamInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "CameraStreamCfg ParseXML fail", "xml.FindElem(StreamList) fail");
		return IVS_XML_INVALID;
	}

	IVS_INT32 i = 0;
	do 
	{
		(void)xml.IntoElem();
		GET_ELEM_VALUE_NUM("StreamType", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiStreamType = static_cast<IVS_UINT32>(iTemp);

		IVS_CHAR cEncodeMode[maxTempLen] = {0};
		GET_ELEM_VALUE_CHAR("EncodeMode", pTemp, cEncodeMode, maxTempLen, xml);
		pCameraStrCfg->stStreamInfo[i].uiEncodeType = static_cast<IVS_UINT32>(CDevEnum::GetEnCodeModeEnum(cEncodeMode));

		GET_ELEM_VALUE_NUM("PicQuality", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiPicQuality = static_cast<IVS_UINT32>(iTemp);

		GET_ELEM_VALUE_CHAR("Resolution", pTemp, pCameraStrCfg->stStreamInfo[i].cResolution, IVS_RESOLUTION_LEN, xml);
		//pCameraStrCfg->stStreamInfo[i].uiResolution = static_cast<IVS_UINT32>(CDevEnum::GetResolutionEnum(cResolution));

		GET_ELEM_VALUE_NUM("BitRate", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiBitRate = static_cast<IVS_UINT32>(iTemp);

		IVS_CHAR cBitRateType[maxTempLen] = {0};
		GET_ELEM_VALUE_CHAR("BitRateType", pTemp, cBitRateType, maxTempLen, xml);
		pCameraStrCfg->stStreamInfo[i].uiBitRateType = static_cast<IVS_UINT32>(CDevEnum::GetBitRateTypeEnum(cBitRateType));

		GET_ELEM_VALUE_NUM("FrameRate", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiFrameRate = static_cast<IVS_UINT32>(iTemp);

		IVS_CHAR cProfile[maxTempLen] = {0};
		GET_ELEM_VALUE_CHAR("Profile", pTemp, cProfile, maxTempLen, xml);
		if (1 == pCameraStrCfg->stStreamInfo[i].uiEncodeType)
		{
			pCameraStrCfg->stStreamInfo[i].uiProfile = static_cast<IVS_UINT32>(CDevEnum::GetH264ProfileEnum(cProfile));
		}
		if (2 == pCameraStrCfg->stStreamInfo[i].uiEncodeType)
		{
			pCameraStrCfg->stStreamInfo[i].uiProfile = static_cast<IVS_UINT32>(CDevEnum::GetMjpegProfileEnum(cProfile));
		}
		
		GET_ELEM_VALUE_NUM("IframeInterval", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiFrameInterval = static_cast<IVS_UINT32>(iTemp);

		GET_ELEM_VALUE_NUM("TransProtocol", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiTransProtocol = static_cast<IVS_UINT32>(iTemp);

		GET_ELEM_VALUE_NUM("PKTProtocol", pTemp, iTemp, xml);
		pCameraStrCfg->stStreamInfo[i].uiPktProtocol = static_cast<IVS_UINT32>(iTemp);
		i++;
		xml.OutOfElem();

	} while (xml.NextElem() && i < IVS_MAX_STREAM_NUM);
	xml.OutOfElem();

	return IVS_SUCCEED;
}


// �޸��������������
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraStreamCfgGetXML(const IVS_CAMERA_STREAM_CFG* pCameraStrCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml)
{
	CHECK_POINTER(pCameraStrCfg, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("VideoInChanInfo");
	{
		(void)xml.IntoElem();
		(void)xml.AddChildElem("CameraCode");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(pDevCode);

		(void)xml.AddElem("StreamNum");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->uiStreamInfoNum)).c_str());

		(void)xml.AddElem("StreamList");
		(void)xml.IntoElem();
		IVS_UINT32 i = 0;

		for(i = 0;i < pCameraStrCfg->uiStreamInfoNum; i++){
			i == 0 ? (void)xml.AddChildElem("StreamInfo"):(void)xml.AddElem("StreamInfo");
			(void)xml.IntoElem();
			(void)xml.AddChildElem("StreamType");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiStreamType)).c_str());

			(void)xml.AddElem("EncodeMode");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CDevEnum::GetEnCodeModeCH(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiEncodeType)));

			(void)xml.AddElem("PicQuality");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiPicQuality)).c_str());

			(void)xml.AddElem("Resolution");
			(void)xml.IntoElem();
			IVS_CHAR cResolution[IVS_RESOLUTION_LEN + 1] = {0};
			if (!CToolsHelp::Memcpy(cResolution, IVS_RESOLUTION_LEN, pCameraStrCfg->stStreamInfo[i].cResolution, IVS_RESOLUTION_LEN))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cResolution, IVS_RESOLUTION_LEN, pCameraStrCfg->stStreamInfo->cResolution, IVS_RESOLUTION_LEN) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cResolution);
			//(void)xml.SetElemValue(CDevEnum::GetResolutionCH(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiResolution)));

			(void)xml.AddElem("BitRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiBitRate)).c_str());

			(void)xml.AddElem("BitRateType");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CDevEnum::GetBitRateTypeCH(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiBitRateType)));

			(void)xml.AddElem("FrameRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiFrameRate)).c_str());

			(void)xml.AddElem("Profile");
			(void)xml.IntoElem();
			if (1 == pCameraStrCfg->stStreamInfo[i].uiEncodeType)
			{
				(void)xml.SetElemValue(CDevEnum::GetH264ProfileCH(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiProfile)));
			}
			if (2 == pCameraStrCfg->stStreamInfo[i].uiEncodeType)
			{
				(void)xml.SetElemValue(CDevEnum::GetMjpegProfileCH(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiProfile)));
			}
			
			(void)xml.AddElem("IframeInterval");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiFrameInterval)).c_str());

			(void)xml.AddElem("TransProtocol");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiTransProtocol)).c_str());

			(void)xml.AddElem("PKTProtocol");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfg->stStreamInfo[i].uiPktProtocol)).c_str());
			xml.OutOfElem();
		}
		xml.OutOfElem();
		xml.OutOfElem();
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}


// �޸���������������������¼�����ͼ������������Ϣ��
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraStreamCfgEventExGetXML(const IVS_CAMERA_STREAM_EVENTEX_CFG* pCameraStrCfgEx, 
	const IVS_CHAR* pDomainCode, 
	const IVS_CHAR* pDevCode, 
	CXml &xml)
{
	CHECK_POINTER(pCameraStrCfgEx, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("VideoInChanInfo");
	{
		(void)xml.IntoElem();
		(void)xml.AddChildElem("CameraCode");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(pDevCode);

		(void)xml.AddElem("StreamNum");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->uiStreamInfoNum)).c_str());

		(void)xml.AddElem("StreamList");
		(void)xml.IntoElem();
		IVS_UINT32 i = 0;

		for(i = 0;i < pCameraStrCfgEx->uiStreamInfoNum; i++){
			i == 0 ? (void)xml.AddChildElem("StreamInfo"):(void)xml.AddElem("StreamInfo");
			(void)xml.IntoElem();
			(void)xml.AddChildElem("StreamType");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiStreamType)).c_str());

			(void)xml.AddElem("EncodeMode");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CDevEnum::GetEnCodeModeCH(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiEncodeType)));

			(void)xml.AddElem("PicQuality");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiPicQuality)).c_str());

			(void)xml.AddElem("Resolution");
			(void)xml.IntoElem();
			IVS_CHAR cResolution[IVS_RESOLUTION_LEN + 1] = {0};
			if (!CToolsHelp::Memcpy(cResolution, IVS_RESOLUTION_LEN, pCameraStrCfgEx->stStreamInfo[i].cResolution, IVS_RESOLUTION_LEN))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cResolution, IVS_RESOLUTION_LEN, pCameraStrCfg->stStreamInfo->cResolution, IVS_RESOLUTION_LEN) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cResolution);

			(void)xml.AddElem("BitRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiBitRate)).c_str());

			(void)xml.AddElem("BitRateType");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CDevEnum::GetBitRateTypeCH(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiBitRateType)));

			(void)xml.AddElem("FrameRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiFrameRate)).c_str());

			(void)xml.AddElem("EventRecordAdapt");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiEventRecordAdapt)).c_str());

			(void)xml.AddElem("EventBitRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiEventBitRate)).c_str());

			(void)xml.AddElem("EventFrameRate");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiEventFrameRate)).c_str());

			(void)xml.AddElem("Profile");
			(void)xml.IntoElem();
			if (1 == pCameraStrCfgEx->stStreamInfo[i].uiEncodeType)
			{
				(void)xml.SetElemValue(CDevEnum::GetH264ProfileCH(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiProfile)));
			}
			if (2 == pCameraStrCfgEx->stStreamInfo[i].uiEncodeType)
			{
				(void)xml.SetElemValue(CDevEnum::GetMjpegProfileCH(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiProfile)));
			}

			(void)xml.AddElem("IframeInterval");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiFrameInterval)).c_str());

			(void)xml.AddElem("TransProtocol");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiTransProtocol)).c_str());

			(void)xml.AddElem("PKTProtocol");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraStrCfgEx->stStreamInfo[i].uiPktProtocol)).c_str());
			xml.OutOfElem();
		}
		xml.OutOfElem();
		xml.OutOfElem();
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}


// ��ѯOSD��������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetOSDParaParseXML(IVS_CAMERA_OSD_CFG* pOSDConfig, CXml& xml)
{
	CHECK_POINTER(pOSDConfig, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/OSDParaInfo"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "xml.FindElemEx(Content/OSDParaInfo) fail", "NA");
		return IVS_SMU_XML_INVALID;
	}

	const IVS_CHAR* pTemp = NULL;
	IVS_CHAR szValue[MAXLEN] = {0};
	if (xml.FindElem("OSDTimeInfo"))
	{
		(void)xml.IntoElem();

		// �Ƿ���ʾʱ�䣬ON/OFF
		GET_ELEM_VALUE_CHAR("OSDTimeEnable",pTemp,szValue,MAXLEN,xml);
		pOSDConfig->stOSDTime.bEnableOSDTime = static_cast<IVS_INT32>(CXmlProcess::TransOnOffToInt(szValue));

		// ʱ���ʽ��1��2009-09-09 09:09:09��2��2009��09��09�� 09 :09 :09��3��UTCʱ��
		GET_ELEM_VALUE_NUM_FOR_UINT("TimeFormatType", pTemp, pOSDConfig->stOSDTime.uiTimeFormat, xml);

		// ʱ��X���꣬��󳤶�4
		GET_ELEM_VALUE_NUM_FOR_FLOAT("TimeX", pTemp, pOSDConfig->stOSDTime.fTimeX, xml);

		// ʱ��Y���꣬��󳤶�4
		GET_ELEM_VALUE_NUM_FOR_FLOAT("TimeY", pTemp, pOSDConfig->stOSDTime.fTimeY, xml);
		xml.OutOfElem();
	}

	if (xml.FindElem("OSDNameInfo"))
	{
		(void)xml.IntoElem();

		// �Ƿ���ʾ���� ON/OFF
		memset(szValue, 0, MAXLEN);
		GET_ELEM_VALUE_CHAR("OSDNameEnable",pTemp,szValue,MAXLEN,xml);
		pOSDConfig->stOSDName.bEnableOSDName = static_cast<IVS_INT32>(CXmlProcess::TransOnOffToInt(szValue));

		// �������ݣ����̿ɼ��ַ�������
		GET_ELEM_VALUE_CHAR("OSDNameText", pTemp, pOSDConfig->stOSDName.cOSDNameText, IVS_MAX_OSD_TEXT_LEN, xml);

		// ����X���꣬��󳤶�4
		GET_ELEM_VALUE_NUM_FOR_FLOAT("TextX",pTemp,pOSDConfig->stOSDName.rectText.left,xml);

		// ����Y���꣬��󳤶�4
		GET_ELEM_VALUE_NUM_FOR_FLOAT("TextY",pTemp,pOSDConfig->stOSDName.rectText.top,xml);

		// �Ƿ�����ʾ��1���棻0������
		GET_ELEM_VALUE_NUM("SwitchEnable", pTemp, pOSDConfig->stOSDName.bEnableSwitch, xml);

		// ������ʾʱ��������󳤶�4
		GET_ELEM_VALUE_NUM_FOR_UINT("SwitchInterval", pTemp, pOSDConfig->stOSDName.uiSwitchInterval, xml);

		// �Ƿ�������˸��1��˸��0����˸
		GET_ELEM_VALUE_NUM("TextBlinkEnable", pTemp, pOSDConfig->stOSDName.bEnableTextBlink, xml);

		// �Ƿ�����͸����1͸����0��͸��
		GET_ELEM_VALUE_NUM("TextTranslucentEnable", pTemp, pOSDConfig->stOSDName.bEnableTextTranslucent, xml);

		xml.OutOfElem();
	}

	return IVS_SUCCEED;
}
// �޸ĸ澯�������豸
IVS_INT32 CDeviceParaConfigXMLProcess::SetAlarmInCfgGetXML(const IVS_ALARM_IN_CFG* pAlarmInCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml)
{
	CHECK_POINTER(pAlarmInCfg, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("AlarmInCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("AlarmInChanInfo");
	{
		(void)xml.IntoElem();
		(void)xml.AddChildElem("AlarmInName");
		(void)xml.IntoElem();
		IVS_CHAR cAlarmInName[IVS_ALARM_IN_NAME_LEN + 1];
		memset(cAlarmInName, 0x0, IVS_ALARM_IN_NAME_LEN + 1);
		if (!CToolsHelp::Memcpy(cAlarmInName, IVS_ALARM_IN_NAME_LEN, pAlarmInCfg->cAlarmInName, IVS_ALARM_IN_NAME_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cAlarmInName, IVS_ALARM_IN_NAME_LEN, pAlarmInCfg->cAlarmInName, IVS_ALARM_IN_NAME_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cAlarmInName);

		(void)xml.AddElem("SwitchType");
		(void)xml.IntoElem();
		const IVS_CHAR* pSwitchType = CDevEnum::GetAlamEnableType(static_cast<IVS_INT32>(pAlarmInCfg->uiSwitchType));
		(void)xml.SetElemValue(pSwitchType);

		(void)xml.AddElem("AutoClearAlarmTime");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAlarmInCfg->uiAutoClearAlarmTime)).c_str());
		xml.OutOfElem();
	}

	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯ�澯�������豸
IVS_INT32 CDeviceParaConfigXMLProcess::GetAlarmInCfgParseXML(IVS_ALARM_IN_CFG* pAlarmInCfg, CXml &xml)
{
	CHECK_POINTER(pAlarmInCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/AlarmInChanInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "GetAlarmInCfg ParseXML fail", "xml.FindElemEx(Content/AlarmInChanInfo)");
		return IVS_XML_INVALID;
	}

	const IVS_INT32 iMaxLen = 10;
	(void)xml.IntoElem();
	IVS_INT32 iTemp = 0;
	const char* pTemp = NULL;
	{
		GET_ELEM_VALUE_CHAR("AlarmInName", pTemp, pAlarmInCfg->cAlarmInName, IVS_ALARM_IN_NAME_LEN, xml);
		GET_ELEM_VALUE_NUM("AlarmInChanIndex", pTemp, iTemp, xml);
		pAlarmInCfg->uiAlarmInChanIndex = static_cast<IVS_UINT32>(iTemp);
		IVS_CHAR cSwitchType[iMaxLen] = {0};
		GET_ELEM_VALUE_CHAR("SwitchType", pTemp, cSwitchType, iMaxLen, xml);
		pAlarmInCfg->uiSwitchType = static_cast<IVS_UINT32>(CDevEnum::GetAlamEnableBool(cSwitchType));
		GET_ELEM_VALUE_NUM("AutoClearAlarmTime", pTemp, iTemp, xml);
		pAlarmInCfg->uiAutoClearAlarmTime = static_cast<IVS_UINT32>(iTemp);
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}


// �޸ĸ澯������豸ƴ��XML
IVS_INT32 CDeviceParaConfigXMLProcess::SetAlarmOutCfgGetXML(const IVS_ALARM_OUT_CFG* pAlarmOutCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml)
{
	CHECK_POINTER(pAlarmOutCfg, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("AlarmOutCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("AlarmOutChanInfo");
	{
		(void)xml.IntoElem();
		(void)xml.AddChildElem("AlarmOutName");
		(void)xml.IntoElem();
		IVS_CHAR cAlarmOutName[IVS_ALARM_OUT_NAME_LEN + 1];
		memset(cAlarmOutName, 0x0, IVS_ALARM_OUT_NAME_LEN + 1);
		if (!CToolsHelp::Memcpy(cAlarmOutName, IVS_ALARM_OUT_NAME_LEN, pAlarmOutCfg->cAlarmOutName, IVS_ALARM_OUT_NAME_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cAlarmOutName, IVS_ALARM_OUT_NAME_LEN, pAlarmOutCfg->cAlarmOutName, IVS_ALARM_OUT_NAME_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cAlarmOutName);

		(void)xml.AddElem("SwitchType");
		(void)xml.IntoElem();
		const IVS_CHAR* pSwitchType = CDevEnum::GetAlamEnableType(static_cast<IVS_INT32>(pAlarmOutCfg->uiSwitchType));
		(void)xml.SetElemValue(pSwitchType);

		(void)xml.AddElem("HoldTime");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pAlarmOutCfg->uiHoldTime)).c_str());
		xml.OutOfElem();
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯ�澯������豸����XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetAlarmOutCfgParseXML(IVS_ALARM_OUT_CFG* pAlarmOutCfg, CXml &xml)
{
	CHECK_POINTER(pAlarmOutCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/AlarmOutChanInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get AlarmOutCfg ParseXML fail", "xml.FindElemEx(Content/AlarmOutChanInfo)");
		return IVS_XML_INVALID;
	}

	const IVS_INT32 iMaxLen = 10;
	(void)xml.IntoElem();
	const char* pTemp = NULL;
	IVS_INT32 iTemp = 0;
	GET_ELEM_VALUE_CHAR("AlarmOutName", pTemp, pAlarmOutCfg->cAlarmOutName, IVS_ALARM_OUT_NAME_LEN, xml);
	GET_ELEM_VALUE_NUM("AlarmOutChanIndex", pTemp, iTemp, xml);
	pAlarmOutCfg->uiAlarmOutChanIndex = static_cast<IVS_UINT32>(iTemp);
	IVS_CHAR cSwitchType[iMaxLen] = {0};
	GET_ELEM_VALUE_CHAR("SwitchType", pTemp, cSwitchType, iMaxLen, xml);
	pAlarmOutCfg->uiSwitchType = static_cast<IVS_UINT32>(CDevEnum::GetAlamEnableBool(cSwitchType));
	GET_ELEM_VALUE_NUM("HoldTime", pTemp, iTemp, xml);
	pAlarmOutCfg->uiHoldTime = static_cast<IVS_UINT32>(iTemp);

	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ������˽������xml
IVS_INT32 CDeviceParaConfigXMLProcess::GetVideoMaskConfigParseXml(IVS_VIDEO_MASK* pVideoMask, CXml& xml)
{
	CHECK_POINTER(pVideoMask,IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/VideoMaskInfo"))
	{
		return IVS_XML_INVALID;
	}
	const IVS_INT32 maxLen = 120;
	IVS_CHAR chTemp[maxLen] = {0};
	const char* szElemValue = NULL;
	IVS_UINT32 index = 0;

	GET_ELEM_VALUE_CHAR("VideoMaskEnable", szElemValue, chTemp, maxLen, xml);
	pVideoMask->bEnableVideoMask = CDevEnum::GetAlamEnableBool(chTemp);

	if (!xml.FindElem("AreaInfoList"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "CameraStreamCfg ParseXML fail", "xml.FindElem(StreamList) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();

	if (!xml.FindElem("AreaInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "CameraStreamCfg ParseXML fail", "xml.FindElem(StreamList) fail");
		pVideoMask->uiAreaNum = 0;
	} 
	else
	{
		do 
		{
			(void)xml.IntoElem();

			GET_ELEM_VALUE_NUM_FOR_FLOAT("X1", szElemValue, pVideoMask->rectArea[index].left, xml);

			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y1", szElemValue, pVideoMask->rectArea[index].top, xml);

			GET_ELEM_VALUE_NUM_FOR_FLOAT("X2", szElemValue, pVideoMask->rectArea[index].right, xml);

			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y2", szElemValue, pVideoMask->rectArea[index].bottom, xml);

			xml.OutOfElem();
			index++;

		} while (xml.NextElem() && index < IVS_MAX_VIDEO_MASK_AREA_NUM);

		pVideoMask->uiAreaNum = index;
	}
	
	xml.GetRootPos();

	return IVS_SUCCEED;
}


//ƴװ��˽����XML
IVS_INT32 CDeviceParaConfigXMLProcess::SetVideoMaskCfgGetXML(const IVS_VIDEO_MASK* pVideoMask, const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, CXml &xml)
{
	CHECK_POINTER(pVideoMask, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pCameraCode);

	(void)xml.AddElem("VideoMaskInfo");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("VideoMaskEnable");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommStatusType(pVideoMask->bEnableVideoMask));

	(void)xml.AddElem("AreaInfoList");
	(void)xml.IntoElem();
	IVS_UINT32 iCount = pVideoMask->uiAreaNum;
	for (IVS_UINT32 index = 0; index < iCount; index ++)
	{
		if (0 == index)
		{
			(void)xml.AddChildElem("AreaInfo");
		}
		else
		{
			(void)xml.AddElem("AreaInfo");
		}

		(void)xml.IntoElem();
		(void)xml.AddChildElem("X1");
		(void)xml.IntoElem();
		IVS_CHAR cLeft[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cLeft, STR_LEN, "%f", pVideoMask->rectArea[index].left))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLeft, STR_LEN, '%f', pVideoMask->rectArea[index].left) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cLeft);

		(void)xml.AddElem("Y1");
		IVS_CHAR cTop[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cTop, STR_LEN, "%f", pVideoMask->rectArea[index].top))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTop, STR_LEN, '%f', pVideoMask->rectArea[index].top) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cTop);

		(void)xml.AddElem("X2");
		IVS_CHAR cRight[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cRight, STR_LEN, "%f", pVideoMask->rectArea[index].right))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cRight, STR_LEN, '%f', pVideoMask->rectArea[index].right) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cRight);

		(void)xml.AddElem("Y2");
		IVS_CHAR cBottom[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cBottom, STR_LEN, "%f", pVideoMask->rectArea[index].bottom))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cBottom, STR_LEN, '%f', pVideoMask->rectArea[index].bottom) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cBottom);
		
		xml.OutOfElem();
	}

	return IVS_SUCCEED;
}


// �޸�OSD����
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraOsgCfgGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_CAMERA_OSD_CFG* pCameraOsgCfg, CXml &xml)
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraOsgCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("OSDParaInfo");
	(void)xml.IntoElem();
	{
		(void)xml.AddChildElem("OSDTimeInfo");
		{
			(void)xml.IntoElem();
			(void)xml.AddChildElem("OSDTimeEnable");
			(void)xml.IntoElem();
			const IVS_CHAR* pOSDTimeEnable = CDevEnum::GetAlamEnableType(pCameraOsgCfg->stOSDTime.bEnableOSDTime);
			(void)xml.SetElemValue(pOSDTimeEnable);

			(void)xml.AddElem("TimeFormatType");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraOsgCfg->stOSDTime.uiTimeFormat)).c_str());

			(void)xml.AddElem("TimeX");
			(void)xml.IntoElem();
			IVS_CHAR cTimeX[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cTimeX, STR_LEN, "%f", pCameraOsgCfg->stOSDTime.fTimeX))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTimeX, STR_LEN, '%f', pCameraOsgCfg->stOSDTime.fTimeX) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cTimeX);

			(void)xml.AddElem("TimeY");
			(void)xml.IntoElem();
			IVS_CHAR cTimeY[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cTimeY, STR_LEN, "%f", pCameraOsgCfg->stOSDTime.fTimeY))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTimeY, STR_LEN, '%f', pCameraOsgCfg->stOSDTime.fTimeY) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cTimeY);
			xml.OutOfElem();
		}
		(void)xml.AddElem("OSDNameInfo");
		{
			(void)xml.IntoElem();
			(void)xml.AddChildElem("OSDNameEnable");
			(void)xml.IntoElem();
			const IVS_CHAR* pOSDNameEnable = CDevEnum::GetAlamEnableType(pCameraOsgCfg->stOSDName.bEnableOSDName);
			(void)xml.SetElemValue(pOSDNameEnable);

			(void)xml.AddElem("OSDNameText");
			(void)xml.IntoElem();
			IVS_CHAR cOsgNameText[IVS_MAX_OSD_TEXT_LEN + 1];
			memset(cOsgNameText, 0x0, IVS_MAX_OSD_TEXT_LEN + 1);
			if (!CToolsHelp::Memcpy(cOsgNameText, IVS_MAX_OSD_TEXT_LEN, pCameraOsgCfg->stOSDName.cOSDNameText, IVS_MAX_OSD_TEXT_LEN))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cOsgNameText, IVS_MAX_OSD_TEXT_LEN, pCameraOsgCfg->stOSDName.cOSDNameText, IVS_MAX_OSD_TEXT_LEN) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cOsgNameText);

			(void)xml.AddElem("TextX");
			(void)xml.IntoElem();
			IVS_CHAR cLeft[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cLeft, STR_LEN, "%f", pCameraOsgCfg->stOSDName.rectText.left))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLeft, STR_LEN, '%f', pCameraOsgCfg->stOSDName.rectText.left) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cLeft);

			(void)xml.AddElem("TextY");
			(void)xml.IntoElem();
			IVS_CHAR cRight[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cRight, STR_LEN, "%f", pCameraOsgCfg->stOSDName.rectText.right))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cRight, STR_LEN, '%f', pCameraOsgCfg->stOSDName.rectText.right) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cRight);

			(void)xml.AddElem("SwitchEnable");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraOsgCfg->stOSDName.bEnableSwitch).c_str());

			(void)xml.AddElem("SwitchInterval");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraOsgCfg->stOSDName.uiSwitchInterval)).c_str());

			(void)xml.AddElem("TextBlinkEnable");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraOsgCfg->stOSDName.bEnableTextBlink).c_str());

			(void)xml.AddElem("TextTranslucentEnable");
			(void)xml.IntoElem();
			(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraOsgCfg->stOSDName.bEnableTextTranslucent).c_str());

			xml.OutOfElem();
		}	
	}
	xml.OutOfElem();
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// �޸�ͼ���ڵ�
IVS_INT32 CDeviceParaConfigXMLProcess::SetVideoHideAlarmGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm, CXml &xml)
{
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pVideoHideAlarm, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("VideoHideInfo");
	
	(void)xml.IntoElem();
	(void)xml.AddChildElem("Sensitivity");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pVideoHideAlarm->uiSensitivity)).c_str());

	(void)xml.AddElem("AutoClearAlarmTime");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pVideoHideAlarm->uiAutoClearAlarmTime)).c_str());

	(void)xml.AddElem("VideoHideEnable");
	(void)xml.IntoElem();
	const IVS_CHAR* pVideoHideEnable = CDevEnum::GetAlamEnableType(pVideoHideAlarm->bEnableVideoHide);
	(void)xml.SetElemValue(pVideoHideEnable);

	(void)xml.AddElem("GuardPlan");
	(void)xml.IntoElem();
	for (IVS_UINT32 i = 0;i < pVideoHideAlarm->uiGuardTimeNum; i++)
	{
		i == 0 ? (void)xml.AddChildElem("GuardTime"):(void)xml.AddElem("GuardTime");
		(void)xml.IntoElem();
		{
			(void)xml.AddChildElem("StartTime");
			(void)xml.IntoElem();
			IVS_CHAR cStartTime[IVS_TIME_LEN + 1];
			memset(cStartTime, 0x0, IVS_TIME_LEN + 1);
			if (!CToolsHelp::Memcpy(cStartTime, IVS_TIME_LEN, pVideoHideAlarm->stGuardTime[i].cStart, IVS_TIME_LEN))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cStartTime, IVS_TIME_LEN, pVideoHideAlarm->stGuardTime[i].cStart, IVS_TIME_LEN) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cStartTime);

			(void)xml.AddElem("EndTime");
			(void)xml.IntoElem();
			IVS_CHAR cEndTime[IVS_TIME_LEN + 1];
			memset(cEndTime, 0x0, IVS_TIME_LEN + 1);
			if (!CToolsHelp::Memcpy(cEndTime, IVS_TIME_LEN, pVideoHideAlarm->stGuardTime[i].cEnd, IVS_TIME_LEN))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cEndTime, IVS_TIME_LEN, pVideoHideAlarm->stGuardTime[i].cEnd, IVS_TIME_LEN) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cEndTime);
		}
		xml.OutOfElem();
	}
	if (0 != pVideoHideAlarm->uiGuardTimeNum)
	{
		xml.OutOfElem();
	}

	(void)xml.AddElem("AreaInfoList");
	(void)xml.IntoElem();
	
	for (IVS_UINT32 j = 0;j < pVideoHideAlarm->uiAreaNum;j++)
	{
		j == 0 ? (void)xml.AddChildElem("AreaInfo"):(void)xml.AddElem("AreaInfo");
		(void)xml.IntoElem();
		{
			(void)xml.AddChildElem("X1");
			(void)xml.IntoElem();
			IVS_CHAR cLeft[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cLeft, STR_LEN, "%f", pVideoHideAlarm->rectArea[j].left))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLeft, STR_LEN, '%f', pVideoHideAlarm->rectArea[j].left) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cLeft);

			(void)xml.AddElem("Y1");
			(void)xml.IntoElem();
			IVS_CHAR cTop[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cTop, STR_LEN, "%f", pVideoHideAlarm->rectArea[j].top))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTop, STR_LEN, '%f', pVideoHideAlarm->rectArea[j].top) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cTop);

			(void)xml.AddElem("X2");
			(void)xml.IntoElem();
			IVS_CHAR cRight[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cRight, STR_LEN, "%f", pVideoHideAlarm->rectArea[j].right))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cRight, STR_LEN, '%f', pVideoHideAlarm->rectArea[j].right) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cRight);

			(void)xml.AddElem("Y2");
			(void)xml.IntoElem();
			IVS_CHAR cBottom[STR_LEN] = {0};
			if (!CToolsHelp::Snprintf(cBottom, STR_LEN, "%f", pVideoHideAlarm->rectArea[j].bottom))
			{
				BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cBottom, STR_LEN, '%f', pVideoHideAlarm->rectArea[j].bottom) fail", "NA");
				return IVS_ALLOC_MEMORY_ERROR;
			}
			(void)xml.SetElemValue(cBottom);
		}
		xml.OutOfElem();
	}
	
	if (0 != pVideoHideAlarm->uiAreaNum)
	{
		xml.OutOfElem();
	}

	xml.OutOfElem();
	
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯͼ���ڵ�����XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetVideoHideAlarmParseXML(IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm, CXml &xml)
{
	CHECK_POINTER(pVideoHideAlarm, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/VideoHideInfo"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_DEV_REQ_XML_INVALID, "Get VideoHideAlarm ParseXML fail", "xml.FindElemEx(Content/VideoHideInfo) fail");
		return IVS_SMU_DEV_REQ_XML_INVALID;
	}
	const IVS_INT32 uiMaxLen = 10;
	(void)xml.IntoElem();
	const char* pTemp = NULL;
	IVS_INT32 iTemp = 0;
	GET_ELEM_VALUE_NUM("Sensitivity", pTemp, iTemp, xml);
	pVideoHideAlarm->uiSensitivity = static_cast<IVS_UINT32>(iTemp);
	GET_ELEM_VALUE_NUM("AutoClearAlarmTime", pTemp, iTemp, xml);
	pVideoHideAlarm->uiAutoClearAlarmTime = static_cast<IVS_UINT32>(iTemp);

	IVS_CHAR cEnableVideoHide[uiMaxLen] = {0};
	GET_ELEM_VALUE_CHAR("VideoHideEnable", pTemp, cEnableVideoHide, uiMaxLen, xml);
	pVideoHideAlarm->bEnableVideoHide = CDevEnum::GetAlamEnableBool(cEnableVideoHide);

	if (!xml.FindElem("GuardPlan"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_DEV_REQ_XML_INVALID, "Get VideoHideAlarm ParseXML fail", "xml.FindElem(GuardPlan) fail");
		return IVS_SMU_DEV_REQ_XML_INVALID;
	}
	(void)xml.IntoElem();
	if (!xml.FindElem("GuardTime"))
	{
		BP_RUN_LOG_INF("Get VideoHideAlarm ParseXML fail", "xml.FindElem(GuardTime) fail");
		pVideoHideAlarm->uiGuardTimeNum = 0;
	}
	else
	{	
		IVS_UINT32 i = 0;
		do 
		{
			(void)xml.IntoElem();
			GET_ELEM_VALUE_CHAR("StartTime", pTemp, pVideoHideAlarm->stGuardTime[i].cStart, IVS_TIME_LEN, xml);
			GET_ELEM_VALUE_CHAR("EndTime", pTemp, pVideoHideAlarm->stGuardTime[i].cEnd, IVS_TIME_LEN, xml);
			xml.OutOfElem();
			i++;
		} while (xml.NextElem() && i < IVS_MAX_VIDEO_HIDE_GUARD_TIME_NUM);
		
		pVideoHideAlarm->uiGuardTimeNum = i;
		xml.OutOfElem();
	}

	if (!xml.FindElem("AreaInfoList"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_DEV_REQ_XML_INVALID, "Get VideoHideAlarm ParseXML fail", "xml.FindElem(AreaInfoList)");
		return IVS_SMU_DEV_REQ_XML_INVALID;
	}
	(void)xml.IntoElem();
	if (!xml.FindElem("AreaInfo"))
	{
		BP_RUN_LOG_INF("Get VideoHideAlarm ParseXML fail", "xml.FindElem(AreaInfo)");
		xml.OutOfElem();
		pVideoHideAlarm->uiAreaNum = 0;
	}
	else
	{
		IVS_UINT32 j = 0;
		do 
		{
			(void)xml.IntoElem();
			GET_ELEM_VALUE_NUM_FOR_FLOAT("X1", pTemp, pVideoHideAlarm->rectArea[j].left, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y1", pTemp, pVideoHideAlarm->rectArea[j].top, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("X2", pTemp, pVideoHideAlarm->rectArea[j].right, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y2", pTemp, pVideoHideAlarm->rectArea[j].bottom, xml);
			xml.OutOfElem();
			j++;
		} while (xml.NextElem() && j < IVS_MAX_VIDEO_HIDE_AREA_NUM);
		xml.OutOfElem();
		pVideoHideAlarm->uiAreaNum = j;
	}
	
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ������ѯ�˶������Ӧ��Ϣxml
IVS_INT32 CDeviceParaConfigXMLProcess::ParseMotionDetecXML(CXml& xml, IVS_MOTION_DETECTION* pMotionDetection)
{
	CHECK_POINTER(pMotionDetection, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/MotionDetectionInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Parse MotionDetecXML", "xml.FindElemEx(Content/MotionDetectionInfo)");
		return IVS_XML_INVALID;
	}

	const IVS_CHAR* szElemValue = NULL;
	IVS_CHAR szTemp[EABLE_LENGTH] = {0};
	GET_ELEM_VALUE_NUM_FOR_UINT("DetectInterval", szElemValue, pMotionDetection->uiDetectInterval, xml);
	GET_ELEM_VALUE_NUM_FOR_UINT("Sensitivity", szElemValue, pMotionDetection->uiSensitivity, xml);
	GET_ELEM_VALUE_NUM_FOR_UINT("AutoClearAlarmTime", szElemValue, pMotionDetection->uiAutoClearAlarmTime, xml);
	GET_ELEM_VALUE_CHAR("MotionDetectionEnable", szElemValue, szTemp, EABLE_LENGTH, xml);
	if (0 == strcmp("ON", szTemp))
	{
		pMotionDetection->bMotionDetectionEnable = 1;
	}
	if (0 == strcmp("OFF", szTemp))
	{
		pMotionDetection->bMotionDetectionEnable = 0;
	}
	memset(szTemp, 0x0, EABLE_LENGTH);
	GET_ELEM_VALUE_CHAR("PlatformMotionDetection", szElemValue, szTemp, EABLE_LENGTH, xml);
	if (0 == strcmp("ON", szTemp))
	{
		pMotionDetection->bPlatformMotionDetection = 1;
	}
	if (0 == strcmp("OFF", szTemp))
	{
		pMotionDetection->bPlatformMotionDetection = 0;
	}

	if (!xml.FindElem("AreaInfoList"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Parse MotionDetecXML fail", "xml.FindElem(AreaInfoList)");
		return IVS_XML_INVALID;
	}
	xml.IntoElem();
	IVS_INT32 iNum = 0;
	(void)CDeviceParaConfigXMLProcess::GetMotionDetecAreaInfoNum(xml, iNum);
	if (0 == iNum)
	{
		return IVS_SUCCEED;
	}
	if (iNum > IVS_MAX_MOTION_DETECTION_AREA_NUM)
	{
 		BP_RUN_LOG_INF("Parse MotionDetecXML fail", "iNum < IVS_MAX_MOTION_DETECTION_AREA_NUM");
		pMotionDetection->uiRectNum = IVS_MAX_MOTION_DETECTION_AREA_NUM;
		iNum = IVS_MAX_MOTION_DETECTION_AREA_NUM;
	}
	else
	{
		pMotionDetection->uiRectNum = static_cast<IVS_UINT32>(iNum);
	}
	
	if (xml.FindElem("AreaInfo"))
	{
		IVS_INT32 i = 0;
		//IVS_INT32 iTemp = 0;
		while(i < iNum)
		{
			if (!xml.FindElemValue("AreaInfo"))
			{
				break;
			}
			IVS_RECT_FLOAT& stRect = pMotionDetection->rectDetection[i];
			xml.IntoElem();
			GET_ELEM_VALUE_NUM_FOR_FLOAT("X1", szElemValue, stRect.left, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y1", szElemValue, stRect.top, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("X2", szElemValue, stRect.right, xml);
			GET_ELEM_VALUE_NUM_FOR_FLOAT("Y2", szElemValue, stRect.bottom, xml);
			xml.OutOfElem();
			(void)xml.NextElem();
			i++;
		}
	}

	return IVS_SUCCEED;

}

// ������Ӧ��Ϣ��AreaInfo������
IVS_INT32 CDeviceParaConfigXMLProcess::GetMotionDetecAreaInfoNum(CXml& xml, IVS_INT32& iNum)
{
	IVS_DEBUG_TRACE("");
	if (!xml.FindElemEx("Content/MotionDetectionInfo/AreaInfoList"))
	{
		BP_RUN_LOG_ERR(IVS_SMU_XML_INVALID, "Get MotionDetec AreaInfoNum fail", "xml.FindElemEx(Content/MotionDetectionInfo/AreaInfoList)");
		return IVS_SMU_XML_INVALID;
	}
	// AreaInfo ��O�ֶΣ�����û�У����û��ֱ�ӷ���0��
	if (!xml.FindElem("AreaInfo"))
	{
		return iNum;
	}

	do 
	{
		if (!xml.FindElemValue("AreaInfo"))
		{
			break;
		}
		iNum++;
	} while (xml.NextElem());

	return iNum;
}

// ƴװ�޸��˶��������xml
IVS_INT32 CDeviceParaConfigXMLProcess::SetMotionDetecGetXML(const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode, const IVS_MOTION_DETECTION* pMotionDetection, CXml& xml)
{
	CHECK_POINTER(pCameraCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pMotionDetection, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");
  
	// ��ʼƴװxml
	(void)xml.AddDeclaration("1.0", "UTF-8", "");
	(void)xml.AddElem("Content");

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pCameraCode);

	(void)xml.AddElem("MotionDetectionInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("DetectInterval");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pMotionDetection->uiDetectInterval)).c_str());

	(void)xml.AddElem("Sensitivity");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pMotionDetection->uiSensitivity)).c_str());

	(void)xml.AddElem("AutoClearAlarmTime");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pMotionDetection->uiAutoClearAlarmTime)).c_str());

	(void)xml.AddElem("MotionDetectionEnable");
	std::string strEnableMotionDetec = CXmlProcess::TransIntToOnOff(static_cast<IVS_UINT32>(pMotionDetection->bMotionDetectionEnable));
	(void)xml.SetElemValue(strEnableMotionDetec.c_str());

	(void)xml.AddElem("PlatformMotionDetection");
	std::string strPlatformMotionDetection = CXmlProcess::TransIntToOnOff(static_cast<IVS_UINT32>(pMotionDetection->bPlatformMotionDetection));
	(void)xml.SetElemValue(strPlatformMotionDetection.c_str());

	(void)xml.AddElem("AreaInfoList");
	(void)xml.IntoElem();
	IVS_UINT32 i = 0;
	while(i < pMotionDetection->uiRectNum)
	{
		const IVS_RECT_FLOAT& stRect = pMotionDetection->rectDetection[i];

		i == 0?(void)xml.AddChildElem("AreaInfo"):(void)xml.AddElem("AreaInfo");
		(void)xml.IntoElem();
		(void)xml.AddChildElem("X1");
		(void)xml.IntoElem();
		IVS_CHAR cLeft[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cLeft, STR_LEN, "%f", stRect.left))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLeft, STR_LEN, %f, stRect.left) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cLeft);

		(void)xml.AddElem("Y1");
		IVS_CHAR cTop[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cTop, STR_LEN, "%f", stRect.top))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTop, STR_LEN, %f, stRect.top) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cTop);

		(void)xml.AddElem("X2");
		IVS_CHAR cRight[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cRight, STR_LEN, "%f", stRect.right))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cRight, STR_LEN, %f, stRect.right) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cRight);

		(void)xml.AddElem("Y2");
		IVS_CHAR cBottom[STR_LEN] = {0};
		if (!CToolsHelp::Snprintf(cBottom, STR_LEN, "%f", stRect.bottom))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cBottom, STR_LEN, %f, stRect.bottom) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cBottom);
		xml.OutOfElem();

		i++;
	}
	xml.OutOfElem();
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// �޸����豸�������ƴ��XML
IVS_INT32 CDeviceParaConfigXMLProcess::SetDevNetCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_DEVICE_NET_CFG* pDevNetCfg, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevNetCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("DevCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("NetInfo");
	(void)xml.IntoElem();
	{
		(void)xml.AddChildElem("DevIPType");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pDevNetCfg->stPUIp.uiIPType)).c_str());

		(void)xml.AddElem("DevIP");
		(void)xml.IntoElem();
		IVS_CHAR cDevIp[IVS_IP_LEN + 1];
		memset(cDevIp, 0x0, IVS_IP_LEN + 1);
		if (!CToolsHelp::Memcpy(cDevIp, IVS_IP_LEN, pDevNetCfg->stPUIp.cIP, IVS_IP_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevIp, IVS_IP_LEN, pDevNetCfg->stPUIp.cIP, IVS_IP_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cDevIp);

		(void)xml.AddElem("DevPort");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pDevNetCfg->uiPUPort)).c_str());

		(void)xml.AddElem("DevGW");
		(void)xml.IntoElem();
		IVS_CHAR cDevGw[IVS_IP_LEN + 1];
		memset(cDevGw, 0x0, IVS_IP_LEN + 1);
		if (!CToolsHelp::Memcpy(cDevGw, IVS_IP_LEN, pDevNetCfg->stPUGateway.cIP, IVS_IP_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cDevGw, IVS_IP_LEN, pDevNetCfg->stPUGateway.cIP, IVS_IP_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cDevGw);

		(void)xml.AddElem("MaxMTU");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pDevNetCfg->uiMaxMtu)).c_str());

		(void)xml.AddElem("SubNetMask");
		(void)xml.IntoElem();
		IVS_CHAR cSubNetMask[IVS_IP_LEN + 1];
		memset(cSubNetMask, 0x0, IVS_IP_LEN + 1);
		if (!CToolsHelp::Memcpy(cSubNetMask, IVS_IP_LEN, pDevNetCfg->stSubNetMask.cIP, IVS_IP_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cSubNetMask, IVS_IP_LEN, pDevNetCfg->cSubNetMask.cIP, IVS_IP_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cSubNetMask);
	}
	xml.OutOfElem();
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯ���豸�����������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetDevNetCfgParseXML(IVS_DEVICE_NET_CFG* pDevNetCfg, CXml &xml)
{
	CHECK_POINTER(pDevNetCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/NetInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevNetCfg ParseXML fail", "xml.FindElemEx(Content/NetInfo)");
		return IVS_XML_INVALID;
	}
	const char* pTemp = NULL;
	(void)xml.IntoElem();
	{
		GET_ELEM_VALUE_NUM_FOR_UINT("DevIPType", pTemp, pDevNetCfg->stPUIp.uiIPType, xml);
		GET_ELEM_VALUE_CHAR("DevIP", pTemp, pDevNetCfg->stPUIp.cIP, IVS_IP_LEN, xml);
		GET_ELEM_VALUE_NUM_FOR_UINT("DevPort", pTemp, pDevNetCfg->uiPUPort, xml);
		GET_ELEM_VALUE_CHAR("DevGW", pTemp, pDevNetCfg->stPUGateway.cIP, IVS_IP_LEN, xml);
		GET_ELEM_VALUE_NUM_FOR_UINT("MaxMTU", pTemp, pDevNetCfg->uiMaxMtu, xml);
		GET_ELEM_VALUE_CHAR("SubNetMask", pTemp, pDevNetCfg->stSubNetMask.cIP, IVS_IP_LEN, xml);
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// �޸����豸ʱ�����ƴװXML
IVS_INT32 CDeviceParaConfigXMLProcess::SetDevTimeCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_DEVICE_TIME_CFG* pDevTimeCfg, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDevTimeCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("DevCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("TimeInfo");
	(void)xml.IntoElem();
	{
		(void)xml.AddChildElem("TimeType");
		(void)xml.IntoElem();
		if (0 == pDevTimeCfg->bEnableNtp)
		{
			(void)xml.SetElemValue("MANUAL");
		}
		else
		{
			(void)xml.SetElemValue("NTP");
		}

		(void)xml.AddElem("NTPType");
		(void)xml.IntoElem();
		if (0 == pDevTimeCfg->stNTP.uiNtpType)
		{
			(void)xml.SetElemValue("DHCP");
		}
		else
		{
			(void)xml.SetElemValue("MANUAL");
		}

		(void)xml.AddElem("NTPIPType");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pDevTimeCfg->stNTP.stNtpServerIP.uiIPType)).c_str());

		(void)xml.AddElem("NTPServerIP");
		(void)xml.IntoElem();
		IVS_CHAR cNtpServerIp[IVS_IP_LEN + 1];
		memset(cNtpServerIp, 0x0, IVS_IP_LEN + 1);
		if (!CToolsHelp::Memcpy(cNtpServerIp, IVS_IP_LEN, pDevTimeCfg->stNTP.stNtpServerIP.cIP, IVS_IP_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cNtpServerIp, IVS_IP_LEN, pDevTimeCfg->stNTP.stNtpServerIP.cIP, IVS_IP_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cNtpServerIp);

		(void)xml.AddElem("NTPDns");
		(void)xml.IntoElem();
		IVS_CHAR cNtpDns[IVS_IP_LEN + 1];
		memset(cNtpDns, 0x0, IVS_IP_LEN + 1);
		if (!CToolsHelp::Memcpy(cNtpDns, IVS_IP_LEN, pDevTimeCfg->stNTP.stDnsIP.cIP, IVS_IP_LEN))
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cNtpDns, IVS_IP_LEN, pDevTimeCfg->stNTP.stDnsIP.cIP, IVS_IP_LEN) fail", "NA");
			return IVS_ALLOC_MEMORY_ERROR;
		}
		(void)xml.SetElemValue(cNtpDns);

		(void)xml.AddElem("DevTimeZone");
		(void)xml.IntoElem();
		(void)xml.SetElemValue(CToolsHelp::Int2Str(pDevTimeCfg->iTimezone).c_str());

		(void)xml.AddElem("DevDSTType");
		(void)xml.IntoElem();
		if (0 == pDevTimeCfg->bEnableDst)
		{
			(void)xml.SetElemValue("OFF");
		}
		else
		{
			(void)xml.SetElemValue("ON");
		}
	}
	xml.OutOfElem();
	xml.OutOfElem();

	return IVS_SUCCEED;
}

// ��ѯ���豸ʱ���������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetDevTimeCfgParseXML(IVS_DEVICE_TIME_CFG* pDevTimeCfg, CXml &xml)
{
	CHECK_POINTER(pDevTimeCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElem("Content"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevTimeCfg ParseXML fail", "xml.FindElem(Content) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	if (!xml.FindElem("TimeInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevTimeCfg ParseXML fail", "xml.FindElem(TimeInfo) fail");
		return IVS_XML_INVALID;
	}
	const char* pTemp = NULL;
	(void)xml.IntoElem();
	{
		IVS_CHAR cTimeType[DEV_TIME_CFG_LEN];
		memset(cTimeType, 0x0, DEV_TIME_CFG_LEN);
		GET_ELEM_VALUE_CHAR("TimeType", pTemp, cTimeType, DEV_TIME_CFG_LEN, xml);
		if (0 == strcmp(cTimeType, "NTP"))
		{
			pDevTimeCfg->bEnableNtp = 1;
		}
		else if (0 == strcmp(cTimeType, "MANUAL"))
		{
			pDevTimeCfg->bEnableNtp = 0;
		}
		else
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevTimeCfg ParseXML fail", "TimeType fail");
			return IVS_XML_INVALID;
		}

		IVS_CHAR cNtpType[DEV_TIME_CFG_LEN];
		memset(cNtpType, 0x0, DEV_TIME_CFG_LEN);
		GET_ELEM_VALUE_CHAR("NTPType", pTemp, cNtpType, DEV_TIME_CFG_LEN, xml);
		if (0 == strcmp(cNtpType, "DHCP"))
		{
			pDevTimeCfg->stNTP.uiNtpType = 0;
		}
		else if (0 == strcmp(cNtpType, "MANUAL"))
		{
			pDevTimeCfg->stNTP.uiNtpType = 1;
		}
		else
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevTimeCfg ParseXML fail", "NTPType fail");
			return IVS_XML_INVALID;
		}

		GET_ELEM_VALUE_NUM_FOR_UINT("NTPIPType", pTemp, pDevTimeCfg->stNTP.stNtpServerIP.uiIPType, xml);
		GET_ELEM_VALUE_CHAR("NTPServerIP", pTemp, pDevTimeCfg->stNTP.stNtpServerIP.cIP, IVS_IP_LEN, xml);
		GET_ELEM_VALUE_CHAR("NTPDns", pTemp, pDevTimeCfg->stNTP.stDnsIP.cIP, IVS_IP_LEN, xml);
		GET_ELEM_VALUE_NUM("DevTimeZone", pTemp, pDevTimeCfg->iTimezone, xml);
		/*GET_ELEM_VALUE_CHAR("DevTime", pTemp, pDevTimeCfg->cDevTime, IVS_TIME_LEN, xml);*/

		IVS_CHAR cDevDtsType[DEV_TIME_CFG_LEN];
		memset(cDevDtsType, 0x0, DEV_TIME_CFG_LEN);
		GET_ELEM_VALUE_CHAR("DevDSTType", pTemp, cDevDtsType, DEV_TIME_CFG_LEN, xml);
		if (0 == strcmp(cDevDtsType, "ON"))
		{
			pDevTimeCfg->bEnableDst = 1;
		}
		else if (0 == strcmp(cDevDtsType, "OFF"))
		{
			pDevTimeCfg->bEnableDst = 0;
		}
		else
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get DevTimeCfg ParseXML fail", "DevDSTType fail");
			return IVS_XML_INVALID;
		}
	}
	xml.OutOfElem();

	return IVS_SUCCEED;
}


// �޸Ĵ������豸��ϸƴװXML
IVS_INT32 CDeviceParaConfigXMLProcess::SetSerialChannelCfgGetXML(const IVS_CHAR* pSerialCode, const IVS_CHAR* pDomainCode, const IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg, CXml &xml)
{
	CHECK_POINTER(pSerialCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pSerialChannelCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("SerialCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pSerialCode);

	(void)xml.AddElem("SerialChanInfo");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("SerialName");
	(void)xml.IntoElem();
	IVS_CHAR cSerialName[IVS_SERIAL_NAME_LEN + 1] = {0};
	if (!CToolsHelp::Memcpy(cSerialName, IVS_SERIAL_NAME_LEN, pSerialChannelCfg->cSerialName, IVS_SERIAL_NAME_LEN))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Memcpy(cSerialName, IVS_SERIAL_NAME_LEN, pSerialChannelCfg->cSerialName, IVS_SERIAL_NAME_LEN) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xml.SetElemValue(cSerialName);

	(void)xml.AddElem("SerialType");
	(void)xml.IntoElem();
	const IVS_CHAR *pSerialType = CDevEnum::GetSerialTypeCH(static_cast<IVS_INT32>(pSerialChannelCfg->uiSerialType));
	(void)xml.SetElemValue(pSerialType);

	(void)xml.AddElem("BaudRate");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pSerialChannelCfg->uiBaudRate)).c_str());

	(void)xml.AddElem("DataBit");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pSerialChannelCfg->uiDataBit)).c_str());

	(void)xml.AddElem("Parity");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pSerialChannelCfg->uiParity)).c_str());

	(void)xml.AddElem("StopBit");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pSerialChannelCfg->uiStopBit)).c_str());

	(void)xml.AddElem("FlowControl");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pSerialChannelCfg->uiFlowControl)).c_str());

	xml.GetRootPos();

	return IVS_SUCCEED;
}

IVS_INT32 CDeviceParaConfigXMLProcess::GetSerialChannelCfgParseXML(IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg, CXml &xml)
{
	CHECK_POINTER(pSerialChannelCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/SerialChanInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get SerialChannelCfg ParseXML fail", "xml.FindElemEx(Content/TimeInfo)");
		return IVS_XML_INVALID;
	}
	const char* pTemp = NULL;
	(void)xml.IntoElem();
	GET_ELEM_VALUE_CHAR("SerialName", pTemp, pSerialChannelCfg->cSerialName, IVS_SERIAL_NAME_LEN, xml);
	
	GET_ELEM_VALUE_NUM_FOR_UINT("SerialIndex", pTemp, pSerialChannelCfg->uiSerialIndex, xml);
	
	IVS_CHAR cSerialType[10] = {0};
	GET_ELEM_VALUE_CHAR("SerialType", pTemp, cSerialType, 10, xml);
	pSerialChannelCfg->uiSerialType = static_cast<IVS_UINT32> (CDevEnum::GetSerialTypeEnum(cSerialType));

	GET_ELEM_VALUE_NUM_FOR_UINT("BaudRate", pTemp, pSerialChannelCfg->uiBaudRate, xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("DataBit", pTemp, pSerialChannelCfg->uiDataBit, xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("Parity", pTemp, pSerialChannelCfg->uiParity, xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("StopBit", pTemp, pSerialChannelCfg->uiStopBit, xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("FlowControl", pTemp, pSerialChannelCfg->uiFlowControl, xml);

	return IVS_SUCCEED;
}

// �����������չͼ�����ƴ��XML
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraImgCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_IMAGING_CFG* pCameraImgCfg, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraImgCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0","UTF-8","");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDevCode);
	// ͼ�������Ϣ
	(void)xml.AddElem("ImagingInfo");
	(void)xml.IntoElem();

	// ���ⲹ����Ϣ
	(void)xml.AddChildElem("BacklightInfo");
	(void)xml.IntoElem();
	// ���ⲹ��ģʽ
	(void)xml.AddChildElem("BacklightMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommStatusType(static_cast<IVS_INT32>(pCameraImgCfg->stBacklight.uiBacklightMode)));
	// ���ⲹ���ȼ�
	(void)xml.AddElem("BacklightLevel");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stBacklight.uiBacklightLevel)).c_str());
	xml.OutOfElem();

	// �ع������Ϣ
	(void)xml.AddElem("ExposureInfo");
	(void)xml.IntoElem();
	// �ع�ģʽ
	(void)xml.AddChildElem("ExposureMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommModeString(pCameraImgCfg->stExposure.uiExposureMode));
	// �ع�����
	(void)xml.AddElem("ExposurePriority");
	(void)xml.SetElemValue(CDevEnum::GetExposurePriorityCH(static_cast<IVS_INT32>(pCameraImgCfg->stExposure.uiExposurePriority)));
	// �عⴰ����Ϣ
	(void)xml.AddElem("AreaInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("X1");
	(void)xml.IntoElem();
	IVS_CHAR cLeft[STR_LEN] = {0};
	if (!CToolsHelp::Snprintf(cLeft, STR_LEN, "%f", pCameraImgCfg->stExposure.rectWindow.left))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cLeft, STR_LEN, %f, pCameraImgCfg->stExposure.rectWindow.left) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xml.SetElemValue(cLeft);

	(void)xml.AddElem("Y1");
	(void)xml.IntoElem();
	IVS_CHAR cTop[STR_LEN] = {0};
	if (!CToolsHelp::Snprintf(cTop, STR_LEN, "%f", pCameraImgCfg->stExposure.rectWindow.top))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cTop, STR_LEN, %f, pCameraImgCfg->stExposure.rectWindow.top) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xml.SetElemValue(cTop);

	(void)xml.AddElem("X2");
	(void)xml.IntoElem();
	IVS_CHAR cRight[STR_LEN] = {0};
	if (!CToolsHelp::Snprintf(cRight, STR_LEN, "%f", pCameraImgCfg->stExposure.rectWindow.right))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cRight, STR_LEN, %f, pCameraImgCfg->stExposure.rectWindow.right) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xml.SetElemValue(cRight);

	(void)xml.AddElem("Y2");
	(void)xml.IntoElem();
	IVS_CHAR cBottom[STR_LEN] = {0};
	if (!CToolsHelp::Snprintf(cBottom, STR_LEN, "%f", pCameraImgCfg->stExposure.rectWindow.bottom))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "Snprintf(cBottom, STR_LEN, %f, pCameraImgCfg->stExposure.rectWindow.bottom) fail", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	(void)xml.SetElemValue(cBottom);
	xml.OutOfElem();
	// �ع�ʱ��
	(void)xml.AddElem("ExposureTime");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stExposure.uiExposureTime)).c_str());
	// �ع�����
	(void)xml.AddElem("ExposureGain");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stExposure.uiExposureGain)).c_str());
	// ��Ȧ
	(void)xml.AddElem("ExposureIris");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stExposure.uiExposureIris)).c_str());
	xml.OutOfElem();

	// �佹������Ϣ
	(void)xml.AddElem("FocusInfo");
	(void)xml.IntoElem();
	// �佹ģʽ
	(void)xml.AddChildElem("FocusMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommModeString(pCameraImgCfg->stFocus.uiFocusMode));
	// �佹�ٶ�
	(void)xml.AddElem("FocusSpeed");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stFocus.uiFocusSpeed)).c_str());
	// ��С�佹��Χ
	(void)xml.AddElem("NearLimit");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stFocus.uiNearLimit)).c_str());
	// ���佹��Χ
	(void)xml.AddElem("FarLimit");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stFocus.uiFarLimit)).c_str());
	xml.OutOfElem();

	// �������
	(void)xml.AddElem("IrCutFilter");
	(void)xml.SetElemValue(CDevEnum::GetEnableModeStr(static_cast<IVS_INT32>(pCameraImgCfg->uiIrCutFilter)));

	// ��̬��Ϣ
	(void)xml.AddElem("WideDynamic");
	(void)xml.IntoElem();
	// ��̬ģʽ
	(void)xml.AddChildElem("WideDynamicMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommStatusType(static_cast<IVS_INT32>(pCameraImgCfg->stWideDynamic.uiWideDynamicMode)));
	// ��̬�ȼ�
	(void)xml.AddElem("WideDynamicLevel");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stWideDynamic.uiWideDynamicLevel)).c_str());
	xml.OutOfElem();

	// ���
	(void)xml.AddElem("Sharpness");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->uiSharpness)).c_str());

	// ��ƽ�����
	(void)xml.AddElem("WhiteBalanceInfo");
	(void)xml.IntoElem();
	// ��ƽ��ģʽ
	(void)xml.AddChildElem("WhiteBalanceMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetCommModeString(pCameraImgCfg->stWhiteBalance.uiWhiteBalanceMode));
	// ��ɫ����
	(void)xml.AddElem("CrGain");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stWhiteBalance.uiCrGain)).c_str());
	// ��ɫ����
	(void)xml.AddElem("CbGain");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stWhiteBalance.uiCbGain)).c_str());
	xml.OutOfElem();

	// ��������Ϣ
	(void)xml.AddElem("StabilizationInfo");
	(void)xml.IntoElem();
	// ������ģʽ
	(void)xml.AddChildElem("StabilizationMode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetEnableModeStr(static_cast<IVS_INT32>(pCameraImgCfg->stStabilization.uiStabilizationMode)));
	// �������ȼ�
	(void)xml.AddElem("StabilizationLevel");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraImgCfg->stStabilization.uiStabilizationLevel)).c_str());
	xml.OutOfElem();

	xml.OutOfElem();

	return IVS_SUCCEED;
}

#define CAMERA_IMGAE_LEN 20
// ��ѯ�������չͼ���������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetCameraImgCfgParseXML(IVS_CAMERA_IMAGING_CFG* pCameraImgCfg, CXml &xml)
{
	CHECK_POINTER(pCameraImgCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/ImagingInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElemEx('Content/ImagingInfo') fail", "NA");
		return IVS_XML_INVALID;
	}

	const char* pTemp = NULL;
	// ���ⲹ����Ϣ
	if (xml.FindElem("BacklightInfo"))
	{
		(void)xml.IntoElem();
		// ���ⲹ��ģʽ
		IVS_CHAR cBacklightMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("BacklightMode", pTemp, cBacklightMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stBacklight.uiBacklightMode = CDevEnum::GetCommStatus(cBacklightMode);

		// ���ⲹ���ȼ�
		GET_ELEM_VALUE_NUM_FOR_UINT("BacklightLevel", pTemp, pCameraImgCfg->stBacklight.uiBacklightLevel, xml);
		xml.OutOfElem();
	}
	// �ع������Ϣ
	if (xml.FindElem("ExposureInfo"))
	{
		(void)xml.IntoElem();
		// �ع�ģʽ
		IVS_CHAR cExposureMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("ExposureMode", pTemp, cExposureMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stExposure.uiExposureMode = CDevEnum::GetCommMode(cExposureMode);

		// �ع�����
		IVS_CHAR cExposurePriority[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("ExposurePriority", pTemp, cExposurePriority, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stExposure.uiExposurePriority = static_cast<IVS_UINT32>(CDevEnum::GetExposurePriorityINT(cExposurePriority));

		// �عⴰ����Ϣ
		if (!xml.FindElem("AreaInfo"))
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElem('AreaInfo')", "NA");
			return IVS_XML_INVALID;
		}
		(void)xml.IntoElem();
		GET_ELEM_VALUE_NUM_FOR_FLOAT("X1", pTemp, pCameraImgCfg->stExposure.rectWindow.left, xml);
		GET_ELEM_VALUE_NUM_FOR_FLOAT("Y1", pTemp, pCameraImgCfg->stExposure.rectWindow.top, xml);
		GET_ELEM_VALUE_NUM_FOR_FLOAT("X2", pTemp, pCameraImgCfg->stExposure.rectWindow.right, xml);
		GET_ELEM_VALUE_NUM_FOR_FLOAT("Y2", pTemp, pCameraImgCfg->stExposure.rectWindow.bottom, xml);
		xml.OutOfElem();

		// �ع�ʱ��
		GET_ELEM_VALUE_NUM_FOR_UINT("ExposureTime", pTemp, pCameraImgCfg->stExposure.uiExposureTime, xml);
		// �ع�����
		GET_ELEM_VALUE_NUM_FOR_UINT("ExposureGain", pTemp, pCameraImgCfg->stExposure.uiExposureGain, xml);
		// ��Ȧ
		GET_ELEM_VALUE_NUM_FOR_UINT("ExposureIris", pTemp, pCameraImgCfg->stExposure.uiExposureIris, xml);
		xml.OutOfElem();
	}
	// �佹������Ϣ
	if (xml.FindElem("FocusInfo"))
	{
		(void)xml.IntoElem();
		// �佹ģʽ
		IVS_CHAR cFocusMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("FocusMode", pTemp, cFocusMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stFocus.uiFocusMode = CDevEnum::GetCommMode(cFocusMode);

		// �佹�ٶ�
		GET_ELEM_VALUE_NUM_FOR_UINT("FocusSpeed", pTemp, pCameraImgCfg->stFocus.uiFocusSpeed, xml);
		// ��С�佹��Χ
		GET_ELEM_VALUE_NUM_FOR_UINT("NearLimit", pTemp, pCameraImgCfg->stFocus.uiNearLimit, xml);
		// ���佹��Χ
		GET_ELEM_VALUE_NUM_FOR_UINT("FarLimit", pTemp, pCameraImgCfg->stFocus.uiFarLimit, xml);
		xml.OutOfElem();
	}
	// �������
	if (xml.FindElem("IrCutFilter"))
	{
		IVS_CHAR cIrCutFilter[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("IrCutFilter", pTemp, cIrCutFilter, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->uiIrCutFilter = static_cast<IVS_UINT32>(CDevEnum::GetEnableMode(cIrCutFilter));
	}
	// ��̬��Ϣ
	if (xml.FindElem("WideDynamic"))
	{
		(void)xml.IntoElem();
		// ��̬ģʽ
		IVS_CHAR cWideDynamicMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("WideDynamicMode", pTemp, cWideDynamicMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stWideDynamic.uiWideDynamicMode = CDevEnum::GetCommStatus(cWideDynamicMode);

		// ��̬�ȼ�
		GET_ELEM_VALUE_NUM_FOR_UINT("WideDynamicLevel", pTemp, pCameraImgCfg->stWideDynamic.uiWideDynamicLevel, xml);
		xml.OutOfElem();
	}
	// ���
	if (xml.FindElem("Sharpness"))
	{
		GET_ELEM_VALUE_NUM_FOR_UINT("Sharpness", pTemp, pCameraImgCfg->uiSharpness, xml);
	}
	// ��ƽ�����
	if (xml.FindElem("WhiteBalanceInfo"))
	{
		(void)xml.IntoElem();
		// ��ƽ��ģʽ
		IVS_CHAR cWhiteBalanceMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("WhiteBalanceMode", pTemp, cWhiteBalanceMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stWhiteBalance.uiWhiteBalanceMode = CDevEnum::GetCommMode(cWhiteBalanceMode);

		// ��ɫ����
		GET_ELEM_VALUE_NUM_FOR_UINT("CrGain", pTemp, pCameraImgCfg->stWhiteBalance.uiCrGain, xml);

		// ��ɫ����
		GET_ELEM_VALUE_NUM_FOR_UINT("CbGain", pTemp, pCameraImgCfg->stWhiteBalance.uiCbGain, xml);
		xml.OutOfElem();
	}
	// ��������Ϣ
	if (xml.FindElem("StabilizationInfo"))
	{
		(void)xml.IntoElem();
		// ������ģʽ
		IVS_CHAR cStabilizationMode[CAMERA_IMGAE_LEN] = {0};
		GET_ELEM_VALUE_CHAR("StabilizationMode", pTemp, cStabilizationMode, CAMERA_IMGAE_LEN, xml);
		pCameraImgCfg->stStabilization.uiStabilizationMode = static_cast<IVS_UINT32>(CDevEnum::GetEnableMode(cStabilizationMode));

		// �������ȼ�
		GET_ELEM_VALUE_NUM_FOR_UINT("StabilizationLevel", pTemp, pCameraImgCfg->stStabilization.uiStabilizationLevel, xml);
		xml.OutOfElem();
	}

	return IVS_SUCCEED;
}

// �����������չ����
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraExCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_EXTEND_CFG* pCameraExCfg, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pDomainCode, IVS_OPERATE_MEMORY_ERROR);
	CHECK_POINTER(pCameraExCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0", "UTF-8", "");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("CameraExtendedInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("WatermarkEnable");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraExCfg->bEnableWatermark).c_str());

	(void)xml.AddElem("BandwidthAdaptive");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraExCfg->bEnableBandwidthAdaptive).c_str());

	(void)xml.AddElem("Cache");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraExCfg->bEnableCache).c_str());

	(void)xml.AddElem("MediaSecurityInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("MediaSecurityEnable");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(pCameraExCfg->stMediaSecurityCfg.bEnableMediaSecurity).c_str());

	(void)xml.AddElem("MediaSecurityAlgorithm");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraExCfg->stMediaSecurityCfg.uiMediaSecurityAlgorithm)).c_str());
	xml.OutOfElem();

	xml.OutOfElem();

	return IVS_SUCCEED;
}

#define CAMERA_EX_LEN 20
// ��ѯ�������չ��������XML
IVS_INT32 CDeviceParaConfigXMLProcess::GetCameraExCfgParseXML(IVS_CAMERA_EXTEND_CFG* pCameraExCfg, CXml &xml)
{
	CHECK_POINTER(pCameraExCfg, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElem("Content"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get CameraExCfg ParseXML fail", "xml.FindElem(Content) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	if (!xml.FindElem("CameraExtendedInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get CameraExCfg ParseXML fail", "xml.FindElem(CameraExtendedInfo) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	const IVS_CHAR* pTemp = NULL;
	GET_ELEM_VALUE_NUM("WatermarkEnable", pTemp, pCameraExCfg->bEnableWatermark, xml);

	GET_ELEM_VALUE_NUM("BandwidthAdaptive", pTemp, pCameraExCfg->bEnableBandwidthAdaptive, xml);

	GET_ELEM_VALUE_NUM("Cache", pTemp, pCameraExCfg->bEnableCache, xml);

	if (!xml.FindElem("MediaSecurityInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "xml.FindElem(MediaSecurityInfo)", "NA");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	GET_ELEM_VALUE_NUM("MediaSecurityEnable", pTemp, pCameraExCfg->stMediaSecurityCfg.bEnableMediaSecurity, xml);

	GET_ELEM_VALUE_NUM_FOR_UINT("MediaSecurityAlgorithm", pTemp, pCameraExCfg->stMediaSecurityCfg.uiMediaSecurityAlgorithm, xml);
	xml.OutOfElem();

	xml.OutOfElem();

	return IVS_SUCCEED;
}


// �����ץ�Ĳ�������ƴװxml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraSnapshotCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	CHECK_POINTER(pDomainCode, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	CHECK_POINTER(pCameraSnapshotCfg, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0", "UTF-8", "");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("SnapInfo");
	(void)xml.IntoElem();
	(void)xml.AddChildElem("SnapCount");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraSnapshotCfg->uiSnapCount)).c_str());

	(void)xml.AddElem("SnapInterval");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraSnapshotCfg->uiSnapInterval)).c_str());

	xml.GetRootPos();

	return IVS_SUCCEED;
}

// �����ץ�Ĳ�������ƴװxml
IVS_INT32 CDeviceParaConfigXMLProcess::GetCameraSnapshotCfgParseXML(IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg, CXml &xml)
{
	CHECK_POINTER(pCameraSnapshotCfg, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/SnapInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Set CameraExCfg ParseXML fail", "xml.FindElem(CameraExtendedInfo) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	const char* pTemp = NULL;

	GET_ELEM_VALUE_NUM_FOR_UINT("SnapCount", pTemp, pCameraSnapshotCfg->uiSnapCount, xml);
	GET_ELEM_VALUE_NUM_FOR_UINT("SnapInterval", pTemp, pCameraSnapshotCfg->uiSnapInterval, xml);

	xml.GetRootPos();
	return IVS_SUCCEED;
}

// �����ǰ��¼����������ƴװxml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraRecordStreamCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode,IVS_UINT32 uiStreamType, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);
	CHECK_POINTER(pDomainCode, IVS_SDK_RET_INIT_RTSPCLIENT_ERROR);

	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0", "UTF-8", "");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("StreamID");
	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(uiStreamType)).c_str());

	xml.GetRootPos();

	return IVS_SUCCEED;
}

// �����ǰ��¼�������������ý���xml
IVS_INT32 CDeviceParaConfigXMLProcess::GetCameraRecordStreamCfgParseXML(IVS_UINT32* pStreamType, CXml &xml)
{
	CHECK_POINTER(pStreamType, IVS_OPERATE_MEMORY_ERROR);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get CameraRecordStreamCfg ParseXML fail", "xml.FindElem(Content) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	const char* pTemp = NULL;

	IVS_UINT32 ulpStreamType = 0;
	GET_ELEM_VALUE_NUM_FOR_UINT("StreamID", pTemp, ulpStreamType, xml);
	*pStreamType = ulpStreamType;

	xml.GetRootPos();
	return IVS_SUCCEED;
}


// �����FEC����ƴװxml
IVS_INT32 CDeviceParaConfigXMLProcess::SetCameraFECCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_FEC_CFG* pCameraFEC, CXml &xml)
{
	CHECK_POINTER(pDevCode, IVS_PARA_INVALID);
	CHECK_POINTER(pDomainCode, IVS_PARA_INVALID);
	CHECK_POINTER(pCameraFEC, IVS_PARA_INVALID);
	
	IVS_DEBUG_TRACE("");

	(void)xml.AddDeclaration("1.0", "UTF-8", "");
	(void)xml.AddElem("Content");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("DomainCode");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(pDomainCode);

	(void)xml.AddElem("CameraCode");
	(void)xml.SetElemValue(pDevCode);

	(void)xml.AddElem("FECInfo");
	(void)xml.IntoElem();

	(void)xml.AddChildElem("ReliableTransmission");
	(void)xml.IntoElem();
	(void)xml.SetElemValue(CDevEnum::GetReliableTransTypeCHAR(static_cast<IVS_INT32>(pCameraFEC->uiReliableTransmission)));
	 
	if (RELIABLE_TRANSMISSION_FEC == pCameraFEC->uiReliableTransmission)
	{
	 	(void)xml.AddElem("DataDropPercent");
	 	(void)xml.SetElemValue(CToolsHelp::Int2Str(static_cast<IVS_INT32>(pCameraFEC->uiDataDropPercent)).c_str());
	}

	xml.GetRootPos();

	return IVS_SUCCEED;
}

// �����FEC�������ý���xml
IVS_INT32 CDeviceParaConfigXMLProcess::GetCameraFECCfgParseXML(IVS_CAMERA_FEC_CFG* pCameraFEC, CXml &xml)
{
	CHECK_POINTER(pCameraFEC, IVS_PARA_INVALID);
	IVS_DEBUG_TRACE("");

	if (!xml.FindElemEx("Content/FECInfo"))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID, "Get CameraFECCfg ParseXML fail", "xml.FindElem(FECInfo) fail");
		return IVS_XML_INVALID;
	}
	(void)xml.IntoElem();
	const char* pTemp = NULL;

	IVS_CHAR cReliableTransmission[CAMERA_EX_LEN] = {0};
	GET_ELEM_VALUE_CHAR("ReliableTransmission", pTemp, cReliableTransmission, CAMERA_EX_LEN, xml);
	pCameraFEC->uiReliableTransmission = static_cast<IVS_UINT32>(CDevEnum::GetReliableTransTypeINT(cReliableTransmission));

	GET_ELEM_VALUE_NUM_FOR_UINT("DataDropPercent", pTemp, pCameraFEC->uiDataDropPercent, xml);

	xml.GetRootPos();
	return IVS_SUCCEED;
}



