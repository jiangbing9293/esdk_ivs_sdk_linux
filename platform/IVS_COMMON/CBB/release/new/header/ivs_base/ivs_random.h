/********************************************************************
filename    :    ivs_random.h
author      :    s90003596, cWX153028, yWX150774
created     :    2012.11.3
description :    ��ȫ�����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_RANDOM_H
#define CBB_BASE_RANDOM_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/************************************************************************
 * ���������
 *      (�ȳ��Ե���MakeRandomFromSys��ȡ, ������ɹ�, �ٵ���MakeRandomFromLib)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GenerateRandom(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * ��������Ŀɴ�ӡ�ַ����� (����GenerateRandom, ��ת��64�ֿɴ�ӡ�ַ�)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL GenerateRandomChars(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * ����ϵͳ������������� (����ϵͳ����, �ٶȽ���, ��ȫ�Խϸ�)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomFromSys(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * ��������Ŀɴ�ӡ�ַ����� (�ȵ���MakeRandomFromSys, ��ת��64�ֿɴ�ӡ�ַ�)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomCharsFromSys(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * ���ÿ⺯����������� (���ÿ⺯��, �ٶȽϿ�, ��ȫ�Խϵ�)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomFromLib(
    unsigned char * pszRandomBuffer, int iBufferLength);

/************************************************************************
 * ��������Ŀɴ�ӡ�ַ����� (�ȵ���MakeRandomFromLib, ��ת��64�ֿɴ�ӡ�ַ�)
 * pszRandomBuffer : ����������ݵĻ�����
 * iBufferLength   : �������Ĵ�С (������'\0', ������Ҫ, �����ߵ��Լ���'\0')
 * ����ֵ           : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ***********************************************************************/
extern "C" int EXPORT_DLL MakeRandomCharsFromLib(
    unsigned char * pszRandomBuffer, int iBufferLength);

NAMESPACE_CBB_END


#endif // CBB_BASE_RANDOM_H
