/********************************************************************
filename : DeviceParaConfig.h
author : gwx151720
created : 2012/12/03
description : 
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __DEVICE_PARA_CONFIG_H__
#define __DEVICE_PARA_CONFIG_H__

#include "hwsdk.h"
#include "SDKDef.h"
#include "SDKSecurityClient.h"

#define SENDRET 0
#define SENDSTR 1

class CUserMgr;
class CDeviceParaConfig
{
public:
	CDeviceParaConfig();
	~CDeviceParaConfig();
public:
    void SetUserMgr(CUserMgr *pUserMgr);
	 
	// �����豸����
    IVS_INT32 SetDeviceConfig(const IVS_CHAR* pDevCode, 
                              IVS_UINT32 uiConfigType,
                              const IVS_VOID* pBuffer,
                              IVS_UINT32 uiBufSize)const;
	// ��ȡ�������豸��ϸ��Ϣ
    IVS_INT32 GetAudioDeviceConfig(const IVS_CHAR* pDevCode, 
		                      const IVS_CHAR* pAudioDevCode, 
                              IVS_UINT32 uiConfigType,
                              IVS_VOID* pBuffer,
                              IVS_UINT32 uiBufSize)const;

	// ��ȡ�豸����
	IVS_INT32 GetDeviceConfig(const IVS_CHAR* pDevCode, 
		IVS_UINT32 uiConfigType,
		IVS_VOID* pBuffer,
		IVS_UINT32 uiBufSize)const;
	// �������豸����
    IVS_INT32 SetMainDevConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

private:
    CUserMgr *m_pUserMgr;

	// У���ڴ��С
    IVS_INT32 CheckBufferSize(IVS_UINT32 uiConfigType, IVS_UINT32 uiBufSize)const;

	// ��ѯ�������������
	IVS_INT32 GetCameraBriefCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	// �޸��������������
	IVS_INT32 SetCameraBriefCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	//��ѯ�������������
	IVS_INT32 GetCameraStreamCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ���������ͼ�����
	IVS_INT32 GetCameraBasicImgCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	// �޸����������ͼ�����
	IVS_INT32 SetCameraBasicImgCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;
	
	// ��ȡ��̨����
	IVS_INT32 GetCameraPTZCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	// ������̨����
	IVS_INT32 SetCameraPTZCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ��Ƶ����(�����豸ͨ��)
	IVS_INT32 GetAudioCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode,const IVS_CHAR* pAudioDevCode, const IVS_CHAR* pDomainCode)const;
	
	// ��ѯ��Ƶ����
	IVS_INT32 GetAudioCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode)const;

	// ������Ƶ����
	IVS_INT32 SetAudioCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode)const;

	//�޸��������������
	IVS_INT32 SetCameraStreamCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const;

	//�޸���������������������¼�����ͼ������������Ϣ��
	IVS_INT32 SetCameraStreamCfgEventEx(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const;

	//��ѯ���豸��������
	IVS_INT32 GetMainDevCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸ĸ澯�������豸
	IVS_INT32 SetAlarmInCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�澯�������豸
	IVS_INT32 GetAlarmInCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸ĸ澯������豸
	IVS_INT32 SetAlarmOutCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;
    
	// ��ѯ�澯������豸
	IVS_INT32 GetAlarmOutCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯOSD����
	IVS_INT32 GetOSDPara(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode) const;

	// ��ѯ��˽����
	IVS_INT32 GetVideoMask(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸���˽����
	IVS_INT32 SetVideoMaskCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸�OSD����
	IVS_INT32 SetOsgPara(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸�ͼ���ڵ�
	IVS_INT32 SetVideoHideAlarm(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯͼ���ڵ�
	IVS_INT32 GetVideoHideAlarm(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ����ǰ���ƶ����
	IVS_INT32 SetMotionDetec(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯǰ���ƶ����
	IVS_INT32 GetMotionDetec(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸����豸�������
	IVS_INT32 SetDevNetConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ���豸�������
	IVS_INT32 GetDevNetConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �޸����豸ʱ�����
	IVS_INT32 SetDevTimeConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ���豸ʱ�����
	IVS_INT32 GetDevTimeConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ���ô������豸��ϸ��Ϣ
	IVS_INT32 SetSerialChannelConfig(const IVS_VOID* pBuffer, const IVS_CHAR* pSerialChannelCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�������豸��ϸ��Ϣ
	IVS_INT32 GetSerialChannelConfig(IVS_VOID*& pBuffer, const IVS_CHAR* pSerialChannelCode, const IVS_CHAR* pDomainCode)const;

	// �����������չͼ�����
	IVS_INT32 SetCameraImgCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�������չͼ�����
	IVS_INT32 GetCameraImgCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// �����������չ����
	IVS_INT32 SetCameraExCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�������չ����
	IVS_INT32 GetCameraExCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ���������ץ�Ĳ�������
	IVS_INT32 SetCameraSnapshotCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�����ץ�Ĳ�������
	IVS_INT32 GetCameraSnapshotCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ���������ǰ��¼����������
	IVS_INT32 SetCameraRecordStreamCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�����ǰ��¼����������
	IVS_INT32 GetCameraRecordStreamCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ���������FEC����
	IVS_INT32 SetCameraFECCfg(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	// ��ѯ�����FEC����
	IVS_INT32 GetCameraFECCfg(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const;

	typedef int (CDeviceParaConfig::*FnSetDevicePara)(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const; 

	typedef int (CDeviceParaConfig::*FnGetDevicePara)(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode)const; 

	typedef struct
	{
		IVS_UINT32 reqID;
		FnSetDevicePara fnSetDevicePara;
		FnGetDevicePara fnGetDevicePara;
	} REQUEST_NAME;
	static REQUEST_NAME m_reqNames[];
	FnSetDevicePara GetFnSetDevicePara(IVS_UINT32 enReqID) const;
	FnGetDevicePara GetFnGetDevicePara(IVS_UINT32 enReqID) const;
	int HandleSetDevicePara(const IVS_VOID* pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, IVS_UINT32 uiConfigType)const;
	int HandleGetDevicePara(IVS_VOID*& pBuffer, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, IVS_UINT32 uiConfigType)const;

	// �������豸������Ϣģʽ
	IVS_INT32 SetMainDevConfigMsg(const IVS_DEVICE_CFG* pMainDevCfg, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const char* pszDestModule, const char* pszDeviceCode, enum BP::DOMAIN_TYPE enDomainType = BP::DOMAIN_NONE)const;

	// ��ѯ���豸������Ϣģʽ
	IVS_INT32 GetMainDevCfgMsg(IVS_DEVICE_CFG* pMainDevCfg, const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, enum BP::DOMAIN_TYPE enDomainType = BP::DOMAIN_NONE)const;

};


#endif
