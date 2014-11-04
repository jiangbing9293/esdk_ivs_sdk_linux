/********************************************************************
	filename	: 	UpgradeMgr.h
	author		:	w00165153
	created		:	2013/11/13
	description	:	����������;
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2013/11/13 ��ʼ�汾;
*********************************************************************/
#ifndef __UPGRADEMGR_H__
#define __UPGRADEMGR_H__
#include "hwsdk.h"
#include "ivs_error.h"


class CUserMgr;
class CUpgradeMgr
{
public:
    CUpgradeMgr(void);
    ~CUpgradeMgr(void);
public:
    // �����û�������
    void SetUserMgr(CUserMgr* pUserMgr);
    IVS_INT32 GetCUVersion(const IVS_CU_INFO* pCUInfo, IVS_UPGRADE_INFO* pUpgradeInfo);
private:
    CUserMgr *m_pUserMgr; //lint !e830 //����Ҫ//
};

#endif