/********************************************************************
 filename    :    ControlBus.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ���ƶ���
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#ifndef __CONTROL_BUS_H__
#define __CONTROL_BUS_H__

#include "hwsdk.h"
#include "ivs_error.h"

/*!< ý�����ĸ���ͷ�ļ� */
#include "StreamMgr.h"

/*!< RDAЭ��ͷ�ļ� */
#include "RDAProt.h"

#define THREAD_MAX_COUNT 81

namespace CUMW
{
    //֪ͨ���Ͷ���
    typedef enum
    {
        LOGIN_STATUS_INIT = 0,  ///<��ʼ��
        LOGIN_STATUS_INITED    ///<�ѳ�ʼ��
    } LOGIN_STATUS;

    //�¼����͵���ʾ�ַ�����󳤶�
    const unsigned long STRING_LOGIN_STATUS = sizeof("CUMW Query Platform Info Success");

    //�¼����͵���ʾ�ַ������鶨��
    const char STR_LOGIN_STATUS[][STRING_LOGIN_STATUS]
    = {
        "CUMW Not Initial",
        "CUMW Initialize Success",
        "CUMW Relogining",
        "CUMW Register Success",
        "CUMW Query Platform Info Success",
        "CUMW Logined Success",
    };
}

typedef struct EventThreadPara
{
    int i;
    void* pThread;
}EVENT_THREAD_PARA;

//ControlBus�ӿ�ʵ����
class CControlBus
{
public:

    //ҵ���첽��Ϣ�ṹ��
    typedef struct _BUSINESS_ASYNC_INFO
    {
        unsigned long       ulAsyncSeq;     //�첽���
        CUMW::ASYNC_TYPE    enAsyncType;    //�첽����
        unsigned long       ulBusinessId;   //ҵ��ID
        void*               pParameter;     //����ָ��

        _BUSINESS_ASYNC_INFO()
        {
            ulAsyncSeq   = 0;
            enAsyncType  = CUMW::ASYNC_TYPE_BUSINESS_START;
            ulBusinessId = 0;
            pParameter = NULL;
        }
    } BUSINESS_ASYNC_INFO;

    CControlBus();

    virtual ~CControlBus();

    /**************************************************************************
    * name       : InitControlBus
    * description: ��ʼ��ControlBus
    * input      : pParaInfo   ��ʼ������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32 InitControlBus(PCUMW_CALLBACK_NOTIFY pFunNotifyCallBack, void* pUserData);

    /**************************************************************************
    * name       : ReleaseControlBus
    * description: �ͷ�ControlBus
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32 ReleaseControlBus();

    /**************************************************************************
    * name       : IniBusiness
    * description: ��ʼ��������
    * input      : pParaInfo   ����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         IniBusiness(IPI_INIT_BUSINESS_REQ* pParaInfo);

    /**************************************************************************
    * name       : HandleEventIniBusiness
    * description: �����ʼ����������Ϣ
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         HandleEventIniBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo);

    /**************************************************************************
    * name       : StartBusiness
    * description: Զ�̽�������ʼ����
    * input      : pParaInfo   ����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         StartBusiness(IPI_START_BUSINESS_REQ* pParaInfo);

    /**************************************************************************
    * name       : HandleEventStartDecoder
    * description: ����������������Ϣ
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         HandleEventStartBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo);

    /**************************************************************************
    * name       : StopBusiness
    * description: ֹͣ����
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         StopBusiness(IPI_STOP_BUSINESS_REQ* pParaInfo);

    /**************************************************************************
    * name       : HandleEventStopBusiness
    * description: ����ֹͣ�������Ϣ
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32         HandleEventStopBusiness(BUSINESS_ASYNC_INFO& stAsyncInfo);

    /**************************************************************************
    * name       : PlayAudio
    * description: ����/ֹͣ��������
    * input      : pParaInfo ��ʼ������
    * output     : void
    * return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
    * remark     : NA
    **************************************************************************/
    IVS_INT32         PlayAudio(IPI_AUDIO_CONTROL_REQ *pParaInfo);

    /**************************************************************************
    * name       : PlayAudio
    * description: ������/ֹͣ����������Ϣ
    * input      : pParaInfo ��ʼ������
    * output     : void
    * return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
    * remark     : NA
    **************************************************************************/
    IVS_INT32         HandleEventPlayAudio(BUSINESS_ASYNC_INFO& stAsyncInfo);

    /**************************************************************************
    * name       : SetVolume
    * description: ��������
    * input      : pParaInfo ��ʼ������
    * output     : void
    * return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
    * remark     : NA
    **************************************************************************/
    IVS_INT32         SetVolume(IPI_TUNEUP_VOLUME_REQ *pParaInfo);

    /**************************************************************************
    * name       : SetVolume
    * description: ��������������Ϣ
    * input      : pParaInfo ��ʼ������
    * output     : void
    * return     : RET_OK-�ɹ�     RET_FAIL-ʧ��
    * remark     : NA
    **************************************************************************/
    IVS_INT32         HandleEventSetVolume(BUSINESS_ASYNC_INFO& stAsyncInfo);

private:

    //ҵ��ID��ҵ������ӳ�����
    typedef map <unsigned long, CBusiness*> MAP_ID_TO_BUSINESS;

    //BEGIN R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯
    //ҵ��ID��
    typedef vector <unsigned long> VECTOR_BUSINESS_ID;

    //ҵ��ID���乲��ҵ��ID���ӳ�����
    typedef map <unsigned long, VECTOR_BUSINESS_ID> MAP_SHARED_BUSINESS_ID;
    //END   R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯

private:

    /**************************************************************************
    * name       : InitControlBusResource
    * description: ��ʼ��ControlBus����Դ
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           InitControlBusResource();

    /**************************************************************************
    * name       : ReleaseControlBusResource
    * description: �ͷ�ControlBus����Դ
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    void                           ReleaseControlBusResource();

    /**************************************************************************
    * name       : InitNvsTimer
    * description: ��ʼ����ʱ��
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           InitNvsTimer() const;

    /**************************************************************************
    * name       : InitConnMgr
    * description: ��ʼ��ͨ�Ź���ģ��
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           InitConnMgr() const;

    /**************************************************************************
    * name       : ProcEventThreadEntry
    * description: �����¼��߳����
    * input      : pPara   �߳���ڲ���
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    static unsigned long __stdcall ProcEventThreadEntry(void* pPara);

    /**************************************************************************
    * name       : ProcEventThread
    * description: �����¼��߳�
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    void                           ProcEventThread(int i);

    /**************************************************************************
    * name       : AddBuesinessEvent
    * description: ���ҵ���¼�
    * input      : ulAsynSeq      �첽���
                   enAsynType  �첽����
                   ulBusinessId    ҵ��ID
                   pParameter  ����ָ�룬ȡ���� �첽����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           AddBuesinessEvent
    (   IN unsigned long       ulAsynSeq,
        IN CUMW::ASYNC_TYPE    enAsynType,
        IN unsigned long       ulBusinessId,
        IN void*               pParameter = NULL);

    /**************************************************************************
    * name       : AddGeneralAsynEvent
    * description: ���ͨ���첽�¼�
    * input      : ulAsynSeq      �첽���
                   enAsynType  �첽����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           AddGeneralAsynEvent
    (   IN unsigned long    ulAsynSeq,
        IN CUMW::ASYNC_TYPE enAsynType);

    /**************************************************************************
    * name       : HandleNotifyCallBack
    * description: ����֪ͨ�ص�
    * input      : pstNotifyInfo   ֪ͨ��Ϣָ��
                   pUserData       �û�����ָ��
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    static IVS_INT32 __stdcall          HandleNotifyCallBack(CUMW_NOTIFY_INFO* pstNotifyInfo, void* pUserData);

    /**************************************************************************
    * name       : CheckStatus
    * description: ����Ƿ�ﵽ�ﵽԤ��״̬
    * input      : enExpectedStatus Ԥ��״̬
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           CheckStatus(CUMW::LOGIN_STATUS enExpectedStatus);

    /**************************************************************************
    * name       : StartBusiness
    * description: ����ҵ��
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           StartBusiness
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : RunBusiness
    * description: ����ҵ��
    * input      : objBusiness     ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           RunBusiness
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : StopBusinessWhole
    * description: ֹͣ����ҵ�񣬰�������ֹͣ���ƽ̨
    * input      : objBusiness     ��ֹͣ��ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           StopBusinessWhole
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : GetNewBusinessId
    * description: ��ȡ�µ�ҵ��ID
    * input      : objBusiness     ��ֹͣ��ҵ���������
    * output     : NA
    * return     : ҵ��ID
    * remark     : NA
    **************************************************************************/
    unsigned long                  GetNewBusinessId();

    /**************************************************************************
    * name       : HandleEventBusinessStop
    * description: ����ҵ��ID��ȡҵ�����
    * input      : ulBusinessId    ҵ��ID
    * output     : pBusiness       ���ص�ҵ�����ָ��
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           GetBusinessById
    (   IN unsigned long    ulBusinessId,
        OUT CBusiness*&     pBusiness);

    /**************************************************************************
    * name       : AddBusinessCtrlEvent
    * description: ���ҵ������¼�
    * input      : ulAsyncSeq                �첽���
                   ulBusinessID              ҵ��ID
                   pParaInfo                 ���Ʋ���
                   ulParaLength              ���Ʋ�������
                   enCtrlCode                ������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           AddBusinessCtrlEvent
    (   IN const unsigned long              ulAsyncSeq,
        IN const unsigned long              ulBusinessID,
        IN const void*                      pParaInfo,
        IN const unsigned long              ulParaLength,
        IN const CUMW::BUSINESS_CTRL_CODE   enCtrlCode);

    /**************************************************************************
    * name       : GetBusinessByID4Invite
    * description: ��ȡý��ҵ��re-invite���ҵ��
    * input      : ulBusinessID   ҵ��ID
    * output     : pBusiness      �첽��Ϣ
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           GetBusinessById4Invite
    (   IN unsigned long ulBusinessID,
        IO CBusiness*&   pBusiness);

    /**************************************************************************
    * name       : StopAllBusiness
    * description: ֹͣ����ҵ��ֻ���ڴ����¼��߳��д���
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    void                           StopAllBusiness();

    /**************************************************************************
    * name       : StopAllReuseBusiness
    * description: ָֹͣ��ҵ��ID�����и���ҵ��
    * input      : objBusiness �����õ�ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    void                           StopAllReuseBusiness
    (   IN CBusiness& objBusiness);

private:

    /**************************************************************************
    * name       : HandleEventBusiness
    * description: ����ý��ҵ���¼�
    * input      : pEventInfo      �¼���Ϣָ��
                   ulEventInfoLen  �¼���Ϣ���ݳ���
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           HandleEventBusiness
    (   IN void*           pEventInfo,
        IN unsigned long   ulEventInfoLen);

    /**************************************************************************
    * name       : HandleEventBusinessStart
    * description: ����ý��ҵ��ʼ�¼�
    * input      : pParameter  ����ָ��
                   ulAsyncSeq  �첽���
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           HandleEventBusinessStart
    (   IN void*           pParameter,
        IN unsigned long   ulAsyncSeq);

    /**************************************************************************
    * name       : HandleEventBusinessRun
    * description: ����ý��ҵ�������¼�
    * input      : ulBusinessId    ҵ��ID
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           HandleEventBusinessRun
    (   IN unsigned long ulBusinessId);

private:    //֪ͨ������

    /**************************************************************************
    * name       : HandleNotifyAsyncRet
    * description: �����첽����֪ͨ
    * input      : stNotifyInfo   ֪ͨ��Ϣ
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           HandleNotifyAsyncRet
    (   IN CUMW_NOTIFY_INFO& stNotifyInfo);

    /**************************************************************************
    * name       : HandleNotifyEvent
    * description: �����¼�֪ͨ
    * input      : stNotifyInfo   ֪ͨ��Ϣ
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    IVS_INT32                           HandleNotifyEvent
    (   IN CUMW_NOTIFY_INFO& stNotifyInfo);

    IVS_INT32 GetIdleThread(long& lIndex);
    IVS_INT32 FindThread(const char* pszDecoderID, long& lIndex);

private:

    //֪ͨ�ص�����״̬��
    CStatusNotifyCallBack m_objStatusNotifyCallBack;

    //�����¼��߳�״̬��
    CStatusThread m_objProcEventThread[THREAD_MAX_COUNT];

    //�ӿ���
    CUMW::CCriticalSection m_objInterfaceLock;

    //StreamMgr����ָ��
    CStreamMgr* m_pStreamMgr;

    //�Ƿ��ѵ�¼
    CUMW::LOGIN_STATUS m_enLoginStatus;

    BOOL m_bisFirstLogin;

    //ҵ��ID
    unsigned long m_ulBusinessId;

    //ҵ��ID��ҵ������ӳ���
    MAP_ID_TO_BUSINESS m_mapIdToBusiness;

    //BEGIN R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯
    //ҵ��ID���乲��ҵ��ID���ӳ�����
    MAP_SHARED_BUSINESS_ID m_mapSharedBusinessId;
    //END   R002C01SPC100 ADD 2011-07-26 ligengqiang 00124479 for ֧����ƵPayloadType��̬�仯

    //��
    VOS_Mutex *m_pThreadMutex;     

    //ҵ��ID��
    VOS_Mutex* m_pBusinessIdMutex;		

    EVENT_THREAD_PARA m_ThreadPara[THREAD_MAX_COUNT];
};

#endif //__CONTROL_BUS_H__

