
/******************************************************************************
   ��Ȩ���� (C), 2008-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �ļ���          : LockGuard.h
  �汾��          : 1.0
  ����            : 
  ��������        : 2008-8-17
  ����޸�        : 
  ��������        : ʵ������������
  �����б�        : 
  �޸���ʷ        : 
  1 ����          : 
    ����          : 
    �޸�����      : 
*******************************************************************************/


#ifndef CLOCKGUARD_H_INCLUDE
#define CLOCKGUARD_H_INCLUDE    


#include  "vos.h"

class CLockGuard
{
  public:
    CLockGuard(VOS_Mutex *pMutex);
    ~CLockGuard();
    
  public:
    static void lock(VOS_Mutex *pMutex);
    static void unlock(VOS_Mutex *pMutex);
    
 private:
    CLockGuard();
    VOS_Mutex *m_pMutex;
};

#endif // CLOCKGUARD_H_INCLUDE


