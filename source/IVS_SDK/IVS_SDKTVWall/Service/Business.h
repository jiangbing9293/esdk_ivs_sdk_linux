/********************************************************************
 filename    :    Business.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ҵ�����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#pragma once

#include "CumwCommon.h"
namespace CUMW
{
	///////////////////////////////////////////////////////////////////////////
	/// @brief ҵ����������
	///
	/// Description.
	///////////////////////////////////////////////////////////////////////////
	enum BUSINESS_TYPE_MASK
	{
		BUSINESS_TYPE_MASK_NONE = 0x00000000,   ///< ��ҵ��
		BUSINESS_TYPE_MASK_SRC_ALL = 0x0000FFFF,   ///< ��������: ������Դ
		BUSINESS_TYPE_MASK_DST_ALL = 0xFFFF0000,   ///< ��������: ����Ŀ��
		BUSINESS_TYPE_MASK_ALL = 0xFFFFFFFF,   ///< ��������: ��������

		//Ŀ����������
		BUSINESS_TYPE_MASK_DST_LOCAL   = 0x00010000,                    ///< ������ʾ
		BUSINESS_TYPE_MASK_DST_REMOTE  = 0x00020000,                    ///< Զ����ʾ
		BUSINESS_TYPE_MASK_DST_PREVIEW = 0x00040000,   ///< Զ����ʾ֮����Ԥ��
		BUSINESS_TYPE_MASK_DST_FILE = 0x00080000,   ///< ����Ϊ�ļ�
		BUSINESS_TYPE_MASK_DST_NET_AUDIO = 0x00100000,   ///< ��������
		BUSINESS_TYPE_MASK_DST_DATA_CALL_BACK = 0x00200000,   ///< ���ݻص�
		BUSINESS_TYPE_MASK_DST_REMOTE_DIRECT_CONNECT = 0x00400000,   ///< Զ����ʾֱ����ƵԴ��������MDU����PU

		//��Դ��������
		BUSINESS_TYPE_MASK_SRC_REAL_VIDEO = 0x00000001,   ///< ʵʱ���
		BUSINESS_TYPE_MASK_SRC_NET_PLAY_BACK = 0x00000002,   ///< ����¼��ط�
		BUSINESS_TYPE_MASK_SRC_RECORD_DOWN = 0x00000004,   ///< ����¼������
		BUSINESS_TYPE_MASK_SRC_NET_AUDIO   = 0x00000008,    ///< ���������������Խ�ҵ�����Դ����
		BUSINESS_TYPE_MASK_SRC_LOCAL_AUDIO = 0x00000010   ///< ������������Ƶ�㲥ҵ�����Դ����
	};

	///////////////////////////////////////////////////////////////////////////
	/// @brief ҵ������
	///
	/// Description.
	///////////////////////////////////////////////////////////////////////////
	enum BUSINESS_TYPE
	{
		BUSINESS_TYPE_NONE = 0x00000000,   ///< ��ҵ��

		//��ʼ��������
		BUSINESS_TYPE_INIT_DECORDER,

		//����������
		BUSINESS_TYPE_START_DECORDER
	};

	const unsigned long INVALID_BUSINESS_ID = 0;    //��Ч��ҵ��ID
}
class CBusiness
{
public:
    /**************************************************************************
    * name       : CBusiness
    * description: Default constructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CBusiness();

    /**************************************************************************
    * name       : ~CBusiness
    * description: Destructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual ~CBusiness();

    /**************************************************************************
    * name       : GetBusinessType
    * description: ��ȡҵ������
    * input      : NA
    * output     : NA
    * return     : ҵ������
    * remark     : NA
    **************************************************************************/
    CUMW::BUSINESS_TYPE      GetBusinessType() const;

    /**************************************************************************
    * name       : GetBusinessSrcType
    * description: ��ȡҵ����Դ����
    * input      : NA
    * output     : NA
    * return     : ҵ����Դ����
    * remark     : NA
    **************************************************************************/
    CUMW::BUSINESS_TYPE_MASK GetBusinessSrcType() const;

    /**************************************************************************
    * name       : GetBusinessDstType
    * description: ��ȡҵ��Ŀ������
    * input      : NA
    * output     : NA
    * return     : ҵ��Ŀ������
    * remark     : NA
    **************************************************************************/
    CUMW::BUSINESS_TYPE_MASK GetBusinessDstType() const;

    /**************************************************************************
    * name       : SetLinkMode
    * description: ��������ģʽ
    * input      : ulLinkMode  ����ģʽ
    * output     : NA
    * return     : ���óɹ����
    * remark     : NA
    **************************************************************************/
    BOOL                     SetLinkMode
    (   IN unsigned long ulLinkMode);

    /**************************************************************************
    * name       : GetLinkMode
    * description: ��ȡ����ģʽ
    * input      : NA
    * output     : NA
    * return     : ����ģʽ
    * remark     : NA
    **************************************************************************/
    unsigned long            GetLinkMode() const;

    /**************************************************************************
    * name       : SetProtocolType
    * description: ����Э������
    * input      : ulProtocolType  Э������
    * output     : NA
    * return     : ���óɹ����
    * remark     : NA
    **************************************************************************/
    BOOL                     SetProtocolType
    (   IN unsigned char ulProtocolType);

    /**************************************************************************
    * name       : GetProtocolType
    * description: ��ȡЭ������
    * input      : NA
    * output     : NA
    * return     : Э������
    * remark     : NA
    **************************************************************************/
    unsigned char            GetProtocolType() const;

    /**************************************************************************
    * name       : GetDecodeProtocolType
    * description: ��ȡ�������õ�Э������
    * input      : NA
    * output     : NA
    * return     : Э������
    * remark     : NA
    **************************************************************************/
    unsigned char            GetDecodeProtocolType() const;

    /**************************************************************************
    * name       : SetPlayType
    * description: ���ò������͡���û�в������Ͳ������ã���������д����FALSE
    * input      : ulPlayType  �������ͣ�����ҵ��Ŀ������
                   hWnd            ���ھ������Ч��ȡ���ڲ�������
    * output     : NA
    * return     : ���óɹ����
    * remark     : NA
    **************************************************************************/
    virtual BOOL             SetPlayType
    (   IN unsigned long ulPlayType);

    /**************************************************************************
    * name       : GetBusinessTypeString
    * description: ��ȡҵ�������ַ���
    * input      : NA
    * output     : NA
    * return     : Э�������ַ���
    * remark     : NA
    **************************************************************************/
    const char*              GetBusinessTypeString() const;

    /**************************************************************************
    * name       : GetOpenMediaChannelPara
    * description: ��ȡ��ý��ͨ������
    * input      : stPara   ��ý��ͨ������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void             GetOpenMediaChannelPara
    (   IO CUMW::OPEN_MEDIA_CHANNEL_PARA& stPara) const;

    /**************************************************************************
    * name       : SetServerMediaInfo
    * description: ���÷���˵�ý����Ϣ
    * input      : stSvrMediaInfo   ����˵�ý����Ϣ
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void                     SetServerMediaInfo
    (   IN const CUMW::SERVER_MEDIA_INFO& stSvrMediaInfo);

    /**************************************************************************
    * name       : CopyServerMediaInfo
    * description: ���Ʒ������˵�ý����Ϣ
    * input      : objSharedBusiness    �����ҵ�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void                     CopyServerMediaInfo
    (   IN const CBusiness& objSharedBusiness);

    /**************************************************************************
    * name       : SetRecordFrameRate
    * description: ����ҵ��ID
    * input      : ulFrameRate     ��Ƶ֡��
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void             SetBusinessId
    (   IN unsigned long ulBusinessId);

    /**************************************************************************
    * name       : GetBusinessId
    * description: ��ȡҵ��ID
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual unsigned long    GetBusinessId() const;

    /**************************************************************************
    * name       : IsRemoteBusiness
    * description: ����Ƿ�ΪԶ����ǽҵ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    BOOL                     IsRemoteBusiness() const;

    /**************************************************************************
    * name       : IsRemoteDirectConnect
    * description: ����Ƿ�ΪԶ����ǽֱ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    BOOL                     IsRemoteDirectConnect() const;

public: //���³�Ա���������ֱ�ӷ���
    // Զ�̽�����ID
    char m_szDecoderId[CUMW_DECODER_ID_LEN + 1];

    //Զ�̽�������ͨ�����
    unsigned long m_ulChannelId;

    //ý��ỰID
    char m_szSessionId[CUMW::MAX_LEN_SESSION_ID + 1];

    //ý�������IP
    char m_szServerIp[IVS_IP_LEN];

    //ý�屾��IP
    char m_szLocalIp[IVS_IP_LEN];

    //������IP
    char m_szDecoderIp[IVS_IP_LEN];

    //ý��ַ�IP
    char m_szMediaDistributeIp[IVS_IP_LEN];

    //�������˿���Ϣ
    CUMW::PORT_INFO m_stDecoderPortInfo;

    //ý��ַ��˿���Ϣ
    CUMW::PORT_INFO m_stMediaDistributePortInfo;

    //ý��������˿���Ϣ
    CUMW::PORT_INFO m_stServerPortInfo;

    //���ض˿���Ϣ
    CUMW::PORT_INFO m_stLocalPortInfo;

    //��ƵPT
    CUMW::PAY_LOAD_TYPE m_enVideoPt;

    //��ƵPT
    CUMW::PAY_LOAD_TYPE m_enAudioPt;

    // �ỰURL
    char m_szSessionUrl[CUMW_PATH_LEN];

    //��ʼʱ��,NTPʱ�䣬�Դ�1900����������
    unsigned long m_ulStartTime;

    //¼�����ʱ��,NTPʱ�䣬�Դ�1900����������
    unsigned long m_ulEndTime;

    //�첽���
    unsigned long m_ulAsyncSeq;

    //�Ƿ����ڸ��á�InitBusiness��StopBusinessʱ��ֵ��������ڸ��ã����跢��ý��ͨ����/�رղ���
    BOOL m_bReusing;

    //������Ƶ����ҵ��ID��InitBusinessʱ�������ڸ�����ֵ�����ڱ���ҵ�������Ϣ
    unsigned long m_ulSharedBusinessId;

    //BEGIN R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯
    //Ingress�Ƿ�������
    BOOL m_bIngressStart;
    //END   R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯

    unsigned long m_ulInitBusinessId;

    //����/ֹͣ������Ƶ��Ϣ
    unsigned char m_cAudioCmd;

    //������С
    unsigned char m_cVolume;

protected:  //���³�Ա����ֻ��ͨ����Ա��������
    //ҵ����Դ����
    CUMW::BUSINESS_TYPE_MASK m_enBusinessSrcType;

    //ҵ��Ŀ������
    CUMW::BUSINESS_TYPE_MASK m_enBusinessDstType;

    //����ģʽ
    unsigned long m_ulLinkMode;

    //Э������
    unsigned char m_ucProtocolType;

    //ҵ��ID
    unsigned long m_ulBusinessId;

    //֡��
    unsigned long m_ulFrameRate;
};

