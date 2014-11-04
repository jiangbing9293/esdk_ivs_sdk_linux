/*******************************************************************************
//  ��Ȩ����    2012 ��Ϊ�������޹�˾
//  �� �� ����  IVS_MLIB_Player
//  �� �� ����  IVS_Sdp.h
//  �ļ�˵����  SDP��������
//  ��      ���� eIVS V100R001C30
//  ��      �ߣ� yangjunhai/y00182336
//  �������ڣ�2012/08/14
//
//  �޸ı�ʶ��
//  �޸�˵����
*******************************************************************************/
#ifndef _IVS_SDP_H_
#define _IVS_SDP_H_
#include "SDKDef.h"


// ����SDP��عؼ���
#define SDP_MEDIA_TYPE_AUDIO         "audio"                 
#define SDP_MEDIA_TYPE_VIDEO         "video"
#define SDP_MEDIA_SYPE_APPLICATION   "application"
#define SDP_MEDIA_STAR_TIME			 "StartTime:"
#define SDP_MEDIA_END_TIME			 "StopTime:"
#define SDP_SESSION_ID               "SessionID:"
#define SDP_ATTRIBUTES_CRYPTO        "crypto:"
#define SDP_ATTRIBUTES_CRYPTOSTORAGE "videokeycipher:"
#define SDP_CER_INDEX                "cerindex:"
#define SDP_RTP_MAP                  "rtpmap:"
#define SDP_ATTRIBUTES_INLINE        "inline:"
#define SDP_ATTRIBUTES_CONTROL       "control:"
#define SDP_ATTRIBUTES_DECODER_TYPE  "decoder-type="
#define SDP_VIDEO_H264_TYPE			 "H264"
#define SDP_VIDEO_MJPEG_TYPE		 "JPEG"
#define SDP_AUDIO_PCMU_TYPE			 "PCMU"
#define SDP_AUDIO_PCMA_TYPE		     "PCMA"
#define IVS_SECRET_KEY_TYPE_SIMPLE					"HW_IVS_IPC_ENCRYPT_SIMPLE"		//������(����ͷ)
#define IVS_SECRET_KEY_TYPE_SIMPLE_HEADER_PLAIN		"HW_IVS_IPC_ENCRYPT_SIMPLE_HEADER_PLAIN" //������(����ͷ)
#define IVS_SECRET_KEY_TYPE_AES256					"HW_IVS_IPC_ENCRYPT_AES256_HEADER_PLAIN"		//AES256(����ͷ)
#define IVS_SECRET_KEY_TYPE_AES256_PARTIAL			"HW_IVS_IPC_ENCRYPT_AES256_PARTIAL_HEADER_PLAIN"//AES-256���ּ���
#define IVS_SECRET_STORAGE_KEY_TYPE  "ASYM_RSA_2048_AES_CM_256"
#define RTP_INTERLEAVE_LENGTH               4              // ���彻֯ͷ����
#define MAX_LEN_SESSION_URL                 512            // ����URL����
#define KEY_MAX_LEN							512             // ������Կ��󳤶�
#define MAX_LEN_SESSION_ID                  32             // ����SessionID����
#define KEY_TYPE_VIDEO                      1              // ������Ƶ��Կ����
#define KEY_TYPE_AUDIO                      2              // ������Ƶ��Կ����
#define SDP_TIME_LEN						14			   // ����ʱ�䳤��

// ý������
#define PLAYER_MEDIA_TPYE_AUDIO     1        // ��Ƶ
#define PLAYER_MEDIA_TPYE_VIDEO     0        // ��Ƶ

// ����space��
#define JUMP_SPACE(psz) {                                                   \
	while (isspace(*(psz)) && ('\0' != *(psz)))     \
							{                                               \
							(psz)++;                                    \
							}                                               \
						}


// ������ܶ��󣬰��򵥼��ܣ�֡�򵥼��ܻ���֡���Ӽ���
typedef enum
{
	HW_NO_DEC_TYPE = -1,
	HW_IPC_ENCRYPT_SIMPLE = 0,
	HW_IPC_ENCRYPT_SIMPLE_HEADER = 1,
	HW_IPC_ENCRYPT_AES256 = 2,
	HW_IPC_ENCRYPT_AES256_PARTIAL = 3
} SECRET_TYPE_SDP;

//ý����Ϣ
typedef struct _stMEDIA_INFO_SDP
{
	char    SessionUrl[MAX_LEN_SESSION_URL + 1];      ///< ý��ỰURL
	char    SessionId[MAX_LEN_SESSION_ID + 1];        ///< ý��ỰID
	IVS_ULONG   port;                                       ///< ����IP
	IVS_ULONG   ManuType;
	IVS_ULONG   VideoDecoderType;
	IVS_ULONG   AudioDecoderType;
}MEDIA_INFO_SDP;

//�洢������Ϣ
typedef struct _stSTORAGESECRET_INFO
{
	IVS_ULONG	keyTpye;
	char		keyValue[KEY_MAX_LEN];
	IVS_ULONG	cerIndex;
}STORAGESECRET_INFO;

//ý�������Ϣ
typedef struct _stMEDIA_KEY_INFO
{
	IVS_UINT32 uPTVideo;    // payload value of video  add by w00210470
	IVS_UINT32 uPTAudio;    // payload value of auido 

	IVS_INT32 iVideoKeyType;
	IVS_VIDEO_DEC_TYPE enVideoDecoderType; //��Ƶ��������
	IVS_INT32 iAudioKeyType;
	IVS_AUDIO_DEC_TYPE enAudioDecoderType; //��Ƶ����
	IVS_CHAR  szStarTime[IVS_TIME_LEN];
	IVS_CHAR  szEndTime[IVS_TIME_LEN];
	IVS_CHAR  szSessionID[SESSION_ID_LEN];
}MEDIA_KEY_INFO;

/***************************************************************************
 �� �� �� : parseSdp
 �������� : ����SDP
 ������� : pSdpMsg                 ���������ַ���
 ������� : pMediaKeyInfo           ý����Կ��Ϣ
            pMediaInfo              ��ȡ����ý����������������SDK���·��
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
IVS_INT32 parseSdp(const char* pSdpMsg, MEDIA_KEY_INFO* pMediaKeyInfo,
						MEDIA_INFO_SDP* pMediaInfo,STORAGESECRET_INFO* pStorageSecretInfo);


/***************************************************************************
 �� �� �� : getNextLine
 �������� : �з�SDP��Ϣ
 ������� : strSdpMsg                   ���������ַ���
            nNextStart                  ƫ��ֵ
 ������� : strBuff                     ��ȡ������һ��SDP�ַ���
 �� �� ֵ : true �ɹ�           fail ʧ��
***************************************************************************/
bool getNextLine(const std::string &strSdpMsg,
                 unsigned int &nNextStart,
                 std::string  &strBuff);


/***************************************************************************
 �� �� �� : parseApplications
 �������� : ����ý�����ͣ���ȡTCP�˿ں�
 ������� : pAppliMsg                   ���������ַ���
 ������� : pPort                       ��ȡ����TCP�˿ں�
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseApplications(const char* pAppliMsg, unsigned long* pPort);


/***************************************************************************
 �� �� �� : parseDecoderType
 �������� : ������������
 ������� : pTmpMsg                     ���������ַ���
 ������� : pDecoderType                ��ȡ���ı�������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseDecoderType(const char* pTmpMsg, IVS_ULONG* pDecoderType);



/***************************************************************************
 �� �� �� : parseStreamID
 �������� : ����SessionID
 ������� : pSdpMsg                     ���������ַ���
 ������� : pSessionId                  ��ȡ����SessionID
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseStreamID(const char* pSdpMsg,  char* pSessionId);


/***************************************************************************
 �� �� �� : parseManuType
 �������� : ������������
 ������� : pTmpMsg                     ���������ַ���
 ������� : pManuType                   ��ȡ���ĳ�������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseManuType(char* pTmp, IVS_ULONG* ulManuType);


/***************************************************************************
 �� �� �� : parseTransSecretKey
 �������� : ������������������Կ
 ������� : pTmpMsg                     ���������ַ���
            ulKeyType                   ý������
������� : pMediaKeyInfo           ý����Կ��Ϣ
�� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

�ַ������ͣ�
 //a=crypto:1 HW_IVS_IPC_ENCRYPT_SIMPLE inline:d0RmdmcmVCspeEc3QGZiNWpVLFJhQX1cfHAwJSoj
***************************************************************************/
int parseTransSecretKey(const char* pTmp, IVS_ULONG ulKeyType, MEDIA_KEY_INFO* pMediaKeyInfo);

/***************************************************************************
 �� �� �� : parseRtpMap
 �������� : ��������Ƶ����
 ������� : pTmp                     ���������ַ���
 ������� : pMediaKeyInfo            ��ȡ������Ƶ����Ƶ������
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseRtpMap(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo);

/***************************************************************************
 �� �� �� : parseStorageSecretKey
 �������� : �����洢������Կ
 ������� : pTmpMsg                     ���������ַ���           
 ������� : pStorageSecretInfo          ��ȡ���洢������Ϣ
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

 �ַ������ͣ�
 a=cryptostorage:1 ASYM_RSA_2048_AES_CM_256 inline:����������Key������ 
***************************************************************************/
int parseStorageSecretKey(const char* pTmp,STORAGESECRET_INFO* pStorageSecretInfo);

/***************************************************************************
 �� �� �� : parseCerIndex
 �������� : �����洢����֤���
 ������� : pTmpMsg                     ���������ַ���           
 ������� : pStorageSecretInfo          ��ȡ���洢������Ϣ
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��

 �ַ������ͣ�
 b=cerindex:֤��������
***************************************************************************/
int parseCerIndex(const char* pTmp,STORAGESECRET_INFO* pStorageSecretInfo);

/***************************************************************************
 �� �� �� : parseMediaInfo
 �������� : ����ý����Ϣ
 ������� : pTmpMsg                     ���������ַ���
            ulKeyType                   ý������
 ������� : pAudioDecoderType           ��ȡ������Ƶ��������
            pVideoDecoderType           ��ȡ������Ƶ��������
            pPort                       ��ȡ���Ķ˿ں�
 �� �� ֵ : IVS_SUCCEED-�ɹ�           ����-ʧ��
***************************************************************************/
int parseMediaInfo(char* pTmp, IVS_ULONG* pKeyType, IVS_ULONG* pAudioDecoderType, 
				IVS_ULONG* pVideoDecoderType, IVS_ULONG* pPort);

/***************************************************************************
 �� �� �� : parseStarTimeAndEndTime
 �������� : ����ý��ý�忪ʼ����ʱ��
 ������� : pTmp                    ���������ַ���
 ������� : pMediaKeyInfo           ý����Կ��Ϣ

 �� �� ֵ : IVS_SUCCEED-�ɹ�        ����-ʧ��
***************************************************************************/
int parseStarTimeAndEndTime(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo);

/***************************************************************************
 �� �� �� : parseSessionID
 �������� : ����ý��ý��SessionID
 ������� : pTmp                    ���������ַ���
 ������� : pMediaKeyInfo           ý����Կ��Ϣ

 �� �� ֵ : IVS_SUCCEED-�ɹ�        ����-ʧ��
***************************************************************************/
int parseSessionID(const char* pTmp,MEDIA_KEY_INFO* pMediaKeyInfo);

#endif  // _IVS_SDP_H_


