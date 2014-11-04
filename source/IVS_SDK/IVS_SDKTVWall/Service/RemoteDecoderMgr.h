/********************************************************************
 filename    :    RemoteDecoderMgr.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ������������
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#ifndef REMOTE_DECODER_MGR_H
#define REMOTE_DECODER_MGR_H

/*!< ģ�����ͷ�ļ� */
#include "vos.h"

/*!< �ڴ������غ�����ͷ�ļ� */
#include "vos_common.h"

/*!< ������Ϣͷ�ļ� */
#include "vos_config.h"

/*!< �����������Ͷ���ͷ�ļ� */
#include "vos_basetype.h"

/*!< ģ�������õĻ���ͷ�ļ� */
#include "CumwCommon.h"

/*!< RDAЭ��ͷ�ļ� */
#include "RDAProtStack.h"

//Զ�̽�������Ϣ
typedef CUMW_NOTIFY_REMOTE_DECODER_STATUS CUMW_DECODER_INFO;

//��Զ�̽�����ͨ���������
#define WAITEVENT_TIMEOUT (5 * 1000)   //�ȴ������¼�����ʱʱ��5��
#define MAX_RECV_WAITTIME (5 * 1000)   //recv��ʱ����ʱ�¼�Ϊ5��
#define MAX_HEARTBEAT_INTERVAL (4 * 1000) //���������ʱ��4��(DTS2013073008710)  //(60 * 1000)  //���������ʱ��60��
#define MAX_HEARTBEAT_RETRY_COUNT 3    //����������Դ���

//RDAЭ����ض���
#define RDA_MSGHEAD_LEN sizeof(RDAMsgHead)
#define MAX_RDAMSG_LEN (16 * 1024)  //�����Ϣ����
#define MAX_RDAPACKET_NUM 81         //���ͬʱʹ��81����
#define MAX_LIST_NUM 81         //�������б���
#define MAX_RD_NUM 81         //���֧��81��������
#define MAX_RDMGR_EVENT_NUM 1024       //CRemoteDecoderMgr�е���Ϣ���г���
#define RDA_PACKTAG 0x8000
#define RDA_REQPACK 0
#define RDA_RSPPACK 1
#define TRANSFER_REQNO_TO_RSPNO(x) (RDA_PACKTAG | (x))
#define TRANSFER_RSPNO_TO_REQNO(x) ((~RDA_PACKTAG) & (x))

#define GARBAGE_PARAM 0        //��Ч��������

#define RDA_ERROR_CODE_OK 0x00
#define RDA_ERROR_DISCONNECT 0x01

typedef enum tagRDA_PACKETMGR_EVENT_TYPE
{
    RDA_PACKETMGR_EVENT_CHECK_RD_INVALID = 0x00, //������������,���������
    RDA_PACKETMGR_EVENT_RD_CONN_TIMEO = 0x01, //������ֻ�ǽ������ӣ�������Ϣ��������ʱ
    RDA_PACKETMGR_EVENT_RECV_RD_DATA   = 0x02,   //������������͹���������
    RDA_PACKETMGR_EVENT_SEND_RD_DATA   = 0x03,   //��Զ�̽�������������
    RDA_PACKETMGR_EVENT_SEND_HEARTBEAT = 0x04, //��Զ�̽�������������
    RDA_PACKETMGR_EVENT_EXIT_THREAD = 0x05, //�˳��߳�

    RDA_PACKETMGR_EVENT_BUTT              //����
} RDA_PACKETMGR_EVENT_TYPE;

//����RDAPacket������ͬ����Ϣ�ĵȴ�����
class CRDAPacket
{
public:
    CRDAPacket();
    virtual ~CRDAPacket();

public:

    //��ʼ��
    IVS_INT32 InitRDAPacket(void);

    //�ȴ������¼�
    IVS_INT32 WaitReadPacketEv(void);

    //���������¼�
    IVS_INT32 SetReadPacketEv(void);

public:
    IVS_ULONG m_ulRecvBuffLen;     //���յ������ݳ��ȣ�������ͷ
    IVS_CHAR      * m_pRecvBuff;         //���յ������ݣ�������ͷ

    IVS_ULONG m_ulSendBuffLen;     //���͵����ݳ��ȣ�������ͷ
    IVS_CHAR      * m_pSendBuff;         //���͵����ݣ�������ͷ

    IVS_USHORT m_usPacketNo;        //�����

private:
    VOS_Event * m_pReadEvent;    //�����¼�������֪ͨ�ȴ�����ȡ��������
};

typedef std::list <CRDAPacket *> ListRDAPacket;
typedef ListRDAPacket::iterator  ListRDAPacketIte;

//����RDAPacket�ķ��䡢�ͷ�
class CRDAPacketMgr
{
public:
    CRDAPacketMgr() : m_pMutexList(NULL), m_pArrayRDAPacket(NULL)
    {
    };
    ~CRDAPacketMgr()
    {
        //ȷ�ϲ�������쳣��VOS_DELETE�ᱣ����
        VOS_DELETE(m_pArrayRDAPacket, MULTI);/*lint !e1551��ȷ�ϲ�������쳣��VOS_DELETE����try-catch�����쳣��*/
        if (NULL != m_pMutexList)
        {
            (void)VOS_DestroyMutex(m_pMutexList);
        }

        m_pMutexList = NULL;
        m_pArrayRDAPacket = NULL;
    };

public:

    //��ʼ��
    IVS_INT32		InitRDAPacketMgr(void);

    //��ȡ��
    CRDAPacket* GetRDAPacket(void);

    //�黹��
    void		ReleaseRDAPacket( CRDAPacket* pRDAPacket);

    //���ݰ���Ų���CRDAPacket
    CRDAPacket* FindRDAPacket(IVS_USHORT usPacketNo);

private:
    ListRDAPacket m_listFreeRDAPacket;     //����RDA������
    ListRDAPacket m_listWorkRDAPacket;     //����RDA������

    VOS_Mutex    * m_pMutexList;        //������
    CRDAPacket   * m_pArrayRDAPacket;   //�������ڴ��ַ����Ҫ��Ϊ�˷��������ͷ�
};

//�����¼�����
class CCUMWEvent
{
public:
    CCUMWEvent(): m_ulEventType(0)
    {
    };
    virtual ~CCUMWEvent(){};

public:
    IVS_ULONG m_ulEventType; //��Ϣ����
};

class CDecoderConnHandle;

//�Զ����¼�
class CDecoderMgrEvent : public CCUMWEvent
{
public:
    CDecoderMgrEvent() : m_pDecoderConnHandle(NULL), m_pRDAPacket(NULL)
    {
    };
    ~CDecoderMgrEvent()
    {
        m_pDecoderConnHandle = NULL;
        m_pRDAPacket = NULL;
    };

public:
    CDecoderConnHandle * m_pDecoderConnHandle; //�ͻ�������ָ��
    CRDAPacket         * m_pRDAPacket;
};

typedef CSyncQueue <CCUMWEvent> CDecoderMgrEventQueue;

//������ӳ�ʱ��
class CCheckConnTimeOTrigger : public ITrigger
{
public:

    //���ͻ������ӳ�ʱ��δ��������
    void onTrigger(void* pArg, ULONGLONG ullScales,
                   TriggerStyle style);
};

class CRemoteDecoder;

//�ͻ������Ӷ���
class CDecoderConnHandle : public CTcpConnHandle
{
public:
    CDecoderConnHandle();
    ~CDecoderConnHandle();

public:

    //��ʼ��
    LONG		Init(CDecoderMgrEventQueue* pEventQ);

    //һ��tcp������������ʱ��Ҫ��������
    void		handle_recv(void);

    //һ��tcp���������Է�������ʱ��Ҫ��������
    void handle_send(void){};

    /*�������ӳ�ʱʱ�䣬��Ҫ��Ϊ�˽���������ֹ���:
        һ���ͻ���ֻ�����ӵ����أ�����û�з���Ϣ*/

    /**
     * Description:  SetTimeOut().    �������ӳ�ʱʱ�䣬��Ҫ��Ϊ�˽���������ֹ���:
     *        һ���ͻ���ֻ�����ӵ����أ�����û�з���Ϣ
     * @param  [in]  ulInterval  ��ʱʱ������λΪ��
     * @return      void
     */
    void		SetTimeOut
    (   IN unsigned long ulInterval);

    //���ע����Ϣ�Ѿ�������ȡ����ʱ����
    void		CancelTimeOut(void);

    //һ���ͻ������ӳ�ʱ�ˣ�Ҫ��������
    void		HandleTimeout(void);

    //�ŵ��������ǰ��Ҫ�������ӵ����ü�����1
    LONG		AddRef(void);

    //����������󣬽����ü�����1��ͬʱ���ݷ���ֵ�жϴ������Ƿ�Ҫɾ��
    VOS_BOOLEAN ReleaseRef(void);

    //BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

    /**
     * Description:  GetLocalIp().    ��ȡ����IP
     * @param  [out]  strIp  ����IP
     * @return      void.
     */
    void		GetLocalIp(string& strIp);
    //END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

public:
    volatile VOS_BOOLEAN m_bNeedUse;      //�Ƿ���Ҫʹ��
    CRemoteDecoder         * m_pRemoteDecoder; //����������

private:
    CDecoderMgrEventQueue  * m_pEventQ;            //�¼�����
    volatile ULONG m_ulReference;        //���ü���
    VOS_Mutex              * m_pMutexDecoderConnHandle; //��Ҫ����ReleaseRef���ٽ�������

    CStatusTrigger m_objTriggerConnTimeOut;    //���ӳ�ʱ������
    //BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����
    char m_szLocalIp[IVS_IP_LEN];   //����IP
    //END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����
};

typedef std::list <CDecoderConnHandle *> ListDecoderConnHandle;
typedef ListDecoderConnHandle::iterator  ListDecoderConnHandleIte;

//����ͻ���������Դ����
class CDecoderConnHandleMgr
{
public:
    CDecoderConnHandleMgr();
    ~CDecoderConnHandleMgr();

public:

    //��ʼ�� CDecoderConnHandleMgr
    LONG				Init(CDecoderMgrEventQueue* pEventQ);

    //��ȡһ���ͻ�������
    CDecoderConnHandle* GetConnHandle(void);

    //�黹һ���ͻ�������
    void				ReleaseConnHandle(CDecoderConnHandle* pDecoderConnHandle);

private:
    ListDecoderConnHandle m_ListDecoderConnHandle;  //�ͻ������Ӷ���
    VOS_Mutex             * m_pMutexList;             //������
    CDecoderConnHandle    * m_pArrayDecoderConnHandle; //�����ڴ��ַ
};

//tcp����������
class CDecoderMgrServerHandle : public CTcpServerHandle
{
public:
    CDecoderMgrServerHandle() : m_pDecoderConnHandleMgr(NULL)
    {

    };
    virtual ~CDecoderMgrServerHandle()
    {
        m_pDecoderConnHandleMgr = NULL;
    };

public:

    //��ʼ�� CDecoderMgrServerHandle
    LONG Init(CDecoderConnHandleMgr* pDecoderConnHandleMgr);

    LONG handle_accept(const CNetworkAddr* pRemoteAddr,
                       CTcpConnHandle* &   pTcpConnHandle);

private:
    CDecoderConnHandleMgr* m_pDecoderConnHandleMgr;
};

//Զ�̽�������
class CRemoteDecoder
{
public:
    CRemoteDecoder() : m_pDecoderConnHandle(NULL), m_ulHeartBeatCount(0)
    {
        memset(&m_stDecoderInfo, 0, sizeof(m_stDecoderInfo));
    };

    ~CRemoteDecoder()
    {
        m_pDecoderConnHandle = NULL;
    };

public:
    CUMW_DECODER_INFO m_stDecoderInfo;              //��������Ϣ
    CDecoderConnHandle*   m_pDecoderConnHandle;     //��������Ϣ
    volatile ULONG m_ulHeartBeatCount;             //��������
};

typedef std::list <CRemoteDecoder *> ListRemoteDecoder;
typedef ListRemoteDecoder::iterator  ListRemoteDecoderIte;

//Զ�̽�������Դ����
class CRemoteDecoderArry
{
public:
	CRemoteDecoderArry():m_pMutexList(NULL)
		,m_pArrayRemoteDecoder(NULL)
		,m_pEventQ(NULL)
		,m_pDecoderConnHandleMgr(NULL)
    {

    };
    ~CRemoteDecoderArry();

public:

    //��ʼ��
    LONG				Init(CDecoderMgrEventQueue*                pEventQ,
                             CDecoderConnHandleMgr *               pDecoderConnHandleMgr);

    //��ȡһ����Զ�̽���������
    CRemoteDecoder*		GetRemoteDecoder(void);

    //�ͷ�Զ�̽���������
    void				ReleaseRemoteDecoder(CRemoteDecoder* pRemoteDecoder);

    //��ȡ���Ӿ��
    CDecoderConnHandle* GetConnHandle(const char* pszDecoderID);

    //��ȡ��������Ϣ
    LONG				GetDecoderInfo
    (   char*               pszDecoderID,
        CUMW_DECODER_INFO&  stDecoderInfo);

    //���Զ�̽������Ƿ���
    void				CheckHeartBeat( void );

    //�����������
    void				ClearHeartBeatCount( char* pszDecoderID );

    //��ӡ��������Ϣ
    void				PrintDecoderInfo
    (   const CRemoteDecoder& objRemoteDecoder, bool bChannel=true) const;

    //�ͷ�Initʱ���ɵ���Դ
    void				Release();

    //BEGIN V100R001C05 ADD 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���
    //���ݽ�����ID���ҽ����������еĶ�Ӧ��������Ϣ�ṹ��
    long				FindDecoderInfoByID(const char*                pszDecoderID,
                                            CRemoteDecoder*&           pDecoderInfo);
    //END V100R001C05 ADD 2010-01-18 zhaoyuzhen z00137994 for ���������ظ���

private:
    ListRemoteDecoder m_FreeListRemoteDecoder;
    ListRemoteDecoder m_WorkListRemoteDecoder;

    VOS_Mutex             *m_pMutexList;        //������
    //�������ڴ��ַ����Ҫ��Ϊ�˷��������ͷ�
    CRemoteDecoder        *m_pArrayRemoteDecoder;
    CDecoderMgrEventQueue *m_pEventQ;
    CDecoderConnHandleMgr *m_pDecoderConnHandleMgr; //������Դ����
};

//������Ҫ���ڴ���Զ�̽��������͹�������Ϣ
class CRemoteDecoderReqProc
{
public:
    CRemoteDecoderReqProc();
    ~CRemoteDecoderReqProc();

public:

    //��ʼ��
    long  Init
    (   IN PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack,
        IN void*                   pCallBackUserData,
        CRDAPacketMgr*             pRDAPacketMgr,
        CDecoderConnHandleMgr*     pDecoderConnHandleMgr,
        CRemoteDecoderArry*        pRemoteDecoderArry,
        CRDAProtStack*             pRDAProtStack);

    //�˳�
    void  Exit(void);

    //�������������������Ϣ
    LONG  ProcMsg( CDecoderMgrEvent* pEventRecv);

    //����������
    VOID  RemoteDecoderDisConnect( CDecoderConnHandle *pDecoderConnHandle, BOOL bUnRigister = FALSE );

protected:

    //����Ƿ��Ѿ���ʼ��
    BOOL  CheckIsInited();

private:

    //����ע����Ϣ
    LONG  ProcRegisterMsg(CDecoderMgrEvent& objEvent);

    //����ע����Ϣ
    LONG  ProcUnRegisterMsg(CDecoderMgrEvent* pEventRecv);

    //����ͬ����Ϣ����Ӧ
    LONG  ProcSyncRspMsg(CDecoderMgrEvent* pEventRecv);

    /**************************************************************************
    * name       : ProcHeartBeatReq
    * description: ��������������Ϣ
    * input      : objEvent    �¼�����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    long  ProcHeartBeatReq
    (   IN CDecoderMgrEvent& objEvent);

    /**************************************************************************
    * name       : ProcHeartBeatRsp
    * description: ����������Ӧ��Ϣ
    * input      : objEvent    �¼�����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    long  ProcHeartBeatRsp();

    /**************************************************************************
    * name       : ProcErrorNotifyReq
    * description: ��������ϱ�������Ϣ
    * input      : objEvent    �¼�����
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    long  ProcErrorNotifyReq
    (   IN CDecoderMgrEvent& objEvent);

    /**************************************************************************
    * name       : GetDecoderInfoFromRegisterReq
    * description: ��ע��������Ϣ����ȡ��������Ϣ
    * input      : ojRegisterReq   ע��������Ϣ
    * output     : stDecoderInfo   ��������Ϣ
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    long  GetDecoderInfoFromRegisterReq
    (   IN CRDARegisteReq&      ojRegisterReq,
        OUT CUMW_DECODER_INFO&  stDecoderInfo) const;

private:
    char              * m_pRecvBuffer;  //�������ݻ�����
    ULONG m_ulRecvBuffLen; //�������ݻ���������

    char              * m_pSendBuffer;  //�������ݻ�����
    ULONG m_ulSendBuffLen; //�������ݻ���������

    CRDAPacketMgr     * m_pRDAPacketMgr; //��������ָ��
    CDecoderConnHandleMgr * m_pDecoderConnHandleMgr;
    CRemoteDecoderArry    * m_pRemoteDecoderArry;

    CRDAProtStack         * m_pRDAProtStack;

    //֪ͨ�ص�����״̬��
    CStatusNotifyCallBack m_objStatusNotifyCallBack;
};

//����̽�ⴥ����
class CCheckHeartBeatTrigger : public ITrigger
{
public:

    //��ʼ��
    LONG Init(CDecoderMgrEventQueue* pEventQ);

    //������������¼��������
    void onTrigger(void* pArg, ULONGLONG ullScales,
                   TriggerStyle style);

private:
    CDecoderMgrEventQueue* m_pEventQ; //�¼�����ָ��
};

//��ʼ��������Ϣ�ṹ
typedef struct
{
    CConnMgr*       pConnMgr;       //����ͨ��ƽ̨
    //BEGIN R001C05LYYS01 MOD 2010-03-08 ligengqiang 00124479 for ��������״̬��
    CStatusConfig*  pStatusConfig;  //����״̬��
    //END   R001C05LYYS01 MOD 2010-03-08 ligengqiang 00124479 for ��������״̬��
    CNVSTimer*      pTimer;         //��ʱ��
    VOS_Mutex*      pCumwMutex;     //�м����
} INITPARAMINFO;

//Զ�̽���������
class CRemoteDecoderMgr
{
public:

    CRemoteDecoderMgr();

    virtual ~CRemoteDecoderMgr();

    /**************************************************************************
    * name       : InitRemoteDecoderMgr
    * description: ��ʼ��RemoteDecoderMgr
    * input      : pFunNotifyCallBack  �ص�����ָ��
                   pCallBackUserData   �ص������û�����ָ��
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  InitRemoteDecoderMgr
    (   IN PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack,
        IN void*                   pCallBackUserData);

    /**************************************************************************
    * name       : StartRemoteDecoderMgr
    * description: ����RemoteDecoderMgr
    * input      : NA
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  StartRemoteDecoderMgr();

    /**************************************************************************
    * name       : StopRemoteDecoderMgr
    * description: ֹͣRemoteDecoderMgr
    * input      : NA
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  StopRemoteDecoderMgr();

    /**************************************************************************
    * name       : InitBusiness
    * description: ��ʼ��ҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  InitBusiness
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : StartBusiness
    * description: ����ҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  StartBusiness
    (   IN const CBusiness& objBusiness);

    /**************************************************************************
    * name       : StopBusiness
    * description: ֹͣҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  StopBusiness
    (   IN const CBusiness& objBusiness);

    /**************************************************************************
    * name       : ControlBusiness
    * description: ����ҵ��
    * input      : objBusiness ҵ���������
                   stCtrlPara  ���Ʋ���
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  ControlBusiness
    (   IN const CBusiness&            objBusiness,
        IN CUMW::BUSINESS_CTRL_PARA&   stCtrlPara);

    /**************************************************************************
    * name       : PlayAudio
    * description: ��������
    * input      : objBusiness ҵ���������
                   bStart          ��ʼ����ֹͣ
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  PlayAudio
    (   IN const CBusiness&    objBusiness,
        IN BOOL                bStart);

    /**************************************************************************
    * name       : SetVolume
    * description: ��������
    * input      : objBusiness ҵ���������
                   ulVolume    ����
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  SetVolume
    (   IN const CBusiness&    objBusiness,
        IN unsigned long       ulVolume);

    //BEGIN R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

    /**************************************************************************
    * name       : GetLocalIp
    * description: ���ݻ�ȡ����IP
    * input      : pszDecoderId    ������ID
    * output     : strLocalIp      ����IP
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void                  GetLocalIp
    (   IN const char*  pszDecoderId,
        OUT string&     strLocalIp);
    //END   R002C01SPC001 ADD 2011-08-16 ligengqiang 00124479 for ������֧�ֱ���˫����

public:

    //�߳���ں���
    static void*          invoke(void* argc);

    //ѭ��������Ϣ�߳�
    void                  ProcEventThread();

protected:

    //����Ƿ��Ѿ���ʼ��
    BOOL                  CheckIsInited();

private:

    /**************************************************************************
    * name       : HandleNotifyCallBack
    * description: ����֪ͨ�ص�
    * input      : pstNotifyInfo   ֪ͨ��Ϣָ��
                   pUserData       �û�����ָ��
    * output     : strLocalIp      ����IP
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    static IVS_INT32 __stdcall HandleNotifyCallBack
    (   IN CUMW_NOTIFY_INFO* pstNotifyInfo,
        IN void*             pUserData);

    //����ֻ�Ƿ�����������У����¼������̴߳���������
    LONG                  SendRDAPacket( CRDAPacket*                  pRDAPacket,
                                         const char*                  pszDecoderID);

    //����ͬ����Ϣ��������
    LONG                  ProcSendMsg( CDecoderMgrEvent* pEventRecv );

    /**************************************************************************
    * name       : SendHeartBeatMsg
    * description: ����������Ϣ
    * input      : pDecoderConnHandle  ���������Ӵ��������
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  SendHeartBeatMsg
    (   IN CDecoderConnHandle* pDecoderConnHandle);

    //�������ӳ�ʱ����
    LONG                  ProcConnTimeOut(const CDecoderMgrEvent* pEventRecv);

    //��ʼ�������������¼�����
    LONG                  InitDecoderMgrEventQ();

    //��ʼ���������
    LONG                  InitDecoderMgrServerHandle();

    //��ʼ�����������ӹ���
    LONG                  InitDecoderConnHandleMgr();

    //��ʼ��Զ�̽��������͹�������Ϣ
    LONG                  InitRemoteDecoderReqProc();

    //��ʼ��RDAЭ�����Դ��
    LONG                  InitRDAPacketMgr();

    //��ʼ����������Դ����
    LONG                  InitRemoteDecoderArry();

    //��ʼ��������ʱ����
    LONG                  InitCheckHeartBeatTrigger();

    //��ʼ��RDAЭ��ջ
    LONG                  InitRDAProtStack();

    /**************************************************************************
    * name       : SendMsg
    * description: ����������Ϣ��ͬ���ȴ���Ӧ��Ϣ������
    * input      : objRDAMsgReq    ������Ϣ����
                   pszDecoderID     ������ID
                   objRDAMsgResp   ��Ӧ��Ϣ����
    * output     : NA
    * return     : long       ������
    * remark     : NA
    **************************************************************************/
    long                  SendReqMsg
    (   IN CRDAMsg&    objRDAMsgReq,
        IN const char* pszDecoderID,
        IN CRDAMsg&    objRDAMsgResp);

    /**************************************************************************
    * name       : GetTransNo
    * description: ��ȡ������
    * input      : NA
    * output     : NA
    * return     : unsigned short.     ������
    * remark     : NA
    **************************************************************************/
    unsigned short        GetTransNo();

    /**************************************************************************
    * name       : ControlAudio
    * description: ������Ƶ
    * input      : objBusiness     ҵ�����
                   stCtrlPara      ���Ʋ���
    * output     : NA
    * return     : long            ������
    * remark     : NA
    **************************************************************************/
    long                  ControlAudio
    (   IN const CBusiness&            objBusiness,
        IN CUMW::BUSINESS_CTRL_PARA&   stCtrlPara);

private:

    //������ҵ��ṹ��
    struct DECODER_BUSINESS
    {
        char          m_szDecoderId[CUMW_DECODER_ID_LEN + 1];   // Զ�̽�����ID
        unsigned long m_ulChannelId;  //Զ�̽�������ͨ�����
        unsigned long m_ulBusinessId; //��һ��ҵ��ID
        long          m_lReferNum;      //���ü���

        DECODER_BUSINESS()
        {
            memset(m_szDecoderId, 0, sizeof(m_szDecoderId));

            m_ulChannelId  = 0;
            m_ulBusinessId = 0;
            m_lReferNum = 1;    //Ĭ�ϼ���Ϊ1
        }

        DECODER_BUSINESS(const char* szDecoderId, unsigned long ulChannelId)
        {
            memset(m_szDecoderId, 0, sizeof(m_szDecoderId));
            if(!CToolsHelp::Strncpy(m_szDecoderId, sizeof(m_szDecoderId), szDecoderId, sizeof(m_szDecoderId) - 1))
			{
				IVS_RUN_LOG_ERR("DECODER_BUSINESS Copy DecoderId failed");
			}

            m_ulChannelId  = ulChannelId;
            m_ulBusinessId = 0;
            m_lReferNum = 1;    //Ĭ�ϼ���Ϊ1
        }
    };

    //������ҵ���б���
    typedef list <DECODER_BUSINESS> LIST_DECODER_BUSINESS;

    //������ҵ����ұȽϷ�����
    class CFindDecoderBusiness
    {
public:
        CFindDecoderBusiness (const DECODER_BUSINESS& stDecoderBusiness) : m_stDecoderBusiness(stDecoderBusiness)
        {}

        bool operator()    (const DECODER_BUSINESS& stDecoderBusiness)
        {
            if (0
                == strncmp(m_stDecoderBusiness.m_szDecoderId, stDecoderBusiness.m_szDecoderId,
                           sizeof(m_stDecoderBusiness.m_szDecoderId))
                && (m_stDecoderBusiness.m_ulChannelId == stDecoderBusiness.m_ulChannelId))
            {
                return true;
            }

            return false;
        }//lint !e1762

private:
		CFindDecoderBusiness(){}//����pc-lint
		const DECODER_BUSINESS m_stDecoderBusiness;
    };

    //���ÿ�����
    enum REFER_CTRL_CODE
    {
        REFER_CTRL_CODE_NONE = 0,    //���仯
        REFER_CTRL_CODE_INC,         //����
        REFER_CTRL_CODE_DEC          //�ݼ�
    };

private:

    /**************************************************************************
    * name       : HasDecoderBusiness
    * description: ����Ƿ����н�����ҵ��
    * input      : stDecoderBusiness   ������ҵ��
                   eReferCtrlCode      ���ÿ�����
    * output     : NA
    * return     : BOOL  �Ƿ����н�����ҵ��
    * remark     : NA
    **************************************************************************/
    BOOL HasDecoderBusiness
    (   DECODER_BUSINESS&   stDecoderBusiness,
        REFER_CTRL_CODE     eReferCtrlCode = REFER_CTRL_CODE_NONE);

private:

    VOS_Thread              * m_pVosThread;
    volatile VOS_BOOLEAN m_bExit;    //�߳�����״̬��־

    CDecoderMgrEventQueue   * m_pDecoderMgrEventQ;   //�¼�����
    CDecoderMgrServerHandle * m_pDecoderMgrServerHandle; //���������
    CDecoderConnHandleMgr   * m_pDecoderConnHandleMgr; //������Դ����
    CRemoteDecoderReqProc   * m_pRemoteDecoderReqProc; //����Զ�̽��������͹�������Ϣ
    CRDAPacketMgr           * m_pRDAPacketMgr; //RDA����Դ����
    CRemoteDecoderArry      * m_pRemoteDecoderArry; //��������������Դ����
    CCheckHeartBeatTrigger  * m_pCheckHeartBeatTrigger; //������ʱ����

    //������
    volatile unsigned short m_usTransNo;

    //RDAЭ��ջ
    CRDAProtStack     * m_pRDAProtStack;

    char              * m_pSendBuffer;  //�������ݻ�����
    ULONG m_ulSendBuffLen; //�������ݻ���������

    //֪ͨ�ص�����״̬��
    CStatusNotifyCallBack m_objStatusNotifyCallBack;

    //������ҵ���б�
    LIST_DECODER_BUSINESS m_listDecoderBusiness;
};

#endif  //REMOTE_DECODER_MGR_H
