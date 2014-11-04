/********************************************************************
 filename    :    TelepresenceMgr.h
 author      :    g00209332
 created     :    2013/3/6
 description :    ����Խӹ�����;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/3/6 ��ʼ�汾
*********************************************************************/

#ifndef _TELEPRESENCE_MGR_H_
#define _TELEPRESENCE_MGR_H_

#include "hwsdk.h"
#include "SDKDef.h"
#include "vos.h"

class CUserMgr;
class CTPPlayChannel;
class CTelepresenceMgr
{
	typedef std::map<IVS_ULONG, CTPPlayChannel*> TP_PLAY_CHANNEL_MAP;
public:
	CTelepresenceMgr(void);
	~CTelepresenceMgr(void);
	//��ʼ������Ƶ
	IVS_INT32 StartRealPlayByIP(const std::string& strCameraCode,
								const IVS_MEDIA_ADDR& stMediaAddrDst,
								IVS_MEDIA_ADDR& stMediaAddrSrc,
								IVS_ULONG& ulHandle);

	//��ʼ������Ƶ
	IVS_INT32 StartRealPlayByIPEx(const std::string& strCameraCode,
								const IVS_REALPLAY_PARAM&  RealplayParam,
								const IVS_MEDIA_ADDR& stMediaAddrDst,
								IVS_MEDIA_ADDR& stMediaAddrSrc,
								IVS_ULONG& ulHandle);

	//��ʼƽ̨¼��ط�
	IVS_INT32 StartPlatformPlayBackByIP(const std::string& strCameraCode,
								const IVS_PLAYBACK_PARAM& stPlaybackParam,
								const IVS_MEDIA_ADDR& stMediaAddrDst,
								IVS_MEDIA_ADDR& stMediaAddrSrc,
								IVS_ULONG& ulHandle);

	//ֹͣ����
	IVS_INT32 StopPlayByIP(IVS_ULONG ulHandle);

	//ֹͣ���в���
	void StopAllPlayByIP();

	//����UserMgr
	void SetUserMgr(CUserMgr* pUserMgr) {m_pUserMgr = pUserMgr;}

	//��SCU��ȡý��URL
	IVS_INT32 GetURL(const char *pszAuthHeader, ST_URL_MEDIA_INFO& stUrlMediaInfo, ST_MEDIA_RSP& stMediaRsp);

	//����طŲ����Ƿ�Ϸ�
    IVS_INT32 CheckParam(const IVS_TIME_SPAN* pTimeSpan,IVS_UINT32 uiProtocolType, IVS_FLOAT fSpeed, TIME_SPAN_INTERNAL& stTimeSpanInter);

	//��ȡ���ž��
	IVS_ULONG GetPlayHandlebyRtspHandle(long iRtspHandle);

	//��ͣƽ̨¼��ط�
	IVS_INT32 PlatformPlayBackPauseByIP(IVS_ULONG ulHandle);

	//�ָ�ƽ̨¼��ط�
	IVS_INT32 PlatformPlayBackResumeByIP(IVS_ULONG ulHandle);

	//����ƽ̨¼�������ط��ٶ�
	IVS_INT32 SetPlayBackSpeedByIP(IVS_ULONG ulHandle, IVS_FLOAT fSpeed);

	//����ƽ̨¼�������ط�ʱ��
	IVS_INT32 SetPlayBackTimeByIP(IVS_ULONG ulHandle, IVS_UINT32 uiTime);

private:
	// ����ulHandle��ȡ����ͨ��
	// ���bCreateΪTURE��û�ҵ��ʹ���һ���µ�
	CTPPlayChannel* GetPlayChannel(IVS_ULONG ulHandle, IVS_BOOL bCreate=FALSE);
	// ����ulHandle�Ƴ�һ������ͨ��
	IVS_VOID RemovePlayChannel(IVS_ULONG ulHandle);

private:
	TP_PLAY_CHANNEL_MAP m_channelMap;	// ����ͨ��MAP
	VOS_Mutex* m_pChannelMapMutex;		// ����ͨ��MAP��
	CUserMgr* m_pUserMgr;				// �û�����

};

#endif
