/********************************************************************
    filename	: 	CumwInterface.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	���Ͷ���
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/
#ifndef CUMW_INTERFACE_H
#define CUMW_INTERFACE_H


#pragma pack(push,1)


////******************************�ӿ���س������忪ʼ*************************
#define CUMW_DEV_ID_LEN              20   //�豸ID�ĳ���
#define CUMW_DEV_NAME_LEN            256  //�豸������󳤶�

#define CUMW_CONTENT_ID_LEN          256  //����ID����󳤶�
#define CUMW_PATH_LEN                256  //·������󳤶�

#define CUMW_MSG_TYPE_LEN            32  //��Ϣ���͵���󳤶�
#define CUMW_MSG_DST_CODE_LEN        10  //������������󳤶�

//ý������ģʽ
typedef enum
{
    CUMW_LINK_MODE_TRANSMIT = 0,  //����MDU��ת
    CUMW_LINK_MODE_DIRECT = 1,    //ֱ��

    CUMW_LINK_MODE_MAX           //���ֵ��У����
} CUMW_LINK_MODE;


//ý�岥������
typedef enum
{
    CUMW_PLAY_TYPE_LOCAL   = 0,              //������ʾ
    CUMW_PLAY_TYPE_REMOTE  = 1,              //Զ����ǽ��ʾ
    CUMW_PLAY_TYPE_PREVIEW = 2,              //Զ����ǽԤ��
    CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT = 4,//Զ����ǽֱ����ƵԴ��������MDU��PU

    CUMW_PLAY_TYPE_MAX                      //���ֵ��У����
} CUMW_PLAY_TYPE;


//��Ƶ�豸����
typedef enum
{

    CUMW_AUDIO_TYPE_OUT = 0,   //��Ƶ���
    CUMW_AUDIO_TYPE_IN = 1,    //��Ƶ����

    CUMW_AUDIO_TYPE_MAX       //���ֵ��У����
} CUMW_AUDIO_TYPE;


//��Ƶ������
typedef enum
{
    CUMW_AUDIO_CTRL_CODE_CLOSE = 0,       //�ر���Ƶ�豸
    CUMW_AUDIO_CTRL_CODE_OPEN = 1,        //����Ƶ�豸
    CUMW_AUDIO_CTRL_CODE_SET_VOLUME = 2,  //��������

    CUMW_AUDIO_CTRL_CODE_MAX             //���ֵ��У����
} CUMW_AUDIO_CTRL_CODE;


//������Χֵ����
typedef enum
{
    CUMW_AUDIO_VOLUME_MIN = 0,    //������Сֵ
    CUMW_AUDIO_VOLUME_MAX = 100   //�������ֵ
} CUMW_AUDIO_VOLUME;


////******************************�ӿ���س����������*************************

////******************************ͨ�ýṹ�嶨�忪ʼ***************************


//�ṹ��������Ϣ
typedef struct
{
    unsigned long   ulArraySize;    //����Ԫ�ظ���
    void*           pArrayAddr;     //ָ��ĳ�ض��ṹ��������ָ�룬����Ԫ�ظ���ΪulNum
    unsigned long   ulArrayLen;     //pArrayAddr���ݵ�ʵ�ʴ�С
} CUMW_STRUCT_ARRAY;




////******************************ͨ�ýṹ�嶨�忪ʼ***************************

////******************************֪ͨ�ص���ض��忪ʼ*************************

//�첽�ӿ�����
typedef enum
{
    CUMW_ASYNC_TYPE_NO_RET_INFO = 0x0000,         //�޷�����Ϣ���첽�ӿ�
    CUMW_ASYNC_TYPE_START_MEDIA_BUSINESS = 0x0001,//��ʼý��ҵ���첽�ӿ�
                                                  //��Ӧ��ϢΪ: �ޡ�ҵ��ID��Ϣͨ��ulBusinessID�ֶη���

    CUMW_ASYNC_TYPE_INTERFACE_MAX = 0x8000        //�ӿ����ֵ
} CUMW_ASYNC_TYPE;



//ͨ�ô�����Ӧ�ϱ��ṹ����Ϣ
typedef struct
{
    ULONG            ulMsgSeq;                            //��Ϣ���к�
    ULONG            ulResultCode;                        //�����룬��������
    char             pMsgType[CUMW_MSG_TYPE_LEN];         //��Ϣ���ͣ��ַ���
    char             pDstCode[CUMW_MSG_DST_CODE_LEN];     //pArrayAddr���ݵ�ʵ�ʴ�С
} COMMON_RETURN_STRUCT; 


//Զ�̽�����״̬֪ͨ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_MEDIA_DATA
typedef struct
{
    unsigned long   ulBusinessID;   //ҵ��ID
    unsigned long   ulDataType;     //ý����������
    char*           pDataBuf;       //ָ��ý�����ݵ�ָ��
    unsigned long   ulDataBufLen;   //ý�����ݣ���pDataBuf��ָ���ݣ��ĳ���
    unsigned long   ulVideoWidth;   //��Ƶ���
    unsigned long   ulVideoHeight;  //��Ƶ�߶�
} CUMW_NOTIFY_MEDIA_DATA;

//Զ�̽�����ע��֪ͨ
//��Ӧ��CUMW_NOTIFY_TYPEΪ: CUMW_NOTIFY_TYPE_REMOTE_DECODER_REGISTER
typedef struct
{
    char*  pszDecoderInfo;          //������ע����Ϣ������XML��ʽ
    unsigned long ulLen;            //���ݳ���
} CUMW_NOTIFY_REMOTE_DECODER_REGISTER_INFO;

////******************************֪ͨ�ص���ض������*************************

////******************************�ӿڽṹ����ض��忪ʼ***********************


//�첽�ӿڲ�������ͷ
typedef struct
{
    unsigned long ulStructLen;    //�ṹ�屾����
    unsigned long ulAsyncSeq;     //�첽�������
} IPI_ASYNC_HEADER;


//��ʼ���м���ӿڲ���
typedef struct
{
    unsigned long           ulClientType;       //�ͻ������ͣ��μ�CUMW_CLIENT_TYPE
    PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack; //֪ͨ�ص�����ָ��
    void*                   pCallBackUserData;  //֪ͨ�ص��������û�����ָ��
} IPI_INIT_CUMW;


//���Ʊ�����Ƶ
typedef struct
{
    unsigned long ulAsyncSeq;     //�첽�������
    unsigned long ulAudioType;    //��Ƶ���ͣ�����μ�CUMW_AUDIO_TYPE
    unsigned long ulCtrlCode;     //��Ƶ�����룬����μ�CUMW_AUDIO_CTRL_CODE
    unsigned long ulVolume;       //������ȡֵ��ΧΪ0-100������0%��100%��ulCtrlCodeΪ�����������μ���Ƶ�����룩ʱ��Ч
} IPI_CTRL_LOCAL_AUDIO;

////******************************�ӿڽṹ����ض������***********************


#pragma pack(pop)


#endif //CUMW_INTERFACE_H
