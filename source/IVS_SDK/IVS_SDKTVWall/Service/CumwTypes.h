/********************************************************************
    filename	: 	CumwTypes.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	���������м���ڲ�����
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/

#ifndef CUMWT_YPES_H
#define CUMWT_YPES_H

#include <list>
using namespace std;

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

#pragma pack(push,1)

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮��������
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    const char PRODUCT_NAME[] = "Huawei CU";      //��Ʒ����
    const char PRODUCT_VERSION[]  = "V100R002C01";         //��Ʒ�汾��
    const char FILE_PATH_CONFIG[] = "conf";           //�����ļ����·��
    const char FILE_NAME_INI_CONFIG[] = "conf\\CUMW.ini"; //�����ļ���
    const char FILE_PATH_LOG[] = "logs";           //��־�ļ����·��
    const char FILE_NAME_LOG[] = ".log";           //��־�ļ�����׺
    //BEGIN R002C01SPC100 DEL 2011-07-15 ligengqiang 00124479 for Purify�������
    //const char FILE_NAME_CRASH_DLL[]    = "XCPTHLR.DLL";    //�����쳣DLL����
    //END   R002C01SPC100 DEL 2011-07-15 ligengqiang 00124479 for Purify�������

    const unsigned long FILE_PATH_LEN = 256;      //�ļ�·����󳤶�

    const unsigned long CONN_MGR_SELECT_PERIOD = 10;   //���ӹ�������Select����
    const unsigned long TIMER_INTERVAL = 100;  //��ʱ�����
    const unsigned long TIMER_SCALES_SECOND = 10;   //1���scale����
    const unsigned long INTERVAL_DECODER_CONN_TIME_OUT = 15;   //���������ӳ�ʱʱ������λΪ��

    const unsigned long WAIT_FOR_THREAD_EXIT_TIME = 30000;    //�߳��˳��ȴ�ʱ�䣬��λ������
    const unsigned long EVENT_SYNC_QUEUE_NUM = 1024;     //�¼�ͬ�����еĸ���

    const unsigned long HEARTBEAT_SEND_INTERVAL = 60;   //�������ͼ��, 60��
    const unsigned long ASYNC_TIMEOUT_CHECK_INTERVAL = 10;   //�첽��ʱ�����,��
    const unsigned long TRY_REGISGER_INTERVAL = 60;   //����register ��ʱ����� ����

    const unsigned long HEARTBEAT_TIMEOUT_COUNT = 3;     //������ʱ����
    const unsigned long ASYNC_MAX_RESEND_COUNT = 0;     //�ط�������

    const unsigned long ASYNC_TIMEOUT_COUNT = 20 / ASYNC_TIMEOUT_CHECK_INTERVAL; //�첽��Ӧ��ʱ����5s
    const unsigned long ASYNC_KEEP_ALIVE_COUNT = ASYNC_TIMEOUT_COUNT * 10; //�첽��Ϣ�����������

    const unsigned long MAX_VOLUME = 100;   //�������ֵ

    const unsigned long MAX_LEN_SESSION_ID = 32;    //ý��ỰID����󳤶�
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮���鼰���������Ͷ���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    typedef char PATH_ARRAY[FILE_PATH_LEN];               //·����������
    typedef char (&PATH_ARRAY_REF)[FILE_PATH_LEN];        //·��������������
    typedef const char (&PATH_CARRAY_REF)[FILE_PATH_LEN]; //·������������������

    //ҵ��ID�б���
    typedef list <unsigned long> LIST_BUSINESS_ID;
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮ö�����Ͷ���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ����Э��ջ��Ϣ״̬
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum PACK_STATUS
    {
        PACK_STATUS_INIT,           ///< ��Ϣ��ʼ��
        PACK_STATUS_SENDING,        ///< ��Ϣ������
        PACK_STATUS_SENDED,         ///< ��Ϣ�ѷ���
        PACK_STATUS_FIN             ///< ��Ϣ���ͽ���
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �¼�����
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum EVENT_TYPE
    {
        //ͨ���¼�����
        EVENT_TYPE_INVALID,                     ///< ��Ч�¼�
        EVENT_TYPE_EXIT_THREAD,                 ///< �˳��̡߳���Ӧ�Ľṹ��ΪNULL
        EVENT_TYPE_GENERAL_ASYN,                ///< ͨ���첽�¼�����Ӧ�Ľṹ��Ϊ@ref CUMW::GENERAL_ASYN_INFO

        //ControlBus���¼�����
        EVENT_TYPE_CTRLBUS_BUSINESS            ///< ý��ҵ���¼�����Ӧ�Ľṹ��Ϊ@ref CControlBus::BUSINESS_ASYNC_INFO
    };

    //�¼����͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_EVENT_TYPE = sizeof("Event Type is Check Asynchronous Time Out");

    //�¼����͵���ʾ�ַ������鶨��
    const char STR_ARR_EVENT_TYPE[][STRING_LEN_EVENT_TYPE]
    = {
        "Event Type is Invalid",
        "Event Type is Exit Thread",
        "Event Type is General Asynchronous Event",
        "Event Type is Media Business",
        "Event Type is Logout",
        "Event Type is Send Message",
        "Event Type is Send Response",
        "Event Type is Receive Response Message",
        "Event Type is Receive Request Message",
        "Event Type is Handle ReLogin Event",
        "Event Type is Check Asynchronous Time Out", //STRING_LEN_EVENT_TYPEΪ���ַ����ĳ���
        "Event Type is Dispatch Stream Data",
        "Event Type is Insert Data Buffer"
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ҵ�������
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum BUSINESS_CTRL_CODE
    {
        BUSINESS_CTRL_CODE_INVALID = 0x00000000,   ///< ��Ч������

        BUSINESS_CTRL_CODE_MASK_INGRESS = 0x0000FFFF,   ///< Ingress����
        BUSINESS_CTRL_CODE_MASK_EGRESS = 0xFFFF0000,   ///< Egress����

        BUSINESS_CTRL_CODE_E_LOCAL_AUDIO = 0x00010000,   ///< Egress�ı�����Ƶ���ơ���Ӧ��vCtrlParaΪ@ref IPI_CTRL_LOCAL_AUDIO
        //BUSINESS_CTRL_CODE_E_CAMPTURE_IMAGE = 0x00020000,   ///< Egress��ץͼ����Ӧ��vCtrlParaΪ@ref IPI_CAPTURE_IMAGE
        BUSINESS_CTRL_CODE_I_LOCAL_AUDIO = 0x00000001   ///< Ingress�ı�����Ƶ���ơ���Ӧ��vCtrlParaΪ@ref IPI_CTRL_LOCAL_AUDIO
        //BUSINESS_CTRL_CODE_I_RECORD_PLAY    = 0x00000002,   ///< Ingress��¼��طſ��ơ���Ӧ��vCtrlParaΪ@ref IPI_RECORD_PLAY_CTRL
        //BUSINESS_CTRL_CODE_I_RE_INVITE      = 0x00000003,   ///< Ingress��ʵʱ���reinvite����Ӧ��vCtrlParaΪ@ref CUMW::NOTIFY_INVITE
        //BUSINESS_CTRL_CODE_I_RE_INVITE_ACK  = 0x00000004,   ///< Ingress��ʵʱ���reinviteAck����Ӧ��vCtrlParaΪ��
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief PayloadType����
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum PAY_LOAD_TYPE
    {
        PAY_LOAD_TYPE_PCMU   = 0,   ///< G711��u��
        PAY_LOAD_TYPE_G723   = 4,   ///< G723
        PAY_LOAD_TYPE_PCMA   = 8,   ///< G711��a��
        PAY_LOAD_TYPE_G722   = 9,   ///< G722
        PAY_LOAD_TYPE_MJPEG  = 24,  ///< MJPEG
        PAY_LOAD_TYPE_AMR_MB = 97,   ///< AMR_NB
        PAY_LOAD_TYPE_MPEG4  = 98,  ///< MPEG4
        PAY_LOAD_TYPE_H264 = 99,   ///< H264
        PAY_LOAD_TYPE_AVS  = 100,   ///< AVS
        PAY_LOAD_TYPE_MP3  = 101,   ///< MP3
        PAY_LOAD_TYPE_3GPP = 102  ///< 3GPP
    };
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮�ӿ���ö�����͵Ķ�Ӧ�ĳ������鶨��
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    //����ģʽ����ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_LINK_MODE = sizeof("Link Mode is Transmit Through MDU");

    //����ģʽ����ʾ�ַ������鶨�壬�μ�CUMW_LINK_MODE�Ķ���
    const char STR_ARR_LINK_MODE[][STRING_LEN_LINK_MODE]
    = {
        "Link Mode is Transmit Through MDU", //STRING_LEN_LINK_MODEΪ���ַ����ĳ���
        "Link Mode is Direct To PU",
    };

    //Э�����͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_PROTOCOL_TYPE = sizeof("Protocol Type is UDP");

    //Э�����͵���ʾ�ַ������鶨�壬�μ�CUMW_PROTOCOL_TYPE����
    const char STR_ARR_PROTOCOL_TYPE[][STRING_LEN_PROTOCOL_TYPE]
    = {
        "Protocol Type is UDP", //STRING_LEN_PROTOCOL_TYPEΪ���ַ����ĳ���
        "Protocol Type is TCP",
    };

    //Э�����͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_PLAY_TYPE = sizeof("Play Type is Remote Video Direct Connect to MDU");

    //Э�����͵���ʾ�ַ������鶨�壬�μ�CUMW_PLAY_TYPE����
    const char STR_ARR_PLAY_TYPE[][STRING_LEN_PLAY_TYPE]
    = {
        "Play Type is Local Video",
        "Play Type is Remote Video",
        "Play Type is Local Preview Of Remote Video",
        "Play Type is Video Data Call Back",
        "Play Type is Remote Video Direct Connect to MDU", //STRING_LEN_PLAY_TYPEΪ���ַ����ĳ���
    };

    //�첽���͵���ʾ�ַ�����󳤶�CUMW_ASYNC_TYPE
    const unsigned long STRING_LEN_ASYNC_TYPE = sizeof("Async Type is Handle Signal Control Info");

    //�첽���͵���ʾ�ַ������鶨��
    const char STR_ARR_ASYNC_TYPE[][STRING_LEN_ASYNC_TYPE]
    = {
        "Async Type is NON Return Info",
        "Async Type is User Login Register",
        "Async Type is Query Plat Info",
        "Async Type is Query User Info",
        "Async Type is Handle Signal Control Info", //STRING_LEN_EVENT_TYPEΪ���ַ����ĳ���
        "Async Type is Start Media Business",
    };

    //��Ƶ�豸���͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_AUDIO_TYPE = sizeof("Audio Type is Audio Out");

    //Э�����͵���ʾ�ַ������鶨�壬�μ�CUMW_PLAY_TYPE����
    const char STR_ARR_AUDIO_TYPE[][STRING_LEN_AUDIO_TYPE]
    = {
        "Audio Type is Audio Out", //STRING_LEN_AUDIO_TYPEΪ���ַ����ĳ���
        "Audio Type is Audio In",
    };

    //��Ƶ�豸���͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LEN_AUDIO_CTRL_CODE = sizeof("Audio Control Code is Set Volume");

    //Э�����͵���ʾ�ַ������鶨�壬�μ�CUMW_PLAY_TYPE����
    const char STR_ARR_AUDIO_CTRL_CODE[][STRING_LEN_AUDIO_CTRL_CODE]
    = {
        "Audio Control Code is Close",      //STRING_LEN_AUDIO_TYPEΪ���ַ����ĳ���
        "Audio Control Code is Open",
        "Audio Control Code is Set Volume",
    };
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮�첽������ض���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ý��˿���Ϣ
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct PORT_INFO
    {
        unsigned short usInterleavedPort;   ///< ��֯�˿�
        unsigned short usVideoRtpPort;      ///< ��ƵRTP�˿�
        unsigned short usVideoRtcpPort;     ///< ��ƵRTCP�˿�
        unsigned short usAudioRtpPort;      ///< ��ƵRTP�˿�
        unsigned short usAudioRtcpPort;     ///< ��ƵRTCP�˿�
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �����ý����Ϣ
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct SERVER_MEDIA_INFO
    {
        char          szSessionId[MAX_LEN_SESSION_ID + 1];      ///< �ỰID
        char          szServerIp[IVS_IP_LEN];      ///< ý�������IP
        PORT_INFO     stServerPortInfo;             ///< ý��������Ķ˿���Ϣ
        PAY_LOAD_TYPE enVideoPt;                  ///< ��ƵPT
        PAY_LOAD_TYPE enAudioPt;                  ///< ��ƵPT
        char          szSessionUrl[CUMW_PATH_LEN];  ///< �ỰURL
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �첽�ӿ�����,������CUMW_ASYNC_TYPE
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum ASYNC_TYPE
    {
        ASYNC_TYPE_INTERFACE_MAX = 0x8000,  ///<�ӿ����ֵ

        ASYNC_TYPE_HEART_BEAT,              ///<����
        ASYNC_TYPE_LOGOUT,                  ///<ע��
        ASYNC_TYPE_USER_INFO,               ///<�첽�����û���Ϣ
        ASYNC_TYPE_PLAT_INFO,               ///<�첽����ƽ̨��Ϣ
        ASYNC_TYPE_SIGNAL_CTRL,             ///<�첽�����������

        ASYNC_TYPE_MEDIA_CHANNEL_OPEN,      ///< ��ý��ͨ������Ӧ��pRetInfo�ṹ��Ϊ@ref CUMW::ASYNC_RET_MEDIA_CHANNEL_OPEN
        ASYNC_TYPE_MEDIA_CHANNEL_CLOSE,     ///< �ر�ý��ͨ������Ӧ��pRetInfo�ṹ��ΪNULL
        ASYNC_TYPE_BUSINESS_START,          ///< ҵ��ʼ
        ASYNC_TYPE_BUSINESS_RUN,            ///< ҵ������
        ASYNC_TYPE_BUSINESS_STOP_INTERFACE, ///< �ӿڴ�����ҵ��ֹͣ
        ASYNC_TYPE_BUSINESS_STOP_INNER,     ///< �ڲ�������ҵ��ֹͣ
        ASYNC_TYPE_BUSINESS_CTRL,           ///< ҵ�����
        ASYNC_TYPE_BUSINESS_INVITE,         ///< invite
        ASYNC_TYPE_BUSINESS_INVITE_ACK,     ///< invite Ack

        ASYNC_TYPE_TV_WALL_SVR_START,       ///< ��������ǽ����
        ASYNC_TYPE_TV_WALL_SVR_STOP,        ///< ֹͣ����ǽ����

        ASYNC_TYPE_DECORDER_INIT,           //���������Ի�
        ASYNC_TYPE_DECORDER_START,          //����������
        ASYNC_TYPE_DECORDER_STOP,          //ֹͣ������
        ASYNC_TYPE_AUDIO_CONTROL,           //����/ֹͣ������Ƶ��Ϣ
        ASYNC_TYPE_TUNEUP_VOLUME           //����������Ϣ
    };

    //�յ�����Ϣ��
    typedef CUMW_NOTIFY_ASYNC_RET RECEIVE_MSG;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ý��ͨ�����첽����
    ///
    /// Description.    ֱ�ӷ��ط����ý����Ϣ
    ///////////////////////////////////////////////////////////////////////////
    typedef SERVER_MEDIA_INFO ASYNC_RET_MEDIA_CHANNEL_OPEN;
}

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮�ṹ�嶨��
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    ///////////////////////////////////////////////////////////////////////////
    /// @brief �¼���Ϣ
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct EVENT_INFO
    {
        EVENT_TYPE      enEventType;        ///< �¼�����
        void*           pEventInfo;         ///< �¼���Ϣָ�룬������ȡ����ulEventType
        unsigned long   ulEventInfoLen;     ///< �¼���Ϣ���ݳ��ȣ�У����

        //���캯��
        EVENT_INFO(EVENT_TYPE enType = EVENT_TYPE_INVALID, void* pInfo = NULL, unsigned long ulInfoLen = 0)
        {
            enEventType = enType;
            pEventInfo = pInfo;
            ulEventInfoLen = ulInfoLen;
        }
    };

    //�¼�ͬ������
    typedef CSyncQueue <EVENT_INFO> CEventSyncQueue;

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ҵ����Ʋ���
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct BUSINESS_CTRL_PARA
    {
        BUSINESS_CTRL_CODE enCtrlCode;     ///< ҵ�������
        unsigned long      ulCtrlParaLen;   ///< ���Ʋ������ȣ���vCtrlPara�ĳ���
        char               vCtrlPara[1];    ///< ���Ʋ���ֵ���ɱ�����
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��ý��ͨ������
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct OPEN_MEDIA_CHANNEL_PARA
    {
        unsigned long ulAsyncSeq;         ///< �첽���
        unsigned long ulBusinessId;       ///< ҵ��ID
        unsigned long ulBusinessSrcType;  ///< ҵ����Դ����
        unsigned long ulLinkMode;         ///< ����ģʽ
        unsigned long ulProtocolType;     ///< ý��ͨ��Э�����͡�����μ�CUMW_PROTOCOL_TYPE
        PORT_INFO     stLocalPortInfo;      ///< ���ض˿���Ϣ
        char          szLocalIp[IVS_IP_LEN];   ///< ����IP
        char          szCameraId[CUMW_DEV_ID_LEN + 1];          ///< ��ͷID��ulBusinessTypeΪʵʱ���ʱ��Ч
        unsigned long ulStreamId;                             ///< ����ID��ulBusinessTypeΪʵʱ���ʱ��Ч
        char          szContentId[CUMW_CONTENT_ID_LEN + 1];     ///< ����ID��ulBusinessTypeΪ¼��ط����ء���Ƶ�㲥ʱ��Ч
        unsigned long ulStartTime;        ///< ��ʼʱ��
        unsigned long ulEndTime;          ///< ����ʱ��
        char          szDevID[CUMW_DEV_ID_LEN + 1];             ///< ��ͷ�����豸ID, �����ַ���������
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �ر�ý��ͨ������
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct CLOSE_MEDIA_CHANNEL_PARA
    {
        unsigned long ulBusinessId;       ///< ҵ��ID
        //ý��ỰID
        char szSessionId[CUMW::MAX_LEN_SESSION_ID + 1];
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ͨ�õ��첽��Ϣ����
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    struct GENERAL_ASYN_INFO
    {
        unsigned long    ulAsynSeq;   //�첽���
        CUMW::ASYNC_TYPE enAsynType;  //�첽����

        GENERAL_ASYN_INFO()
        {
            ulAsynSeq  = 0;
            enAsynType = CUMW::ASYNC_TYPE_BUSINESS_START;
        }
    };
}

#pragma pack(pop)

#endif //CUMWT_TYPES_H
