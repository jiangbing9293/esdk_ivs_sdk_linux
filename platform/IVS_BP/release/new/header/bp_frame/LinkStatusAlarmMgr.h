/********************************************************************
filename    :    LinkStatusAlarmInfo.h
author      :    l00193369
created     :    2013/03/11
description :    ���Ӹ澯������
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2013/03/11 ��ʼ�汾
*********************************************************************/
#ifndef LINK_STATUS_ALARM_MGR_H
#define LINK_STATUS_ALARM_MGR_H


#include "bp_namespace.h"
#include "bp_mem.h"
#include "bp_map.h"
#include "ILink.h"
#include <string>
#include "ace_header.h"

NAMESPACE_BP_BEGIN

typedef struct linkStatusWarnInfo
{
	LINK_STATUS_REPORT_INFO   linkStatusReportInfo;
	time_t                    tTime;
	int                       iNetElementType;
} LINK_STATUS_WARN_INFO;

typedef std::map<string, LINK_STATUS_WARN_INFO*> LSWIMap; //keyֵΪnetElement+LinkID
typedef LSWIMap::iterator          LSWIMapIterator;

class CLinkStatusAlarmMgr
{
public:
	~CLinkStatusAlarmMgr();
	static CLinkStatusAlarmMgr& Instance();

	/**************************************************************************
    * name       : AddAlarm
    * description: ��map�����һ���澯
    * input      : iNetElementType        ��Ԫ����
	*              pLinkStatusReportInfo  �澯��Ϣ
    * output     : N/A
    * return     : �ɹ�����IVS_SUCCESSED,ʧ�ܷ���IVS_FAIL
    * remark     : N/A
    **************************************************************************/
	int AddAlarm(int iNetElementType, LINK_STATUS_REPORT_INFO * pLinkStatusReportInfo);

	/**************************************************************************
    * name       : GetAlarm
    * description: ���ӵ������
    * input      : pCmd    : �������
    * output     : N/A
    * return     : �ɹ����ض�Ӧ�ĸ澯��Ϣָ�룬�޸澯����NULL
    * remark     : N/A
    **************************************************************************/
	LINK_STATUS_WARN_INFO * GetAlarm();

	/**************************************************************************
    * name       : Release
    * description: ���map�����Ϣ
    * input      : pCmd    : �������
    * output     : N/A
    * return     : �ɹ����ض�Ӧ�ĸ澯��Ϣָ�룬�޸澯����NULL
    * remark     : N/A
    **************************************************************************/
	void Release();

private:
	CLinkStatusAlarmMgr();
	LINK_STATUS_WARN_INFO * NewLinkStatusWarnInfo(int iNetElementType, LINK_STATUS_REPORT_INFO * pLinkStatusReportInfo);


	LSWIMap m_LSWIMapForClose;
	LSWIMap m_LSWIMapForOpen;
	ACE_Thread_Mutex                    m_mutex;  // ������Ϣmap��;
};

NAMESPACE_BP_END 

#endif