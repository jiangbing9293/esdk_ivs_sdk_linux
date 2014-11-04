/********************************************************************
filename	: 	IVS_SDKTVWall.h
author		:	g00209332
created		:	2012/12/21	
description	:	SDK�����ṩ����ǽ��ؽӿڶ���
                ��Ϊ����ǽ�ӿڲ����⿪�ţ�ֻ��OCXʹ�ã����Բ�����IVS_SDK.h��
copyright	:	Copyright (C) 2011-2015
history		:	2012/12/21 ��ʼ�汾;
*********************************************************************/
#ifndef __IVS_SDK_TVWALL_H__
#define __IVS_SDK_TVWALL_H__

#include "hwsdk.h"

#ifdef WIN32
#ifdef  IVS_SDK_EXPORTS
#define IVS_SDK_API __declspec(dllexport)
#else
#define IVS_SDK_API __declspec(dllimport)
#endif
#else
//#define IVS_SDK_API __attribute__((visibility))
#define IVS_SDK_API __attribute__((visibility("default")))
#endif

#define CUMW_SESSION_ID_LEN          512  //�ỰID����󳤶�
#define CUMW_SESSION_URL_LEN         512  //�ỰUrl����󳤶�
#define CUMW_ERROR_DESCRIPTION_LEN   512  //�������Ӧ������������󳤶�
#define CUMW_DECODER_ID_LEN          20   //������ID�ĳ���
#define CUMW_DECODER_NAME_LEN        32   //������ͨ�����Ƴ���
#define CUMW_DECODER_ENCODING_LEN    16   //�����������ʽ��󳤶�
#define CUMW_DECODER_CHANNEL_MAX_NUM 36   //������ͨ���������ֵ
#define CUMW_CHANNEL_NAME_LEN        32   //������ͨ�����Ƴ���
#define CUMW_PAY_LOAD_TYPE_NUM       16   //PayLoadType��������

//TVWall���ش����� (���������λ��ţ�OCX 000��199 SDK 200��399 MP 400��599 TVWALL 600~799)
typedef enum
{
	IVS_TVWALL_MEMORY_NULL = 999115600,			      //ָ��Ϊ��
	IVS_TVWALL_CREATE_THREAD_FAIL = 999115601,        //�����߳�ʧ��
	IVS_TVWALL_BUSINESS_ID_NO_EXIST = 999115602,      //ҵ��ID������
	IVS_TVWALL_SEND_FAIL = 999115603,                 //������Ϣʧ��

	IVS_TVWALL_INIT_NO = 999115607,                   //��δ��ʼ��
	IVS_TVWALL_INIT_ALREADY = 999115608,              //�Ѿ���ʼ��
	IVS_TVWALL_TV_WALL_SRC_RUNING = 999115609,        //����ǽ�����Ѿ�����
	IVS_TVWALL_TV_WALL_SRC_STOPED = 999115610,	      //����ǽ������δ����
	IVS_TVWALL_DECODER_OFFLINE = 999115611,           //������������
	IVS_TVWALL_DECODER_NO_CHANNEL = 999115612,        //������û�и�ͨ��
	IVS_TVWALL_DECODER_INIT_CHANNEL_FAIL = 999115613, //��ʼ��������ͨ��ʧ��
	IVS_TVWALL_STOP_FLAG = 999115614                  //����������ǽ�Ĺ����У��յ�ֹͣ������ڲ�ʹ�ã�
} IVS_TVWALL_ERR_CODE;


//��Ƶ������Э��
enum RDA_PROTOCOL
{
	RDA_PROTOCOL_NONE = 0x00,
	RDA_PROTOCOL_NSS_OVER_UDP = 0x02, //NSS Over UDP
	RDA_PROTOCOL_NSS_OVER_TCP = 0x03, //NSS Over TCP
	RDA_PROTOCOL_RTP_OVER_UDP = 0x04, //RTP Over UDP
	RDA_PROTOCOL_RTP_OVER_TCP = 0x05, //RTP Over TCP
	RDA_PROTOCOL_MAX  = 0x06 //���ֵ
};

//ý���������Ϣ
typedef struct
{
	unsigned short usTcpInterleavedPort;       //TCP��֯�˿�
	unsigned short usUdpVideoRtpPort;          //UDP��ƵRTP�˿�
	unsigned short usUdpVideoRtcpPort;         //UDP��ƵRTCP�˿�
	unsigned short usUdpAudioRtpPort;          //UDP��ƵRTP�˿�
	unsigned short usUdpAudioRtcpPort;         //UDP��ƵRTCP�˿�

	char          szMediaIP[IVS_IP_LEN];//��������ý��ͨ��IP
	unsigned char ucTransportProtocol;         //��Ƶ����Э��
} CUMW_MEDIA_NET_INFO;

//�����ʼ����Ϣ
typedef struct
{
	unsigned long  ulAsyncSeq;                          //�첽�������
	unsigned short usChannelNo;                         //ͨ�����
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��
	unsigned char  ucTransportProtocol;                 //��Ƶ����Э��
} IPI_INIT_BUSINESS_REQ;


//����Զ�̽�������Ϣ
typedef struct
{
	unsigned long  ulAsyncSeq;                          //�첽�������
	unsigned long  ulBusinessID;
	unsigned short usChannelNo;                         //ͨ�����

	char           szSessionId[CUMW_SESSION_ID_LEN];    //�ỰID
	char           szSessionUrl[CUMW_SESSION_URL_LEN];  //�ỰURL
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��

	unsigned char  ucVideoPayloadType;                  //��ƵPayload����
	unsigned char  ucAudioPayloadType;                  //��ƵPayload����

	CUMW_MEDIA_NET_INFO stMediaNetInfo;                 //ý���������Ϣ
} IPI_START_BUSINESS_REQ;

//ֹͣ����Ƶ������Ϣ
typedef struct
{
	unsigned long  ulAsyncSeq;                          //�첽�������
	unsigned long  ulBusinessID;
	unsigned short usChannelNo;                         //ͨ�����
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��
} IPI_STOP_BUSINESS_REQ;

//����/ֹͣ������Ƶ��Ϣ
typedef struct
{
	unsigned long  ulAsyncSeq;                          //�첽�������
	unsigned long  ulBusinessID;
	unsigned short usChannelNo;                         //ͨ�����
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��
	unsigned char  cAudioCmd;                           //0x00 ֹͣ���� ��0x01��ʼ����
} IPI_AUDIO_CONTROL_REQ;

//����������Ϣ
typedef struct
{
	unsigned long  ulAsyncSeq;                          //�첽�������
	unsigned long  ulBusinessID;
	unsigned short usChannelNo;                         //ͨ�����
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��
	unsigned char  cVolume;                             //������С��0~100��
} IPI_TUNEUP_VOLUME_REQ;

/*********************֪ͨ�ص���ض���***********************************/
//֪ͨ���Ͷ���
typedef enum
{
	CUMW_NOTIFY_TYPE_EVENT = 0x0001,                     //�¼�֪ͨ
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_EVENT
	CUMW_NOTIFY_TYPE_ASYNC_RET = 0x0002,                 //�첽����֪ͨ
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_ASYNC_RET
	CUMW_NOTIFY_TYPE_REMOTE_DECODER_REGISTER = 0x0003,   //Զ�̽�����ע��֪ͨ
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_REMOTE_DECODER_STATUS
	CUMW_NOTIFY_TYPE_REMOTE_DECODER_UNRIGISTER = 0x0004, //Զ�̽�����ע��
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO
	CUMW_NOTIFY_TYPE_REMOTE_DECODER_DISCONNECT = 0x0005, //Զ�̽���������
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO
	CUMW_NOTIFY_TYPE_INIT_DECODER = 0x0006,              //��ʼ��������
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_INIT_DECODER_INFO
	CUMW_NOTIFY_TYPE_START_DECODER = 0x0007,             //����Զ�̽�����
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_START_STOP_DECODER_INFO
	CUMW_NOTIFY_TYPE_DECODER_ERROR = 0x0008,             //������֪ͨ����
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_TYPE_DECODER_ERROR_INFO
	CUMW_NOTIFY_TYPE_AUDIO_CONTROL = 0x0009,             //����/ֹͣ������Ƶ��Ϣ֪ͨ
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_COM_RES_INFO
	CUMW_NOTIFY_TYPE_SET_VOLUME = 0x000A,                //����������Ϣ֪ͨ
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_COM_RES_INFO
	CUMW_NOTIFY_TYPE_STOP_DECODER = 0x000B,              //ֹͣ����
	//��ӦpNotifyInfo�ṹ��Ϊ: CUMW_NOTIFY_START_STOP_DECODER_INFO
	CUMW_NOTIFY_TYPE_START_AUTO_RESUME_INFO = 0x000C,              

	CUMW_NOTIFY_TYPE_AUTO_RESUME_RES = 0x000D,            

	//RTSPERROR
	CUMW_NOTIFY_TYPE_RTSP_EXCEPTION = 0x000E,

	CUMW_NOTIFY_TYPE_INTERFACE_MAX = 0x8000              //�ӿ����ֵ
} CUMW_NOTIFY_TYPE;

//����������
typedef enum
{
	CUMW_DECODER_TYPE_RDA = 0,			//RDA������
	CUMW_DECODER_TYPE_SDK = 1			//SDK������
} CUMW_DECODER_TYPE;

//�¼�֪ͨ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_EVENT
typedef struct
{
	unsigned long ulEventType;    //�¼����ͣ�ȡֵ��CUMW_NOTIFY_EVENT_TYPE
	unsigned long ulEventPara;    //�¼�����
} CUMW_NOTIFY_EVENT;

//�첽����֪ͨ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_ASYNC_RET
//TVWall->SDK
typedef struct
{
	unsigned long   ulAsyncType;        //�첽�ӿ����ͣ�ȡֵ��CUMW_ASYNC_TYPE
	unsigned long   ulAsyncSeq;         //�첽�������
	unsigned long   ulAsyncRetCode;     //�첽�����룬ȡֵ��CUMW_RET_CODE
	unsigned long   ulAsyncRetReason;   //������ϸԭ��û����ϸԭ�����0.������ο������ĵ�
	unsigned long   ulBusinessID;       //ҵ��ID���첽�ӿ�����Ϊ��ʼý��ҵ����첽�ӿ�ʱ����Ч
	void*           pAsyncRetInfo;      //�첽������Ϣ�������������ulAsyncType��ȡֵ��CUMW_ASYNC_TYPE
	unsigned long   ulAsyncRetInfoLen;  //pAsyncRetInfo���ݵ�ʵ�ʳ���
} CUMW_NOTIFY_ASYNC_RET;
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_ASYNC_RET
//SDK->OCX
typedef enum
{
	CUMW_BREAKDOWN_REASON_UNKNOWN = 0,				//δ֪ԭ��
	CUMW_BREAKDOWN_REASON_DECODER_OFFLINE = 1,		//����������
	CUMW_BREAKDOWN_REASON_NO_VIDEO = 2				//��Ƶ�ж�
}CUMW_BREAKDOWN_REASON;

typedef struct
{
	IVS_INT32 iRetCode;                                 //�첽����������
	IVS_CHAR szCameraCode[IVS_DEV_CODE_LEN+1];          //����ͷ����
	IVS_CHAR szDecoderID[IVS_TVWALL_DECODER_CODE_LEN+1];//������ID
	IVS_UINT32 uiChannel;                               //������ͨ����
	CUMW_BREAKDOWN_REASON iReason;						//�쳣ԭ��
} CUMW_NOTIFY_ASYNC_RET_EX;


//������ͨ����Ϣ
typedef struct
{
	unsigned long ulChannelID;                             //������ͨ����
	char          szChannelName[CUMW_CHANNEL_NAME_LEN + 1];//������ͨ������
	unsigned char ucPortMode;							   //ͨ����Ӧ�˿�ģʽ��SDK�������иù��ܣ� 1-1���� 2-2���� 4-4���� 9-9���� 16-16����
	unsigned char ucChannelType;						   //ͨ������ 0-���� 1-���
} CUMW_DECODER_CHANNEL_INFO;

//Զ�̽�����״̬֪ͨ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_REMOTE_DECODER_STATUS
typedef struct
{
	unsigned char			  ucDecoderType;								   //���������� 0-RDA������ 1-SDK������
	unsigned long             ulDecoderStatus;                                 //������״̬������μ�CUMW_DECODER_STATUS
	unsigned long             ulVideoPayloadTypeNum;                           //֧�ֵ���ƵPayloadType���������ֵ������16
	unsigned long             ulAudioPayloadTypeNum;                           //֧�ֵ���ƵPayloadType���������ֵ������16
	unsigned long             ulChannelNum;                                    //��������Чͨ����������󲻳���CUMW_DECODER_CHANNEL_MAX_NUM
	char                      szDecoderID[CUMW_DECODER_ID_LEN + 1];            //������ID
	char                      szDecoderName[CUMW_DECODER_NAME_LEN + 1];        //����������
	char				      szDecoderEncoding[CUMW_DECODER_ENCODING_LEN + 1];//�����������ʽ 
	char                      szDecoderIP[IVS_IP_LEN];                        //������IP
	unsigned char             ucVideoPayloadTypeArray[CUMW_PAY_LOAD_TYPE_NUM]; //֧�ֵ���ƵPayloadType�̶����顣Ԫ�ظ���Ϊ16��ʵ��Ԫ�ظ���ΪulVideoPayloadTypeNum
	unsigned char             ucAudioPayloadTypeArray[CUMW_PAY_LOAD_TYPE_NUM]; //֧�ֵ���ƵPayloadType�̶����顣Ԫ�ظ���Ϊ16��ʵ��Ԫ�ظ���ΪulAudioPayloadTypeNum
	CUMW_DECODER_CHANNEL_INFO stChannelInfoArray[CUMW_DECODER_CHANNEL_MAX_NUM];//������ͨ����Ϣ������Ԫ�ظ���ΪulChannelNum
} CUMW_NOTIFY_REMOTE_DECODER_STATUS;

typedef struct
{
    IVS_UINT32   uiTotal;                                   // �ܼ�¼��
    IVS_INDEX_RANGE stIndexRange;                           // ��ҳ��Ϣ  
    CUMW_NOTIFY_REMOTE_DECODER_STATUS stDecoderList[1];     // �������б�
} CUMW_NOTIFY_REMOTE_DECODER_STATUS_LIST;

//Զ�̽�����������ע��֪ͨ�ṹ��
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_REMOTE_DECODER_DISCONNECT��CUMW_NOTIFY_TYPE_REMOTE_DECODER_UNRIGISTER
typedef struct
{
	char   szDecoderID[CUMW_DECODER_ID_LEN + 1]; //������ID
} CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO;


//Զ�̽��������صĲ����������Ϣ
typedef struct
{
	unsigned long ulErrorCode;                                   //�����룬0����ɹ�
	char          szErrorDescription[CUMW_ERROR_DESCRIPTION_LEN];//�������Ӧ��������
}CUMW_ERROR_CODE_INFO;

//��������ʼ��֪ͨ��Ϣ�ṹ��
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_INIT_DECODER
typedef struct
{
	unsigned long ulAsyncSeq;                          //�첽�������
	unsigned long ulBusinessID;                         
	char          szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��

	CUMW_MEDIA_NET_INFO  stMediaNetInfo;               //ý���������Ϣ
	CUMW_ERROR_CODE_INFO stErrorCode;                  //������Ϣ
} CUMW_NOTIFY_INIT_DECODER_INFO;


//����Զ�̽�������ֹͣԶ�̽�����֪ͨ��Ϣ�ṹ��
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_START_DECODER��CUMW_NOTIFY_TYPE_STOP_DECODER
typedef struct
{
	unsigned long ulAsyncSeq;                          //�첽�������
	unsigned long ulBusinessID;
	char          szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��

	CUMW_ERROR_CODE_INFO stErrorCode;                  //������Ϣ
}CUMW_NOTIFY_START_STOP_DECODER_INFO;

//������֪ͨ�������Ϣ�ṹ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_DECODER_ERROR
typedef struct
{
	unsigned short usChannelNo;                         //ͨ�����
	char           szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��

	CUMW_ERROR_CODE_INFO stErrorCode;                   //������Ϣ
} CUMW_NOTIFY_TYPE_DECODER_ERROR_INFO;


//ͨ��֪ͨ��Ϣ�ṹ��
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_SET_VOLUME��CUMW_NOTIFY_TYPE_AUDIO_CONTROL
typedef struct
{
	unsigned long ulAsyncSeq;                          //�첽�������
	unsigned long ulBusinessID;
	char          szDecoderID[CUMW_DECODER_ID_LEN + 1];//������ID��20λ��������\0��

	CUMW_ERROR_CODE_INFO stErrorCode;                  //������Ϣ
}CUMW_NOTIFY_COM_RES_INFO;


//֪ͨ�ص�������֪ͨ��Ϣ�ṹ��
typedef struct
{
	unsigned long   ulNotifyType;       //֪ͨ���ͣ�ȡֵ��CUMW_NOTIFY_TYPE
	void*           pNotifyInfo;        //֪ͨ��Ϣ�������������ulNotifyType��ȡֵ��CUMW_NOTIFY_TYPE
	unsigned long   ulNotifyInfoLen;    //pNotifyInfo���ݵ�ʵ�ʳ��ȣ�������ȷ
} CUMW_NOTIFY_INFO;

//֪ͨ�ص���������
typedef IVS_INT32 (__SDK_CALL * PCUMW_CALLBACK_NOTIFY)(CUMW_NOTIFY_INFO* pstNotifyInfo,void* pUserData);

#ifdef __cplusplus
extern "C"
{
#endif
	/*************************************************************************
	* name       : IVS_SDK_InitTVWall
	* description: ��ʼ������ǽ
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	*			   pFunNotifyCallBack �ص�������ַ, 
	*			   pUserData �ص��������ݣ�����ΪNULL
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��gaoshuming g00209332
	* remark     : NA
	* history    : 2012-12-20 17.28[add]
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_InitTVWall(IVS_INT32 iSessionID, PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, 
														void* pUserData);

	/*************************************************************************
	* name       : IVS_SDK_CleanupTVWall
	* description: �ͷŵ���ǽ
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��gaoshuming g00209332
	* remark     : NA
	* history    : 2012-12-20 17.28[add]
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CleanupTVWall(IVS_INT32 iSessionID);

	/**********************************************************************
	* name			: IVS_SDK_GetDecoderList 
	* description	: ��ȡ�������б�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDecoderList(IVS_INT32 iSessionID,
															const IVS_INDEX_RANGE* pIndexRange,
															CUMW_NOTIFY_REMOTE_DECODER_STATUS_LIST* pDecoderList,
															IVS_UINT32 uiBufferSize);
	/**********************************************************************
	* name			: IVS_SDK_StartRealPlayTVWall
	* description	: ��������ǽʵʱ���
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pCameraCode ���������
	*				  pRealplayParam ʵʱ�������XML
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayTVWall(IVS_INT32 iSessionID,
																const IVS_CHAR* pCameraCode,
																const IVS_REALPLAY_PARAM* pRealplayParam,
																const IVS_TVWALL_PARAM* pTVWallParam);

	/**********************************************************************
	* name			: IVS_SDK_StopRealPlayTVWall
	* description	: ֹͣ����ǽʵʱ���
	* input			: pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayTVWall(IVS_INT32 iSessionID,
																const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StartPlatformPlayBackTVWall
	* description	: ��ʼ����ǽƽ̨¼��ط�
    * input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pCameraCode ���������
    *				  pPlaybackParam ¼��طŲ���XML
    *				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackTVWall (IVS_INT32 iSessionID,
                                                                            const IVS_CHAR* pCameraCode,
                                                                            const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                            const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StopPlatformPlayBackTVWall
	* description	: ֹͣ����ǽƽ̨¼��ط�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StartPUPlayBackTVWall
	* description	: ��ʼ����ǽǰ��¼��ط�
    * input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pCameraCode ���������
    *				  pPlaybackParam ¼��طŲ���XML
    *				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUPlayBackTVWall (IVS_INT32 iSessionID,
                                                                            const IVS_CHAR* pCameraCode,
                                                                            const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                            const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StopPUPlayBackTVWall
	* description	: ֹͣ����ǽǰ��¼��ط�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUPlayBackTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StartBackupPlayBackTVWall
	* description	: ��ʼ����ǽ����¼��ط�
    * input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pDomainCode �����
	*				  pNVRCode    NVR����
	*				  pCameraCode ���������
    *				  pPlaybackParam ¼��طŲ���XML
    *				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartBackupPlayBackTVWall (IVS_INT32 iSessionID,
																			const IVS_CHAR* pDomainCode,
																			const IVS_CHAR* pNVRCode,
                                                                            const IVS_CHAR* pCameraCode,
                                                                            const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                            const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_StopBackupPlayBackTVWall
	* description	: ֹͣ����ǽ����¼��ط�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopBackupPlayBackTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_PlayBackPauseTVWall
	* description	: ��ͣ����ǽ¼��ط�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackPauseTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_PlayBackResumeTVWall
	* description	: �ָ�����ǽǰ��¼��ط�
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackResumeTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/**********************************************************************
	* name			: IVS_SDK_PlaybackFrameStepForwardTVWall
	* description	: �ط�ʱ��֡���
	* input			: iSessionID   ��¼�ɹ���ĻỰID
	*				  pTVWallParam ����ǽ����������
	* output     	: NA
	* return     	: �ɹ�-0 ʧ��-������ 
	* remark     	: NA   
	*********************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlaybackFrameStepForwardTVWall(IVS_INT32 iSessionID,
                                                                        const IVS_TVWALL_PARAM* pTVWallParam);
	/*************************************************************************
	* name       : IVS_SDK_AddCallBackTVWall
	* description: ���ӵ���ǽ�¼��ص�
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	*			   pFunNotifyCallBack �ص�������ַ, 
	*			   pUserData �ص��������ݣ�����ΪNULL
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddCallBackTVWall(IVS_INT32 iSessionID, PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, 
																void* pUserData);
	/*************************************************************************
	* name       : IVS_SDK_RemoveCallBackTVWall
	* description: �Ƴ�����ǽ�¼��ص�
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	*			   pUserData �ص��������ݣ�����ΪNULL
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RemoveCallBackTVWall(IVS_INT32 iSessionID, void* pUserData);
#ifdef __cplusplus
}
#endif //__cplusplus

#endif //__IVS_SDK_TVWALL_H__
