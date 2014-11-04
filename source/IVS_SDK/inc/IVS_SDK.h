/********************************************************************
filename	: 	IVSSDK.cpp
author		:	z00201790
created		:	2012/10/23	
description	:	���� DLL Ӧ�ó���ĵ�������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/10/23 ��ʼ�汾;
*********************************************************************/

#ifndef IVS_SDK_H
#define IVS_SDK_H

#include "hwsdk.h"
#include "ivs_error.h"
//#include "SDKDef.h"

#ifdef WIN32
#ifdef  IVS_SDK_EXPORTS
#define IVS_SDK_API __declspec(dllexport)
#else
#define IVS_SDK_API __declspec(dllimport)
#endif
#else
//#define IVS_SDK_API __attribute__((visibility))
#define IVS_SDK_API __attribute__((visibility("default")))
#endif

#ifdef WIN32
#define __SDK_CALL __stdcall
#else
#define __SDK_CALL
#endif

#ifdef __cplusplus
extern "C"
{
#endif

	/**************************************************************************
	* name       : IVS_SDK_Init
	* description: ��ʼ��SDK
	* input      : NA
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Init();

	/**************************************************************************
	* name       : IVS_SDK_Cleanup()
	* description: �ͷ�SDK
	* input      : NA
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Cleanup();

	/**************************************************************************
	* name       : IVS_SDK_GetVersion()
	* description: ��ȡSDK�汾�ţ���ǰ�汾��Ϊ2.1.0.1
	* input      : NA
	* output     : NA
	* return     : �ɹ�����SDK�汾�ţ���16λ��ʾ���汾�ţ���16λ��ʾ�ΰ汾�ţ�ʧ�ܷ��ظ���������;
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVersion();


    /**************************************************************************
	* name       : IVS_SDK_GetCUVersion()
	* description: ��ȡCU�汾��
	* input      : NA
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCUVersion (IVS_INT32 iSessionID, const IVS_CU_INFO* pCUInfo, IVS_UPGRADE_INFO* pUpgradeInfo);


	/**************************************************************************
	* name       : IVS_SDK_SetEventCallBack()
	* description: �����¼��ص�����
	* input      : iSessionID        IVS_SDK_Login���صĻỰID	
	*              fEventCallBack    �û��ص�����;
	*              pUserData		 �û�����;
	* output     : NA
	* return     : �ɹ�����SDK�汾�ţ���16λ��ʾ���汾�ţ���16λ��ʾ�ΰ汾�ţ�ʧ�ܷ��ظ���������;
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetEventCallBack(IVS_INT32 iSessionID, EventCallBack fEventCallBack = NULL, void *pUserData = NULL);

	/**************************************************************************
	* name       : IVS_SDK_Login
	* description: �û���¼
	* input      : IVS_LOGIN_INFO ��½��νṹ��
                       �� pUserName �û���
	                    pPassword ����
	                    pIP       ������IP��ַ
	                    iPort     �������˿�
	                    loginType ��½����	��		0 �C���������֤ 	1 �CWindows�������֤����ǰ�û� 	2 �CWindows�������֤�������û�
	                    pDomainName   ��¼������Ϣ
	                    pMachineName �ͻ��˵�¼�Ļ�������
    * output     : pSessionId  ����sessionId SessionID��SDK�ڲ�����������0��������
                        ��Ϊ�����ӿڵ���Σ��������ַ��ʵķ�����������¼128����������
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/

    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Login(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId);


	/**************************************************************************
	* name       : IVS_SDK_LoginByTicket
	* description: �û������¼;
	* input      : IVS_LOGIN_INFO_EX ��½��νṹ��;
					(cTicketID		�û���Ʊ�ݣ����û�Ψһ��Ӧ;
					stIP			������IP��ַ;
					uiPort			�������˿�;
					uiLoginType		��½����: 0 �C���������֤;
					cDomainName		��¼������Ϣ;
					cMachineName	�ͻ��˵�¼�Ļ�����;
					uiClientType	�ͻ������ͣ��ο� IVS_CLIENT_TYPE 0-PC�ͻ���, 1-web�����, 
						2-�ƶ��ͻ���, 3-PC�ͻ����ӽ��̣�4-����ǽ����;);
    * output     : pSessionId  ����sessionId SessionID��SDK�ڲ�����������0��������
                        ��Ϊ�����ӿڵ���Σ��������ַ��ʵķ�����������¼128��������;
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LoginByTicket(IVS_LOGIN_INFO_EX* pLoginReqInfoEx, IVS_INT32* pSessionId);


    /**************************************************************************
	* name       : IVS_SDK_WinUserLogin
	* description: �û���¼
	* input      : IVS_LOGIN_INFO ��½��νṹ��
	                   �� pUserName �û���
	                      pPassword ����
	                      pIP       ������IP��ַ
	                      iPort     �������˿�
	                      loginType ��½����	��		0 �C���������֤ 	1 �CWindows�������֤����ǰ�û� 	2 �CWindows�������֤�������û�
	                      pDomainName   ��¼������Ϣ
	                      pMachineName �ͻ��˵�¼�Ļ�������
    * output     : pSessionId  ����sessionId SessionID��SDK�ڲ�����������0��������
                        ��Ϊ�����ӿڵ���Σ��������ַ��ʵķ�����������¼128����������
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_WinUserLogin(IVS_LOGIN_INFO* pLoginReqInfo, IVS_INT32* pSessionId);

    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_BackupUserData(IVS_INT32 iSessionID, const IVS_CHAR* pUserDataFile);

	/**************************************************************************
	* name       : IVS_SDK_GetLoginRspInfo
	* description: ��ȡ��¼��Ӧ��Ϣ
	* input      : iSessionID     ��¼���ص�SessionID
	* output     : pLoginRspInfo  ��¼�ɹ����ص���Ϣ�������¼�ɹ���Ӧ��Ϣ�ṹ��
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	//IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetLoginRspInfo(IVS_INT32 iSessionID, LOGIN_RSP_INFO* pLoginRspInfo);

    /**************************************************************************
	* name       : IVS_SDK_GetUserID
	* description: ��ȡ��½�û����û�ID
	* input      : iSessionID     ��¼���ص�SessionID
	* output     : puiUserID  �û�ID
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	**************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserID(IVS_INT32 iSessionID, IVS_UINT32* puiUserID);

	/**************************************************************************
	* name       : IVS_SDK_Logout
	* description: �û�ע��
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Logout(IVS_INT32 iSessionID);

    /******************************************************************
    function : IVS_SDK_AddUser
    description: ����û�
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_USER_INFO * pUserInfo     �û���Ϣ�ṹ��
    output : IVS_UINT32 * puiUserID             ��ӳɹ��󷵻��û�ID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo, IVS_UINT32* puiUserID);

    /******************************************************************
    function : IVS_SDK_ModifyUser
    description: �޸��û�
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_USER_INFO * pUserInfo     �û���Ϣ�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUser(IVS_INT32 iSessionID, const IVS_USER_INFO* pUserInfo);

    /******************************************************************
    function : IVS_SDK_DeleteUser
    description: ɾ���û�
    input : IVS_INT32 iSessionID               ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode       �����
    output : IVS_UINT32 uiUserID               �û�ID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    /******************************************************************
    function : IVS_SDK_GetUserList
    description: ��ѯ�û��б�
    input : IVS_INT32 iSessionID                            ��¼�ɹ���ĻỰID
    input : const IVS_QUERY_UNIFIED_FORMAT * pUnifiedQuery  ͨ�ò�ѯ����
    output : IVS_USER_INFO_LIST * pUserList                 �û���Ϣ�б�ṹ��
    input : IVS_UINT32 uiBufferSize                         �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_USER_INFO_LIST* pUserList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_GetOnlineUser
    description: ��ѯ�����û��б�
    input : IVS_INT32 iSessionID                            ��¼�ɹ���ĻỰID
    input : const IVS_QUERY_UNIFIED_FORMAT * pUnifiedQuery  ͨ�ò�ѯ����
    output : IVS_ONLINE_USER_LIST * pOnlineUserList         �����û���Ϣ�б�ṹ��
    input : IVS_UINT32 uiBufferSize                         �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ONLINE_USER_LIST* pOnlineUserList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_GetUserInfo
    description: ��ѯ�û���Ϣ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserID             �û�ID
    output : IVS_USER_INFO * pUserInfo      �û���Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_INFO* pUserInfo);

    /******************************************************************
    function : IVS_SDK_LockUser
    description: ����/�����û�
    input : IVS_INT32 iSessionID        	��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserID         	�û�ID
    input : IVS_BOOL bLock              	�Ƿ������û���0-������1-����
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_BOOL bLock);

    /******************************************************************
    function : IVS_SDK_SetUserDeviceList
    description: �����û��豸�б�
    input : IVS_INT32 iSessionID                            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode                    �����
    input : IVS_UINT32 uiUserID                             �û�ID
    input : const IVS_USER_DEVICE_INFO_LIST * pUserDevList  �û��б�ṹ��
    input : IVS_UINT32 uiBufferSize                         �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserDeviceList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_GetUserDeviceList
    description: ��ȡ�û��豸�б�
    input : IVS_INT32 iSessionID                            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode                    �����
    input : IVS_UINT32 uiUserID                             �û�ID
    input : const IVS_INDEX_RANGE * pIndexRange             ��ҳ��Ϣ
    output : IVS_USER_DEVICE_INFO_LIST * pUserDevList       �û��б�ṹ��
    input : IVS_UINT32 uiBufferSize                         �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserDeviceList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_ChangePWD
    description: �޸�����
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pOldPWD   ������
    input : const IVS_CHAR * pNewPWD   ������
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ChangePWD(IVS_INT32 iSessionID, const IVS_CHAR* pOldPWD, const IVS_CHAR* pNewPWD);

    /******************************************************************
    function : IVS_SDK_ResetPWD
    description: ��������
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : const IVS_UINT32 iUserID        �û�ID
    input : const IVS_CHAR * pNewPWD        �û�������
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID, const IVS_CHAR* pNewPWD);

    /******************************************************************
    function : IVS_SDK_CheckPWDByRole
    description: У�������Ƿ������˺Ź���
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : const IVS_UINT32 uiRoleID       ��ɫID
    input : const IVS_CHAR * pPWD           �û����õ�����
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckPWDByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, const IVS_CHAR* pPWD);

    /******************************************************************
    function : IVS_SDK_GetAutoCreatePWD
    description: ���ݽ�ɫ��ȡ�����˺Ź��������
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiRoleID             ��ɫID
    input : IVS_CHAR* pLoginName            �û���¼��
    output : IVS_CHAR* pNewPWD              �����ַ���
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAutoCreatePWD(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, IVS_CHAR* pNewPWD, IVS_UINT32 uiPWDLen);

    /******************************************************************
    function : IVS_SDK_GetPWDStrength
    description: ��ȡ����ǿ��
    input : cosnt IVS_CHAR * pPWD       ����
    input : IVS_UINT32 * uiPWDStrength  ����ǿ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDStrength(const IVS_CHAR* pPWD, IVS_UINT32* uiPWDStrength);

    /******************************************************************
    function : IVS_SDK_GetPWDExpireInfo
    description: ��ȡ���������Ϣ
    output : IVS_PWD_EXPIRE_INFO * pPWDExpireInfo    ���������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPWDExpireInfo(IVS_INT32 iSessionID, IVS_PWD_EXPIRE_INFO* pPWDExpireInfo);

    /******************************************************************
    function : IVS_SDK_SignOutUser
    description: �����û�IDǩ���û������лỰ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserID             �û�ID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    /******************************************************************
    function : IVS_SDK_SignOutUserByRole
    description: ���ݽ�ɫIDǩ���û������лỰ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiRoleID             ��ɫID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutUserByRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    /******************************************************************
    function : IVS_SDK_SignOutOnlineUser
    description: ǩ�������û�
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserOnlineID       �����û��ĻỰID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SignOutOnlineUser(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserOnlineID);

    /******************************************************************
    function : IVS_SDK_AddUserGroup
    description: �����û���
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_USER_GROUP * pUserGroup   �û�����Ϣ�ṹ��
    input : IVS_UINT32 * puiUserGroupID          �û���ID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup, IVS_UINT32* puiUserGroupID);

    /******************************************************************
    function : IVS_SDK_DeleteUserGroup
    description: ɾ���û���
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserGroupID        �û���ID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserGroupID);

    /******************************************************************
    function : IVS_SDK_ModifyUserGroup
    description: �޸��û���
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_USER_GROUP * pUserGroup   �û�����Ϣ�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyUserGroup(IVS_INT32 iSessionID, const IVS_USER_GROUP* pUserGroup);

    /******************************************************************
    function : IVS_SDK_MoveUserGroup
    description: �ƶ��û���
    input : IVS_INT32 iSessionID                        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode                �����
    input : IVS_UINT32 uiTargetGroupID                  �ƶ������û���ID
    input : const IVS_USER_GROUP_LIST * pUserGroupList  �û����б�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveUserGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiTargetGroupID, const IVS_USER_GROUP_LIST* pUserGroupList);

    /******************************************************************
    function : IVS_SDK_GetUserGroupList
    description: ��ѯ�û����б�
    input : IVS_INT32 iSessionID                    ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode            �����
    input : const IVS_INDEX_RANGE * pIndexRange     ��ҳ�����ṹ��
    output : IVS_USER_GROUP_LIST * pUserGroupList   �û����б�ṹ��
    input : IVS_UINT32 uiBufferSize                 �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserGroupList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_GROUP_LIST* pUserGroupList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_AddRole
    description: ���ӽ�ɫ
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_ROLE_INFO * pRoleInfo     ��ɫ��Ϣ�ṹ��
    input : IVS_UINT32 * puiRoleID              ��ɫID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID);

    /******************************************************************
    function : IVS_SDK_DeleteRole
    description: ɾ����ɫ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiRoleID             ��ɫID
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRole(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    /******************************************************************
    function : IVS_SDK_ModifyRole
    description: �޸Ľ�ɫ
    input : IVS_INT32 iSessionID                ��¼�ɹ���ĻỰID
    input : const IVS_ROLE_INFO * pRoleInfo     ��ɫ��Ϣ�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRole(IVS_INT32 iSessionID, const IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function : IVS_SDK_GetRoleList
    description: ��ѯ��ɫ�б�
    input : IVS_INT32 iSessionID                    ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode            �����
    input : const IVS_INDEX_RANGE * pIndexRange     ��ҳ�����ṹ��
    output : IVS_ROLE_INFO_LIST * pRoleInfoList     ��ɫ�б�ṹ��
    input : IVS_UINT32 uiBufferSize                 �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_GetRoleInfoByRoleID
    description: ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiRoleID             ��ɫID
    output : IVS_ROLE_INFO * pRoleInfo      ��ɫ��Ϣ�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByRoleID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function : IVS_SDK_GetRoleInfoByUserID
    description: �����û�ID��ȡ��ɫ��ϸ��Ϣ
    input : IVS_INT32 iSessionID            ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode    �����
    input : IVS_UINT32 uiUserID             ��ɫID
    output : IVS_ROLE_INFO * pRoleInfo      ��ɫ��Ϣ�ṹ��
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRoleInfoByUserID(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo);

    /******************************************************************
    function : IVS_SDK_GetDefaultRoleAccountRule
    description: ��ȡĬ���˻�����
    input : IVS_INT32 iSessionID                        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode                �����
    output : IVS_ROLE_ACCOUNT_RULE * pRoleAccountRule   �˻�����
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDefaultRoleAccountRule(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule);

    /******************************************************************
    function : IVS_SDK_GetOperationRightGroup
    description: ��ȡ�û�Ȩ����Ϣ
    input : IVS_INT32 iSessionID                                ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pDomainCode                        �����
    output : IVS_OPERATION_RIGHT_GROUP_LIST * pRightGroupList   Ȩ�����б�
    input : IVS_UINT32 uiBufferSize                             �����С
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationRightGroup(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddPlatformRecordPlan
	* description: ����ƽ̨¼��ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDeviceList,const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddPlatformRecordPlan
	* description: �޸�ƽ̨¼��ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		const IVS_RECORD_PLAN* pRecordPlan,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddPlatformRecordPlan
	* description: ɾ��ƽ̨¼��ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordPlan(IVS_INT32 iSessionID,IVS_UINT32 uiDeviceNum,const IVS_DEVICE_CODE* pDeviceList,
		IVS_UINT32 uiRecordMethod,IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddPlatformRecordPlan
	* description: ��ѯƽ̨¼��ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPlan(IVS_INT32 iSessionID,const char* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_RECORD_PLAN *pRecordPlan);

    /******************************************************************
    function : IVS_SDK_GetCameraPlanInfo
    description: ��ѯ������ƻ�������Ϣ
    input : IVS_INT32 iSessionID                             ��¼�ɹ���ĻỰID
    input : IVS_UINT32 uiPlanType                            ¼������
    output : IVS_CHAR** pRspXml                              ��Ӧ��Ϣ
    return : �ɹ�����0��ʧ�ܷ��ظ���������
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraPlanInfo(IVS_INT32 iSessionID, IVS_UINT32 uiPlanType, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_AddBookmark
	* description: ����¼����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_CHAR* pNVRCode,
		const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID);

	/*************************************************************************
	* name       : IVS_SDK_ModifyBookmark
	* description: �޸�¼����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName);
	/*************************************************************************
	* name       : IVS_SDK_DeleteBookmark
	* description: ɾ��¼����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID);

		/*************************************************************************
	* name       : IVS_SDK_AddRealPlayBookmark
	* description: ����ʵ����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRealPlayBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,
		const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID);

	/*************************************************************************
	* name       : IVS_SDK_ModifyRealPlayBookmark
	* description: �޸�ʵ����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRealPlayBookmark(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName);
	/*************************************************************************
	* name       : IVS_SDK_DeleteRealPlayBookmark
	* description: ɾ��ʵ����ǩ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRealPlayBookmark(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID);

	/*************************************************************************
	* name       : IVS_SDK_GetBookmarkList
	* description: ��ѯ¼����ǩ�б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	               pUnifiedQuery ͨ�ò�ѯ�ṹ
				   pBookmarkList ����¼����ǩ��ѯ�б�
				   uiBufSize �����ڴ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetBookmarkList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize);

	/*************************************************************************
	* name       : IVS_SDK_QueryRecordList
	* description: ��ѯ¼���б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	               pUnifiedQuery ͨ�ò�ѯ�ṹ
				   pRecordInfoList ����¼���ѯ�ṹ�б�
				   uiBufferSize �����ڴ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryRecordList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_INFO_LIST* pRecordInfoList,IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_GetRecordPolicyByTime
	* description: ��ѯƽ̨¼�����
	* input      : pCameraCode �����ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name       : IVS_SDK_SetRecordPolicyByTime
	* description: �޸�¼�����
	* input      : pRecordPolicy �����ID
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetRecordPolicyByTime(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_POLICY_TIME* pRecordPolicy);

	/*************************************************************************
	* name       : IVS_SDK_GetRecordList
	* description: ��ѯ¼���б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	               pCameraCode �����ID
				   iRecordMethod ¼��洢λ��
				   pTimeSpan ʱ�����Ϣ
				   pIndexRange ��ҳ��Ϣ
				   pRecordList ����¼���б�
				   iBufSize �ڴ����ռ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA

	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordList(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordMethod,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,IVS_RECORD_INFO_LIST* pRecordList,IVS_UINT32 uiBufSize);


	/*************************************************************************
	* name       : IVS_SDK_SubscribeAlarmQuery
	* description: �鿴�澯�����б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarmQuery(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_SubscribeAlarm
	* description: �����澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SubscribeAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_AddHelpInfo
	* description: �����澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddHelpInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *pHelpInfo);

	/*************************************************************************
	* name       : IVS_SDK_ModifyHelpInfo
	* description: �޸ĸ澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyHelpInfo(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_HELP_INFO *pHelpInfo);

	/*************************************************************************
	* name       : IVS_SDK_DeleteHelpInfo
	* description: ɾ���澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteHelpInfo(IVS_INT32 iSessionID, IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId, const IVS_CHAR* pDomainCode);

	/*************************************************************************
	* name       : IVS_SDK_QueryHelpInfo
	* description: ��ѯ�澯��������
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryHelpInfo(IVS_INT32 iSessionID, IVS_UINT32 uiAlarmHelpSourceType,IVS_UINT32 uiAlarmHelpSourceId,const IVS_CHAR* pDomainCode, IVS_HELP_INFO* pHelpInfoResult);

    /******************************************************************
     function   : IVS_SDK_GetHelpInfoText
     description: ��ѯ������Ϣ����
     input      : IVS_INT32 iSessionID
     input      : const IVS_CHAR * pAlarmInCode
     input      : const IVS_CHAR * pAlarmType
     input      : IVS_CHAR * pHelpInfo
     input      : IVS_UINT32 uiBufferSize
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetHelpInfoText(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmInCode, const IVS_CHAR* pAlarmType, IVS_CHAR* pHelpInfo, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmTypeList
	* description: ��ѯ�Զ���澯�����б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmTypeList (IVS_INT32 iSessionID, 
        const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_TYPE_LIST* pAlarmTypeList, IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_SetAlarmTypeLevel
	* description: ��ѯ�澯���ͼ���
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetAlarmTypeLevel(IVS_INT32 iSessionID,  
        const IVS_ALARM_TYPE_LIST* pAlarmTypeList);


	/*************************************************************************
	* name       : IVS_SDK_AddAlarmLevel
	* description: �����澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLevel(IVS_INT32 iSessionID, const IVS_ALARM_LEVEL *pAlarmLevel,IVS_UINT32* pAlarmLevelID);

	/*************************************************************************
	* name       : IVS_SDK_ModifyAlarmLevel
	* description: �޸ĸ澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLevel(IVS_INT32 iSessionID, const IVS_ALARM_LEVEL *pAlarmLevel);

	/*************************************************************************
	* name       : IVS_SDK_DeleteAlarmLevel
	* description: ɾ���澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLevel(IVS_INT32 iSessionID, const IVS_UINT32 uiAlarmLevelId);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmLevelList
	* description: ��ѯ�澯�����б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevelList(IVS_INT32 iSessionID,IVS_UINT32 uiMaxAlarmLevelNum,IVS_UINT32* puiAlarmLevelNum,IVS_ALARM_LEVEL* pAlarmLevel,IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddAlarmArea
	* description: ��������
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR** pRsp);

	/*************************************************************************
	* name       : IVS_SDK_ModifyAlarmArea
	* description: �޸ķ���
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_DeleteAlarmArea
	* description: ɾ������
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmArea(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmAreaList
	* description: �鿴�����б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmAreaInfo
	* description: �鿴��������
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmAreaInfo(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId,IVS_CHAR** pAlarmArea);

	/*************************************************************************
	* name       : IVS_SDK_StartAlarmAreaGuard
	* description: �ֶ�����
	* input      : iSessionID ��¼�ɹ���ĻỰID;uiAlarmAreaId,����ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name       : IVS_SDK_SubscribeAlarmQuery
	* description: �ֶ�����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmAreaGuard(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name       : IVS_SDK_AllocAreaGuardPlan
	* description: ���������ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID��pAlarmAreaPlan�����������ƻ��ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     : wulixia
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AllocAreaGuardPlan(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_ModifyAreaGuardPlan
	* description: �޸Ĳ����ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID;  pAlarmAreaPlan�����������ƻ��ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAreaGuardPlan(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_DeleteAreaGuardPlan
	* description: ɾ�������ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAreaGuardPlan(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId);

	/*************************************************************************
	* name       : IVS_SDK_GetAreaGuardPlan
	* description: �鿴�����ƻ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAreaGuardPlan(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode,IVS_UINT32 uiAlarmAreaId, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_AddAlarmLinkage
	* description: �����澯��������
	* input      : iSessionID ��¼�ɹ���ĻỰID;pReqXml��������Ϣ
	* output     : pRspXml,��Ӧ��Ϣ����������������ID
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddAlarmLinkage(IVS_INT32 iSessionID,	const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_ModifyAlarmLinkage
	* description: �޸ĸ澯��������
	* input      : iSessionID ��¼�ɹ���ĻỰID��pReqXml��������Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_DeleteAlarmLinkage
	* description: ɾ���澯��������
	* input      : iSessionID ��¼�ɹ���ĻỰID;uiAlarmLinkageID���澯��������ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteAlarmLinkage(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmlinkageList
	* description: �鿴�澯���������б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmlinkageList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmLinkage
	* description: ��ѯ�澯��������
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkage (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_ConfirmAlarm
	* description: �澯ȷ��
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmConfirm(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name       : IVS_SDK_CancelAlarm
	* description: �澯����
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCancel(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo);

	/*************************************************************************
	* name       : IVS_SDK_AlarmCommission
	* description: �澯��Ȩ
	* input      : iSessionID ��¼�ɹ���ĻỰID;pAlarmCommission,�澯��Ȩ�ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmCommission (IVS_INT32 iSessionID, const IVS_CHAR* pAlarmInCode, const IVS_ALARM_COMMISSION* pAlarmCommission);

	/*************************************************************************
	* name       : IVS_SDK_UserAlarmAuthorization
	* description: ��Ȩ�澯�û���֤
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UserAuthentication(IVS_INT32 iSessionID, const IVS_CHAR *pPWD);

	/*************************************************************************
	* name       : IVS_SDK_AlarmReport
	* description: �澯��Ϣ�ϱ����澯���ͣ�
	* input      : iSessionID ��¼�ɹ���ĻỰID��pAlarmReport�� ��Ϣ�ϱ��ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AlarmReport(IVS_INT32 iSessionID, const IVS_ALARM_REPORT* pAlarmReport);

	/*************************************************************************
	* name       : IVS_SDK_ManualAlarm
	* description: �������澯�ϱ�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ManualAlarm(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp);

	/*************************************************************************
	* name       : IVS_SDK_StartAlarmOut
	* description: ��������������澯
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name       : IVS_SDK_StopAlarmOut
	* description: ֹͣ����������澯
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopAlarmOut(IVS_INT32 iSessionID, const IVS_CHAR* pAlarmOutCode);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmEventInfo
	* description: ��ѯ�澯��Ϣ
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventInfo(IVS_INT32 iSessionID, IVS_UINT64 ullAlarmEventID, const IVS_CHAR* pAlarmInCode, IVS_ALARM_EVENT* pAlarmEvent);

	/*************************************************************************
	* name       : IVS_SDK_GetAlarmEventList
	* description: ��ѯ��ʷ�澯��Ϣ�б�
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmEventList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ALARM_EVENT_LIST* pAlarmEventList, IVS_UINT32 uiBufferSize);


	/*************************************************************************
	* name       : IVS_SDK_AddDevice
	* description: �����豸
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-02
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDevice(IVS_INT32 iSessionID, 
                                                       const IVS_CHAR* pDomainCode, 
                                                       IVS_UINT32 uiDeviceNum, 
                                                       const IVS_DEVICE_OPER_INFO* pDevList, 
                                                       IVS_DEVICE_OPER_RESULT_LIST* pResultList, 
                                                       IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_DeleteDevice
	* description: ɾ���豸
	* input      : iSessionID ��¼�ɹ���ĻỰID, pDomainCode����� uiDeviceNum ɾ�����豸��Ŀ
	               pDevCodeList �豸�����б� pResultList �豸������������б� uiResultBufferSize �����С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��wulixia
	* remark     : NA
	* history    : 2012-11-21
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDevice(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,IVS_UINT32 uiDeviceNum,
		const IVS_DEVICE_CODE* pDevCodeList, IVS_RESULT_LIST* pResultList,IVS_UINT32 uiResultBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_QueryDeviceList
	* description: ��ѯ�豸�б�
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	               2013-1-10 �ָ�pDomainCode
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType, 
		const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
		IVS_VOID* pBuffer, 
		IVS_UINT32 uiBufferSize);
    
	/******************************************************************
	function : IVS_SDK_GetDeviceList
	description: ��ȡ�豸�б�
	author: fengjiaji f00152768
	input : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID
	input : IVS_UINT32 uiDeviceType �豸���ͣ�ֵ�ο�IVS_DEVICE_TYPE
	input : const IVS_INDEX_RANGE * pIndexRange ��ҳ��Ϣ
	output : IVS_VOID * pDeviceList �豸�б�ָ��
	input : IVS_UINT32 uiBufferSize �����С
	return : IVS_SDK_API IVS_INT32
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceList(
		IVS_INT32 iSessionID,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_GetDeviceInfo
	* description: ��ѯ���豸��ϸ��Ϣ
	* input      : 
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��gWX151718
	* remark     : NA
	* history    : 2012-11-21
	*************************************************************************/		
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceInfo(IVS_INT32 iSessionID, 
                                                           const IVS_CHAR* pDomainCode,
                                                           const IVS_CHAR* pDevCode, 
                                                           IVS_DEVICE_INFO* pDeviceInfo);

	/*************************************************************************
	* name       : IVS_SDK_AddDeviceGroup
	* description: �����豸��
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceGroup (IVS_INT32 iSessionID, const IVS_DEV_GROUP* pDevGroup,IVS_UINT32* puiDevGroupID);
	
	/*************************************************************************
	* name       : IVS_SDK_GetDeviceGroup
	* description: ��ѯ�豸���б�
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/		
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceGroup(IVS_INT32 iSessionID,
                                                            const IVS_CHAR* pDomainCode,
                                                            const IVS_CHAR* pDevGroupCode,
                                                            IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
                                                            IVS_UINT32 uiBufferSize);

    /******************************************************************
    function : IVS_SDK_AddDeviceToGroup
    description: �����豸���µ��豸
    input : IVS_INT32 iSessionID
            const IVS_CHAR * pTargetDomainCode
            IVS_UINT32 uiTargetDevGroupID
            const IVS_DEV_GROUP_CAMERA_LIST * pGroupCameraList
            IVS_UINT32 uiBufferSize
    output : NA
    return : IVS_INT32
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddDeviceToGroup( IVS_INT32 iSessionID, 
                                        const IVS_CHAR* pTargetDomainCode,
                                        IVS_UINT32 uiTargetDevGroupID,
                                        IVS_UINT32 uiDeviceNum,
                                        const IVS_DEV_GROUP_CAMERA* pGroupCameraList);

    /*************************************************************************
    * name       : DeleteDeviceGroup
    * description: ɾ���豸��
    * input      :  iSessionID ��½�ɹ����ص�sessionId�����ڱ�ʶΨһ�û�
                       pDeviceGroupList, �豸���б�
                      uiGroupNum  �豸��Num
                      uiResultBufferSize �����С
    * output     : pResultList ɾ�����
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * author     ��xiongfeng 00192614
    * remark     : NA
    * history    : 2012-11-06
    *************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceGroup( IVS_INT32 iSessionID,
                                                                const IVS_CHAR* pDomainCode,
                                                                IVS_UINT32 uiDevGroupID);

	/*************************************************************************
	* name       : IVS_SDK_GetDeviceConfig
	* description: ��ѯ�豸����
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
	//IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig(IVS_INT32 iSessionID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp);

    /**************************************************************************
    * name       : IVS_SDK_StartRealPlay
    * description: ��ʼʵʱ���������������ʱ�ɶ�ε���, 
                   �ڿ���һ��ʵ����ʱ���ϵڶ�����������ô��ڣ�
                   ������ֹͣ��һ��ʵ�����ٲ��ŵڶ�������ͷ��ʵ�����ӿ���Ҫʵ����Ƶ�ؼ�֡���롣
    * input      : iSessionID ��¼�ɹ���ĻỰID
                   pRealplayPara ʵ�����������ý������ṹ��
                   pCamerCode ����ͷ����
                   hWnd       ���ھ��
    * output     : pHandle    ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng00193168
	* remark     : NA
	* history    : 2012-11-09
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlay(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, HWND hWnd, IVS_ULONG* pHandle);
#endif
	/**************************************************************************
    * name       : IVS_SDK_StartRealPlayCBRaw
    * description: ��ʼʵʱ���������������ʱ�ɶ�ε���, 
                   �ڿ���һ��ʵ����ʱ���ϵڶ�����������ô��ڣ�
                   ������ֹͣ��һ��ʵ�����ٲ��ŵڶ�������ͷ��ʵ�����ӿ���Ҫʵ����Ƶ�ؼ�֡���롣
    * input      : iSessionID           ��¼�ɹ���ĻỰID
                   pRealplayPara        ʵ�����������ʵ�������ṹ��
                   pCamerCode           ����ͷ����
                   fRealPlayCallBackRaw ʵʱ����ص�����
				   pUserData            �û�����
    * output     : pHandle              ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng00193168
	* remark     : NA
	* history    : 2012-11-12
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBRaw(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, 
                                                                RealPlayCallBackRaw fRealPlayCallBackRaw, void* pUserData, IVS_ULONG* pHandle );

	/**************************************************************************
    * name       : IVS_SDK_StartRealPlayCBFrame
    * description: ��ʼʵʱ���������������ʱ�ɶ�ε���, 
                   �ڿ���һ��ʵ����ʱ���ϵڶ�����������ô��ڣ�
                   ������ֹͣ��һ��ʵ�����ٲ��ŵڶ�������ͷ��ʵ�����ӿ���Ҫʵ����Ƶ�ؼ�֡���롣
    * input      : iSessionID               ��¼�ɹ���ĻỰID
                   pRealplayPara            ʵ�����������ʵ�������ṹ��
                   pCamerCode               ����ͷ����
                   fRealPlayCallBackFrame   ʵʱ����ص�����
				   pUserData                �û�����
    * output     : pHandle                  ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng00193168
	* remark     : NA
	* history    : 2012-11-12
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayCBFrame(IVS_INT32 iSessionID, IVS_REALPLAY_PARAM* pRealplayPara, const IVS_CHAR* pCameraCode, 
                                                                  RealPlayCallBackFrame fRealPlayCallBackFrame, void* pUserData, IVS_ULONG* pHandle);
#endif

	/**************************************************************************
    * name       : IVS_SDK_StopRealPlay
    * description: ֹͣʵʱ�����
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               ulHandle   ���ž��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng00193168
	* remark     : NA
	* history    : 2012-11-12
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlay( IVS_INT32 iSessionID, IVS_ULONG ulHandle);

    /**************************************************************************
    * name       : IVS_SDK_AddPlayWnd
    * description: 
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               ulPlayHandle   ���ž��
                   hWnd       ���ھ��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��
	* remark     : NA
	* history    : 2012-12-15
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPlayWnd(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd);
#endif

	/**************************************************************************
    * name       : IVS_SDK_DelPlayWnd
    * description: 
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               ulPlayHandle   ���ž��
                   hWnd       ���ھ��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��
	* remark     : NA
	* history    : 2012-12-15
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPlayWnd(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd);
#endif

	/**************************************************************************
    * name       : IVS_SDK_AddPlayWindowPartial
    * description: 
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               ulPlayHandle   ���ž��
                   hWnd       ���ھ��
				   pPartial �Ŵ󴰿ڵİٷֱ�
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��
	* remark     : NA
	* history    : 2013-3-8
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPlayWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial);
#endif
	/**************************************************************************
    * name       : IVS_SDK_UpdateWindowPartial
    * description: 
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               ulPlayHandle   ���ž��
                   hWnd       ���ھ��
				   pPartial �Ŵ󴰿ڵİٷֱ�
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��
	* remark     : NA
	* history    : 2013-3-8
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UpdateWindowPartial(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, void *pPartial);
#endif
	/**************************************************************************
    * name       : IVS_SDK_GetOperationLog
    * description: ��ѯ������־��
    * input      : iSessionID ��¼�ɹ���ĻỰID
	               iPlayHandle ���ž��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��
	* remark     : NA
	* history    : 2012-11-14
    **************************************************************************/

	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLog(IVS_INT32 iSessionID,const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_OPERATION_LOG_LIST* pResOperLogList, IVS_UINT32 uiBufSize);

#ifdef WIN32
	/**************************************************************************
    * name			: IVS_SDK_GetOperationLogEx
    * description: ��չ��ѯ������־
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pUnifiedQuery ͳһͨ�ò�ѯ����
	* input			: pOptLogQueryEx ��չ��ѯ������־����
	* input			: uiBufSize �¼����ݴ�С
    * output			: pResOperLogList ������־��ѯ��ѯ���
    * return			: �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author			: zwx211831 zhuohaofan
	* remark		: NA
	* history			: 2014-5-6
    **************************************************************************/

	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetOperationLogEx(IVS_INT32 iSessionID,
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,
		const IVS_QUERY_OPTLOG_EX *pOptLogQueryEx,
		IVS_OPERATION_LOG_LIST* pResOperLogList,
		const IVS_UINT32 uiBufSize);
#endif

	/**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBack
    * description: ��ʼ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
    * output     : pHandle         ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBack(IVS_INT32 iSessionID,
                                                                   const IVS_CHAR* pCameraCode,
                                                                   const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                   HWND hWnd,
                                                                   IVS_ULONG* pHandle);
#endif

   /**************************************************************************
    * name       : IVS_SDK_StopPlatformPlayBack
    * description: ֹͣ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   ulPlayHandle    ���ž��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBack(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBackByIP
    * description: ��ʼ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
    * output     : pHandle         ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackByIP(IVS_INT32 iSessionID,
																	const IVS_CHAR* pCameraCode,
																	const IVS_PLAYBACK_PARAM* pPlaybackParam,
																	const IVS_MEDIA_ADDR* pMediaAddrDst,
																	IVS_MEDIA_ADDR* pMediaAddrSrc,
																	IVS_ULONG* pHandle);

   /**************************************************************************
    * name       : IVS_SDK_StopPlatformPlayBackByIP
    * description: ֹͣ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   ulPlayHandle    ���ž��
    * output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformPlayBackByIP(IVS_INT32 iSessionID,IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackPauseByIP
    * description: ��ͣƽ̨¼��ط�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
    * output			: NA
    * return			: �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackPauseByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	/**************************************************************************
    * name			: IVS_SDK_PlatformPlayBackResumeByIP
    * description: �ָ�ƽ̨¼��ط�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformPlayBackResumeByIP(IVS_INT32 iSessionID,
																	IVS_ULONG ulPlayHandle);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackSpeedByIP
    * description: ����ƽ̨¼�������ط��ٶ�
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
							: fSpeed	�����ٶ�
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeedByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_FLOAT fSpeed);

	 /**************************************************************************
    * name			: IVS_SDK_SetPlayBackTimeByIP
    * description: ����ƽ̨¼�������ط�ʱ��
    * input			: iSessionID	�ỰID
							: ulPlayHandle	���ž��
							: uiTime	�϶��߶ȣ���λ�룬������������ʱ��
    * output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * remark		: NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTimeByIP(IVS_INT32 iSessionID, 
		IVS_ULONG ulPlayHandle, 
		IVS_UINT32 uiTime);

	/**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBack
    * description: ��ʼ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
    * output     : pHandle         ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 IVS_SDK_StartLocalPlayBack (
		                       IVS_INT32 iSessionID,
		                       const IVS_CHAR* pFileName,
		                       const IVS_LOCAL_PLAYBACK_PARAM* pPlaybackParam,
		                       HWND hWnd, 
							   IVS_ULONG* pHandle);
#endif
	
	/**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBack
    * description: ��ʼ�ͻ���ƽ̨¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
    * output     : pHandle         ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 IVS_SDK_StopLocalPlayBack(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);
#endif

#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartDisasterRecoveryPlayBack (
        IVS_INT32 iSessionID,
        const IVS_CHAR* pCameraCode,
        const IVS_PLAYBACK_PARAM* pPlaybackParam,
        HWND hWnd,
        IVS_ULONG* pHandle);
#endif
#ifdef WIN32
    IVS_SDK_API IVS_INT32  __SDK_CALL  IVS_SDK_StopDisasterRecoveryPlayBack(
        IVS_INT32 iSessionID,
        IVS_ULONG ulPlayHandle);
#endif


    /**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBackCBRaw
    * description: ��ʼƽ̨¼��ط�(�������ص�)
    * input      : iSessionID           ��¼�ɹ���ĻỰID
                   pMediaPara           ý�����
                   pCameraCode          ���������
                   pTimeSpan            �ط�ʱ���
                   fSpeed               ��������
                   fPlayBackCallBackRaw ¼��ط��������ص�����
                   pUserData            �û�����
    * output     : pHandle              ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBRaw(IVS_INT32 iSessionID,
																		const IVS_CHAR* pCameraCode,
																		const IVS_PLAYBACK_PARAM* pPlaybackParam,
																		PlayBackCallBackRaw fPlayBackCallBackRaw, 
																		void* pUserData,
																		IVS_ULONG* pHandle);


	/**************************************************************************
    * name       : IVS_SDK_StartPlatformPlayBackCBFrame
    * description: ��ʼƽ̨¼��ط�(֡�����ص�)
    * input      : iSessionID             ��¼�ɹ���ĻỰID
                   pMediaPara             ý�����
                   pCameraCode            ���������
                   pTimeSpan              �ط�ʱ���
                   fSpeed               ��������

                   fPlayBackCallBackFrame ¼��طŽ����֡�����ص�����
                   pUserData              �û�����
    * output     : pHandle                ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformPlayBackCBFrame(IVS_INT32 iSessionID,
		   																  const IVS_CHAR* pCameraCode,
																		  const IVS_PLAYBACK_PARAM* pPlaybackParam,
																		  PlayBackCallBackFrame fPlayBackCallBackFrame, 
																		  void* pUserData,
																		  IVS_ULONG* pHandle);
#endif


    /******************************************************************
     function   : IVS_SDK_PlayBackPause
     description: ��ͣ�ط�
     input      : IVS_ULONG ulPlayHandle
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackPause(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /******************************************************************
     function   : IVS_SDK_PlayBackResume
     description: �ָ��ط�
     input      : IVS_ULONG ulPlayHandle
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackResume(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

	/******************************************************************
	 function   : IVS_SDK_GetPlayBackStatus
	 description: ��ȡ����״̬
	 input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulPlayHandle
	 output     : IVS_BOOL * pbPause
	 return     : IVS_SDK_API IVS_INT32 __SDK_CALL
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackStatus(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_BOOL* pbPause);

    /******************************************************************
     function   : IVS_SDK_PlayBackFrameStepForward
     description: ��֡���
     input      : IVS_ULONG ulPlayHandle
     output     : NA
     return     : IVS_INT32
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepForward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);


	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlayBackFrameStepBackward(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);

    /******************************************************************
     function   : IVS_SDK_SetPlayBackSpeed
     description: ���ûط��ٶ�
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_FLOAT fSpeed
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT fSpeed);

	/******************************************************************
	 function   : IVS_SDK_GetPlayBackSpeed
	 description: ��ȡ�ط��ٶ�
	 input      : IVS_ULONG ulPlayHandle	���ž��;
	 output     : IVS_FLOAT * pfSpeed		��ǰ�����ٶ�;
	 return     : IVS_INT32
	*******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackSpeed(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_FLOAT* pfSpeed);

    /******************************************************************
     function   : IVS_SDK_SetPlayBackTime
     description: �ط��϶�
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_UINT32 uiTime
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uiTime);

    /******************************************************************
     function   : IVS_SDK_GetPlayBackTime
     description: ��ȡ��ǰ�ط�ʱ��
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_PLAYBACK_TIME * pPlayBackTime
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayBackTime(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_PLAYBACK_TIME* pPlayBackTime);
#endif
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCurrentFrameTick(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT64* pTick);
	/******************************************************************
     function   : IVS_SDK_StartPlatformDownload
     description: ��ʼƽ̨¼������
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_PLAYBACK_TIME * pPlayBackTime
	 input      : IVS_CHAR* pCameraCode
	 input      : IVS_DOWNLOAD_PARAM* pDownloadParam
	 input      : IVS_ULONG*  pulHandle
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle);

	//ƽ̨¼�����أ��������ص�
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformDownloadCB(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam,DownloadCallBackRaw pFPlatformDownLoadRawCB,void* pUserData,IVS_ULONG* pulHandle);
	//ǰ��¼�����أ��������ص�
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUDownloadCB(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam,DownloadCallBackRaw pFPUDownLoadRawCB,void* pUserData,IVS_ULONG* pulHandle);

	/******************************************************************
     function   : IVS_SDK_StopPlatformDownload
     description: ֹͣƽ̨¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle);
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartDisasterRecoveryDownload (
        IVS_INT32 iSessionID,
        const IVS_CHAR* pCameraCode,
        const IVS_DOWNLOAD_PARAM* pDownloadParam,
        IVS_ULONG* pulHandle);

    IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_StopDisasterRecoveryDownload (
        IVS_INT32 iSessionID,
        IVS_ULONG ulHandle);

	/******************************************************************
     function   : IVS_SDK_StartPUDownload
     description: ��ʼǰ��¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_CHAR* pCameraCode
	 input      : IVS_DOWNLOAD_PARAM* pDownloadParam
	 input      : IVS_ULONG* pulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUDownload(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode,
					const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle);

	/******************************************************************
     function   : IVS_SDK_StopPUDownload
     description: ֹͣƽ̨¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle);
#endif
	/******************************************************************
     function   : IVS_SDK_DownloadPause
     description: ��ͣ¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DownloadPause(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/******************************************************************
     function   : IVS_SDK_DownloadResume
     description: �ָ�¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DownloadResume(IVS_INT32 iSessionID,IVS_ULONG ulHandle);

	/******************************************************************
     function   : IVS_SDK_GetDownloadInfo
     description: ��ȡ������Ϣ
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : pDownloadInfo
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDownloadInfo(IVS_INT32 iSessionID, 
		              IVS_ULONG ulHandle, IVS_DOWNLOAD_INFO* pDownloadInfo);
#endif

	/**************************************************************************
    * name       : 
    * description: ��ʼ�ͻ���ǰ��¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
				   hWnd            ���ھ��
                   pHandle         �طž��
    * output     : NA
    * return     : �ɹ����ز��ž����ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPUPlayBack(
                                    IVS_INT32 iSessionID,
                                    const IVS_CHAR* pCameraCode,
                                    const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                    HWND hWnd,
                                    IVS_ULONG* pHandle);

   /**************************************************************************
    * name       : 
    * description: ֹͣ�ͻ���ǰ��¼��ط�
    * input      : iSessionID      �ỰID
				   ulPlayHandle    ���ž��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPUPlayBack(IVS_INT32 iSessionID,
                                                       IVS_ULONG ulPlayHandle);
#endif
    /**************************************************************************
    * name       : IVS_SDK_ModifyDeviceGroup
    * description: �޸��豸��
    * input      : iSessionID         ��¼�ɹ���ĻỰID
                   pDevGroupInfo      �豸��
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyDeviceGroupName(IVS_INT32 iSessionID, 
                                                        const IVS_CHAR* pDomainCode,
                                                        IVS_UINT32 uiDevGroupID,
                                                        const IVS_CHAR* pNewDevGroupName);

	/*************************************************************************
	* name       : IVS_SDK_MoveDeviceGroup
	* description: �ƶ��豸�����豸 
	* input      : iSessionID ��¼�ɹ���ĻỰID, const IVS_CHAR* pReq, IVS_CHAR* &pRsp
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_MoveDeviceGroup(IVS_INT32 iSessionID, 
                                                 const IVS_CHAR* pDomainCode,
                                                 IVS_UINT32 uiNewParentDevGroupID,
                                                 IVS_UINT32 uiDevGroupID);

    /**************************************************************************
    * name       : IVS_SDK_GetDecodeAbility
    * description: ��ȡ��������
    * input      : NA
    * output     : NA
    * return     : VIDEO_DECODE_MODE
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_BOOL __SDK_CALL IVS_SDK_GetGPUDecodeAbility();

    /**************************************************************************
    * name       : IVS_SDK_EnableGPU
    * description: ����/�ر�GPU����
    * input      : bEnable��Ӳ���־������:TRUE;�ر�:FALSE;
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_VOID __SDK_CALL IVS_SDK_EnableGPU(IVS_BOOL bEnable);
#endif
	/**************************************************************************
    * name       : IVS_SDK_GetMediaInfo
    * description: ��ȡý����Ϣ
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   ulHandle           ҵ����������ʵ�����ط�
    * output     : pMediaInfo        ý����Ϣ
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetMediaInfo(IVS_INT32 iSessionID, IVS_ULONG ulHandle, IVS_MEDIA_INFO* pMediaInfo);
#endif

	/**************************************************************************
    * name       : IVS_SDK_GetPlayedTime
    * description: ��ȡ�Ѳ���ʱ��
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   ulHandle           ҵ����������ʵ�����ط�
    * output     : puPlayedTime       ��ǰ����ʱ��(����)
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPlayedTime(IVS_INT32 iSessionID, IVS_ULONG ulHandle, IVS_UINT32* puPlayedTime);
#endif

	/**************************************************************************
    * name       : IVS_SDK_SetPlayQualityMode
    * description: ����ʵ������ģʽ������������
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   ulHandle          ҵ����������ʵ�����ط�
				   iPlayQualityMode  ����ģʽ �ο���IVS_PLAY_QUALITY_MODE
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetPlayQualityMode(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iPlayQualityMode);


	/**************************************************************************
	* name       : IVS_SDK_GetVideoNumAndResource
	* description: ��ѯMU����ԴԤ���Ƿ�ﵽ���޼���ǰ�������Ƶ���Ƿ�����
	* input      : iSessionID        ��¼�ɹ���ĻỰID
				   pCameraCode       ���������
	* output     : bReachLimit       �Ƿ�ﵽ���ޣ�0-δ�ﵽ 1-�Ѵﵽ
				   bWhetherCited     �Ƿ����ã�0-û������ 1-�б�����
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoNumAndResource (IVS_INT32 iSessionID, 
																	 const IVS_CHAR* pCameraCode,
																	 IVS_BOOL* bReachLimit,
																	 IVS_BOOL* bWhetherCited);
#endif

	/**************************************************************************
	* name       : IVS_SDK_SetWaterMarkVerify
	* description: ���ÿ���/ֹͣУ��ˮӡ
	* input      : iSessionID        ��¼�ɹ���ĻỰID
				   ulHandle          ҵ����������ʵ�����ط�
				   bIsStart          1������ˮӡ 0��ֹͣˮӡ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetWaterMarkVerify(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_BOOL bIsStart);

	/**************************************************************************
	* name       : IVS_SDK_WatermarkAlarmConfirm
	* description: ˮӡ�澯ȷ��
	* input      : iSessionID        ��¼�ɹ���ĻỰID
				   pConfirmInfo     ˮӡ�澯ȷ����Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_WatermarkAlarmConfirm (IVS_INT32 iSessionID, const IVS_WATERMARK_ALARM_CONFIRM* pConfirmInfo);

	/**************************************************************************
	* name       : IVS_SDK_QueryWatermarkAlarmList
	* description: ˮӡ�澯��ѯ
	* input      : iSessionID       ��¼�ɹ���ĻỰID
				   pQueryFormat     ˮӡ�澯ȷ����Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryWatermarkAlarmList(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT *pQueryFormat, 
																			IVS_VOID* pBuffer,IVS_UINT32 uiBufferSize);


	/**************************************************************************
	* name       : IVS_SDK_WatermarkAlarmCancel
	* description: ˮӡ�澯����
	* input      : iSessionID       ��¼�ɹ���ĻỰID
				   pCancelInfo      ˮӡ�澯������Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_WatermarkAlarmCancel(IVS_INT32 iSessionID, const IVS_WATERMARK_ALARM_CANCEL* pCancelInfo);

	/**************************************************************************
	* name       : IVS_SDK_SetDisplayScale
	* description: ���ò�����ʾ����
	* input      : iSessionID        ��¼�ɹ���ĻỰID
				   ulHandle          ҵ����������ʵ�����ط�
				   iDisplayScale     ��ʾ����ֵ��1-ԭʼ������ʾ 2-������ʾ 
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	**************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDisplayScale(IVS_INT32 iSessionID, IVS_ULONG ulHandle,IVS_INT32 iDisplayScale);
#endif

	/**************************************************************************
    * name       : IVS_SDK_GetNVRList
    * description: ��ѯ��Ԫ�б�
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   pDomainCode       �����
				   uiServerType      ��������
				   pNvrList          ��Ԫ�����б�ṹָ��
				   uiBufferSize      �ṹָ���С
    * output     : pMediaInfo        
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetNVRList(IVS_INT32 iSessionID, 
		                                                const IVS_CHAR* pDomainCode,
		                                                IVS_UINT32 uiServerType,
		                                                const IVS_INDEX_RANGE* pIndexRange,
		                                                IVS_DEVICE_BRIEF_INFO_LIST* pNvrList,
		                                                IVS_UINT32 uiBufferSize);


    /******************************************************************
    function : IVS_SDK_DeleteDeviceFromGroup
    description: ɾ���豸��������
    input : IVS_INT32 iSessionID
			const IVS_CHAR * pTargetDomainCode
			IVS_UINT32 uiDeviceNum
			const IVS_DEV_GROUP_CAMERA * pGroupCameraList
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteDeviceFromGroup( IVS_INT32 iSessionID, 
                                                                    const IVS_CHAR* pTargetDomainCode,
                                                                    IVS_UINT32 uiDeviceNum,
                                                                    const IVS_DEV_GROUP_CAMERA* pGroupCameraList);



	/**************************************************************************
    * name       : IVS_SDK_SetDeviceConfig
    * description: �������豸�����豸����
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   pDomainCode       �����
				   uiConfigType      �����豸����
				   pDevCode          �豸����
                   pBuffer           �����ṹ��ָ��
				   uiBufferSize      �����ṹ���С
    * output     : NA        
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode, 
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);

	/**************************************************************************
    * name       : IVS_SDK_GetDeviceConfig
    * description: �������豸�����豸����
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   pDomainCode       �����
				   uiConfigType      �����豸����
				   pDevCode          �豸����
                   pBuffer           ���ز����ṹ��ָ��
				   uiBufferSize      �����ṹ���С
    * output     : NA       
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceConfig(IVS_INT32 iSessionID,
                                                             const IVS_CHAR* pDevCode,
                                                             IVS_UINT32 uiConfigType,
                                                             IVS_VOID* pBuffer,
                                                             IVS_UINT32 uiBufSize);


    /******************************************************************
    function : IVS_SDK_GetChannelList
    description: �������豸��ѯ���豸�б�
    input : IVS_INT32 iSessionID
             const IVS_CHAR * pDevCode 
             IVS_UINT32 uiBufferSize
    output : IVS_DEV_CHANNEL_BRIEF_INFO_LIST * pChannelList
    return : IVS_SDK_API IVS_INT32
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetChannelList(IVS_INT32 iSessionID, 
                                                 const IVS_CHAR* pDevCode,
                                                 IVS_DEV_CHANNEL_BRIEF_INFO_LIST* pChannelList,
                                                 IVS_UINT32 uiBufferSize);

	/**************************************************************************
    * name       : IVS_SDK_GetAlarmLevel
    * description: ��ѯ�澯��������
    * input      : iSessionID        ��¼�ɹ���ĻỰID
                   uiAlarmLevelID    �澯����ID
    * output     : pAlarmLevel       �澯�������� 
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLevel(IVS_INT32 iSessionID,IVS_UINT32 uiAlarmLevelID,IVS_ALARM_LEVEL* pAlarmLevel);

	/**************************************************************************
    * name       : IVS_SDK_Aes256CbcEncode
    * description: AES256����
    * input      : pszPlain        ����     
                      ullLenCipher �����pszCipher�Ŀռ��С
    * output     : pszCipher      ���� 
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
	**************************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Aes256CbcEncode(const IVS_CHAR* pszPlain, IVS_CHAR* pszCipher, IVS_UINT64 ullLenCipher);
   
	/**************************************************************************
    * name       : IVS_SDK_Aes256CbcEncode
    * description: AES256����
    * input      : pszCipher      ����      ullLenCipher �����pszPlain�Ŀռ��С
    * output     :   pszPlain        ����  
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
	**************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_Aes256CbcDecode(const IVS_CHAR* pszCipher, IVS_CHAR* pszPlain, IVS_UINT64 ullLenPlain);

  /**************************************************************************
    * name       : IVS_SDK_StartPlatformRecord
    * description: ��ʼƽ̨¼��
    * input      : iSessionID      ��¼�ɹ���ĻỰID
	               pCameraCode     ��ʼƽ̨¼������������
				   iRecordTime     ¼��ʱ��
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
	**************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime);

  /**************************************************************************
    * name       : IVS_SDK_StopPlatformRecord
    * description: ֹͣƽ̨¼��
    * input      : iSessionID      ��¼�ɹ���ĻỰID
	               pCameraCode     ֹͣƽ̨¼������������
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPlatformRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

  /**************************************************************************
  * name       : IVS_SDK_StartPURecord(
  * description: ��ʼǰ��¼��
  * input      : iSessionID      ��¼�ɹ���ĻỰID
  pCameraCode     ��ʼǰ��¼������������
  iRecordTime     ¼��ʱ��
  * return     : �ɹ�����0��ʧ�ܷ��ش�����
  * remark     : NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime);

  /**************************************************************************
    * name       : IVS_SDK_StopPURecord
    * description: ֹͣǰ��¼��
    * input      : iSessionID      ��¼�ɹ���ĻỰID
	               pCameraCode     ֹͣƽ̨¼������������
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * remark     : NA
  **************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopPURecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

  /**************************************************************************
   * name       : IVS_SDK_StartLocalRecord
   * description: ��ʼ����¼��
   * input      : iSessionID     ��¼�ɹ���ĻỰID
	              pRecordParam   ����¼�����
				  ulPlayHandle   ���ž��
				  pSaveFileName  �����ļ���
   * return     : �ɹ�����0��ʧ�ܷ��ش�����
   * remark     : NA
   **************************************************************************/
#ifdef WIN32
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartLocalRecord(IVS_INT32 iSessionID,
	                                                         const IVS_LOCAL_RECORD_PARAM* pRecordParam, 
                                                             IVS_ULONG ulPlayHandle, 
                                                             const IVS_CHAR* pSaveFileName);

  /**************************************************************************
   * name       : IVS_SDK_StopLocalRecord
   * description: ֹͣ����¼��
   * input      : iSessionID      ��¼�ɹ���ĻỰID
	              ulPlayHandle    ���ž��
   * return     : �ɹ�����0�� ʧ�ܷ��ش�����
   * remark     : NA
   **************************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopLocalRecord(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle);
#endif

  /******************************************************************
  function : IVS_SDK_GetDomainRoute
  description: ��ѯ��·����Ϣ
  input : IVS_INT32 iSessionID
          IVS_UINT32 uiBufferSize
  output : IVS_DOMAIN_ROUTE_LIST * pDomainRouteList
  return : 
  *******************************************************************/  
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainRoute( IVS_INT32 iSessionID, 
                          IVS_DOMAIN_ROUTE_LIST* pDomainRouteList,
                          IVS_UINT32 uiBufferSize);

  /******************************************************************
  function : IVS_SDK_ReleaseBuffer
  description: �ͷ�SDK�ڲ�������ڴ�
  input : IVS_INT32 iSessionID
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/  
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ReleaseBuffer(IVS_CHAR *pBuffer);



  /******************************************************************
  function : IVS_SDK_IA
  description: ���ܷ���ͳһ͸���ӿ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  IVS_UINT32		uiInterfaceType �ӿ����ͣ�
		  const IVS_CHAR*	pReqXml			����CBB::CBB::CXml��
  output : IVS_CHAR**		pRspXml			��ӦCBB::CBB::CXml
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_IA(IVS_INT32 iSessionID, IVS_UINT32 uiInterfaceType, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

  /******************************************************************
  function : IVS_SDK_IA_EX
  description: ���ܷ���ͳһ͸���ӿ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  IVS_UINT32		uiInterfaceType �ӿ����ͣ�
		  const IVS_CHAR*	pReqXml			����CBB::CBB::CXml��
		  const IVS_CHAR*   pTransID		������Ϣ��
  output : IVS_CHAR**		pRspXml			��ӦCBB::CBB::CXml
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_IA_EX(IVS_INT32 iSessionID, IVS_UINT32 uiInterfaceType, IVS_CHAR* pTransID, 
												 IVS_UINT32 uiTransIDLen, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

  /******************************************************************
  function : IVS_SDK_SetDrawCallBack
  description: ���û�ͼ��ʾ�Ļص��ӿ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  IVS_ULONG			ulPlayHandle	���ž����
		  DRAW_PROC 		callback		��ͼ�ص�����ָ�룻
		  void*��			pUserData		�û�����ָ��
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
#ifdef WIN32
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDrawCallBack(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, DRAW_PROC callback, void *pUserData);
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDrawCallBackEx(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, HWND hWnd, DRAW_PROC callback, void *pUserData);
#endif
  /******************************************************************
  function : IVS_SDK_SetDelayFrameNum
  description: ������ʱ����֡��
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  IVS_ULONG			ulPlayHandle	���ž����
		  IVS_UINT32 		uDelayFrameNum	��ʱ֡����
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
#ifdef WIN32
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDelayFrameNum(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uDelayFrameNum);
#endif

  /******************************************************************
  function   : IVS_SDK_GetRecordStatus
  description: �ƾ�����
  input      : iSessionID ��¼�ɹ��󷵻صĻỰID
               pCameraCode ���������
			   uiRecordMethod ¼��ʽ
  output     : NA
  return     : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordStatus(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordState);

  /******************************************************************
  function   : IVS_SDK_GetPTZAbsPosition
  description: ��ȡ��̨��ǰλ��
  input      : iSessionID ��¼�ɹ��󷵻صĻỰID
			   pDomainCode �����
               pCameraCode ���������
  output     : pPTZPosInfo ��̨λ����Ϣ
  return     : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZAbsPosition(IVS_INT32 iSessionID,
	  const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, IVS_PTZ_ABSPOSITION *pPtzAbsPosition);

  /******************************************************************
  function   : IVS_SDK_PtzControl
  description: �ƾ�����
  input      : iSessionID ��¼�ɹ��󷵻صĻỰID
               pCameraCode ���������
			   iControlCode ��̨������
			   pControlPara1 ����1
			   pControlPara2 ����2
  output     : NA
  return     : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControl(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode, IVS_INT32 iControlCode,const IVS_CHAR* pControlPara1,const IVS_CHAR* pControlPara2, IVS_UINT32* pLockStatus);

  /******************************************************************
  function   : IVS_SDK_PtzControlWithLockerInfo
  description: �ƾ�����
  input      : iSessionID ��¼�ɹ��󷵻صĻỰID
               pCameraCode ���������
			   iControlCode ��̨������
			   pControlPara1 ����1
			   pControlPara2 ����2
  output     : pPtzControlInfo ��̨������Ϣ
  return     : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PtzControlWithLockerInfo(IVS_INT32 iSessionID,
															  const IVS_CHAR* pCameraCode, 
															  IVS_INT32 iControlCode,
															  const IVS_CHAR* pControlPara1,
															  const IVS_CHAR* pControlPara2, 
															  IVS_PTZ_CONTROL_INFO* pPtzControlInfo);

	/*************************************************************************
	* name       : IVS_SDK_SetMainDeviceName
	* description: �޸����豸����
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pDevCode ���豸����
				   pNewDevName ���豸�µ�����
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��fengjiaji f00152768
	* remark     : NA
	* history    : 2012-12-14 11:12[add]
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMainDeviceName(	IVS_INT32 iSessionID,
																const IVS_CHAR* pDevCode,
																const IVS_CHAR* pNewDevName);


	/*************************************************************************
	* name       : IVS_SDK_SetCameraName
	* description: �޸����������
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pCameraCode ���������
				   pNewCameraName ������µ�����
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��fengjiaji f00152768
	* remark     : NA
	* history    : 2012-12-14 11:21[add]
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraName(	IVS_INT32 iSessionID,
															const IVS_CHAR* pCameraCode,
															const IVS_CHAR* pNewCameraName);
	
    /*************************************************************************
	* name       : IVS_SDK_PlaySound
	* description: ������·����
	* input      : iSessionID   ��¼�ɹ���ĻỰID, 
				   uiPlayHandle ���ž��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2012-12-20[add]
	*************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlaySound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle);

    /*************************************************************************
	* name       : IVS_SDK_StopSound
	* description: ֹͣ������·����
	* input      : iSessionID   ��¼�ɹ���ĻỰID, 
				   uiPlayHandle ���ž��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2012-12-20[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSound(IVS_INT32 iSessionID, IVS_UINT32 uiPlayHandle);


    /*************************************************************************
	* name       : IVS_SDK_SetVolume
	* description: ��������
	* input      : iSessionID   ��¼�ɹ���ĻỰID, 
				   uiPlayHandle ���ž��
                   uiVolumeValue����
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2012-12-25[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVolume(IVS_INT32 iSessionID,
                                                       IVS_UINT32 uiPlayHandle, 
                                                       IVS_UINT32 uiVolumeValue);


    /*************************************************************************
	* name       : IVS_SDK_GetVolume
	* description: ��ȡ����
	* input      : iSessionID   ��¼�ɹ���ĻỰID, 
				   uiPlayHandle ���ž��
	* output     : uiVolumeValue����
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2012-12-25[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVolume(IVS_INT32 iSessionID,
                                                       IVS_UINT32 uiPlayHandle, 
                                                       IVS_UINT32* puiVolumeValue);
#endif
	/*************************************************************************
	* name       : IVS_SDK_SetUserData
	* description: �����û�����
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pFileName �û�����ѹ���ļ�
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��tanjibao
	* remark     : NA
	* history    : 2012-12-25
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetUserData (IVS_INT32 iSessionID, const IVS_CHAR* pFileName);

	/*************************************************************************
	* name       : IVS_SDK_GetUserData
	* description: �ָ��û�����
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pFileName �û�����ѹ���ļ�
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��tanjibao
	* remark     : NA
	* history    : 2012-12-25
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetUserData (IVS_INT32 iSessionID, const IVS_CHAR* pFileName);


	/**************************************************************************
    * name       : IVS_SDK_GetDeviceUserList
    * description: ��ȡӵ��ָ���豸Ȩ�޵��û��б�
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pDevCode   �豸����
				   pIndexRange ��ҳ��Ϣ
				   pUserDevList �û��豸�б�ṹ
				   uiBufferSize �����С
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhougang zWX155350
	* remark     : NA
	* history    : 2012-12-27
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_GetDeviceUserList(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, IVS_UINT32 uiIsUserOnline, const IVS_INDEX_RANGE* pIndexRange, IVS_DEVICE_USER_INFO_LIST* pDevUserList, IVS_UINT32 uiBufferSize);


	/**************************************************************************
    * name       : IVS_SDK_RebootDevice
    * description: ����ǰ���豸
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pDeviceCode  ���豸����
				   uiRebootType �������ͣ�0-���� 1-�ָ�Ĭ�����ò�����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��tanjibao tkf71032
	* remark     : NA
	* history    : 2013-1-6
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RebootDevice (IVS_INT32 iSessionID, const IVS_CHAR* pDeviceCode, IVS_UINT32 uiRebootType);

	/**************************************************************************
    * name       : IVS_SDK_GetConfigInfo
    * description: ��ȡƽ̨������Ϣ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pReqXml  ������Ϣ
				   pRspXml  ������Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhougang zWX155350
	* remark     : NA
	* history    : 2013-1-6
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetConfigInfo (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

		/**************************************************************************
    * name       : IVS_SDK_GetAbilityInfo 
    * description: ��ѯƽ̨��������Ϣ
    * input      : iSessionID ��¼�ɹ���ĻỰID 
	               uiAbility  ƽ̨֧�ֵ�����ֵ�����ֵ��ʾ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��xusida WX168257
	* remark     : NA
	* history    : 2013-1-6
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAbilityInfo  (IVS_INT32 iSessionID,IVS_UINT32* uiAbility);
	/**************************************************************************
    * name       : IVS_SDK_LocalSnapshot
    * description: ����ץ�ĵ���ͼƬ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   ulPlayHandle  ���ž��
				   uiPictureFormat  ͼƬ��ʽ
				   pFileName	ͼƬ�ļ�����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhangshufan z00227377
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LocalSnapshot(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, IVS_UINT32 uiPictureFormat, const IVS_CHAR* pFileName);
	
	/**************************************************************************
    * name       : IVS_SDK_LocalSnapshotRect
    * description: ���ؾֲ��Ŵ�ץ�ĵ���ͼƬ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   ulPlayHandle  ���ž��
				   uiPictureFormat  ͼƬ��ʽ
				   pFileName	ͼƬ�ļ�����
				   pRect		�ֲ��Ŵ�Χ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhangshufan z00227377
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LocalSnapshotRect(IVS_INT32 iSessionID, IVS_ULONG ulPlayHandle, 
																IVS_UINT32 uiPictureFormat, const IVS_CHAR* pFileName, const IVS_RECT* pRect);
#endif

	/**************************************************************************
    * name       : IVS_SDK_GetLogo
    * description: Logo����
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   uiLogoType  �ͻ��˵�Logo������: 0-���Ͻ� 1-��¼/����
				   pLogoFile  Logo�ļ�ȫ·��
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��z00201790
	* remark     : NA
	* history    : 2013-1-23
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetLogo (IVS_INT32 iSessionID, IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoFile);

	/**************************************************************************
    * name       : IVS_SDK_UploadLogo
    * description: Logo����
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   uiLogoType  �ͻ��˵�Logo������: 0-���Ͻ� 1-��¼/���� 2-�ƶ�PAD 3-�ƶ�Phone 4-BS���Ͻ� 5BS��¼/����
				   pLogoFile  Logo�ļ�ȫ·��
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��z00201790
	* remark     : NA
	* history    : 2013-1-23
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UploadLogo(IVS_INT32 iSessionID, IVS_UINT32 uiLogoType, const IVS_CHAR* pLogoFile);

	/**************************************************************************
    * name       : IVS_SDK_GetStatement
    * description: ����������Ϣ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pLanguageType ��������  
	* output     : pStatement  ������Ϣ
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��z00201790
	* remark     : NA
	* history    : 2013-1-23
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStatement(IVS_INT32 iSessionID,const IVS_CHAR* pLanguageType,IVS_CHAR** pStatement);

	/**************************************************************************
    * name       : IVS_SDK_UploadStatement
    * description: �ϴ�������Ϣ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pLanguageType ��������  
				   pStatementFile  ������Ϣ�ļ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��z00201790
	* remark     : NA
	* history    : 2013-1-23
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UploadStatement(IVS_INT32 iSessionID,const IVS_CHAR* pLanguageType,const IVS_CHAR* pStatementFile);

	/**************************************************************************
    * name       : IVS_SDK_AddPTZPreset
    * description: ����Ԥ��λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pPTZPreset  Ԥ��λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName, IVS_UINT32* uiPresetIndex);

	/**************************************************************************
    * name       : IVS_SDK_DelPTZPreset
    * description: ɾ��Ԥ��λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   uiPresetIndex  Ԥ��λ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name       : IVS_SDK_ModPTZPreset
    * description: �޸�Ԥ��λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pPTZPreset  Ԥ��λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModPTZPreset (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset);

	/**************************************************************************
    * name       : IVS_SDK_GetPTZPresetList
    * description: ��ѯԤ��λ�б�
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pIndexRange  ������Χ
				   pPTZPresetList Ԥ��λ��Ϣ�б�
				   uiBufferSize �����С
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPTZPresetList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiPTZPresetNum);

	/**************************************************************************
    * name       : IVS_SDK_SetGuardPos
    * description: ���ÿ���λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name       : IVS_OCX_DelGuardPos
    * description: ɾ������λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   uiPresetIndex  Ԥ��λ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name       : IVS_SDK_GetGuardPos
    * description: ��ȡ����λ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetGuardPos (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name       : IVS_SDK_AddCruiseTrack
    * description: ����Ѳ���켣
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pCuriseTrackInfo  Ѳ���켣��Ϣ
				   uiTrackIndex �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, IVS_UINT32* uiTrackIndex);

	/**************************************************************************
    * name       : IVS_SDK_DelCruiseTrack
    * description: ɾ��Ѳ���켣
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   uiTrackIndex  �켣����
				   uiCruiseType �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiTrackIndex, IVS_UINT32 uiCruiseType);

	/**************************************************************************
    * name       : IVS_SDK_ModCruiseTrack 
    * description: �޸�Ѳ���켣
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pCuriseTrackInfo  Ѳ���켣��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo);

	/**************************************************************************
    * name       : IVS_SDK_GetCruiseTrackList
    * description: ��ѯѲ���켣�б�
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pCruiseTrackInfoList Ѳ���켣�б�
				   uiBufferSize �����С
				   uiCruiseTrackNum ʵ�ʲ�ѯ����Ѳ���켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrackList (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_TRACK* pCruiseTrackList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiCruiseTrackNum);

	/**************************************************************************
    * name       : IVS_SDK_GetCruiseTrack
    * description: ��ѯѲ���켣��Ϣ
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   uiTrackIndex  �켣����
				   pCruiseTrackInfo �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruiseTrack (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_INT32 uiTrackIndex, IVS_CRUISE_TRACK* pCruiseTrackInfo);

	/**************************************************************************
    * name       : IVS_SDK_SetCruisePlan
    * description: ����Ѳ���ƻ�
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pCruisePlan  Ѳ���ƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, const IVS_CRUISE_PLAN* pCruisePlan);

	/**************************************************************************
    * name       : IVS_SDK_GetCruisePlan
    * description: ��ѯѲ���ƻ�
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pCruisePlan  Ѳ���ƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCruisePlan (IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CRUISE_PLAN* pCruisePlan);
   /**************************************************************************
    * name       : IVS_SDK_GetDeviceAlarmList
    * description: ��ѯOMU�澯�¼��б�
				   pUnifiedQuery  ������Ϣ�ṹ��
				   pAlarmEventList  ��Ӧ��Ϣ�ṹ��
    * author     ��wulixia wWX155351
	* history    : 2013-1-23
	/**************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceAlarmList(IVS_INT32 iSessionID, 
																const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
																IVS_DEVICE_ALARM_EVENT_LIST* pAlarmEventList,
																IVS_UINT32 uiBufferSize);

	/*****************************************************************
	     function   : IVS_SDK_SetCameraBackupConfig
	     description: ������������ݲ���     
	     input      : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pBackupXml ¼�񱸷ݲ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetCameraBackupConfig(IVS_INT32 iSessionID, const IVS_CHAR* pBackupXml);

	/*****************************************************************
	     function   : IVS_SDK_GetCameraBackupConfig
	     description: ��ȡ��������ݲ���
	     output     : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pCameraCode ���������
	     output     : IVS_CHAR * * pBackConfigXml  ���ݲ���xml
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraBackupConfig(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_CHAR** pBackConfigXml);
#endif
	/**************************************************************************
    * name       : IVS_SDK_PlatformSnapshot
    * description: ƽ̨ץ�� 
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PlatformSnapshot(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode);

	/**************************************************************************
    * name       : IVS_SDK_SetSnapshotPlan
    * description: ����ƽ̨ץ�ļƻ� 
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pSnapShotPlan ץ�ļƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetSnapshotPlan(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_SNAPSHOT_PLAN* pSnapShotPlan);

	/**************************************************************************
    * name       : IVS_SDK_GetSnapshotPlan 
    * description: ��ѯƽ̨ץ�ļƻ� 
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pSnapShotPlan ץ�ļƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetSnapshotPlan(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_SNAPSHOT_PLAN* pSnapShotPlan);

	/*****************************************************************
	     function   : IVS_SDK_GetSnapshotList
	     description: ��ȡƽ̨ץ��ͼƬ�б�              
	     input     : IVS_INT32 iSessionID                           ��¼�ɹ���ĻỰID
		 input     :  const IVS_CHAR* pCameraCode
	                  const IVS_QUERY_SNAPSHOT_PARAM * pQueryParam   ץ��ͼƬ��ѯ����
	     output    : IVS_SNAPSHOT_INFO_LIST * pSnapshotList         ץ��ͼƬ�б���Ϣ
	     input     : IVS_UINT32 uiBufSize                           �����С
	     return    : �ɹ�����0��ʧ�ܷ��ش�����   
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetSnapshotList(IVS_INT32 iSessionID,
															 const IVS_CHAR* pCameraCode,
															 const IVS_QUERY_SNAPSHOT_PARAM* pQueryParam,
															 IVS_SNAPSHOT_INFO_LIST* pSnapshotList,
															 IVS_UINT32 uiBufSize);

	/*****************************************************************
	     function   : IVS_SDK_GetExDomainList
	     description: ��ѯ�����б�
	     intput     : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID
		 intput     : pQueryInfo ͨ�ò�ѯ      
	     output     : pRspXml �����б���ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainList (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo,IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_ShareCatalog
	     description: Ŀ¼����
	     intput     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
	     output     : pShareInfo Ŀ¼������ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShareCatalog (IVS_INT32 iSessionID,
														   const IVS_CHAR* pShareInfo);

	/*****************************************************************
	     function   : IVS_SDK_GetShareCatalogList
	     description: ��ѯĿ¼������Ϣ�б�
	     intput     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  IVS_CHAR pQueryInfo		   ��ѯ����
	     output     : pRspXml Ŀ¼������Ϣ�б�XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetShareCatalogList (IVS_INT32 iSessionID,
																  const IVS_CHAR* pQueryInfo,
																  IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_StartSearchDevice
	     description: ��������ǰ���豸
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pSearchInfo �豸������ϢXML
	     output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	     function   : IVS_SDK_StopSearchDevice
	     description: ֹͣ����ǰ���豸
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pSearchInfo �豸������ϢXML
	     output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopSearchDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pSearchInfo);

	/*****************************************************************
	     function   : IVS_SDK_VerifyDevice
	     description: ��֤ǰ���豸
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pVerifyInfo �豸��֤��ϢXML
		 output     : IVS_CHAR** pRspXml           ��֤���XML 
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_VerifyDevice(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, const IVS_CHAR* pVerifyInfo, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_GetDevLog
	     description: ��ѯǰ����־
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pQueryXml   ��ѯ����XML
		 output     : IVS_CHAR** pRspXml           ��֤���XML 
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDevLog(IVS_INT32 iSessionID, const IVS_CHAR* pQueryXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_GetDomainList
	     description: ��ѯ�豸���б�
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
		 output     : IVS_CHAR** pDomainList       �豸���б�XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainList(IVS_INT32 iSessionID, IVS_CHAR** pDomainList);

	/*****************************************************************
	     function   : IVS_SDK_GetCPUMemory
	     description: CPU/�ڴ����ܲ�ѯ
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pQueryXml   ��ѯXML
		 output     : IVS_CHAR** pRspXml           ��ӦXML 
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance);

    /*************************************************************************
	* name       : IVS_SDK_StartVoiceTalkback
	* description: ��ʼ�����Խ�
	* input      : iSessionID   ��¼�ɹ���ĻỰID
				   pTalkbackParam �����Խ�������ע��Ŀǰֻ֧��UDP��ʽ��
                   pCameraCode ���������
	* output     : pHandle �Խ����
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
#ifdef WIN32
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartVoiceTalkback(
                                IVS_INT32 iSessionID,
                                const IVS_VOICE_TALKBACK_PARAM* pTalkbackParam,
                                const IVS_CHAR* pCameraCode,
                                IVS_ULONG* pHandle);

    /*************************************************************************
	* name       : IVS_SDK_StopVoiceTalkback
	* description: ֹͣ�����Խ�
	* input      : iSessionID   ��¼�ɹ���ĻỰID
				   ulHandle �Խ����
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopVoiceTalkback(
                                                          IVS_INT32 iSessionID,
                                                          IVS_ULONG ulHandle);

    /**************************************************************************
    * name       : IVS_SDK_SetMicVolume
    * description: ������˷�����
    * input      : uiVolumeValue ����ֵ����Χ0~100
    * output     ��NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetMicVolume(IVS_UINT32 uiVolumeValue);

    /**************************************************************************
    * name       : IVS_SDK_GetMicVolume
    * description: ��ȡ��˷�����
    * input      : NA
    * output     ��NA
    * return     : ����ֵ����Χ0~100
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_UINT32 __SDK_CALL IVS_SDK_GetMicVolume();

    /*************************************************************************
	* name       : IVS_SDK_StartRealBroadcast
	* description: ��ʼʵʱ�����㲥
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name       : IVS_SDK_StopRealBroadcast
	* description: ֹͣʵʱ�����㲥
	* input      : iSessionID   ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name       : IVS_SDK_StartFileBroadcast
	* description: ��ʼ�ļ������㲥
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pFileName  ��Ƶ�ļ���,
                   bCycle     �Ƿ�ѭ�����ţ�0-��ѭ����1-ѭ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartFileBroadcast(
                                                    IVS_INT32 iSessionID,
                                                    const IVS_CHAR* pFileName,
                                                    IVS_BOOL bCycle);

    /*************************************************************************
	* name       : IVS_SDK_StopFileBroadcast
	* description: ֹͣ�ļ������㲥
	* input      : iSessionID   ��¼�ɹ���ĻỰID, 
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopFileBroadcast(IVS_INT32 iSessionID);

    /*************************************************************************
	* name       : IVS_SDK_AddBroadcastDevice
	* description: ��ӹ㲥�豸
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pCameraCode ���������
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBroadcastDevice(
                                                   IVS_INT32 iSessionID,
                                                   const IVS_CHAR* pCameraCode);

    /*************************************************************************
	* name       : IVS_SDK_DeleteBroadcastDevice
	* description: ɾ���㲥�豸
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pCameraCode ���������
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��zhouqiming z90003203
	* remark     : NA
	* history    : 2013-1-29[add]
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBroadcastDevice (
                                                   IVS_INT32 iSessionID,
                                                   const IVS_CHAR* pCameraCode);


	/*****************************************************************
	     function   : IVS_SDK_DelSnapshotPlan
	     description: ɾ��ƽ̨ץ�Ļ���
	     output     : IVS_INT32 iSessionID    ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pCameraCode  ���������
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelSnapshotPlan(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode);
#endif
	/*****************************************************************
	     function   : IVS_SDK_AddRecordBackupPlan
	     description: ���¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pBackupPlan  ���¼�񱸷ݼƻ�XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);


	/*****************************************************************
	     function   : IVS_SDK_ModifyRecordBackupPlan
	     description: �޸�¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
			          const IVS_CHAR * pBackupPlan  �޸�¼�񱸷ݼƻ�XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);

	/*****************************************************************
	     function   : IVS_SDK_DeleteRecordBackupPlan
	     description: ɾ��¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID   ��¼�ɹ���ĻỰID
	                  const IVS_CHAR * pBackupPlan  ɾ��¼�񱸷ݼƻ�XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pBackupPlan);


	/*****************************************************************
	     function   : IVS_SDK_GetRecordBackupPlan
	     description: ��ѯ¼�񱸷ݼƻ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
				      IVS_CHAR * pQueryInfo     ��ѯ¼�񱸷ݼƻ�XML
	     output     : IVS_CHAR * * pBackupPlan  ¼�񱸷ݼƻ�������ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordBackupPlan(IVS_INT32 iSessionID, const IVS_CHAR* pQueryInfo, IVS_CHAR** pBackupPlan);
#endif
	/*****************************************************************
	     function   : IVS_SDK_GetIACPUMemory
	     description: ���ܷ���CPU/�ڴ����ܲ�ѯ
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pQueryXml   ��ѯXML
		 output     : IVS_CHAR** pRspXml           ��ӦXML 
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetIACPUMemory(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance);

	/*****************************************************************
	     function   : IVS_SDK_QueryPerformance
	     description: ��Ԫ���ܲ�ѯ
	     output     : IVS_INT32 iSessionID         ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pQueryXml   ��ѯXML
		 output     : IVS_CHAR** pRspXml           ��ӦXML 
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_QueryPerformance(IVS_INT32 iSessionID, const IVS_CHAR* pNodeCode, IVS_CHAR** pPerformance);
	
	/*****************************************************************
	     function   : IVS_SDK_GetRecordTask
	     description: ��ѯ��ǰ�û����ֶ�¼������
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     output     : IVS_CHAR * * pRspXml  ��ѯ��ǰ�û����ֶ�¼�����񷵻���ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordTask(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_GetRecordBackupTaskList
	     description: ��ѯ¼�񱸷������б�
	     input      : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID;
	     output     : IVS_CHAR * * pRspXml  ��ѯ¼�񱸷������б�����ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetRecordBackupTaskList(IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo,IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_StartRecordBackupTask
	     description: ����/����¼�񱸷�����
	     input      : pQueryInfo
	     output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL  IVS_SDK_StartRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo);

	/*****************************************************************
	     function   : IVS_SDK_StopRecordBackupTask
	     description: ��ͣ/ֹͣ¼�񱸷�����
	     input      : pQueryInfo
	     output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo);

	/*****************************************************************
	     function   : IVS_SDK_DeleteRecordBackupTask
	     description: ɾ��¼�񱸷�����
	     input      : pQueryInfo
	     output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteRecordBackupTask (IVS_INT32 iSessionID,const IVS_CHAR* pQueryInfo);

	/*****************************************************************
	     function   : IVS_SDK_AddRecordBackupTask
	     description: ����¼�񱸷�����
	     input      : IVS_INT32 iSessionID     ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pBackupTask  ����¼�񱸷�����xml
	     return     : 
	*****************************************************************/	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddRecordBackupTask(IVS_INT32 iSessionID, const IVS_CHAR* pBackupTask);
#endif

	/**************************************************************************
    * name       : IVS_SDK_StartRealPlayByIP
    * description: ��ʼʵʱ���
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   pCameraCode  ���������
				   pMediaAddrDst  ý����Ŀ���ַ
	* output     : pMediaAddrSrc ý����Դ��ַ
				   pHandle ���ž����Ψһָ���˲���ͨ����
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��gaoshuming g00209332
	* remark     : NA
	* history    : 2013-3-5
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIP(IVS_INT32 iSessionID, 
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle);
	/**************************************************************************
    * name       : IVS_SDK_StopRealPlayByIP
    * description: ֹͣʵʱ���
    * input      : iSessionID ��¼�ɹ���ĻỰID
				   ulHandle ���ž����Ψһָ���˲���ͨ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��gaoshuming g00209332
	* remark     : NA
	* history    : 2013-3-5
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIP(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/**************************************************************************
    * name			: IVS_SDK_StartRealPlayByIPEx
    * description	: ��ʼʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  pRealplayParam  ���Ų���
						  pCameraCode  ���������
						  pMediaAddrDst  ý����Ŀ���ַ
	* output		: pMediaAddrSrc ý����Դ��ַ
						  pHandle ���ž����Ψһָ���˲���ͨ����
    * return		: �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author		: zhangrui z00233055
	* remark		: NA
	* history		: 2013-12-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartRealPlayByIPEx(IVS_INT32 iSessionID, 
															const IVS_REALPLAY_PARAM  *pRealplayParam,
															const IVS_CHAR* pCameraCode,
															const IVS_MEDIA_ADDR* pMediaAddrDst,
															IVS_MEDIA_ADDR* pMediaAddrSrc,
															IVS_ULONG* pHandle);

	/**************************************************************************
    * name			: IVS_SDK_StopRealPlayByIPEx
    * description	: ֹͣʵʱ���
    * input			: iSessionID ��¼�ɹ���ĻỰID
						  ulHandle ���ž����Ψһָ���˲���ͨ����
	* output		: NA
    * return		: �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author		: zhangrui z00233055
	* remark		: NA
	* history		: 2013-12-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopRealPlayByIPEx(IVS_INT32 iSessionID, IVS_ULONG ulHandle);

	/*****************************************************************
	     function   : IVS_SDK_GetAlarmLinkageAction
	     description: ��ѯ�澯������������
	     input      : IVS_INT32 iSessionID     ��¼�ɹ���ĻỰID
					  const IVS_CHAR * pReqXml  ����xml
					  IVS_CHAR** pRsqXml  ��Ӧxml
	     return     : 
	*****************************************************************/	
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmLinkageAction(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

	/******************************************************************
    function : IVS_SDK_LockRecord
    description: ����¼��
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/******************************************************************
    function : IVS_SDK_ModLockRecord
    description: �޸�¼��������Ϣ
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModLockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo);


	/******************************************************************
    function : IVS_SDK_UnlockRecord
    description: ����¼��
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UnLockRecord(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/******************************************************************
    function : IVS_SDK_LockBackupRecord
    description: ����¼��(���ݷ�����)
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_LockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/******************************************************************
    function : IVS_SDK_UnlockBackupRecord
    description: ����¼��(���ݷ�����)
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_UnlockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo);

	/******************************************************************
    function : IVS_SDK_ModLockBackupRecord
    description: �޸�¼��������Ϣ(���ݷ�����)
    input :  iSessionID                            ��¼�ɹ���ĻỰID
	input : pCameraCode                                     ���������
	input :  pRecordLockInfo                                ¼��������Ϣ
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModLockBackupRecord(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_RECORD_LOCK_INFO* pRecordLockInfo);


	/*************************************************************************
	* name       : IVS_SDK_GetMotionRangeDataPlatform
	* description: ��ѯƽ̨�˶�����������
	* input      : iSessionID ��¼�ɹ���ĻỰID, 
				   pCameraCode ���������
				   pMotionRangeData ƽ̨�˶����������ݽṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* author     ��songjianfeng
	* remark     : NA
	* history    : 2013-3-7 
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetMotionRangeDataPlatform(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode,const IVS_TIME_SPAN* pTimeSpan,const IVS_INDEX_RANGE* pIndexRange,
		IVS_MOTION_RANGE_DATA_LIST* pMotionRangeData,IVS_UINT32 uiBufferSize);

	/*************************************************************************
	* name       : IVS_SDK_AddBookmarkForBackup
	* description: ����¼����ǩ(���ݷ�����)
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddBookmarkForBackup(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,
		const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime);

	/*************************************************************************
	* name       : IVS_SDK_ModifyBookmarkForBackup
	* description: �޸�¼����ǩ(���ݷ�����)
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModifyBookmarkForBackup(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName);
	/*************************************************************************
	* name       : IVS_SDK_DeleteBookmarkForBackup
	* description: ɾ��¼����ǩ(���ݷ�����)
	* input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteBookmarkForBackup(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID);

	/*************************************************************************
	* name       : IVS_SDK_GetBookmarkListForBackup
	* description: ��ѯ¼����ǩ�б�(���ݷ�����)
	* input      : iSessionID ��¼�ɹ���ĻỰID
	               pUnifiedQuery ͨ�ò�ѯ�ṹ
				   pBookmarkList ����¼����ǩ��ѯ�б�
				   uiBufSize �����ڴ��С
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ش�����
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetBookmarkListForBackup(IVS_INT32 iSessionID, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize);

    /******************************************************************
    function : IVS_SDK_OMUQueryDevReport
    description: ʵʱ�����ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDevReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryDevReportDetail
    description: ʵʱ���������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDevReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRealtimeReport
    description: ʵʱ�����ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRealtimeReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRealtimeReportDetail
    description: ʵʱ���������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRealtimeReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryQDReport
    description: ��Ƶ������ϲ�ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryQDReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryQDReportDetail
    description: ��Ƶ������������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryQDReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRecordPlanReport
    description: ¼��ִ�мƻ������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordPlanReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRecordPlanReportDetail
    description: ¼��ִ�мƻ����������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordPlanReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRecordIntegralityReport
    description: ¼�������Ա����ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordIntegralityReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryRecordIntegralityReportDetail
    description: ¼�������Ա��������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRecordIntegralityReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryPacketLossRateReport
    description: �����ʱ����ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryPacketLossRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryPacketLossRateReportDetail
    description: �����ʱ��������ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryPacketLossRateReportDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

    /******************************************************************
    function : IVS_SDK_OMUQueryDiskUsageRateReport
    description: ���̴洢ռ���ʱ����ѯ
    input : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
    input : const IVS_CHAR * pReqXml    ��ѯ����
    input : IVS_CHAR * * pRspXml        ��ѯ���
    output : NA
    return : �ɹ�����0��ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskUsageRateReport(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	 /******************************************************************
    function			: IVS_SDK_OMUQueryDiskProperty
    description	: ��ѯ������Ϣ�ӿ�
    input				: iSessionID ��¼�ɹ���ĻỰID
    output			: pRspXml ���XML
	return				: �ɹ�����0��ʧ�ܷ��ش�����
	author			: zhuohaofan zwx211831
    remark			: pRspXml��SDK�ڲ������ڴ棬ʹ����Ӧ����IVS_SDK_ReleaseBuffer�ͷš�
    return				: NA
    *******************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryDiskProperty(IVS_INT32 iSessionID, 
		IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_GetPrerecordCameraList
	     description: ��ѯ����Ԥ¼��������б�
	     input      : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID;
	     output     : IVS_CHAR * * pRspXml  ��ѯ����Ԥ¼��������б�����ϢXML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetPrerecordCameraList(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_SetDevGroupRemark
	     description: �����豸�鱸ע��Ϣ
	     input      : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID;
	     input      : const IVS_CHAR* pDomainCode  �豸�����������
		 input      : IVS_UINT32 uiDevGroupID  �豸��ID
		 input      : const IVS_CHAR* pDevGroupRemark �豸�鱸ע
		 output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevGroupRemark(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pDevGroupRemark);

	/*****************************************************************
	     function   : IVS_SDK_GetDevGroupRemark
	     description: ��ѯ�豸�鱸ע��Ϣ
	     input      : IVS_INT32 iSessionID ��¼�ɹ���ĻỰID;
	     input      : const IVS_CHAR* pDomainCode  �豸�����������
		 input      : IVS_UINT32 uiDevGroupID  �豸��ID
		 input      : IVS_CHAR* pDevGroupRemark �豸�鱸ע
		 input      : IVS_UINT32 uiRemarkLen ��ע�ַ����ȣ����324�ֽڣ�
		 output     : NA
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDevGroupRemark(IVS_INT32 iSessionID, const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, IVS_CHAR* pDevGroupRemark, IVS_UINT32 uiRemarkLen);

	/*****************************************************************
	     function   : IVS_SDK_OMUAddAlarmLinkageRule
	     description: �����澯֪ͨ����ӿ�
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  ��������XML
	     output     : IVS_CHAR * * pRspXml      ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUAddAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_OMUModAlarmLinkageRule
	     description: �޸ĸ澯֪ͨ����ӿ�
	     input      : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml   ��������XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUModAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*****************************************************************
	     function   : IVS_SDK_OMUDelAlarmLinkageRule
	     description: ɾ���澯֪ͨ����ӿ�
	     input      : IVS_INT32 iSessionID        ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml    ��������XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUDelAlarmLinkageRule(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

	/*****************************************************************
	     function   : IVS_SDK_OMUGetAlarmLinkageRuleList
	     description: ��ȡ�澯֪ͨ�����б�ӿ�
	     input      : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml   ��ѯ����
	     output     : IVS_CHAR * * pRspXml       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetAlarmLinkageRuleList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_OMUGetAlarmLinkageRuleDetail
	     description: ��ȡ�澯֪ͨ��������ӿ�
	     input      : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml   ��������XML
	     output     : IVS_CHAR * * pRspXml        ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetAlarmLinkageRuleDetail(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_OMUGetDevTypeByNodeCode
	     description: ������Ԫ��ȡ�豸���ͽӿ�   
	     output     : IVS_INT32 iSessionID       ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pReqXml   ��������XML
	     output     : IVS_CHAR * * pRspXml       ���XML
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUGetDevTypeByNodeCode(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/*****************************************************************
	     function   : IVS_SDK_DeleteSnapshot
	     description: ɾ��ƽ̨ץ��ͼƬ
	     output     : IVS_INT32 iSessionID              ��¼�ɹ���ĻỰID
	     output     : const IVS_CHAR * pCameraCode      ���������
	     output     : IVS_UINT32 uiPictureID            ͼƬID
	     return     : 
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DeleteSnapshot(IVS_INT32 iSessionID, const IVS_CHAR* pCameraCode, IVS_UINT32 uiPictureID);

	/*************************************************************************
	* name       : IVS_SDK_GetExDomainRecordPolicy
	* description: ��ѯ�������������¼�����
	* input      : pReqXml ����XML
	* output     : pRspXml ��ӦXML
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainRecordPolicy(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_SetExDomainRecordPolicy
	* description: �����������������¼�����
	* input      : pReqXml ����XML
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetExDomainRecordPolicy(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml);

   /*************************************************************************
	* name       : IVS_SDK_SetExDomainRecordPolicy
	* description: �����������������¼��ƻ�
	* input      : pReqXml ����XML
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_AddExDomainRecordPlan(
        IVS_INT32 iSessionID, 
        IVS_UINT32 uiDeviceNum,
        const IVS_DEVICE_CODE* pDeviceList,
        const IVS_RECORD_PLAN* pRecordPlan,
        IVS_RESULT_LIST* pResultList,
        IVS_UINT32 uiResultBufferSize);
    /*************************************************************************
	* name       : IVS_SDK_ModExDomainRecordPlan
	* description: �޸��������������¼��ƻ�
	* input      : pReqXml ����XML
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ModExDomainRecordPlan(
        IVS_INT32 iSessionID, 
        IVS_UINT32 uiDeviceNum,
        const IVS_DEVICE_CODE* pDeviceList,
        const IVS_RECORD_PLAN* pRecordPlan,
        IVS_RESULT_LIST* pResultList,
        IVS_UINT32 uiResultBufferSize);

    /*************************************************************************
	* name       : IVS_SDK_DelExDomainRecordPlan
	* description: ɾ���������������¼��ƻ�
	* input      : pReqXml ����XML
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_DelExDomainRecordPlan(
        IVS_INT32 iSessionID,
        IVS_UINT32 uiDeviceNum,
        const IVS_DEVICE_CODE* pDeviceList,
        IVS_UINT32 uiRecordMethod,
        IVS_RESULT_LIST* pResultList,
        IVS_UINT32 uiResultBufferSize);

    /*************************************************************************
	* name       : IVS_SDK_GetExDomainRecordPlan
	* description: ɾ���������������¼��ƻ�
	* input      : pReqXml ����XML
	               iSessionID  ��¼�ɹ���ĻỰID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExDomainRecordPlan(
        IVS_INT32 iSessionID,
        const IVS_CHAR* pCameraCode,
        IVS_UINT32 uiRecordMethod,
        IVS_RECORD_PLAN* pRecordPlan);


	/*****************************************************************
	     function   : IVS_SDK_GetDeviceCapability
	     description: ��ѯ�������������
	     input      : IVS_INT32 iSessionID             ��¼�ɹ���ĻỰID
	     input      : IVS_UINT32 uiCapabilityType      �������ͣ�IVS_DEVICE_CAPABILITY_TYPE��DEVICE_ENCODE_CAPABILITY
	     input      : const IVS_CHAR* pDevCode         �豸����
		 output     : IVS_CHAR** pRspXml               ��ѯXML
	     return     : 
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceCapability(IVS_INT32 iSessionID, IVS_UINT32 uiCapabilityType, const IVS_CHAR* pDevCode, IVS_CHAR** pRspXml);

    /*********************************************************************************************
    * function       : SetListenAddr
    * description: ���÷������˼���IP�Ͷ˿ں�
    * input      : iPort�������˼����˿ں�
    * output     : NA
    * return     : NA
    * remark     : NA
    *********************************************************************************************/
    //IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetNetKeyBoardPort(ULONG ulPort);

    /*********************************************************************************************
    * name       : StartListenThread
    * description: ���������߳�
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : g_hThread
    * remark     : NA
    *********************************************************************************************/
    //IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartNetKeyBoard(LPCallBack lpCallBack);

    /*********************************************************************************************
    * name       : ReleaseThread
    * description: �ͷ��߳�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    *********************************************************************************************/
    //IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopNetKeyBoard();

    /******************************************************************
     function   : IVS_SDK_SetLogPath
     description: ������־��Ŀ¼
     input      : const IVS_CHAR * pLogPath ��־��Ŀ¼
     return     : IVS_INT32 �ɹ�����0 ʧ�ܷ��ش�����
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetLogPath(const IVS_CHAR* pLogPath);

	/*********************************************************************************************
	     function   : IVS_SDK_SetDeviceGuard
	     description: �豸������
	     input      : IVS_INT32 iSessionID             ��¼�ɹ���ĻỰID
	     input      : IVS_CHAR* pReqXml                ������Ϣ
		 output     : NA
	     return     : �ɹ�����0�� ʧ�ܷ��ش�����
	**********************************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDeviceGuard(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);
	/*****************************************************************
	     function   : IVS_SDK_CheckNewVersion
	     description: �ͻ����������
	     input      : IVS_INT32 iSessionID      ��¼�ɹ���ĻỰID
	     input      : const IVS_CHAR * pReqXml  �����汾У��xml
	     output     : IVS_CHAR * * pRspXml      �����豸¼�������Ӧxml
	     return     : �ɹ�����0��ʧ�ܷ��ش�����
	*****************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_CheckNewVersion(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name       : IVS_SDK_StopBackupPlayBack
    * description: ֹͣ�ͻ��˱���¼��ط�
    * input      : iSessionID      �ỰID
				   ulPlayHandle    ���ž��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    **************************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopBackupPlayBack(IVS_INT32 iSessionID,
                                                       IVS_ULONG ulPlayHandle);
	/**************************************************************************
    * name       : IVS_SDK_StartBackupPlayBack
    * description: ��ʼ�ͻ��˱���¼��ط�
    * input      : iSessionID      �ỰID
				   pCameraCode     ���������
                   pPlaybackParam  �طŲ���
    * output     : pHandle         ���ž��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * remark     : NA
    **************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartBackupPlayBack(IVS_INT32 iSessionID,
		                                                           const IVS_CHAR* pDomainCode,
		                                                           const IVS_CHAR* pNVRCode,
                                                                   const IVS_CHAR* pCameraCode,
                                                                   const IVS_PLAYBACK_PARAM* pPlaybackParam,
                                                                   HWND hWnd,
                                                                   IVS_ULONG* pHandle);
#endif

  /******************************************************************
  function : IVS_SDK_GetCameraThumbnail
  description: ��ȡ�����Ԥ��ͼ�ӿ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  const IVS_CHAR*	pReqXml			����CBB::CBB::CXml��
  output : IVS_CHAR**		pRspXml			��ӦCBB::CBB::CXml
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetCameraThumbnail(IVS_INT32 iSessionID,const IVS_CHAR* pCameraCode, IVS_CHAR* pThumbnailURL,IVS_UINT32 uiBufferSize);


  	/******************************************************************
     function   : IVS_SDK_StartBackupDownload
     description: ��ʼ����¼������
     input      : IVS_ULONG ulPlayHandle
     input      : IVS_PLAYBACK_TIME * pPlayBackTime
	 input      : IVS_CHAR* pCameraCode
	 input      : IVS_DOWNLOAD_PARAM* pDownloadParam
	 input      : IVS_ULONG*  pulHandle
     output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
#ifdef WIN32
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StartBackupDownload(IVS_INT32 iSessionID,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,
		const IVS_DOWNLOAD_PARAM* pDownloadParam, IVS_ULONG* pulHandle);

	/******************************************************************
     function   : IVS_SDK_StopBackupDownload
     description: ֹͣ����¼������
     input      : IVS_INT32 iSessionID
	 input      : IVS_ULONG ulHandle
	 output     : NA
     return     : IVS_SDK_API IVS_INT32 __SDK_CALL
    *******************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_StopBackupDownload(IVS_INT32 iSessionID, IVS_ULONG ulHandle);
#endif
  /******************************************************************
  function : IVS_SDK_SetDevPWD
  description: ����ǰ���豸����
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  const IVS_CHAR*	pDevCode		�豸���룻
		  const IVS_CHAR* pDevUserName      �豸��¼�û���
		  const IVS_CHAR* pDevPWD           �豸��¼����
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetDevPWD(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pDevUserName, const IVS_CHAR* pDevPWD, const IVS_CHAR* pDevRegPWD);

  /******************************************************************
  function : IVS_SDK_ImportDevCfgFile
  description: ����ǰ�������ļ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  const IVS_CHAR*	pDevCode		�豸����
		  const IVS_CHAR*   pCFGFile        �����ļ�ȫ·��
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ImportDevCfgFile(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pCFGFile);

  /******************************************************************
  function : IVS_SDK_ExportDevCfgFile
  description: ����ǰ�������ļ�
  input : IVS_INT32			iSessionID		��¼�ɹ��󷵻صĻỰID��
		  const IVS_CHAR*	pDevCode		�豸����
		  const IVS_CHAR*   pCFGFile        �����ļ�ȫ·��
  output : NA
  return : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExportDevCfgFile(IVS_INT32 iSessionID, const IVS_CHAR* pDevCode, const IVS_CHAR* pCFGFilePath);

  /********************************************************************************************
  function	  : IVS_SDK_ResetAlarm
  description : ������λ
  input		  : iSessionID		��¼�ɹ��󷵻صĻỰID��
			    pReqXml		    ������Ϣ
  output	  : NA
  return	  : �ɹ�����0��ʧ�ܷ��ش�����
  *********************************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ResetAlarm (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

  /********************************************************************************************
  function     : IVS_SDK_ExEventSubscribe
  description  : �ⲿ��澯�¼�����
  input		   : iSessionID		��¼�ɹ��󷵻صĻỰID��
		         pReqXml		    ������Ϣ
  output       : NA
  return       : �ɹ�����0��ʧ�ܷ��ش�����
  *********************************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExEventSubscribe (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

  /********************************************************************************************
  function     : IVS_SDK_GetExEventSubscribe
  description  : ��ѯ�ⲿ��澯�¼�����
  input        : iSessionID		��¼�ɹ��󷵻صĻỰID��
		         pReqXml		    ������Ϣ
  output       : pRspXml, ��Ӧ��Ϣ
  return       : �ɹ�����0��ʧ�ܷ��ش�����
  *********************************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExEventSubscribe (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

  /********************************************************************************************
  function    : IVS_SDK_ExCatalogSubscribe
  description : �ⲿ���豸Ŀ¼����
  input       : iSessionID		��¼�ɹ��󷵻صĻỰID��
		        pReqXml		    ������Ϣ
  output      : NA
  return      : �ɹ�����0��ʧ�ܷ��ش�����
  *********************************************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExCatalogSubscribe (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml);

  /******************************************************************
  function    : IVS_SDK_GetExCatalogSubscribeList
  description : ��ѯ�ⲿ���豸Ŀ¼����
  input       : iSessionID		��¼�ɹ��󷵻صĻỰID��
		        pReqXml		    ������Ϣ
  output      : pRspXml, ��Ӧ��Ϣ
  return      : �ɹ�����0��ʧ�ܷ��ش�����
  *******************************************************************/
  IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetExCatalogSubscribeList (IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

  /*********************************************************************************************
    * name       : IVS_OCX_GetAlarmSnapshotList
    * description: ��ȡ�澯ץ��ͼƬ�б�
    * input      : iSessionID, �ỰID��
	               pReqXml�� ����xml
    * output     : pRsqXml, ��Ӧxml
    * return     : �ɹ�����0�� ʧ�ܷ��ش�����
    * remark     : NA
    *********************************************************************************************/
   IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetAlarmSnapshotList(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRsqXml);

   	/*************************************************************************
	* name       : IVS_SDK_ExGetDeviceStatus
	* description: �豸״̬��ѯ
	* input      : pReqXml ����XML
	* output     : pRspXml ��ӦXML
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExGetDeviceStatus(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml,IVS_CHAR** pRspXml);

	/*************************************************************************
	* name       : IVS_SDK_ExGetDeviceInfo
	* description: �豸��Ϣ��ѯ
	* input      : pReqXml ����XML
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ExGetDeviceInfo(IVS_INT32 iSessionID,const IVS_CHAR* pReqXml);

    /*************************************************************************
	* name       : IVS_SDK_RefreshCameraList
	* description: ˢ��������б�
	* input      : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
    IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_RefreshCameraList(IVS_INT32 iSessionID);

	/*************************************************************************
	* name       : IVS_SDK_GetCameraEncodeCapabilities
	* description: Onvif�豸��ǰ�˻�ȡ����������Χ
	* input      : pCameraCode �������������
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDeviceEncodeCapability(IVS_INT32 iSessionID,const IVS_CHAR* pDevCode,
		IVS_STREAM_INFO_LIST* pStreamInfoList);

    IVS_SDK_API void __SDK_CALL IVS_SDK_SetLogLevel(IVS_ULONG ulLogLevel);
	/**************************************************************************
    * name       : IVS_SDK_OMUQueryRaidGroup
    * description: ��������Ϣ��ѯ
    * input      : iSessionID ��¼�ɹ���ĻỰID
	* output     : pRspXml  ��ӦXML
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * author     ��zhangrui z00233055
	* remark     : NA
	* history    : 2013-12-4
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroup(IVS_INT32 iSessionID, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name       : IVS_SDK_OMUQueryRaidGroupDisk
    * description: ������Ϣ��ѯ
    * input      : iSessionID ��¼�ɹ���ĻỰID
						pReqXml ����XML
	* output     : pRspXml  ��ӦXML
    * return     : �ɹ�����0��ʧ�ܷ��ش�����
    * author     ��zhangrui z00233055
	* remark     : NA
	* history    : 2013-12-4
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_OMUQueryRaidGroupDisk(IVS_INT32 iSessionID, const IVS_CHAR* pReqXml, IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_SetReceiveOMUAlarm
    * description: ����SDK�Ƿ����OMU�豸��澯
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: bReceive �Ƿ����OMU�豸��澯
	* output			: NA
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-5-8
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetReceiveOMUAlarm(
		IVS_INT32 iSessionID,
		IVS_BOOL bReceive);

	/**************************************************************************
    * name			: IVS_SDK_GetVideoDiagnose
    * description: ��ѯ��̨VCN����Ƶ������ϼƻ�
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: pCameraCode ���������
	* output			: pRspXml ��̨VCN����Ƶ������ϼƻ�xml
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: pRspXml��SDK�ڲ������ڴ棬ʹ����Ӧ����IVS_SDK_ReleaseBuffer�ͷš�
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetVideoDiagnose(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_CHAR** pRspXml);

	/**************************************************************************
    * name			: IVS_SDK_SetVideoDiagnose
    * description: ���õ�̨VCN����Ƶ������ϼƻ�
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pReqXml ��̨VCN����Ƶ������ϼƻ�xml
	* output			: NA
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-6-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoDiagnose(IVS_INT32 iSessionID, 
		const IVS_CHAR* pReqXml);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByCam
    * description: ���������Code����ѯ����������ڸ���Щ�û��ṩ��Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: pCameraCode ���������
	* input			: uiBufSize �����ڴ��С
	* output			: pStreamListByCam �����������ý��ҵ���б�
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: C/C++��uiBufSize�̶�Ϊsizeof(IVS_STREAM_LIST_BY_CAM) + 
								(IVS_MAX_STREAM_USER_INFO_NUM -1) * sizeof(IVS_STREAM_USER_INFO)
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByCam(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_CAM* pStreamListByCam);

	/**************************************************************************
    * name			: IVS_SDK_GetStreamListByUser
    * description: �����û����룬��ѯ���û����ڸ�������Щ���������Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode ����ѯ���û����������
	* input			: uiUserID ����ѯ���û�ID
	* input			: uiBufSize �����ڴ��С
	* output			: pStreamListByUser �û����ڵ��õ�ý��ҵ���б�
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: C/C++��uiBufSize�̶�Ϊsizeof(IVS_STREAM_LIST_BY_USER) + 
								(IVS_MAX_STREAM_CAM_INFO_NUM -1) * sizeof(IVS_STREAM_CAM_INFO)
	* history			: 2014-6-24
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetStreamListByUser(IVS_INT32 iSessionID, 
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_USER* pStreamListByUser);

	/**************************************************************************
    * name			: IVS_SDK_ShutdownStreamByUser
    * description: ���������Code���û�ID���رո��û�����ʹ�õ���Ƶҵ��
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pDomainCode �����
	* input			: uiUserID �û�ID
	* input			: pCameraCode ���������
	* input			: iStreamType ��Ƶҵ������ 1-ʵ����2-�ط�
	* input			: iDuration �ر�ʱ�����ڴ�ʱ��֮�ڸ��û������ٴε㲥����λΪ�룬0Ϊ�����ƹر�ʱ��
	* output			: NA
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-6-10
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_ShutdownStreamByUser(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		const IVS_CHAR* pCameraCode,
		IVS_INT32 iStreamType,
		IVS_INT32 iDuration);

	/**************************************************************************
    * name			: IVS_SDK_PasswdValidation
    * description: �û�����������֤
    * input			: iSessionID ��¼�ɹ���ĻỰID
	* input			: pLoginName ��¼�û���
	* input			: pPasswd ����
	* output			: NA
    * return			: �ɹ�����0��ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-6-27
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_PasswdValidation(IVS_INT32 iSessionID,
		const IVS_CHAR* pLoginName,
		const IVS_CHAR *pPasswd);

#ifdef WIN32
	/**************************************************************************
    * name			: IVS_SDK_SetVideoEffect
    * description: ¼����Ƶ��������
	* input			: iSessionID ��¼�ɹ���ĻỰID
    * input			: ulHandle ���ž��
	* input			: lBrightValue ����
	* input			: lContrastValue �Աȶ�
	* input			: lSaturationValue ���Ͷ�
	* input			: lHueValue ɫ��
	* output			: NA
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-9-2
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_SetVideoEffect(IVS_INT32 iSessionID,
		IVS_ULONG ulPlayHandle, IVS_LONG lBrightValue,
		IVS_LONG lContrastValue, IVS_LONG lSaturationValue,
		IVS_LONG lHueValue);
#endif

	/**************************************************************************
    * name			: IVS_SDK_GetDomainDeviceList
    * description: ��ȡָ�������������豸�б�
	* input			: iSessionID ��¼�ɹ���ĻỰID
    * input			: pDomainCode ����룬Ϊ��Ҫ��ѯ�豸����ı���
	* input			: uiDeviceType �豸���ͣ�ֵ�ο�IVS_DEVICE_TYPE��֧�ֳ����豸������澯�豸��������豸
	* input			: pIndexRange ��ҳ��Ϣ
	* output			: pDeviceList �豸�б�ָ��
	* input			: uiBufferSize �����С
    * return			: �ɹ�����0��IVS_SUCCEED����ʧ�ܷ��ش�����
    * author			: zhuohaofan zwx211831
	* remark		: NA
	* history			: 2014-9-9
    **************************************************************************/
	IVS_SDK_API IVS_INT32 __SDK_CALL IVS_SDK_GetDomainDeviceList(IVS_INT32 iSessionID,
		const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiDeviceType,
		const IVS_INDEX_RANGE* pIndexRange,
		IVS_VOID* pDeviceList,
		IVS_UINT32 uiBufferSize);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif //_IVS_SDK_H_

