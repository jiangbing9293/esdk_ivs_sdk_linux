
/******************************************************************************
   ��Ȩ���� (C), 2008-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �ļ���          : NVSLog.h
  �汾��          : 1.0
  ����            : xuxin 00114494
  ��������        : 2008-8-17
  ����޸�        : 
  ��������        : NVS��־ģ���û��ӿ�
  �����б�        : 
  �޸���ʷ        : 
  1 ����          : 
    ����          : 
    �޸�����      : 
*******************************************************************************/


#ifndef IVS_LOG_H
#define IVS_LOG_H

#include <stdarg.h>

#define IVSLOG_API

//BEGIN R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־
namespace IVSLOG
{
    const unsigned long LOG_FILE_COUNT_MAX  = 10;   //��־�ļ�������
    const unsigned long MAXLEN_FILEPATH     = 256;  //�ļ�·����󳤶�

    typedef char PATH_ARRAY[MAXLEN_FILEPATH];       //·����������
}
//END   R001C05LITS01 ADD 2010-09-17 ligengqiang 00124479 for ʵʱѭ��������־����ౣ��10����־

//��־�ȼ�
typedef enum _IvsLogLevel
{
    LOG_EMERGENCY   = 0,    //ϵͳ������
    LOG_ALERT       = 1,    //�������̲�ȡ�ж�������ϵͳ������
    LOG_CRITICAL    = 2,    //���ش���
    LOG_ERROR       = 3,    //һ�����
    LOG_WARNING     = 4,    //����
    LOG_NOTICE      = 5,    //��Ҫ��������Ϣ
    LOG_INFO        = 6,    //һ���������Ϣ
    LOG_DEBUG       = 7,    //������Ϣ
    LOG_MAX                 //��־�������ֵ�����Ϸ���У����
}IvsLogLevel;

//д��־�̺߳����Ĵ�������
typedef enum _IvsLogThreadErr
{
   SUCCESS = 0,       //�ɹ� 
   NEGLECTABLE_ERR,   //���Ժ��Ե����⣬�߳̿��Լ���ִ��
   CRITICAL_ERR       //�������⣬�̱߳����˳�
}IvsLogThreadErr;

//������־����Ĭ����:LOG_INFO
IVSLOG_API void IvsSetLogLevel(long lLevel);

//���õ�ǰд����־�ļ�·����(����·�������·��)
//Ĭ���ǵ�ǰ·����:exename.log
IVSLOG_API bool IvsSetLogFilePathName(const char* szPathName);

//������־�ļ��������ƣ������˳���ʱ�������ļ�����λKB(100K-100M,Ĭ����10M)
IVSLOG_API void IvsSetLogFileLengthLimit(unsigned long ulLimitLengthKB);

//��������������ɺ󣬿�������NVS��־ģ��
IVSLOG_API void IvsStartLog(void);

//дһ����־(�����涨���NVSWriteLog����д��־)
IVSLOG_API void __IvsWriteLog(const char* szFileName, long lLine,long lLevel, const char* format, va_list argp);
//ֹͣNVS��־ģ��
IVSLOG_API void IvsStopLog(void);

//vc6�Լ�vc7.1����֧��C99(��g++֧��)
//�������ﲻ��ʹ�ÿɱ�����궨�壬���ö�()������������ʵ��
class CWriter   
{
    public:
        CWriter(const char* file, long line)
        {
            m_file_ = file;
            m_line_ = line;
        }
        void operator()(long level, const char* format, ...)
        {
            va_list argp;
            va_start(argp, format);
            __IvsWriteLog(m_file_,m_line_,level,format,argp);
            va_end(argp);
        }
    private:
        CWriter()   //��PC-LINT
        {
            m_file_ = NULL;
            m_line_ = 0;
        }
        const char* m_file_;
        long m_line_;
};

//������ʹ�����º�д��־
#define IVS_LOG (CWriter(__FILE__, __LINE__))
//���磺IVS_LOG(LOG_INFO, "Recv=%d,Send=%d", nRecv,nSend);


#endif//IVS_LOG_H

