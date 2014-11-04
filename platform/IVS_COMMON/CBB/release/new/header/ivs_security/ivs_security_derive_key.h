/********************************************************************
filename    :    ivs_security_derive_key.h
author      :    yWX150774
created     :    2013/01/23
description :    ��Կ�����㷨
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/01/23 ��ʼ�汾
*********************************************************************/

#ifndef CBB_SECURITY_DERIVE_KEY_H
#define CBB_SECURITY_DERIVE_KEY_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/****************************************************************
 * ��Կ���� (���ɵ���Կ����ѭ������Keyֵ)
 *    ʵ��˵��:
 *       1. �����趨��Ȼ����C30�Ķ���
 *       2. ��Ҫ����C30��ʵ��
 *       3. C30�ڲ���uiIterateCount�ټ�����һ��, 
 *          Ϊ�˼���, �˴���Ҳ������һ��
 *       4. ������ÿ�����C30�ļ���, ���Կ���Ϊ��������ֵ�����趨Ĭ��ֵ
 *       5. ��C30�汾��һ�㲻ͬ��, ��һ�����������͸�Ϊ�޷��ŵ�
 *          ��Ȼ, ����Ǹ������
 * pszKeyBuffer  : ���ڱ��浼������Կ�Ļ�����
 * uiKeyBufferLen: ���ڱ��浼������Կ�Ļ������Ĵ�С (����'\0'��β)
 * pszInitKey    : ��ʼ��Կ
 * uiInitKeyLen  : ��ʼ��Կ�ĳ���, Ϊ0ʱ, �Զ�ȡ�ַ���pszInitKey�ĳ���
 * pszSalt       : ������Ϣ
 * uiSaltLen     : ������Ϣ�ĳ���, Ϊ0ʱ, �Զ�ȡ�ַ���pszSalt�ĳ���
 * uiIterateCount: �ظ�����, Ϊ0ʱ, �Զ�ȡ2048
 * ����ֵ         : �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ****************************************************************/
extern "C" int EXPORT_DLL IVS_Security_Derive_Key(
    unsigned char * pszKeyBuffer, unsigned int uiKeyBufferLen, 
    const char * pszInitKey, unsigned int uiInitKeyLen, 
    const char * pszSalt, unsigned int uiSaltLen, 
    unsigned int uiIterateCount);

NAMESPACE_CBB_END


#endif // CBB_SECURITY_DERIVE_KEY_H
