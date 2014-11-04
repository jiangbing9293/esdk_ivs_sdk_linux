/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : rtsp_client_datatype.h
  ��    �� : y00182336
  �������� : ��װ�ͽ���SDP
  �޸���ʷ :
    1 ���� : 2011-09-26
      ���� : y00182336
      �޸� : ����

    2 ���� : 
      ���� : 
      �޸� : 
 ******************************************************************************/
#ifndef __RTSP_CLIENT_DATATYPE_H_
#define __RTSP_CLIENT_DATATYPE_H_

namespace RTSP_SDK{

/// ����ģ����
#define RTSP_CLIENT_SERVICE_NAME  "rtsp_client"
/// ��־Ŀ¼
#define RTSP_CLIENT_LOG_PATH      "./log"

/// ������־·������
#define RTSP_CLIENT_PATH_LEN      100

// ��֯ͨ����
#define     RTSP_INTER_LEAVE_NUM            0


// ��ַ��˿�
typedef struct _stINET_ADDR
{
    unsigned int        unAddrIp;
    unsigned short      unAddrPort;
}INET_ADDR;


// ����ý���ַ
typedef struct _stMEDIA_ADDR
{
    unsigned int        unMediaIp;              // ý��IP
    unsigned short      unMediaVideoPort;       // ý����ƵRTP�˿ڣ�RTCP�˿�+1
    unsigned short      unMediaAudioPort;       // ý����ƵRTP�˿ڣ�RTCP�˿�+1
}MEDIA_ADDR;


// �����ض��ļ���ʱ��ƫ����
#define OFFSET_BEGIN  ((unsigned int)-3)  // ��ʾý���ʼ��λ��
#define OFFSET_CUR    ((unsigned int)-2)  // ��ʾý�嵱ǰλ��
#define OFFSET_END    ((unsigned int)-1)  // ��ʾý�����λ��

// ý�岥�ŷ�Χ��ʱ���ʾ����
typedef enum
{
    RANGE_TYPE_NPT = 0, // NormalPlayTime(���ʱ��)
    RANGE_TYPE_UTC,     // UTC AbsoluteTime(UTC����ʱ��)
    RANGE_TYPE_SMPTE    // SMPTEʱ�䣬Ŀǰ�ݲ�֧��
}MEDIA_RANGE_TYPE_E;

// ��ʾý�岥�ŷ�Χ�Ľṹ��
typedef struct
{
    unsigned int  MediaBeginOffset;   // ��ʼ����λ��(��λ:��)
    unsigned int  MediaEndOffset;     // ��������λ��(��λ:��)
    MEDIA_RANGE_TYPE_E enRangeType;   // Range��ʱ������
}MEDIA_RANGE_S;

// ��־����
typedef enum 
{
    RTSP_LOG_LEVEL_EMERGENCY = 0,   // Emergency �����������־��
    RTSP_LOG_LEVEL_ALART,           // Alert �����������־��
    RTSP_LOG_LEVEL_CRITICAL,        // Critical �����������־��
    RTSP_LOG_LEVEL_ERROR,           // Error �����������־��
    RTSP_LOG_LEVEL_WARNING,         // Warning �����������־��
    RTSP_LOG_LEVEL_NOTICE,          // Notice �����������־��
    RTSP_LOG_LEVEL_INFO,            // Info �����������־��
    RTSP_LOG_LEVEL_DEBUG            // Debug �����������־��
}RTSP_LOG_LEVEL_E;


// ����Session״̬
typedef enum _enRTSP_SESSION_STATUS
{
    RTSP_SESSION_STATUS_INIT,
    RTSP_SESSION_STATUS_READY,
    RTSP_SESSION_STATUS_PLAYING,

    RTSP_SESSION_STATUS_ABNORMAL,        // �쳣״̬
	RTSP_SESSION_STATUS_DELETE
}RTSP_SESSION_STATUS;


// ����Rtsp��Ϣ����
typedef enum _enRTSP_METHOD_TYPE
{
    RTSP_METHOD_SETUP = 0,     // SETUP����
    RTSP_METHOD_PLAY,          // PLAY����
    RTSP_METHOD_PAUSE,         // PAUSE����
    RTSP_METHOD_TEARDOWN,      // TEARDOWN����
    RTSP_METHOD_ANNOUNCE,      // ANNOUNCE����
    RTSP_METHOD_DESCRIBE,      // DESCRIBE����
    RTSP_METHOD_OPTIONS,       // OPTION����
    RTSP_METHOD_NUM

}RTSP_METHOD_TYPE;


// RTSP Client������
typedef enum _enRTSP_CLIENT_RET_CODE
{
    RTSP_ERR_NOT_EXIST          = 70001,                        /// ���Ӿ��������
    RTSP_ERR_DISCONNECT         = 70002,                        /// ����ʧ��
    RTSP_ERR_RESPONSE           = 70003,                        /// ��Ӧ��Ϣ����
    RTSP_ERR_TIMEOUT            = 70004,                        /// ��Ӧ��ʱ
    RTSP_ERR_STAUTS_ABNORMAL    = 70005,                        /// ����״̬�쳣
    RTSP_ERR_SEND_FAIL          = 70006,                        /// ���ͳ���
    RTSP_ERR_RECV_FAIL          = 70007,                        /// ���ճ���
    RTSP_ERR_PARAMETER          = 70008,                        /// ����������

    RTSP_ERR_NORMAL_STOP        = 70009,                        /// ����������ANNOUNCE������������
    RTSP_ERR_ABNORMAL_STOP      = 70010,                        /// �쳣������ANNOUNCE�쳣��������

    RTSP_ERR_NOTENOUGH_BANDWIDTH= 70011,                        /// û���㹻�Ĵ�����Ӧ453������

    RTSP_ERR_RET_SYS            = 70030                         /// ͨ��ϵͳ����

}RTSP_CLIENT_RET_CODE;

// Windows��Ҫ__stdcall��Liunx����Ҫ
#ifdef WIN32
#define RTSPAPI __stdcall
#else
#define RTSPAPI
#endif

// �쳣�ص����� //__stdcall
typedef  void (RTSPAPI *fExceptionCallBack)(long handle,
                                    unsigned int ulMsgType,
                                    unsigned int ulErrCode,           // �ص����������룬���_enRtspClientRetCode
                                    void *pParameter, 
                                    void *pUser);

// ý�����ص�//__stdcall
typedef  void (RTSPAPI *fDataCallBack)(long handle, 
                               char *pBuffer,
                               unsigned int ulBufSize,
                               void *pUser);
}

#endif //__RTSP_CLIENT_DATATYPE_H_
