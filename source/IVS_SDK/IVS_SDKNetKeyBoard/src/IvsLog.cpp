
/******************************************************************************
   ��Ȩ���� (C), 2008-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �ļ���          : IvsLog.cpp
  �汾��          : 1.0
  ����            : xuxin 00114494
  ��������        : 2008-08-17
  ����޸�        : 
  ��������        : IVS��־ģ��
  �����б�        : 
  �޸���ʷ        : 
  1 ����          : 
    ����          : 
    �޸�����      : 
*******************************************************************************/

//BEGIN R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
//ȥ��map�澯
#pragma warning(disable : 4786 4503)

#include <list>
#include <string>
using namespace std;
//END   R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־

#include <stdio.h>
#include "RingCache.h"
#include "IvsLog.h"

#include <tchar.h>

/************************** Begin ��־����ʵ�� ********************************/

#define IVSLOG_ERROR_OK             0       //�ɹ�
#define IVSLOG_ERROR_INIT_CACHE     (-1)    //��ʼ������������
#define IVSLOG_ERROR_FILE_NAME      (-2)    //�Զ�������־�ļ�������
#define IVSLOG_ERROR_OPEN_FILE      (-3)    //���ļ�����
#define IVSLOG_ERROR_CREATE_EVENT   (-4)    //�����¼�����
#define IVSLOG_ERROR_CREATE_THREAD  (-5)    //�����̳߳���

//Ĭ�ϻ�������С1M
#define DEFAULT_CACHE_SIZE          (1024*1024)
//Ĭ���ļ��л����ȣ���λByte
#define DEFAULT_CHANGE_FILE_LEN     (10*1024*1024)
//Ĭ������ļ��л����ȣ���λByte
#define MAX_CHANGE_FILE_LEN         (100*1024*1024)
//Ĭ����С�ļ��л����ȣ���λByte
#define MIN_CHANGE_FILE_LEN         (100*1024)
//��־�ļ�·��������
#define MAX_LOG_FILE_PATH_NAME_LEN  1024
//������־��󳤶�
#define MAX_LEN_OF_SINGLE_LOG       2048


//�ȴ��˳��¼�����ʱ��
#define LOG_WAIT_FOR_EXIT_EVENT     5000
//�ȴ��������
#define LOG_WAIT_FOR_EXIT_EVENT_INTERVAL 50
//�ȴ�д���
#define LOG_WAIT_FOR_WRITE_OVER_INTERVAL 10


class CIvsLog
{
    private:    //��ʵ��
        CIvsLog();
    public:
        virtual ~CIvsLog();

    public:
        static CIvsLog* GetIvsLogInstance();    //��ȡ��־����
        static void DeleteIvsLogInstance();        //ɾ����־����

    public:
        //������־
        long Start();
        //������־���� 
        void SetLevel(long lLevel);
        //���õ�ǰд����־�ļ�·����
        bool SetLogFilePathName(const char* szPathName);
        //������־�ļ��������ƣ������˳���ʱ�������ļ���������λKB
        void SetFileLengthLimit(unsigned long ulLimitLengthKB);
        //д��־
        long Write(const char* szFile, long lLine,
            long lLevel, const char* format, va_list argp);
        //ֹͣ��־
        long Stop();

    private:
        //������д�ļ��߳�
        static DWORD CALLBACK ThreadEntry(LPVOID lpvoid);
        void WriteLogFileThread();
        bool LogFileLenDeal();
        IvsLogThreadErr WriteFile(char* pLogInfo, unsigned long ulLogDataLen);
        
        const char* GetLevelString(long lLevel) const;
        
        //BEGIN R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
        void DeleteBackLogFiles() const;
        //END   R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־

    private:
        //��־ģ���Ƿ�����
        bool    m_bRun;        
        
        //�Ƿ�����д��־
        bool    m_bAllowWrite;  

        //�Ƿ������������־ֹͣ
        bool    m_bDiskFull;
        //����⵽���̿ռ����1Mʱ���ָ���־��ӡ
        #define MIN_DISK_SPACE_FOR_LOG      (1024*1024) //���̿ռ����1Mʱ�ָ���־
        #define DISK_SPACE_CHECK_INTERVAL   10000       //���̿ռ�����10����
        //�ϴμ���������ʱ��(���ڴ�����������־ֹͣʱ������)
        DWORD   m_dwLastCheckDiskSpaceTime;
        
        //��־����Ĭ��ΪINFO����
        long    m_lLevel;        
        
        //д�ļ��̵߳ľ��
        HANDLE    m_hWriteThread;    
        
        //д��־�¼��ľ��
        HANDLE  m_hWriteEvent;  
        
        //д�߳��˳��¼��ľ��
        HANDLE    m_hThreadExitEvent;    

        //��־����
        CRingCache    m_Cache;    

        //��־�ļ�
        FILE*    m_pLogFile;        
        
        //��־�ļ��������ƣ���λByte
        unsigned long    m_ulFileLenLimit;

        //��־�ļ�·��������
        char    m_szLogFilePathName[MAX_LOG_FILE_PATH_NAME_LEN];

    private:
        //��ʵ����־����
        static CIvsLog*    g_pIvsLog;    
};

CIvsLog* CIvsLog::g_pIvsLog = NULL;

CIvsLog::CIvsLog()
{
    m_bRun = false;
    m_bAllowWrite = false;
    m_bDiskFull = false;
    m_dwLastCheckDiskSpaceTime = 0;
    m_lLevel = LOG_INFO;
    m_hWriteThread = NULL;
    m_hWriteEvent = NULL;
    m_hThreadExitEvent = NULL;

    m_pLogFile = NULL;
    m_ulFileLenLimit = DEFAULT_CHANGE_FILE_LEN;
    ::memset(m_szLogFilePathName,0,MAX_LOG_FILE_PATH_NAME_LEN);
}

CIvsLog::~CIvsLog()
{
    try
    {
        //ֹͣ��־ģ��
        (void)Stop();

        //������Ϊ��PC-LINT
        if(NULL != m_hWriteThread)
        {
            (void)::CloseHandle(m_hWriteThread);
            m_hWriteThread = NULL;
        }

        if(NULL != m_hThreadExitEvent)
        {
            (void)::CloseHandle(m_hThreadExitEvent);
            m_hThreadExitEvent = NULL;
        }
		if(NULL != m_hWriteEvent)
		{
			(void)::CloseHandle(m_hWriteEvent);
			m_hWriteEvent = NULL;
		}
        if(NULL != m_pLogFile)
        {
            (void)::fclose(m_pLogFile);
            m_pLogFile = NULL;
        }

    }
    catch(...)
    {
    }
}

//��ȡ��־ģ���Ψһʵ��
CIvsLog* CIvsLog::GetIvsLogInstance()
{
    //����־ʵ��δ�������������
    if(NULL == g_pIvsLog)
    {
        g_pIvsLog = new CIvsLog;
    }

    return g_pIvsLog;
}

//ɾ����־ģ���ʵ��
void CIvsLog::DeleteIvsLogInstance()
{
    //BEGIN R001C05LYYS01 MOD 2010-03-19 ligengqiang 00124479 for ��������ֵ��߼�����
    //���Ѿ�����������ͷ�
    if(NULL != g_pIvsLog)
    //END   R001C05LYYS01 MOD 2010-03-19 ligengqiang 00124479 for ��������ֵ��߼�����
    {
        //����Ҫ��ֹͣ����
        (void)(g_pIvsLog->Stop());

        //ɾ������
        delete g_pIvsLog;
        g_pIvsLog = NULL;
    }
}

//������־ģ��
long CIvsLog::Start()
{
    //����Ѿ�������ֱ�ӷ���
    if(m_bRun)
    {
        m_bAllowWrite = true;
        return IVSLOG_ERROR_OK;
    }

    //��ʼ��������Ϊ2M
    unsigned long ulCacheSize = m_Cache.SetCacheSize(DEFAULT_CACHE_SIZE);
    if(DEFAULT_CACHE_SIZE != ulCacheSize)
    {
        //����������ʧ��
        return IVSLOG_ERROR_INIT_CACHE;
    }

    //����ļ���
    if(::strlen(m_szLogFilePathName) == 0)
    {
        //�����м��ģ�����õ�·��
        (void)::GetModuleFileName(NULL, m_szLogFilePathName, MAX_LOG_FILE_PATH_NAME_LEN-1);
        char* pszFind = ::strrchr(m_szLogFilePathName, '.');
        if(NULL == pszFind)
        {
            //�쳣
            return IVSLOG_ERROR_FILE_NAME;
        }
        //��Ӻ�׺
        (void)::sprintf(pszFind, ".log");
    }
    //���ļ�
    m_pLogFile = ::fopen(m_szLogFilePathName, "a+");
    if(NULL == m_pLogFile)
    {
        //�ļ��޷���
        return IVSLOG_ERROR_OPEN_FILE;
    }

    //��ʼ��д�¼�
    m_hWriteEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(NULL == m_hWriteEvent)
    {
        //д�¼�����ʧ��
        
        //�ر��ļ�
        (void)::fclose(m_pLogFile);
        m_pLogFile = NULL;
        return IVSLOG_ERROR_CREATE_EVENT;
    }

    //��ʼ��д�߳��˳��¼�
    m_hThreadExitEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
    if(NULL == m_hThreadExitEvent)
    {
        //д�߳��˳��¼�����ʧ��
        
        //�ر��ļ�
        (void)::fclose(m_pLogFile);
        m_pLogFile = NULL;

        //�ر�д�¼����
        (void)::CloseHandle(m_hWriteEvent);
        m_hWriteEvent = NULL;
        return IVSLOG_ERROR_CREATE_EVENT;
    }

    //������־������־
    m_bRun = true;

    //����������д�ļ��߳�
    unsigned long ulThreadID = 0;
    m_hWriteThread = ::CreateThread(NULL, NULL, ThreadEntry, this, 0, &ulThreadID);
    if(NULL == m_hWriteThread)
    {
        //д�̴߳���ʧ�ܣ�������Դ
        m_bRun = false;

        //�ر��ļ�
        (void)::fclose(m_pLogFile);
        m_pLogFile = NULL;

        //�ر�д�¼����
        (void)::CloseHandle(m_hWriteEvent);
        m_hWriteEvent = NULL;

        //�ر�д�߳��˳��¼����
        (void)::CloseHandle(m_hThreadExitEvent);
        m_hThreadExitEvent = NULL;
        return IVSLOG_ERROR_CREATE_THREAD;
    }

    //��ʼ������־
    m_bAllowWrite = true;

    return IVSLOG_ERROR_OK;
}
//������־����
void CIvsLog::SetLevel(long lLevel)
{
    switch(lLevel)
    {
        case LOG_EMERGENCY:
        case LOG_ALERT:
        case LOG_CRITICAL:
        case LOG_ERROR:
        case LOG_WARNING:
        case LOG_NOTICE:
        case LOG_INFO:
        case LOG_DEBUG:
            m_lLevel = lLevel;
            break;
        default:
            break;
    }
}

//���õ�ǰд����־�ļ�·����
bool CIvsLog::SetLogFilePathName(const char* szPathName)
{
    bool bSetOk = false;
    //�������
    if((NULL!=szPathName) && (::strlen(szPathName)<MAX_LOG_FILE_PATH_NAME_LEN))
    {
        //���Ʊ����ļ���������һ���ַ��Ƿ�Ϊб��
        if('\\' == szPathName[0])
        {
            //ȥ��б��
            (void)::sprintf(m_szLogFilePathName, "%s", szPathName+1);
        }
        else
        {
            (void)::sprintf(m_szLogFilePathName, "%s", szPathName);
        }

        //ѭ�����������ļ�Ŀ¼
        char* pszFind = ::strchr(m_szLogFilePathName, '\\');
        while(NULL != pszFind)
        {
            pszFind[0] = '\0';
            (void)::CreateDirectory(m_szLogFilePathName, NULL);
            pszFind[0] = '\\';
            pszFind = ::strchr(pszFind+1, '\\');
        }

        //�ļ�Ŀ¼�������
        bSetOk = true;
    }

    return bSetOk;
}

//������־�ļ��������ƣ������˳���ʱ�������ļ���������λKB
void CIvsLog::SetFileLengthLimit(unsigned long ulLimitLengthKB)
{
    //KBתΪByte
    unsigned long ulNewLimitLength = ulLimitLengthKB * 1024;

    //��Χȷ��
    if(ulNewLimitLength < MIN_CHANGE_FILE_LEN)
    {
        //С����Сֵʹ����Сֵ
        m_ulFileLenLimit = MIN_CHANGE_FILE_LEN;
    }
    else if(ulNewLimitLength < MAX_CHANGE_FILE_LEN)
    {
        //������Χ
        m_ulFileLenLimit = ulNewLimitLength;
    }
    else
    {
        //�������ֵʹ�����ֵ
        m_ulFileLenLimit = MAX_CHANGE_FILE_LEN;
    }
}

//дһ����־��������������:
//2008-08-17 10:45:45.939[DEBUG|Log.cpp:152|PID:772|TID:2342|Err:0]��������...
long CIvsLog::Write(const char* szFile, long lLine,
                long lLevel, const char* format, va_list argp)
{
    if(!m_bRun)
    {   
        //δ����
        return 0;
    }

    //δ����д
    if(!m_bAllowWrite)
    {
        //�������ڴ���������ֹͣ
        if(!m_bDiskFull)
        {
            return 0;
        }

        //����������ֹͣ
        DWORD dwTime = GetTickCount();
        if(dwTime-m_dwLastCheckDiskSpaceTime < DISK_SPACE_CHECK_INTERVAL)
        {
            //���ʱ��δ��
            return 0;
        }

        //�������Ƿ�ﵽ�ָ���־��ӡ��Ҫ��
        char szDirectory[MAX_LOG_FILE_PATH_NAME_LEN] = {0};
        ::strncpy(szDirectory, m_szLogFilePathName, MAX_LOG_FILE_PATH_NAME_LEN-1);
        char* pFind = ::strchr(szDirectory, '\\');
        if(NULL == pFind)
        {
            //�쳣�����ټ��
            m_bDiskFull = false;
            return 0;
        }
        pFind[1] = '\0';
        ULARGE_INTEGER uliFreeBytesAvailableToCaller;
        ULARGE_INTEGER uliTotalNumberOfBytes;
        ULARGE_INTEGER uliTotalNumberOfFreeBytes;
        uliFreeBytesAvailableToCaller.QuadPart = 0;
        uliTotalNumberOfBytes.QuadPart = 0;
        uliTotalNumberOfFreeBytes.QuadPart = 0;
        BOOL bRes = GetDiskFreeSpaceEx(szDirectory, &uliFreeBytesAvailableToCaller,
            &uliTotalNumberOfBytes, &uliTotalNumberOfFreeBytes);
        if(!bRes)
        {
            //���ʧ�ܣ����ټ��
            m_bDiskFull = false;
            return 0;
        }

        if(uliFreeBytesAvailableToCaller.QuadPart < MIN_DISK_SPACE_FOR_LOG)
        {
            //���̿ռ仹δ�ﵽ�ָ���׼����¼���μ��ʱ�䣬�ȴ��ٴμ��
            m_dwLastCheckDiskSpaceTime = dwTime;
            return 0;
        }

        //���̿ռ�ﵽ�ָ���׼���ָ���־���룬�����������־
        m_dwLastCheckDiskSpaceTime = 0;
        m_bDiskFull = false;
        m_bAllowWrite = true;
        
        //����ļ�δ�򿪣���Ҫ���´���־�ļ���д������־��Ϣ
        if(NULL == m_pLogFile)
        {
            m_pLogFile = ::fopen(m_szLogFilePathName, "a+");
            if(NULL == m_pLogFile)
            {
                //��Ȼ�����⣬�ָ������ǣ��´��ٳ��Դ�
                m_bDiskFull = true;
                m_bAllowWrite = false;
                return 0;
            }
            //֪ͨд�߳�д��־
            (void)::SetEvent(m_hWriteEvent);
        }
    }

    //��־��������
    if(lLevel > m_lLevel)
    {
        return 0;
    }

//������Ϣ׼��
    //��־ʱ��
    SYSTEMTIME st;
    ::GetLocalTime(&st);        
    //��־����
    const char* pszLevel = GetLevelString(lLevel);
    //�ļ���
    const char* pszFileName = ::strrchr(szFile, '\\');
    if(NULL != pszFileName)
    {
        //Խ��б��
        pszFileName += 1;
    }
    else
    {
        pszFileName = szFile;
    }

    //�к� ��lLine
    //����ID
    unsigned long ulProcessID = ::GetCurrentProcessId();
    //�߳�ID
    unsigned long ulThreadID = ::GetCurrentThreadId();
    //��ǰ������
    unsigned long ulErr = GetLastError();
//������Ϣ׼�����

    //����ʹ�ó�Ա���������̻߳��������
    char szLogTmp [MAX_LEN_OF_SINGLE_LOG] = {0};
    //������������־��Ϣ�м��������Ϣ
    (void)::sprintf(szLogTmp,"%04d-%02d-%02d %02d:%02d:%02d.%03d[%8s|%20s:%05d|PID:0x%04X|TID:0x%04X|Err:0x%04X]    ",
        st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        pszLevel, pszFileName, lLine, ulProcessID, ulThreadID, ulErr);//lint !e705

    //���û���Ϣ���ڶ�����Ϣ��д����־��
    unsigned long ulLen = ::strlen(szLogTmp);
    (void)::_vsnprintf(szLogTmp+ulLen, (MAX_LEN_OF_SINGLE_LOG-ulLen)-1, format, argp);
    szLogTmp[MAX_LEN_OF_SINGLE_LOG-1] = '\0';
    ulLen = ::strlen(szLogTmp);
    if((ulLen+2) < MAX_LEN_OF_SINGLE_LOG)
    {//�Զ�����һ������
        szLogTmp[ulLen] = '\n';
        szLogTmp[ulLen+1] = '\0';
    }

    //����־д��������
    unsigned long ulWriteLen = m_Cache.Write(szLogTmp, ::strlen(szLogTmp));
    while(0 == ulWriteLen)
    {
        //֪ͨд�߳�д��־
        (void)::SetEvent(m_hWriteEvent);
        //�ȴ�����д�뻺��
        Sleep(LOG_WAIT_FOR_WRITE_OVER_INTERVAL);
        ulWriteLen = m_Cache.Write(szLogTmp, ::strlen(szLogTmp));
    }

    //֪ͨд�߳�д��־
    (void)::SetEvent(m_hWriteEvent);
    
    return 0;
}

//ֹͣ��־����ֹд�߳�
long CIvsLog::Stop()
{
    //������д��־
    m_bAllowWrite = false;
    m_bDiskFull = false;
    m_dwLastCheckDiskSpaceTime = 0;

    //���Ѿ�ֹͣ����δ������ֱ���˳�
    if(!m_bRun)
    {
        return IVSLOG_ERROR_OK;
    }

    //�Ȼ��������ݶ�д���ļ���(�ȴ�5��)
    long lWaitTime = LOG_WAIT_FOR_EXIT_EVENT;
    while(lWaitTime >= 0)
    {
        if(0 == m_Cache.GetDataSize())
        {
            //��������־�Ѿ���д���ļ�����
            break;
        }
        
        //����д�¼�
        (void)::SetEvent(m_hWriteEvent);

        Sleep(LOG_WAIT_FOR_WRITE_OVER_INTERVAL);
        lWaitTime -= LOG_WAIT_FOR_WRITE_OVER_INTERVAL;
    }
    
    //������־��־Ϊֹͣ������д�¼������߳��Լ��˳�
    //�ȴ�5�룬����δ�˳�����ǿ����ֹ
    m_bRun = false;    
    lWaitTime = LOG_WAIT_FOR_EXIT_EVENT;
    while(lWaitTime >= 0)
    {
        //����д�¼������߳��Լ��˳�
        (void)::SetEvent(m_hWriteEvent);

        if(WAIT_TIMEOUT != ::WaitForSingleObject(m_hThreadExitEvent, LOG_WAIT_FOR_EXIT_EVENT_INTERVAL))
        {
            //�߳̽���
            //begin delete for AQ1D01618 by xuxin
            //m_hThreadExitEvent = NULL; //������CloseHandle�������ﲻ���ÿ�
            //end delete for AQ1D01618 by xuxin
            break;
        }
        
        lWaitTime -= LOG_WAIT_FOR_EXIT_EVENT_INTERVAL;
    }

    if(NULL != m_hWriteThread)
    {
        //ǿ����ֹд�߳�
        (void)::TerminateThread(m_hWriteThread, 0);
        (void)::CloseHandle(m_hWriteThread);
        m_hWriteThread = NULL;
    }

    //����д�¼�
    if(NULL != m_hWriteEvent)
    {
        (void)::CloseHandle(m_hWriteEvent);
        m_hWriteEvent = NULL;
    }

    //����д�߳��˳��¼�
    if(NULL != m_hThreadExitEvent)
    {
        (void)::CloseHandle(m_hThreadExitEvent);
        m_hThreadExitEvent = NULL;
    }
    
    //�ر��ļ�
    if(NULL != m_pLogFile)
    {
        (void)::fclose(m_pLogFile);
        m_pLogFile = NULL;
    }

    //��ջ���
    m_Cache.Clear();

    return IVSLOG_ERROR_OK;
}

//��־д�߳����
DWORD CALLBACK CIvsLog::ThreadEntry(LPVOID lpvoid)
{
    if(NULL != lpvoid)
    {
        //����д�̺߳�����
        CIvsLog* pIvsLog = (CIvsLog *)lpvoid;
        pIvsLog->WriteLogFileThread();
    }

    return 0;
}
/*lint -e438 */
/*lint -e838*/
//д��־�߳�
void CIvsLog::WriteLogFileThread()
{
    //�߳��б���׼��
    unsigned long ulLogDataLen = 0;
    IvsLogThreadErr enRet;
    char* pLogInfo = NULL;
    __try
    {
        //��������ݿռ�
        pLogInfo = new char[m_Cache.GetCacheSize()];
    }
    __except(1)
    {
    }
    if(NULL == pLogInfo)
    {
        //�ͷŲ�����ֵ
        (void)::CloseHandle(m_hWriteThread);
        m_hWriteThread = NULL;

        //�߳��˳��¼�֪ͨ
        (void)::SetEvent(m_hThreadExitEvent);

        return ;
    }

    //����������ѭ��
    while(m_bRun)
    {
        //�ȴ�д�ļ��¼�
        (void)::WaitForSingleObject(m_hWriteEvent, INFINITE);

        //�������ļ�δ��
        if(m_bDiskFull || NULL == m_pLogFile)
        {
            //�����Ǵ��������ȴ�
            if(m_bDiskFull)//lint !e774
            {
                continue;
            }
            else
            {   //������Ϊ��������ɵģ��˳�
                break;
            }
        }

        //������
        ulLogDataLen = m_Cache.GetCacheSize();
        ulLogDataLen= m_Cache.Read(pLogInfo, ulLogDataLen);
        if(0 == ulLogDataLen)
        {
            //����Ϊ��δ��������
            continue;
        }

        enRet = WriteFile(pLogInfo, ulLogDataLen);
        if (NEGLECTABLE_ERR == enRet)
        {
            continue;
        }
        else if (CRITICAL_ERR == enRet)
        {
            break;
        }

        if (!LogFileLenDeal())
        {
            break;
        }
    }

    //�ָ�״̬
    m_bRun = false;
    m_bAllowWrite = false;
    m_bDiskFull = false;
    m_dwLastCheckDiskSpaceTime = 0;

    //�ͷŲ�����ֵ
    (void)::CloseHandle(m_hWriteThread);
    m_hWriteThread = NULL;

    //�رյ�ǰ��־�ļ�
    if(NULL != m_pLogFile)
    {
        (void)::fclose(m_pLogFile);
        m_pLogFile = NULL;
    }

    //�ͷ���ʱ�ռ�
    __try
    {
        delete[] pLogInfo;
    }
    __except(1)
    {
    }
    pLogInfo = NULL;

    //�߳��˳��¼�֪ͨ
    (void)::SetEvent(m_hThreadExitEvent);
}
/*lint +e838*/

//���ʹ�����־�ļ����ȳ�������
bool CIvsLog::LogFileLenDeal()
{
    SYSTEMTIME sysTime;
    char szNewFileName[MAX_LOG_FILE_PATH_NAME_LEN] = {0};

    if(NULL == m_pLogFile)//PCLint
    {
        return true;
    }

    //����ļ������Ƿ񳬹�����
    unsigned long ulCurFileLen = (unsigned long)::ftell(m_pLogFile);
    if(ulCurFileLen < m_ulFileLenLimit)
    {
        return true;
    }

    if(::strlen(m_szLogFilePathName) >= (MAX_LOG_FILE_PATH_NAME_LEN-20))
    {
        //�ļ���̫�����޷��޸��ļ���Ϊ�����ļ���
        return true;
    }

    //�رյ�ǰ��־�ļ�
    (void)::fclose(m_pLogFile);
    m_pLogFile = NULL;

    //���ɱ����ļ���
    ::GetLocalTime(&sysTime);

    char szTemp[MAX_LOG_FILE_PATH_NAME_LEN] = {0};
    memcpy(szTemp, m_szLogFilePathName, strlen(m_szLogFilePathName) - 4);
    (void)::sprintf(szNewFileName, "%s_%4d%02d%02d%02d%02d%02d%s",
        szTemp, sysTime.wYear, sysTime.wMonth,
        sysTime.wDay, sysTime.wHour, sysTime.wMinute,
        sysTime.wSecond, ".log");

    //�޸ĵ�ǰ��־�ļ�Ϊ�����ļ����޸��ļ���ʧ��Ҳû�й�ϵ�������ٴ������������ļ���д
    (void)::rename(m_szLogFilePathName, szNewFileName);

    //BEGIN R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
    //ɾ��������־
    DeleteBackLogFiles();
    //END   R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־

    //���´���־�ļ�
    m_pLogFile = ::fopen(m_szLogFilePathName, "a+");
    if(NULL != m_pLogFile)
    {
        //�ļ�������
        return true;
    }

    //����־�ļ���ʧ��
    if(ERROR_DISK_FULL != ::GetLastError())
    {
        //�쳣��ֹͣ��־
        return false;
    }

    //���Ǵ���������ͣ��־���ȴ��ռ�
    m_bAllowWrite = false;
    m_bDiskFull = true;

    return true;
}
/*lint -e429 */
//���ʹ�����־�ļ����ȳ�������
IvsLogThreadErr CIvsLog::WriteFile(char* pLogInfo, unsigned long ulLogDataLen)
{

    if(NULL == m_pLogFile)//PCLint
    {
        return NEGLECTABLE_ERR;
    }

    //д���ݵ��ļ�
    if(1 != fwrite(pLogInfo, ulLogDataLen, 1, m_pLogFile))
    {
        //д���ݵ��ļ��г����쳣�����Ǵ���������ͣ��־���ȴ��ռ�
        if(ERROR_DISK_FULL == ::GetLastError())
        {
            m_bAllowWrite = false;
            m_bDiskFull = true;
            return NEGLECTABLE_ERR;
        }
        else
        {//�����������˳�д��־
            return CRITICAL_ERR;
        }
    }

    //���ļ�����������д��Ӳ��
    if(fflush(m_pLogFile) != 0)
    {
        //д���ݵ�Ӳ���г����쳣�����Ǵ���������ͣ��־���ȴ��ռ�
        if(ERROR_DISK_FULL == ::GetLastError())
        {
            m_bAllowWrite = false;
            m_bDiskFull = true;
            return NEGLECTABLE_ERR;
        }
        else
        {//�����������˳�д��־
            return CRITICAL_ERR;
        }
    }

    return SUCCESS;
}//lint !e818

//��ȡ��־�����ַ���
const char* CIvsLog::GetLevelString(long lLevel) const
{
    const char* pstrLevel = NULL;
    switch(lLevel)
    {
        case LOG_EMERGENCY:
            pstrLevel = "EMERGENCY";
            break;
        case LOG_ALERT:
            pstrLevel = "ALERT";
            break;
        case LOG_CRITICAL:
            pstrLevel = "CRITICAL";
            break;
        case LOG_ERROR:
            pstrLevel = "ERROR";
            break;
        case LOG_WARNING:
            pstrLevel = "WARNING";
            break;
        case LOG_NOTICE:
            pstrLevel = "NOTICE";
            break;
        case LOG_INFO:
            pstrLevel = "INFO";
            break;
        case LOG_DEBUG:
            pstrLevel = "DEBUG";
            break;
        default:
            pstrLevel = "NOLEVEL";
            break;
    }

    return pstrLevel;
}
/*************************** End ��־����ʵ�� *********************************/


/********************** Begin ��־ģ���û��ӿ�ʵ�� ****************************/
//����NVS��־ģ��
IVSLOG_API void IvsStartLog(void)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //��ȡ��־ʵ��
    (void)(pIvsLog->Start());
    //д������Ϣ
    (void)(pIvsLog->Write(__FILE__, __LINE__,
                LOG_INFO, "IVS Log Module Start!", NULL));
}

//дһ����־
IVSLOG_API void __IvsWriteLog(const char* szFileName, long lLine,
                      long lLevel, const char* format, va_list argp)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //дһ����־
    (void)(pIvsLog->Write(szFileName, lLine, lLevel, format, argp));
}

//ֹͣIVS��־ģ��
IVSLOG_API void IvsStopLog(void)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //дֹͣ��Ϣ
    (void)(pIvsLog->Write(__FILE__, __LINE__,
                 LOG_INFO, "IVS SDK Log Module Stop!\n\n\n\n", NULL));
    //ֹͣ��־
    (void)(pIvsLog->Stop());
    //ɾ����־����
    CIvsLog::DeleteIvsLogInstance();
}

//������־����
IVSLOG_API void IvsSetLogLevel(long lLevel)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //������־����
    pIvsLog->SetLevel(lLevel);
}

//���õ�ǰд����־�ļ�·����(����·�������·��)
IVSLOG_API bool IvsSetLogFilePathName(const char* szPathName)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //�����ļ���
    bool bSetOk = pIvsLog->SetLogFilePathName(szPathName);
    return bSetOk;
}

//������־�ļ��������ƣ������˳���ʱ�������ļ�����λKB(100K��100M֮��,Ĭ����10M)
IVSLOG_API void IvsSetLogFileLengthLimit(unsigned long ulLimitLengthKB)
{
    //��ȡ��־ʵ��
    CIvsLog* pIvsLog = CIvsLog::GetIvsLogInstance();
    //�����ļ���������
    pIvsLog->SetFileLengthLimit(ulLimitLengthKB);
}
/************************ End ��־ģ���û��ӿ�ʵ�� ****************************/

//BEGIN R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
//ɾ�����ڵ���־�ļ�
void CIvsLog::DeleteBackLogFiles() const
{
    if (strlen(m_szLogFilePathName) > 256)
    {
        return;
    }

    //��ȡ��־����Ŀ¼
    IVSLOG::PATH_ARRAY szAbsolutePath = {0};

    memcpy(szAbsolutePath, m_szLogFilePathName, strlen(m_szLogFilePathName) - 4);
        
    //�滻���һ����б��
    char* pszFind = strrchr(szAbsolutePath, '\\');
    if (NULL == pszFind)
    {
        return;
    }
    //����'\\'��ȥ��'\\'������ַ���
    *(pszFind + 1) = '\0';    //�滻Ϊ������


    //��־�ļ�������
    IVSLOG::PATH_ARRAY szLogFileNameFilter = {0};
    IVSLOG::PATH_ARRAY szTemp = {0};
    memcpy(szTemp, m_szLogFilePathName, strlen(m_szLogFilePathName) - 4);
    (void)_snprintf(szLogFileNameFilter, sizeof(szLogFileNameFilter), "%s*", szTemp);

    //��־�ļ��б�
    list<string> LogFileList;
    
    WIN32_FIND_DATA stData = {0};
    BOOL bFind = TRUE;

    HANDLE hFind = FindFirstFile(szLogFileNameFilter, &stData);
    if (INVALID_HANDLE_VALUE != hFind)
    {
        bFind = TRUE;
        while (bFind)
        {
            //���浽��־�ļ��б�
            LogFileList.push_back(stData.cFileName);

            bFind = FindNextFile(hFind, &stData);
        }
    }
    (void)FindClose(hFind);

    //��־�ļ�����
    unsigned long ulFileCount = LogFileList.size();

    //û����ָ����Ŀ������ɾ��
    if (IVSLOG::LOG_FILE_COUNT_MAX > ulFileCount)
    {
        return;
    }

    //����
    LogFileList.sort();

    //ɾ��������־
    list<string>::iterator Iter = LogFileList.begin();
    for (; IVSLOG::LOG_FILE_COUNT_MAX <= ulFileCount && Iter != LogFileList.end(); ++Iter)
    {
        //��־�ļ�·��
        IVSLOG::PATH_ARRAY szDelLogFileName = {0};
        (void)_snprintf(szDelLogFileName, sizeof(szDelLogFileName), "%s\\%s", 
            szAbsolutePath, Iter->c_str());

        //ɾ���ù�����־
        (void)remove(szDelLogFileName);

        ulFileCount--;
    }
}
//END   R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
