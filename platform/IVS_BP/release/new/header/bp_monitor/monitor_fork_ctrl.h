/********************************************************************
filename    :     monitor_fork_ctrl.h
author      :     s69859
created     :     2012/09/20
description :     ��ؽ��̽���������
copyright   :     Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :     2012/09/20 ��ʼ�汾
*********************************************************************/

#ifndef MONITOR_FORKCTRL_H_
#define MONITOR_FORKCTRL_H_

#include "MainCtrl.h"
#include "Cmd.h"
#include "Thread.h"
#define CHECK_STATFILE_INTERVAL 5


NAMESPACE_BP_BEGIN
class CMonMainctrl : public IService
{
public:
	CMonMainctrl(void);
    virtual ~CMonMainctrl(void);

    virtual int HandleCmd(CCmd* pCmd);

    virtual uint32_t Routing(void);  //���ڼ��;

private:
	void checkProcess();

    bool killprocess(const char *pfile, pid_t currentPid);

private:
	time_t m_iLastCheckStatTime; //�ϴν��̽�����ؼ��ʱ��
	std::string m_szPidFilePath;    //��pid�ļ���·��

};
NAMESPACE_BP_END

#endif /* MONITOR_FORKCTRL_H_ */

