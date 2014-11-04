/********************************************************************
	filename	: 	DeviceGroupMgr.h 
	author		:	gwx151720
	created		:	2012/11/01	
	description	:	�豸������࣬�ṩ��cuʹ��
	copyright	:	Copyright (C) 2012
	history		:	2012/11/01
*********************************************************************/
#ifndef __DEVICE_RROUP_Mgr_H__
#define __DEVICE_RROUP_Mgr_H__

#include "hwsdk.h"
#include "SDKDef.h"

#define SENDRET 0
#define SENDSTR 1
//2012-11-7 2012-11-9
class CUserMgr;
class CDeviceGroupMgr
{
public:
	CDeviceGroupMgr(void);
	~CDeviceGroupMgr(void);

public:
	void SetUserMgr(CUserMgr *pUserMgr);

	/*************************************************************************
	* name       : AddDeviceGroup
	* description: �����豸��
	* input      : pGroupInfo:�豸��ṹ��; uiDevGroupID:�豸��ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 AddDeviceGroup(const IVS_DEV_GROUP* pDevGroupInfo,IVS_UINT32* uiDevGroupID)const;	

	/*************************************************************************
	* name       : DeleteDeviceFromGroup
	* description: ɾ���豸��������
	* input      : pTargetDomainCode:�����; uiDeviceNum:�豸����; pGroupCameraList:�豸��������ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 DeleteDeviceFromGroup(const IVS_CHAR* pTargetDomainCode,
                                    IVS_UINT32 uiDeviceNum,
                                    const IVS_DEV_GROUP_CAMERA* pGroupCameraList)const;

	/*************************************************************************
	* name       : AddDeviceToGroup
	* description: �����豸��������
	* input      : pTargetDomainCode:�����; uiTargetDevGroupID:�豸��ID; uiDeviceNum:�豸����; pGroupCameraList:�豸��������ṹ��
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 AddDeviceToGroup(const IVS_CHAR* pTargetDomainCode,
                               IVS_UINT32 uiTargetDevGroupID,
                               IVS_UINT32 uiDeviceNum,
                               const IVS_DEV_GROUP_CAMERA* pGroupCameraList)const;
    
	/*************************************************************************
	* name       : GetDeviceGroup
	* description: ��ѯ�豸���б�	
	* input      : pDomainCode:�����; uiDevGroupID:�豸��ID; pIndexRange:ͨ�ò�ѯ��Χ�ṹ��;pDeviceGroupList:�豸���б�ṹ��;uiBufferSize:�����ڴ��С;
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/	
    IVS_INT32 GetDeviceGroup(const IVS_CHAR* pDomainCode,
                             const IVS_CHAR* pDevGroupCode,
                             IVS_DEVICE_GROUP_LIST* pDeviceGroupList,
                             IVS_UINT32 uiBufferSize);
 	
 	/*************************************************************************
	* name       : DeleteDeviceGroup
	* description: ɾ���豸��
	* input      : pDomainCode:�����
				   uiDevGroupID:�豸��ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 DeleteDeviceGroup(const IVS_CHAR* pDomainCode,
                                IVS_UINT32 uiDevGroupID)const;

	/*************************************************************************
	* name       : MoveDeviceGroup
	* description: �ƶ��豸��
	* input      : pDomainCode:�����
				   uiDevGroupID:�豸��ID
				   uiNewParentDevGroupID:�¸��豸��ID
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 MoveDeviceGroup(const IVS_CHAR* pDomainCode,
                              IVS_UINT32 uiNewParentDevGroupID,
                              IVS_UINT32 uiDevGroupID)const;

	/*************************************************************************
	* name       : ModifyDeviceGroupName
	* description: �޸��豸������
	* input      : pDomainCode:�����
				   uiDevGroupID:�豸��ID
				   pNewDevGroupName:���豸������
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* author     ��guandiqun
	* remark     : NA
	* history    : 2012-11-06
	*************************************************************************/
    IVS_INT32 ModifyDeviceGroupName(const IVS_CHAR* pDomainCode,
                                IVS_UINT32 uiDevGroupID,
                                const IVS_CHAR* pNewDevGroupName)const;	
	/*************************************************************************
	* name       : SetDevGroupRemark
	* description: �����豸�鱸ע��Ϣ
	* input      : pDomainCode:�����
				   uiDevGroupID:�豸��ID
				   pDevGroupRemark:�豸�鱸ע��Ϣ
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	*************************************************************************/
	IVS_INT32 SetDevGroupRemark(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, const IVS_CHAR* pDevGroupRemark)const;

	/*************************************************************************
	* name       : GetDevGroupRemark
	* description: ��ѯ�豸�鱸ע��Ϣ
	* input      : pDomainCode:�����
				   uiDevGroupID:�豸��ID
				   pDevGroupRemark:�豸�鱸ע��Ϣ
				   uiRemarkLen:��ע�ַ����ȣ����324�ֽڣ�
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	*************************************************************************/
	IVS_INT32 GetDevGroupRemark(const IVS_CHAR* pDomainCode, IVS_UINT32 uiDevGroupID, IVS_CHAR* pDevGroupRemark, IVS_UINT32 uiRemarkLen)const;

private:
	CUserMgr *m_pUserMgr;

};

#endif
