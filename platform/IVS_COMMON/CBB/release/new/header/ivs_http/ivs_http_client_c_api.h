/********************************************************************
filename    :    ivs_http_client_c_api.h
author      :    yWX150774
created     :    2013/03/06
description :    HTTP�ͻ����ϴ������ص�C�ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/06 ��ʼ�汾
*********************************************************************/
#ifndef IVS_HTTP_CLIENT_C_API_H
#define IVS_HTTP_CLIENT_C_API_H

#include "ivs_namespace.h"

/* ����ӿڲ�����CBB�����ռ��ԭ����, ����C#Ҫ������ӿ�, ���Բ�ʹ�������ռ� */

/*********************************************************
 * ����:   ��HTTP�����������ļ�
 * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Http_Client_DownloadFile(
    const char * pszLocalWriteFileName, 
    const char * pszRemoteURL, 
    const char * pszUserName = NULL, 
    const char * pszPassword = NULL);

/*********************************************************
 * ����:   �ϴ��ļ���HTTP������
 * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Http_Client_UploadFile(
    const char * pszLocalReadFilename, 
    const char * pszRemoteURL, 
    const char * pszUserName = NULL, 
    const char * pszPassword = NULL);

#endif // IVS_HTTP_CLIENT_C_API_H
