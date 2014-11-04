/********************************************************************
	filename	: 	SDKConfig.cpp
	author		:	x00192614
	created		:	2012/11/24
	description	:	SDKConfig������
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/10 ��ʼ�汾
*********************************************************************/
#include "bp_os.h"
#include "SDKConfig.h"
#include "hwsdk.h"
#include "ivs_error.h"
#include "ivs_xml.h"
#include "ToolsHelp.h"

#ifdef WIN32
static const std::string DEFAULT_CONFIG_FILE_NAME = "\\SDKConfig.xml";
static const std::string DEFAULT_LOG_XML_NAME = "\\log.xml";
#else
static const std::string DEFAULT_CONFIG_FILE_NAME = "/SDKConfig.xml";
static const std::string DEFAULT_LOG_XML_NAME = "/log.xml";
#endif

#define SDK_DEBUG_LOG 7

CSDKConfig::CSDKConfig(void)
    : m_uiTimeOut(35)
    , m_uiBackUpInterval(30)
	, m_uiPUIAVideoDelayValue(1000)
    , m_uiPlatformIAVideoDelayValue(200)
    , m_uiLowerLimit(10)
    , m_uiUpperLimit(80)
	, m_uiFontSize(8)
    , m_uiRtspLogLevel(7)      // 7��ʾRTSP_LOG_LEVEL_DEBUG debug����
	, bInit(false)
{
}

CSDKConfig::~CSDKConfig(void)
{
}

// ��ȡ���õ�ʵ������
CSDKConfig& CSDKConfig::instance()
{ 
    static CSDKConfig _instance; 
    return _instance; 
} 

// ���������ļ�,����BP������ȡconfigĿ¼
IVS_INT32 CSDKConfig::LoadConfigFile()
{
	if (bInit)
	{
		return IVS_SUCCEED;
	}
	bInit = true;
    // ��ȡconfigĿ¼
    std::string strConfigDir;
    int iRet = CToolsHelp::get_config_directory(strConfigDir);
    if (IVS_SUCCEED != iRet)
    {		
        BP_RUN_LOG_ERR(IVS_FAIL, "get conf dir failed.","dir=null");
        return IVS_FAIL;
    }
    std::string strConfigFilePath = strConfigDir;
    strConfigFilePath += DEFAULT_CONFIG_FILE_NAME;

    // ��ȡsdk�����ļ�
    CBB::CXml configXml;
    bool bLoadRet = configXml.Load(strConfigFilePath.c_str(), TIXML_ENCODING_LEGACY);
    if (!bLoadRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Load Config File.","$filepath=%s", strConfigFilePath.c_str());
        return IVS_FAIL;
    }
    iRet = InitMember(configXml);
    if (IVS_SUCCEED != iRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Init Member.","$filepath=%s", strConfigFilePath.c_str());
        return IVS_FAIL;
    }
    return IVS_SUCCEED;
}

// ������������,��ʱֻ��TimeOut,�����в����ڴ˷������
IVS_INT32 CSDKConfig::InitMember(CBB::CXml& oCXml)
{
    int iRet = IVS_SUCCEED;
    // ��ʼ��TimeOut������ֵ
    iRet = SetTimeOutValue(oCXml);

    // ��ʼ��BackUpInterval������ֵ
    iRet += SetBackUpInterval(oCXml);

	// ��ʼ��PUIAVideoDelayValue������ֵ
	iRet += SetPUIAVideoDelayValue(oCXml);

    // ��ʼ��PlatformIAVideoDelayValue������ֵ
    iRet += SetPlatformIAVideoDelayValue(oCXml);

	// ��ʼ��Ĭ������������ֵ
	iRet += SetDefaultLanguageValue(oCXml);

    // ����ý�建���������޵İٷֱ�
    iRet += SetMediaBufferLimit(oCXml);

	// ��ʼ������������ֵ
	iRet += SetFontFamilyValue(oCXml);

	// ��ʼ�������С������ֵ
	iRet += SetFontSizeValue(oCXml);

    // ��ʼ��rtsp��־����������ֵ
    iRet += SetRtspLogLevel(oCXml);
    return iRet;
}

IVS_INT32 CSDKConfig::SetTimeOutValue(CBB::CXml& oCXml)
{
    int iRet = IVS_FAIL;
    // ��ʼ��TimeOut������ֵ
    const char* pTmpVal = NULL;
    if(oCXml.FindElemEx("Config/SDKConfig/TimeOut"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiTimeOut = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }
    return iRet;
}

IVS_INT32 CSDKConfig::SetBackUpInterval(CBB::CXml& oCXml)
{
    int iRet = IVS_FAIL;
    // ��ʼ��TimeOut������ֵ
    const char* pTmpVal = NULL;
    if(oCXml.FindElemEx("Config/SDKConfig/BackUpInterval"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiBackUpInterval = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }
    return iRet;
}

IVS_INT32 CSDKConfig::SetPUIAVideoDelayValue(CBB::CXml& oCXml)
{
	int iRet = IVS_FAIL;
	// ��ʼ��TimeOut������ֵ
	const char* pTmpVal = NULL;
	if(oCXml.FindElemEx("Config/OCXConfig/PUIAVideoDelayValue"))
	{
		pTmpVal = oCXml.GetElemValue();
		if (NULL != pTmpVal)
		{
			iRet = IVS_SUCCEED;
			m_uiPUIAVideoDelayValue = static_cast<IVS_UINT32>(atoi(pTmpVal));
		}
	}
	return iRet;

}

IVS_INT32 CSDKConfig::SetPlatformIAVideoDelayValue(CBB::CXml& oCXml)
{
    int iRet = IVS_FAIL;
    // ��ʼ��TimeOut������ֵ
    const char* pTmpVal = NULL;
    if(oCXml.FindElemEx("Config/OCXConfig/PlatformIAVideoDelayValue"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiPlatformIAVideoDelayValue = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }
    return iRet;

}

IVS_INT32 CSDKConfig::SetDefaultLanguageValue(CBB::CXml& oCXml)
{
	int iRet = IVS_FAIL;
	// ��ʼ��DefaultLanguage������ֵ
	const char* pTmpVal = NULL;
	if(oCXml.FindElemEx("Config/OCXConfig/DefaultLanguage"))
	{
		pTmpVal = oCXml.GetElemValue();
		if (NULL != pTmpVal)
		{
			iRet = IVS_SUCCEED;
			m_strDefaultLanguageValue = pTmpVal;
		}
	}
	return iRet;

}

IVS_INT32 CSDKConfig::SetMediaBufferLimit(CBB::CXml& oCXml)
{
    int iRet = IVS_FAIL;

    const char* pTmpVal = NULL;
    if(oCXml.FindElemEx("Config/SDKConfig/LowerLimit"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiLowerLimit = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }

    if(oCXml.FindElemEx("Config/SDKConfig/UpperLimit"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiUpperLimit = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }
    return iRet;
}

IVS_INT32 CSDKConfig::SetFontFamilyValue(CBB::CXml& oCXml)
{
	int iRet = IVS_FAIL;
	// ��ʼ��DefaultLanguage������ֵ
	const char* pTmpVal = NULL;
	if(oCXml.FindElemEx("Config/OCXConfig/FontFamily"))
	{
		pTmpVal = oCXml.GetElemValue();
		if (NULL != pTmpVal)
		{
			iRet = IVS_SUCCEED;

			// ת�����룬��ֹ����
		//	char* xml2ANSI = CToolsHelp::UTF8ToANSI(pTmpVal);
			m_strFontFamily = pTmpVal;
			//free(xml2ANSI);
		}
	}
	return iRet;
}

IVS_INT32 CSDKConfig::SetFontSizeValue(CBB::CXml& oCXml)
{
	int iRet = IVS_FAIL;
	// ��ʼ��TimeOut������ֵ
	const char* pTmpVal = NULL;
	if(oCXml.FindElemEx("Config/OCXConfig/FontSize"))
	{
		pTmpVal = oCXml.GetElemValue();
		if (NULL != pTmpVal)
		{
			iRet = IVS_SUCCEED;
			m_uiFontSize = static_cast<IVS_UINT32>(atoi(pTmpVal));
		}
	}
	return iRet;

}

IVS_INT32 CSDKConfig::SetRtspLogLevel(CBB::CXml& oCXml)
{
    int iRet = IVS_FAIL;
    // ��ʼ��TimeOut������ֵ
    const char* pTmpVal = NULL;
    if(oCXml.FindElemEx("Config/MPConfig/RTSPLogLevel"))
    {
        pTmpVal = oCXml.GetElemValue();
        if (NULL != pTmpVal)
        {
            iRet = IVS_SUCCEED;
            m_uiRtspLogLevel = static_cast<IVS_UINT32>(atoi(pTmpVal));
        }
    }
    return iRet;

}

IVS_INT32 CSDKConfig::GetUtcTimeSpan(std::string& strStartUtcTime, std::string& strEndUtcTime, const char *pFormat, IVS_INT32 iSecond)
{
    char strTime[IVS_TIME_LEN] = {0};

    struct tm stTmv;
    time_t t = time(NULL);

    (void)BP_OS::gmtime_r(&t, &stTmv);

    std::string strFmt("");
    if (0 == strcmp("YYYYMMDDhhmmss", pFormat))
    {
        strFmt = "%04d%02d%02d%02d%02d%02d";
    }
    else if (0 == strcmp("YYYYMMDDThhmmssZ", pFormat))
    {
        strFmt = "%04d%02d%02dT%02d%02d%02dZ";
    }
    else
    {
        strFmt = "%04d-%02d-%02d %02d:%02d:%02d";
    }

	stTmv.tm_sec -= 300;
	(void)BP_OS::mktime(&stTmv);

    (void)ACE_OS::snprintf(strTime,
        (size_t)IVS_TIME_LEN,
        strFmt.c_str(),
        stTmv.tm_year + 1900,
        stTmv.tm_mon + 1,
        stTmv.tm_mday,
        stTmv.tm_hour,
        stTmv.tm_min,
        stTmv.tm_sec);
    strStartUtcTime = strTime;

    stTmv.tm_sec += iSecond;

    (void)BP_OS::mktime(&stTmv);

    (void)ACE_OS::snprintf(strTime,
        (size_t)IVS_TIME_LEN,
        strFmt.c_str(),
        stTmv.tm_year + 1900,
        stTmv.tm_mon + 1,
        stTmv.tm_mday,
        stTmv.tm_hour,
        stTmv.tm_min,
        stTmv.tm_sec);
    strEndUtcTime = strTime;
    return IVS_SUCCEED;
}

IVS_INT32 CSDKConfig::ModifyLogLevel(IVS_ULONG ulLogLevel)
{
    // ��ȡconfigĿ¼
    std::string strConfigDir;
    int iRet = CToolsHelp::get_config_directory(strConfigDir);
    if (IVS_SUCCEED != iRet)
    {		
        BP_RUN_LOG_ERR(IVS_FAIL, "get conf dir failed.","dir=null");
        return IVS_FAIL;
    }
    std::string strConfigFilePath = strConfigDir;
    strConfigFilePath += DEFAULT_LOG_XML_NAME;

    // ��ȡsdk��־�����ļ�
    CBB::CXml configXml;
    bool bLoadRet = configXml.Load(strConfigFilePath.c_str(), TIXML_ENCODING_LEGACY);
    if (!bLoadRet)
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "Load Config File.","$filepath=%s", strConfigFilePath.c_str());
        return IVS_FAIL;
    }

    //�����������־�����޸�log.xml��DebugLogSwitch��ֵ
    if (SDK_DEBUG_LOG == ulLogLevel)
    {
        iRet = SetSDKDebugLogSwitch(configXml, true);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(IVS_FAIL, "Set SDK DebugLogSwitch[true] Fail.","");
            return IVS_FAIL;
        }
    }
    else
    {
        iRet = SetSDKDebugLogSwitch(configXml, false);
        if (IVS_SUCCEED != iRet)
        {
            BP_RUN_LOG_ERR(IVS_FAIL, "Set SDK DebugLogSwitch[false] Fail.","");
            return IVS_FAIL;
        }
    }

    if (!configXml.Save(strConfigFilePath.c_str()))
    {
        BP_RUN_LOG_ERR(IVS_WRITE_FILE_ERROR, "Set SDK DebugLogSwitch", "save to log.xml fail.");
        return IVS_WRITE_FILE_ERROR;
    }

    return IVS_SUCCEED;
}

IVS_INT32 CSDKConfig::SetSDKDebugLogSwitch(CBB::CXml& oCXml, bool bSwitch)
{
    int iRet = IVS_FAIL;

    //����DebugLogSwitch���޸�
    if(oCXml.FindElemEx("Log/DebugLog/DebugLogSwitch"))
    {
        if (bSwitch)
        {
            oCXml.ModifyElemValue("True");
            return IVS_SUCCEED;
        }
        else
        {
            oCXml.ModifyElemValue("False");
            return IVS_SUCCEED;
        }
    }
    return iRet;
}