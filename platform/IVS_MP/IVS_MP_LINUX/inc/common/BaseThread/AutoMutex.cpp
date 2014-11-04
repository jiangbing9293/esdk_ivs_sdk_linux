/***********************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :Decoder.cpp
Author         :����    z00003175
Description    :������Զ�������
Function List  :
   AutoMutex
   ~AutoMutex
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/
// #include "StdAfx.h"
#include "AutoMutex.h"

/************************************************************
Func Name    :AutoMutex
Date Created :2010-07-20
Author       :����  z00003175
Description  :���캯��,������Ļ������������,�����뻥����
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
AutoMutex::AutoMutex(VOS_Mutex* hMutex)
:m_hMutex(hMutex)
{
    if (NULL == m_hMutex)
    {
        return;
    }
	/*
    ::WaitForSingleObject(m_hMutex,INFINITE);
	*/
	VOS_MutexLock(m_hMutex);
}

/************************************************************
Func Name    :~AutoMutex
Date Created :2010-07-20
Author       :����  z00003175
Description  :��������,�ͷŻ�����
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
AutoMutex::~AutoMutex(void)
{
    if (m_hMutex)
    {
        //::ReleaseMutex(m_hMutex);
        //m_hMutex = NULL;
		VOS_MutexUnlock(m_hMutex);
    }
}
