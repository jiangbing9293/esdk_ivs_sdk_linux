/*****************************************************************
 filename    :    devEnum.cpp
 author      :    guandiqun
 created     :    2012/11/19
 description :    ʵ���豸��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*****************************************************************/
#include "devEnum.h"
#include "IVSCommon.h"
#ifdef WIN32
	#include "wtypes.h"
#endif

#ifndef ELEMMAX
#define ELEMMAX 10
#endif

//��Ƶ��ʽ��ö����
const IVS_CHAR chVideoType[][ELEMMAX]  = {"NONE", "PAL", "NTSC"};
const IVS_CHAR chDevType[][ELEMMAX] = {"", "IPC", "DVS", "DVR", "eNVR"};
const IVS_CHAR chResolutionType[][ELEMMAX] = {"","QCIF", "QVGA", "CIF", "2CIF", "DCIF", "VGA", "4CIF",  "HD1", "D1",
                                            "SVGA", "XGA", "720P", "XVGA", "SXGA", "USVGA", "UXGA", "1080P", "4096P" };
const IVS_CHAR chFormatType[][ELEMMAX] = {"NONE", "PAL","NTSC"};

const IVS_CHAR chH264ProfileType[][ELEMMAX] = {"","BP", "EP", "HP", "MP"};
const IVS_CHAR chEnCodeModeCHType[][ELEMMAX] = {"", "H264", "MJPEG", "MPEG4", "MPEG2", "AVS"};

const IVS_CHAR chDevStatus[][ELEMMAX] = {"OFF", "ON"};
const IVS_CHAR chAlamEnable[][ELEMMAX] = {"OFF", "ON"};
const IVS_CHAR chCommStatus[][ELEMMAX] = {"OFF", "ON"};
const IVS_CHAR chTransType[][ELEMMAX] = {"NONE", "FEC", "ARQ", "HARQ"};
const IVS_CHAR chDataFormat[][ELEMMAX] = {"", "G711A", "G711U", "G726", "AAC"};

const IVS_CHAR chCameraType[][ELEMMAX] = {"FIX", "PTZ", "DOME", "FIX_DOME"};

const IVS_CHAR chCommMode[][ELEMMAX] = {"AUTO", "MANUAL"};
const IVS_CHAR chExposurePriority[][ELEMMAX] = {"LowNoise", "FrameRate"};
const IVS_CHAR chEnableMode[][ELEMMAX] = {"OFF", "ON", "AUTO", "EXTENDED"};

// edit by wanglei 00165153:2013.10.14 DTS2013101404169 ��IVS_UINT32 - > IVS_CAMERA_RECORD_STREAM_CFG��
// CONFIG_CAMERA_RECORD_STREAM_CFG ͨ��SDK�ӿڵ��ò�ѯǰ��¼�������������᷵��109100005�����ڴ�ʧ��
const IVS_UINT32 chSizeOfConfigType[] = {0, 
sizeof(IVS_DEVICE_CFG),sizeof(IVS_DEVICE_NET_CFG),sizeof(IVS_DEVICE_TIME_CFG),
sizeof(IVS_CAMERA_CFG),sizeof(IVS_CAMERA_STREAM_CFG),sizeof(IVS_CAMERA_DISPLAY_CFG),
sizeof(IVS_CAMERA_IMAGING_CFG),sizeof(IVS_CAMERA_OSD_CFG),sizeof(IVS_MOTION_DETECTION),
sizeof(IVS_VIDEO_HIDE_ALARM),sizeof(IVS_VIDEO_MASK),sizeof(IVS_AUDIO_CFG),
sizeof(IVS_SERIAL_CHANNEL_CFG),sizeof(IVS_ALARM_IN_CFG),sizeof(IVS_ALARM_OUT_CFG),
sizeof(IVS_PTZ_CFG),sizeof(IVS_CAMERA_EXTEND_CFG),sizeof(IVS_CAMERA_SNAPSHOT_CFG),
sizeof(IVS_CAMERA_RECORD_STREAM_CFG),sizeof(IVS_CAMERA_FEC_CFG), sizeof(IVS_CAMERA_STREAM_EVENTEX_CFG)
};


// ��ȡͨ��״̬�ַ�
const IVS_CHAR* CDevEnum::GetCommStatusType( IVS_BOOL bEnable )
{
    if (bEnable)
    {
        return chCommStatus[1];
    }

    return chCommStatus[0];
}

// ��ȡͨ��״̬
IVS_UINT32 CDevEnum::GetCommStatus(const char* pEnable)
{
	IVS_UINT32 iStatus = 0;
	if (0 == strcmp(chCommStatus[0], pEnable))
	{
		iStatus = 0;
	}  
	else if (0 == strcmp(chCommStatus[1], pEnable))
	{
		iStatus = 1;
	}
	else
	{
		iStatus = 2;
	}

	return iStatus;
}

// ��ȡͨ��ģʽ
IVS_UINT32 CDevEnum::GetCommMode(const char* pEnable)
{
	IVS_UINT32 iMode = 0;
	if (0 == strcmp(chCommMode[0], pEnable))
	{
		iMode = 0;
	}  
	else if (0 == strcmp(chCommMode[1], pEnable))
	{
		iMode = 1;
	}
	else
	{
		iMode = 2;
	}

	return iMode;
}

// ��ȡͨ��ģʽ�ִ�
const IVS_CHAR* CDevEnum::GetCommModeString(IVS_UINT32 uiMode)
{
	if (uiMode == 1)
	{
		return chCommMode[1];
	}
	
	return chCommMode[0];
}

// ��ȡ�澯ʹ��
const IVS_CHAR* CDevEnum::GetAlamEnableType( IVS_BOOL bAlamEnable )
{
    if (bAlamEnable)
    {
        return chAlamEnable[1];
    }
    
    return chAlamEnable[0];
}

// ��ȡ�澯ʹ��
IVS_BOOL CDevEnum::GetAlamEnableBool(const IVS_CHAR* pAlamEnable)
{
	CHECK_POINTER(pAlamEnable, FALSE);
    IVS_BOOL bRet = FALSE;
   
    if (0 == strcmp(chAlamEnable[1], pAlamEnable))
    {
        bRet = TRUE ;
    }   

    return bRet;
}

// ��ȡ�豸״̬
const IVS_CHAR* CDevEnum::GetDevStatus( IVS_INT32 iStatus )
{

    IVS_INT32 iMaxLen = sizeof(chDevStatus)/sizeof(chDevStatus[0]);

    if (iStatus > iMaxLen || iStatus < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iStatus fail", "NA");
        return "";
    }

    return chDevStatus[iStatus];
}

// ��ȡ�����ʽ
IVS_INT32 CDevEnum::GetEnCodeModeEnum(const IVS_CHAR* pCodeMode)
{
    CHECK_POINTER(pCodeMode, IVS_OPERATE_MEMORY_ERROR);

    IVS_INT32 iRet = 0;

    IVS_INT32 iLen = sizeof(chEnCodeModeCHType)/sizeof(chEnCodeModeCHType[0]);
    for (IVS_INT32 i = 0; i < iLen; i++ )
    {
        if (0 == strcmp(chEnCodeModeCHType[i], pCodeMode))
        {
            iRet = i ;
        }
    }
    BP_RUN_LOG_INF("Get H264ProfileEnum fail", "VideoType=%d",iRet);

    return iRet;
}

// ��ȡ�����ʽ
const IVS_CHAR *CDevEnum::GetEnCodeModeCH(IVS_INT32 iCodeMode)
{
  
    IVS_INT32 iMaxLen = sizeof(chEnCodeModeCHType)/sizeof(chEnCodeModeCHType[0]);

    if (iCodeMode > iMaxLen || iCodeMode < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iCodeMode fail", "NA");
        return "";
    }

    return chEnCodeModeCHType[iCodeMode];
}

// ��ȡ�����ʽΪH264��Profile
const IVS_CHAR *CDevEnum::GetH264ProfileCH(IVS_INT32 iProfile)
{

    IVS_INT32 iMaxLen = sizeof(chH264ProfileType)/sizeof(chH264ProfileType[0]);

    if (iProfile > iMaxLen || iProfile < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iProfile fail", "NA");
        return "";
    }

    return chH264ProfileType[iProfile];
}

// ��ȡ�����ʽΪH264��Profile
IVS_INT32 CDevEnum::GetH264ProfileEnum(const IVS_CHAR* pH264Profile)
{

    CHECK_POINTER(pH264Profile, IVS_OPERATE_MEMORY_ERROR);

    IVS_INT32 iRet = 0;
    
    IVS_INT32 iLen = sizeof(chH264ProfileType)/sizeof(chH264ProfileType[0]);
    for (IVS_INT32 i = 0; i < iLen; i++ )
    {
        if (0 == strcmp(chH264ProfileType[i], pH264Profile))
        {
            iRet = i ;
			break;
        }
    }
    BP_DBG_LOG("Get VideoFormatEnum fail", "VideoType=%d",iRet);

    return iRet;

}


// ��ȡ��Ƶ��ʽ��ö����
IVS_INT32 CDevEnum::GetVideoFormatEnum(const IVS_CHAR* pVideoType )
{

    CHECK_POINTER(pVideoType, IVS_OPERATE_MEMORY_ERROR);
    IVS_INT32 iRet = 0;
    IVS_INT32 iLen = sizeof(chVideoType)/sizeof(chVideoType[0]);
    for (IVS_INT32 i = 0; i < iLen; i++ )
    {
        if (0 == strcmp(chVideoType[i], pVideoType))
        {
            iRet = i ;
			break;
        }
    }
    BP_DBG_LOG("Get VideoFormatEnum fail", "VideoType=%d",iRet);

    return iRet;
}

// ��ȡ��Ƶ��ʽ��ö����
const IVS_CHAR* CDevEnum::GetVideoFormatType( const IVS_INT32 iFormatType )
{
    
    IVS_INT32 iMaxLen = sizeof(chFormatType)/sizeof(chFormatType[0]);

    if (iFormatType > iMaxLen || iFormatType < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iFormatType fail", "NA");
        return "";
    }

    return chFormatType[iFormatType];
}

// ��ȡ��ͷ�����ֱ���
const IVS_CHAR *CDevEnum::GetResolutionCH(IVS_INT32 iResolution)
{
    IVS_INT32 iMaxLen = sizeof(chResolutionType)/sizeof(chResolutionType[0]);

    if (iResolution > iMaxLen || iResolution < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iResolution fail", "NA");
        return "";
    }

    return chResolutionType[iResolution];
}

// ��ȡ��ͷ�����ֱ���
IVS_INT32 CDevEnum::GetResolutionEnum(const IVS_CHAR*pResolution)
{

    IVS_INT32 iRet = 0;
    CHECK_POINTER(pResolution, IVS_OPERATE_MEMORY_ERROR);

    IVS_INT32 iLen = sizeof(chResolutionType)/sizeof(chResolutionType[0]);
    for (IVS_INT32 i = 0; i < iLen ; i ++)
    {
        if (0 == strcmp(chResolutionType[i], pResolution))
        {
            iRet = i;
			break;
        }
    }  
   
    BP_RUN_LOG_INF("Get ResolutionEnum fail", "resolution=%d",iRet);

    return iRet;
}

// �������ͻ�ȡ
const IVS_CHAR* CDevEnum::GetDevType(IVS_INT32 iDeviceType)
{

    IVS_INT32 iMaxLen = sizeof(chDevType)/sizeof(chDevType[0]);

    if (iDeviceType > iMaxLen || iDeviceType < 0)
    {
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iDeviceType fail", "NA");
        return "";
    }

    return chDevType[iDeviceType];
}

// �������ͻ�ȡ
IVS_UINT32 CDevEnum::GetDevTypeEnum( const IVS_CHAR* pDeviceType )
{

    IVS_INT32 iRet = 0;
    CHECK_POINTER(pDeviceType, IVS_OPERATE_MEMORY_ERROR);

    IVS_INT32 iLen = sizeof(chDevType)/sizeof(chDevType[0]);
    for (IVS_INT32 i = 0; i < iLen ; i ++)
    {
        if (0 == strcmp(chDevType[i], pDeviceType))
        {

            iRet = i;
        }
    }  

    IVS_UINT32 uiRet = static_cast<IVS_UINT32>(iRet);
    return uiRet;
}

// ��ȡ�豸״̬
IVS_UINT32 CDevEnum::GetDevStatusEnum( const IVS_CHAR* pDevStatus )
{

    CHECK_POINTER(pDevStatus, IVS_OPERATE_MEMORY_ERROR);
    IVS_INT32 iRet = DEV_STATUS_ONLINE;
    if (0 == strcmp("OFF", pDevStatus))//  ����
    {
        iRet = DEV_STATUS_OFFLINE;
    }         
    IVS_UINT32 uiRet = static_cast<IVS_UINT32>(iRet);

    return uiRet; 
}

// ��ȡ�����ö����
IVS_BOOL CDevEnum::GetOrderTypeBool( const IVS_CHAR* pLoginType )
{

    CHECK_POINTER(pLoginType, IVS_OPERATE_MEMORY_ERROR);

    if (0 == strcmp(pLoginType, "DOWN"))
    {
        return TRUE;
    }

    return  FALSE;
}
// ��ȡField��ö����
IVS_BOOL CDevEnum::GetFieldEnum(const IVS_CHAR* pFieldType)
{

    CHECK_POINTER(pFieldType, IVS_OPERATE_MEMORY_ERROR);

    if (0 == strcmp(pFieldType, "EXACT"))
    {
        return TRUE;
    }

    return  FALSE;
}


// ��ȡ�澯����
const IVS_CHAR* CDevEnum::GetAlamType( IVS_BOOL bAlam )
{
    if (bAlam)
    {
        return "1";
    }

    return "0";   
}


// ��ȡ���������
const IVS_CHAR *CDevEnum::GetPosNegDirCtlCH(bool bPosNegDirCtl)
{

    if (bPosNegDirCtl)
    {
        return "FORWARD";
    }
    else
    {
        return "BACK";
    }
}

// ��ȡ��������
const IVS_CHAR *CDevEnum::GetBitRateTypeCH(IVS_INT32 bitRateType)
{
    const IVS_CHAR *result = "";
    switch (bitRateType)
    {
    case BITRATE_CBR:
        result = "CBR";
        break;
    case BITRATE_VBR:
        result = "VBR";
        break;
    default:;
    }

    return result;
}


// ��ȡ�����ʽΪMJPEG��Profile
const IVS_CHAR *CDevEnum::GetMjpegProfileCH(IVS_INT32 profile)
{
    const IVS_CHAR* result = "";
    switch (profile)
    {
    case MJPEG_PROFILE_SP:
        result ="SP";
        break;
    case MJPEG_PROFILE_ASP:
        result ="ASP";
        break;
    default:;
    }

    return result;
}

// ��ȡ���������
IVS_INT32 CDevEnum::GetPosNegDirCtlEnum(const IVS_CHAR* posNegDirectControl)
{

    IVS_INT32 iRet = 1;
    CHECK_POINTER(posNegDirectControl, iRet);
    //const std::string strPosNegDirectControl[] = {"BACK", "FORWARD"};
    if (0 == strcmp("BACK", posNegDirectControl))
    {
        iRet = 0;
    }

    return iRet;

}



// ��ȡ��������
IVS_INT32 CDevEnum::GetBitRateTypeEnum(const IVS_CHAR* bitRateType)
{

    IVS_INT32 iRet = -1;
    CHECK_POINTER(bitRateType, IVS_FAIL);

    if (0 == strcmp("CBR", bitRateType))
    {
        iRet = BITRATE_CBR;
    }
    else if (0 == strcmp("VBR", bitRateType))
    {
        iRet = BITRATE_VBR; 
    }

    return iRet;

}


// ��ȡ�����ʽΪMJPEG��Profile
IVS_INT32 CDevEnum::GetMjpegProfileEnum(const IVS_CHAR* profile)
{

    IVS_INT32  iRet = -1;
    CHECK_POINTER(profile, iRet);

    if (0 == strcmp("SP", profile))
    {
        iRet = MJPEG_PROFILE_SP;
    }
    else if (0 == strcmp("ASP", profile))
    {
        iRet =  MJPEG_PROFILE_ASP; 
    } 

    return  iRet;

}

// ��ȡ���豸Э������
const IVS_CHAR *CDevEnum::GetDevProtocolTypeCH(IVS_INT32 typeCode)
{

	const IVS_CHAR* result = "";
	switch (typeCode)
	{
	case DEVICE_PRO_TEYES:
		result ="TEYES";
		break;
	case DEVICE_PRO_ONVIF:
		result ="ONVIF";
		break;
	case DEVICE_PRO_HUAWEI:
		result ="HWSDK";
		break;
	case DEVICE_PRO_HIK:
		result ="HIKSDK";
		break;
	default:
		result = "-1";
		break;
	}

	return result;
}

// ��ȡ���豸Э������
IVS_INT32 CDevEnum::GetDevProtocolTypeEnum( const IVS_CHAR* typeCode )
{

	IVS_INT32  iRet = -1;
	CHECK_POINTER(typeCode, IVS_FAIL);

	if (0 == strcmp("TEYES", typeCode))
	{
		iRet = DEVICE_PRO_TEYES;
	}
	else if (0 == strcmp("ONVIF", typeCode))
	{
		iRet =  DEVICE_PRO_ONVIF; 
	} 
	else if (0 == strcmp("HWSDK", typeCode))
	{
		iRet =  DEVICE_PRO_HUAWEI; 
	}
	else if (0 == strcmp("HIKSDK", typeCode))
	{
		iRet =  DEVICE_PRO_HIK; 
	}

	return  iRet;

}

// ��ȡ���豸�ṩ������
const IVS_CHAR *CDevEnum::GetDevVendorTypeCH(IVS_INT32 typeCode)
{

	const IVS_CHAR* result = "";
	switch (typeCode)
	{
	case VENDOR_HUAWEI:
		result ="HUAWEI";
		break;
	case VENDOR_HIKVISION:
		result ="HIK";
		break;
	case VENDOR_DAHUA:
		result ="DAHUA";
		break;
// 	case DEVICE_PRO_HIK:
// 		result ="HIKSDK";
// 		break;
	default:
		result = "-1";
		break;
	}

	return result;
}

// ��ȡ���豸�ṩ������
IVS_INT32 CDevEnum::GetDevVendorTypeEnum( const IVS_CHAR* typeCode )
{

	IVS_INT32  iRet = -1;
	CHECK_POINTER(typeCode, IVS_FAIL);

	if (0 == strcmp("HUAWEI", typeCode))
	{
		iRet = VENDOR_HUAWEI;
	}
	else if (0 == strcmp("HIK", typeCode))
	{
		iRet =  VENDOR_HIKVISION; 
	} 
	else if (0 == strcmp("DAHUA", typeCode))
	{
		iRet =  VENDOR_DAHUA; 
	}

	return  iRet;
}

// ��ȡ��������
IVS_INT32 CDevEnum::GetSerialTypeEnum( const IVS_CHAR* typeCode )
{

	IVS_INT32  iRet = -1;
	CHECK_POINTER(typeCode, IVS_FAIL);

	if (0 == strcmp("RS232", typeCode))
	{
		iRet = SERIAL_RS232;
	}
	else if (0 == strcmp("RS485", typeCode))
	{
		iRet =  SERIAL_RS485; 
	} 
	else if (0 == strcmp("RS422", typeCode))
	{
		iRet =  SERIAL_RS422; 
	}

	return  iRet;
}

// ��ȡ�������Ͷ�Ӧ���ַ���
const IVS_CHAR *CDevEnum::GetSerialTypeCH(IVS_INT32 typeCode)
{

	const IVS_CHAR* result = "";
	switch (typeCode)
	{
	case SERIAL_RS232:
		result ="RS232";
		break;
	case SERIAL_RS485:
		result ="RS485";
		break;
	case SERIAL_RS422:
		result ="RS422";
		break;
	default:
		result = "-1";
		break;
	}

	return result;
}

// ��ȡ�ɿ��������ʹ��ַ���������
IVS_INT32 CDevEnum::GetReliableTransTypeINT(const IVS_CHAR* pTransType)
{
	CHECK_POINTER(pTransType, IVS_OPERATE_MEMORY_ERROR);

	IVS_INT32 iRet = -1;
	IVS_INT32 iLen = sizeof(chTransType) / sizeof(chTransType[0]);
	for (int i = 0;i < iLen;i++)
	{
		if (0 == strcmp(chTransType[i], pTransType))
		{
			iRet = i;
			break;
		}
	}

	BP_RUN_LOG_INF("Get ReliableTransType", "resolution=%d",iRet);	

	return iRet;
}

// ��ȡ�ɿ��������ʹ����ε��ַ���
const IVS_CHAR* CDevEnum::GetReliableTransTypeCHAR(IVS_INT32 iTransType)
{
	const IVS_CHAR* result = "";
	switch(iTransType)
	{
	case RELIABLE_TRANSMISSION_NONE:
		result = "NONE";
		break;
	case RELIABLE_TRANSMISSION_FEC:
		result = "FEC";
		break;
	case RELIABLE_TRANSMISSION_ARQ:
		result = "ARQ";
		break;
	case RELIABLE_TRANSMISSION_HARQ:
		result = "HARQ";
		break;
	default:
		{
			BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "iTransType fail", "NA");
			result = NULL;
		}	
		break;
	}

	return result;
}

// ��ȡ���ݸ�ʽ���ʹ��ַ���������
IVS_INT32 CDevEnum::GetDataFormatINT(const IVS_CHAR* pDataFormat)
{
	CHECK_POINTER(pDataFormat, IVS_OPERATE_MEMORY_ERROR);
	IVS_INT32 iRet = -1;
	IVS_INT32 iLen = sizeof(chDataFormat)/sizeof(chDataFormat[0]);
	for (int i = 1; i < iLen; i++)
	{
		if (0 == strcmp(chDataFormat[i], pDataFormat))
		{
			iRet = i;
			break;
		}
	}
	BP_RUN_LOG_INF("Get DataFormat", "resolution=%d",iRet);
	return iRet;
}

// ��ȡ���ݸ�ʽ���ʹ����ε��ַ���
const IVS_CHAR* CDevEnum::GetDataFormatCHAR(IVS_INT32 iDataFormat)
{
	const IVS_CHAR* result = "";
	switch(iDataFormat)
	{
	case PAY_AUDIO_LOAD_TYPE_PCMA:
		result = "G711A";
		break;
	case PAY_AUDIO_LOAD_TYPE_PCMU:
		result = "G711U";
		break;
	case PAY_AUDIO_LOAD_TYPE_G726:
		result = "G726";
		break;
	case PAY_AUDIO_LOAD_TYPE_AAC:
		result = "AAC";
		break;
	default:
		{
			BP_RUN_LOG_ERR(IVS_XML_INVALID, "iDataFormat fail", "NA");
			result = NULL;
		}
		break;
	}
	return result;
}

// ��ȡ�豸���ô�С
IVS_UINT32 CDevEnum::GetSizeOfConfigType(IVS_UINT32 uiConfigType)
{
	IVS_UINT32 iSize = sizeof(chSizeOfConfigType)/sizeof(chSizeOfConfigType[0]);
	if (uiConfigType >= iSize)
	{
		return 0;
	}
	return chSizeOfConfigType[uiConfigType]; 
}

// ��ȡ���������
IVS_INT32 CDevEnum::GetCameraTypeEnum(const IVS_CHAR* pCameraType)
{
	CHECK_POINTER(pCameraType, IVS_OPERATE_MEMORY_ERROR);

	IVS_INT32 iLen = sizeof(chCameraType)/sizeof(chCameraType[0]);
	IVS_INT32 i = 0;
	for (i = 0; i < iLen; i++ )
	{
		if (0 == strcmp(chCameraType[i], pCameraType))
		{
			break;
		}
	}
	return i; 
}

// ��ȡ�ع�����
const IVS_CHAR* CDevEnum::GetExposurePriorityCH(IVS_INT32 iExposurePriority)
{	
	if (iExposurePriority)
	{
		return chExposurePriority[1];
	}

	return chExposurePriority[0];
}

// ��ȡͨ��ģʽ
const IVS_CHAR* CDevEnum::GetEnableModeStr(IVS_INT32 iEnableMode)
{
	IVS_INT32 iLen = sizeof(chEnableMode)/sizeof(chEnableMode[0]);
	for (IVS_INT32 i = 0; i != iLen; i++)
	{
		if (iEnableMode == i)
		{
			return chEnableMode[i];
		}
	}

	return chEnableMode[0];
}

// ��ȡ�ع�����
IVS_INT32 CDevEnum::GetExposurePriorityINT(const IVS_CHAR* pExposurePriority)
{
	CHECK_POINTER(pExposurePriority, IVS_OPERATE_MEMORY_ERROR);
	IVS_INT32 iLen = sizeof(chExposurePriority)/sizeof(chExposurePriority[0]);
	IVS_INT32 i = 0;
	for (i = 0; i != iLen; i++)
	{
		if (0 == strcmp(chExposurePriority[i], pExposurePriority))
		{
			return i;
		}
	}

	return i;
}

// ��ȡͨ��ģʽ
IVS_INT32 CDevEnum::GetEnableMode(const IVS_CHAR* pEnableMode)
{
	CHECK_POINTER(pEnableMode, IVS_OPERATE_MEMORY_ERROR);
	IVS_INT32 iLen = sizeof(chEnableMode)/sizeof(chEnableMode[0]);
	IVS_INT32 i = 0;
	for (i = 0; i != iLen; i++)
	{
		if (0 == strcmp(chEnableMode[i], pEnableMode))
		{
			return i;
		}
	}
	return i;
}
