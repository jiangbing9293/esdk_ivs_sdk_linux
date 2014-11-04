/********************************************************************
	filename	: 	RealPlayBookmark.h
	author		:	z00201790
	created		:	2012/12/06	
	description	:	����¼����ǩ������;
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/

#ifndef __REALPLAY_BOOKMARK_H__
#define __REALPLAY_BOOKMARK_H__

#include "RealXMLProcess.h"

class CUserMgr;
class CRealPlayBookmark
{
public:
	CRealPlayBookmark(void);
	~CRealPlayBookmark(void);
	void SetUserMgr(CUserMgr *pUserMgr);

	/***********************************************************************************
    * name       : AddBookmark
    * description: ����ʵ����ǩ
    * input      : pCameraCode:���������; pBookmarkTime:��ǩʱ��; pNewBookmarkName:����ǩ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 AddBookmark(const IVS_CHAR* pDomainCode,const IVS_CHAR* pCameraCode,const IVS_CHAR* pBookmarkName,const IVS_CHAR* pBookmarkTime,IVS_UINT32* uiBookmarkID)const;

	/***********************************************************************************
    * name       : ModifyBookmark
    * description: �޸�ʵ����ǩ
    * input      :pCameraCode:���������; uiBookmarkID:��ǩID; pNewBookmarkName:����ǩ��
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 ModifyBookmark(const IVS_CHAR* pDomainCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID,const IVS_CHAR* pNewBookmarkName)const;

	/***********************************************************************************
    * name       : DeleteBookmark
    * description: ɾ��ʵ����ǩ
    * input      : pCameraCode:���������; uiBookmarkID:��ǩID
    * output     : NA
    * return     : �ɹ�����0��ʧ�ܷ��ظ���������
    * remark     : NA
    ***********************************************************************************/
	IVS_INT32 DeleteBookmark(const IVS_CHAR* pDomainCode,const IVS_CHAR* pCameraCode,IVS_UINT32 uiBookmarkID)const;
	

private:
	CUserMgr *m_UserMgr;
};
#endif
