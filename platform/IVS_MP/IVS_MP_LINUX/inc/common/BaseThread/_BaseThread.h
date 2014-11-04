/************************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :_BaseThread.h
Author         :����    z00003175
Description    :��Ҫ�����߳�ʱ�������ͷ�ļ�
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/


#ifndef BASETHREAD_H_HEADER_INCLUDED_B3F0F5E3
#define BASETHREAD_H_HEADER_INCLUDED_B3F0F5E3
class BaseJob;
#include "BaseJob.h"
//##ModelId=4C0E0850009F
class _BaseThread
{
  public:
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
    _BaseThread(BaseJob* pBaseJob, int StackSize);

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
    ~_BaseThread();
  private:
    //##ModelId=4C0F5F6402FC
    //HANDLE m_hThread;
	VOS_Thread *m_pThread;

    //##ModelId=4C3C03CD00F8
    BaseJob* m_pBaseJob;
    //##ModelId=4C3C034700F8
};//lint !e1712 ����Ĭ�Ϲ��캯��;
#endif /* BASETHREAD_H_HEADER_INCLUDED_B3F0F5E3 */
