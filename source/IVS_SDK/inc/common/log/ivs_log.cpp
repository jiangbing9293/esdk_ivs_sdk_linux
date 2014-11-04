#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <list>
#include <fstream>

#ifdef _MSC_VER
# include <windows.h>
#else
# include <pthread.h>
# include <sys/time.h>
# include <syslog.h>   // for syslog
# include <dirent.h>
# include <string.h>
# include <limits.h>
# include <unistd.h>
#endif


#include "compat.h"
#include "ivs_io.h"
#include "ivs_log.h"

#ifdef LOG_HAS_CONFIG_FILE
#include "log_conf.h"
#endif

#ifdef  LOG_HAS_STACK_TRACE
#include "ivs_btrace.h"
#endif

#if defined _MSC_VER && (_MSC_VER >= 1500)
#pragma warning (disable : 4996)
#endif

#ifdef WIN32
	#include <Dbghelp.h>   // MakeSureDirectoryPathExists
	#pragma comment (lib, "Dbghelp.lib")
#else
	#include "unistd.h" 
	#include <stdarg.h>
	#include <sys/stat.h>  
	#define ACCESS access 
#endif

/************************************************************************/
/* OSƽ̨��غ�������                                                   */
/************************************************************************/

#ifdef _MSC_VER
#  define lock_t          CRITICAL_SECTION
#  define LOCK_INIT(p)    InitializeCriticalSection((p))
#  define LOCK_DESTROY(p) DeleteCriticalSection((p))
#  define LOCK(p)         EnterCriticalSection((p))
#  define UNLOCK(p)       LeaveCriticalSection((p))
#  define THREAD_ID       GetCurrentThreadId()
#  define SLEEP(ms)       Sleep(ms)
#  define THREAD_HANDLE   HANDLE
#else
#  define lock_t          pthread_spinlock_t
#  define LOCK_INIT(p)    pthread_spin_init((p), 0)
#  define LOCK_DESTROY(p) pthread_spin_destroy((p))
#  define LOCK(p)         pthread_spin_lock((p))
#  define UNLOCK(p)       pthread_spin_unlock((p))
#  define THREAD_ID       pthread_self()
#  define SLEEP(ms)       usleep(ms * 1000)
#  define THREAD_HANDLE   pthread_t
#endif // #ifdef _MSC_VER

/* ·���ָ��� */
#ifdef _WIN32 
# define DIR_SEP '\\'
#else
# define DIR_SEP '/'
#endif

/************************************************************************/
/* LOG_FILE �ṹ�嶨��                                                  */
/************************************************************************/
#define FILE_BUFFER_SIZE   (16 * 1024)     /* д�����С Ĭ��16k */
#define MAX_PATH_LEN       256             /* ����ļ�·�����  */
#define MAX_FNAME_LEN      512
#define LOG_LINE_LENGTH    2048            /* ��־�г��� */

#if (FILE_BUFFER_SIZE <= LOG_LINE_LENGTH)
# error "Wrong Definition of FILE_BUFFER_SIZE and LOG_LINE_LENGTH"
#endif

#ifdef LOG_HAS_WRITE_THREAD
# define BUFFER_RING_COUNT     256        /* 4M */
# define WRITE_THREAD_SLEEPMS  100        /* д�����߳�poll��� */

typedef struct 
{
    unsigned int size;
    char buf[FILE_BUFFER_SIZE];
} BUF_CHUNK;

#endif

typedef struct LogFile
{
    int           nFd;                      // posix �ļ����
    int           nLogType;                 // ��־�ļ�����
    char          FileName[MAX_FNAME_LEN];  // ��־�ļ�ȫ·������

    char          FinishedFiles[FILE_PRESERVE_COUNT][MAX_FNAME_LEN];
    unsigned int  uFirstIdx;
    unsigned int  uFinished;

    lock_t        lockWrite;              // �ļ�д��
    unsigned long ulSize;                 // ��ǰ�ļ���С
    unsigned long ulDrop;                 // �򻺴治���д���̴���ʧ���ֽ���
    unsigned int  uPos;                   // ��ǰд����λ��

#ifndef LOG_HAS_WRITE_THREAD
    char          Buffer[FILE_BUFFER_SIZE];
#else
    /* ���ζ��� */
    unsigned int  uIdxInput;                   
    unsigned int  uIdxOutput;                  
    BUF_CHUNK     Ring[BUFFER_RING_COUNT]; 
#endif
} LOG_FILE;

typedef struct LogFileName
{
    unsigned int uiTime;                      // �ļ��޸�ʱ��
    char szFileName[MAX_FNAME_LEN];           // �ļ�����

    LogFileName(unsigned int uiTime, char* pFileName)
    {
        this->uiTime = uiTime;
        memset(szFileName, 0, MAX_FNAME_LEN);
        memcpy(szFileName,pFileName,strlen(pFileName));
    };

} LOG_FILENAME;

// ���޸�ʱ����������
bool compare_filename (const LOG_FILENAME& first, const LOG_FILENAME& second)
{
    if (first.uiTime < second.uiTime)
        return true;
    else 
        return false;
}

/************************************************************************/
/* ȫ�ֱ�������                                                         */
/************************************************************************/

/* �쳣�ص�����ָ�� */
void (*g_ExceptionCB)(int code) = NULL; 

/* ģ��������ID */
static char g_FileName[32];
static char g_ModuleType[32];
static int  g_ModuleCode;

/* ʱ��ƫ������ Ĭ�� +8 */
static unsigned int g_TzHour = 8; 
static unsigned int g_TzMin  = 0;
static char         g_TzFlag = '+';

static int  g_bInitOk    = 0;
static int  g_bEnableDbg = 0;
static int  g_LogLevel   = IVS_LOG_INFO;
static char g_LogRootDir[MAX_FNAME_LEN];

static unsigned int g_MaxFileSize  = FILE_MAX_SIZE_DEF;
static unsigned int g_MaxFileCount = FILE_PRESERVE_COUNT;

#ifdef LOG_HAS_SFTP_UPLOAD
/* sftp �ϴ���������ڵ� */
typedef struct FileNode
{
    struct FileNode *pNext;
    int             bIsZip;
    int             nFails;
    char            Name[MAX_FNAME_LEN];
} FNAME_NODE;

/* sftp �ϴ��������� */
static lock_t        g_lockSftp;
static FNAME_NODE   *g_SftpFilesHead;
static unsigned int  g_SftpListSize;

/* sftp ���������� */
static char           g_SftpUser[64];     /* sftp�û���     */
static char           g_SftpPass[64];     /* sftp����       */
static char           g_SftpPath[512];    /* sftp������·�� */
static char           g_SftpSvrAddr[32];  /* sftp��������ַ */
static unsigned short g_SftpPort;         /* sftp�������˿� */

#endif  // end of LOG_HAS_SFTP_UPLOAD

#ifdef LOG_HAS_CONFIG_FILE
static char g_ConfFileName[MAX_FNAME_LEN];
#endif 

#ifdef LOG_HAS_WRITE_THREAD
static int     g_bWriteRun = 1;
THREAD_HANDLE  g_hWriteThread = NULL;
#endif 

#ifdef LOG_HAS_MONITOR_THREAD
static int     g_bMonitorRun = 1;
THREAD_HANDLE  g_hMonitorThread = NULL;
#define   MONITOR_THREAD_SLEEPMS   500    /* ����߳���ѯʱ�� 500ms */
#endif 

/* ��־�ṹ���� */
LOG_FILE g_RunLog;
LOG_FILE g_DbgLog;

#ifdef LOG_HAS_USER
LOG_FILE g_UserLog;
#endif 

#ifdef LOG_HAS_ITF
LOG_FILE g_InterFaceLog;
#endif 

#ifdef LOG_HAS_SEC
LOG_FILE g_SecurityLog;
#endif 

#ifdef LOG_HAS_OPR
LOG_FILE g_OperationLog;
#endif 

const char* g_strLogType[] = {
    "security", 
    "operation",
    "run", 
    "interface",
    "user",
    "debug"
};

const char* g_strLogLevel[] = {
    "Emergency",
    "Alert    ",
    "Critical ",
    "Error    ",
    "Warning  ",
    "Notice   ",
    "Info     ",
    "Debug    "
};


/************************************************************************/
/*  ϵͳ��־�ӿ�                                                        */
/************************************************************************/
/*
 * SysLog
 * ��־ģ�������쳣ʱ��д��ϵͳ��־
 */
#ifdef WIN32
#define LOG_MODULE_FAIL 0x01
static void SysLog(const char *fmt, ...)
{
    HANDLE h = RegisterEventSource(NULL, g_ModuleType);
    if (INVALID_HANDLE_VALUE == h)
    {
        printf("RegisterEventSource Fail\n");
        return;
    }
    
    char MsgBuf[1024];
    va_list args;
    va_start(args, fmt);
    int len = IVS_vsnprintf(MsgBuf, sizeof(MsgBuf) - 1, fmt, args);
    va_end(args);
    MsgBuf[len] = '\0';
    char *Msg = MsgBuf;
    ReportEvent(h, EVENTLOG_ERROR_TYPE, 0, LOG_MODULE_FAIL, 
                  NULL, 1, 0, (LPCSTR *)&Msg, NULL);//lint !e1773
    printf("%s\n", MsgBuf);
    DeregisterEventSource(h);
}
#else
static void SysLog(const char *fmt, ...)
{
    openlog(g_ModuleType, LOG_CONS | LOG_PID, 0);
    va_list ap;
    va_start(ap, fmt);
    vsyslog(LOG_ERR, fmt, ap);
    va_end(ap);
    closelog();
}
#endif 

/************************************************************************/
/* �����ļ���ȡ�ӿ�                                                     */
/************************************************************************/
/*
 * ReadConfigure
 * ��ȡ��־�����ļ�
 */
#ifdef LOG_HAS_CONFIG_FILE
static int ReadConfigure(const char* szConf)
{
    static time_t tmLastModify = 0;
    IVS_STATE  _st;
    if (IVS_stat(szConf, &_st))
    {
        printf("Log Error: Read Config %s Fail error: %d\n", szConf, errno);
        return 0;
    }

    if (_st.st_mtime != tmLastModify)
    {
        tmLastModify = _st.st_mtime;
        if (ReadConfig(szConf))
        {
            printf("Log Error: Some error in Configure file %s\n", szConf);
        }
    }
    return 0;
}
#endif

/************************************************************************/
/* SFTP ���ܽӿ�                                                        */
/************************************************************************/

#ifdef LOG_HAS_SFTP_UPLOAD
/*
 * SftpTransferInit
 * ��ʼ��Sftp�ϴ�����
 */
static int SftpTransferInit()
{
    LOCK_INIT(&g_lockSftp);
    g_SftpFilesHead = NULL;
    return 0;
}

/*
 * SftpTransferClean
 * ���Sftp�ϴ�����
 */
static void SftpTransferClean()
{
    FNAME_NODE *pNode, *pNext;
    LOCK(&g_lockSftp);
    pNode = g_SftpFilesHead;
    while (pNode)
    {
        pNext = pNode->pNext;
        free(pNode);   // ֻɾ���ڵ㣬�ļ��´γ�������ʱ�����ϴ�
        pNode = pNext;
    }
    UNLOCK(&g_lockSftp);
    LOCK_DESTROY(&g_lockSftp);
}

/*
 * SftpInsertTransList
 * �ļ�szName����Sftp�ϴ�������
 */
static int SftpInsertTransList(char *szName)
{
    FNAME_NODE *newNode = (FNAME_NODE *)malloc(sizeof(FNAME_NODE));
    if (NULL == newNode)
    {
        SysLog("Insert Sftp List Fail, Out of Memory Del File %s\n", szName);
        return -1;
    }
    else 
    {
        memset(newNode, 0, sizeof(FNAME_NODE));
        newNode->bIsZip = strstr(szName, ".zip")? 1 : 0;
        strcpy(newNode->Name, szName);
        LOCK(&g_lockSftp);
        FNAME_NODE **ppNode = &g_SftpFilesHead;
        while (*ppNode)
        {
            ppNode = &((*ppNode)->pNext);
        }
        *ppNode = newNode;
        g_SftpListSize++;
        UNLOCK(&g_lockSftp);
    }
    return 0;
}

/*
 * SftpTransfer
 * sftp�ϴ��ļ�
 */
static int SftpTransfer()
{
    FNAME_NODE *pNode = NULL;
    LOCK(&g_lockSftp);
    pNode = g_SftpFilesHead;
    if (NULL != pNode)
    {
        g_SftpFilesHead = pNode->pNext;
        pNode->pNext = NULL;
    }
    UNLOCK(&g_lockSftp);

    if (NULL == pNode)
    {
        return 0;
    }
  
    if (!pNode->bIsZip)
    {
        // TODO  ѹ���ļ���ɾ�����ļ�


        pNode->bIsZip = 1;
    }

    /**********************************/
    int error = 0; // sftp upload ���sftp�ϴ����� TODO
    /**********************************/
    if (error)
    {
        if (pNode->nFails++ > 3)
        {
            IVS_unlink(pNode->Name);
        }
        else
        {
            LOCK(&g_lockSftp);
            if (g_SftpFilesHead)
            {
                pNode->pNext = g_SftpFilesHead;
                g_SftpFilesHead = pNode;
            }
            else
            {
                g_SftpFilesHead = pNode;
            }
            UNLOCK(&g_lockSftp);
        }
        return -1;
    }
    printf("Unlink file: %s\n", pNode->Name);
    if (IVS_unlink(pNode->Name))
    {
        SysLog("Unlink File %s Fail,errno = %d", pNode->Name, errno);
    }
    free(pNode);
    return 0;
}
#endif // #ifdef LOG_HAS_SFTP_UPLOAD

/************************************************************************/
/* ��־�ļ�����                                                         */
/************************************************************************/
/*
 * ExtractFinishedFile
 * �Ƴ������������޵ı����ļ�
 */
static void RemoveExtraFiles(LOG_FILE *pFile)
{
    if (pFile->uFinished + 1 < g_MaxFileCount)
    {
        return;
    }
    unsigned int toDel = static_cast<unsigned int>((pFile->uFinished + 1 - g_MaxFileCount) + 1);

    for (unsigned int i = 0; i < toDel; i++)
    {
        char *pName = pFile->FinishedFiles[pFile->uFirstIdx];
#ifdef LOG_HAS_SFTP_UPLOAD
        if (SftpInsertTransList(pName))
        {
            IVS_unlink(pName); // �ڴ����ʧ�ܣ�ֱ��ɾ���ļ�
        }
#else
        IVS_unlink(pName);
#endif 
        pFile->uFirstIdx++;
        if (pFile->uFirstIdx == FILE_PRESERVE_COUNT)
        {
            pFile->uFirstIdx = 0;
        }
    }
    pFile->uFinished -= toDel;
}

/*
 * HandleRenameFail
 * �ļ�������ʧ�ܴ���
 * 1.�����µ��ļ�
 * 2.�����ļ����ݿ��������ļ�
 * 3.ɾ�����ļ���ɾ��ʧ����������ļ�����
 */
void HandleRenameFail(const char * pszOldFileName, const char * pszNewFileName)
{
    if (NULL == pszOldFileName || NULL == pszNewFileName)
    {
        return;
    }

    std::ifstream ifs(pszOldFileName, std::ios::binary);
    if (!ifs.is_open())
    {
        return;
    }

    std::ofstream ofs(pszNewFileName, std::ios::binary | std::ios::trunc);
    if (!ofs.is_open())
    {
        return;
    }

    ofs << ifs.rdbuf();

    ofs.close();
    ifs.close();

    if (0 != IVS_unlink(pszOldFileName)) //lint !e534
    {
        std::ofstream trun(pszOldFileName, std::ios::binary | std::ios::trunc);
        trun.close();
    }
}

/*
 * BackupFinishedFile
 * ��������ɵ��ļ�
 */
static void BackupFinishedFile(LOG_FILE *pFile, time_t _tt)
{
    struct tm _tm;
    (void)IVS_localtime(&_tm, &_tt);
    char NewFileName[MAX_FNAME_LEN] = {0};

    /* ���ݸ�ʽ SMU1_operation_20121010101010 */
    (void)IVS_snprintf(NewFileName, MAX_FNAME_LEN - 1, "%s%c%s%c%s_%s_%04d%02d%02d%02d%02d%02d.log", 
                       g_LogRootDir, DIR_SEP, g_strLogType[pFile->nLogType], DIR_SEP, 
                       g_FileName, g_strLogType[pFile->nLogType],
                       _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday, _tm.tm_hour, _tm.tm_min, _tm.tm_sec);

    if (IVS_rename(pFile->FileName, NewFileName))
    {
        SysLog("Rename file Fail, error = %d,src filename = %s,dst filename = %s", errno, pFile->FileName, NewFileName);
        HandleRenameFail(pFile->FileName, NewFileName);
    }

    /* ���ݾ��ļ� */ 
    RemoveExtraFiles(pFile);
    unsigned int uInsAt = (pFile->uFinished + pFile->uFirstIdx) % FILE_PRESERVE_COUNT;
	//modify by limingjie lWX175483: 2013-5-24
	//strcpy �ĳ� strncpy
    strncpy(pFile->FinishedFiles[uInsAt], NewFileName, MAX_FNAME_LEN-1);
    pFile->uFinished++;
}

/*
 * logFileOpen
 * ����־�ļ�
 */
static int LogFileOpen(LOG_FILE *pFile)
{
	//////////////////////////////////////////////////////////////////////////
	//���ⵥ DTS2013022002725
	//���ݹ淶ֻ���ļ�����10Mʱ�ű���
    ///* �����ļ��������䱸�� */
    if (!IVS_access(pFile->FileName, 0))
    {    
        IVS_STATE _st;
        IVS_stat(pFile->FileName, &_st);
		pFile->ulSize = static_cast<unsigned long>(_st.st_size);		//�����ļ���С
		if (pFile->ulSize >= g_MaxFileSize)
		{
			BackupFinishedFile(pFile, _st.st_mtime); 
			pFile->ulSize = 0;
		}
    }
	//////////////////////////////////////////////////////////////////////////
    pFile->nFd = IVS_open(pFile->FileName, O_RDWR|O_CREAT|O_APPEND, S_IWRITE|S_IREAD);
    if (pFile->nFd < 0)
    {
		pFile->ulSize = 0;
        SysLog("Open File %s Fail, error = %d", pFile->FileName, errno);
        if (g_ExceptionCB)
        {
            g_ExceptionCB(LOG_ERR_OPENFAIL);
        }
        return -1;
    }
    return 0;
}

/*
 * LogFileClose
 * �ر���־�ļ�������
 */
static int LogFileClose(LOG_FILE *pFile)
{
    if (pFile->nFd < 0)
    {
        return 0;
    }

    IVS_close(pFile->nFd);
    pFile->nFd = -1;
    pFile->ulSize = 0;
    BackupFinishedFile(pFile, time(NULL));
    return 0;
}

/* 
 * FileWrite
 * д�ļ��ײ�IO��װ
 */
static unsigned int FileWrite(int nFd, const char *buffer, unsigned int count)
{
    size_t toWrite = count;
    unsigned int hasWrote = 0;
    while (hasWrote < count)
    {
        int n = IVS_write(nFd, buffer + hasWrote, toWrite);
        if (n <= 0)
        {
            break;
        }
        toWrite -= n; //lint !e737 ��ʱnֵ�Ǹ�
        hasWrote += n; //lint !e737 ��ʱnֵ�Ǹ�
    }
	IVS_fsync(nFd);
	return hasWrote;
}

/*
 * LogFileWrite
 * д����־�ļ�����
 */
#ifdef LOG_HAS_WRITE_THREAD
static size_t LogFileWrite(LOG_FILE *pFile, const char *data, size_t len)
{
    char  *pBuf = NULL;
    size_t uWrite = len;

    LOCK(&pFile->lockWrite);

    BUF_CHUNK *pChunk = &pFile->Ring[pFile->uIdxInput];
    pBuf = pChunk->buf;

    /*
     * д�볤�ȴ��ڱ��ڵ�ʣ��ռ�,
     * ���ȡ��һ��buffer�����ֱ��ڵ���־������ 
     */
    if ((pFile->uPos + len) > FILE_BUFFER_SIZE)
    {
        unsigned int nextInput = pFile->uIdxInput + 1;
        if (nextInput == BUFFER_RING_COUNT)
        {
            nextInput = 0;
        }

        if (nextInput == pFile->uIdxOutput)
        {
            pFile->ulDrop += len;
            uWrite = 0;
            SysLog("Write File %s, Drop %u Bytes", pFile->FileName, len);
        }
        else
        {
            pFile->uIdxInput = nextInput;
            pChunk = &pFile->Ring[pFile->uIdxInput];
            pBuf = pChunk->buf;
            memcpy(pBuf, data, len);
            pChunk->size = pFile->uPos = len;
        }
    }
    else
    {
        memcpy(pBuf + pFile->uPos, data, len);
        pFile->uPos  += len;
        pChunk->size += len;
    }
    
    UNLOCK(&pFile->lockWrite);
    return uWrite;
}

/*
 * FlushRingBuffer
 * ˢ�»��λ������ݵ��ں�
 */
void FlushRingBuffer(LOG_FILE *pFile)
{
	unsigned int uIdxInput  = pFile->uIdxInput;
    unsigned int uIdxOutput = pFile->uIdxOutput;
    int bLocked = 0;
    
    if (pFile->nFd < 0 && LogFileOpen(pFile))
    {
        return;
    }

    /* �����ζ���Ϊ��, ����ˢ�µ�ǰ���� */
    if (uIdxInput == uIdxOutput)
    {
        LOCK(&pFile->lockWrite);
        if (!pFile->uPos)
        {
            UNLOCK(&pFile->lockWrite);
            return;
        }
        /*
         * �ڴ˹����������߳̿��ܸı�pFile->uIdxInput��ֵ,
         * ����ˢ�� uIdxInput
         */
        uIdxInput = pFile->uIdxInput;
        bLocked = 1;
    }

    while (uIdxInput != uIdxOutput)
    {
        BUF_CHUNK *pChunk = &pFile->Ring[uIdxOutput];
        unsigned int hasWrote = FileWrite(pFile->nFd, pChunk->buf, pChunk->size);
        if (hasWrote < pChunk->size)
        {
            int err = errno;
            SysLog("Write File %s error, errno = %d", pFile->FileName, err);
            if (ENOSPC == err && g_ExceptionCB)
            {
                g_ExceptionCB(LOG_ERR_DISKFULL);
            }
        }
    
        pFile->ulSize += hasWrote;
        pChunk->size = 0;

        uIdxOutput++;
        if (uIdxOutput == BUFFER_RING_COUNT)
        {
            uIdxOutput = 0;
        }

        if (pFile->ulSize >= g_MaxFileSize)
        {
            (void)LogFileClose(pFile);
            LogFileOpen(pFile);
        }
    }

    if (bLocked)
    {
        BUF_CHUNK *pChunk = &pFile->Ring[uIdxOutput];
        unsigned int hasWrote = FileWrite(pFile->nFd, pChunk->buf, pFile->uPos);
        if (hasWrote < pFile->uPos)
        {
            int err = errno;
            SysLog("Write File %s error, errno = %d", pFile->FileName, err);
            if (ENOSPC == err && g_ExceptionCB)
            {
                g_ExceptionCB(LOG_ERR_DISKFULL);
            }
            
        }
        pFile->ulSize += hasWrote;
        pChunk->size = 0;
        pFile->uPos = 0;

        UNLOCK(&pFile->lockWrite);
        if (pFile->ulSize >= g_MaxFileSize)
        {
            (void)LogFileClose(pFile);
            LogFileOpen(pFile);
        }
    }
    pFile->uIdxOutput = uIdxOutput;
}

/*
 * ClearRingBuffer
 * ˢ�����л��ζ����еĻ���
 */
static void ClearRingBuffer(LOG_FILE *pFile)
{
    FlushRingBuffer(pFile); /* ������������ */
    FlushRingBuffer(pFile); /* ����δ������ */
}

/*
 * WriteProc
 * д������ѯ�̺߳���
 */
static long WriteProc(void *arg)
{
#if defined LOG_HAS_CONFIG_FILE
    unsigned int uConfTimer = 0 ;
#endif   
    
    while (g_bWriteRun)
    {
        SLEEP(WRITE_THREAD_SLEEPMS);
        FlushRingBuffer(&g_RunLog);
        FlushRingBuffer(&g_DbgLog);
#ifdef LOG_HAS_ITF
        FlushRingBuffer(&g_InterFaceLog);
#endif 
#ifdef LOG_HAS_SEC
        FlushRingBuffer(&g_SecurityLog);
#endif 

#ifdef LOG_HAS_USER
        FlushRingBuffer(&g_UserLog);
#endif 

#ifdef LOG_HAS_OPR
        FlushRingBuffer(&g_OperationLog);
#endif

#if defined LOG_HAS_CONFIG_FILE
        if ((uConfTimer += WRITE_THREAD_SLEEPMS) > 1000 /* 1s */)
        {
            (void)ReadConfigure(g_ConfFileName);
            uConfTimer = 0;
        }
#endif 
    }
    
    /*
     * ����ʹ����־ģ��������߳̾��˳�, 
     * �����־�����еĲ������� 
     */
    ClearRingBuffer(&g_RunLog);
    ClearRingBuffer(&g_DbgLog);
#ifdef LOG_HAS_ITF
    ClearRingBuffer(&g_InterFaceLog);
#endif 
#ifdef LOG_HAS_SEC
    ClearRingBuffer(&g_SecurityLog);
#endif 

#ifdef LOG_HAS_USER
    ClearRingBuffer(&g_UserLog);
#endif 

#ifdef LOG_HAS_OPR
    ClearRingBuffer(&g_OperationLog);
#endif
    
    return 0;
}//lint !e818 arg ��������Ϊconst

/*
 *  LogManageProc
 */
# ifdef _MSC_VER
static DWORD WINAPI LogWriteProc(LPVOID lpParameter)
{
    return (DWORD)WriteProc(lpParameter);
}
# else
static void * LogWriteProc(void *lpParameter)
{
    return (void *)WriteProc(lpParameter);
}
# endif // _MSC_VER


#else // !LOG_HAS_WRITE_THREAD

static size_t LogFileWrite(LOG_FILE *pFile, char *data, size_t len)
{
    LOCK(&pFile->lockWrite);
    if (pFile->nFd < 0 && LogFileOpen(pFile))
    {
        UNLOCK(&pFile->lockWrite);
        return -1;
    }
    
    /*
     * д�볤�ȴ���ʣ��ռ�,��ˢ�����ݵ��ں�
     */
    if ((pFile->uPos + len) > FILE_BUFFER_SIZE)
    {
        int hasWrote = FileWrite(pFile->nFd, pFile->Buffer, pFile->uPos);
        if ((unsigned int)hasWrote < pFile->uPos)
        {
            int err = errno;
            SysLog("Write File %s error, errno = %d", pFile->FileName, err);
            if (ENOSPC == err && g_ExceptionCB)
            {
                g_ExceptionCB(LOG_ERR_DISKFULL);
            }
        }
        pFile->ulSize += hasWrote;
        pFile->uPos = 0;
 
        if (pFile->ulSize >= g_MaxFileSize)
        {
            LogFileClose(pFile);
        }
    }

    memcpy(pFile->Buffer + pFile->uPos, data, len);
    pFile->uPos += len;
    UNLOCK(&pFile->lockWrite);
    return len;
}
#endif  // ifdef LOG_HAS_WRITE_THREAD


/*
 * LogFileFlush
 * ˢ�»������ݵ��ں�ͳһ�ӿ�
 */
static void LogFileFlush(LOG_FILE *pFile)
{
#ifdef LOG_HAS_WRITE_THREAD
    /* д�����߳��Զ�Flush ����ֱ��Sleep���� */
    SLEEP(5);
#else
    LOCK(&pFile->lockWrite);
    if (pFile->uPos > 0 && pFile->nFd >= 0)
    {
        FileWrite(pFile->nFd, pFile->Buffer, pFile->uPos);
    }
    pFile->uPos = 0;
    pFile->ulSize += pFile->uPos;
    /* check size */
    if (pFile->ulSize >= g_MaxFileSize)
    {
        LogFileClose(pFile);
    }
    UNLOCK(&pFile->lockWrite);
#endif 
}//lint !e818 ��������Ϊconst����������Ϊ���ö���д�߳�ʱ��Ч

#ifndef LOG_HAS_WRITE_THREAD
/*
 *  FlushLogAll
 *  ˢ�������ļ�����
 */
static void FlushLogAll()
{
    LogFileFlush(&g_RunLog);
    LogFileFlush(&g_DbgLog);
#ifdef LOG_HAS_ITF
    LogFileFlush(&g_InterFaceLog);
#endif 
#ifdef LOG_HAS_SEC
    LogFileFlush(&g_SecurityLog);
#endif 

#ifdef LOG_HAS_USER
    LogFileFlush(&g_UserLog);
#endif 

#ifdef LOG_HAS_OPR
    LogFileFlush(&g_OperationLog);
#endif
}
#endif

#if defined LOG_HAS_MONITOR_THREAD
/*
 * MonitorProc 
 * ����̺߳���
 */
static long MonitorProc(void *arg)
{
#if defined LOG_HAS_CONFIG_FILE  && !defined LOG_HAS_WRITE_THREAD
    unsigned long uConfTimer = 0;
#endif 
    
    while (g_bMonitorRun)
    {
        SLEEP(MONITOR_THREAD_SLEEPMS);
#ifdef LOG_HAS_SFTP_UPLOAD
        SftpTransfer();
#endif 

/*
 * �����������ļ���δ����д�����߳�, 
 * ���߳����ȡ�����ļ� 
 */
#if defined LOG_HAS_CONFIG_FILE  && !defined LOG_HAS_WRITE_THREAD
        if (uConfTimer += MONITOR_THREAD_SLEEPMS > 1000 /* 1s */)
        {
            ReadConfigure(g_ConfFileName);
            uConfTimer = 0;
        }
#endif 

/*
 * ��д�����̣߳����̶߳�ʱˢ����־�ļ����� 
 */
#ifndef LOG_HAS_WRITE_THREAD
        FlushLogAll();
#endif 
    }
    return 0;
}

/*
 *  LogManageProc
 */
# ifdef _MSC_VER
static DWORD WINAPI LogMonitorProc(LPVOID lpParameter)
{
    return (DWORD)MonitorProc(lpParameter);
}
# else
static void * LogMonitorProc(void *lpParameter)
{
    return (void *)MonitorProc(lpParameter);
}
# endif // _MSC_VER

#endif  // defined LOG_HAS_MONITOR_THREAD

// ��ʼ�����ζ���
static void InitRingQueue( std::list<LOG_FILENAME> &aFileNameList, LOG_FILE *&pFile )
{
    int iBackupCount = 0;
    if (aFileNameList.size() > g_MaxFileCount -1 - pFile->uFinished)
    {
        iBackupCount = aFileNameList.size() - (g_MaxFileCount - 1 - pFile->uFinished);
    }
    // ɾ����������ļ������ļ�
    std::list<LOG_FILENAME>::iterator iter = aFileNameList.begin();
    for (int iIndex = 0; iIndex < iBackupCount; ++iIndex, ++iter)
    {
#ifdef LOG_HAS_SFTP_UPLOAD
        if (SftpInsertTransList(iter->szFileName))
        {
            IVS_unlink(iter->szFileName);
        }
#else
        IVS_unlink(iter->szFileName); //lint !e534
#endif
    }
    // ʣ���ļ���ʼ�����ζ���
    while (aFileNameList.end() != iter)
    {
        unsigned int uiInsertIndex = 
            (pFile->uFinished + pFile->uFirstIdx) % FILE_PRESERVE_COUNT;
        strncpy(pFile->FinishedFiles[uiInsertIndex], iter->szFileName, MAX_FNAME_LEN);
        pFile->FinishedFiles[uiInsertIndex][MAX_FNAME_LEN - 1] = '\0';
        ++pFile->uFinished;
        ++iter;
    }
}

#ifdef _MSC_VER
/*
 * ScanDirectory
 * ��ʼ��ʱ��鵱ǰĿ¼���Ѵ�����־
 */
static void ScanDirectory(char *szDir, LOG_FILE *&pFile)
{
    char szTempDir[MAX_PATH+1];
    (void)IVS_snprintf(szTempDir, MAX_PATH, "%s\\*", szDir);
    WIN32_FIND_DATA FileData;
    memset(&FileData, 0, sizeof(FileData));

    HANDLE hFindFile = FindFirstFile(szTempDir, &FileData);
    if (INVALID_HANDLE_VALUE == hFindFile)
    {
        return;
    }
    /************************************************************************
    * ��ʼ����־���̣�
    * 1.�����ļ������Ѵ��ڵ���־�ļ������˵�������λ�������ֵ��ļ�������+.log����
    * 2.�����ļ������ļ��޸�ʱ����Ϣ��
    * 3.���ļ����޸�ʱ���������У�
    * 4.ɾ����������ļ������ļ���
    * 5.��ʣ���ļ���ʼ�����ζ��С�
    ************************************************************************/
    std::list<LOG_FILENAME> aFileNameList;
    aFileNameList.clear();
    do
    {
        if (FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
        {
            continue;
        }
        else
        {
            char szName[MAX_PATH+1] = {0};
            (void)IVS_snprintf(szName, MAX_PATH, "%s\\%s", szDir, FileData.cFileName);
            IVS_STATE st;
            IVS_stat(szName, &st);

            // ����־�ļ����к���ʱ����Ϣ�ֶε��ļ�, ��������ɵ��ļ�ӦΪ����+��.log��
            if (szName[strlen(szName) - 5] <= '9' && szName[strlen(szName) - 5] >= '0')
            {
                LOG_FILENAME stLogFileName((unsigned int)st.st_mtime, szName);
                aFileNameList.push_back(stLogFileName);
            }
        }
    }
    while(FindNextFile(hFindFile,&FileData));
    FindClose(hFindFile);
    // ���޸�ʱ�併������
    aFileNameList.sort(compare_filename);
    // ��ʼ�����ζ���
    InitRingQueue(aFileNameList, pFile);
}

#endif // _MSC_VER

#if defined __GNUC__ || defined LINUX
//add by wangguangyan start
void  CreatLogDir(const char *pszDir)  
{ 
	printf("Enter CreatLogDir pszDir = %s\n",pszDir);
	char DirName[256] = {0};      
	strcpy(DirName,pszDir);      
	int len = strlen(DirName);      
	if(DirName[len-1]!='/')
	{
		strcat(DirName,"/");
	}
	len = strlen(DirName);      
	for(int i=1;i<len;i++)      
	{      
		if(DirName[i]=='/')      
		{      
			DirName[i] = 0;      
			if(access(DirName,NULL) != 0)      
			{      
				if(mkdir(DirName,0777) == -1)      
				{       
					perror("mkdir error");       
					return ;       
				}      
			}    
			DirName[i] = '/';      
		}      
	}        
	printf("Leave CreatLogDir \n");
}
//add by wangguangyan end
static void ScanDirectory(char *szDir, LOG_FILE *pFile)
{
	//add by wangguangyan start
	if(access(szDir,0) != 0)//�ж��Ƿ������־Ŀ¼
	{
		CreatLogDir(szDir);//�½���־Ŀ¼
	}
	//add by wangguangyan end

    struct dirent *ent = NULL;   
    DIR  *pDir = opendir(szDir);
	if(NULL == pDir)
	{
		printf("opendir[%s]failed\n.",szDir);
		return;
	}
	printf("opendir[%s] success\n",szDir);


    std::list<LOG_FILENAME> aFileNameList;
    aFileNameList.clear();
    while ((ent = readdir(pDir))!= NULL) 
    {  
        if(ent->d_type & DT_DIR)
        {  
            continue;
        }  
        else
        {
            char szName[PATH_MAX+1] = {0};
            IVS_snprintf(szName, PATH_MAX, "%s/%s", szDir, ent->d_name);

            IVS_STATE st;
            IVS_stat(szName, &st);
            // ����־�ļ����к���ʱ����Ϣ�ֶε��ļ�, ��������ɵ��ļ�ӦΪ����+��.log��
            if (szName[strlen(szName) - 5] <= '9' && szName[strlen(szName) - 5] >= '0')
            {
                LOG_FILENAME stLogFileName((unsigned int)st.st_mtime, szName);
                aFileNameList.push_back(stLogFileName);
            }
        }  
    }
    closedir(pDir);
    // ���޸�ʱ�併������
    aFileNameList.sort(compare_filename);
    // ��ʼ�����ζ���
    InitRingQueue(aFileNameList, pFile);


}
#endif // defined __GNUC__ || defined LINUX

/*
 * LogFileInit
 * ��ʼ��LOG_FILE�ṹ
 */
static int LogFileInit(LOG_FILE *pFile, int nType)
{
    char subDir[MAX_FNAME_LEN] = {0};
    size_t pathLen = strlen(g_LogRootDir);
    memcpy(subDir, g_LogRootDir, pathLen);
    subDir[pathLen++] = DIR_SEP;

    /* ���Ŀ¼ */
	//modify by limingjie lWX175483 :2013-5-24
	//strcpy �ĳ� strncpy
    strncpy(subDir + pathLen, g_strLogType[nType], strlen(g_strLogType[nType])+1);
    pathLen = strlen(subDir);
    subDir[pathLen++] = DIR_SEP;
#ifdef WIN32
    if (0 > IVS_access(subDir, 0) && !MakeSureDirectoryPathExists(subDir)/*0 > IVS_mkdir(subDir)*/)
#else
	if (0 > IVS_access(subDir, 0) && access(subDir,R_OK|W_OK) == 0/*0 > IVS_mkdir(subDir)*/)
#endif
    { 
        printf("Log Error: Create Log Directory %s Fail\n", subDir);
        return -1;
    }
    memset(pFile, 0, sizeof(LOG_FILE));
    ScanDirectory(subDir, pFile); 
    pFile->nLogType = nType;
    pFile->nFd = -1;
    LOCK_INIT(&pFile->lockWrite);
    (void)IVS_snprintf(pFile->FileName, sizeof(pFile->FileName) - 1, 
             "%s%c%s_%s.log", subDir, DIR_SEP, g_FileName, g_strLogType[nType]);
    return 0;
}

/*
 * LogFileClean
 * ����LOG_FILE�ṹ
 */
static void LogFileClean(LOG_FILE *pFile)
{
    LogFileFlush(pFile);
    //LogFileClose(pFile);             //lint !e534 �����Ƿ���ֵ
    // cleanʱ����д���ļ�����Ҫ���� modify by xiongfeng192614
    if (pFile->nFd >= 0)
    {
        IVS_close(pFile->nFd);
        pFile->nFd = -1;
        pFile->ulSize = 0;
    }
    // modify end
	LOCK_DESTROY(&pFile->lockWrite);
}

/************************************************************************/
/* �ⲿ�ӿ�                                                             */
/************************************************************************/
/*
 * IVS_logInit
 * @szDir:        ��־����Ŀ¼, �����Ѵ���
 * @szConf:       �����ļ�·��, ʹ�������ļ������
 * @szModuleType: ģ������
 * @uModuleID:    ģ��ID
 * @callback:     �쳣�ص�����
 */
int IVS_LogInit(const char *szDir, const char *szConf, const char* szFileName, const char *szModuleType, 
                int nModuleID, void (*callback)(int code))
{
#ifdef LOG_HAS_CONFIG_FILE 
    if (NULL == szDir || NULL == szFileName || NULL == szConf || NULL == szModuleType)
#else
    if (NULL == szDir || NULL == szFileName || NULL == szModuleType)
#endif 
    {
        printf("Log Error: Log Init Fail, Invalid parameter\n");
        return -1;
    }

#ifdef LOG_HAS_STACK_TRACE
    if (IVS_BackTraceInit())
    {
        printf("Log Error: Init BackTrace Fail\n");
        return -1;
    }
#endif

#ifdef LOG_HAS_SFTP_UPLOAD
    SftpTransferInit();
#endif 

#ifdef LOG_HAS_CONFIG_FILE
    memset(g_ConfFileName, 0, sizeof(g_ConfFileName));
    strncpy(g_ConfFileName, szConf, sizeof(g_ConfFileName) - 1);
    if (ReadConfigure(g_ConfFileName))
    {
        printf("Log Error: Read Configure file %s Fail Use Default Configure\n", g_ConfFileName);
    }
#endif
    memset(g_FileName, 0, sizeof(g_FileName));
    strncpy(g_FileName,  szFileName, sizeof(g_FileName) - 1);

    memset(g_ModuleType, 0, sizeof(g_ModuleType));
    strncpy(g_ModuleType,  szModuleType, sizeof(g_ModuleType) - 1);
    g_ModuleCode = nModuleID;

    /* ��鲢������־��Ŀ¼ */
    int pathLen = IVS_snprintf(g_LogRootDir, sizeof(g_LogRootDir) - 1,
             "%s%c%s", szDir, DIR_SEP, szModuleType);
    g_LogRootDir[pathLen++] = DIR_SEP;
    g_LogRootDir[pathLen] = 0;
    if (pathLen >= MAX_PATH_LEN)
    {
        printf("Log Error: Out of Mem, Log Path too deep\n");
        return -1;
    }
#ifdef WIN32
    if (0 > IVS_access(g_LogRootDir, 0) && !MakeSureDirectoryPathExists(g_LogRootDir)/*0 > IVS_mkdir(g_LogRootDir)*/)
#else
	if (0 > IVS_access(g_LogRootDir, 0) && access(g_LogRootDir,R_OK|W_OK) == 0/*0 > IVS_mkdir(subDir)*/)
#endif
    {
        printf("Log Error: Create Log Root Directory Fail\n");
        return -1;
    }

    int error = 0;
    error += LogFileInit(&g_RunLog, TYPE_RUN);
    error += LogFileInit(&g_DbgLog, TYPE_DBG);

#ifdef LOG_HAS_ITF
    error += LogFileInit(&g_InterFaceLog, TYPE_ITF);
#endif 

#ifdef LOG_HAS_SEC
    error += LogFileInit(&g_SecurityLog, TYPE_SEC);
#endif 

#ifdef LOG_HAS_USER
    error += LogFileInit(&g_UserLog, TYPE_USR);
#endif 

#ifdef LOG_HAS_OPR
    error += LogFileInit(&g_OperationLog, TYPE_OPR);
#endif

    if (error)
    {
        printf("Init Log File error\n");
        return -1;
    }

#ifdef LOG_HAS_WRITE_THREAD
# ifdef _MSC_VER
    g_hWriteThread = CreateThread(NULL, 0, LogWriteProc, NULL, 0, NULL);
    if (NULL == g_hWriteThread)
    {
        printf("Create Log Write Thread Fail\n");
        return -1;
    }
# else
    if (pthread_create(&g_hWriteThread, NULL, LogWriteProc, NULL))
    {
        printf("Create Log Write Thread Fail\n");
        return -1;
    }
# endif // _MSC_VER
#endif // ifdef  LOG_HAS_WRITE_THREAD

#ifdef LOG_HAS_MONITOR_THREAD
# ifdef _MSC_VER
    g_hMonitorThread = CreateThread(NULL, 0, LogMonitorProc, NULL, 0, NULL);
    if (NULL == g_hMonitorThread)
    {
        printf("Create Log Monitor Thread Fail\n");
        return -1;
    }
# else
    if (pthread_create(&g_hMonitorThread, NULL, LogMonitorProc, NULL))
    {
        printf("Create Log Write Thread Fail\n");
        return -1;
    }
# endif  // ifdef _MSC_VER 
#endif  // ifdef LOG_HAS_MONITOR_THREAD
    
    g_ExceptionCB = callback;
    g_bInitOk = 1;
    return 0;
}

/*
 * IVS_LogClean
 */
void IVS_LogClean()
{
#ifdef LOG_HAS_WRITE_THREAD
    g_bWriteRun = 0;
#ifdef _MSC_VER
    WaitForSingleObject(g_hWriteThread, INFINITE); 
#else
    pthread_join(g_hWriteThread, NULL);
#endif 
    g_hWriteThread = NULL;
    g_bWriteRun = 1;
#endif 

#ifdef LOG_HAS_MONITOR_THREAD
    g_bMonitorRun = 0;
# ifdef _MSC_VER
    WaitForSingleObject(g_hMonitorThread, INFINITE); 
# else
    pthread_join(g_hMonitorThread, NULL);
# endif 
    g_hMonitorThread = NULL;
    g_bMonitorRun = 1;
#endif 

#ifdef LOG_HAS_SFTP_UPLOAD
    SftpTransferClean();
#endif 

    LogFileClean(&g_RunLog);
    LogFileClean(&g_DbgLog);
#ifdef LOG_HAS_ITF
    LogFileClean(&g_InterFaceLog);
#endif 

#ifdef LOG_HAS_SEC
    LogFileClean(&g_SecurityLog);
#endif 

#ifdef LOG_HAS_USER
    LogFileClean(&g_UserLog);
#endif 

#ifdef LOG_HAS_OPR
    LogFileClean(&g_OperationLog);
#endif  

#ifdef LOG_HAS_STACK_TRACE
    IVS_BackTraceClean();
#endif
}

/*
 * IVS_LogSetSftp
 * ����sftp����
 */
#ifdef LOG_HAS_SFTP_UPLOAD
int IVS_LogSetSftp(char *szSvrDir, char* szUser, char *szPassWord,
                char *szSvrAddr, unsigned short usSvrPort)
{
    if (NULL == szSvrDir || NULL == szUser 
        || NULL == szPassWord || NULL == szSvrAddr)
    {
        return  -1;
    }

    strncpy(g_SftpPath, szSvrDir, sizeof(g_SftpPath) - 1);
    strncpy(g_SftpUser, szUser, sizeof(g_SftpUser) - 1);
    strncpy(g_SftpPass, szPassWord, sizeof(g_SftpPass) - 1);
    strncpy(g_SftpSvrAddr, szSvrAddr, sizeof(g_SftpSvrAddr) - 1);
    g_SftpPort = usSvrPort;
   
    return 0;
}
#endif


/*
 * IVS_LogSetLevel
 * ������־����
 */
int IVS_LogSetLevel(int level)
{
    if (g_LogLevel < IVS_LOG_EMERG || g_LogLevel > IVS_LOG_DEBUG)
    {
        return -1;
    }
    g_LogLevel = level;
    return 0;
}

/*
 * IVS_LogSetDebug
 * ʹ�ܵ�����־����
 */
int IVS_LogSetDebug(int enable)
{
    g_bEnableDbg = enable;
    return 0;
}


/*
 * IVS_LogSetFileSize
 * �����ļ����ޣ���С10M
 */
int IVS_LogSetFileSize(unsigned long s)
{
    g_MaxFileSize = s < FILE_MAX_SIZE_DEF ? FILE_MAX_SIZE_DEF : s;
    return 0;
}

/*
 * IVS_LogFileCount
 * ���ñ����ļ�����
 */
int IVS_LogSetFileCount(unsigned int c)
{
    g_MaxFileCount = (c > FILE_PRESERVE_COUNT) ? FILE_PRESERVE_COUNT : c;
    return 0;
}

/*
 *  IVS_LogSetTimeZone
 *  ����ʱ��
 */
void IVS_LogSetTimeZone(unsigned int hour, unsigned int min, int flag)
{
    g_TzFlag = (flag == UTC_FLG_PLUS)? '+' : '-';
    g_TzHour = hour;
    g_TzMin  = min;
}

/************************************************************************/
/* д��־�ӿ�                                                           */
/************************************************************************/

/*
 *  FillTime
 *  �������ʱ��
 */
static unsigned int FillTime(char *buf, unsigned int len)
{
    time_t _tt = time(NULL);
    struct tm _tm;
    IVS_localtime(&_tm, &_tt); //lint !e534 �����Ƿ���ֵ

    struct timeval tv;
    IVS_gettimeofday(&tv);    //lint !e534 �����Ƿ���ֵ
   
    /* ʱ���ʽ��2005-07-08 21:06:09.010+05:30 */
    return IVS_snprintf(buf, len, "%04d-%02d-%02d %02d:%02d:%02d.%03d%c%02d:%02d", 
        _tm.tm_year + 1900, _tm.tm_mon + 1, _tm.tm_mday,
        _tm.tm_hour, _tm.tm_min, _tm.tm_sec, tv.tv_usec/1000, g_TzFlag, g_TzHour, g_TzMin);//lint !e732
}

/**************************************************************************
* name       : PacketLog
* description: ��װ��־
* input      : 
* output     : NA
* return     : void
* remark     :               
 **************************************************************************/
static unsigned int PacketLog(char * szLogTmp, const char * stcMsgText, const char* szFile, long lLine, long lLevel)
{

    unsigned int pos = 0;
    //ʱ��ʱ��
    pos += FillTime(szLogTmp, LOG_LINE_LENGTH);
    //��־����
    const char* pszLevel = g_strLogLevel[lLevel];

    //�ļ���
    const char* pszFileName = NULL;
#ifdef _MSC_VER
    pszFileName = ::strrchr(szFile, '\\');
#else
    pszFileName = ::strrchr(szFile, '/');
#endif

    if(NULL != pszFileName)
    {
        //Խ��б��
        pszFileName += 1;
    }
    else
    {
        pszFileName = szFile;
    }

    //�߳�ID 
    unsigned long ulThreadID = THREAD_ID;

    //��ǰ������
    int ulErr = 0;
#ifdef _MSC_VER
    ulErr = (int)::GetLastError();
#else
    ulErr = errno;
#endif
    //������Ϣ׼�����

    //��ʽ����־
    pos += IVS_snprintf(szLogTmp + pos, LOG_LINE_LENGTH - pos, "[%7s|TID:0x%04X|RtnCode:0x%04X]    %s:%05d %s ", 
        pszLevel, (unsigned int)ulThreadID, (unsigned int)ulErr,pszFileName, (int)lLine,stcMsgText);

    return pos;
}

/*
 * IVS_LogRun
 * ��¼�ӿ���־
 * @level:     ��־����
 * @stcMsg:    ��̬��Ϣ
 * @file:      Դ�ļ���
 * @line:      Դ�ļ���
 * @func:      ������
 * @fmt:       ��ʽ��
 * ����ʱ��|����|�̺߳�|��־��̬��Ϣ|��־������Ϣ
 */
void IVS_LogRun(int level, const char* stcMsgText, const char *file, int line,
               const char *func, const char *fmt, ...)
{
    char buf[LOG_LINE_LENGTH + 1]; // no need init
    if (!g_bInitOk || (level > g_LogLevel && !g_bEnableDbg))
    {
        return;  // ����־����ͻ�Debug����δ�򿪣���ֱ�ӷ���
    }
    unsigned int pos = FillTime(buf, LOG_LINE_LENGTH);
    pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|%s|0x%08x|%s|",
            g_strLogLevel[level], (unsigned long)THREAD_ID, stcMsgText);   //lint !e737 �����߱�֤level�Ǹ�

    va_list  ap;
    va_start(ap, fmt);
    pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, ap); //lint !e737 IVS_vsnprintf ����ֵ�Ǹ�
    va_end(ap);

    buf[pos++] = '\n';
 
#ifdef LOG_HAS_STACK_TRACE
    if (level <= IVS_LOG_CRIT && pos < LOG_LINE_LENGTH)
    {
        pos += IVS_DumpBackTrace(buf + pos, LOG_LINE_LENGTH - pos);
    }
#endif

    if (level <= g_LogLevel)
    {
        LogFileWrite(&g_RunLog, buf, pos); //lint !e534 �����Է���ֵ
    }
    
    if (g_bEnableDbg)
    {
        pos = PacketLog(buf, stcMsgText, file, line, level);
        va_list  apDbg;
        va_start(apDbg, fmt);
        pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, apDbg); //lint !e737 IVS_snprintf���طǸ�
        va_end(apDbg);
        buf[pos++] = '\n';
        //pos--;  // earse '\n'     
        //pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|0x%08x|%s:%u|%s\n",
        //     (unsigned long)THREAD_ID, file, line, func);  //lint !e737 IVS_snprintf���طǸ�
        LogFileWrite(&g_DbgLog, buf, pos); //lint !e534 �����Է���ֵ
    }

    if (level <= IVS_LOG_CRIT)
    {
        /* ��־����ߣ�����ˢ�µ��ں� */    
        LogFileFlush(&g_RunLog);
    }
    return;
}

/*
 * IVS_LogDbg
 * ��¼������־
 * @level:     ��־����
 * @file:      Դ�ļ���
 * @line:      Դ�ļ���
 * @func:      ������
 * @fmt:       ��ʽ��
 */
void IVS_LogDbg(const char *file, int line, const char *func, const char *fmt, ...)
{
    if (!g_bInitOk || !g_bEnableDbg)
    {
        return ;
    }

    char buf[LOG_LINE_LENGTH + 1]; 
    unsigned int pos = 0;
    //pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|%s|", g_strLogLevel[IVS_LOG_DEBUG]); //lint !e737 IVS_snprintf���طǸ�

    pos = PacketLog(buf, "", file, line, IVS_LOG_DEBUG);
    va_list  ap;
    va_start(ap, fmt);
    pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, ap); //lint !e737 IVS_snprintf���طǸ�
    va_end(ap);
    buf[pos++] = '\n';

    //pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|0x%08x|%s:%u|%s\n", 
    //         (unsigned long)THREAD_ID, file, line, func); //lint !e737 IVS_snprintf���طǸ�
    LogFileWrite(&g_DbgLog, buf, pos); //lint !e534 �����Է���ֵ
    return ;
}


/*
 * IVS_LogInterface
 * ��¼�ӿ���־
 * @level:       ��־����
 * @inftype:     �ӿ�����
 * @infname:     �ӿ�����
 * @sender:      ����ģ�����
 * @receiver:    ���շ�ģ�����
 * @traceMark:   ���ٱ�ʶ
 * @operation:   ��������
 * @result:      �������
 * @file:        Դ�ļ���
 * @line:        Դ�ļ���
 * @func:        ������
 * @fmt:         ��ʽ��
 * ����ʱ��|�ӿ�����| �ӿ����� |��־���� | ����ģ����� | ���շ�ģ�����|���ٱ�ʶ|�������� | ������� |��־������Ϣ
 */
#ifdef LOG_HAS_ITF
void IVS_LogInterface(int level, char *inftype, char *infname, 
              char *sender, char *receiver, char *traceMark, char *operation, char *result, 
              const char *file, int line, const char *func, const char *fmt, ...)
{
    char buf[LOG_LINE_LENGTH + 1]; 
    if (!g_bInitOk || (level > g_LogLevel && !g_bEnableDbg))
    {
        return;
    }

    int pos = FillTime(buf, LOG_LINE_LENGTH);
    pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|%s|%s|%s|%s|%s|%s|%s|%s|",
              inftype, infname, g_strLogLevel[level], sender, receiver, traceMark, operation, result);
    va_list ap;
    va_start(ap, fmt);
    pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, ap);
    va_end(ap);
    buf[pos++] = '\n';

    if (level <= g_LogLevel)
    {
        LogFileWrite(&g_InterFaceLog, buf, pos);
    }

    if (g_bEnableDbg)
    {
        pos--;
        pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|0x%08x|%s:%u|%s\n", 
            (unsigned long)THREAD_ID, file, line, func);
        LogFileWrite(&g_DbgLog, buf, pos);
    }
    return ;
}
#endif

/*
 * IVS_LogUser
 * ��¼�û���־
 * @level:     ��־����
 * @user:      �û�ID
 * @operation: ����
 * @result:    �������
 * @file:      Դ�ļ���
 * @line:      Դ�ļ���
 * @func:      ������
 * @fmt:       ��ʽ��
 */
#ifdef LOG_HAS_USER
void IVS_LogUser(int level, char *user, char *operation, char *result, 
           const char *file, int line, const char *func, const char *fmt, ...)
{
    return ;
}
#endif

/*
 * IVS_LogSecurity
 * ��¼��ȫ��־
 * @level:      ��־����(DB)
 * @user:       �û��˺�(DB)
 * @ip:         �ͻ���IP(DB)
 * @stcMsgNum�� ��̬��Ϣ����(DB)
 * @stcMsgText: ��̬��Ϣ�ı�
 * @opcode:     ��������(DB)
 * @result:     �������(DB)
 * @file:       Դ�ļ���
 * @line:       Դ�ļ���
 * @func:       ������
 * @fmt:        ��ʽ��
 * ����ʱ�� | ��־���� | �û��˺� |�ͻ���IP |��̬��Ϣ| ������� |��־������Ϣ
 */
#ifdef LOG_HAS_SEC
void IVS_LogSecurity(int level, char *user, char *ip, char *stcMsgNum, char*stcMsgText, 
           char*opcode, char *result, const char *file, int line, const char *func, const char *fmt, ...)
{
    char buf[LOG_LINE_LENGTH + 1];  // no need init
    if (!g_bInitOk || (level > g_LogLevel && !g_bEnableDbg))
    {
        return;
    }

    int pos = FillTime(buf, LOG_LINE_LENGTH);
    pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|%s|%s|%s|%s|%s|%s|",
        g_strLogLevel[level], user, ip, stcMsgNum, stcMsgText, result);

    va_list ap;
    va_start(ap, fmt);
    pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, ap);
    va_end(ap);
    buf[pos++] = '\n';

    if (level <= g_LogLevel)
    {
        LogFileWrite(&g_UserLog, buf, pos);
    }

    if (g_bEnableDbg)
    {
        pos--;  
        pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|0x%08x|%s:%u|%s\n", 
            (unsigned long)THREAD_ID, file, line, func);
        LogFileWrite(&g_DbgLog, buf, pos);
    }

    /****/
     // TODO ���ݿ⵼�����

    /****/


    return;
}
#endif 

/*
 * IVS_LogOperation
 * ��¼������־
 * @level:      ��־���� (DB)
 * @user:       �û��˺� (DB)
 * @ip:         �ͻ���IP (DB)
 * @object      ��������  
 * @stcMsgNum�� ��̬��Ϣ���� (DB)
 * @stcMsgText: ��̬��Ϣ�ı�
 * @opcode:     �������� (DB)
 * @result:     ������� (DB)  successfully/failed   
 * @file:       Դ�ļ��� 
 * @line:       Դ�ļ���
 * @func:       ������
 * @fmt:        ��ʽ��
 * ��ʽ������ʱ�� | ��־���� | �û��˺� | �ͻ���IP | �������� |��̬��Ϣ | ������� |��־������Ϣ
 */
#ifdef LOG_HAS_OPR
void IVS_LogOperation(int level, char *user, char* ip, char *object, 
                     char *stcMsgNum, char *stcMsgText, char *opcode, char *result, 
                     const char *file, int line, const char *func, const char *fmt, ...)
{
    char buf[LOG_LINE_LENGTH + 1];
    if (!g_bInitOk || (level > g_LogLevel && !g_bEnableDbg))
    {
        return;
    }

    int pos = FillTime(buf, LOG_LINE_LENGTH);
    pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|%s|%s|%s|%s|%s|%s|%s|",
          g_strLogLevel[level], user, ip, object, stcMsgNum, stcMsgText, result);

    va_list ap;
    va_start(ap, fmt);
    pos += IVS_vsnprintf(buf + pos, LOG_LINE_LENGTH - pos, fmt, ap);
    va_end(ap);
    buf[pos++] = '\n';

    if (level <= g_LogLevel)
    {
        LogFileWrite(&g_OperationLog, buf, pos);
    }

    if (g_bEnableDbg)
    {
        pos--;
        pos += IVS_snprintf(buf + pos, LOG_LINE_LENGTH - pos, "|0x%08x|%s:%u|%s\n", 
            (unsigned long)THREAD_ID, file, line, func);
        LogFileWrite(&g_DbgLog, buf, pos);
    }

    /****/
    // TODO ���ݿ⵼�����

    /****/
    return;
}
#endif 

/*
 * IVS_LogAssert
 * ��¼������Ϣ�����жϳ���
 */
void IVS_LogAssert(const char *expr, const char *file, int line, const char *func)
{
    IVS_LogDbg(file, line, func, "Assert %s fail ", expr);
#if defined _MSC_VER || defined _WIN32
    DebugBreak();
#else
    abort();
#endif
}

