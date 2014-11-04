/********************************************************************
	filename	: 	RecordBookmark.h
	author		:	z00201790
	created		:	2012/12/06	
	description	:	����¼����ǩ������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/

#ifndef __RECORD_BOOKMARK_H__
#define __RECORD_BOOKMARK_H__

#include "RecordXMLProcess.h"

class CUserMgr;
class CRecordBookmark
{
public:
	CRecordBookmark(void);
	~CRecordBookmark(void);
	void SetUserMgr(CUserMgr *pUserMgr);

	/***********************************************************************************
    * name       : AddBookmark
    * description: ����¼����ǩ
    * input      : pCameraCode:���������; pBookmarkTime:��ǩʱ��; pNewBookmarkName:����ǩ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 AddBookmark(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID)const;

	/***********************************************************************************
    * name       : ModifyBookmark
    * description: �޸�¼����ǩ
    * input      :pCameraCode:���������; uiBookmarkID:��ǩID; pNewBookmarkName:����ǩ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 ModifyBookmark(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName)const;

	/***********************************************************************************
    * name       : DeleteBookmark
    * description: ɾ��¼����ǩ
    * input      : pCameraCode:���������; uiBookmarkID:��ǩID
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 DeleteBookmark(IVS_INT32 iRecordMethod,const IVS_CHAR* pDomainCode,const IVS_CHAR* pNVRCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID)const;
	
	/***********************************************************************************
    * name       : GetBookmarkList
    * description: ��ѯ¼����ǩ�б�
    * input      : pUnifiedQuery:��ѯ����; pBookmarkList:��ǩ��ѯ����б�; uiBufSize:�����С
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetBookmarkList(IVS_INT32 iRecordMethod,const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiBufSize)const;

	/***********************************************************************************
    * name       : GetBookmarkListByBackupServer
    * description: ��ѯ¼����ǩ�б�(�ӱ��ݷ�����)
    * input      : pUnifiedQuery:��ѯ����; pBookmarkList:��ǩ��ѯ����б�; uiBufSize:�����С
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 GetBookmarkListByBackupServer(const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiMaxBufSize)const;

private:
	IVS_INT32 GetBookmarkListByBackupServerForOneDomain(IVS_UINT32 uiFromIndex,IVS_UINT32 uiToIndex,const char* cNVRCode,const char* cMBUDomainCode,
		const IVS_QUERY_UNIFIED_FORMAT* pUnifiedQuery,IVS_RECORD_BOOKMARK_INFO_LIST* pBookmarkList,IVS_UINT32 uiMaxBufSize)const;
private:
	CUserMgr *m_UserMgr;

};

#endif
