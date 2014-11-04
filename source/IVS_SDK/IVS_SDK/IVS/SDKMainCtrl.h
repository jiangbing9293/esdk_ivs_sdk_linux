/********************************************************************
	filename	: 	CSDKMainctrl.h
	author		:	z00193167
	created		:	2012/11/01	
	description	:	BP���߳�
	copyright	:	Copyright (C) 2011-2015
	history		:	2012/11/01 ��ʼ�汾
*********************************************************************/

#ifndef SDK_MAIN_CTRL_H
#define SDK_MAIN_CTRL_H

#include "MainCtrl.h"
#include "Cmd.h"
#include "SDKDef.h"

USING_NAMESPACE_BP

class CSDKMainctrl : public CMainCtrl
{
public:
	CSDKMainctrl();
	~CSDKMainctrl();

	virtual int HandleCmd(CCmd* pCmd);

	virtual uint32_t Routing(void); //lint !e601 !e10 //���ڼ��;  

	virtual int Init();

	//virtual void UnInit();
protected:
	virtual void OnLinkOpened(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

	virtual void OnLinkClosed(const std::string& strDevName, int iNetElementType, const std::string& strLinkID, const std::string& strIP, int iPort);

private:
	// ����澯�ϱ���Ϣ
	int OnAlarmNotify(CCmd* pCmd)const;
	// �������ܷ���
	int HandleMAUCmd(CCmd* pCmd) const;
	// ����澯״̬
	int OnAlarmStatusNotify(CCmd* pCmd)const;
	// �û�����֪ͨ
    int OnUserOffLineNotify(CCmd* pCmd)const;
	// ������������֪ͨ
	int DoLinkageAction(CCmd* pCmd)const;
    // ������������֪ͨ
	int StopLinkageAction(CCmd* pCmd)const;
	//��XML�е��豸����ƴװ�����
	int ModifyDevCode(CXml& xml, IVS_CHAR* pData)const;
	// ���ͷ��ֵ���ǰ���豸
	int OnDisCoveredDevNotify(CCmd* pCmd)const;
	// �豸�澯�ϱ�OMU
	int OnDevAlarmNotify(CCmd* pCmd)const;
	// �����ֶ�¼��״̬
#ifdef WIN32
	int OnManualRecordStateNotify(CCmd* pCmd)const;
#endif
	// ģʽ�켣¼�ƽ���֪ͨ
	int OnModeCruiseRecordOver(CCmd* pCmd)const;
	// ���豸���豸������Ϣ�첽��Ӧ����
    int HandleGetUserChannelListRsp(CCmd* pCmd)const;
	// add by zwx211831, Date:20140605 ����ͻ��˹ر���Ƶҵ��֪ͨ
	int OnShutdownStreamNotify(CCmd* pCmd)const;

private:
	typedef int (CSDKMainctrl::*FnProcessCmd)(CCmd* pCmd)const; 

	typedef struct
	{
		uint32_t reqID;
		FnProcessCmd fnProcessCmd;
	} REQUEST_NAME;
	static REQUEST_NAME m_reqNames[];
	FnProcessCmd GetFnProcessCmd(uint32_t enReqID) const;
};

#endif //SDK_MAIN_CTRL_H
