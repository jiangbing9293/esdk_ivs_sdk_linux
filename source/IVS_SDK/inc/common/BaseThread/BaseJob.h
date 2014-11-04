/************************************************************
     Copyright, 2008 - 2010, Huawei Tech. co.,Ltd.
         All Rigths  Reserved
-------------------------------------------------------------
Project Code   :VRPV8
File name      :BaseJbo.h
Author         :����    z00003175
Description    :��Ҫ�����߳�TASKʱ�������ͷ�ļ�
-------------------------------------------------------------
History
Date           Author         Modification
20100720       ����           created file
*************************************************************/

#ifndef BASEJOB_H_HEADER_INCLUDED_B3F0ABAE
#define BASEJOB_H_HEADER_INCLUDED_B3F0ABAE

//#include <WTypes.h>
#include "vos.h"

//##ModelId=4C0E09D9011A
class BaseJob
{
  public:

      //����״̬����
      //created --- �����մ��������ǹ�����Ϊ������ʼ�����Կ���Ϊ�̹߳���
      //running  --- ������������״̬
      //finished --- �������״̬
      enum JOB_STATE
      {
          created,
          running,
          finished
      };
/************************************************************
Func Name    :MainLoop
Date Created :2010-07-20
Author       :����  z00003175
Description  :������ѭ���ӿڶ��壬������ɹ���ѭ���߼�
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
    //##ModelId=4C0E09F1038B
    virtual void MainLoop() = 0;
/************************************************************
Func Name    :kill
Date Created :2010-07-20
Author       :����  z00003175
Description  :ֹͣ����ѭ���ӿڶ��壬������ͣ����
Input        :
Output       :
Return       :
Caution      :
History
Date           Author         Modification
20100720       ����           created file

************************************************************/
    //##ModelId=4C0E0A5003A9
    virtual void kill() = 0;

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
    BaseJob();
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
    virtual ~BaseJob();

protected:
    VOS_Mutex* m_hMutex;

};



#endif /* BASEJOB_H_HEADER_INCLUDED_B3F0ABAE */
