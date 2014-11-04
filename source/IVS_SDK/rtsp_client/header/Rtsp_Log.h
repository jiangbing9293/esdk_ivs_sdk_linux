/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� ��   : NSS_Log.h
  �� �� ��   : ����
  ��    ��   : qianjiuyun
  ��������   : 2010��11��27��
  ����޸�   :
  ��������   : ��־ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2010��11��27��
    ��    ��   : qianjiuyun
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __RTSP_LOG_H__
#define __RTSP_LOG_H__

#include "rtsp_client_datatype.h"

// ��־����
typedef enum
{
    LOG_TYPE_SEC = 0,   // ��ȫ��־
    LOG_TYPE_OPR,       // ������־
    LOG_TYPE_RUN,       // ������־
    LOG_TYPE_USR,       // �û���־
    LOG_TYPE_DBG,       // ������־
    LOG_TYPE_MAX
}NSS_LOG_TYPE;

// �����־
void nssPrintLog(const NSS_LOG_TYPE logType, const RTSP_LOG_LEVEL_E logLevel, 
                const char * usr, const char* ip, const char* ret,
                const char* flowID,const char* sessionID,
                const char * file, const long line, const char * format, ...);


// ��ȫ��־:Error �����������־��Info �����������־��
#define IVS_SEC_LOG_ERR(usr, ret, ...) \
    nssPrintLog(LOG_TYPE_SEC,RTSP_LOG_LEVEL_ERROR, usr,"", ret, "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_SEC_LOG_INF(usr, ret, ...) \
    nssPrintLog(LOG_TYPE_SEC,RTSP_LOG_LEVEL_INFO, usr,"", ret, "", "", __FILE__, __LINE__, __VA_ARGS__);

// ������־:Error �����������־��Info �����������־��
#define IVS_OPR_LOG_ERR(usr, ip,ret,...) \
    nssPrintLog(LOG_TYPE_OPR,RTSP_LOG_LEVEL_ERROR, usr,ip, ret, "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_OPR_LOG_INF(usr, ip,ret,...) \
    nssPrintLog(LOG_TYPE_OPR,RTSP_LOG_LEVEL_INFO, usr,ip, ret, "", "", __FILE__, __LINE__, __VA_ARGS__);

// ������־: Emergency �����������־��Alert �����������־��Critical �����������־��Error �����������־��
            // Warning �����������־�� Notice �����������־��Info �����������־��
#define IVS_RUN_LOG_EMR(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_EMERGENCY, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_ALT(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_ALART, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_CRI(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_CRITICAL, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_ERR(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_ERROR, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_WAR(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_WARNING, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_NTC(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_NOTICE, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);
#define IVS_RUN_LOG_INF(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_INFO, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);

// �û���־:Critical ������û���־�� Error ������û���־��Info ������û���־��
#define IVS_USR_LOG_CRI(usr,flowID,sessionID,...) \
    nssPrintLog(LOG_TYPE_USR,RTSP_LOG_LEVEL_CRITICAL, usr, "", "", flowID, sessionID, __FILE__, __LINE__, __VA_ARGS__);
#define IVS_USR_LOG_ERR(usr,flowID,sessionID,...) \
    nssPrintLog(LOG_TYPE_USR,RTSP_LOG_LEVEL_ERROR, usr, "", "", flowID, sessionID, __FILE__, __LINE__, __VA_ARGS__);
#define IVS_USR_LOG_INF(usr,flowID,sessionID,...) \
    nssPrintLog(LOG_TYPE_USR,RTSP_LOG_LEVEL_INFO, usr, "", "", flowID, sessionID, __FILE__, __LINE__, __VA_ARGS__);

// ������־:���ּ���  rtspֻ��¼run��־��������־��¼��runĿ¼��
#define IVS_DBG_LOG(...) \
    nssPrintLog(LOG_TYPE_RUN,RTSP_LOG_LEVEL_DEBUG, "", "", "", "", "", __FILE__, __LINE__, __VA_ARGS__);

#endif // __RTSP_LOG_H__

