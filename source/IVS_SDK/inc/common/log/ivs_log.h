/*******************************************************************************
//  ��Ȩ����    2012 ��Ϊ�������޹�˾
//  �� �� ����  IVS_Player
//  �� �� ����  ivs_log.h
//  �ļ�˵��:   ��־�ӿ�ͷ�ļ�
//  ��    ��:   IVS V100R002C01
//  ��    ��:   w00210470
//  ��������:   2012/11/12
//  �޸ı�ʶ��
//  �޸�˵����
*******************************************************************************/
#ifndef  __IVS_LOG_H__
#define  __IVS_LOG_H__

enum LOG_LEVEL 
{
    IVS_LOG_EMERG   = 0,   /* ������ϵͳ������ */
    IVS_LOG_ALERT   = 1,   /* ����������ȡ��ʩ */
    IVS_LOG_CRIT    = 2,   /* �������         */
    IVS_LOG_ERR     = 3,   /* �������         */
    IVS_LOG_WARNING = 4,   /* �������         */
    IVS_LOG_NOTICE  = 5,   /* һ����Ҫ���     */
    IVS_LOG_INFO    = 6,   /* ��ͨ��Ϣ         */
    IVS_LOG_DEBUG   = 7    /* ������Ϣ         */
};

enum LOG_TYPE
{
    TYPE_SEC = 0,   /* ��ȫ��־ */
    TYPE_OPR = 1,   /* ������־ */
    TYPE_RUN = 2,   /* ������־ */
    TYPE_ITF = 3,   /* �ӿ���־ */
    TYPE_USR = 4,   /* �û���־ */
    TYPE_DBG = 5    /* ������־ */
};


/* ʱ�����ñ�־ */
enum  {
    UTC_FLG_PLUS = 0,   /* UTC + */
    UTC_FLG_NEG  = 1    /* UTC - */
};


/* ����������� */
//#define LOG_HAS_SFTP_UPLOAD       /* ��־sftp�ϴ����� ע��ȡ���ù��� */
#define LOG_HAS_CONFIG_FILE       /* ��־���������ļ� ע��ȡ���ù��� */
#define LOG_HAS_WRITE_THREAD      /* ʹ�ö����߳�д�� ע��ȡ���ù��� */
//#define LOG_HAS_STACK_TRACE       /* �������ص���־ͬʱ��ӡ����ջ��ע��ȡ���ù��� */

#if defined LOG_HAS_SFTP_UPLOAD || (!defined LOG_HAS_SFTP_UPLOAD && !defined LOG_HAS_WRITE_THREAD)
#define LOG_HAS_MONITOR_THREAD   /* ����߳� */
#endif 

//#define LOG_HAS_SEC               /* ʹ�ܰ�ȫ��־ ע��ȥʹ�� */ 
//#define LOG_HAS_OPR               /* ʹ�ܲ�����־ ע��ȥʹ�� */
//#define LOG_HAS_ITF               /* ʹ�ܽӿ���־ ע��ȥʹ�� */
//#define LOG_HAS_USER              /* ʹ���û���־ ע��ȥʹ�� */

#define FILE_PRESERVE_COUNT     10                /* ������־�ļ���󱣴����� */
#define FILE_MAX_SIZE_DEF      (10 * 1024 * 1024) /* Ĭ���ļ����� 10 M */


/* �쳣�ص����� */
#define LOG_ERR_DISKFULL       500   /* ������         */   
#define LOG_ERR_DISKRESTORE    501   /* ���ָ̻�       */
#define LOG_ERR_OPENFAIL       502   /* ���ļ�ʧ��   */
#define LOG_ERR_DBIMPORT       503   /* �������ݿ���� */
#define LOG_ERR_SFTPFAIL       504   /* sftp�쳣       */

/* --------------------------------------------------------------------------- */

/* �ӿں��� */
int  IVS_LogInit(const char *szDir, const char *szConf, const char* szFileName, const char *szModuleType, 
	             int uModuleCode, void (*callback)(int code));
void IVS_LogClean();

int  IVS_LogSetFileSize(unsigned long s);
int  IVS_LogSetFileCount(unsigned int c);
int  IVS_LogSetLevel(int level);
int  IVS_LogSetDebug(int enable);

void IVS_LogSetTimeZone(unsigned int hour, unsigned int min, int flag);

#ifdef LOG_HAS_SFTP_UPLOAD
int  IVS_LogSetSftp(char *szSvrDir, char *szUser, char *szPassWord, 
                   char *szSvrAddr, unsigned short usSvrPort);
#endif


/* ������־ */
void IVS_LogRun(int level, const char *stcMsgText, const char *file, int line, const char *func, const char *fmt, ...);
#define IVS_LOG_RUN(level, stcMsgText, fmt, ...) \
    IVS_LogRun(level, stcMsgText, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)

#define IVS_LOG(level, stcMsgText, fmt, ...)\
    IVS_LogRun(level, stcMsgText, __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
    
/* ������־ */
void IVS_LogDbg(const char *file, int line, const char *func, const char *fmt, ...);
#define IVS_LOG_DBG(fmt, ...) \
    IVS_LogDbg(__FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)


/* �ӿ���־ */
#ifdef LOG_HAS_ITF 
void IVS_LogInterface(int level, char *inftype, char *infname, char *sender, char *receiver,
      char *traceMark, char *operation, char *result, const char *file,
      int line, const char *func, const char *fmt, ...);

# define IVS_LOG_ITF(level, inftype, infname, sender, receiver, traceMark, operation, result, fmt, ...)\
    IVS_LogInterface(level, inftype, infname, sender, receiver, traceMark, operation, result,\
            __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* ������־ */
#ifdef LOG_HAS_OPR
void IVS_LogOperation(int level, char *user,  char* ip, char *object, char *stcMsgNum, char*stcMsgText, char *opcode, 
                     char *result, const char *file, int line, const char *func, const char *fmt, ...);

# define IVS_LOG_OPR(level, user, ip, object, stcMsgNum, stcMsgText, opcode, result, fmt, ...)\
    IVS_LogOperation(level, user, ip, object, stcMsgNum, stcMsgText, opcode, result,\
         __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 

/* ��ȫ��־ */
#ifdef LOG_HAS_SEC
void IVS_LogSecurity(int level, char *user, char *ip, char *stcMsgNum, char*stcMsgText, 
           char*opcode, char *result, const char *file, int line, const char *func, const char *fmt, ...);

# define IVS_LOG_SEC(level, user, ip, stcMsgNum, stcMsgText, operation, result, fmt, ...)\
    IVS_LogSecurity(level, user, ip, stcMsgNum, stcMsgText, operation, result,\
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* �û���־ */
#ifdef LOG_HAS_USER
void IVS_LogUser(int level, char *user, char *operation, char *result,
             char *file, int line, char *func, const char *fmt, ...);

# define IVS_LOG_USR(level, user, operation, result, fmt, ...)\
    IVS_LogUser(level, user, operation, result, \
        __FILE__, __LINE__, __FUNCTION__, fmt, ##__VA_ARGS__)
#endif 


/* ������� */
void IVS_LogAssert(const char *expr, const char *file, int line, const char *func);
#if defined(_DEBUG) || defined (DEBUG) 
# define IVS_ASSERT(expr) ((expr) ? void(0): IVS_LogAssert(#expr, __FILE__, __LINE__, __FUNCTION__))
#else
# define IVS_ASSERT(expr)
#endif 

#ifdef __cplusplus
class CDbgTrace {
public:
    CDbgTrace(const char *file, int line, const char *func)
        :m_pFunc(func), m_pFile(file), m_nLine(line)
    {
        IVS_LogDbg(m_pFile, line, m_pFunc, "Enter Function %s", func);   
    }
    ~CDbgTrace()
    {
		try {IVS_LogDbg(m_pFile, m_nLine, m_pFunc, "Leave Function %s", m_pFunc);} catch(...){} ; //lint !e1740
    }
private:    
    const char *m_pFunc;
    const char *m_pFile;
    int         m_nLine;
};//lint !e1712 ����Ĭ�Ϲ��캯��
# define IVS_TRACE()   CDbgTrace t(__FILE__, __LINE__, __FUNCTION__)
#else
# define IVS_TRACE()
#endif  // __cplusplus 

#endif // __IVS_LOG_H__

