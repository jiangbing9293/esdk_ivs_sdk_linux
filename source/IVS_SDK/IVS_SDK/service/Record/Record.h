/********************************************************************
	filename	: 	CRecord.h
	author		:	z00201790
	created		:	2012/11/12	
	description	:	����¼��ҵ����;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/11/12 ��ʼ�汾;
*********************************************************************/
#ifndef __RECORD_H__
#define __RECORD_H__

#include "RecordXMLProcess.h"
#include "SDKDef.h"

typedef enum
{
	RECORD_IDLE    = 0,
	RECORDING
}RECORD_STATUS;

class CRecord;
typedef struct ST_STOP_RECORD_INFO
{
	unsigned long ulPort;
	CRecord* pRecord;
}STOP_RECORD_INFO;

class CUserMgr;
class CRecord
{
public:
	CRecord(void);
	~CRecord(void);

public:

	int GetPlayHandle()const;     
	void SetPlayHandle(int iPlayHandle);

	//�����û�������
	void SetUserMgr(void *pUserMgr);

	RECORD_STATUS GetState()const;                 
	void SetState(RECORD_STATUS RecordStatus);   

	unsigned long GetPort()const;  

	/***********************************************************************************
    * name       : DealException
    * description: ����MP�����쳣�ص�
    * input      : iPort:�˿�; iMsgType:��Ϣ����; pHandle:���
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	void DealException(IVS_INT32 /*iPort*/, IVS_INT32 /*iMsgType*/,const void* pHandle);

	void Release();

	/***********************************************************************************
    * name       : SetRecordSpace
    * description: ����¼��ռ�
    * input      : pCameraCode:��������; pRecordSpace:¼��ռ���Ϣ;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 SetRecordSpace(const IVS_CHAR* pCameraCode, const IVS_RECORD_POLICY_TIME* pRecordSpace) const;

	/***********************************************************************************
    * name       : DeleteRecordSpace
    * description: ɾ��¼��ռ�
    * input      : pCameraCode:��������; iRecordMethod:¼��ʽ; 
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 DeleteRecordSpace(const IVS_CHAR* pCameraCode, IVS_INT32 /*iRecordMethod*/) const;

	/***********************************************************************************
    * name       : StartPlatformRecord
    * description: ��ʼƽ̨¼��
    * input      : pCameraCode:��������; iRecordTime:¼��ʱ��;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StartPlatformRecord(const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime) const;

	/***********************************************************************************
    * name       : StopPlatformRecord
    * description: ֹͣƽ̨¼��
    * input      : pCameraCode:��������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StopPlatformRecord(const IVS_CHAR* pCameraCode) const;

	/***********************************************************************************
    * name       : StartPURecord
    * description: ��ʼǰ��¼��
    * input      :  pCameraCode:��������; iRecordTime:¼��ʱ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StartPURecord(const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime) const;

	/***********************************************************************************
    * name       : StopPURecord
    * description: ֹͣǰ��¼��
    * input      :  pCameraCode:��������;
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StopPURecord(const IVS_CHAR* pCameraCode) const;

	/***********************************************************************************
    * name       : StartRemoteRecord
    * description: ��ʼԶ��¼��
    * input      : eRecordType:¼������; pCameraCode:��������; iRecordTime:¼��ʱ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StartRemoteRecord(RECORD_TYPE eRecordType,const IVS_CHAR* pCameraCode,IVS_INT32 iRecordTime)const;

	/***********************************************************************************
    * name       : StopRemoteRecord
    * description: ֹͣԶ��¼��
    * input      : eRecordType:¼������; pCameraCode:��������
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 StopRemoteRecord(RECORD_TYPE eRecordType,const IVS_CHAR* pCameraCode)const;

private:
	int m_iPlayHandle;  //���ž��

	CUserMgr *m_pUserMgr; //�û�����ָ�����
	
	unsigned long m_ulPort; //ռ�õĶ˿�

	RECORD_STATUS m_RecordStatus; //¼��״̬
};

#endif
