///////////////////////////////////////////////////////////////////////////////
/// @file  RtspPacket.h
/// @brief RTSP��װ�����ඨ��  
///
/// Copyright (c) 2010
/// Huawei Tech.Co.,Ltd
/// 
/// @author   Li GengQiang/l00124479
/// @date     2011/01/21
/// @version  IVS V100R002C01
///////////////////////////////////////////////////////////////////////////////

#ifndef _RTSP_PACKET_H_HEADER_INCLUDED_
#define _RTSP_PACKET_H_HEADER_INCLUDED_

#pragma warning(disable : 4786 4503 4996)

#include <string>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
using namespace std;

#ifndef snprintf
#define snprintf _snprintf
#endif

//����������/�������
#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef IO
#define IO
#endif

typedef int BOOL;

namespace RTSP
{
    const unsigned long MAX_LEN_SESSION_URL     = 256;  //ý��ỰURL����󳤶�
    const unsigned long MAX_LEN_SESSION_ID      = 32;   //ý��ỰID����󳤶�
    const unsigned long MAX_LEN_IP              = 16;   //IP����󳤶�

    const char USER_AGENT[]     = "HUAWEI CU/V100R002C02";  //User-Agent�ֶ�
    const char RTSP_VERSION[]   = "RTSP/1.0";   //RTSP����汾��
    const char METHOD_RECORD[]  = "RECORD";     //PLAY�����ַ���

    const char TAB          = '\t';
    const char SPACE        = ' ';
    const char DOUBLE_QUOTE = '"';

    const string SEMI_COLON = ";";
    const string COLON      = ":";
    const string EQUAL_SIGN = "=";
    const string CRLF       = "\r\n";
    const unsigned long CRLF_LEN = 2;   //CRLF�ĳ���

    const string METHOD_PLAY    = "PLAY";       //PLAY�����ַ���
    const string METHOD_OPTIONS = "OPTIONS";    //OPTINS�����ַ���
    const string METHOD_PAUSE   = "PAUSE";      //PAUSE�����ַ���

    //������
    enum RET_CODE
    {
        RET_CODE_OK,    ///< �ɹ�
        RET_CODE_FAIL   ///< ʧ��
    };

    //Э������
    enum PROTOCAL_TYPE
    {
        PROTOCAL_TYPE_RTP,  ///< RTP
        PROTOCAL_TYPE_RTCP  ///< RTCP
    };

    //VCR������
    enum VCR_CODE
    {
        VCR_CODE_SPEED,     ///< �ı�¼�������ٶ�
        VCR_CODE_SCALE,     ///< �ı�¼��ط��ٶ�
        VCR_CODE_DROP,      ///< �϶�
        VCR_CODE_PAUSE,     ///< ��ͣ
        VCR_CODE_CONTINUE  ///< ����
    };

    //��Ϣ����
    enum MSG_TYPE
    {
        MSG_TYPE_RESP,          ///< ��Ӧ��Ϣ
        MSG_TYPE_SET_PARAMETER  ///< ���ò�����Ϣ
    };

    enum PLAY_INFO_TYPE
    {
        PLAY_INFO_TYPE_EOS,     ///<    EOS
        PLAY_INFO_TYPE_BOS,     ///<    BOS
        PLAY_INFO_TYPE_CLOSE   ///<    �������������
    };

    //ý����Ϣ
    struct MEDIA_INFO
    {
        char            szSessionUrl[MAX_LEN_SESSION_URL + 1];      ///< ý��ỰURL
        char            szSessionId[MAX_LEN_SESSION_ID + 1];        ///< ý��ỰID
        char            szLocalIp[MAX_LEN_IP + 1];                  ///< ����IP
        unsigned short  usLocalPort;                                ///< ���ض˿ں�
    };

    //NAT�������
    struct NAT_REQ_PARA
    {
        unsigned long   ulProtocalType; ///< Э�����ͣ�����μ�@ref RTSP::PROTOCAL_TYPE
        unsigned short  usLocalPort;    ///< ���ض˿�
    };

    //VCR�������
    struct VCR_REQ_PARA
    {
        unsigned long   ulVcrCode;      ///< VCR�����룬����μ�@ref RTSP::VCR_CODE
        unsigned long   ulStartTime;    ///< ��ʼʱ�䣬��λΪ�룬@ref RTSP::VCR_CODE_DROPʱ��Ч
        float           fSpeed;         ///< ���ػ�ط��ٶȣ�@ref RTSP::VCR_CODE_SPEED��@ref RTSP::VCR_CODE_SCALEʱ��Ч
        BOOL            bRelativeTime;  ///< �Ƿ�Ϊ���ʱ�䣬@ref RTSP::VCR_CODE_DROPʱ��Ч
    };

    struct NAT_DECODE_RESULT
    {
        string          strProtocalType;///< Э�����ͣ�����μ�@ref RTSP::PROTOCAL_TYPE
        string          strRemoteIp;    ///< Զ��IP
        string          strRemotePort;  ///< Զ�̶˿�
        string          strCSeq;        ///< CSeq������Ӧ��Ϣ��ԭֵ����
        string          strSession;     ///< Session,����Ӧ��Ϣ��ԭֵ����
    };

    //������
    struct DECODE_RESULT
    {
        unsigned long   ulMsgType;      ///< ��Ϣ���ͣ�����μ�@ref RTSP::MSG_TYPE
        unsigned long   ulPlayInfoType; ///< ������Ϣ����
        BOOL            bHasRtpInfo;    ///< �Ƿ���RTP-Info��Ϣ
        unsigned long   ulRtpSeq;       ///< RTP�������кţ�bHasRtpInfoΪTRUEʱ��Ч
        unsigned long   ulRtpTime;      ///< RTP����ʱ�����bHasRtpInfoΪTRUEʱ��Ч
        string          strCSeq;        ///< CSeq������Ӧ��Ϣ��ԭֵ����

        //���캯����ʼ��
        DECODE_RESULT()
        {
            ulMsgType       = MSG_TYPE_RESP;
            ulPlayInfoType  = PLAY_INFO_TYPE_EOS;
            bHasRtpInfo     = 0;
            ulRtpSeq        = 0;
            ulRtpTime       = 0;
        }
    };
}

///////////////////////////////////////////////////////////////////////////////
/// @brief RTSP�ķ�װ������
/// 
/// Description:  
///////////////////////////////////////////////////////////////////////////////
class CRtspPacket
{
public:
    /**
    * Description:    CRtspPacket(). Default constructor
    */
    CRtspPacket();

    /**
    * Description:  SetMediaInfo(). ����ý����Ϣ
    * @param  [in]  stMediaInfo ý����Ϣ
    * @return       void.
    */
    void SetMediaInfo
    (
        IN  const RTSP::MEDIA_INFO& stMediaInfo
    );

    /**
    * Description:  GetNatPacket(). ��ȡNAT������Ϣ��
    * @param  [in]  stNatReqPara    NAT����Ĳ���
    * @param  [out] strPacket       ��װ����ַ���
    * @return       long.   ������
    */
    long GetNatPacket
    (
        IN  const RTSP::NAT_REQ_PARA&   stNatReqPara,
        OUT string&                     strPacket
    );

    /**
    * Description:  GetVcrPacket(). ��ȡVCR������Ϣ��
    * @param  [in]  stVcrReqPara    VCR����Ĳ���
    * @param  [out] strPacket       ��װ����ַ���
    * @return       long.   ������
    */
    long GetVcrPacket
    (
        IN  const RTSP::VCR_REQ_PARA&   stVcrReqPara,
        OUT string&                     strPacket
    );

    /**
    * Description:  GetKeepAlivePacket(). ��ȡ����������Ϣ��
    * @param  [out] strPacket       ��װ����ַ���
    * @return       void.
    */
    void GetKeepAlivePacket
    (
        OUT string& strPacket
    );

    /**
    * Description:  DecodePacket().     ����RTSP��
    * @param  [in]  strPacket   RTSP��
    * @param  [out] stResult    �������
    * @return       long.   ������
    */
    long DecodePacket
    (
        IN  const string&           strPacket,
        OUT RTSP::DECODE_RESULT&    stResult
    )const;

    /**
    * Description:  GetRspPacket().     ��ȡ��Ӧ��Ϣ��
    * @param  [in]  strCSeq         �����CSeq
    * @param  [out] strPacket   ��װ����ַ���
    * @return       void.
    */
    void GetRspPacket
    (
        IN  const string&   strCSeq,
        OUT string&         strPacket
    )const;

    /**
    * Description:  GetFieldValue().    ��ȡָ��ͷ�������
    * @param  [in]  strField    ����ͷ���ַ���
    * @param  [in]  strKey      ͷ��Keyֵ
    * @param  [out] strContent  ͷ������
    * @param  [in]  strEnd          ������
    * @param  [in]  strInterval     �ָ���
    * @return       long.   ������
    */
    static long GetFieldValue
    (
        IN  const string&   strField,
        IN  const string&   strKey,
        OUT string&         strContent,
        IN  const string&   strEnd      = RTSP::CRLF,
        IN  const string&   strInterval = RTSP::COLON
    );

    /**
    * Description:  TrimString().   ȥ���ַ�����β�Ŀո�Tab
    * @param  [io]  str     ��������ַ���
    * @return       void.
    */
    static void TrimString
    (
        IO  string& str
    );

    /**
    * Description:  MuiltLineToOneLine().   ��ͷ�����ݵĶ���תΪ����
    * @param  [io]  strFieldContent     �������ͷ�������ַ���
    * @return       void.
    */
    static void MuiltLineToOneLine
    (
        IO  string& strFieldContent
    );

    /**
    * Description:  DecodeNatPacket(). ����NAT��Խ��
    * @param  [in]  strPacket      RTSP��NAT��
    * @param  [out] stNatResult    NAT�������
    * @return       long.   ������
    */
    long DecodeNatPacket
    (
    IN  const string&           strPacket,
    OUT RTSP::NAT_DECODE_RESULT&    stNatResult
    ) const;

    /**
    * Description:  GetNATRspPacket().     ��ȡNAT��Ӧ��Ϣ��
    * @param  [in]  stNatDecodeResult      ����NAT��Խ���Ľ��
    * @param  [out] strPacket   ��װ����ַ���
    * @return       void.
    */
    void GetNATRspPacket
    (
        IN  const RTSP::NAT_DECODE_RESULT&   stNatDecodeResult,
        OUT string&         strPacket
    )const;
private:    
    /**
    * Description:  GetCSeq().  ��ȡ�µ�CSeq
    * @return       string.     �µ�CSeq�ַ���
    */
    string GetCSeq();

    /**
    * Description:  SetCommonField().   ���ù���ͷ��
    * @param  [in]  strMethod   ���󷽷���
    * @param  [io]  strPacket   ��װ����ַ���
    * @return       void.
    */
    void SetCommonField
    (
        IN  const string&   strMethod,
        IO  string&         strPacket
    );

    /**
    * Description:  SetRangeField().    ����Rangeͷ��
    * @param  [in]  ulStartTime         ��ʼʱ��
    * @param  [in]  bRelativeTime   �Ƿ�Ϊ���ʱ��
    * @param  [io]  strRange        ���ú���ַ���
    * @return       long.   ������
    */
    long SetRangeField
    (
        IN  unsigned long   ulStartTime,
        IN  BOOL            bRelativeTime,
        IO  string&         strRange
    )const;

    /**
    * Description:  GetFirstLine(). ��ȡ�������ֶκ�����ͷ���ַ���
    * @param  [in]  strPacket       ��Ϣ��
    * @param  [out] strFirstHeader  �������ֶ�
    * @param  [out] strField        ͷ��
    * @return       long.   ������
    */
    long GetFirstLine
    (
        IN  const string&   strPacket,
        OUT string&         strFirstHeader,
        OUT string&         strField
    )const;

    /**
    * Description:  GetPlayInfoType().  ��ȡ������Ϣ����
    * @param  [in]  strField        ����ͷ���ַ���
    * @param  [out] ulPlayInfoType  ������Ϣ����
    * @return       long.   ������
    */
    long GetPlayInfoType
    (
        IN  const string&   strField,
        OUT unsigned long&  ulPlayInfoType
    )const;

    /**
    * Description:  GetRtpInfo().  ��ȡRTP-Info
    * @param  [in]  strField        ����ͷ���ַ���
    * @param  [out] stResult    �������
    * @return       long.   ������
    */
    long GetRtpInfo
    (
        IN  const string&           strField,
        OUT RTSP::DECODE_RESULT&    stResult
    )const;

private:
    //���к�
    unsigned long m_ulCSeq;
    //ý����Ϣ
    RTSP::MEDIA_INFO m_stMediaInfo;
};

#endif //_RTSP_PACKET_H_HEADER_INCLUDED_

