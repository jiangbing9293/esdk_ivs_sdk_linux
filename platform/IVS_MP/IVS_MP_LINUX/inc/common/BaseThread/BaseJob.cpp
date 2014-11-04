/***********************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :BaseJob.cpp
Author         :����    z00003175
Description    :��ʼ��������TASK
Function List  :
  BaseJob
  ~BaseJob
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/
// #include "stdafx.h"
#include "BaseJob.h"

/************************************************************
Func Name    :BaseJob
Date Created :2010-07-20
Author       :����  z00003175
Description  :Task���캯������Ҫ��ɶ��ź����Ĵ���
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
//##ModelId=4C3C08450286
BaseJob::BaseJob()
:m_hMutex(NULL)
{
    //m_hMutex = ::CreateMutex(NULL,FALSE,NULL);
	m_hMutex = VOS_CreateMutex();
}

/************************************************************
Func Name    :~BaseJob
Date Created :2010-07-20
Author       :����  z00003175
Description  :Task������������Ҫ��ɶ��ź����ͷ�
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
//##ModelId=4C3C085A0063
BaseJob::~BaseJob()
{
	/*
    if (m_hMutex != NULL)
    {
        ::CloseHandle(m_hMutex);
        m_hMutex = NULL;
    }
	*/
	VOS_DestroyMutex(m_hMutex);
}

