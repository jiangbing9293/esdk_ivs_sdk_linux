/********************************************************************
	filename	: 	Playback.h
	author		:	z00201790
	created		:	2012/12/06	
	description	:	����ط�ҵ����;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/
#ifndef __PLAYBACK_H__
#define __PLAYBACK_H__

#include "MediaBase.h"
#include "vos.h"

class CPlayback: public CMediaBase
{
public:
	CPlayback(void);
	~CPlayback(void);

public:
	void SetHandle(unsigned long ulHandle);

	unsigned long GetHandle();

	/******************************************************************
	function : StartPlatformPlayBack
	description: ��ʼƽ̨¼��ط�
	input : uiProtocolType,Э�����ͣ�pCameraCode���������ţ�hWnd����������stTimeSpanInter��ʱ�����Ϣ ;playBackServiceType ҵ������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    int StartPlatformOrBackupPlayBack(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const char* pCameraCode,
                              unsigned int uiProtocolType,
                              const TIME_SPAN_INTERNAL& stTimeSpanInter,
                              HWND hWnd,
                              float fSpeed,IVS_SERVICE_TYPE playBackServiceType = SERVICE_TYPE_PLAYBACK);
#endif

	int StartPlatformPlayBackCB(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const char* pCameraCode,
								unsigned int uiProtocolType,
								const TIME_SPAN_INTERNAL& stTimeSpanInter,
								PlayBackCallBackRaw fPlayBackCallBackRaw,
								void* pUserData, 
								float fSpeed);

	int StartPlatformPlayBackCBFrame(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const char* pCameraCode,
									unsigned int uiProtocolType,
									const TIME_SPAN_INTERNAL& stTimeSpanInter,
									PlayBackCallBackFrame fPlayBackCallBackFrame,
									void* pUserData, 
									float fSpeed);

	

	/******************************************************************
	function : StopPlatformPlayBack
	description: ֹͣƽ̨¼��ط�
	input : 
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int StopPlatformPlayBack();

	/******************************************************************
	function : PlayBackPause
	description: ��ͣ¼��ط�
	input : 
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int PlayBackPause();

	/******************************************************************
	function : PlayBackResume
	description: �ָ�¼��ط�
	input : 
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int PlayBackResume();
	

	/******************************************************************
	function : StepForward
	description: ����ǰ������;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StepForward();

	/******************************************************************
	function : StepForward
	description: �������˲���;
	input : ulHandle,���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StepBackward();

	/******************************************************************
	function   : SetPlayBackSpeed
	description: ���ûظ��ٶ�
	input : 
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int SetPlayBackSpeed(float fSpeed);

	/******************************************************************
	function : GetPlayBackTime
	description: ��ȡ¼��ط�ʱ��
	input : stPlayBackTime��¼��ط�ʱ����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int GetPlayBackTime(IVS_PLAYBACK_TIME& stPlayBackTime);
	int GetCurrentFrameTick(unsigned long long &tick);

	/******************************************************************
	function : SetPlayBackTime
	description: ����¼��ط�ʱ��
	input : uiTime��ʱ������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int SetPlayBackTime(unsigned int uiTime);

	/******************************************************************
	 function   : GetPlayBackSpeed
	 description: ��ȡ�����ٶ�;
	 output     : NA
	 return     : int �����ٶ�
	*******************************************************************/
	float GetPlayBackSpeed()const { return m_fSpeed;};

	/******************************************************************
	function : IsExistHWnd
	description: ��鴰�����Ƿ����
	input : hWnd��������
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    bool IsExistHWnd(const HWND& hWnd) const;
#endif

    /******************************************************************
     function   : NotifyStreamBufferState
     description: ֪ͨ��������״̬
     input      : IVS_INT32 iBufferState
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 NotifyStreamBufferState(IVS_INT32 iBufferState);

	IVS_VOID  StopNotifyBufEvent();
#ifdef WIN32
    // ��ʼǰ��¼��ط�
    int StartPUPlayBack(const char* pCameraCode,
                        unsigned int uiProtocolType,
                        const TIME_SPAN_INTERNAL& stTimeSpanInter,
                        HWND hWnd,
                        float fSpeed,
                        const char* pFileName);
#endif
    // ֹͣǰ��¼��ط�
    int StopPUPlayBack();


	/******************************************************************
	function : StopBackupPlayBack
	description: ֹͣ����¼��ط�
	input : 
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int StopBackupPlayBack();

    inline IVS_SERVICE_TYPE GetServiceType() const
    {
        return m_enServiceType;
    }

    inline void SetServiceType(IVS_SERVICE_TYPE enServiceType)
    {
        m_enServiceType = enServiceType;
    }

	bool IsPaused() const { return m_bPaused; }

    bool IsControlPaused() const { return m_bControlPause; }
	/******************************************************************
	function :   StartLocalPlayBack
	description: ��������¼��ط�;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
	int StartLocalPlayBack (
			const IVS_CHAR* pFileName,
			const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam,
			HWND hWnd);
#endif

	/******************************************************************
	function :   StopLocalPlayBack
	description: ;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int StopLocalPlayBack();

	/******************************************************************
	function : AddPlayWindow
	description: ��Ӳ��Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
	int AddPlayWindow(HWND hWnd);
	/******************************************************************
	function : DelPlayWindow
	description: ɾ�����Ŵ���;
	input : hWnd����������ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	int DelPlayWindow(HWND hWnd);

	/*****************************************************************************
	�������ƣ�AddPlayWindowPartial
	�������������Ӿֲ�ͼ�񲥷Ŵ���
	���������hWnd��       ���ھ��
			 pPartial��   �ֲ�ͼƬ����
	���������NA
	�� �� ֵ��int
	****************************************************************************/
	int AddPlayWindowPartial(HWND hWnd, void* pPartial);

	/*****************************************************************************
	�������ƣ�UpdateWindowPartial
	�������������¾ֲ��Ŵ�Դ����
	���������hWnd��       ���ھ��
			 pPartial��   �ֲ�����
	���������NA
	�� �� ֵ��int
	****************************************************************************/

	int UpdateWindowPartial(HWND hWnd, void* pPartial);
#endif

	void DoRawFrameCallBack(IVS_RAW_FRAME_INFO* pstRawFrameInfo, IVS_CHAR* pFrame, IVS_UINT32 uFrameSize);

	void DoFrameCallBack(IVS_INT32 iStreamType, IVS_VOID* pFrameInfo);

	virtual void RetSet();

private:
	/******************************************************************
	function : StartPlay
	description: ��ʼ�ط�
	input : pstMediaPara��ý����Ϣ��pCameraCode���������ţ�stTimeSpanInter��ʱ��Ƭ��Ϣ��stParam�����Ų���
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int StartPlay(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,
		          const IVS_MEDIA_PARA* pstMediaPara, 
                  const IVS_CHAR* pCameraCode, 
                  const TIME_SPAN_INTERNAL& stTimeSpanInter,
                  START_PLAY_PARAM& stParam,
                  const char* pFileName);

	/******************************************************************
	function : DoGetUrlPlayBack
	description: ��ȡ�ط�Url
	input : stTimeSpanInter��ʱ�����Ϣ
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    int DoGetUrlPlayBack(const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,IVS_SERVICE_TYPE ServiceType,
                         const TIME_SPAN_INTERNAL& stTimeSpanInter,
                         const char *pFileName);
private:
#ifdef WIN32
    HWND m_hWnd;    // ��ʾ����;
#endif
    //RealPlayCallBackRaw m_fRealPlayCallBackRaw;         // �������Ļص�����
    //void* m_pRealPlayUserDataRaw;
    //VOS_Mutex*          m_pRealPlayRawCBMutex;              // �������ص���;
    TIME_SPAN_INTERNAL m_stTimeSpanInter;                // �������Ŀ�ʼ����ʱ��;
    
    unsigned int       m_uiDragPlayTime;                 // �϶���Ļط�ʱ��;

	unsigned int       m_FirstRtpStamp;

	bool               m_bPaused;
	bool               m_bLocalFile;

	PlayBackCallBackRaw		m_fPlayBackCallBackRaw;         // �������Ļص�����
	void*					m_pPlayBackUserDataRaw;
	PlayBackCallBackFrame	m_fPlayBackCallBackFrame;     // �����֡���ݵĻص�����
	void*					m_pPlayBackUserDataFrame;
	VOS_Mutex*				m_pPlayBackRawCBMutex;              // �������ص���;
	VOS_Mutex*				m_pPlayBackFrameDecoderCBMutex;   // �������ص���;      

	//unsigned long       m_ulLastStepBackwardTime;        // �ϴε������˵�ʱ��;
};

#endif //__PLAYBACK_H__
