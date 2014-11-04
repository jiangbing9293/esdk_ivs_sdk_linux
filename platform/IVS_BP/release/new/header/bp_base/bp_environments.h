/********************************************************************
filename    :    bp_enviroments.h
author      :    w90004666
created     :    2012/9/18
description :	 ��ȡIVS��������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/9/18 ��ʼ�汾
*********************************************************************/

#ifndef _bp_enviroments_h
#define _bp_enviroments_h

#include <string>
#include "bp_namespace.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN

	extern "C" int EXPORT_DLL get_home_directory(std::string& dir);
	extern "C" void EXPORT_DLL set_home_directory(std::string dir);
	extern "C" int EXPORT_DLL get_bin_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_log_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_config_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_service_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_protocol_directory(std::string& dir);
	extern "C" int EXPORT_DLL get_lib_directory(std::string& dir);

	/************************************************************************
	* �޸�Linux�½��̴򿪵��ļ����ޣ�ֻ��Linux��Ч
	*************************************************************************/
	extern "C" void EXPORT_DLL set_open_files_limits(int value);
	extern "C" int EXPORT_DLL get_open_files_limits();

	/************************************************************************
	* ��NULL�������ַ���<src>�еĻ��������滻Ϊ����������ֵ��������<dst>�з��ء�
	* �ɹ�����0��ʧ�ܷ��ش�����.
	*************************************************************************/
	extern int replace_env_var(const char *src, std::string& dst);

    /*************************************************************************
    * FUNCTION: time_t str2time()
    * DESCRIPTION: ����20090629182030��ʱ�䴮ת��������Ϊ��λ������ʱ��,
    *              ���Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00����������������
    * INPUT :  const char *pStr -- ʱ���ַ���
    * OUTPUT:
    *      �Թ��ʱ�׼ʱ�乫Ԫ1970��1��1��00:00:00��������������
    *      ֻ֧��"YYYYMMDDhhmmss"��ʽ��ʱ��ת��
    *************************************************************************/
    extern time_t EXPORT_DLL str2time(const char *pStr);

    /**************************************************************************
    * ֧��"YYYYMMDDhhmmss"��"YYYYMMDDThhmmssZ"��"YYYY-MM-DD hh:mm:ss"
    * ����ʱ���ʽ��Ĭ�ϲ���UTC��ʾ��
    *************************************************************************/
    extern EXPORT_DLL char* time2str(char *pDestBuf, int nbuflen, const time_t t, const char *pFormat = "YYYYMMDDhhmmss");

    enum TIME_STRUCT_CONST
    {
        UTILITY_YEAR_LEN   = 4,   //����2010�ĳ���
        UTILITY_MDHMS_LEN  = 2,   //����ʱ������ռ����
        CBB_YEAR_BASE     = 1900, //ϵͳʱ��ԭ��
        CBB_INVALID_TIME  = -1,   //��Чʱ��
    };

    #pragma pack(push, 1)
    
    //��ʽΪ20101208010108��ʱ��ṹ
    struct TIME_YYYYMMDDH24MISS
    {
        char  year[UTILITY_YEAR_LEN];
        char month[UTILITY_MDHMS_LEN];
        char   day[UTILITY_MDHMS_LEN];
        char  hour[UTILITY_MDHMS_LEN];
        char   min[UTILITY_MDHMS_LEN];
        char   sec[UTILITY_MDHMS_LEN];
    };

    #pragma pack(pop)

    // �ṩ64λ�ֳ����ֽ���ת����
    #if __BYTE_ORDER == __LITTLE_ENDIAN || defined(WIN32)
    #define BP_ntohll(net, host) \
    (\
        (host) \
             = \
        (\
            (((net) >> 56) & 0x00000000000000ffull) | \
            (((net) >> 40) & 0x000000000000ff00ull) | \
            (((net) >> 24) & 0x0000000000ff0000ull) | \
            (((net) >>  8) & 0x00000000ff000000ull) | \
            (((net) <<  8) & 0x000000ff00000000ull) | \
            (((net) << 24) & 0x0000ff0000000000ull) | \
            (((net) << 40) & 0x00ff000000000000ull) | \
            (((net) << 56) & 0xff00000000000000ull)\
        )\
    )
    #define BP_htonll(host, net) BP_ntohll(host, net)
    #else
    #define BP_ntohll(net, host) (host) = (net)
    #define BP_htonll(host, net) (net) = (host)
    #endif

NAMESPACE_BP_END

#endif //_bp_enviroments_h

