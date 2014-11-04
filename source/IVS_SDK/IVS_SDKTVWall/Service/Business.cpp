#include "StdAfx.h"
#include "Business.h"
//#include "RDAProt.h"

CBusiness::CBusiness(): m_ulChannelId(0)//Զ�̽�������ͨ�����
						,m_enVideoPt (CUMW::PAY_LOAD_TYPE_H264)//��ƵPT
						,m_enAudioPt (CUMW::PAY_LOAD_TYPE_PCMA)//��ƵPT
						,m_ulStartTime(0)//��ʼʱ��
						,m_ulEndTime(0)//����ʱ��
						,m_ulAsyncSeq(0)//�첽���
						,m_bReusing(FALSE)//�Ƿ���
						,m_ulSharedBusinessId(0)//����ý������ҵ��ID
						,m_bIngressStart(FALSE)//Ingress�Ƿ�������
						,m_ulInitBusinessId(0)
						,m_enBusinessSrcType(CUMW::BUSINESS_TYPE_MASK_NONE)//ҵ����Դ����
						,m_enBusinessDstType(CUMW::BUSINESS_TYPE_MASK_NONE)//ҵ��Ŀ������
						,m_ulLinkMode(CUMW_LINK_MODE_TRANSMIT)  //����ģʽ
						,m_ucProtocolType(RDA_PROTOCOL_RTP_OVER_UDP)   //Э������
						,m_ulBusinessId(0)//ҵ��ID

{
    // Զ�̽�����ID
    memset(m_szDecoderId, 0, sizeof(m_szDecoderId));

    //ý�������IP
    memset(m_szServerIp, 0, sizeof(m_szServerIp));

    //ý�屾��IP
    memset(m_szLocalIp, 0, sizeof(m_szLocalIp));

    //������IP
    memset(m_szDecoderIp, 0, sizeof(m_szDecoderIp));

    //ý��ַ�IP
    memset(m_szMediaDistributeIp, 0, sizeof(m_szMediaDistributeIp));

    //�������˿���Ϣ
    memset(&m_stDecoderPortInfo, 0, sizeof(m_stDecoderPortInfo));

    //ý��ַ��˿���Ϣ
    memset(&m_stMediaDistributePortInfo, 0, sizeof(m_stMediaDistributePortInfo));

    //ý��������˿���Ϣ
    memset(&m_stServerPortInfo, 0, sizeof(m_stServerPortInfo));

    //���ض˿���Ϣ
    memset(&m_stLocalPortInfo, 0, sizeof(m_stLocalPortInfo));

    //�ỰURL
    memset(m_szSessionUrl, 0, sizeof(m_szSessionUrl));

    //ý��ỰID
    memset(m_szSessionId, 0, sizeof(m_szSessionId));
	//����/ֹͣ������Ƶ��Ϣ
	m_cAudioCmd = 0;
	//������С
	m_cVolume = 0;
	//֡��
	m_ulFrameRate = 0;
}

CBusiness::~CBusiness()
{}

CUMW::BUSINESS_TYPE CBusiness::GetBusinessType() const
{
    //ҵ����Դ������Ŀ�����͵����
    return (CUMW::BUSINESS_TYPE)(m_enBusinessSrcType | m_enBusinessDstType);
}

CUMW::BUSINESS_TYPE_MASK CBusiness::GetBusinessSrcType() const
{
    return m_enBusinessSrcType;
}

CUMW::BUSINESS_TYPE_MASK CBusiness::GetBusinessDstType() const
{
    return m_enBusinessDstType;
}

BOOL CBusiness::SetLinkMode
(   IN unsigned long ulLinkMode)
{
    //�������
    if (CUMW_LINK_MODE_MAX <= ulLinkMode)
    {
        IVS_RUN_LOG_ERR( "Set Link Mode Failed. Link Mode Value(%lu) Invalidate.", ulLinkMode);
        return FALSE;
    }

    //����Ϸ�ֵ
    m_ulLinkMode = ulLinkMode;

    return TRUE;
}

unsigned long CBusiness::GetLinkMode() const
{
    return m_ulLinkMode;
}

BOOL CBusiness::SetProtocolType
(   IN unsigned char ulProtocolType)
{
    //�������
    if (RDA_PROTOCOL_MAX <= ulProtocolType)
    {
        IVS_RUN_LOG_ERR( "Set Protocol Type Failed. Protocol Type Value(%lu) Invalidate.", ulProtocolType);
        return FALSE;
    }

    //����Ϸ�ֵ
    m_ucProtocolType = ulProtocolType;

    return TRUE;
}

unsigned char CBusiness::GetProtocolType() const
{
    return m_ucProtocolType;
}

unsigned char CBusiness::GetDecodeProtocolType() const
{
    //��ֱ����ǽ��ǿ��ʹ��TCP
    /*if (CUMW::BUSINESS_TYPE_MASK_DST_REMOTE == m_enBusinessDstType)
    {
        return CUMW_PROTOCOL_TYPE_TCP;
    }*/

    return m_ucProtocolType;
}

BOOL CBusiness::SetPlayType
(   IN unsigned long ulPlayType)
{
    BOOL bResult = TRUE;

    //���ݲ������ͣ�����ҵ��Ŀ������
    switch (ulPlayType)
    {
    case CUMW_PLAY_TYPE_REMOTE: //��ǽ��ʾ
    {
        m_enBusinessDstType = CUMW::BUSINESS_TYPE_MASK_DST_REMOTE;
        break;
    }

    case CUMW_PLAY_TYPE_PREVIEW: //��ǽԤ��
    {
        m_enBusinessDstType = CUMW::BUSINESS_TYPE_MASK_DST_PREVIEW;
        break;
    }

    case CUMW_PLAY_TYPE_REMOTE_DIRECT_CONNECT:  //Զ����ǽֱ����ƵԴ��������MDU��PU
    {
        m_enBusinessDstType = CUMW::BUSINESS_TYPE_MASK_DST_REMOTE_DIRECT_CONNECT;
        break;
    }

    default:
    {
        IVS_RUN_LOG_ERR( "Set Play Type Failed. Play Type Value(%lu) Invalidate.", ulPlayType);
        bResult = FALSE;
        break;
    }
    }

    return bResult;
}

const char* CBusiness::GetBusinessTypeString() const
{
    const char* pszResult = NULL;

    //unsigned long ulType = GetBusinessType();

    return pszResult;
}

void CBusiness::GetOpenMediaChannelPara
(   IO CUMW::OPEN_MEDIA_CHANNEL_PARA& stPara) const
{
    //���ù���ֵ
    stPara.ulAsyncSeq   = m_ulAsyncSeq;
    stPara.ulBusinessId = m_ulBusinessId;
    stPara.ulBusinessSrcType = m_enBusinessSrcType;
    stPara.ulLinkMode = m_ulLinkMode;
    stPara.ulProtocolType  = m_ucProtocolType;
    stPara.stLocalPortInfo = m_stLocalPortInfo;
    stPara.ulStartTime = m_ulStartTime;
    stPara.ulEndTime = m_ulEndTime;

    if(!CToolsHelp::Strncpy(stPara.szLocalIp, IVS_IP_LEN, m_szLocalIp, IVS_IP_LEN - 1))
	{
		IVS_RUN_LOG_ERR("Get Open Media Channel Para Copy LocalIp failed");
	}
}

void CBusiness::SetServerMediaInfo
(   IN const CUMW::SERVER_MEDIA_INFO& stSvrMediaInfo)
{
    m_stServerPortInfo = stSvrMediaInfo.stServerPortInfo;
    m_enVideoPt = stSvrMediaInfo.enVideoPt;
    m_enAudioPt = stSvrMediaInfo.enAudioPt;

    if(!CToolsHelp::Strncpy(m_szServerIp, sizeof(m_szServerIp), stSvrMediaInfo.szServerIp, sizeof(m_szServerIp) - 1))
	{
		IVS_RUN_LOG_ERR("Set Server Media Info Copy SeverIp failed");
	}
    if(!CToolsHelp::Strncpy(m_szSessionUrl, sizeof(m_szSessionUrl), stSvrMediaInfo.szSessionUrl, sizeof(m_szSessionUrl) - 1))
	{
		IVS_RUN_LOG_ERR("Set Server Media Info Copy SessionUrl failed");
	}
    if(!CToolsHelp::Strncpy(m_szSessionId, sizeof(m_szSessionId), stSvrMediaInfo.szSessionId, sizeof(m_szSessionId) - 1))
	{
		IVS_RUN_LOG_ERR("Set Server Media Info Copy SessionId failed");
	}
}

void CBusiness::CopyServerMediaInfo
(   IN const CBusiness& objSharedBusiness)
{
    //���渴�õ���ԭʼҵ��ID
    m_ulSharedBusinessId = objSharedBusiness.m_ulSharedBusinessId;

    m_stServerPortInfo = objSharedBusiness.m_stServerPortInfo;
    m_enVideoPt = objSharedBusiness.m_enVideoPt;
    m_enAudioPt = objSharedBusiness.m_enAudioPt;

    if(!CToolsHelp::Strncpy(m_szServerIp, sizeof(m_szServerIp), objSharedBusiness.m_szServerIp, sizeof(m_szServerIp) - 1))
	{
		IVS_RUN_LOG_ERR("Copy Server Media Info Copy SeverIp failed");
	}
    if(!CToolsHelp::Strncpy(m_szSessionUrl, sizeof(m_szSessionUrl), objSharedBusiness.m_szSessionUrl, sizeof(m_szSessionUrl) - 1))
	{
		IVS_RUN_LOG_ERR("Copy Server Media Info Copy SessionUrl failed");
	}
    if(!CToolsHelp::Strncpy(m_szSessionId, sizeof(m_szSessionId), objSharedBusiness.m_szSessionId, sizeof(m_szSessionId) - 1))
	{
		IVS_RUN_LOG_ERR("Copy Server Media Info Copy SessionId failed");
	}
}

void CBusiness::SetBusinessId
(   IN unsigned long ulBusinessId)
{
    m_ulBusinessId = ulBusinessId;

    //������ԭʼҵ��ID
    m_ulSharedBusinessId = ulBusinessId;
}

unsigned long CBusiness::GetBusinessId() const
{
    return m_ulBusinessId;
}

BOOL CBusiness::IsRemoteBusiness() const
{
    BOOL bResult = FALSE;

    switch ((unsigned long)m_enBusinessDstType)
    {
    case CUMW::BUSINESS_TYPE_MASK_DST_REMOTE:
    case CUMW::BUSINESS_TYPE_MASK_DST_REMOTE_DIRECT_CONNECT:
    {
        bResult = TRUE;
        break;
    }

    default:
    {
		IVS_DBG_LOG("is not Remote Business");
        bResult = FALSE;
        break;
    }
    }

    return bResult;
}

BOOL CBusiness::IsRemoteDirectConnect() const
{
    BOOL bResult = FALSE;

    if (CUMW::BUSINESS_TYPE_MASK_DST_REMOTE_DIRECT_CONNECT == m_enBusinessDstType)
    {
        bResult = TRUE;
    }

    return bResult;
}
