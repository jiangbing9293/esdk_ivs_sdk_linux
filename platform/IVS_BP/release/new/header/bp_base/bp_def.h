/********************************************************************
    filename    :     bp_def.h
    author        :    s00191067
    created        :    2012/03/27
    description    :    �ṩMAU��������
    copyright    :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
    history        :    2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_DEF_H
#define BP_DEF_H

#include <string>
#include <iostream>
#include "bp_log.h"
#include "bp_namespace.h"
#include "bp_mem.h"
#include "bp_stdint.h"

NAMESPACE_BP_BEGIN // namespace begin

#ifdef WIN32
#define EXPORT_DLL __declspec(dllexport)
#else
#define EXPORT_DLL
#endif

// �������ݿ⶯̬���ӿ��ļ�����
#if defined(_DEBUG) || defined (DEBUG) 
# define DB_DLL_NAME_ORACLE "db_oracled"
# define DB_DLL_NAME_PG "db_pgsqld"
#else
# define DB_DLL_NAME_ORACLE "db_oracle"
# define DB_DLL_NAME_PG "db_pgsql"
#endif 

#define TRANSACTIONNO_LEN 64	//����ID
#define RESERVE_LEN 32		//�����ֶγ���
#define DEVICE_CODE_LEN	40	//��Ԫ�豸����

#define LOOP_RETRY             (0)     // linkѭ������
#define IP_LENGTH              (65)    // IP��ַ����
#define NET_ELEMENT_ID_LEGTH   (41)    // ��ԪID����

//�������̴߳������Ϣ��һ�������ϱ��¼����澯��״̬�ı䣩
#define LINK_STATUS_REPORT_CMD 0x8001   //LINK״̬�ϱ�����
#define FLUX_CONTROL_WARN_CMD  0x8002   //���ظ澯�ϱ�
#define LOG_WARN_CMD           0x8003   //��־�澯�ϱ�
#define DB_WARN_CMD            0x8004   //���ݿ�澯�ϱ�
#define CONFIG_WARN_CMD        0x8005   //���ø澯�ϱ�

// �ж�ָ���Ƿ�Ϊ�� 
#define CHECK_POINTER( pPoint, retValue )    \
if ( NULL == ( pPoint ) )                    \
{                                            \
    BP_DBG_LOG("Point " #pPoint " is null.");\
    return retValue;                         \
}                                            \

// �ж�ָ���Ƿ�Ϊ��,�޷���ֵ,���ܻ���ʾ���˷ֺ�
#define CHECK_POINTER_VOID( pPoint )        \
if ( NULL == ( pPoint ) )                   \
{                                           \
    BP_DBG_LOG("Point " #pPoint" is null.");\
    return ;                                \
}                                           \

// �ж�ִ���Ƿ�ɹ���ʧ����return RetValue,����Դ���ͷ�ʱ����
#define CHECK_SUCCESS_RET( ifPcLint, retValue )   \
do{                                               \
    if ( !(ifPcLint) ) {return retValue;}         \
}while(0)

// �ж�AB�Ƿ����  ��==��
#define CHECK_EQUAL( A, B, retValue ) \
if ( (B) == (A) )                     \
{                                     \
    BP_DBG_LOG("A Equal B.");        \
    return retValue;                  \
}

#define NOLINT_MAIN if (false){ACE_Main acem;(void)ace_os_main_i(acem, argc, argv);}

// MAU��Ϣ����ض���
typedef enum enMsgBlock
{
    MSG_BLOCK_SIZE          = 2 * 1024,     // ������Ϣ���С
    NSS_BLOCK_SIZE          = 1800,         // ����һ��NSS����С
    BLOCK_NUM_PER_CHANNEL   = 200,          // ÿ·��Ƶ�������Ϣ�����
    MAX_QUEUE_MSG_NUM       = 1 * 1024,     // ������Ϣ���������Ϣ����
    MAX_QUEUE_BYTE_SIZE     = (MSG_BLOCK_SIZE * MAX_QUEUE_MSG_NUM)  // ������Ϣ�������SIZE
} MSG_BLOCK;

// �����̶߳�ջ��С
#define DEFAULT_THREAD_STACK_SIZE   (128 * 1024)

// ���б��� �������Ĭ���������
#define DEFAULT_QUEUE_PROTECT_SIZE  (2 * 1024)

// Transaction�����ֵ
#define DEFAULT_TRANSACTION_MAP_MAX_SIZE 2000000

// Session���������
#define DEFAULT_SESSION_MAP_MAX_SIZE     200000

// �߳���Ϣ���е��������
#define DEFAULT_CMD_QUEUE_LENGTH    3000

// ���б��� ���Sequence���е�����
#define MAX_QUEUE_PROTECT_SIZE  (8 * 1024)

const int     SERVICE_COMMU = 1;

//�澯��Ϣ�ظ�
const unsigned int RESEND_INTERVAL_OFFSET_MIN        = 1;     // ��С�ط�ʱ����
const unsigned int RESEND_INTERVAL_OFFSET_DEFAULT    = 5;     // Ĭ���ط�ʱ����
const unsigned int RESEND_INTERVAL_OFFSET_MAX        = 40;    // ����ط�ʱ����

const unsigned int RESEND_TIMES_MIN                  = 0;     // ��С�ط�����
const unsigned int RESEND_TIMES_DEFAULT              = 10;    // Ĭ���ظ�����
const unsigned int RESEND_TIMES_MAX                  = 15;    // ����ط�����

// CmdType��ö�ٶ���
typedef enum enCmdType
{
    CMD_UNDEF   = 0,
    CMD_SEND,
    CMD_RECEIVE,
    CMD_CREATE_LINK,
    CMD_DESTROY_LINK,
	CMD_CREATE_LINK_SYNC,
	CMD_DESTROY_LINK_SYNC,
	CMD_LIMIT,

} CMD_TYPE;


typedef enum enSubsysType
{
    SUBSYS_NONE,  //��Чֵ
    SUBSYS_SCU = 1,
    SUBSYS_SMU = 2,
    SUBSYS_MU  = 3,
    SUBSYS_DCG = 4,
    SUBSYS_PCG = 5,
    SUBSYS_OMU = 6,
    SUBSYS_CMU = 7,
    SUBSYS_SDK = 8,
	SUBSYS_MULM = 9,
    SUBSYS_MAU = 16,
    //�ڴ�֮ǰ�������ֵ
    SUBSYS_MAX
} SUBSYS_TYPE;


// ��������
typedef enum enLinkType
{
    LTYPE_TCP,                    // TCP����
    LTYPE_UDP,                    // UDP����
}LINK_TYPE;

// Ӧ�ò�Э������
typedef enum enProtocolType
{
    PROTOCOL_UNDEF = 0,
    PROTOCOL_IVSP  = 1,      // ˽��Э��
    PROTOCOL_RTSP  = 2,      // RTSP
    PROTOCOL_RTP   = 3,       // RTPЭ��
    PROTOCOL_RTCP  = 4,      // RTCPЭ��
    PROTOCOL_NSS   = 5,
	PROTOCOL_LIMIT
}PROTOCOL_TYPE;


/*
 * ������Ԫ����
 * ��Ԫ����Ϊ�з���32λ����, ���ֵΪ2147483647.
  xx xx xx
        |--- Э������,�μ�PROTOCOL_TYPE
  |   |--- ��������, ��ҵ���壬��Ҫ���ܵ�BP
  |--- ��ϵͳ����, �μ�IVS_SUBSYS_TYPE
*/
#define MAKE_NET_ELEMENT_TYPE(subsytem, process, protocl_type) ((int)(subsytem) * 10000 + (int)(process) * 100 + (int)(protocl_type) )

#define NET_ELE_SCU_NONE    0
#define NET_ELE_SCU_NSS     10105
#define NET_ELE_SMU_NSS     20105
#define NET_ELE_MU_NSS      30105
#define NET_ELE_DCG_NSS     40105
#define NET_ELE_PCG_NSS     50105
#define NET_ELE_OMU_NSS     60105
#define NET_ELE_CMU_NSS     70105
#define NET_ELE_SDK_NSS		80105
#define NET_ELE_SUB_SMU_NSS 90105
#define NET_ELE_MAU_NSS     160105


using std::string;

// �߳�����
#define THREAD_SERVICE_NAME "Service"
#define THREAD_COMMU_NAME   "Commu"
#define THREAD_ROUTE_LOCUS_SERVICE_NAME "RouteLocusService"

//reinterpret_cast����ת��
#ifdef WIN32
#define MAU_REINTEPERET_CAST(dstType) reinterpret_cast<dstType>
#else
#define MAU_REINTEPERET_CAST(dstType) (dstType)
#endif

// snprintf����
#ifdef  WIN32  
#define SNPRINTF  _snprintf_s    
#else 
#define SNPRINTF  snprintf
#endif 

#ifdef WIN32
#define STRTOK  strtok_s
#else
#define STRTOK  strtok_r
#endif


NAMESPACE_BP_END   // namespace end
#endif //BP_DEF_H
