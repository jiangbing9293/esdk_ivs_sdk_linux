/********************************************************************
 filename    :    MediaBase.h
 author      :    s00191067
 created     :    2012/11/19
 description :    ý�����;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*********************************************************************/
#ifndef __MEDIA_BASE_H__
#define __MEDIA_BASE_H__

/*!< ������ͷ�ļ� */
#include "vos.h"
#include "ToolsHelp.h"
#include "UserInfo.h"
#include "rtsp_client_api.h"
#include "RtspRetCode.h"
#include "IVS_NetSource.h"
#include "IVSPlayerDataType.h"
#include "SDKDef.h"
#include "ParseSdp.h"
#include "bp_environments.h"




//#define MLIB_TIMEOUT                        (20*1000)      // ���峬ʱʱ��20S

//typedef struct  
//{
//	int iExceptionCode;
//	unsigned long ulProtocolType;
//	unsigned long ulReseved1;
//	unsigned long ulReseved2;
//}SOURCE_EXCEPTION_DATA;
////�����YUV��Ϣ
//typedef struct ST_DECODE_YUV_INFO
//{
//	IVS_INT64 iTimeStamp;//ʱ���
//	IVS_INT32 iVideoWidth; //��Ƶ����
//	IVS_INT32 iVideoHeigth; //��Ƶ�߶�
//}DECODE_YUV_INFO;
enum SourcePlayType
{
	PLAY_TYPE_HWND = 0,         //��Ⱦ�����;
	PLAY_TYPE_CB,               //��ȡƴ֡�����
	PLAY_TYPE_YUV               //��ȡ��������
};

typedef enum
{
	MEDIA_STATUS_BUSY = 0,
	MEDIA_STATUS_FREE
}MEDIA_STATUS;

typedef struct tagStartPlayParam
{
    SOURCE_TYPE         enSourceType;
    IVS_SERVICE_TYPE    enServiceType;
    float               fSpeed;
    fDataCallBack       cbRtspDataCallBack;
    NET_DATA_CALLBACK   cbNetDataCallBack;

    tagStartPlayParam()
        : enSourceType(SOURCE_RTP)
        , enServiceType(SERVICE_TYPE_REALVIDEO)
        , fSpeed(1.0)// Ĭ����1���ٵķ�ʽ����;
        , cbRtspDataCallBack(NULL)
        , cbNetDataCallBack(NULL)
    {}
}START_PLAY_PARAM;

class CUserMgr;
class CMediaBase
{
public:
	CMediaBase(void);
	virtual ~CMediaBase(void);

public:
    virtual void RetSet();

/**************************************************************************
    * name       : SetWaterMarkVerify
	* description: ���ÿ���/ֹͣУ��ˮӡ
	* input      : ulHandle          ҵ����������ʵ�����ط�
				   bIsStart          1������ˮӡ 0��ֹͣˮӡ
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * author     ��chenxianxiao00206592
	* remark     : NA
	* history    : 2012-11-23
**************************************************************************/
	IVS_INT32 SetWaterMarkVerify(IVS_UINT32 uiChannel, IVS_BOOL bIsStart) const;

/**************************************************************************
    * name       : SetPlayQualityMode
	* description: ����ʵ������ģʽ������������
	* input      : ulHandle          ҵ����������ʵ�����ط�
	               iPlayQualityMode  ����ģʽ �ο���IVS_PLAY_QUALITY_MODE
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * author     ��chenxianxiao00206592
	* remark     : NA
	* history    : 2012-11-23
**************************************************************************/
	IVS_INT32 SetPlayQualityMode(IVS_UINT32 uiChannel,IVS_INT32 iPlayQualityMode) const;

 	/**************************************************************************
    * name       : GetMediaInfo
    * description: ��ȡ��Ƶ��������
    * input      : ulHandle          ҵ����������ʵ�����ط�
    * output     : pMediaInfo        ý����Ϣ
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
	IVS_INT32 GetMediaInfo(IVS_UINT32 uiChannel,IVS_MEDIA_INFO* pMediaInfo) const;

/**************************************************************************
    * name       : GetPlayedTime
    * description: ��ȡ�Ѳ���ʱ�䡣
	* input      : ulHandle           ҵ����������ʵ�����ط�
	* output     : puPlayedTime       ��ǰ����ʱ��
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * author     ��chenxianxiao00206592
	* remark     : NA
	* history    : 2012-11-23
**************************************************************************/
	IVS_INT32 GetPlayedTime(IVS_UINT32 uiChannel,IVS_UINT32 *puPlayTime) const;

/**************************************************************************
    * name       : SetDisplayScale
    * description: ���ò�����ʾ���ʡ�
	* input      : uiChannel           �ŵ�
	* output     : iDisplayScale       ��ʾ����
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * author     ��chenxianxiao00206592
	* remark     : NA
	* history    : 2012-11-23
**************************************************************************/
	IVS_INT32 SetDisplayScale(IVS_UINT32 uiChannel, IVS_UINT32 iDisplayScale) const;

	void SetUserMgr(CUserMgr* pUserMgr);

	IVS_INT32 GetStatus();
	void SetStatus(IVS_INT32 iStatus);

    /*****************************************************************************
     �������ƣ�IsReuseRtspConnection
     �������������ݷ������ͺ�Э�������ж��Ƿ���rtsp����;
     ���������enServiceType  ��������;
               enProtocolType Э������;
     ���������NA
     �� �� ֵ��true false;
    *****************************************************************************/
    bool IsReuseRtspConnection(IVS_SERVICE_TYPE enServiceType, IVS_PROTOCOL_TYPE enProtocolType, 
                               IVS_MEDIA_TRANS_MODE enTransMode, IVS_BROADCAST_TYPE enBroadcastType);

    /******************************************************************
     function   : StartPlay
     description: ��������;
     input      : const START_PLAY_PARAM & stParam
     output     : NA
     return     : int
    *******************************************************************/
    virtual int StartPlay(const START_PLAY_PARAM& stParam);

	virtual int StartDownload(const START_PLAY_PARAM& stParam, const char *pCam, const IVS_DOWNLOAD_PARAM *pParam);


    /******************************************************************
     function   : ReStartStream
     description: ʵ������������;
     input      : const START_PLAY_PARAM & stParam
     output     : NA
     return     : int
    *******************************************************************/
	virtual int ReStartStream(const START_PLAY_PARAM& stParam);

    /******************************************************************
     function   : StopStream
     description: ʵ����ֹͣ
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
	virtual int StopStream();

   /******************************************************************
     function   : StopPlay
     description: ʵ��ֹͣ
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/ 
    virtual int StopPlay();

	/******************************************************************
     function   : StopDownLoad
     description: ֹͣ����
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/ 
	virtual int StopDownLoad();

    /******************************************************************
     function   : PlaySound
     description: ������Ƶ����
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
    virtual int PlaySound();


    /******************************************************************
     function   : StopSound
     description: �ر���Ƶ����
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
    virtual int StopSound();

    /******************************************************************
     function   : SetVolume
     description: ��������
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
    virtual int SetVolume(unsigned int uiVolumeValue);

    /******************************************************************
     function   : GetVolume
     description: ��ȡ����
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
    virtual int GetVolume(unsigned int* puiVolumeValue);

    /******************************************************************
     function   : NotifyStreamBufferState
     description: ֪ͨ��������״̬
     input      : IVS_INT32 iBufferState
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    virtual IVS_INT32 NotifyStreamBufferState(IVS_INT32 iBufferState);

    /******************************************************************
     function   : StartLocalRecord
     description: ��ʼ����¼��
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
	int StartLocalRecord(const IVS_LOCAL_RECORD_PARAM* pRecordParam,const IVS_CHAR* pSaveFileName);

    /******************************************************************
     function   : StopLocalRecord
     description: ֹͣ����¼��
     input      : NA
     output     : NA
     return     : int
    *******************************************************************/
	int StopLocalRecord();

    /******************************************************************
     function   : LocalSnapshot
     description: ����ץ�ĵ���ͼƬ
	 input      : pSavePath			ͼƬ����·����
				  ulSnapshotFormat	ͼƬ��������
     output     : NA
     return     : int
    *******************************************************************/
	int LocalSnapshot(const char *pSavePath, IVS_ULONG ulSnapshotFormat);
	int LocalSnapshotRect(const char *pSavePath, IVS_ULONG ulSnapshotFormat, const IVS_RECT* pRect);


    /******************************************************************
     function   : SetDrawCallBack
     description: ���û�ͼ�ص�
     input      : callback;pUserData
     output     : NA
     return     : int
    *******************************************************************/
#ifdef WIN32
	int SetDrawCallBack(DRAW_PROC callback, void *pUserData);

	int SetDrawCallBackEx(HWND hWnd, DRAW_PROC callback, void *pUserData);
#endif

	int SetDelayFrameNum(IVS_UINT32 uDelayFrameNum);
	int GetCameraName(IVS_ULONG ulNameLen, char *pCamName);


    /******************************************************************
     function   : UTCStr2Time
     description: ��UTCʱ�䴮��ʽ��YYYY,MM,DD,hh,mm,ss�ֶ�ת�����������Ϊ��λ������ʱ��,
                  ���Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00����������������
     input      : const char * pStr     ��ת�����ַ���
     input      : const char * pFormat  �ַ�����Ӧ�ĸ�ʽ;
     output     : NA
     return     : time_t ת�����ʱ��
    *******************************************************************/
    static time_t UTCStr2Time(const char *pStr, const char *pFormat);

    /******************************************************************
     function   : UTCTime2Str
     description: ��UTCʱ�䴮��ʽ��YYYY,MM,DD,hh,mm,ss�ֶ�ת�����������Ϊ��λ������ʱ��,
                  ���Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00����������������
     input      : const char * pStr     ��ת�����ַ���
     input      : const char * pFormat  �ַ�����Ӧ�ĸ�ʽ;
     output     : NA
     return     : time_t ת�����ʱ��
    *******************************************************************/
    static void UTCTime2Str(const time_t t, std::string& strTime, const std::string& strFormat);

	/******************************************************************
	 function   : GetRtspErrorCode
	 description: ��rtsp client�Ĵ�����ת��Ϊivs������;
	 input      : int iErrorCode
	 output     : NA
	 return     : int ������;
	*******************************************************************/
	static int GetRtspErrorCode(int iErrorCode);

	/******************************************************************
	function	: SetSecretKey
	description : ������Կ��Ϣ;
	input		: uiChannel   ҵ����
				  stMediaKeyInfo   ������Ϣ
	output		: NA
	return		: �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 SetSecretKey(IVS_UINT32 uiChannel,MEDIA_KEY_INFO& stMediaKeyInfo);

    IVS_INT32 SetNoNeedSecretKey(IVS_UINT32 uiChannel, MEDIA_KEY_INFO& stMediaKeyInfo);

	/******************************************************************
	function	: GetStreamencryKey
	description : ����¼����ܵĹ�����Կ�����������
	input		: stMediaKeyInfo ý�������Ϣ
	output		: strSecretKey ��Կ 
	return		: �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 GetStreamencryKey(IVS_CHAR* cSecretKey,const MEDIA_KEY_INFO& stMediaKeyInfo) const;

	/******************************************************************
	 function   : InitMediaPara
	 description: ��ʼ��MediaPara
	 input      : IVS_MEDIA_PARA & stMediaPara
	 output     : NA
	 return     : void
	*******************************************************************/
	void InitMediaPara(IVS_MEDIA_PARA& stMediaPara) const;

	/******************************************************************
	 function   : GetSourceBufferRemain
	 description: ��ȡδ�������ݵĸ���;
	 input      : IVS_UINT32 & uBufferCount
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetSourceBufferRemain(IVS_UINT32 &uBufferCount);





	IVS_INT32 SetMeidaAttribute(const MEDIA_ATTR &VideoAttr, const MEDIA_ATTR &AudioAttr);//lint !e601







private:

    /******************************************************************
     function   : ProcessRtspPlayMsg
     description: ����rtsp��play����;
     input      : const char * pszURL
     input      : const MEDIA_ADDR & stServerMediaAddr
     input      : const START_PLAY_PARAM & stParam
     output     : NA
     return     : int
    *******************************************************************/
    int ProcessRtspPlayMsg(const char* pszURL, const MEDIA_ADDR& /*stServerMediaAddr*/, const START_PLAY_PARAM& stParam);

	int ProcessRtspPlayMsgDownLoad(const char* pszURL, const MEDIA_ADDR& /*stServerMediaAddr*/, const START_PLAY_PARAM& stParam, 
		           const char *pCam, const IVS_DOWNLOAD_PARAM *pParam);

    /******************************************************************
     function   : Str2Time
     description: ��ʱ�䴮��ʽ��YYYY,MM,DD,hh,mm,ss�ֶ�ת�����������Ϊ��λ������ʱ��,
                  ���Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00����������������
     input      : const char * pStr
     input      : const char * pFormat
     output     : NA
     return     : time_t
    *******************************************************************/
    static time_t Str2Time(const char *pStr, const char *pFormat);

    /******************************************************************
     function   : Time2Str
	 description: ������ʱ�䣨�Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00����������������
	 *              ת��Ϊ����"20090630091030"���ַ���
	 input      : char *pDestBuf������ʱ���ַ���������ָ�루ֵ�����������int nbuflen������ʱ���ַ�����������С��
	              const time_t *calptr���Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00��������������
     output     : ����ָ��pDestBuf
     return     : time_t
    *******************************************************************/
    static void Time2Str(const time_t t, std::string& strTime, const std::string& strFormat);

public:
	/***************************************************************************
		�� �� �� : AddrUint2Char
		�������� : ��ַת������������-���ַ�����
		������� : name                    IP��ַ����
		������� : addr                    IP��ַ�ַ���
		�� �� ֵ : RET_OK -�ɹ�       RET_FAIL -ʧ��
	***************************************************************************/
	static int AddrUint2Char(const unsigned int name, std::string &addr);
	 /***************************************************************************
     �� �� �� : GetURL
     �������� : ��SMU/SCU��ȡý��URL
     ������� : stUrlMediaInfo��urlý����Ϣ��stMediaRsp����Ӧ��ý����Ϣ
     ������� : xml                    ����xml
     �� �� ֵ : 0 -�ɹ� ;1-ʧ��
    ***************************************************************************/
	static IVS_INT32 GetURL(const char * /*pszAuthHeader*/, CUserMgr *pUserMgr, 
							ST_URL_MEDIA_INFO& stUrlMediaInfo, 
							ST_MEDIA_RSP& stMediaRsp, bool& bAssociatedAudio);

	IVS_INT32 GetLocalIpPort();

    unsigned long GetPlayerChannel() const { return m_ulPlayerChannel; }

	long GetRtspHandle() const { return m_iRtspHandle; }

	unsigned long GetNetSourceHandle() const { return m_ulNetSourceChannel; }

	const char *GetUrlBuffer()const { return m_stMediaRsp.szURL; } 

	bool  IsStepPlay() const {return m_bStepPlay;}

	bool  IsStepPlayLastFrame() const {return m_bStepPlayIsLastFrame;}

	void  GetCameraID(std::string& strCameraID)const {strCameraID = m_szCameraID;}

	void  GetMediaSourceIP(std::string& strMediaSourceIP)const {strMediaSourceIP = m_strMuIpAddr;}

    void SetSecretFlag(bool bGetKeyIsSuccess);

    bool GetSecretFlag()const;

    void SetSessionInfo(char* pSessionUrl, char* pSessionId);
protected:
    /******************************************************************
     function   : SetMediaParam
     description: ����ý�����;
     input      : MEDIA_PARA * pMediaPara
     output     : strValue 
     return     : int
    *******************************************************************/
    int SetMediaParam(const IVS_MEDIA_PARA* pMediaPara);

    /******************************************************************
     function   : ParseURL
     description: ����URL,pKey����Ҫ�õ���ֵ
     input      : const char * pUrl
     input      : const char * pKey
     input      : std::string & strValue
     output     : NA
     return     : int IVS_SUCCEED�ɹ� IVS_FAILʧ��;
    *******************************************************************/
    int ParseURL(const char* pUrl, const char* pKey, std::string& strValue);

	 /******************************************************************
     function   : ParseIPAddr
     description: ����URL,��ȡMU���ڵ�IP��ַ
     input      : const char * pUrl
     input      : std::string & strValue
     output     : NA
     return     : int IVS_SUCCEED�ɹ� IVS_FAILʧ��;
    *******************************************************************/
    int ParseIPAddr(const char* pUrl,std::string& strValue) const;


	CUserMgr *m_pUserMgr;

protected:
	CUserInfo m_UserInfo;
	std::string m_strLocalIp;
	std::string m_strLocalPort;

	std::string m_strMuIpAddr;

    char m_szCameraID[CODE_LEN];                    // ���������
    IVS_MEDIA_PARA m_stuMediaParam;                 // ý�����
    SourcePlayType m_iStartType;                    // �򿪷�ʽ(�о��/CB/YUV)
    
    unsigned long m_ulHandle;                       // ���ž��
    VOS_Sem*  m_hRtspSetupEvent;

    bool m_bReuseRtspConn;                          // �Ƿ�����Rtsp����;

    IVS_SERVICE_TYPE m_enServiceType;               //��������;

    ST_MEDIA_RSP m_stMediaRsp;

    long m_iRtspHandle;                              //Rtsp���;
    unsigned long m_ulNetSourceChannel;              //�����������;
    unsigned long m_ulPlayerChannel;                 //���ſ���;

    VOS_Mutex*    m_pMediaMutex;       // ý����;
	bool          m_bStepPlay;         // �������ţ�����ƽ̨�ط�
	bool          m_bStepPlayIsLastFrame;  // �������ţ�����ƽ̨�ط�

	bool		  m_bAssociatedAudio;		// �Ƿ�����·����
	ST_URL_MEDIA_INFO m_stUrlMediaInfo;

    //char m_szLocalIp[IVS_IP_LEN];
    unsigned int m_uLocalPort;

    char m_szRemoteIp[IVS_IP_LEN];
    unsigned int m_uRemotePort;

    float              m_fSpeed;                         // �����ٶ�;
    bool m_bControlPause;                    // ��������������ͣ��־
private:
	//״̬,�Ƿ����;
	IVS_INT32 m_iStatus;

    bool m_bGetKeyIsSuccess;
protected:
	time_t   m_tmStart;
	time_t   m_tmEnd;
};

#endif

