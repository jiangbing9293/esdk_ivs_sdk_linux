/************************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :AutoMutex.h
Author         :����    z00003175
Description    :������Զ�������
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/
#pragma once

#ifdef WIN32
	#include <WTypes.h>
#endif

#include "vos.h"

class AutoMutex
{
public:
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
    //AutoMutex(HANDLE hMutex);
	AutoMutex(VOS_Mutex* m_hMutex);
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
    ~AutoMutex(void);
private:
    //HANDLE m_hMutex;
	VOS_Mutex* m_hMutex;
};//lint !e1712 ����ҪĬ�Ϲ��캯��;
