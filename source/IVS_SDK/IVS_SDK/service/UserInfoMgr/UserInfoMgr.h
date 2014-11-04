/********************************************************************
filename : UserInfoMgr.h
author : pkf57481
created : 2012/12/03
description : �û�����
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/


#ifndef __USERINFO_MGR_H_
#define __USERINFO_MGR_H_

#include "IVSCommon.h"
#include "hwsdk.h"
#include "SDKDef.h"

class CUserMgr;
class CUserInfoMgr
{
public:
    CUserInfoMgr(void);
    ~CUserInfoMgr(void);

    void SetUserMgr(CUserMgr* pUserMgr);

public:
    // ����û���Ϣ
    IVS_INT32 AddUser(const IVS_USER_INFO* pUserInfo, IVS_UINT32* puiUserID);

    // �����û���Ϣ
    IVS_INT32 ModifyUser(const IVS_USER_INFO* pUserInfo);

    // ɾ���û�
    IVS_INT32 DeleteUser(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    // ��ѯ�û���Ϣ
    IVS_INT32 GetUserInfo(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_INFO* pUserInfo);

    // ����/�����û�
    IVS_INT32 LockUser(const IVS_CHAR* pDomainCode, IVS_INT32 /*iSessionID*/, IVS_UINT32 uiUserID, IVS_BOOL bLock);

    // ��ѯ�û��б�
    IVS_INT32 GetUserList(const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_USER_INFO_LIST* pUserList, IVS_UINT32 /*uiBufferSize*/);

    // ��ѯ�����û��б�
    IVS_INT32 GetOnlineUser(const IVS_CHAR* pDomainCode, const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, IVS_ONLINE_USER_LIST* pOnlineUserList, IVS_UINT32 /*uiBufferSize*/);

    // �����û��豸�б�
    IVS_INT32 SetUserDeviceList(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 /*uiBufferSize*/);

    //��ȡ�û��豸�б�
    IVS_INT32 GetUserDeviceList(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, const IVS_INDEX_RANGE* pIndexRange, IVS_USER_DEVICE_INFO_LIST* pUserDevList, IVS_UINT32 /*uiBufferSize*/);

    // �޸�����
    IVS_INT32 ChangePWD(const IVS_CHAR* pOldPWD, const IVS_CHAR* pNewPWD);

    // ��������
    IVS_INT32 ResetPWD(const IVS_CHAR* pDomainCode, const IVS_UINT32 uiUserID, const IVS_CHAR* pNewPWD);

    // У������������Ƿ�����˺Ź���
    IVS_INT32 CheckPWDByRole(const IVS_CHAR* pDomainCode, const IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, const IVS_CHAR* pPWD);

    // ���ݽ�ɫ��ȡ�����˺Ź��������
    IVS_INT32 GetAutoCreatePWD(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, const IVS_CHAR* pLoginName, IVS_CHAR* pNewPWD, IVS_UINT32 /*uiPWDLen*/);

    // ��ȡ���������Ϣ
    IVS_INT32 GetPWDExpireInfo(IVS_PWD_EXPIRE_INFO* pPWDExpireInfo);

    // �����û�IDǩ���û������лỰ
    IVS_INT32 SignOutUser(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID);

    // ���ݽ�ɫIDǩ���û������лỰ
    IVS_INT32 SignOutUserByRole(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    // ǩ�������û��Ự
    IVS_INT32 SignOutOnlineUser(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserOnlineID);

	/******************************************************************
	function			: GetStreamListByCam
	description	: ���������Code����ѯ����������ڸ���Щ�û��ṩ��Ƶҵ��
	author			: zwx211831
	input				: pDomainCode �����
	input				: pCameraCode ���������
	input				: uiBufSize �����ڴ��С
	output			: pStreamListByCam �����������ý��ҵ���б�
	return				: �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_INT32 GetStreamListByCam(const IVS_CHAR* pDomainCode,
		const IVS_CHAR* pCameraCode,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_CAM* pStreamListByCam)const;

	/******************************************************************
	function			: GetStreamListByUser
	description	: �����û����룬��ѯ���û����ڸ�������Щ���������Ƶҵ��
	author			: zwx211831
	input				: pDomainCode �����
	input				: uiUserID �û�ID
	input				: uiBufSize �����ڴ��С
	output			: pStreamListByUser �û����ڵ��õ�ý��ҵ���б�
	return				: �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_INT32 GetStreamListByUser(const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,
		IVS_UINT32 uiBufSize,
		IVS_STREAM_LIST_BY_USER* pStreamListByUser)const;

	/******************************************************************
	function			: ShutdownStreamByUser
	description	: ���������Code���û�ID���رո��û�����ʹ�õ���Ƶҵ��
	author			: zwx211831
	input				: pDomainCode �����
	input				: uiUserID �û�ID
	input				: pCameraCode ���������
	input				: iStreamType ��Ƶҵ������ 1-ʵ����2-�ط�
	input				: iDuration �ر�ʱ�����ڴ�ʱ��֮�ڸ��û������ٴε㲥����λΪ�룬0Ϊ�����ƹر�ʱ��
	output			: NA
	return				: �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_INT32 ShutdownStreamByUser(const IVS_CHAR* pDomainCode,
		IVS_UINT32 uiUserID,  
		const IVS_CHAR* pCameraCode,
		IVS_INT32 iStreamType,
		IVS_INT32 iDuration)const;

	/******************************************************************
	function			: PasswdValidation
	description	: �û�����������֤
	author			: zwx211831
	input				: pLoginName ��¼�û���
	input				: pPasswd ����
	output			: NA
	return				: �ɹ�����0��ʧ�ܷ��ش�����
	*******************************************************************/
	IVS_INT32 PasswdValidation(const IVS_CHAR* pLoginName,
		const IVS_CHAR *pPasswd)const;

private:
    IVS_INT32 SetUserDeviceListInUserDomain(const IVS_CHAR* pUserDomainCode, IVS_UINT32 uiUserID, const std::list<IVS_USER_DEVICE_INFO *> &pUserDevList);

    // ��ѯ�豸���б�
    IVS_INT32 GetDeviceGroup(const IVS_CHAR* pDomainCode, const IVS_CHAR* cDevGroupCode, IVS_BOOL bIsExDomain, std::vector<IVS_USER_DEVICE_INFO> &userDevList);

    // ��ѯ�豸���б� -- �����ڴ�
    IVS_INT32 AllocateDevGroupSpace(IVS_DEVICE_GROUP_LIST* &pDevGroupList, IVS_UINT32* iBufferSize)const;

    IVS_INT32 GetCurrentUserDev(IVS_USER_DEVICE_INFO_LIST* &userDevList);

    IVS_INT32 GetUserDevList(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_USER_DEVICE_INFO_LIST* &pUserDevList);

    IVS_INT32 AllocateUserDevListSpace(IVS_UINT32 uiToIndex, IVS_INDEX_RANGE* pIndexRange, IVS_USER_DEVICE_INFO_LIST* &pUserDevList, IVS_UINT32* iBufferSize)const;


private:
    CUserMgr* m_UserMgr;

    IVS_ROLE_INFO m_stRoleInfo; 
};

#endif
