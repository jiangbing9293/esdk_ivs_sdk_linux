/********************************************************************
	filename	: 	SDKConfig.h
	author		:	x00192614
	created		:	2012/11/24
	description	:	SDKConfig������
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/10 ��ʼ�汾
*********************************************************************/
#ifndef __SDKCONFIG_H__
#define __SDKCONFIG_H__

#include "hwsdk.h"
#include "ivs_xml.h"
class CSDKConfig
{
public:
    CSDKConfig(void);
    ~CSDKConfig(void);

    static CSDKConfig& instance();
    
    // ��־��·����ȡ���÷���
    std::string GetLogPath() { return m_LogPath; }; //lint !e1036 !e1536
    void SetLogPath(std::string strLogPath) { m_LogPath = strLogPath; }; //lint !e1036 !e1536
    /******************************************************************
     function   : LoadConfigFile
     description: ���������ļ�
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 LoadConfigFile();

    /******************************************************************
     function   : GetTimeOutValue
     description: ��ȡ������Ϣ��ʱʱ��
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetTimeOutValue()const { return m_uiTimeOut; }

    /******************************************************************
     function   : GetBackDataInterval
     description: ��ȡ�����û������ļ�ʱ����
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetBackUpInterval()const { return m_uiBackUpInterval; }
    
	/******************************************************************
     function   : GetIAVideoDelayValue
     description: ��ȡƽ̨���ܷ����켣����ʵ����ʱ
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetPlatformIAVideoDelayValue()const { return m_uiPlatformIAVideoDelayValue; }

    /******************************************************************
     function   : GetIAVideoDelayValue
     description: ��ȡǰ�����ܷ����켣����ʵ����ʱ
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetPUIAVideoDelayValue()const { return m_uiPUIAVideoDelayValue; }

	/******************************************************************
     function   : GetDefaultLanguageValue
     description: ��ȡĬ������
     output     : NA
     return     : std::string
    *******************************************************************/
    std::string GetDefaultLanguageValue()const { return m_strDefaultLanguageValue; }

    /******************************************************************
     function   : GetMediaBufferLimit
     description: ��ȡ�������������ްٷֱ�
     input      : IVS_UINT32 & uiLowerLimit ����
     input      : IVS_UINT32 & uiUpperLimit ����
     output     : NA
     return     : void
    *******************************************************************/
    void GetMediaBufferLimit(IVS_UINT32& uiLowerLimit, IVS_UINT32& uiUpperLimit) const
    {
        uiLowerLimit = m_uiLowerLimit;
        uiUpperLimit = m_uiUpperLimit;
    }

	/******************************************************************
     function   : GetFontFamilyValue
     description: ��ȡ����
     output     : NA
     return     : std::string
    *******************************************************************/
    std::string GetFontFamilyValue()const { return m_strFontFamily; }

	/******************************************************************
     function   : GetFontSizeValue
     description: ��ȡ�����С
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetFontSizeValue()const { return m_uiFontSize; }

	/******************************************************************
     function   : GetRtspLogLevel
     description: ��ȡrtsp��־����
     output     : NA
     return     : IVS_UINT32
    *******************************************************************/
    IVS_UINT32 GetRtspLogLevel()const { return m_uiRtspLogLevel; }

    IVS_INT32 GetUtcTimeSpan(std::string& strStartUtcTime, std::string& strEndUtcTime, const char *pFormat, IVS_INT32 iSecond);

    IVS_INT32 ModifyLogLevel(IVS_ULONG ulLogLevel);
    IVS_INT32 SetSDKDebugLogSwitch(CBB::CXml& oCXml, bool bSwitch);
private:
    /******************************************************************
     function   : InitMember
     description: ��ʼ��������
     input      : CBB::CXml & oCXml ������CBB::CBB::CXml�ļ�
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 InitMember(CBB::CXml& oCXml);

    /******************************************************************
     function   : SetTimeOutValue
     description: ����TimeOut
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetTimeOutValue(CBB::CXml& oCXml);

    /******************************************************************
     function   : SetTimeOutValue
     description: ����BackUpInterval
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetBackUpInterval(CBB::CXml& oCXml);

	/******************************************************************
     function   : SetPUIAVideoDelayValue
     description: ����ǰ�����ܷ����켣����ʵ����ʱ
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetPUIAVideoDelayValue(CBB::CXml& oCXml);

    /******************************************************************
     function   : SetPlatformIAVideoDelayValue
     description: ����ƽ̨���ܷ����켣����ʵ����ʱ
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetPlatformIAVideoDelayValue(CBB::CXml& oCXml);

	/******************************************************************
     function   : SetDefaultLanguageValue
     description: ����Ĭ������
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
	IVS_INT32 SetDefaultLanguageValue(CBB::CXml& oCXml);

    /******************************************************************
     function   : SetMediaBufferLimit
     description: ����ý�建����������
     input      : CBB::CXml & oCXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetMediaBufferLimit(CBB::CXml& oCXml);

	/******************************************************************
     function   : SetFontFamilyValue
     description: ��������
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
	IVS_INT32 SetFontFamilyValue(CBB::CXml& oCXml);
	
	/******************************************************************
     function   : SetFontSizeValue
     description: ���������С
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetFontSizeValue(CBB::CXml& oCXml);

	/******************************************************************
     function   : SetRtspLogLevel
     description: ����rtsp��־����
     input      : CBB::CXml& oCXml �����ļ�CBB::CBB::CXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 SetRtspLogLevel(CBB::CXml& oCXml);

private:
    IVS_UINT32 m_uiTimeOut;                  // ������Ϣ��ʱʱ�䣬��λ���룩
    IVS_UINT32 m_uiBackUpInterval;           // �û��ļ��ϴ�ʱ��������λ�����ӣ�
    IVS_UINT32 m_uiPlatformIAVideoDelayValue;// ƽ̨���ܷ����켣����ʵ����ʱ����λ�����룩
    IVS_UINT32 m_uiPUIAVideoDelayValue;      // ǰ�����ܷ����켣����ʵ����ʱ����λ�����룩
	IVS_UINT32 m_uiFontSize;                 // �����С
	std::string m_strFontFamily;             // ����
	std::string m_strDefaultLanguageValue;   // Ĭ������
    std::string m_LogPath;                   // ��־��·��
	bool bInit;

    IVS_UINT32 m_uiLowerLimit;              // ý�建��������
    IVS_UINT32 m_uiUpperLimit;              // ý�建��������
    IVS_UINT32 m_uiRtspLogLevel;            // rtsp��־����

};
#endif

