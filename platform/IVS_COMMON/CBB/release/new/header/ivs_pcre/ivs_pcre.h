/********************************************************************
filename    :    ivs_pcre.h
author      :    l00193369
created     :    2012.11.3
description :    ������ʽ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 ��ʼ�汾
*********************************************************************/

#ifndef CBB_PCRE_H
#define CBB_PCRE_H


#include "ivs_namespace.h"

#include "pcre.h"

NAMESPACE_CBB_BEGIN

/************************************************************************
 * ����������ʽ 
 * pattern    ������ʽ
 * options    Ϊ0��������������ѡ��
 * errptr     ������Ϣ
 * erroffset  ����λ��
 * tableptr   ָ��һ���ַ������ָ�룬��������Ϊ��NULL
 * ����ֵ      �ɹ����� pcre*, ʧ�ܷ��� NULL
 ***********************************************************************/
extern "C"  EXPORT_DLL pcre * ivs_pcre_compile(const char *pattern, int options, const char **errptr, 
                                               int *erroffset, const unsigned char *tableptr);

/************************************************************************
 * ʹ�ñ���õ�ģʽ����ƥ�䣬���õ���һ�ַǴ�ͳ�ķ���DFA��ֻ�Ƕ�ƥ�䴮ɨ��һ�� 
 * code      ����õ�ģʽ
 * extra     ָ��һ��pcre_extra�ṹ�壬����ΪNULL
 * subject   ��Ҫƥ����ַ���
 * length    ƥ����ַ������ȣ�Byte
 * startoffset        ƥ��Ŀ�ʼλ��
 * options   ѡ��λ
 * ovector   ָ��һ���������������
 * ovecsize  �����С
 * ����ֵ     �ɹ����� ����ƥ�䴮��ƫ��λ��, ʧ�ܷ��� -1
 ***********************************************************************/
extern "C"  EXPORT_DLL int ivs_pcre_exec(const pcre *code, const pcre_extra *extra, const char *subject, 
                                         int length, int startoffset, int options, int *ovector, int ovecsize);

/************************************************************************
 * �ͷű������ɵ�pcre
 * void *    �ͷŵ�ָ��
 * ����ֵ     void
 ***********************************************************************/
extern "C"  EXPORT_DLL void ivs_pcre_free(void * pattern);

NAMESPACE_CBB_END

#endif // CBB_PCRE_H
