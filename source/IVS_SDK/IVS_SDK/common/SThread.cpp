
// #include "stdafx.h"
#include "SThread.h"
//#include "NSS_Log_Manager.h"
//lint -e40 -e63 -e10 -e1401 -e1055 -e746 -e1013 -e1762
CSThread::CSThread ()
	:m_pVosThread(VOS_NULL)
{
}

CSThread::~CSThread ()
{
	if(m_pVosThread != VOS_NULL)
	{
		VOS_free(m_pVosThread);
	}

	m_pVosThread = VOS_NULL;
}

// ִ���¼�
void CSThread::Process ()
{
	return;
}

// �����߳�
IVS_INT32 CSThread::CreateThread()
{
	if (IsCreated())
	{
		// ���߳��Ѵ�
		return IVS_SUCCEED;
	}

	if (VOS_OK != VOS_CreateThread((VOS_THREAD_FUNC)invoke, (void *)this, 
		&m_pVosThread, VOS_DEFAULT_STACK_SIZE))
	{
		return IVS_FAIL;
	};

	// �����ɹ�
	return IVS_SUCCEED;

}

void CSThread::CloseThread()
{
	//IVS_LOG(LOG_INFO, "Enter %s", __FUNCTION__);

	if (NULL == m_pVosThread)
	{
		//IVS_LOG(LOG_INFO, "Leave %s", __FUNCTION__);
		return;
	}

	if(VOS_pthread_self() != m_pVosThread->pthead)
	{
		//IVS_LOG(LOG_INFO, "wait %s", __FUNCTION__);
		VOS_ThreadJoin(m_pVosThread);
	}
	
	//�ͷ��ڴ�
	VOS_free(m_pVosThread);
	m_pVosThread = VOS_NULL;

	//IVS_LOG(LOG_INFO, "Leave %s", __FUNCTION__);
}

//lint +e40 +e63 +e10 +e1401 +e1055 +e746 +e1013 +e1762
