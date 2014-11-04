
/******************************************************************************
   ��Ȩ���� (C), 2008-2011, ��Ϊ�������޹�˾

 *
 *                 ���λ�����
 *
 *	������
 *      1���������� 2008-08-07 (���� 00114494)
 *
 *	˵����
 *      1����дλ��˵��
 *         readerΪ������ͷ�����������ݿ�ʼλ��
 *         writerΪ������ͷ������д���ݿ�ʼλ��
 *
 *      2�����ݱ�����Ҫ���������������ͼ��
 *
 *         A��   reader      writer
 *                 ��          ��
 *           ��������������������������������
 *
 *         B��   writer              reader
 *                 ��                  ��
 *           ��������������������������������
 *
 */

#ifndef RING_CACHE_H
#define RING_CACHE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

////#include "vos.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "Windows.h"
#pragma warning(disable : 4786 4503 4996)


class CRingCache
{
	public:
		CRingCache();
		~CRingCache();
	public:

		//��õ�ǰ�����������ݳ��Ⱥͻ��������ȵı����İٷ���
		unsigned long GetUsingPercent() const;

		//���û�������С������������ɺ󻺳�Ĵ�С
		unsigned long SetCacheSize(unsigned long ulCacheSize);

		//��õ�ǰ��������С
		unsigned long GetCacheSize() const;

		//�鿴ָ���������ݣ�����������Ȼ������Щ���ݣ�����ʵ�ʶ�ȡ���ݳ���
		unsigned long Peek(char* pBuf, unsigned long ulPeekLen);

		//��ȡָ���������ݣ�������Щ���ݴӻ����������������ʵ�ʶ�ȡ���ݳ���
		unsigned long Read(char* pBuf, unsigned long ulReadLen);

		//дָ���������ݣ�����ʵ��д���ݳ��ȣ����������ռ䲻������ֹд��
		unsigned long Write(const char* pBuf, unsigned long ulWriteLen);

		//��õ�ǰ���������ݴ�С
		unsigned long GetDataSize() const;

		//��õ�ǰ���໺���С
		unsigned long GetEmptySize() const;

		//�������
		void Clear();
	private:
		CRITICAL_SECTION m_cs;	//���������ʱ���

		char*	m_pBuffer;		//������
		unsigned long	m_ulBufferSize;	//��������С
		unsigned long	m_ulDataSize;	//���ݳ���

		//�������������ֽڴ�0��ʼ��ţ���(m_lBufferSize-1)
		//������ֵ���Ǵ˱�ţ���������ƫ��ֵ
		unsigned long	m_ulReader;		//������ͷ��(���ݶ�ȡ��)
		unsigned long	m_ulWriter;		//������ͷ��(����д���)
};

#endif //RING_CACHE_H
