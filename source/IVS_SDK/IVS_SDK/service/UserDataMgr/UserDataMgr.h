/********************************************************************
 filename    :    UserDataMgr.h
 author      :    x00192614
 created     :    2013/01/17
 description :    �û������ϴ�����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2013/01/17 ��ʼ�汾
*********************************************************************/

#ifndef __USER_DATA_MGR_H__
#define __USER_DATA_MGR_H__

#include "ivs_error.h"
#include "hwsdk.h"
#include "NSSOperator.h"
#include "SDKDef.h"

class CUserMgr;
class CUserDataMgr
{
public:
    CUserDataMgr(void);
    ~CUserDataMgr(void);

    /******************************************************************
     function   : BackupUserData
     description: �ϴ��û����ݵ�������
     input      : const IVS_CHAR * pFileName �û�����·��
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 BackupUserData(const IVS_CHAR* pFileName)const;

    /******************************************************************
     function   : RestoreUserData
     description:  �ӷ����������û�����
     input      : const IVS_CHAR * pFileName �û�����·��
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 RestoreUserData(const IVS_CHAR* pFileName);

	/******************************************************************
	 function   : GetUserDataResp
	 description: �ӷ����������û���������XML
	 input      : const IVS_CHAR * pFileName 
	 input      : const IVS_CHAR * pResp
	 output     : NA
	 return     : IVS_INT32
	*******************************************************************/
	IVS_INT32 GetUserDataResp(const IVS_CHAR* pFileName, const IVS_CHAR* pResp) const;

    /******************************************************************
     function   : SetUserMgr
     description: �����û�ָ��
     input      : CUserMgr * pUserMgr
     output     : NA
     return     : void
    *******************************************************************/
    void SetUserMgr(CUserMgr *pUserMgr);

	/******************************************************************
     function   : UploadLogo
     description: �ϴ�logoͼƬ
     input      : uiLogoType logo����
				  pLogoFile logo·��
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_INT32 UploadLogo(IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoFile)const;

	/******************************************************************
     function   : GetLogo
     description: ����logoͼƬ
     input      : uiLogoType logo����
				  pLogoMD5 MD5ֵ
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_INT32 GetLogo(IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoMD5)const;

	/******************************************************************
     function   : UploadStatement
     description: �ϴ�������Ϣ
     input      : pLanguageType ��������
				  pStatementFile ������Ϣ·��
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_INT32 UploadStatement(const IVS_CHAR* pLanguageType,const IVS_CHAR* pStatementFile)const;

	/******************************************************************
     function   : GetStatement
     description: ��ȡ������Ϣ
     input      : pLanguageType ��������
				  pStatement ������Ϣ����ֵ
     return     : IVS_INT32 �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_INT32 GetStatement(const IVS_CHAR* pLanguageType,IVS_CHAR** pStatement);
private:
    CUserMgr *m_pUserMgr;   // �û�ָ��

    /******************************************************************
     function   : GetRestoreUserDataReqXml
     description: �����û���������xml
     input      : const IVS_CHAR * cLoginId
     input      : CXml & reqXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_INT32 GetRestoreUserDataReqXml(const IVS_CHAR* cLoginId, CXml &reqXml) const;

    /******************************************************************
     function   : GetRestoreUserDataParseRspXml
     description: ���������û�����
     input      : const string & strFailMd5
     input      : const IVS_CHAR * pFileName
     input      : CXml & rspXml
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    //IVS_INT32 GetRestoreUserDataParseRspXml(const string &strFailMd5, const IVS_CHAR* pFileName, CXml &rspXml) const;

	/******************************************************************
	 function   : GetFileMd5
	 description: ��ȡ�ļ�MD5ֵ
	 input      : const IVS_CHAR * pFileName
	 output     : NA
	 return     : std::string
	*******************************************************************/
	std::string GetFileMd5(const IVS_CHAR *pFileName) const;

};
#endif


