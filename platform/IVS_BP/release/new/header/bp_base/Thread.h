/********************************************************************
filename    :    Thread.h
author      :    s00191067
created     :    2012/03/27
description :    �̷߳�װ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_THREAD_H
#define BP_THREAD_H


#include <map>
#include <queue>
#include "ace_header.h"
#include "IService.h"
#include "bp_namespace.h"
#include "DBInterface.h"
#include "FluxControl.h"

//extern ACE_TSS<IDBConnect* > g_db_conn;

NAMESPACE_BP_BEGIN // namespace begin

typedef struct fluxControlWarnInfo
{
	uint32_t      uiThreadID;               // �߳�ID
	bool          bIsFluxControlWarn;       // �Ƿ������ظ澯��true�����ز����澯��false�����ػָ��澯
	uint32_t      uiLevel;                  // �澯����
	uint32_t      uiNameLength;             // �߳����Ƴ���
	char          pThreadName[1];           // �߳�����
}FLUX_CONTROL_WARN_INFO;

class EXPORT_DLL CThread : public ACE_Task<ACE_MT_SYNCH>
{
public:
    CThread(void);
    virtual ~CThread(void);
    static void SetMainThread(CThread* pThread);
    static CThread* GetMainThread();

    /**************************************************************************
    * name       : open
    * description: �����̣߳�����ACE��activate����һ���߳�
    * input      : args ����ָ�룬�����߳����� bIsCreateThread�Ƿ񴴽��߳�,false����Ҫ������ֱ��ʹ�����߳�
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    virtual int open(const void *args, bool bIsCreateThread = true);

    /**************************************************************************
    * name       : svc
    * description: �̴߳�������ѭ��ִ������
    * input      : args ����ָ�룬�����߳�����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : �̳���ACE��Task
    **************************************************************************/
    virtual int svc();

    /**************************************************************************
    * name       : close
    * description: �����̣߳����ñ�־λΪfalse��֪ͨ�߳̽���ѭ����
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int close();

    /**************************************************************************
    * name       : kill
    * description: ɱ���߳�
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int kill(); 

    /**************************************************************************
    * name       : AddService
    * description: ���߳�����������
    * input      : pService ����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void AddService(IService* pService); 

    /**************************************************************************
    * name       : DelService
    * description: ɾ���߳��е�����
    * input      : pService ����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    int DelService(IService*& pService);

	    /**************************************************************************
    * name       : FindService
    * description: ͨ��serviceID����IService
    * input      : serviceID
    * output     : NA
    * return     : IService*
    * remark     : NA
    **************************************************************************/
    IService* FindService(uint32_t serviceID);

    /**************************************************************************
    * name       : Notify
    * description: ���߳�������������Ϣ
    * input      : pCmd ������Ϣ
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA
    **************************************************************************/
    int Notify(CCmd* pCmd);

    /**************************************************************************
    * name       : NotifyEx
    * description: ���߳�������������Ϣ
    * input      : pCmd ������Ϣ�� lVal ˽������
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : CThread ����ֱ�ӷ���ʧ�ܣ��ɼ̳���ʵ��
    **************************************************************************/
    virtual int NotifyEx(CCmd* pCmd, long lVal);

    /**************************************************************************
    * name       : GetThreadPayload
    * description: ��ȡ��ǰ�̸߳���
    * input      : NA
    * output     : NA
    * return     : ��ǰ�̸߳���Ȩֵ
    * remark     : �Ȳ����߳�������service�ĸ���Ȩֵ����ֵΪ0����ȡcmdQueue����
    **************************************************************************/
    unsigned int GetThreadPayload();

    void SetIdleMode(bool bIdleMode);

    void setTimeout(unsigned int uiTimeout);

    bool IsTimeout(time_t tCurrent);

    void SetThreadName(const std::string& strName);
    std::string GetThreadName();

    ACE_thread_t GetThreadID(void)const;
    ACE_hthread_t GetThreadHandle()const;

    time_t GetUpdateTime(void) const;

    void SetUpdateTime(time_t time);

	void SetCmdQueueLength(unsigned int unQueueMaxLength);
	unsigned int GetCmdQueueLength();

    /**************************************************************************
    * name       : setConnectDatabase
    * description: �����߳��Ƿ񴴽����ݿ�����
    * input      : bDbConnection - �Ƿ񴴽����ݿ����ӵ��߼�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void SetConnectDatabase(const bool bDbConnection);//daxie

    ///**************************************************************************
    //* name       : ConnectDatabase
    //* description: �������ݿ�����
    //* input      : NA
    //* output     : NA
    //* return     : BP_OK - ���ӳɹ� BP_FAIL - ����ʧ��
    //* remark     : NA
    //**************************************************************************/
    //int ConnectDatabase( void ) throw();

    /**************************************************************************
    * name       : ProcessSyncCmd()
    * description: ����ͬ�����Ŀǰֻ����ͬ������, ����ͬ�����Ҫ�ô˽ӿ�
    * input      : NA
    * output     : NA
    * return     : BP_OK - ���ӳɹ� BP_FAIL - ����ʧ��
    * remark     : NA
    **************************************************************************/
    int ProcessSyncCmd(CCmd &cmd);
	uint32_t GetCmdCount();
	uint32_t GetSessionCount();

	/**************************************************************************
    * name       : ProcessSyncCmd()
    * description: ����ͬ�����Ŀǰֻ����ͬ������, ����ͬ�����Ҫ�ô˽ӿ�
    * input      : NA
    * output     : NA
    * return     : BP_OK - ���ӳɹ� BP_FAIL - ����ʧ��
    * remark     : NA
    **************************************************************************/
	void SetIsCommuThread(bool bIsCommuThread){m_bIsCommuThread = bIsCommuThread;};
	bool IsCommuThread(){return m_bIsCommuThread;};

protected:
    /**************************************************************************
    * name       : NotifyUpdateTime
    * description: ˢ������ִ��ʱ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void NotifyUpdateTime(void);

    /**************************************************************************
    * name       : DestroyCmd
    * description: �����������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DestroyCmd(void);

    /**************************************************************************
    * name       : ProcessCmd
    * description: �����������
    * input      : NA
    * output     : NA
    * return     : ������������
    * remark     : NA
    **************************************************************************/
    virtual unsigned int ProcessCmd(void);

    /**************************************************************************
    * name       : GetQueueCmd
    * description: �Ӷ����л�ȡһ������
    * input      : NA
    * output     : NA
    * return     : CCmd* ����һ������
    * remark     : NA
    **************************************************************************/
    CCmd* GetQueueCmd(void);

    /**************************************************************************
    * name       : PopQueueCmd
    * description: ������г�ջ������һ������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PopQueueCmd(void);

    /**
     * �������ݿ�
     */
    void ConnectDb();

private:
	/**************************************************************************
    * name       : SendFluxControlWarn
    * description: �������ظ澯
    * input      : bIsWarn  true ��ʾ�����ظ澯��false��ʾ�����ػָ��澯
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void SendFluxControlWarn(bool bIsWarn, unsigned short usLevel);

protected:
    unsigned int                 m_uiTimeout;        // ��ʱʱ��
    time_t                       m_tLastUpdateTime;  // ���һ�λỰ����ʱ�䣬�����жϳ�ʱʹ��;
    ACE_thread_t                 m_threadID;         // �߳�ID
    ACE_hthread_t                m_hThread;          // �߳̾��,windows ʹ�þ�������߳�;
    std::string                  m_strName;          // �߳���
    bool                         m_bRunning;         // ����״̬���λ
    bool                         m_bIdleMode;        // �߳����з�ʽ��Ĭ��false����ѯ�ķ�ʽ���У�Ϊtrue������Idle��ʽ����

    CServiceSet                  m_setService;       // ����
    typedef std::queue<CCmd*>    CCmdQueue;          // ���
    CCmdQueue                    m_queueCmd;         // �������
	unsigned int				 m_uiCmdQueueLength; // ������󳤶�
    ACE_Thread_Mutex             m_mutexCmd;         // ������б���;
    ACE_Auto_Event               m_evWait;

    time_t m_nLastConnectDbTime; //�ϴ��������ݿ��ʱ��, ����0ʱ����ʾ�������ݿ�ɹ�, ��ʼֵλ-1 -- ��ʾҪ�������ݿ�
    bool                         m_bDbConnection;   //�Ƿ񴴽����ݿ����ӱ��

	bool                         m_szAlreadyWarned[LEVEL_NUMBER];  //�Ƿ����صı�־�����ڸ澯������ָ�
	bool                         m_bIsCommuThread;  //�Ƿ���ͨѶ�߳�
};

NAMESPACE_BP_END   // namespace end
#endif //BP_THREAD_H
