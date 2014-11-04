#ifndef __IVS_BACKTRACE_H__
#define __IVS_BACKTRACE_H__


/* linux ʹ��backtrace���� ����ʱ���� -g  -funwind-tables  -rdynamic ѡ�� */
/* windows ʹ��backtrace ��Ҫ����Ӧ�� pdb �ļ����������г���Ŀ¼�� */

/*
 * ��ʼ���⼰���̷�����Դ
 */
int  IVS_BackTraceInit();

/*
 * ������̷�����Դ
 */
void IVS_BackTraceClean();

/*
 * Dump ����ջ��������
 */
size_t IVS_DumpBackTrace(char *buffer, size_t len);


#define MAX_BACKTRACE_SIZ  16   // �����ݲ���

#endif 