#include "SDKMainService.h"
#include "bp_environments.h"
#include "ivs_xml.h"
#include "ivs_error.h"
#include "IVSCommon.h"
#include "ToolsHelp.h"
#ifdef WIN32
#include "RecordXMLProcess.h"
#endif

// ����澯��Ϣ֪ͨCmd
IVS_ALARM_NOTIFY* CSDKMainService::ProcessAlarmAlarmNotifyCmd(CCmd* pCmd)
{
	BP_RUN_LOG_INF("Process AlarmAlarmNotify", "start");
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "cmd is null", "NA");
		return NULL;
	}

	int len = 0;
	IVS_CHAR* pData = CNSSOperator::instance().ParseCmd2Data(pCmd, len);
	CHECK_POINTER(pData, NULL);
	if (len < (int)((((sizeof(IVS_ALARM_NOTIFY) - IVS_ALARM_DESCRIPTION_LEN) - 32) - IVS_ALARM_EX_PARAM_LEN) + 6))  // 32Ϊ�����ֶ�cReserver,6ΪTLVͷ���� 
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "data from server error", "data error");
        IVS_DELETE(pData, MUILI);
        return NULL;
    }
	IVS_ALARM_NOTIFY* pAlarmNotifyTmp = reinterpret_cast<IVS_ALARM_NOTIFY*>(pData); //lint !e826
//     if (NULL == pAlarmNotifyTmp)
//     {
//         BP_RUN_LOG_ERR(IVS_SDK_RET_INTRINSIC_PTR_ERROR, "pAlarmNotifyTmp is null", "NA");
//         IVS_DELETE(pData, MUILI);
//         return NULL;
//     }
	// �������ֽ���ת�������ֽ��� ntohl 32 ntohs16
	BP_ntohll(pAlarmNotifyTmp->ullAlarmEventID, pAlarmNotifyTmp->ullAlarmEventID);
	pAlarmNotifyTmp->uiAlarmInType = ntohl(pAlarmNotifyTmp->uiAlarmInType);
	pAlarmNotifyTmp->uiAlarmLevelValue = ntohl(pAlarmNotifyTmp->uiAlarmLevelValue);
	pAlarmNotifyTmp->uiAlarmStatus = ntohl(pAlarmNotifyTmp->uiAlarmStatus);
	pAlarmNotifyTmp->uiOccurNumber = ntohl(pAlarmNotifyTmp->uiOccurNumber);

	pAlarmNotifyTmp->bExistsRecord = static_cast<IVS_INT32>(ntohl(static_cast<IVS_UINT32>(pAlarmNotifyTmp->bExistsRecord)));
	pAlarmNotifyTmp->bIsCommission = static_cast<IVS_INT32>(ntohl(static_cast<IVS_UINT32>(pAlarmNotifyTmp->bIsCommission)));

	// ����ǰ�����ֽ�
	IVS_UINT32 iSize = ((sizeof(IVS_ALARM_NOTIFY) - IVS_ALARM_DESCRIPTION_LEN) - IVS_ALARM_EX_PARAM_LEN) - 32;
	IVS_ALARM_NOTIFY* pAlarmNotify = IVS_NEW(pAlarmNotify);
    if (NULL == pAlarmNotify)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "pAlarmNotify is null", "NA");
        IVS_DELETE(pData, MUILI);
        return NULL;
    }
	memset(pAlarmNotify, 0x0, sizeof(IVS_ALARM_NOTIFY));
	if (!CToolsHelp::Memcpy(pAlarmNotify, sizeof(IVS_ALARM_NOTIFY), pAlarmNotifyTmp, iSize))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	// ������TLV�ֶ�(�澯��������չ����)�Ĵ���
	TNssTLVMsg tlvMsg;
	CNSSOperator::instance().ParseData2TLV((void*)(pData + iSize), tlvMsg);//����Խ��
	
	// �ж�tag
	if (tlvMsg.usiTag != 0x0004) //lint !e1013
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "tag error", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	//  IVS_ALARM_DESCRIPTION_LEN
	if (tlvMsg.uiLength > IVS_ALARM_DESCRIPTION_LEN)  //lint !e1013
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "length error", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	if (!CToolsHelp::Memcpy((IVS_CHAR*)pAlarmNotify + iSize + 32, IVS_ALARM_DESCRIPTION_LEN, tlvMsg.pszValue, tlvMsg.uiLength))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	// ��ʼ����ڶ���TLV
	iSize += sizeof(IVS_USHORT) + sizeof(IVS_INT32) + tlvMsg.uiLength;
	memset(&tlvMsg, 0x0, sizeof(TNssTLVMsg));
	CNSSOperator::instance().ParseData2TLV((void*)(pData + iSize), tlvMsg);
	if (0x0005 != tlvMsg.usiTag)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "0x0002 != tlvMsg.usiTag(Second)", "NA");
		IVS_DELETE(pData, MUILI);
		IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	if (IVS_ALARM_EX_PARAM_LEN < tlvMsg.uiLength)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "0x0004 != tlvMsg.uiLength(second) fail", "NA");
		IVS_DELETE(pData, MUILI);
		IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	iSize = sizeof(IVS_ALARM_NOTIFY) - IVS_ALARM_EX_PARAM_LEN;
	if (!CToolsHelp::Memcpy((IVS_CHAR*)pAlarmNotify + iSize, IVS_ALARM_EX_PARAM_LEN, tlvMsg.pszValue, tlvMsg.uiLength))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy ExParam to pAlarmNotify fail", "NA");
		IVS_DELETE(pData, MUILI);
		IVS_DELETE(pAlarmNotify);
		return NULL;
	}
	IVS_DELETE(pData, MUILI);
	BP_RUN_LOG_INF("Process AlarmAlarmNotify", "end");
	return pAlarmNotify;
}

//������������ִ��֪ͨ
IVS_CHAR* CSDKMainService::ProcessMAUCmd(CCmd* pCmd)
{
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "cmd is null", "NA");
		return NULL;
	}
	IVS_INT32 iRet = IVS_SUCCEED;
	IVS_CHAR* pDataXML = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);
	BP_RUN_LOG_INF("CNSSOperator::instance().ParseCmd2XML(pCmd, iRet)", "iRet = %d", iRet);
	if (NULL == pDataXML)
	{
		BP_RUN_LOG_INF("NULL == pDataXML", "NA");
		return NULL;
	}

	return pDataXML;
}

IVS_ALARM_STATUS_NOTIFY* CSDKMainService::ProcessAlarmStatusNotify(CCmd* pCmd)
{
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "cmd is null", "NA");
		return NULL;
	}

	int len = 0;
	IVS_CHAR* pData = CNSSOperator::instance().ParseCmd2Data(pCmd, len);
    CHECK_POINTER(pData, NULL);
    if (len < (int)(((sizeof(IVS_ALARM_STATUS_NOTIFY) - sizeof(IVS_ALARM_OPERATE_INFO)) - 32) + 6)) // 32Ϊ�����ֶ�cReserver,6ΪTLVͷ����
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "data from server error", "data error");
        IVS_DELETE(pData, MUILI);
        return NULL;
    }
	// ��ø澯�ϱ���Ϣ
	IVS_ALARM_STATUS_NOTIFY* pAlarmStatusNotifyTmp = reinterpret_cast<IVS_ALARM_STATUS_NOTIFY*>(pData); //lint !e826
//     if (NULL == pAlarmStatusNotifyTmp)
//     {
//         BP_RUN_LOG_ERR(IVS_SDK_RET_INTRINSIC_PTR_ERROR, "pAlarmStatusNotifyTmp is null", "NA");
//         IVS_DELETE(pData, MUILI);
// 		return NULL;
//     }
	IVS_ALARM_STATUS_NOTIFY* pAlarmStatusNotify = IVS_NEW(pAlarmStatusNotify);
    if (NULL == pAlarmStatusNotify)
    {
        BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "pAlarmStatusNotifyTmp is null", "NA");
        IVS_DELETE(pData, MUILI);
		return NULL;
    }
	memset(pAlarmStatusNotify, 0x0, sizeof(IVS_ALARM_STATUS_NOTIFY));
	/*=========================== Notify ======================================*/
	// �������ֽ���ת�������ֽ��� ntohl 32 ntohs16
	BP_ntohll(pAlarmStatusNotifyTmp->ullAlarmEventID, pAlarmStatusNotifyTmp->ullAlarmEventID);
	pAlarmStatusNotifyTmp->uiAlarmStatus = ntohl(pAlarmStatusNotifyTmp->uiAlarmStatus);
	pAlarmStatusNotifyTmp->uiOccurNumber = ntohl(pAlarmStatusNotifyTmp->uiOccurNumber);

	// ����AlarmNotify
	IVS_UINT32 iSize = (sizeof(IVS_ALARM_STATUS_NOTIFY) - 32) - sizeof(IVS_ALARM_OPERATE_INFO);
	if (!CToolsHelp::Memcpy(pAlarmStatusNotify, sizeof(IVS_ALARM_STATUS_NOTIFY), pAlarmStatusNotifyTmp, iSize))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmStatusNotify);
		return NULL;
	}

	/*=========================== Operator =====================================*/
	IVS_ALARM_OPERATE_INFO* pAlarmOperateInfo = reinterpret_cast<IVS_ALARM_OPERATE_INFO*>(pData + iSize);  //lint !e826
//     if (NULL == pAlarmOperateInfo)
//     {
//         BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "pAlarmOperateInfo is null", "NA");
//         IVS_DELETE(pData, MUILI);
//         IVS_DELETE(pAlarmStatusNotify);
// 		return NULL;
//     }
	// �������ֽ���ת�������ֽ��� ntohl 32 ntohs16
	pAlarmOperateInfo->uiOperatorID = ntohl(pAlarmOperateInfo->uiOperatorID);

	// ����Operator
	IVS_UINT32 uiSize = sizeof(IVS_ALARM_OPERATE_INFO) - IVS_ALARM_DESCRIPTION_LEN;
	if (!CToolsHelp::Memcpy((IVS_CHAR*)pAlarmStatusNotify + iSize + 32, sizeof(IVS_ALARM_OPERATE_INFO), pAlarmOperateInfo, uiSize))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmStatusNotify);
		return NULL;
	}

	/*========================= ��TLV�ֶεĴ��� ================================*/
	TNssTLVMsg tlvMsg;
	CNSSOperator::instance().ParseData2TLV((void*)(pData + iSize + uiSize), tlvMsg);

	// �ж�tag
	if (tlvMsg.usiTag != 0x0006) //lint !e1013
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "tag error", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmStatusNotify);
		return NULL;
	}
	if (tlvMsg.uiLength > IVS_ALARM_DESCRIPTION_LEN)  //lint !e1013
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "length error", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmStatusNotify);
		return NULL;
	}
	if (!CToolsHelp::Memcpy((IVS_CHAR*)pAlarmStatusNotify + iSize + 32 + uiSize, IVS_ALARM_DESCRIPTION_LEN,tlvMsg.pszValue, tlvMsg.uiLength))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy fail", "NA");
        IVS_DELETE(pData, MUILI);
        IVS_DELETE(pAlarmStatusNotify);
		return NULL;
	}
	// ɾ���ڴ�
	IVS_DELETE(pData, MUILI);

	return pAlarmStatusNotify;
}

// //�����û�����
//IVS_ALARM_NOTIFY* CSDKMainService::ProcessUserOffLineCmd(CCmd* pCmd)
//{
//
//}

///**************************************************************************
//* name       : ParseUserOffLineXML
//* description: ������¼��ӦXML
//* input      : xmlRsq           ��ӦXML
//* output     : ST_SRU_LOGIN_RSP ��¼�ɹ����ص���Ϣ�������¼�ɹ���Ӧ��Ϣ�ṹ��
//* return     : �ɹ�-IVS_SUCCEED,ʧ��-IVS_FAIL
//* remark     : NA
//**************************************************************************/
//IVS_INT32 CSDKMainService::ParseUserOffLineXML(CXml& xmlRsq, IVS_USER_OFFLINE_NOTIFY &stOffLineInfo)
//{
//    BP_RUN_LOG_INF("Enter CSDKMainService::ParseUserOffLineXML", "");
//    memset(&stOffLineInfo, 0, sizeof(stOffLineInfo));
//
//    const char*szXMLValue = NULL;   //��ʱ�洢�����ڵ�ֵ
//
//    if (!xmlRsq.FindElemEx("/"))
//    {
//        BP_RUN_LOG_INF("Leave CSDKMainService::ParseUserOffLineXML", "Content is null");
//        return IVS_FAIL;
//    }
//    
//    //GET_ELEM_VALUE_CHAR
//    if (xmlRsq.FindElem("LoginId"))
//    {								               
//        szXMLValue = xmlRsq.GetElemValue();
//        if (NULL != szXMLValue)
//        {
//            //(void)CToolsHelp::Strncpy(stOffLineInfo.cLoginId, LOGIN_ID_LEN + 1, szXMLValue, strlen(szXMLValue));
//            int iCopyLen = strlen(szXMLValue);
//            if (iCopyLen >= LOGIN_ID_LEN + 1 )
//            {
//                
//            }
//            else
//            {
//                strncpy( stOffLineInfo.cLoginId, szXMLValue, iCopyLen );
//                stOffLineInfo.cLoginId[iCopyLen] = '\0';
//            }
//            
//        }
//    }
//    //GET_ELEM_VALUE_NUM
//    stOffLineInfo.iOfflineType = 0;
//    if (xmlRsq.FindElem("OfflineType"))
//    {								               
//        szXMLValue = xmlRsq.GetElemValue();
//        if (NULL != szXMLValue)
//        {
//            stOffLineInfo.iOfflineType = atoi(szXMLValue);
//        }
//    }
//    
//    BP_RUN_LOG_INF("Leave CSDKMainService::ParseUserOffLineXML", "");
//    return IVS_SUCCEED;
//}

// ���ͷ��ֵ���ǰ���豸;ģʽ�켣¼�ƽ���֪ͨ
IVS_CHAR* CSDKMainService::ProcessDevNotify(CCmd* pCmd)
{
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "cmd is null", "NA");
		return NULL;
	}
	IVS_INT32 iRet = IVS_SUCCEED;
	IVS_CHAR* pDataXML = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);
	BP_RUN_LOG_INF("Parse Cmd2XML(pCmd) ReturnCode", "iRet = %d", iRet);
	if (NULL == pDataXML)
	{
		BP_RUN_LOG_INF("NULL == pDataXML", "NA");
		return NULL;
	}

	return pDataXML;
}
// �豸�澯�ϱ�
IVS_INT32 CSDKMainService::ProcessDevAlarmNotify(CCmd* pCmd, IVS_DEVICE_ALARM_NOTIFY* pDevAlarmNotify)
{
	if (NULL == pCmd)
	{
		BP_RUN_LOG_INF("NULL == pCmd", "NA");
		return IVS_FAIL;
	}
	if (NULL == pDevAlarmNotify)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "NULL == pDevAlarmNotify", "NA");
		return IVS_ALLOC_MEMORY_ERROR;
	}
	IVS_INT32 xmlLen = 0;
	IVS_CHAR* pData = CNSSOperator::instance().ParseCmd2Data(pCmd, xmlLen);
	CHECK_POINTER(pData, IVS_FAIL);
    int iNeedLen = (int)((sizeof(IVS_DEVICE_ALARM_NOTIFY) - IVS_ALARM_DESCRIPTION_LEN) - IVS_ALARM_EX_PARAM_LEN) + 12;
    if (xmlLen < iNeedLen)  // 12Ϊ����TLV��ͷ����
    {
        BP_RUN_LOG_ERR(IVS_FAIL, "data from server error", "data error");
        IVS_DELETE(pData, MUILI);
        return IVS_FAIL;
    }
	IVS_DEVICE_ALARM_NOTIFY* pDevAlarmNotifyTep = reinterpret_cast<IVS_DEVICE_ALARM_NOTIFY*>(pData);//lint !e826
//     if (NULL == pDevAlarmNotifyTep)
//     {
//         BP_RUN_LOG_ERR(IVS_SDK_RET_INTRINSIC_PTR_ERROR, "pDevAlarmNotifyTep is null", "NA");
//         IVS_DELETE(pData, MUILI);
//         return IVS_FAIL;
//     }
	// �����ֽ��򵽱����ֽڵ�ת��
	BP_ntohll(pDevAlarmNotifyTep->ullAlarmEventID, pDevAlarmNotifyTep->ullAlarmEventID);
	pDevAlarmNotifyTep->uiAlarmInType = ntohl(pDevAlarmNotifyTep->uiAlarmInType);
	pDevAlarmNotifyTep->uiAlarmLevelValue = ntohl(pDevAlarmNotifyTep->uiAlarmLevelValue);
	pDevAlarmNotifyTep->uiOccurNumber = ntohl(pDevAlarmNotifyTep->uiOccurNumber);
	
	// �����̶�������
	IVS_UINT32 uiSTSize = (sizeof(IVS_DEVICE_ALARM_NOTIFY) - IVS_ALARM_DESCRIPTION_LEN) - IVS_ALARM_EX_PARAM_LEN;
	if (!CToolsHelp::Memcpy((IVS_CHAR*)pDevAlarmNotify, sizeof(IVS_DEVICE_ALARM_NOTIFY), pDevAlarmNotifyTep, uiSTSize))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy pDevAlarmNotifyTep to pDevAlarmNotify fail", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}

	/************************************************************************/
	/*   ��������TLV�ṹ                                                                                                             */
	/************************************************************************/                                                  
	// ��һ��TLV
	TNssTLVMsg tlvMsg;
	CNSSOperator::instance().ParseData2TLV((void*)(pData + uiSTSize), tlvMsg);

	if (0x0004 != tlvMsg.usiTag)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "TLV tag error", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (IVS_ALARM_DESCRIPTION_LEN < tlvMsg.uiLength)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "TLV Length error", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (!CToolsHelp::Memcpy((IVS_CHAR*)(pDevAlarmNotify) + uiSTSize, IVS_ALARM_DESCRIPTION_LEN, tlvMsg.pszValue, tlvMsg.uiLength))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy tlvMsg.pszValue to pDevAlarmNotify fail", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}
	uiSTSize += (sizeof(IVS_SHORT) + sizeof(IVS_INT32)) + tlvMsg.uiLength;
	
	// �ڶ���TLV
	memset(&tlvMsg, 0x0, sizeof(TNssTLVMsg));
	CNSSOperator::instance().ParseData2TLV((void*)(pData + uiSTSize), tlvMsg);

	if (0x0005 != tlvMsg.usiTag)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "TLV tag error", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	if (IVS_ALARM_EX_PARAM_LEN < tlvMsg.uiLength)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "TLV Length error", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_FAIL;
	}
	uiSTSize = sizeof(IVS_DEVICE_ALARM_NOTIFY) - IVS_ALARM_EX_PARAM_LEN;
	if (!CToolsHelp::Memcpy((IVS_CHAR*)(pDevAlarmNotify) + uiSTSize, IVS_ALARM_EX_PARAM_LEN, tlvMsg.pszValue, tlvMsg.uiLength))
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "CToolsHelp::Memcpy tlvMsg.pszValue to pDevAlarmNotify fail", "NA");
		IVS_DELETE(pData, MUILI);
		return IVS_ALLOC_MEMORY_ERROR;
	}

	IVS_DELETE(pData, MUILI);
	return IVS_SUCCEED;
}
#ifdef WIN32
IVS_INT32 CSDKMainService::ProcessManualRecordStateNotify(CCmd* pCmd, IVS_MANUAL_RECORD_STATUS_NOTIFY& stManualRecordStatusNotify)
{
	if (NULL == pCmd)
	{
		BP_RUN_LOG_ERR(IVS_OPERATE_MEMORY_ERROR, "cmd is null", "NA");
		return IVS_PARA_INVALID;
	}

	IVS_INT32 iRet = IVS_SUCCEED;
	//IVS_MANUAL_RECORD_STATUS_NOTIFY pManualRecordStatusNotify; 
	//memset(&pManualRecordStatusNotify, 0, sizeof(IVS_MANUAL_RECORD_STATUS_NOTIFY));

	IVS_CHAR* pDataXML = CNSSOperator::instance().ParseCmd2XML(pCmd, iRet);
	BP_RUN_LOG_INF("Parse Cmd2XML(pCmd) ReturnCode", "iRet = %d", iRet);
	if (NULL == pDataXML)
	{
		BP_RUN_LOG_INF("NULL == pDataXML", "NA");
		return IVS_PARA_INVALID;
	}
	CXml xmlData;
	(void)xmlData.Parse(pDataXML);

	//����XML
	if (IVS_SUCCEED != CRecordXMLProcess::ParseManualRecordStateXML(xmlData,&stManualRecordStatusNotify))
	{
		BP_RUN_LOG_ERR(IVS_XML_INVALID,"Parse Manual Record State failed","failed");
	    IVS_DELETE(pDataXML, MUILI);
		return IVS_FAIL;
	}	

	IVS_DELETE(pDataXML, MUILI);
	return IVS_SUCCEED;
}
#endif 
