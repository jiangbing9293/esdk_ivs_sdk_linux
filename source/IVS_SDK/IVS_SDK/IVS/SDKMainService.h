#ifndef SDK_MAIN_SERVICE_H
#define SDK_MAIN_SERVICE_H
#include "NSSOperator.h"
#include "Cmd.h"
#include "hwsdk.h"

class CSDKMainService
{
public:
	/**************************************************************************
	* name       : ProcessAlarmAlarmNotifyCmd
	* description: ����澯��Ϣ֪ͨCmd
	* input      : CCmd
	* output     : NA
	* return     : IVS_ALARM_NOTIFY �澯��Ϣ�ṹ��
	* remark     : NA
	**************************************************************************/
	static IVS_ALARM_NOTIFY* ProcessAlarmAlarmNotifyCmd(CCmd* pCmd);

	/**************************************************************************
	* name       : ProcessDoLinkageActionCmd
	* description: ������������ִ��֪ͨ
	* input      : CCmd
	* output     : NA
	* return     : IVS_CHAR* ֪ͨxml
	* remark     : NA
	**************************************************************************/
	//static IVS_CHAR* ProcessDoLinkageActionCmd(CCmd* pCmd);

	/**************************************************************************
	* name       : ProcessBALearningSchedule
	* description: �������ܷ���
	* input      : CCmd
	* output     : NA
	* return     : IVS_CHAR* xml
	* remark     : NA
	**************************************************************************/
	static IVS_CHAR* ProcessMAUCmd(CCmd* pCmd);

    //// �û�����֪ͨ
    //static IVS_INT32 ParseUserOffLineXML(CXml& xmlRsq, IVS_USER_OFFLINE_NOTIFY &stOffLineInfo);

    //static IVS_ALARM_NOTIFY* ProcessUserOffLineCmd(CCmd* pCmd);

	/**************************************************************************
	* name       : ProcessAlarmStatusNotify
	* description: �澯��Ϣ״̬�ϱ�
	* input      : CCmd
	* output     : NA
	* return     : IVS_ALARM_STATUS_NOTIFY* ״̬�ṹ��
	* remark     : NA
	**************************************************************************/
	static IVS_ALARM_STATUS_NOTIFY* ProcessAlarmStatusNotify(CCmd* pCmd);

	/**************************************************************************
	* name       : ProcessDevNotify
	* description: ���ͷ��ֵ���ǰ���豸
	* input      : CCmd
	* output     : NA
	* return     : ���ֵ��豸��Ϣ
	* remark     : NA
	**************************************************************************/
	static IVS_CHAR* ProcessDevNotify(CCmd* pCmd);

	/**************************************************************************
	* name       : ProcessDevAlarmNotify
	* description: �豸�澯�ϱ�
	* input      : CCmd
	* output     : NA
	* return     : IVS_DEVICE_ALARM_NOTIFY* �豸�澯�ϱ��ṹ��
	* remark     : NA
	**************************************************************************/
	static IVS_INT32 ProcessDevAlarmNotify(CCmd* pCmd, IVS_DEVICE_ALARM_NOTIFY* pDevAlarmNotify);
	/**************************************************************************
	* name       : ProcessManualRecordStateNotify
	* description: �ֶ�¼����Ϣ״̬�ϱ�
	* input      : CCmd
	* output     : NA
	* return     : IVS_MANUAL_RECORD_STATUS_NOTIFY* ״̬�ṹ��
	* remark     : NA
	**************************************************************************/
#ifdef WIN32
	static IVS_INT32 ProcessManualRecordStateNotify(CCmd* pCmd,IVS_MANUAL_RECORD_STATUS_NOTIFY& stManualRecordStatusNotify);
#endif

	/**************************************************************************
	* name       : ProcessModeCruiseRecordOver
	* description: ģʽ�켣¼�ƽ���֪ͨ
	* input      : CCmd
	* output     : NA
	* return     : ģʽ�켣¼�ƽ�����Ϣ
	* remark     : NA
	**************************************************************************/
	//static IVS_CHAR* ProcessModeCruiseRecordOver(CCmd* pCmd);
	
};


#endif // SDK_MAIN_SERVICE_H
