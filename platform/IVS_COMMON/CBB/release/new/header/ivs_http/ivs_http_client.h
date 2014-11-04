/********************************************************************
filename    :    ivs_http_client.h
author      :    yWX150774
created     :    2013/03/05
description :    HTTP�ͻ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_HTTP_CLIENT_H
#define CBB_HTTP_CLIENT_H


#include "ivs_uncopy.hpp"

#include <string>

#include "ace_header.h"

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * HTTP�ͻ�����, ��һ�����ɿ�������
 *
 ************************************************************/
class EXPORT_DLL CHttpClient : private CUnCopy
{
public:
    CHttpClient();
    ~CHttpClient();

public:
    /*********************************************************
     * ��ʼ������
     ********************************************************/
    bool Init();

    /*********************************************************
     * ������
     ********************************************************/
    void Exit();

    /*********************************************************
     * ������HTTP������ͨ�ŵ��׽���
     ********************************************************/
    bool Connect(const char * pszHostName, unsigned short usHostPort);

    /*********************************************************
     * �ر���HTTP������ͨ�ŵ��׽���
     ********************************************************/
    void Close();

    /*********************************************************
     * �������ļ��ϴ���HTTP������
     ********************************************************/
    bool UploadFileByRemotePath(
        const char * pszLocalReadFilename, 
        const char * pszRemoteWritePathName);

    /*********************************************************
     * ��HTTP�������ļ����ص�����
     ********************************************************/
    bool DownloadFileByRemotePath(
        const char * pszLocalWriteFileName, 
        const char * pszRemoteReadPathName);

    /*********************************************************
     * �������ļ��ϴ���HTTP������
     ********************************************************/
    bool UploadFileByRemoteURL(
        const char * pszLocalReadFilename, 
        const char * pszRemoteURL);

    /*********************************************************
     * ��HTTP�������ļ����ص�����
     ********************************************************/
    bool DownloadFileByRemoteURL(
        const char * pszLocalWriteFileName, 
        const char * pszRemoteURL);

    /*********************************************************
     * ��֪���һ�β����Ĵ�����
     ********************************************************/
    int GetLastErrorCode();

    /*********************************************************
     * �����û���
     ********************************************************/
    bool SetUserName(const char * pszUserName);

    /*********************************************************
     * ��������
     ********************************************************/
    bool SetPassword(const char * pszPassword);

    /*********************************************************
     * �����û���
     ********************************************************/
    bool SetUserName(const std::string & strUserName);

    /*********************************************************
     * ��������
     ********************************************************/
    bool SetPassword(const std::string & strPassword);

private:
    /*********************************************************
     * ��֪ϵͳ�ڵ�ǰ�̵߳����һ�δ�����Ϣ
     ********************************************************/
    int GetSystemErrno();

    /*********************************************************
     * ��������
     ********************************************************/
    bool Send(const char * pszData, unsigned int uiDataLen);

    /*********************************************************
     * ��������
     ********************************************************/
    int Recv(char * pszBuffer, unsigned int uiBufferLen);

    /*********************************************************
     * ��������HTTP��������URL
     ********************************************************/
    bool ParseRemoteURL(
        const char * pszRemoteURL, std::string & strRemotePathName, 
        std::string & strHostName, unsigned short & usHostPort);

    /*********************************************************
     * ����URL������HTTP������
     ********************************************************/
    bool ParseAndConnectRemoteURL(
        const char * pszRemoteURL, std::string & strRemotePathName, 
        std::string & strHostName, unsigned short & usHostPort);

    /*********************************************************
     * ��ϳ��ϴ�����
     ********************************************************/
    bool GenerateNormalUploadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteWritePathName);

    /*********************************************************
     * ��ϳ���������
     ********************************************************/
    bool GenerateNormalDownloadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteReadPathName);

    /*********************************************************
     * ��ϳ�����֤��Ϣ���ϴ�����
     ********************************************************/
    bool GenerateAuthInfoUploadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteWritePathName, 
        const std::string & strRealm, 
        const std::string & strNonce, 
        const std::string & strResponse);

    /*********************************************************
     * ��ϳ�����֤��Ϣ����������
     ********************************************************/
    bool GenerateAuthInfoDownloadRequest(
        std::string & strRequestCmd, 
        const char * pszRemoteReadPathName, 
        const std::string & strRealm, 
        const std::string & strNonce, 
        const std::string & strResponse);

    /*********************************************************
     * ������һ��
     ********************************************************/
    bool PassToNextLine(const char *& pszData);

    /*********************************************************
     * ��ȡKey��Ӧ��ֵ
     ********************************************************/
    bool GetValueOfKey(
        const char *& pszData, 
        const char * pszKey, 
        std::string & strValue);

    /*********************************************************
     * ������realm��nonce
     ********************************************************/
    bool ParseRealmAndNonce(
        const char * pszRecvResponse, 
        std::string & strRealm, std::string & strNonce);

    /*********************************************************
     * ���ʹ���֤��Ϣ���ϴ�����
     ********************************************************/
    bool SendRequestWithAuthInfo(
        const char * pszRemotePathName, bool bUploadRequest, 
        const std::string & strRealm, const std::string & strNonce);

    /*********************************************************
     * ������������յ����ϴ���Ӧ
     ********************************************************/
    bool ParseAndHandleResponse(
        const char * pszRecvResponse, int uiRecvResponseLen, 
        const char *& pszData, unsigned int & uiDataLen, 
        const char * pszRemotePathName, bool bUploadRequest);

    /*********************************************************
     * �����ϴ�����
     ********************************************************/
    bool SendUploadRequestAndHandleResponse(
        const char * pszRemoteWritePathName);

    /*********************************************************
     * ������������
     ********************************************************/
    bool SendDownloadRequestAndHandleResponse(
        const char * pszRemoteReadPathName, 
        char * pszBuffer, unsigned int uiBufferLen, 
        const char *& pszData, unsigned int & uiDataLen);

private:
    ACE_SOCK_Stream          m_stream;               // ��HTTP������ͨ�ŵ��׽���
    bool                     m_bSocketInit;          // �Ƿ��Ѿ�������ʼ�����绷��
    std::string              m_strHostName;          // ������ӵķ�������������
    std::string              m_strHostPort;          // ������ӵķ������Ķ˿ں�
    unsigned short           m_usHostPort;           // ������ӵķ������Ķ˿ں�
    std::string              m_strUserName;          // ������ӵķ��������û���
    std::string              m_strPassword;          // ������ӵķ�����������
    std::string              m_strFileLength;        // �����ϴ����ļ��ĳ���

    int                      m_iLastErrorCode;       // ��¼���һ�β����Ĵ�����
};

NAMESPACE_CBB_END


#endif // CBB_HTTP_CLIENT_H
