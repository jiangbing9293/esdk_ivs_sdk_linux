/*******************************************************************************
//  ��Ȩ����    ��Ϊ�������޹�˾
//  �� �� ����  IVS_Player
//  �� �� ����  Timer.h
//  �ļ�˵��:   
//  ��    ��:   IVS V100R002C01
//  ��    ��:   chenxianxiao/00206592
//  ��������:   2012/11/14
//  �޸ı�ʶ��
//  �޸�˵����
*******************************************************************************/

#ifndef __IVS_TIMER_H__
#define __IVS_TIMER_H__

//time_a after time_b
#define TIME_AFTER(time_a, time_b) ((long)(time_b) - (long)(time_a) < 0)

//time_a before time_b
#define TIME_BEFORE(time_a, time_b) TIME_AFTER(time_b, time_a)

int  IVS_StarTimer();
void IVS_StopTimer();

unsigned long  IVS_GetTimeTick();

extern unsigned long timeTick;

unsigned long long IVS_GetSysTickUs(void);

#endif
