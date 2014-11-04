/********************************************************************
	filename	: 	RecordBackup.h
	author		:	tkf71032
	created		:	2013/1/23	
	description	:	����¼����ǩ������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2013/1/23 ��ʼ�汾;
*********************************************************************/

#ifndef __RECORD_BACKUP_H__
#define __RECORD_BACKUP_H__

#include "hwsdk.h"
#include "ivs_error.h"
#include "IVSCommon.h"
#include "ToolsHelp.h"
#include "SDKConfig.h"
//#include "ivs_xml.h"

class CUserMgr;
 class CRecordBackupMgr
 {
 public:
	 CRecordBackupMgr(void);
	 ~CRecordBackupMgr(void);

 public:
	 // �����û�������
	 void SetUserMgr(void* pUserMgr);

	 // ������������ݲ���
	 IVS_INT32 SetCameraBackupCfg(const IVS_CHAR* pBackupXml);

	 // ��ȡ��������ݲ���
	 IVS_INT32 GetCameraBackupCfg(const IVS_CHAR* pCameraCode, IVS_CHAR** pBackConfigXml);

	 // �������������
	 IVS_INT32 SetCameraCodeValue(CXml& xml, IVS_CHAR *pDevCode, IVS_CHAR* pDomainCode) const;

	 // ƴװ��ȡ��������ݲ�����xml
	 IVS_INT32 GetCameraBackupCfgGetXml(CXml& xml, const IVS_CHAR *pDevCode, const IVS_CHAR* pDomainCode)const;

	 // ���¼�񱸷ݼƻ�
	 IVS_INT32 AddRecordBackupPlan(const IVS_CHAR* pBackupPlan);

	 // �޸�¼�񱸷ݼƻ�
	 IVS_INT32 ModifyRecordBackupPlan(const IVS_CHAR* pBackupPlan);

	 // �޸�¼�񱸷ݼƻ�
	 IVS_INT32 DeleteRecordBackupPlan(const IVS_CHAR* pBackupPlan);

	 // ��ѯ¼�񱸷ݼƻ�
	 IVS_INT32 GetRecordBackupPlan(const IVS_CHAR* pQueryInfo, IVS_CHAR** pBackupPlan);

	 // ����¼�񱸷�����
	 IVS_INT32 AddRecordBackupTask(const IVS_CHAR* pBackupTask);

	 /*****************************************************************
	      function   : RecordBackupPlan
	      description: ¼�񱸷ݼƻ��������ͽӿ�
	      input      : NSS_MSG_TYPE_E msgType  ���͵���Ϣ����
					   const IVS_CHAR * pBackupPlan   ¼�񱸷ݼƻ�XML
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 RecordBackupPlan(NSS_MSG_TYPE_E msgType, const IVS_CHAR* pBackupPlan);

	 // �������¼�񱸷ݼƻ�����������벢���domianCode
	 /*****************************************************************
	      function   : SetNodeCameraCodeValue
	      description: ����cameraCode����ƴװdomianCode�ֶν�xml
	      output     : CXml & xml  ������xml 
	                   IVS_CHAR * pDevCode  ��������豸����
			           IVS_CHAR * pDomainCode   �������������
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 SetNodeCameraCodeValue(CXml& xml, IVS_CHAR *pDevCode, IVS_CHAR* pDomainCode) const;

	 /*****************************************************************
	      function   : SetNodeFileNameValue
	      description: ����cameraCode����ƴװdomianCode�ֶν�xml
	      output     : CXml & xml  ������xml 
	                   IVS_CHAR * pDevCode  ��������豸����
			           IVS_CHAR * pDomainCode   �������������
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 SetNodeFileNameValue(CXml& xml) const;

	 /*****************************************************************
	      function   : StartRecordBackupTask
	      description: ����/����¼�񱸷�����
	      input     : pQueryInfo  �������������Ϣ
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 StartRecordBackupTask(const IVS_CHAR* pQueryInfo);

	 /*****************************************************************
	      function   : StopRecordBackupTask
	      description: ��ͣ/ֹͣ¼�񱸷�����
	      input     : pQueryInfo  �������������Ϣ
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 StopRecordBackupTask(const IVS_CHAR* pQueryInfo);

	 /*****************************************************************
	      function   : GetRecordBackupTaskList
	      description: ��ѯ¼�񱸷������б�
	      input     : pQueryInfo  �������������Ϣ
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 GetRecordBackupTaskList(const IVS_CHAR* pQueryInfo, IVS_CHAR** pRspXml);

	 /*****************************************************************
	      function   : DeleteRecordBackupTask
	      description: ɾ��¼�񱸷�����
	      input     : pQueryInfo  �������������Ϣ
	      return     : �ɹ�����0��ʧ�ܷ��ش�����
	 *****************************************************************/
	 IVS_INT32 DeleteRecordBackupTask(const IVS_CHAR* pQueryInfo);

 private:
	 CUserMgr *m_pUserMgr;   
 };

#endif
