/********************************************************************
filename    :    ivs_sftp_client_c_api.h
author      :    yWX150774
created     :    2013/03/04
description :    SFTP�ͻ����ϴ������ص�C�ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/04 ��ʼ�汾
*********************************************************************/
#ifndef IVS_SFTP_CLIENT_C_API_H
#define IVS_SFTP_CLIENT_C_API_H

#include "ivs_namespace.h"

/* ����ӿڲ�����CBB�����ռ��ԭ����, C#Ҫ������ӿ�, ����ʹ�������ռ� */

/*********************************************************
 * ����:   ��SFTP�����������ļ�
 * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Sftp_Client_DownloadFile(
    const char * pszHostname, unsigned short usHostPort, 
    const char * pszUsername, const char * pszPassword, 
    const char * pszRemoteReadFilename, const char * pszLocalWriteFilename);

/*********************************************************
 * ����:   �ϴ��ļ���SFTP������
 * ����ֵ: �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
 ********************************************************/
extern "C" EXPORT_DLL int IVS_Sftp_Client_UploadFile(
    const char * pszHostname, unsigned short usHostPort, 
    const char * pszUsername, const char * pszPassword, 
    const char * pszLocalReadFilename, const char * pszRemoteWriteFilename);

#endif // IVS_SFTP_CLIENT_C_API_H
