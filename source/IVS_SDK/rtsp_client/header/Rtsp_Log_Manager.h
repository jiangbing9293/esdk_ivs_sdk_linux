/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NSS_Log_Manager.h
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
#ifndef __RTSP_LOG_MANAGER_H__
#define  __RTSP_LOG_MANAGER_H__

#include <string>
#include <list>

#ifdef WIN32
typedef unsigned int uint32_t;
typedef _ULonglong uint64_t;
#endif

#include "ace_header.h"
#include "Rtsp_Log.h"

using namespace std;

#define NSS_LOG_RET_SUC   "SUCCESS"
#define NSS_LOG_RET_FAI   "FAILURE"

typedef enum
{
    LOG_DEBUG_OPENED = 0,
    LOG_DEBUG_CLOSED = 1
}LOG_DEBUG_SWITCH;

// ������־����󳤶�
#define NSS_SINGLE_LOG_LEN_MAX     1024
#define NSS_LOG_HEADER_LEN_MAX     512

// ��־��������С,��λ:B
#define NSS_LOG_BUFF_SIZE      (1024 * 1024)

// ��ʱˢ����־ʱ��,��λ:us
#define NSS_WRITE_LOG_TIME     (100 * 1000)

// ��־�ļ�·����󳤶�
#define NSS_LOG_FILE_PATH_LEN       (1024)

#define NSS_LOG_SERVER_NAME_PATH 128

// �����̶߳�ջ��С
#define DEFAULT_LOG_STACK_SIZE   (128 * 1024)

// ������־�ļ���С,��λ:B
#define NSS_LOG_SIZE_MIN        (10 * 1024 *1024)
#define NSS_LOG_SIZE_MAX        (1024 * 1024 *1024)
#define NSS_LOG_SIZE_DEFAULT    (10 * 1024 *1024)

// ��ʷ��־�ļ���
#define NSS_LOG_NUM_MIN         1
#define NSS_LOG_NUM_MAX         1024
#define NSS_LOG_NUM_DEFAULT     5

// ��־�������,��λ:��
#define NSS_LOG_CHECK_TIME_MIN      (5)
#define NSS_LOG_CHECK_TIME_MAX      (10*60)
#define NSS_LOG_CHECK_TIME_DEFAULT  (1*60)

// ��־�ļ���׺
#define NSS_LOG_NAME_SUFFIX   ".log"

#define NSS_LOG_TEMP_BUFF_LEN  1024
//==============================CNSS_Log BEGIN================================
// ��־����.������־�������־����־�ļ��������ļ���С�л���־�ļ�
class CRtsp_Log
{
public:
    CRtsp_Log();
    virtual ~CRtsp_Log();

    // ��ʼ����־
    void init(const char * strPrefix, const char * strFileName, NSS_LOG_TYPE enLogType);

    // �ر���־ģ��
    void close();

    // ����������־ģ������
    void resetConf(uint32_t ulBackupLogNum, uint32_t ulLogMaxSize,RTSP_LOG_LEVEL_E enLevel);

    // ˢ�»������־���ݵ���־�ļ�
    void refreshLog();

    // �����ʷ��־�ļ�
    void checkHistoryLogCount();

    // ��ӡ��־
    void printLog(const char* strTime, const char* strAddInfo,
                  const RTSP_LOG_LEVEL_E logLevel, const char * file, long line, 
                  const char* format, va_list args);

    // ��ȡ��ǰ��־�ļ���Ϣ���жϵ�ǰ��־�ļ��Ƿ����
    void statLogFile();

    void closeFd();
private:
    // ����־�ļ�
    int openLogFile();

    // �����־����־�ļ�
    void outputLog2File();

    // �����־����־�ļ���������ķ�ʽ��ֱ�����������־
    void outputLog2File(const char * strLog, uint32_t ulLogLen);

    // ������ʷ��־�ļ�
    void backupLogFile();

private:
    // �ļ����
    ACE_HANDLE   m_nFd;

    // ��ǰ��־�ļ���С
    uint64_t m_ulFileSize;

    // ��־�ļ���С���ֵ�����ڸ�ֵʱ����Ҫ�л���־�ļ�
    uint32_t m_ulFileMaxSize;

    // ����������־�ļ�������
    uint32_t m_ulBackupFileMaxNum;

    // ��־����
    RTSP_LOG_LEVEL_E m_enLevel;
    
    // ��־�ļ�ȫ·��
    char m_strFileAllPath[NSS_LOG_FILE_PATH_LEN + 1];

    // ��־�ļ�����
    char m_strFileName[NSS_LOG_FILE_PATH_LEN + 1];
    
	// ��־��ʾ
	char m_strPrefix[NSS_LOG_FILE_PATH_LEN + 1];

    // ��־������
    char m_strBuff[NSS_LOG_BUFF_SIZE];
    
    // ��־�������Ѿ�ʹ�õĳ���
    uint32_t m_ulBuffLen;
    
    // ��־�ļ���
    ACE_Thread_Mutex m_mLock;

    // ��־��������
    NSS_LOG_TYPE m_enLogType;

    // ���д��־ʱ��
    volatile time_t m_ulLastWriteLogTime;

    // ��ʷ�ļ���
    uint32_t m_ulBackupLogFileCount;
    
    // ��ʷ�ļ��б�
    std::list<string> m_ltBackupLogFileList;
    
    // ��ʷ�ļ���
    ACE_Thread_Mutex m_mBackupLogLock;
};
//==============================CNSS_Log END==================================

//===========================CNSS_Log_Manager BEGIN===========================
class CRtsp_Log_Manager: public ACE_Task<ACE_MT_SYNCH>
{
public:
    virtual ~CRtsp_Log_Manager();

    // ����
    static CRtsp_Log_Manager & instance()
    {
        static CRtsp_Log_Manager logManager;
        return logManager;
    };

    // ��ʼ����־ģ��
    int initLog(const char *strServerName,
                const char* strLogRootPath,
                unsigned int unLevel);

    int endLog();
    
    void printLog(const NSS_LOG_TYPE logType, const RTSP_LOG_LEVEL_E logLevel, 
                const char * usr, const char* ip, const char* ret,
                const char * flowID, const char* sessionID,
                const char * file, long line, const char* format, va_list args);

public:// Ϊ�˼̳����ԣ���㲻Ҫ����
    // �����̴߳���
    int open(void* );

    // ��־�����߳�
    int svc(void);

    int close(unsigned long);    


public:// ������������
    const char* logRootPath()const;

    // ��ȡ�����ļ��е���־������Ϣ
    int readLogConf(NSS_LOG_TYPE enLogType,
                          uint32_t & ulFileMaxSize,
                          uint32_t & ulBackupFileNum, 
                          RTSP_LOG_LEVEL_E &enLevel);
    // ��ȡdebug�Ŀ���
    bool DebugOpenedFlag()const
    {
        return m_bDebugWitch;
    }

private:
    CRtsp_Log_Manager();
    // ˢ����־����
    void refreshLogConf();

    // ��鱸���ļ���
    void checkHistoryLogFile();

    // ��鵱ǰ����д����־�ļ��Ƿ����
    void statLogFile();

public:
    RTSP_LOG_LEVEL_E   m_RefreshLevel;
    bool            m_bDebugFlag;
    char m_strLogRootPath[NSS_LOG_FILE_PATH_LEN + 1];

private:
    // ������־����
    CRtsp_Log m_debugLog;

    // ������־����
    CRtsp_Log m_operationLog;

    // ������־����
    CRtsp_Log m_runLog;

    // ��ȫ��־����
    CRtsp_Log m_securityLog;

    // �û���־����
    CRtsp_Log m_userLog; 

    // �����־����
    uint32_t m_ulCheckInterval;

    // ��ʼ����ʶ
    bool m_bRunFlag;

    char m_strConfigPath[NSS_LOG_FILE_PATH_LEN + 1];

    char m_strServerName[NSS_LOG_SERVER_NAME_PATH + 1];

    // ��־�ļ���С���ֵ�����ڸ�ֵʱ����Ҫ�л���־�ļ�
    uint32_t m_ulWholeFileMaxSize;

    // ����������־�ļ�������
    uint32_t m_ulWholeBackupFileNum;

    // ��־����
    RTSP_LOG_LEVEL_E m_enWholeLevel;

    // debug����. Ĭ�Ϲر�:false
    bool m_bDebugWitch;
};// CNSS_Log_Manager
//===========================CNSS_Log_Manager END=============================

#endif // __RTSP_LOG_MANAGER_H__


