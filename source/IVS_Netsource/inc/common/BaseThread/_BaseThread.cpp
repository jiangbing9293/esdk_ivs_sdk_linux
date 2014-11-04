/***********************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :_BaseThread.cpp
Author         :����    z00003175
Description    :�����̡߳��˳��߳�
Function List  :
    _BaseThread
    ~_BaseThread
    ThreadFun
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/
#ifdef WIN32
#include <windows.h>
#include <process.h>    /* _beginthread, _endthread */
#endif

#include "_BaseThread.h"
#include "hwsdk.h"
/************************************************************
Func Name    :ThreadFun
Date Created :2010-07-20
Author       :����  z00003175
Description  :�߳���ѭ��
Input        :
    BaseJob* job  :Task����ָ��
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
static unsigned int __SDK_CALL ThreadFun(void *arg)
{
	BaseJob* job  = (BaseJob*)arg;
	if (NULL == job)
	{
		//_endthreadex(0);
		VOS_pthread_exit(0);
		//modify by limingjie lWX175483��2013-5-29  Ӧ�÷���unsigned int
		return 1;
	}
    job->MainLoop();
	//_endthreadex(0);
	VOS_pthread_exit(0);
	return 0;
}


/************************************************************
Func Name    :_BaseThread
Date Created :2010-07-20
Author       :����  z00003175
Description  :�̵߳Ĵ���
Input        :
    BaseJob* job  :Task����ָ��
    int StackSize  :�̴߳���ʱ��ջ��С 
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
//##ModelId=4C0E087E015B
_BaseThread::_BaseThread(BaseJob* pBaseJob, int StackSize)
:m_pBaseJob (pBaseJob)
{
	/*
    m_hThread = (HANDLE)_beginthreadex((LPSECURITY_ATTRIBUTES)0,
                                (DWORD)StackSize,
                                 ThreadFun,
                                (LPVOID)pBaseJob,
                                0, NULL);
	
    if(m_hThread == NULL)
    {
        //��¼��־

        return ;
    }*/
	if (VOS_OK != VOS_CreateThread((VOS_THREAD_FUNC)ThreadFun, (void *)pBaseJob, &m_pThread, StackSize))
	{
		return;
	}
	if(NULL == m_pThread)
	{
		return;
	}
}

/************************************************************
Func Name    :~_BaseThread
Date Created :2010-07-20
Author       :����  z00003175
Description  :�̶߳�����������������ͷ�
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
//##ModelId=4C0E08980310
_BaseThread::~_BaseThread()
{
    try
    {
        m_pBaseJob->kill();
		/*
        if (m_hThread != NULL)
        {
            WaitForSingleObject(m_hThread, INFINITE);
            CloseHandle(m_hThread);
            m_hThread = NULL;
        }
		*/
		if(m_pThread != NULL)
		{
			VOS_ThreadJoin(m_pThread);
			m_pThread = NULL;
		}

        // add by s00191067 ��Ա����δ���;
        m_pBaseJob = NULL;
        // end 
    }
    catch(...)
    {
         //IVS_LOG(IVS_LOG_ERR, "_BaseThread", "~_BaseThread() exception!\n");//lint !e1551
        //exit(1);
    }

}

