/*******************************************************************************
//  ��Ȩ����    2012 ��Ϊ�������޹�˾
//  �� �� ����  IVS_Player
//  �� �� ����  vos_atomic.h
//  �ļ�˵��:   ԭ�ӱ�������
//  ��    ��:   IVS V100R002C01
//  ��    ��:   w00210470
//  ��������:   2013/10/10
//  �޸ı�ʶ��
//  �޸�˵����
*******************************************************************************/

#ifndef __VOS_ATOMIC_H__
#define __VOS_ATOMIC_H__

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif 

typedef struct {
	volatile long counter;
} vos_atomic_t;

#if defined WIN32 || defined _MSC_VER

static inline long atomic_inc(vos_atomic_t *v)
{
	return InterlockedIncrement(&v->counter);
}

static inline long atomic_dec(vos_atomic_t *v)
{
	return InterlockedDecrement(&v->counter);
}

static inline bool atomic_dec_and_test(vos_atomic_t *v)
{
	return (0 == InterlockedDecrement(&v->counter));
}

static inline bool atomic_inc_and_test(vos_atomic_t *v)
{
	return  (0 == InterlockedIncrement(&v->counter));
}

#else  // linux

static inline long atomic_inc(vos_atomic_t *v)
{
	return __sync_add_and_fetch((&v->counter),1);
}

static inline long atomic_dec(vos_atomic_t *v)
{
	return __sync_sub_and_fetch((&v->counter),1);
}

#endif //  defined WIN32 || defined _MSC_VER






#endif 



