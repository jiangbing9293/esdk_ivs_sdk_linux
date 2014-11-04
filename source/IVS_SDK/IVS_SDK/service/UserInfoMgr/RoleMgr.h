/********************************************************************
filename : RoleMgr.h
author : pkf57481
created : 2013/01/14
description : ��ɫ������
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2013/01/14 ��ʼ�汾
*********************************************************************/


#ifndef __ROLE_MGR_H_
#define __ROLE_MGR_H_

#include "hwsdk.h"
#include "IVSCommon.h"

class CUserMgr;
class CRoleMgr
{
public:
    CRoleMgr(void);
    ~CRoleMgr(void);

    void SetUserMgr(CUserMgr *pUserMgr);

public:
    // ���ӽ�ɫ
    IVS_INT32 AddRole(const IVS_ROLE_INFO* pRoleInfo, IVS_UINT32* puiRoleID);

    // ɾ����ɫ
    IVS_INT32 DeleteRole(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID);

    // �޸Ľ�ɫ
    IVS_INT32 ModifyRole(const IVS_ROLE_INFO* pRoleInfo);

    // ��ѯ��ɫ�б�
    IVS_INT32 GetRoleList(const IVS_CHAR* pDomainCode, const IVS_INDEX_RANGE* pIndexRange, IVS_ROLE_INFO_LIST* pRoleInfoList, IVS_UINT32 /*uiBufferSize*/);

    // ���ݽ�ɫID��ȡ��ɫ��ϸ��Ϣ
    IVS_INT32 GetRoleInfoByRoleID(const IVS_CHAR* pDomainCode, IVS_UINT32 uiRoleID, IVS_ROLE_INFO* pRoleInfo);

    // �����û�ID��ȡ��ɫ��ϸ��Ϣ
    IVS_INT32 GetRoleInfoByUserID(const IVS_CHAR* pDomainCode, IVS_UINT32 uiUserID, IVS_ROLE_INFO* pRoleInfo);

    // ��ȡĬ���˺Ź���
    IVS_INT32 GetDefaultRoleAccountRule(const IVS_CHAR* pDomainCode, IVS_ROLE_ACCOUNT_RULE* pRoleAccountRule);

    // ��ȡ�û�Ȩ����Ϣ
    IVS_INT32 GetOperationRightGroup(const IVS_CHAR* pDomainCode, IVS_OPERATION_RIGHT_GROUP_LIST* pRightGroupList);

private:
    CUserMgr *m_UserMgr;
};

#endif
