//lint -e1551
/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NSS_Log_Manager.cpp
  �� �� ��   : ����
  ��    ��   : qianjiuyun
  ��������   : 2010��11��27��
  ����޸�   :
  ��������   : ��־������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��11��27��
    ��    ��   : qianjiuyun
    �޸�����   : �����ļ�

******************************************************************************/
#include "ace_header.h"
#include "Rtsp_Log.h"
#include "Rtsp_Log_Manager.h"
#include "ace/Dirent.h"

const char* g_strRtspLogType[] = {"security", 
                                  "operation",
                                  "run", 
                                  "user",
                                  "debug", 
                                  NULL};

const char* g_strRtspLogLevel[] = {"EMR",
                                   "ALT",
                                   "CRI",
                                   "ERR",
                                   "WAR",
                                   "NTC",
                                   "INF",
                                   "DBG",
                                   NULL};

void nssPrintLog(const NSS_LOG_TYPE logType, const RTSP_LOG_LEVEL_E logLevel, 
                 const char * usr, const char* ip, const char* ret,
                 const char* flowID,const char* sessionID,
                 const char * file, const long line, const char * format, ...)
{
    if (NULL == format)
    {
        // Give a chance for the caller to address NULL input bug
        format = "NULL!!";
    }

    va_list args;
    va_start(args, format);
    CRtsp_Log_Manager::instance().printLog(logType, logLevel, 
                            usr, ip, ret, flowID, sessionID, 
                            file, line, format, args);
    va_end(args);
    return; //lint !e438
}

//==============================CNSS_Log BEGIN================================
CRtsp_Log::CRtsp_Log()
{
    // �ļ����
    m_nFd = ACE_INVALID_HANDLE;

    // ��־�ļ���С
    m_ulFileSize = 0;
    
    // ��־�ļ���С���ֵ�����ڸ�ֵʱ����Ҫ�л���־�ļ�
    m_ulFileMaxSize = NSS_LOG_SIZE_DEFAULT;

    // ����������־�ļ�������
    m_ulBackupFileMaxNum = NSS_LOG_NUM_DEFAULT;

    m_enLevel = RTSP_LOG_LEVEL_INFO;

    // ��־�ļ�����
    memset(m_strFileAllPath, 0, sizeof(m_strFileAllPath));
    memset(m_strFileName, 0, sizeof(m_strFileName));
    memset(m_strPrefix, 0, sizeof(m_strPrefix));

    // ��־������
    memset(m_strBuff, 0, sizeof(m_strBuff));
    
    // ��־�������Ѿ�ʹ�õĳ���
    m_ulBuffLen = 0;

    m_enLogType = LOG_TYPE_DBG;

    // ���д��־ʱ��
    m_ulLastWriteLogTime = 0;

    // ��ʷ�ļ���
    m_ulBackupLogFileCount = 0;
    
    // ��ʷ�ļ���
    ACE_Guard<ACE_Thread_Mutex> locker(m_mBackupLogLock);
    // ��ʷ�ļ��б�
    m_ltBackupLogFileList.clear();
}

CRtsp_Log::~CRtsp_Log()
{
    ACE_Guard<ACE_Thread_Mutex> locker(m_mBackupLogLock);
    try
    {
        // ��ʷ�ļ��б�
        m_ltBackupLogFileList.clear();
    }catch(...)
    {
        
    }
}//lint !e1740

void CRtsp_Log::init(const char * strPrefix, const char * strFileName, NSS_LOG_TYPE enLogType)
{
    (void)ACE_OS::snprintf(m_strFileName, sizeof(m_strFileName), 
                           "%s",
                           strFileName);
    
    m_enLogType = enLogType;

	(void)ACE_OS::snprintf(m_strPrefix, sizeof(m_strPrefix), 
		"%s",
		strPrefix);

    // ��־�ļ�ȫ·��
    (void)ACE_OS::snprintf(m_strFileAllPath, 
                           sizeof(m_strFileAllPath), 
                           "%s/%s/%s_%s%s",
                           CRtsp_Log_Manager::instance().logRootPath(),
                           g_strRtspLogType[enLogType],
                           strFileName,
                           m_strPrefix,
                           NSS_LOG_NAME_SUFFIX);

    //// ���ݾɵ���־�ļ�,��ʽ:����־��_YYYYMMDDHHMMSS.log"
    //ACE_stat stat;
    //if(0 == ACE_OS::stat(m_strFileAllPath, &stat))
    //{
    //    time_t currTime = time(NULL);
    //    struct tm tmv;
    //    (void)ACE_OS::localtime_r(&currTime, &tmv);
    //    char strNewFileName[NSS_LOG_FILE_PATH_LEN + 1] = {0,};
    //    (void)ACE_OS::snprintf(strNewFileName, 
    //                           sizeof(strNewFileName), 
    //                           "%s/%s/%s_%s_"
    //                           "%04d%02d%02d%02d%02d%02d"
    //                           "%s",
    //                           CRtsp_Log_Manager::instance().logRootPath(), g_strRtspLogType[enLogType],strFileName, m_strPrefix,
    //                           tmv.tm_year + 1900, tmv.tm_mon + 1,tmv.tm_mday, tmv.tm_hour,tmv.tm_min,tmv.tm_sec,
    //                           NSS_LOG_NAME_SUFFIX);

    //    // ��������־�ļ�
    //    (void)ACE_OS::rename(m_strFileAllPath, strNewFileName);
    //}

    char strTempFileAllPath[NSS_LOG_FILE_PATH_LEN + 1];
    (void)ACE_OS::snprintf(strTempFileAllPath, 
        sizeof(strTempFileAllPath), 
        "%s/%s",
        CRtsp_Log_Manager::instance().logRootPath(),
        g_strRtspLogType[enLogType]);

    int iRet = ACE_OS::access(strTempFileAllPath,F_OK);
    if (0 == iRet)
    {
        ACE_Dirent indexDir(strTempFileAllPath);

        for (dirent * directory = indexDir.read(); NULL != directory;  directory = indexDir.read())
        {
            std::string strIndexFileName = directory->d_name;

            std::string::size_type nPosBegin = strIndexFileName.rfind(m_strFileName);
            if (std::string::npos == nPosBegin)
            {
                continue;
            }

            std::string::size_type nPosEnd = strIndexFileName.rfind(NSS_LOG_NAME_SUFFIX);
            if ((std::string::npos == nPosEnd)
                ||((nPosBegin + strlen (m_strFileName)) == nPosEnd))
            {
                continue;
            }
            std::string strLogPath = strTempFileAllPath;
            strIndexFileName = strLogPath + "/" + strIndexFileName;
            ACE_Guard<ACE_Thread_Mutex> locker(m_mBackupLogLock);
            m_ltBackupLogFileList.push_back(strIndexFileName);
            ++m_ulBackupLogFileCount;
        }
    }
    
    return;
}

void CRtsp_Log::close()
{
    refreshLog();
}

void CRtsp_Log::closeFd()
{
    if (ACE_INVALID_HANDLE != m_nFd)
    {
        (void)ACE_OS::close(m_nFd);
        m_nFd = ACE_INVALID_HANDLE;
    }
}

void CRtsp_Log::printLog(const char* strTime, const char* strAddInfo,
                        const RTSP_LOG_LEVEL_E logLevel, const char * file, long line, 
                        const char* format, va_list args)
{
    // ���Ҫ��ӡ����־�ȵ�ǰ����־���õļ���ͣ�����־������
    if(logLevel > m_enLevel)
    {
        return;
    }
   
    char strSingleLog[NSS_SINGLE_LOG_LEN_MAX + 1] = {0,};
    uint32_t ulLogLen = 0;
    if(LOG_TYPE_DBG == m_enLogType)
    {
        
        // Get file title of the caller
#ifndef WIN32
        char *strFileTitle = strrchr(const_cast<char *>(file), '/'); 
#else
        char *strFileTitle = strrchr(const_cast<char *>(file), '\\'); 
#endif
        if (NULL == strFileTitle)
        {
            strFileTitle = const_cast<char *>(file);
        }
        else
        {
            strFileTitle++;
        }
    
        // ��������Ϣ
        char strErrno[NSS_LOG_TEMP_BUFF_LEN] = {0,};
        if(0 != errno)
        {
            (void)ACE_OS::snprintf(strErrno, NSS_LOG_TEMP_BUFF_LEN, "[%d:%s]", errno, strerror(errno));
        }
        
        ulLogLen = (uint32_t)ACE_OS::snprintf(strSingleLog, 
                                NSS_SINGLE_LOG_LEN_MAX, 
                                "%s [%s] [0x%04x] | (%s:%d%s) %s ",
                                strTime, 
                                g_strRtspLogLevel[logLevel],
                                (uint32_t)ACE_OS::thr_self(),
                                strFileTitle, 
                                (int)line,
                                strErrno, 
                                strAddInfo);
    }
    else
    {
        ulLogLen = (uint32_t)ACE_OS::snprintf(strSingleLog, NSS_SINGLE_LOG_LEN_MAX, 
                                "%s [%s] [0x%04x] | %s ",
                                strTime, 
                                g_strRtspLogLevel[logLevel],
                                (uint32_t)ACE_OS::thr_self(),
                                strAddInfo);
    }
    
    ulLogLen += (uint32_t)ACE_OS::vsnprintf(strSingleLog + ulLogLen,
                         NSS_SINGLE_LOG_LEN_MAX - ulLogLen,
                         format,
                         args);
    if (ulLogLen >= NSS_SINGLE_LOG_LEN_MAX)
    {
        ulLogLen = NSS_SINGLE_LOG_LEN_MAX - 1;
        strSingleLog[ulLogLen++] = '\n';
        strSingleLog[ulLogLen] = '\0';
    }
    else
    {
        if (strSingleLog[ulLogLen - 1] != '\n')
        {
            strSingleLog[ulLogLen] = '\n';
            strSingleLog[ulLogLen + 1] = '\0';
            ulLogLen++;
        }
    }

    //1 TODO ��־��������
    ACE_Guard<ACE_Thread_Mutex> locker(m_mLock);
    outputLog2File(strSingleLog, ulLogLen);

    return;
}

// ����������־����
void CRtsp_Log::resetConf(uint32_t ulBackupLogNum, uint32_t ulLogMaxSize,RTSP_LOG_LEVEL_E enLevel)
{
    m_ulBackupFileMaxNum = ulBackupLogNum;

    m_ulFileMaxSize = ulLogMaxSize;

    m_enLevel = enLevel;
}

int CRtsp_Log::openLogFile()
{
    // ������־�ļ�·��,�ݹ鴴��Ŀ¼
    char strTempDir[NSS_LOG_FILE_PATH_LEN + 1] = {0, };
    char * strLastPos = m_strFileAllPath;
    char* strNewPos = NULL;
    if(('/' != m_strFileAllPath[0])
        && ('.' != m_strFileAllPath[0]))
    {
        strNewPos = strchr(strLastPos, '/');
        if (NULL != strNewPos)
        {
            strLastPos = strNewPos + 1;
        }
    }
    while(NULL != (strNewPos = strchr(strLastPos, '/'))) //lint !e838
    {
        strNewPos++;
        strncpy(strTempDir, m_strFileAllPath, (uint32_t)(strNewPos - m_strFileAllPath));
        // �������Ŀ¼ʧ��ʱ������Ŀ¼�Ѿ����ڵĴ���
#ifndef WIN32
        if(0 != ACE_OS::mkdir(strTempDir, 0744))
#else
        if(0 != ACE_OS::mkdir(strTempDir))
#endif
        {
            if(EEXIST != errno )
            {
                return -1;
            }

            errno = 0;                
        }

        strLastPos = strNewPos;
    }

    // ������־�ļ�
    int nFlags = O_CREAT | O_WRONLY | O_APPEND;
    m_nFd = ACE_OS::open(m_strFileAllPath, nFlags);
    if (ACE_INVALID_HANDLE == m_nFd)
    {
        return -1;
    }

    m_ulFileSize = 0;
    ACE_stat  stat;
    if(0 == ACE_OS::fstat(m_nFd, &stat))
    {
        m_ulFileSize = (uint64_t)stat.st_size;//lint !e67 !e571
    }

    return 0;
}
// ��־�ļ����ݲ��á���־����nvs_ģ����_PID_YYYYMMDDHH24MISS.log����������ʽ��
void CRtsp_Log::backupLogFile()
{
    //char strTime[15] = {0,};
    struct tm tmv;
    time_t currTime = time(NULL);
    (void)ACE_OS::localtime_r( &currTime, &tmv );
    char strNewFilePath[NSS_LOG_FILE_PATH_LEN + 1] = {0, };

	(void)ACE_OS::snprintf(strNewFilePath, 
		sizeof(strNewFilePath), 
		"%s/%s/%s_%s"
		"%04d%02d%02d%02d%02d%02d"
		"%s",
		CRtsp_Log_Manager::instance().logRootPath(), g_strRtspLogType[m_enLogType],m_strFileName, m_strPrefix,
		tmv.tm_year + 1900, tmv.tm_mon + 1,tmv.tm_mday, tmv.tm_hour,tmv.tm_min,tmv.tm_sec,
		NSS_LOG_NAME_SUFFIX);

    // �ر���־�ļ�
    if(ACE_INVALID_HANDLE != m_nFd)
    {
        ACE_OS::close(m_nFd);
        m_nFd = ACE_INVALID_HANDLE;
    }
        
    (void)ACE_OS::rename(m_strFileAllPath, strNewFilePath);

    // ��ʷ�ļ�������һ��
    ACE_Guard<ACE_Thread_Mutex> locker(m_mBackupLogLock);
    m_ltBackupLogFileList.push_back(strNewFilePath);
    ++m_ulBackupLogFileCount;

    return;
}

void CRtsp_Log::outputLog2File()
{
    if(0 == m_ulBuffLen)
    {
        return;
    }

	ACE_stat  stat;
	if(0 != ACE_OS::fstat(m_nFd, &stat))
	{
		m_ulFileSize = (uint64_t)stat.st_size;//lint !e67 !e571
	}
    // ����ļ���С�Ѿ��ﵽ���ֵ�����л��ļ�
    if(m_ulFileSize >= m_ulFileMaxSize)
    {
        backupLogFile();
    }

    // ����ļ������Ч�������־�ļ�
    if(ACE_INVALID_HANDLE == m_nFd)
    {
        if(0 != openLogFile())
        {
            return;
        }
    }

    // �������������־����־�ļ���
    int nWriteRet = ACE_OS::write(m_nFd, m_strBuff, m_ulBuffLen);

    // ���дʧ���ˣ���ر���־�ļ����´���д
    if(nWriteRet != (int)m_ulBuffLen)
    {
        printf("\n ======error[%d]====write failed.nWriteRet=[%d] , m_ulBuffLen[%u]\n", errno,nWriteRet, m_ulBuffLen);
        (void)ACE_OS::close(m_nFd);
        m_nFd = ACE_INVALID_HANDLE;
        return;
    }


    // д�ɹ����޸��ļ���С�ͻ�������С
    (void)ACE_OS::fsync(m_nFd);
    m_ulFileSize += m_ulBuffLen;

    m_ulBuffLen = 0;
    return ;
}

void CRtsp_Log::outputLog2File(const char* strLog, uint32_t ulLogLen)
{
    // ����ļ���С�Ѿ��ﵽ���ֵ�����л��ļ�
    if(m_ulFileSize >= m_ulFileMaxSize)
    {
        backupLogFile();
    }

    // ����ļ������Ч�������־�ļ�
    if(ACE_INVALID_HANDLE == m_nFd)
    {
        if(0 != openLogFile())
        {
            return;
        }
    }

    // �������������־����־�ļ���
    int nWriteRet = ACE_OS::write(m_nFd, strLog, ulLogLen);

    // ���дʧ���ˣ���ر���־�ļ����´���д
    if(nWriteRet != (int)ulLogLen)
    {
        printf(
            "\n ======error[%d]====write failed.nWriteRet=[%d] , ulLogLen[%u]\n", 
            errno,nWriteRet, ulLogLen);
        (void)ACE_OS::close(m_nFd);
        m_nFd = ACE_INVALID_HANDLE;
        
        return;
    }

    m_ulFileSize += ulLogLen;

    return ;
}


// ��鵱ǰ����д����־�ļ��Ƿ����
void CRtsp_Log::statLogFile()
{
    ACE_Guard<ACE_Thread_Mutex> locker(m_mLock);
    // �����ǰд����־�ļ���������ã����ü���ļ��Ƿ����
    if(ACE_INVALID_HANDLE == m_nFd)
    {
        return;
    }

    // ���stat�ɹ������ô���
    ACE_stat stat;
    if(0 == ACE_OS::stat(m_strFileAllPath, &stat))
    {
        return;
    }

    // ��ȡ��־�ļ�״̬ʧ�ܣ���ر���־�ļ�������д��־���̴�����־�ļ�
    (void)ACE_OS::close(m_nFd);
    m_nFd = ACE_INVALID_HANDLE;
    m_ulFileSize = 0;
}

// ˢ�»������־���ݵ���־�ļ�
void CRtsp_Log::refreshLog()
{
    //1 TODO ��־������,����ˢ����
    if(ACE_INVALID_HANDLE != m_nFd)
    {
        ACE_Guard<ACE_Thread_Mutex> locker(m_mLock);
        (void)ACE_OS::fsync(m_nFd);
    }

    return;
}

void CRtsp_Log::checkHistoryLogCount()
{
    ACE_Guard<ACE_Thread_Mutex> locker(m_mBackupLogLock);
    // �����ʷ�ļ��������������õ����ֵ����ɾ���������ʷ�ļ�����ʹ��ʷ�ļ������ñ仯
    while(m_ulBackupFileMaxNum <= m_ulBackupLogFileCount)
    {
        m_ltBackupLogFileList.sort();
        string strFileName = m_ltBackupLogFileList.front();
        m_ltBackupLogFileList.pop_front();
        (void)ACE_OS::unlink(strFileName.c_str());
        --m_ulBackupLogFileCount;
    }
}

//==============================CNSS_Log END==================================

//===========================CNSS_Log_Manager BEGIN===========================
CRtsp_Log_Manager::CRtsp_Log_Manager()
{
    m_bRunFlag = false;
    memset(m_strLogRootPath, 0 , sizeof(m_strLogRootPath));
    memset(m_strConfigPath, 0 , sizeof(m_strConfigPath));
    memset(m_strServerName, 0 , sizeof(m_strServerName));
    m_ulCheckInterval = NSS_LOG_CHECK_TIME_DEFAULT;

    // ��־�ļ���С���ֵ�����ڸ�ֵʱ����Ҫ�л���־�ļ�
    m_ulWholeFileMaxSize = NSS_LOG_SIZE_DEFAULT;

    // ����������־�ļ�������
    m_ulWholeBackupFileNum = NSS_LOG_NUM_DEFAULT;

    // ��־����
    m_enWholeLevel = RTSP_LOG_LEVEL_INFO;
    m_RefreshLevel = RTSP_LOG_LEVEL_WARNING;
    m_bDebugFlag   = false;

    m_bDebugWitch = false;
}

CRtsp_Log_Manager::~CRtsp_Log_Manager()
{
    
}

int CRtsp_Log_Manager::initLog(const char * strServerName,
                               const char * strLogRootPath,
                               unsigned int unLevel)
{
    // ��������ж�
    if ((NULL == strServerName) || (NULL == strLogRootPath))
    {
        printf("Init log failed. The parameter is invalid.\n");
        return -1;
    }
    
    (void)ACE_OS::snprintf(m_strServerName, sizeof(m_strServerName), "%s", strServerName);
    if ('\0' == m_strLogRootPath[0])
    {
		//modify by limingjie lWX175483 :2013-5-24
		//strcpy �ĳ� strncpy
        strncpy(m_strLogRootPath, strLogRootPath, strlen(strLogRootPath)+1);
    }

    uint32_t ulLen = strlen(m_strLogRootPath);
    if ((0 != strcmp("/", m_strLogRootPath))
        && (0 != strcmp("./", m_strLogRootPath))
        && (ulLen > 1)
        && ('/' == m_strLogRootPath[ulLen - 1]))
    {
        m_strLogRootPath[ulLen - 1] = '\0';
    }
    // ������־
    if (RTSP_LOG_LEVEL_DEBUG < unLevel)
    {
        unLevel = RTSP_LOG_LEVEL_WARNING;
    }
    CRtsp_Log_Manager::instance().m_RefreshLevel = (RTSP_LOG_LEVEL_E) unLevel;

    // ��ʼ��������־ģ��
    m_securityLog.init("security", strServerName, LOG_TYPE_SEC);
    m_operationLog.init("operation", strServerName, LOG_TYPE_OPR);
    m_runLog.init("run", strServerName, LOG_TYPE_RUN);
    m_userLog.init("user", strServerName, LOG_TYPE_USR);
    m_debugLog.init("debug", strServerName, LOG_TYPE_DBG);
    m_bDebugWitch = m_bDebugFlag;

    // ��ʼ����־����
    refreshLogConf();  

    // ��־�����������
    m_ulCheckInterval = NSS_LOG_CHECK_TIME_DEFAULT;

    if (0 != this->open(0))
    {
        printf("\n\r=======[%s] failed to start.Create log thread failed.========\n",
               strServerName);
        
        return -1;
    }

    // ��ʼ����ɺ󣬴�ӡһ��������������־
    IVS_RUN_LOG_EMR("\n\r==========[%s]System start running=========,ProcessID[%d]", 
                    strServerName, 
                    (int)ACE_OS::getpid());

    return 0;
}

int CRtsp_Log_Manager::endLog()
{
    m_bRunFlag = false;
    //ǿ�ư�������־ˢ�µ���־�ļ���
    m_securityLog.close();
    m_securityLog.closeFd();
    m_operationLog.close();
    m_operationLog.closeFd();
    m_runLog.close();
    m_runLog.closeFd();
    m_userLog.close();
    m_userLog.closeFd();
    m_debugLog.close();
    m_debugLog.closeFd();
    return this->close(0);
}

int CRtsp_Log_Manager::open(void *)
{
    m_bRunFlag = true;

    size_t stack_size[1] = {DEFAULT_LOG_STACK_SIZE};
    // ����ý�崦���߳�
    int nRet = activate(THR_NEW_LWP | THR_JOINABLE,                      //lint !e835
                        1, // �߳���
                        0,
                        ACE_DEFAULT_THREAD_PRIORITY,
                        -1,
                        0,
                        0,
                        0,
                        stack_size, // �̶߳�ջ��С
                        0 );
    if (-1 == nRet)
    {
        return -1;
    }
    
    return 0;    
}

int CRtsp_Log_Manager::svc(void)
{  
    //time_t lastChangeTime = 0;
    
    // �ȼ���ļ�����sleep����֤��һ������
    while(m_bRunFlag)
    {
        // ÿ��һ��������ڼ��һ����־����
        for (uint32_t i = 0; i < m_ulCheckInterval; i++)
        {
            // Ϊ��ֹsleepʱ����������ܼ�ʱ��Ӧ�˳���Ϣ������ֻsleep 1��
            ACE_OS::sleep(ACE_Time_Value((time_t)1, 0));

            // ˢ����־�ļ�����ֹ̫��ʱ��û��д��־
            m_securityLog.refreshLog();

            m_operationLog.refreshLog();

            m_runLog.refreshLog();

            m_userLog.refreshLog();

            m_debugLog.refreshLog();

            if (!m_bRunFlag)
            {
                return 0;
            }
        }

        // ��鵱ǰд����־�ļ��Ƿ���ڣ���������ڣ���ر���־�ļ����������д��־���̴�����־�ļ�
        statLogFile();
        
        // ���ÿ����־�ļ��������Ƿ񳬹������������������������ɾ�������
        checkHistoryLogFile();
        refreshLogConf();
        m_bDebugWitch = m_bDebugFlag;
    }
        
    
    return 0;
}

int CRtsp_Log_Manager::close(unsigned long)
{
    //(void)wait();
    this->thr_mgr_ = 0;
    
    return 0;
}

// ��ӡ��־�����debug���ش��ˣ����ӡ���������־��ͬʱ����ӡ������־��debug�ļ���
void CRtsp_Log_Manager::printLog(const NSS_LOG_TYPE logType, const RTSP_LOG_LEVEL_E logLevel, 
                const char * usr, const char* ip, const char* ret,
                const char * flowID, const char* sessionID,
                const char * file, long line, const char* format, va_list args)
{
    // δ��ʼ��ʱ����ӡǰ̨��־
    if(!m_bRunFlag)
    {
        return;
    }
    // 11-25 11:42:13.786404
    ACE_Time_Value uSec = ACE_OS::gettimeofday();
    struct tm tmv;
    time_t currTime = time(NULL);
    (void)ACE_OS::localtime_r( &currTime, &tmv );
    char strTime[64];
    (void)ACE_OS::snprintf(strTime, sizeof(strTime), 
                "%04d-%02d-%02d %02d:%02d:%02d.%06d", 
                tmv.tm_year + 1900, tmv.tm_mon + 1, tmv.tm_mday, tmv.tm_hour, tmv.tm_min, tmv.tm_sec,
                (int)uSec.usec());

    // ��������Ϣ�ĸ�ʽƴ��
    char strAddInfo[NSS_LOG_TEMP_BUFF_LEN+ 1] = {0,};
    // ��־���ദ��
    switch(logType)
    {
        case LOG_TYPE_SEC:
            (void)ACE_OS::snprintf(strAddInfo, sizeof(strAddInfo), "%s %s |",usr, ret);
            m_securityLog.printLog(strTime, strAddInfo,logLevel,file,line, format,args);
            break;
        
        case LOG_TYPE_OPR:
            (void)ACE_OS::snprintf(strAddInfo, sizeof(strAddInfo), "%s %s %s |",usr, ip, ret);
            m_operationLog.printLog(strTime,strAddInfo,logLevel,file,line, format,args);
            break;

        case LOG_TYPE_RUN:
            m_runLog.printLog(strTime,strAddInfo,logLevel,file,line, format,args);
            break;
            
        case LOG_TYPE_USR:
            (void)ACE_OS::snprintf(strAddInfo, sizeof(strAddInfo), "%s |[%s] [%s]",usr, flowID, sessionID);
            m_userLog.printLog(strTime,strAddInfo,logLevel,file,line, format,args);
            break;

        case LOG_TYPE_DBG:
            break;
            
        default:
            return;
    }

    // ���debug�������־�����ˣ����ӡdebug�������־��
    if(m_bDebugWitch)
    {
        m_debugLog.printLog(strTime,strAddInfo, logLevel, file, line, format,args);
    }

    errno = 0;
    return;
}

// ��ȡ��־���ã�����enLogType��ȡ��Ӧ��������
int CRtsp_Log_Manager::readLogConf(NSS_LOG_TYPE enLogType, 
                                       uint32_t& ulFileMaxSize,
                                       uint32_t& ulBackupFileNum,
                                       RTSP_LOG_LEVEL_E& enLevel)
{
    ulFileMaxSize = m_ulWholeFileMaxSize;        
    ulBackupFileNum = m_ulWholeBackupFileNum;
 
    // ��ȡ��־��������,������־ֻ�ǿ��أ�û�м���Ĭ���ǹرյ�
    if(LOG_TYPE_DBG == enLogType)
    {
        // Ĭ�Ϲر�
        enLevel = RTSP_LOG_LEVEL_EMERGENCY;
        m_bDebugWitch = false;
    }
    else
    {
        enLevel = m_enWholeLevel;
    }

    return 0;
}

const char* CRtsp_Log_Manager::logRootPath()const
{
    return m_strLogRootPath;
}

// ˢ����־������Ϣ:��־���𡢱�����־�ļ�������־�����ļ��Ĵ�С
void CRtsp_Log_Manager::refreshLogConf()
{
    // ��ȡ���������
    // ��־�ļ���С���ֵ�����ڸ�ֵʱ����Ҫ�л���־�ļ�
    uint32_t ulFileMaxSize = NSS_LOG_SIZE_DEFAULT;

    // ����������־�ļ�������
    uint32_t ulBackupFileNum = NSS_LOG_NUM_DEFAULT;

    // ��־����
    RTSP_LOG_LEVEL_E enLevel = m_RefreshLevel;
    m_securityLog.resetConf(ulBackupFileNum, ulFileMaxSize, enLevel);
    m_operationLog.resetConf(ulBackupFileNum, ulFileMaxSize, enLevel);
    m_runLog.resetConf(ulBackupFileNum, ulFileMaxSize, enLevel);
    m_userLog.resetConf(ulBackupFileNum, ulFileMaxSize, enLevel);
    m_debugLog.resetConf(ulBackupFileNum, ulFileMaxSize, enLevel);    

    return;
}

// ��鵱ǰ����д����־�ļ��Ƿ����
void CRtsp_Log_Manager::statLogFile()
{
    m_securityLog.statLogFile();

    m_operationLog.statLogFile();

    m_runLog.statLogFile();
    
    m_userLog.statLogFile();

    m_debugLog.statLogFile();

    return ;
}

void CRtsp_Log_Manager::checkHistoryLogFile()
{
    m_securityLog.checkHistoryLogCount();

    m_operationLog.checkHistoryLogCount();

    m_runLog.checkHistoryLogCount();
    
    m_userLog.checkHistoryLogCount();

    m_debugLog.checkHistoryLogCount();

    return ;
}

//===========================CNSS_Log_Manager END=============================
//lint +e1551


