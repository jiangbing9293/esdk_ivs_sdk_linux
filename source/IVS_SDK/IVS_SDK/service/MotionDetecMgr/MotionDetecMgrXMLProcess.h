/********************************************************************
filename	: 	MotionDetecMgrXMLProcess.h
author		:	z00201790
created		:	2012/12/06	
description	:	������ƴװ�ƶ�������xml����;
copyright	:	Copyright (C) 2011-2015
history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/
#ifndef __MOTIONDETECTION_MGR_XML_PROCESS_H__
#define __MOTIONDETECTION_MGR_XML_PROCESS_H__

#include "XmlProcess.h"
#include "SDKDef.h"

class CMotionDetecMgrXMLProcess
{
private:
	CMotionDetecMgrXMLProcess();
public:
	~CMotionDetecMgrXMLProcess();

public:
	/***********************************************************************************
    * function       : GetMotionDetecXML
	* description: ƴװ��ѯ�˶���������xml
	* input      : pLoginId,��¼Id��pCameraCode����������룻pDomainCode�������
	* output     : xml�������Ŀ��xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetMotionDetecXML(const IVS_CHAR* pLoginId, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode, CXml& xml);

	/***********************************************************************************
    * function       : ParseMotionDetecXML
	* description: �����˶�������Ӧ��Ϣxml
	* input      : xml,��Ӧ��Ϣxml
	* output     : pMotionDetection���˶����ṹ��
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 ParseMotionDetecXML(CXml& xml, IVS_MOTION_DETECTION* pMotionDetection);

	/***********************************************************************************
    * function       : GetMotionDetecAreaInfoNum
	* description: �����˶�������Ӧ��Ϣ��AreaInfo������
	* input      : xml,��Ӧ��Ϣxml
	* output     : iNum��AreaInfo������
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetMotionDetecAreaInfoNum(CXml& xml, IVS_INT32& iNum);

	/***********************************************************************************
    * function       : SetMotionDetecGetXML
	* description: ƴװ�޸��˶���������xml
	* input      : pLoginId,��¼Id��pCameraCode����������룻pDomainCode�������
	               pMotionDetection, �˶����ṹ��
	* output     : xml�������Ŀ��xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 SetMotionDetecGetXML(const IVS_CHAR* pLoginId, const IVS_CHAR* pCameraCode, const IVS_CHAR* pDomainCode, const IVS_MOTION_DETECTION* pMotionDetection, CXml& xml);
	
	/***********************************************************************************
    * function   : GetMotionRangeDataPlatformReqXML
	* description: ƴװ��ѯ�ƶ����������ݵ�����xml
	* input      : pCameraCode����������룻pDomainCode�������
	               pTimeSpan, ʱ�����ṹ�壻pIndexRange����ҳ��Ϣ�ṹ��
	* output     : xml�������Ŀ��xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetMotionRangeDataPlatformReqXML(const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,CXml& xml);

	/***********************************************************************************
    * function   : ParseGetMotionRangeDataPlatformRspXML
	* description: ������ѯ�ƶ����������ݵ���Ӧxml
	* input      : uiBufferSize:��Ӧ�������ռ��С��
				   RspXml�������Ŀ��xml
	* output     : pMotionRangeData���ƶ����������ݵ���Ӧ��Ϣ�ṹ��
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 ParseGetMotionRangeDataPlatformRspXML(CXml &RspXml, IVS_MOTION_RANGE_DATA_LIST* pMotionRangeData,IVS_UINT32 uiBufferSize);
	
};

#endif //__MOTIONDETECTION_MGR_XML_PROCESS_H__
