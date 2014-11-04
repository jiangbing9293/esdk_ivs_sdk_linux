/********************************************************************
filename : DeviceParaConfigXMLProcess.h
author : gwx151720
created : 2012/12/03
description : 
copyright : Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history : 2012/12/03 ��ʼ�汾
*********************************************************************/
#ifndef __DEVICE_PARA_CONFIG_XML_PROCESS_H__
#define __DEVICE_PARA_CONFIG_XML_PROCESS_H__

#include "hwsdk.h"
#include "ivs_xml.h"
#include "SDKDef.h"


class CDeviceParaConfigXMLProcess
{
public:
	// ��ѯ���豸��������XML
    static IVS_INT32 GetMainDevConfigParseXml(IVS_DEVICE_CFG* pMainDevCfg, CXml& xml);

	// �������豸����ƴ��XML
    static IVS_INT32 SetMainParaConfigGetXml( 
                                              const IVS_CHAR* pDomainCode, 
											  const IVS_CHAR* pDevCode, 
											  const IVS_DEVICE_CFG* pDevCfg,
											  const IVS_CHAR* pDevPWD,
											  const IVS_CHAR* pDevRegPWD,
                                              CXml& xmlTemp );
	
	// ��ѯOSD��������XML
	static IVS_INT32 GetOSDParaParseXML(IVS_CAMERA_OSD_CFG* pOSDConfig, CXml& xml);

	// ��������ѡ�����������������Ӧ
    static IVS_INT32 ParseCameraBriefInfoRsp(CXml& xml,
        IVS_CAMERA_CFG* pCameraCfg);

	// ��������ѡ�������̨��������Ӧ
	static IVS_INT32 ParseCameraPTZCfgRsp(CXml& xml,
		IVS_PTZ_CFG* pCameraPTZCfg, const IVS_CHAR* pDomainCode);

	// ��������ѡ�������豸��������Ӧ
	static IVS_INT32 ParseAudioCfgRsp(CXml& xml,
		IVS_AUDIO_CFG* pAudioCfg);

	// ���޸��������������������xml
	static IVS_INT32 SetCameraBriefCfgGetXml(
		const IVS_CHAR* pDomainCode, 
		const IVS_CHAR* pCameraCode, 
		const IVS_CAMERA_CFG* pCameraCfg,
		CXml& xmlTemp );

	// ��������ѡ���������ͼ���������Ӧ
    static IVS_INT32 ParseCameraDispalyCfgRsp(CXml& xml,
        IVS_CAMERA_DISPLAY_CFG* pCameraBriefInfo);

	// "�޸����������ͼ��"����xml
	static IVS_INT32 SetCameraBasicImgCfgGetXml(
		const IVS_CHAR* pDomainCode, 
		const IVS_CHAR* pCameraCode, 
		const IVS_CAMERA_DISPLAY_CFG* pCameraBasicImgCfg,
		CXml& xmlTemp );

	// "�޸���̨����"����xml
	static IVS_INT32 SetCameraPTZCfgGetXml(
		const IVS_CHAR* pDomainCode, 
		const IVS_CHAR* pCameraCode, 
		const IVS_PTZ_CFG* pCameraBasicImgCfg,
		CXml& reqXml );

	// "�޸��������豸����"����xml
	static IVS_INT32 SetAudioCfgGetXml( 
		const IVS_CHAR* pDomainCode, 
		const IVS_CHAR* pCameraCode, 
		const IVS_AUDIO_CFG* pAudioCfg,
		CXml& reqXml );

	// ��ѯ�����������������XML
	static IVS_INT32 CameraStreamCfgParseXML(IVS_CAMERA_STREAM_CFG* pCameraStrCfg, CXml &xml);
	
	// �����������������ƴ��XML
	static IVS_INT32 SetCameraStreamCfgGetXML(const IVS_CAMERA_STREAM_CFG* pCameraStrCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml);

	// �����������������ƴ��XML�������¼�����ͼ������������Ϣ��
	static IVS_INT32 SetCameraStreamCfgEventExGetXML(const IVS_CAMERA_STREAM_EVENTEX_CFG* pCameraStrCfgEx, 
		const IVS_CHAR* pDomainCode, 
		const IVS_CHAR* pDevCode, 
		CXml &xml);

	// ���ø澯�������豸ƴ��XML
	static IVS_INT32 SetAlarmInCfgGetXML(const IVS_ALARM_IN_CFG* pAlarmInCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml);

	// ��ѯ�澯�������豸����XML
	static IVS_INT32 GetAlarmInCfgParseXML(IVS_ALARM_IN_CFG* pAlarmInCfg, CXml &xml);

	// ���ø澯������豸ƴ��XML
	static IVS_INT32 SetAlarmOutCfgGetXML(const IVS_ALARM_OUT_CFG* pAlarmOutCfg, const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, CXml &xml);

	// ��ѯ�澯������豸����XML
	static IVS_INT32 GetAlarmOutCfgParseXML(IVS_ALARM_OUT_CFG* pAlarmOutCfg, CXml &xml);

	//������˽������xml
	static IVS_INT32 GetVideoMaskConfigParseXml(IVS_VIDEO_MASK* pVideoMask, CXml& xml);

	//ƴװ��˽����XML
	static IVS_INT32 SetVideoMaskCfgGetXML(const IVS_VIDEO_MASK* pVideoMask, const IVS_CHAR* pDomainCode, const IVS_CHAR* pCameraCode, CXml &xml);
	
	// ����OSD����ƴ��XML
	static IVS_INT32 SetCameraOsgCfgGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_CAMERA_OSD_CFG* pCameraOsgCfg, CXml &xml);

	// ����ͼ���ڵ�ƴ��XML
	static IVS_INT32 SetVideoHideAlarmGetXML(const IVS_CHAR* pDomainCode, const IVS_CHAR* pDevCode, const IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm, CXml &xml);

	// ��ѯͼ���ڵ�����XML
	static IVS_INT32 GetVideoHideAlarmParseXML(IVS_VIDEO_HIDE_ALARM* pVideoHideAlarm, CXml &xml);

	// ������ѯ�ƶ������Ӧxml
	static IVS_INT32 ParseMotionDetecXML(CXml& xml, IVS_MOTION_DETECTION* pMotionDetection);

	// �����˶�������Ӧ��Ϣ��AreaInfo������
	static IVS_INT32 GetMotionDetecAreaInfoNum(CXml& xml, IVS_INT32& iNum);

	// ƴװ�����ƶ��������xml
	static IVS_INT32 SetMotionDetecGetXML(const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode, const IVS_MOTION_DETECTION* pMotionDetection, CXml& xml);

	// �޸����豸�������ƴ��XML
	static IVS_INT32 SetDevNetCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_DEVICE_NET_CFG* pDevNetCfg, CXml &xml);

	// �޸����豸�����������XML
	static IVS_INT32 GetDevNetCfgParseXML(IVS_DEVICE_NET_CFG* pDevNetCfg, CXml &xml);

	// �޸����豸ʱ�����ƴ��XML
	static IVS_INT32 SetDevTimeCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_DEVICE_TIME_CFG* pDevTimeCfg, CXml &xml);

	// ��ѯ���豸ʱ���������XML
	static IVS_INT32 GetDevTimeCfgParseXML(IVS_DEVICE_TIME_CFG* pDevTimeCfg, CXml &xml);

	// ���ô������豸ƴ��XML
	static IVS_INT32 SetSerialChannelCfgGetXML(const IVS_CHAR* pSerialCode, const IVS_CHAR* pDomainCode, const IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg, CXml &xml);
    
	// ��ѯ�������豸����XML
	static IVS_INT32 GetSerialChannelCfgParseXML(IVS_SERIAL_CHANNEL_CFG* pSerialChannelCfg, CXml &xml);

	// �����������չͼ�����ƴ��XML
	static IVS_INT32 SetCameraImgCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_IMAGING_CFG* pCameraImgCfg, CXml &xml);

	// ��ѯ�������չͼ���������XML
	static IVS_INT32 GetCameraImgCfgParseXML(IVS_CAMERA_IMAGING_CFG* pCameraImgCfg, CXml &xml);

	// �����������չ����ƴ��XML
	static IVS_INT32 SetCameraExCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_EXTEND_CFG* pCameraExCfg, CXml &xml);

	// ��ѯ�������չ��������XML
	static IVS_INT32 GetCameraExCfgParseXML(IVS_CAMERA_EXTEND_CFG* pCameraExCfg, CXml &xml);


	// �����ץ�Ĳ�������ƴװXML
	static IVS_INT32 SetCameraSnapshotCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg, CXml &xml);

	// �����ץ�Ĳ������ý���XML
	static IVS_INT32 GetCameraSnapshotCfgParseXML(IVS_CAMERA_SNAPSHOT_CFG* pCameraSnapshotCfg, CXml &xml);

	// �����ǰ��¼����������ƴװXML
	static IVS_INT32 SetCameraRecordStreamCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, IVS_UINT32 uiStreamType, CXml &xml);

	// �����ǰ��¼�������������ý���XML
	static IVS_INT32 GetCameraRecordStreamCfgParseXML(IVS_UINT32* pStreamType, CXml &xml);

	// �����FEC����ƴװXML
	static IVS_INT32 SetCameraFECCfgGetXML(const IVS_CHAR* pDevCode, const IVS_CHAR* pDomainCode, const IVS_CAMERA_FEC_CFG* pCameraFEC, CXml &xml);

	// �����FEC�������ý���XML
	static IVS_INT32 GetCameraFECCfgParseXML(IVS_CAMERA_FEC_CFG* pCameraFEC, CXml &xml);

	
private: 
    CDeviceParaConfigXMLProcess();

};

#endif
