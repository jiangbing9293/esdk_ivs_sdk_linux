///////////////////////////////////////////////////////////////////////////////
/// @file  RtspPacket.cpp
/// @brief RTSP��װ�������ʵ��  
///
/// Copyright (c) 2010
/// Huawei Tech.Co.,Ltd
/// 
/// @author   Li GengQiang/l00124479
/// @date     2011/01/21
/// @version  IVS V100R002C01
///////////////////////////////////////////////////////////////////////////////

#include <time.h>
#include "RtspPacket.h"
#include "compat.h"
/**
* Description:    CRtspPacket(). Default constructor
*/
CRtspPacket::CRtspPacket()
{
    m_ulCSeq = 0;

    memset(&m_stMediaInfo, 0, sizeof(m_stMediaInfo));
}

/**
* Description:  GetCSeq().  ��ȡ�µ�CSeq
* @return       string.     �µ�CSeq
*/
string CRtspPacket::GetCSeq()
{
    const unsigned long ulCSeq = ++m_ulCSeq;
    char szCSeq[16] = {0};
    (void)IVS_snprintf(szCSeq, sizeof(szCSeq) - 1, "%lu", ulCSeq);

    return szCSeq;
}

/**
* Description:  SetMediaInfo(). ����ý����Ϣ
* @param  [in]  stMediaInfo ý����Ϣ
* @return       void.
*/
void CRtspPacket::SetMediaInfo
(
    IN  const RTSP::MEDIA_INFO& stMediaInfo
)
{
    m_stMediaInfo = stMediaInfo;
}

/**
* Description:  SetCommonField().   ���ù���ͷ��
* @param  [in]  strMethod   ���󷽷���
* @param  [io]  strPacket   ��װ����ַ���
* @return       void.
*/
void CRtspPacket::SetCommonField
(
    IN  const string&   strMethod,
    IO  string&         strPacket
)
{
    //Request-Line
    strPacket = strMethod;
    strPacket += " ";
    strPacket += m_stMediaInfo.szSessionUrl;
    strPacket += " RTSP/1.0\r\n";

    //CSeq
    strPacket += "CSeq: ";
    strPacket += GetCSeq();
    strPacket += RTSP::CRLF;

    //Session
	strPacket += "Session: ";
    strPacket += m_stMediaInfo.szSessionId;
    strPacket += RTSP::CRLF;

    //User-Agent
    strPacket += "User-Agent: ";
    strPacket += RTSP::USER_AGENT;
    strPacket += RTSP::CRLF;

    //Content-Length
    strPacket += "Content-Length:0\r\n";

	//Range
	strPacket += "Range:npt=0-\r\n";
}

/**
* Description:  SetRangeField().    ����Rangeͷ��
* @param  [in]  ulStartTime         ��ʼʱ��
* @param  [in]  bRelativeTime   �Ƿ�Ϊ���ʱ��
* @param  [io]  strRange        ���ú���ַ���
* @return       long.   ������
*/
long CRtspPacket::SetRangeField
(
    IN  unsigned long   ulStartTime,
    IN  BOOL            bRelativeTime,
    IO  string&         strRange
)const
{
    //��ʼʱ��ת��Ϊ�ַ�������󳤶Ȳ��ᳬ��32�ֽ�
    char szStartTime[32] = {0};
    
    if (bRelativeTime)
    {
        (void)IVS_snprintf(szStartTime, sizeof(szStartTime) - 1, "%u-", ulStartTime);

        //Rangeʹ��npt���ʱ��
        strRange += "Range:npt=";
    }
    else
    {
        time_t tStartTime = ulStartTime;
        
        //localtime����NULL������:  Before midnight, January 1, 1970.  After 03:14:07, January 19, 2038,
        struct tm _tm;
		memset(&_tm, 0, sizeof(tm));
		IVS_localtime(&_tm, &tStartTime);
        
        (void)IVS_snprintf(szStartTime, sizeof(szStartTime) - 1, 
                        "%04d%02d%02dT%02d%02d%02dZ-", 
                        _tm.tm_year + 1900, 
                        _tm.tm_mon + 1, 
                        _tm.tm_mday, 
                        _tm.tm_hour, 
                        _tm.tm_min, 
                        _tm.tm_sec);

        //Rangeʹ��clock����ʱ��
        strRange += "Range:clock=";
    }
    
    strRange += szStartTime;
    strRange += RTSP::CRLF;

    return RTSP::RET_CODE_OK; 
}

/**
* Description:  GetNatPacket(). ��ȡNAT������Ϣ��
* @param  [in]  stNatReqPara    NAT����Ĳ���
* @param  [out] strPacket       ��װ����ַ���
* @return       long.       ������
*/
long CRtspPacket::GetNatPacket
(
    IN  const RTSP::NAT_REQ_PARA&   stNatReqPara,
    OUT string&                     strPacket
)
{
    string strProtocalType;
    switch (stNatReqPara.ulProtocalType)
    {
    case RTSP::PROTOCAL_TYPE_RTP:
        {
            strProtocalType = "RTP";
            break;
        }

    case RTSP::PROTOCAL_TYPE_RTCP:
        {
            strProtocalType = "RTCP";
            break;
        }

    default:
        {
            return RTSP::RET_CODE_FAIL;
            //break;
        }
    }

    //���ض˿�ת��Ϊ�ַ���
    char szLocalPort[16] = {0};
    (void)IVS_snprintf(szLocalPort, sizeof(szLocalPort) - 1, "%u", stNatReqPara.usLocalPort);

    //���ù���ͷ��
    SetCommonField(RTSP::METHOD_PLAY, strPacket);

    //Range
    //strPacket += "Range:npt=0-\r\n";

    //x-NAT-Info
    strPacket += "x-NAT-Info:type=";
    strPacket += strProtocalType;
    strPacket += ";local_addr=";
    strPacket += m_stMediaInfo.szLocalIp;
    strPacket += ";local_port=";
    strPacket += szLocalPort;
    strPacket += RTSP::CRLF;

    //���Ͽ���
    strPacket += RTSP::CRLF;

    return RTSP::RET_CODE_OK;
}

/**
* Description:  GetVcrPacket(). ��ȡVCR������Ϣ��
* @param  [in]  stVcrReqPara    VCR����Ĳ���
* @param  [out] strPacket       ��װ����ַ���
* @return       long.   ������
*/
long CRtspPacket::GetVcrPacket
(
    IN  const RTSP::VCR_REQ_PARA&   stVcrReqPara,
    OUT string&                     strPacket
)
{
    long lResult = RTSP::RET_CODE_OK;
    
    //���󷽷���Ĭ��ΪPLAY
    string strMethod = RTSP::METHOD_PLAY;
    //Range��Ĭ��Ϊ��
    string strRange;
    //���ػ�ط��ٶȣ�Ĭ��Ϊ��
    string strSpeed;

    //�ٶ�ת��Ϊ�ַ���
    char szSpeed[16] = {0};
    (void)IVS_snprintf(szSpeed, sizeof(szSpeed) - 1, "%f", stVcrReqPara.fSpeed);

    switch (stVcrReqPara.ulVcrCode)
    {
    case RTSP::VCR_CODE_SPEED:
        {
            //Speed
            strSpeed = "Speed:";
            strSpeed += szSpeed;
            strSpeed += RTSP::CRLF;
            break;
        }

    case RTSP::VCR_CODE_SCALE:
        {
            //Scale
            strSpeed = "Scale:";
            strSpeed += szSpeed;
            strSpeed += RTSP::CRLF;
            break;
        }

    case RTSP::VCR_CODE_DROP:
        {
            lResult = SetRangeField(stVcrReqPara.ulStartTime, stVcrReqPara.bRelativeTime, strRange);
            break;
        }

    case RTSP::VCR_CODE_PAUSE:
        {
            //����ΪPAUSE
            strMethod = RTSP::METHOD_PAUSE;
            break;
        }

    case RTSP::VCR_CODE_CONTINUE:
        {
            //ֱ��ʹ��Ĭ��ֵ
            break;
        }

    default:
        {
            return RTSP::RET_CODE_FAIL;
            //break;
        }
    }

    //���ù���ͷ��
    SetCommonField(strMethod, strPacket);
    //Range
    strPacket += strRange;
    //Speed
    strPacket += strSpeed;
    //����
    strPacket += RTSP::CRLF;

    return lResult;
}

/**
* Description:  GetKeepAlivePacket(). ��ȡ����������Ϣ��
* @param  [out] strPacket       ��װ����ַ���
* @return       long.       ������
*/
void CRtspPacket::GetKeepAlivePacket
(
    OUT string& strPacket
)
{
    //���ù���ͷ��
    SetCommonField(RTSP::METHOD_OPTIONS, strPacket);

    //���Ͽ���
    strPacket += RTSP::CRLF;
}

/**
* Description:  DecodePacket().     ����RTSP��
* @param  [in]  strPacket   RTSP��
* @param  [out] stResult    �������
* @return       long.   ������
*/
long CRtspPacket::DecodePacket
(
    IN  const string&           strPacket,
    OUT RTSP::DECODE_RESULT&    stResult
)const
{
    //long lResult = RTSP::RET_CODE_OK;

    string strFirstHeader;
    string strField;

    //��ȡ�������ֶκ�ͷ������
    long lResult = GetFirstLine(strPacket, strFirstHeader, strField);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return lResult;
    }

    //��Ӧ��Ϣֱ�ӷ���
    if (0 == strFirstHeader.compare(RTSP::RTSP_VERSION))
    {
        stResult.ulMsgType = RTSP::MSG_TYPE_RESP;

        //��ȡRTP-Info��ֻ��NAT��Խ��Ӧ��Ϣ����
        (void)GetRtpInfo(strField, stResult);
    
        return RTSP::RET_CODE_OK;
    }
    //ֻ����SET_PARAMETER��Ϣ
    else if (0 != strFirstHeader.compare("SET_PARAMETER"))
    {
        return RTSP::RET_CODE_FAIL;
    }

    stResult.ulMsgType = RTSP::MSG_TYPE_SET_PARAMETER;

    //��ͷ�����ݵĶ���תΪ����
    MuiltLineToOneLine(strField);

    //��ȡCSeq
    lResult = GetFieldValue(strField, "CSeq", stResult.strCSeq);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡ������Ϣ����
    lResult = GetPlayInfoType(strField, stResult.ulPlayInfoType);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    return RTSP::RET_CODE_OK;
}

/**
* Description:  GetRspPacket().     ��ȡ��Ӧ��Ϣ��
* @param  [in]  strCSeq         �����CSeq
* @param  [out] strPacket   ��װ����ַ���
* @return       void.
*/
void CRtspPacket::GetRspPacket
(
    IN  const string&   strCSeq,
    OUT string&         strPacket
)const
{
    //��Ӧͷ
    strPacket = "RTSP/1.0 200 OK\r\n";

    //CSeq
    strPacket += "CSeq: ";
    strPacket += strCSeq;
    strPacket += RTSP::CRLF;

    //Session
    strPacket += "Session: ";
    strPacket += m_stMediaInfo.szSessionId;
    strPacket += RTSP::CRLF;

    //����
    strPacket += RTSP::CRLF;
}

/**
* Description:  GetFirstLine(). ��ȡ�������ֶκ�����ͷ���ַ���
* @param  [in]  strPacket       ��Ϣ��
* @param  [out] strFirstHeader  �������ֶ�
* @param  [out] strField        ͷ��
* @return       long.   ������
*/
long CRtspPacket::GetFirstLine
(
    IN  const string&   strPacket,
    OUT string&         strFirstHeader,
    OUT string&         strField
)const
{
    size_t nPosLineStart = 0;
    size_t nPosLineEnd = strPacket.find(RTSP::CRLF, nPosLineStart);
    if (string::npos == nPosLineEnd)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //����
    string strFirstLine = strPacket.substr(nPosLineStart, (nPosLineEnd - nPosLineStart));   //lint !e845
    
    size_t nPosStart = 0;
    size_t nPosEnd = strFirstLine.find(RTSP::SPACE, nPosStart);
    if (string::npos == nPosEnd)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //�������ֶ�
    strFirstHeader = strFirstLine.substr(nPosStart, (nPosEnd - nPosStart));   //lint !e845
    //ͷ������
    strField = strPacket.substr(nPosLineEnd + RTSP::CRLF_LEN);

    return RTSP::RET_CODE_OK;
}

/**
* Description:  GetFieldValue().    ��ȡָ��ͷ�������
* @param  [in]  strField    ����ͷ���ַ���
* @param  [in]  strKey      ͷ��Keyֵ
* @param  [out] strContent  ͷ������
* @param  [in]  strEnd          ������
* @param  [in]  strInterval     �ָ���
* @return       long.   ������
*/
long CRtspPacket::GetFieldValue
(
    IN  const string&   strField,
    IN  const string&   strKey,
    OUT string&         strContent,
    IN  const string&   strEnd,
    IN  const string&   strInterval
)
{
    //�ȼ�һ��������
    std::string strFieldTmp = strField + strEnd;//lint !e1036
    
    string strLine;
    string strValue;
    size_t nPosLineStart = 0;
    size_t nPosLineEnd = strFieldTmp.find(strEnd, nPosLineStart);

    long lResult = RTSP::RET_CODE_FAIL;
    
    while(string::npos != nPosLineEnd)
    {
        strLine = strFieldTmp.substr(nPosLineStart, nPosLineEnd - nPosLineStart);
    
        size_t nPosStart = 0;
        size_t nPosEnd = strLine.find(strInterval, nPosStart);

        //�ҵ�ð�ţ�˵����ĳͷ�򣬽�һ������ͷ���ֵ
        if (string::npos != nPosEnd)
        {
            strValue = strLine.substr(nPosStart, (nPosEnd - nPosStart));   //lint !e845

            //ȥ���ַ�����β�Ŀո�Tab
            TrimString(strValue);

            //�����ִ�Сд�ıȽϣ��ҵ���ֱ���˳�
            if (0 == IVS_strncasecmp(strKey.c_str(), strValue.c_str(), strKey.length()))
            {
                //��ȡ����
                strContent = strLine.substr(nPosEnd + 1, nPosLineEnd - nPosEnd);
                //ȥ���ַ�����β�Ŀո�Tab
                TrimString(strContent);
                
                lResult = RTSP::RET_CODE_OK;
                break;
            }
        }        

        //��һ����
        nPosLineStart = nPosLineEnd + strEnd.size();
        nPosLineEnd = strFieldTmp.find(strEnd, nPosLineStart);
    }
    
    return lResult;
}

/**
* Description:  GetPlayInfoType().  ��ȡ������Ϣ����
* @param  [in]  strField        ����ͷ���ַ���
* @param  [out] ulPlayInfoType  ������Ϣ����
* @return       long.   ������
*/
long CRtspPacket::GetPlayInfoType
(
    IN  const string&   strField,
    OUT unsigned long&  ulPlayInfoType
)const
{
    //��ȡX-PLAYINFOͷ��
    string strPlayInfo;
    long lResult = GetFieldValue(strField, "X-PLAYINFO", strPlayInfo);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡ�׸�����
    size_t nPosStart = strPlayInfo.find(RTSP::DOUBLE_QUOTE, 0);
    if (string::npos == nPosStart)
    {
        return RTSP::RET_CODE_FAIL;
    }
    nPosStart += 1;

    size_t nPosEnd = strPlayInfo.find(RTSP::DOUBLE_QUOTE, nPosStart);
    if (string::npos == nPosEnd)
    {
        return RTSP::RET_CODE_FAIL;
    }

    string strValue = strPlayInfo.substr(nPosStart, nPosEnd - nPosStart);

    //ת���׸�����Ϊ������Ϣ����
    if (0 == strValue.compare("EOS"))
    {
        ulPlayInfoType = RTSP::PLAY_INFO_TYPE_EOS;
    }
    else if (0 == strValue.compare("BOS"))
    {
        ulPlayInfoType = RTSP::PLAY_INFO_TYPE_BOS;
    }
    else
    {
        nPosStart = 0;
        nPosEnd = strValue.find(RTSP::SPACE, nPosStart);
        if (string::npos == nPosEnd)
        {
            return RTSP::RET_CODE_FAIL;
        }

        strValue = strValue.substr(nPosStart, (nPosEnd - nPosStart));   //lint !e845

        //�Ƿ���Ϣ
        if (0 != strValue.compare("CLOSE"))
        {
            return RTSP::RET_CODE_FAIL;
        }

        ulPlayInfoType = RTSP::PLAY_INFO_TYPE_CLOSE;
    }

    return RTSP::RET_CODE_OK;
}

/**
* Description:  GetRtpInfo().  ��ȡRTP-Info
* @param  [in]  strField        ����ͷ���ַ���
* @param  [out] stResult    �������
* @return       long.   ������
*/
long CRtspPacket::GetRtpInfo
(
    IN  const string&           strField,
    OUT RTSP::DECODE_RESULT&    stResult
)const
{
    string strRtpInfo;
    //��ȡRTP-Infoͷ��
    long lResult = GetFieldValue(strField, "RTP-Info", strRtpInfo);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡRTP-Infoͷ���е�һ��seq��rtptime
    string strRtpSeq;
    string strRtpTime;
    
    lResult = GetFieldValue(strRtpInfo, "seq", strRtpSeq, RTSP::SEMI_COLON, RTSP::EQUAL_SIGN);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    lResult = GetFieldValue(strRtpInfo, "rtptime", strRtpTime, RTSP::SEMI_COLON, RTSP::EQUAL_SIGN);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    stResult.bHasRtpInfo = 1;
    stResult.ulRtpSeq    = strtoul(strRtpSeq.c_str(), NULL, 0);
    stResult.ulRtpTime   = strtoul(strRtpTime.c_str(), NULL, 0);

    return RTSP::RET_CODE_OK;
}

/**
* Description:  TrimString().   ȥ���ַ�����β�Ŀո�Tab
* @param  [io]  str     ��������ַ���
* @return       void.
*/
void CRtspPacket::TrimString
(
    IO  string& str
)
{
    //���ַ���ֱ�ӷ���
    if (str.empty())
    {
        return;
    }
    
    //ȥ��β���Ŀո��Tab
    for (size_t nPosLast = str.length() - 1; 0 < nPosLast; nPosLast = str.length() - 1)
    {
        const char cLast = str.at(nPosLast);

        //�ǿո��Tab��ֱ���˳�ѭ��
        if (RTSP::SPACE != cLast && RTSP::TAB != cLast)
        {
            break;
        }

        //ȥ���ո��Tab
        (void)str.erase(nPosLast, 1);
    }

    //ȥ��ͷ���Ŀո��Tab
    for (size_t nPosFirst = 0; nPosFirst < str.length(); )
    {
        const char cFirst = str.at(nPosFirst);

        //�ǿո��Tab��ֱ���˳�ѭ��
        if (RTSP::SPACE != cFirst && RTSP::TAB != cFirst)
        {
            break;
        }

        //ȥ���ո��Tab
        (void)str.erase(nPosFirst, 1);
    }
}

/**
* Description:  MuiltLineToOneLine().   ��ͷ�����ݵĶ���תΪ����
* @param  [io]  strFieldContent     �������ͷ�������ַ���
* @return       void.
*/
void CRtspPacket::MuiltLineToOneLine
(
    IO  string& strFieldContent
)
{
    size_t nPosStart = 0;
    size_t nPosEnd = strFieldContent.find(RTSP::CRLF, nPosStart);

    //�����滻
    while (string::npos != nPosEnd)
    {
        //����LWS�ĳ���
        size_t nLwsCount = 0;
        for (size_t nPosNext = nPosEnd + RTSP::CRLF_LEN; strFieldContent.length() - 1 > nPosNext; ++nPosNext)
        {
            if (RTSP::SPACE != strFieldContent.at(nPosNext) && RTSP::TAB != strFieldContent.at(nPosNext))
            {
                break;
            }

            nLwsCount++;
        }

        //���У��滻Ϊ����
        if (0 < nLwsCount)
        {
            //�滻Ϊ�ո�
            (void)strFieldContent.replace(nPosEnd, nLwsCount + RTSP::CRLF_LEN, " ");
            //�滻���ֵ
            nPosStart = nPosEnd + 1;
        }
        else
        {
            //δ�滻��ֵ
            nPosStart = nPosEnd + RTSP::CRLF_LEN;
        }
        
        //������һ��CRLF
        nPosEnd = strFieldContent.find(RTSP::CRLF, nPosStart);
    }
}

/**
* Description:  GetNatPacket(). ����NAT��Խ��
* @param  [in]  strPacket      RTSP��NAT��
* @param  [out] stNatResult    NAT�������
* @return       long.   ������
*/
long CRtspPacket::DecodeNatPacket
(
    IN  const string&           strPacket,
    OUT RTSP::NAT_DECODE_RESULT&    stNatResult
) const
{
    //long lResult = RTSP::RET_CODE_OK;

    string strFirstHeader;
    string strField;

    //��ȡ�������ֶκ�ͷ������
    long lResult = GetFirstLine(strPacket, strFirstHeader, strField);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return lResult;
    }

    //ֻ����NAT��Խ��
    if (0 != strFirstHeader.compare(RTSP::METHOD_RECORD))
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ͷ�����ݵĶ���תΪ����
    MuiltLineToOneLine(strField);

    //��ȡCSeq
    lResult = GetFieldValue(strField, "CSeq", stNatResult.strCSeq);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡSession
    lResult = GetFieldValue(strField, "Session", stNatResult.strSession);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }
    //��ȡx-NAT-Info
    string strNatInfo;
    lResult = GetFieldValue(strField, "x-NAT-Info", strNatInfo);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }
    //��ȡЭ������
    lResult = GetFieldValue(strNatInfo, "type", stNatResult.strProtocalType, RTSP::SEMI_COLON, RTSP::EQUAL_SIGN);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡIP
    lResult = GetFieldValue(strNatInfo, "local_addr", stNatResult.strRemoteIp, RTSP::SEMI_COLON, RTSP::EQUAL_SIGN);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    //��ȡ�˿ں�
    lResult = GetFieldValue(strNatInfo, "local_port", stNatResult.strRemotePort, RTSP::SEMI_COLON, RTSP::EQUAL_SIGN);
    if (RTSP::RET_CODE_OK != lResult)
    {
        return RTSP::RET_CODE_FAIL;
    }

    return RTSP::RET_CODE_OK;
}

/**
* Description:  GetNATRspPacket().     ��ȡNAT��Ӧ��Ϣ��
* @param  [in]  stNatDecodeResult      ����NAT��Խ���Ľ��
* @param  [out] strPacket   ��װ����ַ���
* @return       void.
*/
void CRtspPacket::GetNATRspPacket
(
    IN  const RTSP::NAT_DECODE_RESULT&   stNatDecodeResult,
    OUT string&         strPacket
)const
{
    //��Ӧͷ
    strPacket = "RTSP/1.0 200 OK\r\n";

    //CSeq
    strPacket += "CSeq: ";
    strPacket += stNatDecodeResult.strCSeq;
    strPacket += RTSP::CRLF;

    //Session
    strPacket += "Session: ";
    strPacket += stNatDecodeResult.strSession;
    strPacket += RTSP::CRLF;

    //x-NAT-Info
    strPacket += "x-NAT-Info:";
    strPacket += RTSP::CRLF;

    //type
    strPacket += "type=";
    strPacket += stNatDecodeResult.strProtocalType;
    strPacket += ";";
    //local_addr
    strPacket += "local_addr=";
    strPacket += m_stMediaInfo.szLocalIp;
    strPacket += ";";
    //local_port
    char strTmpPort[6] = {0};
    //UIntToChar(m_stMediaInfo.usLocalPort,strTmpPort);
    strPacket += "local_port=";
#ifdef WIN32
	char* pszLocalPort =  itoa(m_stMediaInfo.usLocalPort, strTmpPort, 10); //lint !e734 !e1055
	if (NULL != pszLocalPort)
	{
		strPacket += pszLocalPort;
	}
#else 
	sprintf(strTmpPort,"%d",m_stMediaInfo.usLocalPort);
	strPacket += strTmpPort;
#endif


    strPacket += ";";
    //src_addr
    strPacket += "src_addr=";
    strPacket += stNatDecodeResult.strRemoteIp;
    strPacket += ";";
    //src_port
    strPacket += "src_port=";
    strPacket += stNatDecodeResult.strRemotePort;
    strPacket += ";";
    
    //����
    strPacket += RTSP::CRLF;
}
