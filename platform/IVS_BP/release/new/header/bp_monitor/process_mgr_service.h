/********************************************************************
filename    :     process_mgr_service.h
author      :     s69859
created     :     2012/09/20
description :     ��ؽ���,ͨ���ź��������ӽ���
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 ��ʼ�汾
*********************************************************************/

#ifndef PROCESS_MGR_SERVICE_H_
#define PROCESS_MGR_SERVICE_H_

#include "MainCtrl.h"
#include "ivs_xml.h"
#include "bp_config_alarmer.h"

#define TRUE_TAG  "True"
#define FALSE_TAG "False"
#define  MAXPROCESS 16
#define MAXPHL 1024


NAMESPACE_BP_BEGIN

typedef struct stProcessStatus
{
	//������
	std::string szProcessName;//[MAXPHL];

	//ģ����--�������ɸ澯�ļ�������
	std::string szModuleName;//[MAXPHL];

	//�Ƿ����
	std::string szIfLoad;//[MAXPHL];

	//��չ�ļ�·��
	std::string szExeFilePath;//[MAXPHL];

	//�Ƿ���Ҫ�ػ�
	std::string szIsKill;

	//����ģʽ�������ͼ�Ⱥ:0 ; ˫��:1
	std::string szWorkMode;

	//�ñ�־λ�����ж��Ƿ���Ҫ�����澯�ļ�
	bool bifRuning;

	bool bifFristRun;

	pid_t iProcessID;
	
	stProcessStatus()
	{
		  bifRuning = false;
		  bifFristRun = true;
		  iProcessID = 0;
		  szProcessName = "";
		  szModuleName = "";
		  szIfLoad = "";
		  szExeFilePath = "";
		  szWorkMode = "";
		  szIsKill = "";
	}
	
    //���ڽṹ�廥����ʵ���
    BP_Recursive_Thread_Mutex mutex;
	
} PROCESS_STATUS;

class CProcessMgrService : public CMainCtrl, private CConfigAlarmer
{
public:
     CProcessMgrService();
	 virtual ~CProcessMgrService();
	 virtual uint32_t Routing(void);
	 virtual int HandleCmd(CCmd* pCmd);
	 virtual void InitService(void);
	 PROCESS_STATUS* getProcessMap();
	 int getProcessMapSize();
private:
     void runDaemon(void);
     bool initMember(CBB::CXml& oCXml);
	 bool DaemonCreateProcess(PROCESS_STATUS* pProcessMap, int pIndex);
	 bool CreateWarnFile(std::string szFilePath);
#ifdef WIN32
	 int InitWinService();
#endif

private:
	void checkPidFile();

    void CheckProcessItemConfig(
        const std::vector<bool> & vFind, 
        const std::vector<std::string> & vItem, 
        const std::vector<std::string> & vValue, 
        const std::vector<std::string> & vDefault);

private:
    int m_iConfNum;
	PROCESS_STATUS m_oProcessMap[MAXPROCESS];
	std::string m_szWarnFilePath;
	time_t m_iLastCheckStatTime;
};
NAMESPACE_BP_END
#endif /* PROCESS_MGR_SERVICE_H_ */
