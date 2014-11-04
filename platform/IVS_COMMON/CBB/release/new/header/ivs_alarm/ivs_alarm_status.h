/********************************************************************
filename    :    ivs_alarm_status.h
author      :    
created     :    2012.11.3
description :    ������ʽ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012.11.3 ��ʼ�汾
*********************************************************************/

#ifndef CBB_ALARM_STATUS_H
#define CBB_ALARM_STATUS_H

#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

#define ALARM_TYPE_LENGTH (10)
#define ALARM_INFO_LENGTH (256)

typedef struct
{
    char AlarmType[ALARM_TYPE_LENGTH];
    char AlarmInfo[ALARM_INFO_LENGTH];
} Alarm_Item;

/************************************************************************
 * ��ʼ���ӿڣ����ڳ�ʼ�������ļ�����·��
 * pszFilePath    �����ļ�����·��
 * ����ֵ         �ɹ�����IVS_SUCCEED��ʧ�ܷ���IVS_FAIL
 ***********************************************************************/
extern "C"  EXPORT_DLL int IVS_AlarmInit(const char * pszFilePath);

/************************************************************************
 * �ͷ���Դ�ӿ�
 * ����ֵ    
 ***********************************************************************/
extern "C"  EXPORT_DLL void IVS_AlarmRelease();

/************************************************************************
 * ���������¼�ӿ�
 * pAlarmItem    ��¼��Ϣָ��
 * ����ֵ        ����ӳɹ�������IVS_SUCCEED
 *               �����ʧ�ܻ��¼�Ѵ��ڣ�����IVS_FAIL
 ***********************************************************************/
extern "C"  EXPORT_DLL int IVS_AddAlarmItem(const Alarm_Item * pAlarmItem);

/************************************************************************
* ɾ�������¼�ӿ�
* pAlarmItem    ��¼��Ϣָ��
* ����ֵ        ��ɾ���ɹ����¼�����ڣ�����IVS_SUCCEED
*               ��ɾ��ʧ�ܣ�����IVS_FAIL
***********************************************************************/
extern "C"  EXPORT_DLL int IVS_DelAlarmItem(const Alarm_Item * pAlarmItem);

/************************************************************************
* ��ȡ�����¼�ӿ�
* pAlarmItem    ��¼��Ϣָ��
* pCount        ��¼����
* ����ֵ        ���ɹ�������IVS_SUCCEED
*               ��ʧ�ܣ�����IVS_FAIL
* ��ע          ��GetAllAlarmItem�ӿڵ�һ������Ϊ��ʱ����ʾ��ȡ�澯��¼�������ڵڶ�����������
*               ��GetAllAlarmItem�ӿڵ�һ��������Ϊ�գ���ʾ�澯��¼�����׵�ַ���ڶ�������Ϊ���鳤��
*               ����Ӧ�ò����Ե�һ�ֵ��÷�ʽ����ȡ�������ٸ������������ڴ�ռ䣬
*               �ڶ��ε��õ�ʱ�򣬽���ǰ������ڴ�ռ��׵�ַ�����鳤����Ϊ��������
***********************************************************************/
extern "C"  EXPORT_DLL int IVS_GetAllAlarmItem(Alarm_Item * pAlarmItem, int * pCount);

NAMESPACE_CBB_END

#endif // CBB_ALARM_STATUS_H
