/********************************************************************
    filename	: 	CumwStatusClasses.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	����״̬���ʵ��
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/
#ifndef CUMW_STATUS_CLASSES_H
#define CUMW_STATUS_CLASSES_H

/*!< ģ�������õĻ���ͷ�ļ� */
#include "CumwCommon.h"

using namespace CUMW;

//CStatusConfig����ض���
namespace CUMW
{
    const unsigned short PORT_MIN = 1024;   //�˿ڵ���Сֵ

    //�����ļ���section��key�������壬�Լ�Ĭ��ֵ
    const char SECTION_LOG_CFG[] = "LOG_CFG";    //��־����
    //��־����
    const char KEY_LOG_LM[] = "LogLM";      //��־�����key�ַ���
    const unsigned long DEFAULT_VALUE_LOG_LM = IVSLOG_ERR;    //��־�����Ĭ��ֵ

    //Զ�̽���������
    const char SECTION_REMOTE_DECODER[] = "REMOTE_DECODER";     //Զ�̽���������
    const char KEY_REMOTE_DECODER_LISTEN_PORT[] = "ListenPort"; //Զ�̽����������˿ڵ�key�ַ���
    const unsigned short DEFAULT_VALUE_REMOTE_DECODER_LISTEN_PORT = 5050;         //Զ�̽����������˿ڵ�Ĭ��ֵ
}

//����״̬��
class CStatusConfig
{
public:

    /**************************************************************************
    * name       : CStatusConfig
    * description: Default constructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusConfig();

    /**************************************************************************
    * name       : InitConfig
    * description: �������ļ��Ӷ�ȡ��ʼ������
    * input      : strConfigFileFullName   �����ļ���
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long           InitConfig(const string& strConfigFileFullName);

    /**************************************************************************
    * name       : SaveConifg
    * description: �������õ������ļ�
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long           SaveConifg();

    /**************************************************************************
    * name       : ReloadConfig
    * description: ���¼���������
    * input      : NA
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long           ReloadConfig();

    /**************************************************************************
    * name       : GetLogLM
    * description: ��ȡ��־����
    * input      : NA
    * output     : NA
    * return     : ��־����
    * remark     : NA
    **************************************************************************/
    unsigned long  GetLogLM() const;

    /**************************************************************************
    * name       : SetLogLM
    * description: ������־����
    * input      : ulLogLM     ��־����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void           SetLogLM(const unsigned long ulLogLM);

    /**************************************************************************
    * name       : GetRemoteDecoderListenPort
    * description: ��ȡԶ�̽����������˿�
    * input      : NA
    * output     : NA
    * return     : �����˿�
    * remark     : NA
    **************************************************************************/
    unsigned short GetRemoteDecoderListenPort() const;

    /**************************************************************************
    * name       : SetRemoteDecoderListenPort
    * description: ����Զ�̽����������˿�
    * input      : usListenPort    �����˿�
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void           SetRemoteDecoderListenPort
    (   IN const unsigned short usListenPort);

protected:

    /**************************************************************************
    * name       : GetConfigValue
    * description: ��ȡ�������ֵ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void   GetConfigValue();

    /**************************************************************************
    * name       : SetConfigValue
    * description: �����������ֵ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void   SetConfigValue();

private:

    //�����ļ�ȫ·��
    string m_strConfigFileFullName;

    //������
    unsigned long m_ulLogLM;                      // ��־����
    unsigned short m_usRemoteDecoderListenPort;    //Զ�̽����������˿�
};

//����״̬��
class CStatusSingleton
{
private:

    /**************************************************************************
    * name       : CStatusSingleton
    * description: ˽�й��캯�����������ⲿ��������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusSingleton();

public:

    /**************************************************************************
    * name       : Instance
    * description: ��ȡʵ��
    * input      : NA
    * output     : NA
    * return     : ����ʵ������
    * remark     : NA
    **************************************************************************/
    static CStatusSingleton& Instance()
    {
        static CStatusSingleton instance;

        return instance;
    }

    /**************************************************************************
    * name       : GetConfig
    * description: ��ȡ����״̬��ʵ��
    * input      : NA
    * output     : NA
    * return     : ����״̬��ʵ������
    * remark     : NA
    **************************************************************************/
    CStatusConfig& GetConfig()
    {
        return m_objStatusConfig;//lint !e1536
    };

    /**************************************************************************
    * name       : GetConnMgr
    * description: ��ȡͨ�Ź�����ʵ��
    * input      : NA
    * output     : NA
    * return     : ͨ�Ź�����ʵ������
    * remark     : NA
    **************************************************************************/
    CConnMgr& GetConnMgr()
    {
        return m_objConnMgr;//lint !e1536
    };

    /**************************************************************************
    * name       : GetNVSTimer
    * description: ��ȡ��ʱ��ʵ��
    * input      : NA
    * output     : NA
    * return     : ͨ�Ŷ�ʱ��ʵ������
    * remark     : NA
    **************************************************************************/
    CNVSTimer& GetNVSTimer()
    {
        return m_objTimer;//lint !e1536
    };

    /**************************************************************************
    * name       : GetLocalIp
    * description: ��ȡ����IP
    * input      : NA
    * output     : NA
    * return     : ����IP�ַ���
    * remark     : NA
    **************************************************************************/
    const char* GetLocalIp();

    /**************************************************************************
    * name       : SetLocalIp
    * description: ���ñ���IP
    * input      : pszIp    ����IP
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void		SetLocalIp(const char* pszIp);

private:
    CStatusConfig m_objStatusConfig;  //����״̬��
    CConnMgr m_objConnMgr;       //����ͨ����
    CNVSTimer m_objTimer;         //��ʱ��
    char m_szLocalIp[IVS_IP_LEN];   //����IP
};

//֪ͨ�ص�״̬��
class CStatusNotifyCallBack
{
public:

    /**************************************************************************
    * name       : CStatusNotifyCallBack
    * description: Default constructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusNotifyCallBack();

    /**************************************************************************
    * name       : ResetStatus
    * description: ����״̬
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ResetStatus();

    /**************************************************************************
    * name       : SetStatusValue
    * description: ����״ֵ̬
    * input      : pFunCallBack    �ص�����ָ��
                   pUserData       �û�����ָ��
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void SetStatusValue(PCUMW_CALLBACK_NOTIFY pFunCallBack, void* pUserData);

    /**************************************************************************
    * name       : NotifyCallBack
    * description: ���ûص�����
    * input      : stInfo  ֪ͨ��Ϣ�ṹ��
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyCallBack(CUMW_NOTIFY_INFO& stInfo);

    /**************************************************************************
    * name       : NotifyAsynRet
    * description: ֪ͨ�첽����
    * input      : stNotifyInfo        �첽����֪ͨ�ṹ��
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyAsynRet
    (   const CUMW_NOTIFY_ASYNC_RET& stNotifyInfo);

    /**************************************************************************
    * name       : NotifyAsynRet
    * description: ֪ͨ�첽����
    * input      : ulAsynSeq        �첽�������
                   lAsynRetCode     �첽������
                   ulBusinessId     ҵ��ID
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyAsynRetNoInfo
    (   IN unsigned long ulAsynSeq,
        IN long          lAsynRetCode,
        IN unsigned long ulBusinessId = 0);

    /**************************************************************************
    * name       : NotifyBusinessAsynRet
    * description: ͨý��ҵ���첽����֪ͨ
    * input      : ulAsynSeq        �첽�������
                   lAsynRetCode     �첽������
                   ulBusinessId     ҵ��ID
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyBusinessAsynRet
    (   IN unsigned long ulAsyncSeq,
        IN long          lAsyncRetCode,
        IN unsigned long ulBusinessID);

    /**************************************************************************
    * name       : NotifyEvent
    * description: �¼�֪ͨ
    * input      : ulEventType     �¼�����
                   ulEventPara     �¼�����
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyEvent
    (   IN unsigned long ulEventType,
        IN unsigned long ulEventPara = 0);

    /**************************************************************************
    * name       : NotifyDecoderStatus
    * description: ������״̬֪ͨ
    * input      : stNotifyInfo  ֪ͨ����
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyDecoderDisConnect
    (   IN CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO& stNotifyInfo);

    /**************************************************************************
    * name       : NotifyDecoderStatus
    * description: ������״̬֪ͨ
    * input      : stNotifyInfo  ֪ͨ����
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyDecoderRegister
    (   IN CUMW_NOTIFY_REMOTE_DECODER_STATUS& stNotifyInfo);

    /**************************************************************************
    * name       : NotifyDecoderUnRegister
    * description: ������״̬֪ͨ
    * input      : stNotifyInfo  ֪ͨ����
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long NotifyDecoderUnRegister
    (   IN CUMW_NOTIFY_REMOTE_DECODER_OFF_LINE_INFO& stNotifyInfo);
private:
    PCUMW_CALLBACK_NOTIFY m_pFunCallBack; //�ص�����ָ��
    void*                   m_pUserData;    //�û�����ָ��
};

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮��ʱ������״̬����ض���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    ///////////////////////////////////////////////////////////////////////////
    /// @brief ����������
    ///
    /// Description.
    ///////////////////////////////////////////////////////////////////////////
    enum TRIGGER_TYPE
    {
        TRIGGER_TYPE_DECODER_CONN_TIME_OUT //���������ӳ�ʱ������
    };
}

///////////////////////////////////////////////////////////////////////////////
/// @brief ��ʱ������״̬��
///
/// Description:
///////////////////////////////////////////////////////////////////////////////
class CStatusTrigger : public ITrigger
{
private:

    /**************************************************************************
    * name       : CStatusTrigger
    * description: ��ֹ����Ĭ�Ϲ��캯��
    * input      : enTriggerType   ����������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusTrigger();

public:

    /**************************************************************************
    * name       : CStatusTrigger
    * description: ��ֹ����Ĭ�Ϲ��캯��
    * input      : enTriggerType   ����������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusTrigger
    (
        IN CUMW::TRIGGER_TYPE enTriggerType
    );

    /**************************************************************************
    * name       : onTrigger
    * description: ������ʱ�¼�����
    * input      : pArg        ���崦����
                   ullScales   ��ʱ��ָ��
                   enStyle     ��������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual void onTrigger
    (   IN void*           pArg,
        IN ULONGLONG       ullScales,
        IN TriggerStyle    enStyle);

private:

    /**************************************************************************
    * name       : HandleTriggerDecoderConnTimeOut
    * description: ������������ӳ�ʱ������
    * input      : pArg    ���崦����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void         HandleTriggerDecoderConnTimeOut
    (   IN void* pArg) const;

private:
    CUMW::TRIGGER_TYPE m_enTriggerType; //����������
};

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮�߳�״̬����ض���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    //�̴߳���������
    typedef DWORD (WINAPI * PTHREAD_FUN)(LPVOID lpParameter);
}

///////////////////////////////////////////////////////////////////////////////
/// @brief �߳�״̬��
///
/// Description:
///////////////////////////////////////////////////////////////////////////////
class CStatusThread
{
public:

    /**************************************************************************
    * name       : CStatusThread
    * description: Default constructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    CStatusThread();

    /**************************************************************************
    * name       : ~CStatusThread
    * description: Destructor
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    virtual ~CStatusThread();

    /**************************************************************************
    * name       : StartThread
    * description: �����߳�
    * input      : pThreadFun      �����̺߳���ָ��
                   pThreadEntryPara    �߳���ڲ���
                   ulQueueSize     ���г���
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long StartThread
    (   IN CUMW::PTHREAD_FUN   pThreadFun,
        IN void*               pThreadEntryPara,
        IN unsigned long       ulQueueSize = 0);

    /**************************************************************************
    * name       : StopThread
    * description: ֹͣ�߳�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void StopThread();

    /**************************************************************************
    * name       : AddEvent
    * description: ����¼�
    * input      : enEventType    �¼�����
                   pEventInfo     �¼���Ϣָ��
                   ulEventInfoLen �¼���Ϣ���ݳ���
    * output     : NA
    * return     : ������
    * remark     : NA
    **************************************************************************/
    long AddEvent
    (   IN CUMW::EVENT_TYPE    enEventType,
        IN void*               pEventInfo = NULL,
        IN unsigned long       ulEventInfoLen = 0);

public:     //���³�Ա���������ֱ�ӷ���
    //ͬ���¼�����
    CUMW::CEventSyncQueue m_objEventSyncQueue;

    //�����¼��߳��Ƿ�����
    BOOL m_bThreadRun;

    bool m_bIdle;

    //������ID��20λ��������\0��
    char m_szDecoderID[CUMW_DECODER_ID_LEN + 1];

private:    //���³�Ա����ֻ��ͨ����Ա��������
    //�����¼��߳̾��
    HANDLE m_hThread;

    //���д�С
    unsigned long m_ulQueueSize;
};

#endif //CUMW_STATUS_CLASSES_H
