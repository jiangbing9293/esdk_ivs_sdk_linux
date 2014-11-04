/********************************************************************
	filename	: 	CRecordList.h
	author		:	z00201790
	created		:	2012/11/12	
	description	:	����¼���б��ѯ������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/11/12 ��ʼ�汾;
*********************************************************************/

#ifndef __RECORD_LIST_H__
#define __RECORD_LIST_H__

#include "hwsdk.h"
#include "ivs_error.h"
#include "XmlProcess.h"


typedef std::list<IVS_RECORD_DATA_PLACE*> RECORD_DATA_PLACE_List;
typedef RECORD_DATA_PLACE_List::iterator RECORD_DATA_PLACE_List_ITER;

//MBU�����¼��Ϣmap
typedef std::map<std::string, std::string> MBU_CHANGE_INFO_LIST_MAP;
typedef MBU_CHANGE_INFO_LIST_MAP::iterator MBU_CHANGE_INFO_LIST_MAP_ITER;

class CUserMgr;
class CSendNssMsgInfo;
class CRecordList
{
public:
	CRecordList(void);
	~CRecordList(void);

public:
	void SetUserMgr(CUserMgr *pUserMgr);

public:

	/***********************************************************************************
    * name       : QueryRecordList
    * description: ��ѯ¼���б�
	* input      : pUnifiedQuery����ѯ������pRecordInfoList����ѯ¼������uiBufferSize�������С
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 QueryRecordList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery, 
		IVS_RECORD_INFO_LIST* pDeviceList,IVS_UINT32 uiBufferSize);

	/***********************************************************************************
    * name       : QueryRecordListByBackupServer
    * description: ��ѯ¼���б�:�ӱ��ݷ�����
	* input      : pUnifiedQuery����ѯ������pRecordInfoList����ѯ¼������uiBufferSize�������С
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 QueryRecordListByBackupServer(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,const IVS_CHAR* pCameraCode,const IVS_CHAR* pDomainCode, 
		IVS_RECORD_INFO_LIST* pDeviceList,IVS_UINT32 uiBufferSize);

	/***********************************************************************************
    * name       : QueryMBUChangeHistoryList
    * description: ��ѯMBU�仯�б�
	* input      : pUnifiedQuery����ѯ������pMBUInfoList����ѯMBU�仯�б�
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 QueryMBUChangeHistoryList(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,const IVS_CHAR* pCameraCode,
		const IVS_CHAR* pDomainCode,IVS_MBU_CHANGE_INFO_LIST* pMBUInfoList,IVS_INT32 iToIndex,IVS_UINT32* uiDiffTotalNum);
	/***********************************************************************************
    * name       : ComputeIndexOfRecordList
    * description: ����ÿ��MBU�����Ľ�����ͷ�ҳ���������ż����Ŀ���ÿ��MBU�켣�Ľ������
	* input      : uiFromIndex����ʼ������uiToIndex,��������;pMBUInfoList����ѯMBU�仯�б�
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 ComputeIndexOfRecordList(const IVS_UINT32 uiFromIndex,const IVS_UINT32 uiToIndex,IVS_MBU_CHANGE_INFO_LIST* pMBUInfoList)const;

	/***********************************************************************************
    * name       : GetRecordList
    * description: ��ȡ¼���б�
	* input      : pCameraCode��������б�iRecordMethod��¼��ʽ��pTimeSpan����ѯʱ��Σ�pIndexRange����ҳ��Ϣ��pRecordList����ѯ¼������uiBufSize�������С
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetRecordList(const IVS_CHAR* pCameraCode,IVS_INT32 iRecordMethod,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,IVS_RECORD_INFO_LIST* pRecordList,IVS_UINT32 uiBufSize);

	/***********************************************************************************
    * name       : GetRecordStatus
    * description: ��ѯ¼��״̬
    * input      : pCameraCode:������б�; uiRecordMethod:¼��ʽ; pRecordStatus:¼��״̬
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetRecordStatus(const IVS_CHAR* pCameraCode,IVS_UINT32 uiRecordMethod,IVS_UINT32* pRecordStatus)const;

	/********************************************************************
	* name       : ParseGetMBUChangeHistoryList
	* description: MBU�����������¼����Ӧ����XML
	* input      : xml:��Ӧ���xml;pMBUChangeInfoList:MBU�����������¼����
	* output     : NA
	* return     : �ɹ�����0��ʧ�ܷ��ظ���������
	* remark     : NA
	*********************************************************************/
	IVS_INT32 ParseGetMBUChangeHistoryList(CXml& xml,IVS_MBU_CHANGE_INFO_LIST* pMBUChangeInfoList,IVS_UINT32* uiDiffTotalNum);

	/***********************************************************************************
    * name       : GetRecordNVRInfo
    * description: ��ȡ������ڸ�ʱ�����������NVR��Ϣ
    * input      : pCameraCode:�����ID; pStart:��ʼʱ��;pEnd������ʱ��
    * output     : pNVR��NVR��Ϣ
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetRecordNVRInfo(const IVS_CHAR* pCameraCode,const IVS_INT32 iStarTime,const IVS_INT32 iEndTime, IVS_CHAR* pNVR);


	/***********************************************************************************
    * name       : GetRecordTask
    * description: ��ѯ��ǰ�û����ֶ�¼������
    * input      : pRspXml:��ǰ�û����ֶ�¼��������Ϣ
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetRecordTask(IVS_CHAR** pRspXml);


	/***********************************************************************************
    * name       : LockRecord
    * description: ����¼��
    * input      : pCameraCode:���������;  iRecordMethod:¼��ʽ
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 LockRecord(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)const;

	/***********************************************************************************
    * name       : UnLockRecord
    * description: ����¼��
    * input      : pCameraCode:���������;  iRecordMethod:¼��ʽ
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 UnLockRecord(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)const;

	/***********************************************************************************
    * name       : ModLockRecord
    * description: �޸�¼��������Ϣ
    * input      : pCameraCode:���������;  iRecordMethod:¼��ʽ
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 ModLockRecord(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_RECORD_LOCK_INFO* pRecordLockInfo)const;

private:
	//����¼�����ݣ���໺��25�����������Ϣ�������ɾ�����������
	void InsertRecordInfoToList(IVS_RECORD_DATA_PLACE* pRecordData);
	//���¼������
	void ClearUpList();
	//��Ⱥ�µ�¼�����
	//int QueryInCluster(CSendNssMsgInfo& sendNssMsgInfo,const IVS_INDEX_RANGE* pIndex,const IVS_TIME_SPAN* pTimeSpan ,std::string& strpRsp);
	int QueryInCluster(CSendNssMsgInfo& sendNssMsgInfo,const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,std::string& strpRsp);

private:
	//��Ⱥģʽ�µĵ�¼���������ĺ�������


private:
	CUserMgr *m_pUserMgr;

	RECORD_DATA_PLACE_List m_RecordInfoList;		//����Session����¼����Ϣ����Ҫ���ڼ�Ⱥģʽ��

	MBU_CHANGE_INFO_LIST_MAP m_MBUChangeInfoMap;   //MBU�����¼��Ϣ 
};

#endif
