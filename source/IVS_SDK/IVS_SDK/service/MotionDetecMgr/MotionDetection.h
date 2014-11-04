/********************************************************************
filename	: 	MotionDetection.h
author		:	z00201790
created		:	2012/12/06	
description	:	�����ƶ���������;
copyright	:	Copyright (C) 2011-2015
history		:	2012/12/06 ��ʼ�汾;
*********************************************************************/

#ifndef __MOTIONDETECTION_H__
#define __MOTIONDETECTION_H__

#include "hwsdk.h"
#include "ivs_error.h"


class CUserMgr;
class CMotionDetection
{
public:
	CMotionDetection(void);
	~CMotionDetection(void);

	// �����û�������
	void SetUserMgr(void *pUserMgr);

private:
	CUserMgr *m_pUserMgr;

public:
	
	// ��ѯƽ̨�˶�����������
	IVS_INT32 GetMotionRangeDataPlatform(const IVS_CHAR* pCameraCode,const IVS_TIME_SPAN* pTimeSpan,
		const IVS_INDEX_RANGE* pIndexRange,IVS_MOTION_RANGE_DATA_LIST* pMotionRangeData,IVS_UINT32 uiBufferSize);
};

#endif // __MOTIONDETECTION_H__
