/********************************************************************
	filename	: 	PtzControl.h
	author		:	z00201790
	created		:	2012/12/06	
	description	:	������̨������;
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/
#ifndef __PTZCONTROL_H__
#define __PTZCONTROL_H__
#include "hwsdk.h"
#include "ivs_error.h"

class CUserMgr;
class CPtzControl
{
public:
	CPtzControl(void);
	~CPtzControl(void);

public:
	// �����û�������
	void SetUserMgr(CUserMgr* pUserMgr);

	/**************************************************************************
    * name       : GetPtzAbsPosition
    * description: ��ȡ��̨��ǰλ��
    * input      : pCameraCode  ���������
	* output     : pPTZPosInfo  ��̨λ����Ϣ
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuohaofan zwx211831
	* remark     : Add
	* history    : 2014-3-15
    **************************************************************************/
	IVS_INT32 GetPtzAbsPosition(const IVS_CHAR* pCameraCode, IVS_PTZ_ABSPOSITION* pPTZPosInfo);

	// �ƾ�����
	IVS_INT32 PtzControl(const IVS_CHAR* pCameraCode, IVS_INT32 iControlCode,const IVS_CHAR* pControlPara1,const IVS_CHAR* pControlPara2, IVS_PTZ_CONTROL_INFO* pPtzControlInfo);

	/**************************************************************************
    * name       : AddPTZPreset
    * description: ����Ԥ��λ
    * input      : pCameraCode  ���������
				   pPresetName  Ԥ��λ����
				   uiPresetIndex Ԥ��λ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  AddPTZPreset (const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName, IVS_UINT32* uiPresetIndex);

	/**************************************************************************
    * name       : DelPTZPreset
    * description: ɾ��Ԥ��λ
    * input      : pCameraCode  ���������
				   uiPresetIndex  Ԥ��λ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  DelPTZPreset (const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name       : ModPTZPreset
    * description: �޸�Ԥ��λ
    * input      : pCameraCode  ���������
				   pPTZPreset  Ԥ��λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  ModPTZPreset (const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset);

	/**************************************************************************
    * name       : GetPTZPresetList
    * description: ��ѯԤ��λ�б�
    * input      : pCameraCode  ���������
				   pPTZPresetList Ԥ��λ��Ϣ�б�
				   uiBufferSize �����С
				   uiPTZPresetNum ʵ��Ԥ��λ����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  GetPTZPresetList (const IVS_CHAR* pCameraCode, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiPTZPresetNum);

	/**************************************************************************
    * name       : SetGuardPos
    * description: ���ÿ���λ
    * input      : pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  SetGuardPos (const IVS_CHAR* pCameraCode, const IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name       : DelGuardPos
    * description: ɾ������λ
    * input      : pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��xusida WX168257
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  DelGuardPos (const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex);

	/**************************************************************************
    * name       : GetGuardPos
    * description: ��ȡ����λ
    * input      : pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  GetGuardPos (const IVS_CHAR* pCameraCode, IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name       : AddCruiseTrack
    * description: ����Ѳ���켣
    * input      : pCameraCode  ���������
				   pCuriseTrackInfo  Ѳ���켣��Ϣ
				   uiTrackIndex �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  AddCruiseTrack (const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, IVS_UINT32* uiTrackIndex);

	/**************************************************************************
    * name       : DelCruiseTrack
    * description: ɾ��Ѳ���켣
    * input      : pCameraCode  ���������
				   uiTrackIndex  �켣����
				   uiCruiseType �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  DelCruiseTrack (const IVS_CHAR* pCameraCode, IVS_UINT32 uiTrackIndex, IVS_UINT32 uiCruiseType);

	/**************************************************************************
    * name       : ModCruiseTrack 
    * description: �޸�Ѳ���켣
    * input      : pCameraCode  ���������
				   pCuriseTrackInfo  Ѳ���켣��Ϣ
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  ModCruiseTrack (const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo);

	/**************************************************************************
    * name       : GetCruiseTrackList
    * description: ��ѯѲ���켣�б�
    * input      : pCameraCode  ���������
				   pCruiseTrackInfoList Ѳ���켣�б�
				   uiBufferSize �����С
				   uiCruiseTrackNum ʵ�ʲ�ѯ����Ѳ���켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  GetCruiseTrackList (const IVS_CHAR* pCameraCode, IVS_CRUISE_TRACK* pCruiseTrackList, IVS_UINT32 uiBufferSize, IVS_UINT32* uiCruiseTrackNum);

	/**************************************************************************
    * name       : GetCruiseTrack
    * description: ��ѯѲ���켣��Ϣ
    * input      : pCameraCode  ���������
				   uiTrackIndex  �켣����
				   pCruiseTrackInfo �켣����
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  GetCruiseTrack (const IVS_CHAR* pCameraCode, IVS_INT32 uiTrackIndex, IVS_CRUISE_TRACK* pCruiseTrackInfo);

	/**************************************************************************
    * name       : SetCruisePlan
    * description: ����Ѳ���ƻ�
    * input      : pCameraCode  ���������
				   pCruisePlan  Ѳ���ƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  SetCruisePlan (const IVS_CHAR* pCameraCode, const IVS_CRUISE_PLAN* pCruisePlan);

	/**************************************************************************
    * name       : GetCruisePlan
    * description: ��ѯѲ���ƻ�
    * input      : pCameraCode  ���������
				   pCruisePlan  Ѳ���ƻ�
	* output     : NA
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    * author     ��zhuwenbin z00193167
	* remark     : NA
	* history    : 2013-1-22
    **************************************************************************/
	IVS_INT32  GetCruisePlan (const IVS_CHAR* pCameraCode, IVS_CRUISE_PLAN* pCruisePlan);

private:
	CUserMgr *m_pUserMgr; //lint !e830 //����Ҫ//
};

#endif
