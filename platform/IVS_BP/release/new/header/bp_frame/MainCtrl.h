/********************************************************************
filename    :    MainCtrl.h
author      :    s00191067
created     :    2012/03/27
description :    ���߳̿���ģ��
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef MAIN_CTRL_H
#define MAIN_CTRL_H

#include "IService.h"
#include "bp_namespace.h"
#include "bp_mem.h"
#include "ThreadMgr.h"
#include "DumpProcess.h"
#include "HandleSignal.h"
#include "bp_memoutput.h"
#include "ILink.h"
#include "bp_config_alarm.h"
#include "Onlyone_Process.h"

#define WRITE_STATFILE_INTERVAL 5
#define PRINT_STAT_INFO_INTERVAL 60

NAMESPACE_BP_BEGIN // namespace begin


const unsigned int WARN_PATH_NAME_LENGTH = 256;
const unsigned int WARN_FILE_NAME_LENGTH = 96; /* Moudle_Type(32) + Node_Code(32) + Other(32) */

typedef struct LogAlarmInfo
{
    uint32_t      uiErrorCode;                       // ������
    char          szPathname[WARN_PATH_NAME_LENGTH]; // ����ʱ���ڲ������ļ�·����
    char          szFilename[WARN_FILE_NAME_LENGTH]; // ����ʱ���ڲ������ļ���
} LOG_ALARM_INFO;

class EXPORT_DLL CMainCtrl : public IService
{
public:
    CMainCtrl(void);
    virtual ~CMainCtrl(void);

    static void SetMainCtrl(CMainCtrl* pMainCtrl);
    static CMainCtrl* GetMainCtrl();

	void   SetIsMonitor(bool bIsMonitor){m_bIsMonitor = bIsMonitor;};

    virtual void SetParam(int argc, char* argv[]);

    // �����������ʵ������Ҫ��������
    virtual int HandleCmd(CCmd* pCmd);

    /**************************************************************************
    * name       : Routing
    * description: ���ڼ�飬��������ʵ���ദ���ڼ������
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
    virtual uint32_t Routing(void);

    /**************************************************************************
    * name       : Init
    * description: ��ʼ�����̹߳����߼�
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ���IVS_FAIL - ʧ��
    * remark     : NA    
    **************************************************************************/
    virtual int Init();

    /**************************************************************************
    * name       : UnInit
    * description: ���ٲ���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual void UnInit();

    /**************************************************************************
    * name       : UnInit
    * description: ��ʼ����־
    * input      : pszLogPath - ��־�ļ���Ŀ¼, NULLʱȡ�������е��趨Ŀ¼
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int InitLog(const char * pszLogPath = NULL);

    /**************************************************************************
    * name       : UnInit
    * description: ��ʼ���̼߳��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int InitMon(void);


    /**************************************************************************
    * name       : LoadNetService
    * description: ������Ԫ����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadNetService(void);

    /**************************************************************************
    * name       : UnInit
    * description: ����ҵ�����
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadBusinessService(bool bConfig = true);

    /**************************************************************************
    * name       : LoadLogService
    * description: ������־������
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    virtual int LoadLogDbService(bool bEnableOperationLog, bool bEnableSecurityLog);

protected:
    /**************************************************************************
    * name       : ProcessStat
    * description: ���̶�ʱд�ļ�������Daemon���̼���ӽ����Ƿ���
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void ProcessStat(void); 

    /**************************************************************************
    * name       : PrintStatInfo
    * description: ��ӡͳ����Ϣ����TPS�����г��ȵ�;
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void PrintStatInfo(void); 

	/**************************************************************************
    * name       : LinkStatusReport
    * description: ����LINK״̬�ϱ��¼�
    * input      : pCmd  --- LINK״̬�¼�����
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void LinkStatusReport(CCmd* pCmd);

    virtual void OnLinkOpened(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

    virtual void OnLinkClosed(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

	virtual void OnLinkTimeout(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

	virtual void OnLinkOpened(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkClosed(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkTimeout(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	virtual void OnLinkStatusWarn(int iNetElementType, LINK_STATUS_REPORT_INFO *pInfo){};

	void FluxControlWarn(CCmd* pCmd);

	virtual void OnFluxControlWarn(FLUX_CONTROL_WARN_INFO *pInfo){};

	virtual void OnFluxControlResume(FLUX_CONTROL_WARN_INFO *pInfo){};

	void LogWarn(CCmd *pCmd);

	void DBWarn(CCmd *pCmd);

	void ConfigWarn(CCmd *pCmd);

	virtual void OnLogWarn(LOG_ALARM_INFO *pLogAlarmInfo){};

	virtual void OnDBWarn(DB_ALARM_INFO *pDBAlarmInfo){};

	virtual void OnConfigWarn(CONFIG_ALARM_INFO *pConfigAlarmInfo){};

private:
    /**************************************************************************
    * name       : ProcessLinkStatusAlarm
    * description: ����LINK״̬�澯�¼�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
	void ProcessLinkStatusAlarm();

    /**************************************************************************
    * name       : CleanAlarmMsgStorage
    * description: �������ڸ澯��Ϣ�ֿ��еĸ澯
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA    
    **************************************************************************/
    void CleanAlarmMsgStorage();

protected:
    int           m_iArgc;

    char **       m_pszArgv;

private:
    time_t         m_iLastWriteStatTime;        // �ϴν����ļ�д��ʱ��

    CMemOutput     m_MemOutput;                 // ������־�Ͱ�ȫ��־������

    std::string    m_ModuleType;                // ģ�� ����

    time_t         m_tLastCheckTransactionTime; // �ϴμ��Transaction��ʱ��

	bool           m_bIsMonitor;                // �Ƿ����ػ�����
};


#define BP_MAIN(classname)                                                         \
do                                                                                 \
{                                                                                  \
	printf("Service Start to Run....\n");                                          \
	if(1 < argc && (0 == strcmp("-v", argv[1]) || 0 == strcmp("-V", argv[1])))     \
    {                                                                              \
	    printf("Compile time: %s %s.\n"                                            \
                  , __DATE__, __TIME__);                                           \
	    break;                                                                     \
    }                                                                              \
    if(!Onlyone_Process::onlyone(argv[0]))                                         \
    {                                                                              \
        break;                                                                     \
    }                                                                              \
    ACE::init();                                                                   \
    InitExceptionFilter();                                                         \
    classname *pMainService = NULL;                                                \
    HW_NEW(pMainService, classname);                                               \
    if (NULL == pMainService)                                                      \
    {                                                                              \
        return -1;                                                                 \
    }                                                                              \
    pMainService->SetParam(argc, argv);                                            \
    int iRet = pMainService->InitLog();                                            \
    if (IVS_SUCCEED != iRet)                                                       \
    {                                                                              \
        return iRet;                                                               \
    }                                                                              \
    CHandleSignal::Init();                                                         \
    CThread* pMainThread = NULL;                                                   \
    HW_NEW(pMainThread, CThread);                                                  \
    if (NULL == pMainThread)                                                       \
    {                                                                              \
        HW_DELETE(pMainService);                                                   \
        return -1;                                                                 \
    }                                                                              \
    CThread::SetMainThread(pMainThread);                                           \
    pMainThread->open("main", false);                                              \
    CThreadMgr::instance().AddThread(pMainThread);                                 \
    if (BP_OK != pMainService->Init())                                             \
    {                                                                              \
        return -1;                                                                 \
    }                                                                              \
    CMainCtrl::SetMainCtrl(pMainService);                                          \
    pMainThread->AddService(pMainService);                                         \
	printf("Service Runing....\n");                                                \
    pMainThread->svc();                                                            \
    CMainCtrl* pMainCtrl = CMainCtrl::GetMainCtrl();                               \
    if (NULL != pMainCtrl)                                                         \
    {                                                                              \
        pMainCtrl->UnInit();                                                       \
    }                                                                              \
    ACE::fini();                                                                   \
	printf("Service Close....\n");                                                 \
} while(false)

NAMESPACE_BP_END   // namespace end

#endif //MAIN_CTRL_H
