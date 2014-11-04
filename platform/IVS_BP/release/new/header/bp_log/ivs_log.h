#ifndef  __IVS_LOG_H__
#define  __IVS_LOG_H__

#include "bp_logoutput.h"

//#ifdef WIN32
//# ifdef BP_LOG_EXPORTS
//#  define EXPORT_BP_LOG_API __declspec(dllexport)
//# else
//#  define EXPORT_BP_LOG_API __declspec(dllimport)
//# endif
//#else
//#define EXPORT_BP_LOG_API
//#endif

enum LOG_LEVEL 
{
    IVSLOG_EMERG   = 0,   /* ������ϵͳ������ */
    IVSLOG_ALERT   = 1,   /* ����������ȡ��ʩ */
    IVSLOG_CRIT    = 2,   /* �������         */
    IVSLOG_ERR     = 3,   /* �������         */
    IVSLOG_WARNING = 4,   /* �������         */
    IVSLOG_NOTICE  = 5,   /* һ����Ҫ���     */
    IVSLOG_INFO    = 6,   /* ��ͨ��Ϣ         */
    IVSLOG_DEBUG   = 7,   /* ������Ϣ         */
};

enum LOG_TYPE
{
    LOG_TYPE_MIN,
    TYPE_SEC = LOG_TYPE_MIN, /* ��ȫ��־ */
    TYPE_OPR,                /* ������־ */
    TYPE_RUN,                /* ������־ */
    TYPE_ITF,                /* �ӿ���־ */
    //TYPE_USR,   /* �û���־ */  //ȥ���û���־
    TYPE_DBG,                /* ������־ */

    //����֮ǰ���ֵ
    LOG_TYPE_MAX
};


/* ʱ�����ñ�־ */
enum  {
    UTC_FLG_PLUS = 0,   /* UTC + */
    UTC_FLG_NEG  = 1    /* UTC - */
};


#pragma pack(push, 1)

struct BP_LOG_INFO
{
    unsigned long Size;   //��־��С
    unsigned int  Number; //��־����
    int Level;            //��־����
};

enum SFTP_INFO_LENGTH
{
    SFTP_WORKSPACE_LENGTH   = 768, 
    SFTP_SERVER_ADDR_LENGTH = 64, 
    SFTP_USER_NAME_LENGTH   = 64, 
    SFTP_PASSWORD_LENGTH    = 64
};

struct BP_LOG_SFTP_INFO
{
    char szWorkSpace[SFTP_WORKSPACE_LENGTH];    // �����ļ�һ��Ŀ¼
    char szRemoteAddr[SFTP_SERVER_ADDR_LENGTH]; // sftp��������ַ
    unsigned short usPort;                      // sftp�������˿�
    char szUsername[SFTP_USER_NAME_LENGTH];     // sftp�����û���
    char szPassword[SFTP_PASSWORD_LENGTH];      // sftp��������
};

#define MODULE_TYPE_LENGTH 33
struct BP_LOG_CONF
{
    char Path[768];
    char ModuleType[MODULE_TYPE_LENGTH];
    char NodeCode[MODULE_TYPE_LENGTH];
    struct BP_LOG_INFO LogInfo[LOG_TYPE_MAX];
    bool NeedOperationLog;      // �Ƿ���Ҫд������־
    bool NeedSecurityLog;       // �Ƿ���Ҫд��ȫ��־
    bool BackupToDebugLog;      // �Ƿ񽫱����־���ݱ��ݵ�Debug��־�ļ���.
    bool BackupOperationLog;    // �Ƿ����������.
    bool BackupSecurityLog;     // �Ƿ����������.
    bool LogBufferEnable;       // д��־��ʽ�ǻ��滹��ֱ��д��trueΪͨ�����棩��ֻ��run��dbg��־��Ч
    bool IsBufferDebugLog;      // �Ƿ񻺴�debug��־������������ؼ������ϴ���ʱ����ӡ��ǰ������debug�������־
    int BufferNumber;           // ����debug��־����
    struct BP_LOG_SFTP_INFO SftpInfo;
};

#pragma pack(pop)


/* ����������� */
#define LOG_HAS_SFTP_UPLOAD       /* ��־sftp�ϴ����� ע��ȡ���ù��� */
#define LOG_HAS_CONFIG_FILE       /* ��־���������ļ� ע��ȡ���ù��� */
#define LOG_HAS_WRITE_THREAD      /* ʹ�ö����߳�д�� ע��ȡ���ù��� */
#define LOG_HAS_STACK_TRACE       /* �������ص���־ͬʱ��ӡ����ջ��ע��ȡ���ù��� */

#if defined LOG_HAS_SFTP_UPLOAD || (!defined LOG_HAS_SFTP_UPLOAD && !defined LOG_HAS_WRITE_THREAD)
#define LOG_HAS_MONITOR_THREAD   /* ����߳� */
#endif 

#define LOG_HAS_SEC               /* ʹ�ܰ�ȫ��־ ע��ȥʹ�� */ 
#define LOG_HAS_OPR               /* ʹ�ܲ�����־ ע��ȥʹ�� */
#define LOG_HAS_ITF               /* ʹ�ܽӿ���־ ע��ȥʹ�� */
//#define LOG_HAS_USER              /* ʹ���û���־ ע��ȥʹ�� */

#define FILE_COUNT_LOWER_BOUND         ( 5)  /* ������־�ļ���С�������� (�������ļ��ֱ�ͳ��) */
#define FILE_COUNT_UPPER_BOUND         (10)  /* ������־�ļ���󱣴����� (�������ļ��ֱ�ͳ��) */

#define FILE_SIZE_LOWER_BOUND          (10 * 1024 * 1024) /* ��־�ļ���С���� (��λΪM) */
#define FILE_SIZE_UPPER_BOUND          (30 * 1024 * 1024) /* ��־�ļ���С���� (��λΪM) */

/* �쳣�ص����� */
#define LOG_ERR_DISKFULL       500   /* ������         */
//#define LOG_ERR_DISKRESTORE    501   /* ���ָ̻�       */
#define LOG_ERR_OPENFAIL       502   /* ���ļ�ʧ��    */
#define LOG_ERR_DBIMPORT       503   /* �������ݿ����   */
#define LOG_ERR_RENAMEFAIL     504   /* �������ļ�ʧ��  */
#define LOG_ERR_ZIPFAIL        505   /* ѹ���ļ�ʧ��    */
#define LOG_ERR_SFTPFAIL       506   /* ��־�ϴ�ʧ��    */

/* �쳣�ص�����---�ָ� */
#define LOG_RESUME_DISKFULL       550   /* �������ָ�         */
//#define LOG_RESUME_DISKRESTORE    551   /* ���ָ̻��ָ�       */
#define LOG_RESUME_OPENFAIL       552   /* ���ļ�ʧ�ָܻ�   */
#define LOG_RESUME_DBIMPORT       553   /* �������ݿ����ָ� */
#define LOG_RESUME_RENAMEFAIL     554   /* �������ļ�ʧ�ָܻ� */
#define LOG_RESUME_ZIPFAIL        555   /* ѹ���ļ�ʧ�ָܻ�   */
#define LOG_RESUME_SFTPFAIL       556   /* ��־�ϴ�ʧ�ָܻ�   */

#define LOG_LINE_LENGTH              2048            /* ��־�г���   */
#define LOG_ADDITIONALINFO_LENGTH    1024            /* ������Ϣ���� */

/* ������־���ؽ�� */
#define LOG_RET_STR_OK ("successful")
#define LOG_RET_STR_FAIL ("failed")
/* --------------------------------------------------------------------------- */

/* �ӿں���
 * IVS_logInit
 * @szDir:        ��־����Ŀ¼, �����Ѵ���
 * @szConf:       �����ļ�·��, ʹ�������ļ������
 * @szModuleType: ģ������
 * @uModuleID:    ģ��ID
 * @callback:     �쳣�ص�����
 */
//extern "C" int EXPORT_BP_LOG_API IVS_LogInit(const char *szDir, char *szConf, char *szModuleType, unsigned int uModuleCode, void (*callback)(int code));
extern "C" int EXPORT_BP_LOG_API IVS_LogInit(struct BP_LOG_CONF& log_conf, bool (*callback)(int, const char *));
extern "C" void EXPORT_BP_LOG_API IVS_LogClean();

/* �ӿں���
 * IVS_SetLogConf
 * @logType:        ��־���֧�ְ�ȫ��־(TYPE_SEC)��������־(TYPE_OPR)��������־(TYPE_RUN)
 * @logLevel:       ��־����
 * @szBeginTime:    ������Чʱ�䣬UTCʱ�䣬��ʽΪyyyymmddHHMMss
 * @szEndTime:      ����ʧЧʱ�䣬UTCʱ�䣬��ʽΪyyyymmddHHMMss
 * @����ֵ��        -1 - ʧ�� 0 - �ɹ� 
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_SetLogConf(LOG_TYPE logType, LOG_LEVEL logLevel, const char* szBeginTime, const char* szEndTime);

/* �ӿں���
 * IVS_GetLogConf
 * @logType:        ��־���֧�ְ�ȫ��־(TYPE_SEC)��������־(TYPE_OPR)��������־(TYPE_RUN)
 * @logLevel:       ��־����
 * @����ֵ��        -1 - ʧ�� 0 - �ɹ� 
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_GetLogConf(LOG_TYPE logType, LOG_LEVEL &logLevel);

/* �ӿں���
 * IVS_LogInitOutput
 * @pOutput:        ��־�������ָ��
 * @����ֵ��-1 - ʧ�� 0 - �ɹ� 
 * remarks: �����ָ�룬�ڲ���������������
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogInitOutput ( ILogOutput* pOutput );

/* �ӿں���
 * IVS_LogCleanOutput
 * @����ֵ��-1 - ʧ�� 0 - �ɹ� 
 * remarks: �ڲ�������־�����������������ֻ�ǽ�ָ���ÿգ��������������ⲿ��
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogCleanOutput ();

/* �ӿں���
 * IVS_LogSetFileSize
 * @����ֵ���ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 * ��������������ָ�����͵���־�ļ���С
 * remarks: ��־�ļ���СĬ��Ϊ10M��10 * 1024 * 1024
 */
extern "C" int EXPORT_BP_LOG_API  IVS_LogSetFileSize(enum LOG_TYPE type, unsigned long s);

/* �ӿں���
 * IVS_LogSetFileCount
 * @����ֵ���ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 * ��������������ָ�����͵���־�ļ�����
 * remarks: 
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetFileCount(enum LOG_TYPE type, unsigned int c);

/* �ӿں���
 * IVS_LogSetLevel
 * @����ֵ���ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 * ��������������ָ�����͵���־�ļ�����level��Ч������Ϊ0
 * remarks: �����־�ļ��ļ���С�����õļ��𣬸���־�ļ�������
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetLevel(enum LOG_TYPE type, int iLevel);

/* �ӿں���
 * IVS_LogSetDebug
 * @����ֵ���ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 * ��������������ָ�����͵���־�ļ��Ƿ�򿪵�����־
 * remarks: ���û�д򿪵�����־������д������־�ļ���Ϣ��
 */
extern "C" int EXPORT_BP_LOG_API IVS_LogSetDebug(enum LOG_TYPE type, bool bEnable);

/* �ӿں���
 * IVS_LogSetTimeZone
 * @����ֵ��-1 - ʧ�� 0 - �ɹ� 
 * ��������������ʱ����Ϣ
 * remarks: ���õ�ǰʱ������ڸ�������ʱ����ƫ����
 */
extern "C" void EXPORT_BP_LOG_API IVS_LogSetTimeZone(unsigned int hour, unsigned int min, unsigned int sec, int flag);
extern "C" const char EXPORT_BP_LOG_API *IVS_GetLogTypeName(int logtype);


#ifdef LOG_HAS_SFTP_UPLOAD
extern "C" void EXPORT_BP_LOG_API IVS_LogSetSftp(const struct BP_LOG_SFTP_INFO & rSftpInfo);
#endif

/* ���������ʽ���������ɱ���� */
int EXPORT_BP_LOG_API IVS_LogFormatErrCode(char* dest, const int len, const int errCode, const char* fmt);

/* ������־ */
extern "C" int EXPORT_BP_LOG_API IVS_LogRun( const int level, const char *operation, const char *file, const int line, const char *func, const char *fmt, ...);
#define IVS_LOG_RUN(level, operation, fmt, ...) \
    IVS_LogRun(level, operation, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define IVS_LOG(level, fmt, ...)\
    IVS_LogRun(level, "", __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

/* ������־ */
extern "C" int EXPORT_BP_LOG_API IVS_LogDbg(const char *file, const int line, const char *func, const char *fmt, ...);
#define IVS_LOG_DBG(fmt, ...) \
    IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)


/* �ӿ���־ */
#ifdef LOG_HAS_ITF 
extern "C" int EXPORT_BP_LOG_API IVS_LogInterface ( const int level,
                                                    const char *inftype, const char *infname,
                                                    const char *sender, const char *receiver,
                                                    const char *traceMark, const char *operation, const char *result,
                                                    const char *file, const int line, const char *func,
                                                    const char *fmt, ...
                                                   );

# define IVS_LOG_ITF(level, inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...)\
    IVS_LogInterface(level, inftype, infname, sender, receiver, traceMark, operation, result,\
                     __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* ������־ */
#ifdef LOG_HAS_OPR
extern "C" int EXPORT_BP_LOG_API IVS_LogOperation ( const int level,
                                                    const char *user,  const char* ip, const char *object,
                                                    const int opcode, const char* operation, const char *result,
                                                    const char *file, const int line, const char *func,
                                                    const int errcode,
                                                    const char *fmt, ...
                                                   );

# define IVS_LOG_OPR(level, user, ip, object, opcode, operation, result, errcode, fmt, ...)\
    IVS_LogOperation(level, user, ip, object, opcode, operation, result,\
                     __FILE__, __LINE__, __FUNCTION__, errcode, fmt, ##__VA_ARGS__)
#endif 

/* ��ȫ��־ */
#ifdef LOG_HAS_SEC
extern "C" int EXPORT_BP_LOG_API IVS_LogSecurity ( const int level,
                                                   const char *user, const char *ip,
                                                   const int opcode, const char* operation, const char *result,
                                                   const char *file, int line, const char *func,
                                                   const int errcode,
                                                   const char *fmt, ...
                                                 );

# define IVS_LOG_SEC(level, user, ip, opcode, operation, result, errcode, fmt, ...)\
    IVS_LogSecurity(level, user, ip, opcode, operation, result,\
                    __FILE__, __LINE__, __FUNCTION__, errcode, fmt, ##__VA_ARGS__)
#endif 


/* �û���־ */
#ifdef LOG_HAS_USER
extern "C" int EXPORT_BP_LOG_API IVS_LogUser(int level, char *user, char *operation, char *result,
             char *file, int line, char *func, const char *fmt, ...);

# define IVS_LOG_USR(level, user, operation, result, fmt, ...)\
    IVS_LogUser(level, user, operation, result, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* ������� */
extern "C" void EXPORT_BP_LOG_API IVS_LogAssert(const char *expr, const char *file, unsigned int line, const char *func);
#if defined(_DEBUG) || defined (DEBUG) 
# define IVS_ASSERT(expr) ((expr) ? void(0): IVS_LogAssert(#expr, __FILE__, __LINE__, __FUNCTION__))
#else
# define IVS_ASSERT()
#endif 

#ifdef __cplusplus
class EXPORT_BP_LOG_API CDbgTrace {
    CDbgTrace(const char *file, unsigned int line, const char *func)
        :m_pFunc(func), m_pFile(file), m_uLine(line)
    {
        IVS_LogDbg(m_pFile, line, m_pFunc, "Enter Function %s", func);   
    }
    ~CDbgTrace()
    {
        IVS_LogDbg(m_pFile, m_uLine, m_pFunc, "Leave Function %s", m_pFunc);
    }
private:    
    const char *m_pFunc;
    const char *m_pFile;
    unsigned int m_uLine;
};
# define IVS_TRACE()   CDbgTrace t(__FILE__, __LINE__, __FUNCTION__)
#else
# define IVS_TRACE()
#endif  // __cplusplus 

#endif // __IVS_LOG_H__

