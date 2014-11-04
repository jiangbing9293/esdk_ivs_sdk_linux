/********************************************************************
	filename	: 	PlaybackMgr.h
	author		:	s00193168
	created		:	2012/12/06	
	description	:	����ط�ҵ�������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/
#ifndef __PLAYBACK_MGR_H__
#define __PLAYBACK_MGR_H__


#include "Playback.h"
#include "UserInfo.h"
#include "hwsdk.h"
#include "SDKDef.h"


typedef struct stVodPlayInfo
{
	IVS_UINT32              ulPlayType;			    //�ط����� added by l0076138
#ifdef WIN32
	HWND                    hWnd;					//���ž��
#endif
	IVS_UINT32              ulPort;                 //�˿ں�
	IVS_BOOL                bPause;                 //�Ƿ�����ͣ�ط�״̬
	IVS_BOOL                bFirstGetTime;		    //�Ƿ��ǵ�һ��get time(1 �ǵ�һ�� 0 ���ǵ�һ��)
	IVS_BOOL                bUserFirstGetTime;		//�Ƿ����û���һ��get time(1 �ǵ�һ�� 0 ���ǵ�һ��)
	IVS_FLOAT               fPlaySpeed;             //�ط��ٶ�
	//CVodPlayTime *m_pVodPlayTime;                 //��ȡ�طŵĵ�һ֡��ʱ���
	IVS_DOUBLE              dVedioStartTime;        //���ڱ���¼���ļ��ʼʱ�䣬����¼����ק
	IVS_DOUBLE              dStartTime;             //¼���ļ��Ŀ�ʼʱ�䣬ȡ�ⲿ��������ʱ��
	IVS_DOUBLE              dEndTime;               //¼���ļ��Ŀ�ʼʱ�䣬ȡ�ⲿ��������ʱ��
	IVS_DOUBLE              dPlayTime;              //��ǰ����ʱ��(����ʱ��)
	IVS_UINT32              ulFirstTime;			//�ʼ��õ�ʱ���
	IVS_UINT32              ulLastTime;             //����ȡ��ʱ���
	time_t t;					                    //Ӳ����ǽʱ���ʾ
	IVS_INT32               iHandle;                //�ɹ����صľ��
	//PlayBackCallBackCB      fCallBackFun;           //�������ص�����
	//PlayBackCallBackYUV     fCallBackYUVFun;        //YUV�ص�����
	void*                   pUserDataCB;            //�û�����(������)
	void*                   pUserDataYUV;           //�û�����(YUV)
	SourcePlayType          iStartType;             // �򿪷�ʽ(�о��/CB/YUV)
	IVS_CHAR                szCameraCode[33];		//�����ID add by wKF71044
#ifdef WIN32
	IVS_CHAR                szLocalPlayFileName[MAX_PATH];	  //����¼��ط��ļ���
#else
	IVS_CHAR                szLocalPlayFileName[PATH_MAX];	  //����¼��ط��ļ���
#endif
}ST_VOD_PLAY_INFO;

//���ھ�����˿ںŵ�ӳ�����
#ifdef WIN32
typedef std::map<HWND, ST_VOD_PLAY_INFO*> MAP_HWND_TO_PORT;
typedef std::pair<HWND, ST_VOD_PLAY_INFO*> PAIR_HWND_TO_PORT;
#endif
typedef std::map<int, ST_VOD_PLAY_INFO*> MAP_PLAYHWND_TO_PORT;
typedef std::pair<int, ST_VOD_PLAY_INFO*> PAIR_PLAYHWND_TO_PORT;


class CPlaybackMgr
{
public:
	CPlaybackMgr(void);
	virtual ~CPlaybackMgr(void);
public:
	void SetUserMgr(CUserMgr* pUserMgr);

	/******************************************************************
	function : StartPlatformPlayBack
	description: ��ʼƽ̨¼��ط�;
	input : pPlaybackParam,�طŲ�����pCameraCode���������ţ�hWnd����������pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    IVS_INT32 StartPlatformPlayBack(const IVS_CHAR* pCameraCode,
                                    const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                    HWND hWnd,
                                    IVS_ULONG* pHandle);
#endif

	IVS_INT32 StartPlatformPlayBackCB(const IVS_CHAR* pCameraCode,
									  const IVS_PLAYBACK_PARAM* pPlaybackParam,
									  PlayBackCallBackRaw fPlayBackCallBackRaw,
									  void* pUserData, 
									  IVS_ULONG* pHandle);

	IVS_INT32 StartPlatformPlayBackCBFrame(const IVS_CHAR* pCameraCode,
										   const IVS_PLAYBACK_PARAM* pPlaybackParam,
										   PlayBackCallBackFrame fPlayBackCallBackFrame,
										   void* pUserData, 
										   IVS_ULONG* pHandle);

	/******************************************************************
	function : StartbackupPlayBack
	description: ��ʼ����¼��ط�;
	input : pPlaybackParam,�طŲ�����pCameraCode���������ţ�hWnd����������pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    IVS_INT32 StartBackupPlayBack(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,
                                    const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                    HWND hWnd,
                                    IVS_ULONG* pHandle);
#endif

	/******************************************************************
	function : CheckParam
	description: ����طŲ����Ƿ�Ϸ�;
	input : pTimeSpan,ʱ�����Ϣ��pCameraCode���������ţ�uiProtocolType��Э�����ͣ�fSpeed�����ʣ�stTimeSpanInter��ʱ��Ƭ�ṹ����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 CheckParam(const IVS_CHAR* pCameraCode,const IVS_TIME_SPAN* pTimeSpan,IVS_UINT32 uiProtocolType, IVS_FLOAT fSpeed, TIME_SPAN_INTERNAL& stTimeSpanInter);

	/******************************************************************
	function : StopPlatformPlayBack
	description: ֹͣƽ̨¼��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StopPlatformPlayBack(IVS_ULONG ulHandle);

    IVS_INT32 StopDisasterRecoveryPlayBack(IVS_ULONG ulHandle);
#ifdef WIN32
    IVS_INT32 StartDisasterRecoveryPlayBack(const IVS_CHAR* pCameraCode,const IVS_PLAYBACK_PARAM* pPlaybackParam,HWND hWnd,IVS_ULONG* pHandle);
#endif
	/******************************************************************
	function : PlayBackFrameStepForward
	description: ƽ̨¼�񵥲�ǰ���ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 PlayBackFrameStepForward(IVS_ULONG ulPlayHandle);

	/******************************************************************
	function : PlayBackFrameStepBackWard
	description: ƽ̨¼�񵥲����˻ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 PlayBackFrameStepBackward(IVS_ULONG ulPlayHandle);

	/******************************************************************
	function : PlayBackPause
	description: ��ͣ�ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 PlayBackPause(IVS_ULONG ulHandle);

	/******************************************************************
	function : PlayBackResume
	description: �ָ��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 PlayBackResume(IVS_ULONG ulHandle);

	/******************************************************************
	 function   : GetPlayBackStatus
	 description: ��ȡ�ط�״̬;
	 input      : IVS_ULONG ulPlayHandle
	 output     : IVS_BOOL& bPause
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetPlayBackStatus(IVS_ULONG ulPlayHandle, IVS_BOOL& bPause);

	/******************************************************************
	function : SetPlayBackSpeed
	description: ���ò����ٶ�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 SetPlayBackSpeed(IVS_ULONG ulHandle, float fSpeed);

	/******************************************************************
	 function   : GetPlayBackSpeed
	 description: ��ȡ�����ٶ�
	 input      : IVS_ULONG ulHandle
	 output     : float & fSpeed
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetPlayBackSpeed(IVS_ULONG ulHandle, float& fSpeed);

	/******************************************************************
	function : GetPlayBackTime
	description: ��ȡ�ط�ʱ��;
	input : ulHandle,�����pPlayBackTime������ʱ��
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 GetPlayBackTime(IVS_ULONG ulPlayHandle, IVS_PLAYBACK_TIME* pPlayBackTime);
	IVS_INT32 GetCurrentFrameTick(IVS_ULONG ulPlayHandle, IVS_UINT64* pTick);

	/******************************************************************
	function : SetPlayBackTime
	description: ����¼��طŲ���ʱ��;
	input : ulHandle,�����uiTime������ʱ��
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 SetPlayBackTime(IVS_ULONG ulHandle ,IVS_UINT32 uiTime);

	/******************************************************************
	function : GetFreePlayback
	description: ���ݾ����ȡ���ж���;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	CPlayback* GetFreePlayback(IVS_ULONG& ulHandle);

	/******************************************************************
	function : GetPlayBack
	description: ���ݾ����ȡ����;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    CPlayback* GetPlayBack(IVS_ULONG ulHandle);

	/******************************************************************
	function : GetPlayHandlebyRtspHandle
	description: ����Rtsp�����ȡ���ž��;
	input : iRtspHandle��Rtsp���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_ULONG GetPlayHandlebyRtspHandle(int iRtspHandle);

	/******************************************************************
	function : GetPlayHandleByNetSourceChannel
	description: ��ȡ���ž��;
	input : ulNetSourceChannel��NetSource�ŵ�
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_ULONG GetPlayHandleByNetSourceChannel(unsigned long ulNetSourceChannel);

	/******************************************************************
	function : GetPlayHandleByPlayChannel
	description: ���ݲ����ŵ��Ż�ȡ���ž��;
	input : ulPlayChannel�������ŵ���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_ULONG GetPlayHandleByPlayChannel(unsigned long ulPlayChannel);

	/******************************************************************
	function : GetPlayerChannelByHandle
	description: ���ݾ����ȡ�����ŵ�;
	input : ulHandle�����
	output : NA
	return : �����ŵ���
	*******************************************************************/
	unsigned int GetPlayerChannelByHandle(unsigned long ulHandle);

	/******************************************************************
	function : *GetMediaBase
	description: ���ݾ����ȡmeidiaBase����ָ��;
	input : ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	CMediaBase *GetMediaBase(IVS_ULONG ulHandle);
	/******************************************************************
	function : PlaySound
	description: ������·����;
	input : uiPlayHandle�����ž��
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 PlayAudioSound(unsigned int uiPlayHandle);

	/******************************************************************
	function : StopSound
	description: ֹͣ������·����;
	input : uiPlayHandle�����ž��
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 StopAudioSound(unsigned int uiPlayHandle);

	/******************************************************************
	function : SetVolume
	description: ��������;
	input : uiPlayHandle,���ž����puiVolumeValue������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 SetVolume(unsigned int uiPlayHandle, unsigned int uiVolumeValue);

	/******************************************************************
	function : GetVolume
	description: ��ȡ����;
	input : uiPlayHandle,���ž����puiVolumeValue������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 GetVolume(unsigned int uiPlayHandle, unsigned int* puiVolumeValue);

    /******************************************************************
	function : StartPUPlayBack
	description: ��ʼǰ��¼��ط�;
	input : pPlaybackParam,�طŲ�����pCameraCode���������ţ�hWnd����������pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    IVS_INT32 StartPUPlayBack(const IVS_CHAR* pCameraCode,
                              const IVS_PLAYBACK_PARAM* pPlaybackParam,
                              HWND hWnd,
                              IVS_ULONG* pHandle);
#endif

	/******************************************************************
	function : StopPUPlayBack
	description: ֹͣǰ��¼��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StopPUPlayBack(IVS_ULONG ulHandle);

    IVS_INT32 GetPUPlayBackFileName(const char* pCameraCode,
                                    const IVS_TIME_SPAN* pTimeSpan,
                                    IVS_RECORD_INFO &stRecordInfo);

    /******************************************************************
     function   : NotifyStreamBufferState
     description: ֪ͨ��������״̬
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_INT32 iBufferState
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 NotifyStreamBufferState(IVS_ULONG ulPlayHandle, IVS_INT32 iBufferState);

	IVS_VOID  StopNotifyBufEvent(IVS_ULONG ulPlayHandle);

	/******************************************************************
	function : StopBackupPlayBack
	description: ֹͣ����¼��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StopBackupPlayBack(IVS_ULONG ulHandle);

	/******************************************************************
	function : StartLocalPlayBack
	description: ��ʼ����¼��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
	IVS_INT32 StartLocalPlayBack (
		const IVS_CHAR* pFileName,
		const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam,
		HWND hWnd, 
		IVS_ULONG* pHandle);
#endif

	/******************************************************************
	function : StartLocalPlayBack
	description: ֹͣ����¼��ط�;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StopLocalPlayBack(IVS_ULONG ulPlayHandle);

	/******************************************************************
	function : AddPlayWindow
	description: ��Ӳ��Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
	IVS_INT32 AddPlayWindow(IVS_ULONG ulHandle, HWND hWnd);
	/******************************************************************
	function : DelPlayWindow
	description: ɾ�����Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 DelPlayWindow(IVS_ULONG ulHandle, HWND hWnd);

	IVS_INT32 AddPlayWindowPartial(IVS_ULONG ulHandle, HWND hWnd, void* pPartial);
	IVS_INT32 UpdateWindowPartial(IVS_ULONG ulHandle, HWND hWnd, void* pPartial);
#endif
	//ֹͣ���лط�
	void StopAllPlayBack();
private:
	/******************************************************************
	function : GetHandleByHwnd
	description: ���ݴ�������þ����
	input : hWnd,������
	output : NA
	return : �����
	*******************************************************************/
#ifdef WIN32
    IVS_ULONG GetHandleByHwnd(HWND hWnd);
#endif

private:
    CUserMgr *m_pUserMgr;

	typedef std::map<unsigned long, CPlayback*> HandlePlayBackMap;
	typedef HandlePlayBackMap::iterator HandlePlayBackMapIter;
	HandlePlayBackMap m_HanlePlayBackMap;

	VOS_Mutex* m_pHandlePlayBackMutex;	
};

#endif
