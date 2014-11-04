/********************************************************************
    filename	: 	CRDAProt.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	RDAЭ��ջ
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/

#ifndef RDAPROT_H
#define RDAPROT_H

/*!< ģ�����ͷ�ļ� */
#include "vos.h"

/*!< �ڴ������غ�����ͷ�ļ� */
#include "vos_common.h"

#define RDA_PROT_VERSION 0x02  //Э��汾��
#define RDA_PROT_TYPE 0x01  //Э������

//BEGINЭ�鳤����ض���
#define LEN_RDA_MSG_HEAD (2 + 2 + 4 + MD5_DIGEST_LENGTH + 1 + 1 + 2)

/* 2���ֽڱ�ʾtag��2���ֽڱ�ʾlength */
#define TLV_HEAD_LENGTH (4 * sizeof(RDA_CHAR))
#define LEN_RDA_NAME 32   //RDAЭ�����õ������ֳ���
#define LEN_RDA_ID 20   //������id����
#define LEN_MAX_RDA_STR 512  //һ���ַ������Ե�ֵ�����3K�ֽ�

#define LEN_RDA_ATTRTAG_NAME 32  //���Ա�ʶ�����Ƴ���
#define LEN_RDA_MSGTYPE_NAME 32  //��Ϣ���͵����Ƴ���
#define LEN_RDA_SESSION_ID 20  //Sessionid����

//ENDЭ�鳤����ض���

#define MAX_CHANNEL_NUM CUMW_DECODER_CHANNEL_MAX_NUM //һ�����������֧�ֵ�ͨ����Ŀ

const unsigned char MAX_PAYLOAD_TYPE_NUM = CUMW_PAY_LOAD_TYPE_NUM;  //֧�ֵ������Ƶ/��Ƶ��PayloadType

//BEGIN MD5��ض���
#define MAX_MD5KEY_LENGTH 128
#define MD5_DIGEST_LENGTH 16

//MD5 ��Կ����Կ����
#define DEFAULT_MD5_KEY "NVS"
#define DEFAULT_MD5_KEY_LENGTH strlen(DEFAULT_MD5_KEY)

//�Ƿ����MD5У�鿪��,1��ʾҪ����MD5У�飬0��ʾ����Ҫ����MD5У��
#define NO_MD5_CHECK 1

//�����������ʽ����
#define LEN_RDA_ENCODING 16

//END  MD5��ض���

//�����Ͷ���
enum RDA_PACK_TYPE
{
    RDA_MSG_REQ = 0x0001,  //����
    RDA_MSG_RSP = 0x0002   //��Ӧ
};

//������Ϣ��������
#define RDA_MSG_NAME_INIT_BUSINESS_REQ "RDAInitBusinessReq"
#define RDA_MSG_NAME_INIT_BUSINESS_RSP "RDAInitBusinessRsp"
#define RDA_MSG_NAME_REGISTE_REQ "RDARegisteReq"
#define RDA_MSG_NAME_REGISTE_RSP "RDARegisteRsp"
#define RDA_MSG_NAME_HEARTBEAT_REQ "RDAHeartBeatReq"
#define RDA_MSG_NAME_HEARTBEAT_RSP "RDAHeartBeatRsp"
#define RDA_MSG_NAME_START_BUSINESS_REQ "RDAStartBusinessReq"
#define RDA_MSG_NAME_START_BUSINESS_RSP "RDAStartBusinessRsp"
#define RDA_MSG_NAME_STOP_BUSINESS_REQ "RDAStopBusinessReq"
#define RDA_MSG_NAME_STOP_BUSINESS_RSP "RDAStopBusinessRsp"
#define RDA_MSG_NAME_AUDIO_CONTROL_REQ "RDAAudioControlReq"
#define RDA_MSG_NAME_AUDIO_CONTROL_RSP "RDAAudioControlRsp"
#define RDA_MSG_NAME_TUNEUP_VOLUME_REQ "RDATuneupVolumeReq"
#define RDA_MSG_NAME_TUNEUP_VOLUME_RSP "RDATuneupVolumeRsp"
#define RDA_MSG_NAME_UNREGISTER_REQ "RDAUnRegisteReq"
#define RDA_MSG_NAME_UNREGISTER_RSP "RDAUnRegisteRsp"
#define RDA_MSG_NAME_CONSOLE_SHUTDOWN_REQ "RDAConsoleShutDownReq"
#define RDA_MSG_NAME_CONSOLE_SHUTDOWN_RSP "RDAConsoleShutDownRsp"
#define RDA_MSG_NAME_ERROR_NOTIFY_REQ "RDAErrorNotifyReq"
#define RDA_MSG_NAME_ERROR_NOTIFY_RSP "RDAErrorNotifyRsp"
#define RDA_MSG_NAME_SET_RD_DISPLAY_MODE_REQ "RDASetRDDisplayModeReq"
#define RDA_MSG_NAME_SET_RD_DISPLAY_MODE_RSP "RDASetRDDisplayModeRsp"

//������������
#define RDA_TAG_NAME_BUSINESSID "BusinessID"
#define RDA_TAG_NAME_DECODERID "DecoderID"
#define RDA_TAG_NAME_DECODERNAME "DecoderName"
#define RDA_TAG_NAME_CHANNELNO "ChannelNo"
#define RDA_TAG_NAME_CHANNELSINFO "ChannelsInfo"
#define RDA_TAG_NAME_AUDIOCONTROL "AudioControl"
#define RDA_TAG_NAME_TUNEUPVOLUME "TuneupVolume"
#define RDA_TAG_NAME_RD_DISPLAY_MODE "RDDisplayMode"
#define RDA_TAG_NAME_ENCODING "Encoding"

const char RDA_TAG_NAME_TRANSPROT_PROTOCOL[] = "TransportProtocol";
const char RDA_TAG_NAME_ERROR_CODE[] = "ResultCode";
const char RDA_TAG_NAME_ERROR_DESCRIPTION[] = "ResultDescription";
const char RDA_TAG_NAME_VIDEO_PAY_LOAD_TYPES[] = "VideoPayloadTypes";
const char RDA_TAG_NAME_AUDIO_PAY_LOAD_TYPES[] = "AudioPayloadTypes";
const char RDA_TAG_NAME_MEDIA_IP[] = "MediaIP";
const char RDA_TAG_NAME_TCP_INTERLEAVED_PORT[] = "TcpInterleavedPort";
const char RDA_TAG_NAME_UDP_VIDEO_RTP_PORT[]  = "UdpVideoRtpPort";
const char RDA_TAG_NAME_UDP_VIDEO_RTCP_PORT[] = "UdpVideoRtcpPort";
const char RDA_TAG_NAME_UDP_AUDIO_RTP_PORT[]  = "UdpAudioRtpPort";
const char RDA_TAG_NAME_UDP_AUDIO_RTCP_PORT[] = "UdpAudioRtcpPort";
const char RDA_TAG_NAME_VIDEO_PAY_LOAD_TYPE[] = "VideoPayloadType";
const char RDA_TAG_NAME_AUDIO_PAY_LOAD_TYPE[] = "AudioPayloadType";
const char RDA_TAG_NAME_SESSION_URL[] = "SessionUrl";
const char RDA_TAG_NAME_SESSION_ID[] = "SessionID";

//�������������ض���
typedef char           RDA_CHAR;   //Integer1: 1���ֽ�����
typedef unsigned char  RDA_UCHAR;  //unsigned Integer1: 1���ֽ��޷������ͣ�Enum: 1���ֽ�ö������
typedef short          RDA_INT2;   //Integer2: 2���ֽ�����
typedef unsigned short RDA_UINT2;  //unsigned Integer2: 2���ֽ��޷�������
typedef long           RDA_INT4;   //Integer4: 4���ֽ�����
typedef unsigned long  RDA_UINT4;  //Unsigned Integer: 4���ֽ��޷�������

//�ַ�������������Ͷ���
typedef char RDA_STRING_NAME[LEN_RDA_NAME + 1]; //�������������Ͷ���
typedef char RDA_STRING_ID[LEN_RDA_ID + 1]; //������id���Ͷ���
typedef char RDA_STRING[LEN_MAX_RDA_STR];
typedef char RDA_STRING_ENCODING[LEN_RDA_ENCODING + 1]; //�����������ʽ���Ͷ���

//�ṹ�����Ͷ���
typedef struct tagChannelInfo
{
    RDA_UINT2       usChannelNo;
    RDA_STRING_NAME szChannelName;
} RDA_CHANNEL_INFO;

typedef struct tagChannelsInfo
{
    RDA_UINT2        usChannelNum;
    RDA_CHANNEL_INFO stChannelInfo[MAX_CHANNEL_NUM];
} RDA_CHANNELS_INFO;

//֧�ֵ�PayloadType��Ϣ
struct RDA_PAY_LOAD_TYPES_INFO
{
    RDA_UCHAR ucPayloadTypeNum;   //֧�ֵ�PayloadType��������󲻳���MAX_PAYLOAD_TYPE_NUM
    RDA_UCHAR stPayloadTypeArry[MAX_PAYLOAD_TYPE_NUM];    //֧�ֵ�PayloadTypeֵ��Ԫ�ظ���ΪucPayloadTypeNum
};

//�������ͱ�ʶ����
enum RDA_DATA_TYPE
{
    enRDA_CHAR  = 0x00,
    enRDA_UCHAR = 0x01,
    enRDA_INT2  = 0x02,
    enRDA_UINT2 = 0x03,
    enRDA_INT4   = 0x04,
    enRDA_UINT4  = 0x05,
    enRDA_STRING = 0x06,
    enRDA_STRING_NAME = 0x07,
    enRDA_STRING_ID = 0x08,
    enRDA_CHANNELS_INFO = 0x09,
    RDA_TYPE_PAY_LOAD_TYPES_INFO = 0x0A
};

//RDA��Ϣ����TAG����
enum RDA_ATTR_TAG
{
    TagBusinessID = 0x01,
    TagDecoderID   = 0x02,
    TagDecoderName = 0x03,
    TagChannelNo   = 0x04,
    TagChannelName = 0x05,
    TagChannelNum  = 0x06,

    //TagDecoderType = 0x07,
    //TagLensID               = 0x08,
    TAG_TRANSPORT_PROTOCOL = 0x09, //ý��������Э��
    //TagSendMediaPort        = 0x0A,
    TagVolume = 0x0B,
    TagChannelInfo  = 0x0C,
    TagChannelsInfo = 0x0D,
    TagErrorCode = 0x0E,
    TagErrorDescription = 0x0F,
    TagAudioCmd = 0x10,
    //TagSendMediaIP          = 0x11,
    TagRDDispayMode = 0x12,
    TAG_SESSION_ID = 0x13,     //�ỰID
    //TagStreamID             = 0x14, //��ID
    TAG_VIDEO_PAY_LOAD_TYPES = 0x15, //������֧�ֵ���ƵPayloadType�����Ͳμ�RDA_PAY_LOAD_TYPES_INFO
    TAG_AUDIO_PAY_LOAD_TYPES = 0x16, //������֧�ֵ���ƵPayloadType�����Ͳμ�RDA_PAY_LOAD_TYPES_INFO
    TAG_MEDIA_IP = 0x17, //������/����˵�ý��IP
    TAG_TCP_INTERLEAVED_PORT = 0x18, //������/����˵�TCP��֯�˿ڡ�TCP��ʽ���и�����
    TAG_UDP_VIDEO_RTP_PORT  = 0x19,     //������/����˵�UDP��ƵRTP�˿ڡ�UDP��ʽ���и�����
    TAG_UDP_VIDEO_RTCP_PORT = 0x1A, //������/����˵�UDP��ƵRTCP�˿ڡ�UDP��ʽ���и�����
    TAG_UDP_AUDIO_RTP_PORT  = 0x1B,     //������/����˵�UDP��ƵRTP�˿ڡ�UDP��ʽ���и�����
    TAG_UDP_AUDIO_RTCP_PORT = 0x1C, //������/����˵�UDP��ƵRTCP�˿ڡ�UDP��ʽ���и�����
    TAG_VIDEO_PAY_LOAD_TYPE = 0x1D, //Э�̺����ƵPayloadType
    TAG_AUDIO_PAY_LOAD_TYPE = 0x1E, //Э�̺����ƵPayloadType
    TAG_SESSION_URL = 0x1F, //�ỰURL����ΪRTSP��Request-URI
	TagEncoding = 0x20   //���������Ʊ�������
};

//��Ϣ���Ͷ���
enum RDAMsgType
{
    RDA_HEARTBEAT_REQ = 0x0001, //��������
    RDA_HEARTBEAT_RSP = 0x1001, //������Ӧ

    RDA_REGISTER_REQ = 0x0002, //Զ�̽�����ע����Ϣ
    RDA_REGISTER_RSP = 0x1002,

    RDA_INIT_BUSINESS_REQ = 0x0003, //���س�ʼ��Զ�̽���ҵ��������Ϣ
    RDA_INIT_BUSINESS_RSP = 0x1003, //���س�ʼ��Զ�̽���ҵ����Ӧ��Ϣ

    RDA_START_BUSINESS_REQ = 0x0004, //����֪ͨ����Զ�̽���ҵ��
    RDA_START_BUSINESS_RSP = 0x1004,

    RDA_STOP_BUSINESS_REQ = 0x0005, //����ֹͣԶ�̽���ҵ��
    RDA_STOP_BUSINESS_RSP = 0x1005,

    RDA_AUDIO_CONTROL_REQ = 0x0006, //���ؿ�ʼ/ֹͣԶ�̽�����������������Ϣ
    RDA_AUDIO_CONTROL_RSP = 0x1006,

    RDA_TUNEUP_VOLUME_REQ = 0x0007, //���ط�����������������Ϣ
    RDA_TUNEUP_VOLUME_RSP = 0x1007,

    RDA_UNREGISTER_REQ = 0x0008, //Զ�̽�����ע����Ϣ
    RDA_UNREGISTER_RSP = 0x1008,

    RDA_CONSOLE_SHUTDOWN_REQ = 0x0009, //���عر�֪ͨ��Ϣ
    RDA_CONSOLE_SHUTDOWN_RSP = 0x1009,

    RDA_ERROR_NOTIFY_REQ = 0x000A, //Զ�̽����������ϱ�����
    RDA_ERROR_NOTIFY_RSP = 0x100A,

    RDA_SET_RD_DISPLAY_MODE_REQ = 0x0010, //���ط�������Զ�̽�������ʾģʽ������Ϣ
    RDA_SET_RD_DISPLAY_MODE_RSP = 0x1010
};

typedef struct tagRDAMsgHead
{
    RDA_UINT2 usPackType; //�����ͣ���ʶ�ð����ͣ�ռ��2�ֽڡ�1 �����, 2 ��Ӧ��
    RDA_UINT2 usPackID; //����ˮ�ţ���ʶ�ð���ռ��2�ֽڡ�
    RDA_UINT4 ulPackLength; //�����ȣ�������Ϣ���ܳ��ȣ��ֽ�������������ͷ����Ϣ�塣ռ��4�ֽڡ�
    RDA_CHAR  arrayMD5[MD5_DIGEST_LENGTH]; //MD5У���
    RDA_UCHAR ucProtType; //Э������: ռ1�ֽ�, RDA��0x01
    RDA_UCHAR ucVersion; //Э��汾���壬ռ��1�ֽ�, RDA��0x01
    RDA_UINT2 usMsgType; //��Ϣ���ͣ�ռ2�ֽ�
} RDAMsgHead;

class CRDAMsgAttrMgr;
class CRDAMsg
{
public:
    CRDAMsg();
    virtual ~CRDAMsg();
    // �����warning C4150: ɾ��ָ��������m_pRDAMsgAttrMgr�����͵�ָ�룻û�е����������� 
    // ���waring�ᵼ���ڴ�й¶��ǰ�������������������û�б�����
    // class CRDAMsgAttrMgr;���ַ�ʽ��ǰ��������������ֻ��������ָ�룬ǰ�������Ժ�����޷���������ʵ�塣
    // ���ԣ�delete��ʱ��CRDAMsgAttrMgr�����������Ժ��������͸�����ɼ��ģ�����ʹ��ͷ�ļ�������
    // sxj �����������Ƶ�cpp��
    //{
    //    VOS_DELETE(m_pRDAMsgAttrMgr); /*lint !e1551*/ //����ȷ��VOS_DELETE���б���
    //};
public:

    /**************************************************************************
    * name       : Init
    * description: ��ʼ��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    LONG         Init( );

    /**************************************************************************
    * name       : DecodeBody
    * description: ����
    * input      : pBuffer  Ҫ��������ݻ�����
                   ulLength Ҫ��������ݳ���
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    LONG         DecodeBody(const char * pBuffer, const ULONG ulLength);

    /**************************************************************************
    * name       : EncodeBody
    * description: ����
    * input      : pBuffer      Ҫ��������ݻ�����
                   ulMaxLength  ��󳤶�
                   ulUsedLength ���õĳ���
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    LONG         EncodeBody(char *pBuffer, const ULONG ulMaxLength,
                            ULONG &ulUsedLength);

    /**************************************************************************
    * name       : Print
    * description: д��־
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    VOID         Print();

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual long RegisterAttrs( );

public:
    RDAMsgHead m_stMsgHead; //��Ϣͷ
    char m_szRDAMsgTypeName[LEN_RDA_MSGTYPE_NAME+1]; //��Ϣ����
public:
    CRDAMsgAttrMgr * m_pRDAMsgAttrMgr;
};

/* ������ע����Ϣ */
class CRDARegisteReq : public CRDAMsg
{
public:
    CRDARegisteReq();
    ~CRDARegisteReq(){};
public:
    RDA_STRING_ID m_szDecoderID;          //������ID��20λ��������\0��
    RDA_STRING_NAME m_szDecoderName;        //���������ƣ�32λ��������\0��
    RDA_CHANNELS_INFO m_stChannelsInfo;       //���ͨ����Ϣ
    RDA_PAY_LOAD_TYPES_INFO m_stVideoPayloadTypes;  //������֧�ֵ���ƵPayloadType��Ϣ�����16��
    RDA_PAY_LOAD_TYPES_INFO m_stAudioPayloadTypes;  //������֧�ֵ���ƵPayloadType��Ϣ�����16��
	RDA_STRING_ENCODING m_szDecoderEncoding; //�����������ʽ
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ע����Ϣ����Ӧ��Ϣ */
class CRDARegisteRsp : public CRDAMsg
{
public:
    CRDARegisteRsp();
    ~CRDARegisteRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* �����ʼ����Ϣ */
class CRDAInitBusinessReq : public CRDAMsg
{
public:
    CRDAInitBusinessReq( );
    ~CRDAInitBusinessReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo; //ͨ�����
    RDA_UCHAR m_ucTransportProtocol; //��Ƶ����Э��
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* �����ʼ����Ӧ��Ϣ */
class CRDAInitBusinessRsp : public CRDAMsg
{
public:
    CRDAInitBusinessRsp();
    ~CRDAInitBusinessRsp(){};

public:
    RDA_STRING_ID m_szDecoderID;          //������ID��20λ��������\0��
    RDA_UINT4 m_ulErrorCode;          //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription;   //�������Ӧ��������
    RDA_UCHAR m_ucTransportProtocol;  //��Ƶ����Э��
    RDA_STRING m_szMediaIP;            //��������ý��ͨ��IP
    RDA_UINT2 m_usTcpInterleavedPort; //TCP��֯�˿�
    RDA_UINT2 m_usUdpVideoRtpPort;    //UDP��ƵRTP�˿�
    RDA_UINT2 m_usUdpVideoRtcpPort;   //UDP��ƵRTCP�˿�
    RDA_UINT2 m_usUdpAudioRtpPort;    //UDP��ƵRTP�˿�
    RDA_UINT2 m_usUdpAudioRtcpPort;   //UDP��ƵRTCP�˿�

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ��������Ƶ������Ϣ */
class CRDAStartBusinessReq : public CRDAMsg
{
public:
    CRDAStartBusinessReq( );
    ~CRDAStartBusinessReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID;  //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo;  //ͨ�����
    RDA_UCHAR m_ucTransportProtocol;  //��Ƶ����Э��
    RDA_STRING m_szMediaIP;            //��������ý��ͨ��IP
    RDA_UINT2 m_usTcpInterleavedPort; //TCP��֯�˿�
    RDA_UINT2 m_usUdpVideoRtpPort;    //UDP��ƵRTP�˿�
    RDA_UINT2 m_usUdpVideoRtcpPort;   //UDP��ƵRTCP�˿�
    RDA_UINT2 m_usUdpAudioRtpPort;    //UDP��ƵRTP�˿�
    RDA_UINT2 m_usUdpAudioRtcpPort;   //UDP��ƵRTCP�˿�
    RDA_UCHAR m_ucVideoPayloadType;   //��ƵPayload����
    RDA_UCHAR m_ucAudioPayloadType;   //��ƵPayload����
    RDA_STRING m_szSessionId;          //�ỰID
    RDA_STRING m_szSessionUrl;         //�ỰURL

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs();
};

/* ����������Ϣ����Ӧ��Ϣ */
class CRDAStartBusinessRsp : public CRDAMsg
{
public:
    CRDAStartBusinessRsp();
    ~CRDAStartBusinessRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ֹͣ����Ƶ������Ϣ */
class CRDAStopBusinessReq : public CRDAMsg
{
public:
    CRDAStopBusinessReq( );
    ~CRDAStopBusinessReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo; //ͨ�����
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ֹͣ����Ƶ������Ϣ */
class CRDAStopBusinessRsp : public CRDAMsg
{
public:
    CRDAStopBusinessRsp();
    ~CRDAStopBusinessRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //ҵ��ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ����/ֹͣ������Ƶ��Ϣ */
class CRDAAudioCtrolReq : public CRDAMsg
{
public:
    CRDAAudioCtrolReq( );
    ~CRDAAudioCtrolReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT2 m_usChannelNo; //ͨ�����
    RDA_UCHAR m_ucAudioCmd; //0x00 ֹͣ����; 0x01��ʼ����

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ����/ֹͣ������Ƶ��Ϣ����Ӧ��Ϣ */
class CRDAAudioCtrolRsp : public CRDAMsg
{
public:
    CRDAAudioCtrolRsp();
    ~CRDAAudioCtrolRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ������������������Ϣ */
class CRDATuneupVolumeReq : public CRDAMsg
{
public:
    CRDATuneupVolumeReq( );
    ~CRDATuneupVolumeReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID;  //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo;  //ͨ�����
    RDA_UCHAR m_ucVolume;     //������С��0~100��
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ����������Ϣ����Ӧ��Ϣ */
class CRDATuneupVolumeRsp : public CRDAMsg
{
public:
    CRDATuneupVolumeRsp();
    ~CRDATuneupVolumeRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ����Զ�̽�������ʾģʽ������Ϣ */
class CRDASetRDDisplayModeReq : public CRDAMsg
{
public:
    CRDASetRDDisplayModeReq();
    ~CRDASetRDDisplayModeReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo; //ͨ�����
    RDA_UINT2 m_usMode; //��ʾģʽ
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ����Զ�̽�������ʾģʽ��Ӧ��Ϣ */
class CRDASetRDDisplayModeRsp : public CRDAMsg
{
public:
    CRDASetRDDisplayModeRsp();
    ~CRDASetRDDisplayModeRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ������ע����Ϣ */
class CRDAUnregisterReq : public CRDAMsg
{
public:
    CRDAUnregisterReq( );
    ~CRDAUnregisterReq( ){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ����������Ϣ����Ӧ��Ϣ */
class CRDAUnregisterRsp : public CRDAMsg
{
public:
    CRDAUnregisterRsp();
    ~CRDAUnregisterRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ����̨�ر���Ϣ */
class CRDAConsoleShutdownReq : public CRDAMsg
{
public:
    CRDAConsoleShutdownReq( );
    ~CRDAConsoleShutdownReq( ){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* ����̨�ر���Ϣ */
class CRDAConsoleShutdownRsp : public CRDAMsg
{
public:
    CRDAConsoleShutdownRsp();
    ~CRDAConsoleShutdownRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* �����ϱ���Ϣ */
class CRDAErrorNotifyReq : public CRDAMsg
{
public:
    CRDAErrorNotifyReq( );
    ~CRDAErrorNotifyReq( ){};
public:
    RDA_UINT4 m_ulBusinessID; //ҵ��ID
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
    RDA_UINT2 m_usChannelNo; //ͨ�����
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs( );
};

/* �����ϱ�����Ӧ��Ϣ */
class CRDAErrorNotifyRsp : public CRDAMsg
{
public:
    CRDAErrorNotifyRsp();
    ~CRDAErrorNotifyRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID
    RDA_UINT4 m_ulErrorCode; //�����룬0����ɹ�
    RDA_STRING m_szErrorDescription; //�������Ӧ��������

protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ����������Ϣ */
class CRDAHeartBeatReq : public CRDAMsg
{
public:
    CRDAHeartBeatReq();
    ~CRDAHeartBeatReq(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

/* ������Ӧ��Ϣ */
class CRDAHeartBeatRsp : public CRDAMsg
{
public:
    CRDAHeartBeatRsp();
    ~CRDAHeartBeatRsp(){};
public:
    RDA_STRING_ID m_szDecoderID; //������ID��20λ��������\0��
protected:

    /**************************************************************************
    * name       : RegisterAttrs
    * description: ע���ֶε�list��
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    virtual LONG RegisterAttrs(void);
};

#endif //RDAPROT_H
