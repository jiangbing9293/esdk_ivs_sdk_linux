/********************************************************************
 filename    :    RealPlayMgr.h
 author      :    s00191067
 created     :    2012/11/19
 description :    ʵ������ʵ��ҵ��;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/11/19 ��ʼ�汾
*********************************************************************/
#ifndef __REALPLAY_MGR_H__
#define __REALPLAY_MGR_H__

#include "RealPlay.h"
#include "SDKDef.h"
#include "Lock.h"

class CRealPlayMgr
{
public:
	CRealPlayMgr(void);
	~CRealPlayMgr(void);

public:
	void SetUserMgr(CUserMgr* pUserMgr);

 
	/******************************************************************
	function : GetMediaBase
	description: ���ݾ����ȡrealplay;
	input : ulHandle�����
	output : NA
	return : MediaBase����
	*******************************************************************/
    CMediaBase *GetMediaBase(IVS_ULONG ulHandle);

    /******************************************************************
	function : StartRealPlay
	description: ��ʼʵ��;
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�hWnd����������pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
#ifdef WIN32
    IVS_INT32 StartRealPlay(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, HWND hWnd, IVS_ULONG* pHandle);
#endif
	/******************************************************************
	function : StartRealPlayCBRaw
	description: ��ʼʵ��(������);
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�fRealPlayCallBackRaw���������ص���pUserData���û���Ϣ��pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StartRealPlayCBRaw(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackRaw fRealPlayCallBackRaw, void* pUserData, IVS_ULONG* pHandle );

	/******************************************************************
	function : StartRealPlayCBFrame
	description: ��ʼʵ��(YUV��);
	input : pRealplayPara,ʵ��������Ϣ��pCameraCode���������ţ�fRealPlayCallBackFrame��YUV�����ص���pUserData���û���Ϣ��pHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StartRealPlayCBFrame(IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData, IVS_ULONG* pHandle );

	/******************************************************************
	function : StopRealPlay
	description: ֹͣʵ��;
	input : ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_INT32 StopRealPlay(IVS_ULONG ulHandle);

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
	/******************************************************************
	function : GetPlayerChannelByHandle
	description: ��ȡͨ����;
	input : ulHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
	IVS_UINT32 GetPlayerChannelByHandle(IVS_ULONG ulHandle);

	/******************************************************************
	function : GetCameraCodeByHandle
	description: ���ݾ����ȡCameraCode;
	input : strCameraCode���������ţ�ulHandle�����
	output : NA
	return : ��
	*******************************************************************/
	void GetCameraCodeByHandle(IVS_ULONG ulHandle, std::string& strCameraCode);

	/******************************************************************
	function : StopAllRealPlay
	description: �ر�����ʵ��;
	input : ��
	output : NA
	return : ��
	*******************************************************************/
	void StopAllRealPlay();

	/******************************************************************
	function : GetPlayHandlebyRtspHandle
	description: ��ȡʵ�����;
	input : iRtspHandle�����
	output : NA
	return : ʵ�����;
	*******************************************************************/
	IVS_ULONG GetPlayHandlebyRtspHandle(long iRtspHandle);
	IVS_ULONG GetPlayHandlebyRtspHandleEx(long iRtspHandle);

	/******************************************************************
	function : GetPlayHandleByNetSourceChannel
	description: ��ȡʵ�����;
	input : ulNetSourceChannel�����
	output : NA
	return : ʵ�����;
	*******************************************************************/
	IVS_ULONG GetPlayHandleByNetSourceChannel(unsigned long ulNetSourceChannel);

	/******************************************************************
	function : GetPlayHandleByPlayChannel
	description: ���ݲ����ŵ���ȡ���ž��;
	input : ulPlayChannel�������ŵ�
	output : NA
	return : ʵ�����;
	*******************************************************************/
	IVS_ULONG GetPlayHandleByPlayChannel(unsigned long ulPlayChannel);

	/******************************************************************
	function : PlaySound
	description: ������·����;
	input : uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 PlayAudioSound(unsigned int uiPlayHandle);

	/******************************************************************
	function : StopSound
	description: ֹͣ������·����;
	input : uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 StopAudioSound(unsigned int uiPlayHandle);

	/******************************************************************
	function : SetVolume
	description: ��������;
	input : uiVolumeValue������ֵ��uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 SetVolume(unsigned int uiPlayHandle, unsigned int uiVolumeValue);

	/******************************************************************
	function : GetVolume
	description: ��ȡ����;
	input : puiVolumeValue������ֵ��uiPlayHandle�����
	output : NA
	return : �ɹ�-0��ʧ��-1
	*******************************************************************/
    IVS_INT32 GetVolume(unsigned int uiPlayHandle, unsigned int* puiVolumeValue);

	CRealPlay *GetRealPlay(IVS_ULONG  ulHandle);
	CRealPlay *GetRealPlayEx(IVS_ULONG ulHandle);
	void  ReleaseRealPlay(CRealPlay *pRealPlay);


private:
	/******************************************************************
	function : GetHandleByHwnd
	description: ��鴰���Ƿ����;
	input : hWnd��������
	output : NA
	return : ���ž��
	*******************************************************************/
#ifdef WIN32
	IVS_ULONG GetHandleByHwnd(HWND hWnd);
#endif

	/******************************************************************
	function : GetFreeRealPlay
	description: ��ȡ���е�ʵ������;
	input : ulHandle�����
	output : NA
	return : ʵ������
	*******************************************************************/
	CRealPlay *GetFreeRealPlay(IVS_ULONG& ulHandle);


	CRealPlay *GetFreeRealPlayEx(IVS_ULONG& ulHandle);

	void FreeRealPlay(IVS_ULONG ulHandle);

	void FreeRealPlayEx(IVS_ULONG ulHandle);

private:
	CUserMgr *m_pUserMgr;
	//IVS_INT32 m_iHandle;

	//�����ʵ�����ӳ���;
	typedef std::map<unsigned long, CRealPlay*> HandleRealPlayMap;
	typedef HandleRealPlayMap::iterator HandleRealPlayMapIter;
	HandleRealPlayMap m_HanleRealPlayMap;

	VOS_Mutex *m_pHanleRealPlayMutex;

};

#endif
