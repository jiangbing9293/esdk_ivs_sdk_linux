//#pragma once
/********************************************************************
	filename	: 	CapturePicture.h
	author		:	s00193168
	created		:	2013/1/24	
	description	:	����ץ�Ĺ�����;
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2013/1/24 ��ʼ�汾;
*********************************************************************/
#ifndef __CAPTUREPICTURE_H__
#define __CAPTUREPICTURE_H__
#include "hwsdk.h"
#include "ivs_error.h"

class CUserMgr;

class CCapturePicture
{
public:
	CCapturePicture(void);
	~CCapturePicture(void);
	// �����û�������
	void SetUserMgr(CUserMgr* pUserMgr);

	/**************************************************************************
    * name       : PlatformSnapshot
    * description: ƽ̨�ֶ�ץ��
    * input      : pCameraCode  ���������
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
	IVS_INT32  PlatformSnapshot(const IVS_CHAR* pCameraCode)const;

	/**************************************************************************
    * name       : SetSnapshotPlan
    * description: ����ƽ̨ץ�ļƻ�
    * input      : pCameraCode  ���������
	               pSnapShotPlan ץ�ļƻ���Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
	IVS_INT32  SetSnapshotPlan(const IVS_CHAR* pCameraCode,const IVS_SNAPSHOT_PLAN* pSnapShotPlan)const;

	/**************************************************************************
    * name       : GetSnapshotPlan
    * description: ��ѯƽ̨ץ�ļƻ�
    * input      : pCameraCode  ���������
	               pSnapShotPlan ץ�ļƻ���Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��songjianfeng s00193168
	* remark     : NA
	* history    : 2013-1-24
    **************************************************************************/
	IVS_INT32  GetSnapshotPlan(const IVS_CHAR* pCameraCode,IVS_SNAPSHOT_PLAN* pSnapShotPlan)const;
	
	// ��ȡƽ̨ץ��ͼ���б�
	IVS_INT32 GetSnapshotList(const IVS_CHAR* pCameraCode, const IVS_QUERY_SNAPSHOT_PARAM* pQueryParam, IVS_SNAPSHOT_INFO_LIST* pSnapshotList, IVS_UINT32 uiBufSize) const;

	// ɾ��ƽ̨ץ�ļƻ�
	IVS_INT32 DelSnapshotPlan(const IVS_CHAR* pCameraCode) const;

	// ɾ��ƽ̨ץ��ͼƬ
	IVS_INT32 DeleteSnapshot(const IVS_CHAR* pCameraCode, IVS_UINT32 uiPictureID)const;
private:
	CUserMgr *m_pUserMgr; //lint !e830 //����Ҫ//
};

#endif

