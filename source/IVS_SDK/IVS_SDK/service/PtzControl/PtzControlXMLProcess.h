/********************************************************************
	filename	: 	PtzControlXMLProcess.h
	author		:	z00201790
	created		:	2012/12/13
	description	:	������̨������;
	copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
	history		:	2012/12/13 ��ʼ�汾;
*********************************************************************/

#ifndef __PTZCONTROL_XML_PROCESS_H__
#define __PTZCONTROL_XML_PROCESS_H__

#include "XmlProcess.h"

class PtzControlProcess
{
public:
	/***********************************************************************************
    * function   : GetPtzPositionGetXML
	* description: ƴװ�ƾ�λ�õ�xml
	* input      : pCameraCode ���������
	* output     : xml  �ƾ����Ƶ�xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 GetPtzPositionGetXML(const IVS_CHAR* pCameraCode, CXml &xml);

	/**************************************************************************
    * name       : GetPtzPositionParseXML
    * description: ������̨λ�õ�xml
    * input      : xmlRsq  ��Ӧxml
	* output     : pPTZPosInfo ������Ӧ��Ϣ��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetPtzPositionParseXML(const std::string& strRsp, IVS_PTZ_ABSPOSITION* pPTZPosInfo);

	/***********************************************************************************
    * function       : PtzControlGetXML
	* description: ƴװ�ƾ����Ƶ�xml
	* input      : pLoginID ��¼ID
	               pCameraCode ���������
				   iControlCode	�ƾ�������
				   pControlPara1 �ƾ�����1: 1-�㶯ģʽ  2-����ģʽ
				   pControlPara2 �ƾ�����2: 1-����  2-�ٶ�
	* output     : xml  �ƾ����Ƶ�xml
	* return     : �ɹ�����0��ʧ�ܷ��ش�����;
    ***********************************************************************************/
	static IVS_INT32 PtzControlGetXML(const IVS_CHAR* pCameraCode,IVS_INT32 iControlCode,const IVS_CHAR* pControlPara1,const IVS_CHAR* pControlPara2,CXml &xml);

	/**************************************************************************
    * name       : PtzControlParseXML
    * description: �����ƾ����Ƶ�xml
    * input      : xmlRsq  ��Ӧxml
	* output     :   PtzControlInfo ������Ӧ��Ϣ��
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 PtzControlParseXML(const std::string& strRsp, IVS_PTZ_CONTROL_INFO* pPtzControlInfo);

	/**************************************************************************
    * name       : AddPTZPresetGetXML
    * description: ����Ԥ��λ��xml
    * input      : pLoginID ��¼ID
				   pCameraCode  ���������
				   pPresetName  Ԥ��λ����
	* output     : xml  ����Ԥ��λ��xml
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 AddPTZPresetGetXML(const IVS_CHAR* pCameraCode, const IVS_CHAR* pPresetName,CXml &xml);

	/**************************************************************************
    * name       : AddPTZPresetParseXML
    * description: ��������Ԥ��λ��xml
    * input      : strRsp  ��Ӧxml
				   uiPresetIndex  Ԥ��λ����
	* output     : 
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 AddPTZPresetParseXML(const std::string& strRsp, IVS_UINT32* uiPresetIndex);

	/**************************************************************************
    * name       : DelPTZPresetGetXML
    * description: ����Ԥ��λ��xml
    * input      : pCameraCode  ���������
				   pPresetName  Ԥ��λ����
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 DelPTZPresetGetXML(const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex, CXml &xml);

	/**************************************************************************
    * name       : ModPTZPresetGetXML
    * description: �޸�Ԥ��λ��xml
    * input      : pCameraCode  ���������
				   pPTZPreset  Ԥ��λ��Ϣ
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 ModPTZPresetGetXML(const IVS_CHAR* pCameraCode, const IVS_PTZ_PRESET* pPTZPreset, CXml &xml);

	/**************************************************************************
    * name       : GetPTZPresetListGetXML
    * description: ��ȡԤ��λ�б������xml
    * input      : pCameraCode  ���������
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetPTZPresetListGetXML(const IVS_CHAR* pCameraCode, CXml &xml);

	/**************************************************************************
    * name       : GetPTZPresetListParseXML
    * description: ��ȡԤ��λ�б����Ӧxml
    * input      : strRsp  ��ӦXML
				   pPTZPresetList  Ԥ��λ�б�
				   uiBufferSize  ��С
				   uiPTZPresetNum Ԥ��λ����
	* output     : 
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetPTZPresetListParseXML(const std::string& strRsp, IVS_PTZ_PRESET* pPTZPresetList, IVS_UINT32 /*uiBufferSize*/, IVS_UINT32* uiPTZPresetNum);

	/**************************************************************************
    * name       : SetGuardPosGetXML
    * description: ���ÿ���λ������xml
    * input      : pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 SetGuardPosGetXML(const IVS_CHAR* pCameraCode, const IVS_GUARD_POS_INFO* pGuardPosInfo, CXml &xml);


		/**************************************************************************
    * name       : DelGuardPosGetXML
    * description: ɾ������λ������xml
    * input      : pCameraCode  ���������
				   pGuardPosInfo  ����λ��Ϣ
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 DelGuardPosGetXML(const IVS_CHAR* pCameraCode, IVS_UINT32 uiPresetIndex, CXml &xml);


	/**************************************************************************
    * name       : GetGuardPosGetXML
    * description: ��ȡ����λ������xml
    * input      : pCameraCode  ���������
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetGuardPosGetXML(const IVS_CHAR* pCameraCode, CXml &xml);

	/**************************************************************************
    * name       : GetGuardPosParseXML
    * description: ��ȡ����λ����Ӧxml
    * input      : strRsp ��Ӧxml
				   pGuardPosInfo  ����λ��Ϣ
	* output     : 
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetGuardPosParseXML(const std::string& strRsp, IVS_GUARD_POS_INFO* pGuardPosInfo);

	/**************************************************************************
    * name       : AddCruiseTrackGetXML
    * description: ����Ѳ���켣������xml
    * input      : pCameraCode  ���������
				   pCuriseTrackInfo  �켣��Ϣ
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 AddCruiseTrackGetXML(const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, CXml &xml);

	/**************************************************************************
    * name       : AddCruiseTrackGetParseXML
    * description: ����Ѳ���켣����Ӧxml
    * input      : strRsp ��Ӧ����
				   uiTrackIndex  �켣����
	* output     : xml
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 AddCruiseTrackGetParseXML(const std::string& strRsp, IVS_UINT32* uiTrackIndex);

	/**************************************************************************
    * name       : DelCruiseTrackGetXML
    * description: ɾ���켣xml
    * input      : pCameraCode  ���������
				   uiTrackIndex  �켣����
				   uiCruiseType  �켣����
	* output     : xml  ����Ԥ��λ��xml
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 DelCruiseTrackGetXML(const IVS_CHAR* pCameraCode, IVS_UINT32 uiTrackIndex, IVS_UINT32 uiCruiseType, CXml &xml);

	/**************************************************************************
    * name       : ModCruiseTrackGetXML
    * description: �޸Ĺ켣��xml
    * input      : pCameraCode  ���������
				   pCuriseTrackInfo  �켣��Ϣ
	* output     : xml  ����Ԥ��λ��xml
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 ModCruiseTrackGetXML(const IVS_CHAR* pCameraCode, const IVS_CRUISE_TRACK* pCuriseTrackInfo, CXml &xml);

	/**************************************************************************
    * name       : GetCruiseTrackListGetXML
    * description: ��ȡ�켣�б�����xml
    * input      : pCameraCode  ���������
	* output     : xml  ����Ԥ��λ��xml
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruiseTrackListGetXML(const IVS_CHAR* pCameraCode, CXml &xml);

	/**************************************************************************
    * name       : GetCruiseTrackListParseXML
    * description: ��ȡ�켣�б���Ӧxml
    * input      : strRsp ��Ӧ����
				   pCruiseTrackList  �ڴ��׵�ַ
				   uiBufferSize  �ڴ��С
				   uiCruiseTrackNum �켣����
	* output     : 
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruiseTrackListParseXML(const std::string& strRsp, IVS_CRUISE_TRACK* pCruiseTrackList, IVS_UINT32 /*uiBufferSize*/, IVS_UINT32* uiCruiseTrackNum);

	/**************************************************************************
    * name       : GetCruiseTrackGetXML
    * description: ��ȡѲ���켣������xml
    * input      : pCameraCode  ���������
				   uiTrackIndex  �켣����
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruiseTrackGetXML(const IVS_CHAR* pCameraCode, IVS_INT32 uiTrackIndex, CXml &xml);

	/**************************************************************************
    * name       : GetCruiseTrackParseXML
    * description: ��ȡѲ���켣����Ӧxml
    * input      : strRsp  ��ӦXML
				   pCruiseTrackInfo �켣��Ϣ
	* output     :   
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruiseTrackParseXML(const std::string& strRsp, IVS_CRUISE_TRACK* pCruiseTrackInfo);

	/**************************************************************************
    * name       : SetCruisePlanGetXML
    * description: ����Ѳ���ƻ�������xml
    * input      : pCameraCode  ���������
				   pCruisePlan  Ѳ���ƻ�
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 SetCruisePlanGetXML(const IVS_CHAR* pCameraCode, const IVS_CRUISE_PLAN* pCruisePlan, CXml &xml);

	/**************************************************************************
    * name       : GetCruisePlanGetXML
    * description: ��ȡѲ���ƻ�������xml
    * input      : pCameraCode  ���������
	* output     : xml  
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruisePlanGetXML(const IVS_CHAR* pCameraCode, CXml &xml);

	/**************************************************************************
    * name       : GetCruisePlanParseXML
    * description: ��ȡѲ���ƻ�����Ӧxml
    * input      : strRsp  ��Ӧxml
				   pCruisePlan Ѳ���ƻ�
	* output     :   
    * return     : �ɹ�����IVS_SUCCESSED��ʧ�ܷ��ش�����
    **************************************************************************/
	static IVS_INT32 GetCruisePlanParseXML(const std::string& strRsp, IVS_CRUISE_PLAN* pCruisePlan);
};
#endif //__PTZCONTROL_XML_PROCESS_H__
