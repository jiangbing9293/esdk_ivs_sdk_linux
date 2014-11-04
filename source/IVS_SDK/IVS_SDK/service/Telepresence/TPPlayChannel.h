/********************************************************************
 filename    :    TPPlayChannel.h
 author      :    g00209332
 created     :    2013/3/6
 description :    ����ԽӲ�����;
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/3/6 ��ʼ�汾
*********************************************************************/

#ifndef _TP_PLAY_CHANNEL_H_
#define _TP_PLAY_CHANNEL_H_

#include "hwsdk.h"
#include "SDKDef.h"

class CTelepresenceMgr;
class CTPPlayChannel
{
public:
	CTPPlayChannel(void);
	~CTPPlayChannel(void);
	//��ʼ������Ƶ
	IVS_INT32 StartRealPlayByIP(const std::string& strCameraCode,
								const IVS_MEDIA_ADDR& stMediaAddrDst,
								IVS_MEDIA_ADDR& stMediaAddrSrc);

	//��ʼƽ̨¼��ط�
	IVS_INT32 StartPlatformPlayBackByIP(const std::string& strCameraCode,
								const IVS_MEDIA_ADDR& stMediaAddrDst,
								IVS_MEDIA_ADDR& stMediaAddrSrc,
								const TIME_SPAN_INTERNAL& stTimeSpanInter,
								IVS_FLOAT fSpeed);

	//ֹͣʵ����¼��ط�
	IVS_INT32 StopPlayByIP();

	//���������������
	IVS_VOID SetTPMgr(CTelepresenceMgr* pTPMgr)
	{
		m_pTPMgr = pTPMgr;
	}

	//����ý�����
	IVS_INT32 SetMediaParam(const IVS_MEDIA_PARA* pMediaPara);

	//��ȡRTSP���
	long GetRtspHandle() const { return m_nRtspHandle; }
	
	//��ͣƽ̨¼��ط�
	IVS_INT32 PlatformPlayBackPauseByIP(void);

	//�ָ�ƽ̨¼��ط�
	IVS_INT32 PlatformPlayBackResumeByIP(void);

	//����ƽ̨¼�������ط��ٶ�
	IVS_INT32 SetPlayBackSpeedByIP(IVS_FLOAT fSpeed);

	//����ƽ̨¼�������ط�ʱ��
	IVS_INT32 SetPlayBackTimeByIP(IVS_UINT32 uiTime);

private:
	//��ȡʵ��URL
	IVS_INT32 GetRealPlayURL(const std::string& strCamera, 
		const IVS_MEDIA_ADDR& stMediaAddrDst, 
		ST_MEDIA_RSP& stMediaRsp);

	//��ȡ¼��ط�URL
	IVS_INT32 GetPlayBackURL(const std::string& strCamera, 
		const TIME_SPAN_INTERNAL& stTimeSpanInter, 
		const IVS_MEDIA_ADDR& stMediaAddrDst, 
		ST_MEDIA_RSP& stMediaRsp);

	//��ȡ�������Ŀ�ʼ����ʱ��
	IVS_INT32 GetPlayBackTimeSpan(const std::string& strURL,
		TIME_SPAN_INTERNAL& stTimeSpan);

private:
	long m_nRtspHandle;				//RTSP���
	std::string m_strURL;			//ý��URL
	IVS_MEDIA_PARA m_stuMediaParam;   // ý�����
	CTelepresenceMgr* m_pTPMgr;		//����Խӹ���
	float m_fRTSPSpeed;									// RTSP�����ٶ�;
	TIME_SPAN_INTERNAL m_stTimeSpanInter;                // �������Ŀ�ʼ����ʱ��;
};

#endif
