/********************************************************************
filename	: 	buff_pool.h
author		:	s90003596
created		:	2012/09/21
description	:	�������ͷ�ļ�
copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history		:	2012/03/27 ��ʼ�汾
*********************************************************************/

#ifndef _buff_pool_h
#define _buff_pool_h


#include "bp_namespace.h"
#include "bp_mutex.h"
#include "bp_list.h"
#include "bp_map.h"

NAMESPACE_BP_BEGIN

/**
 * @class CBuffPool
 * @brief �������
 *        ʵ�ֲ�ͬ��С����Ļ����
 */
class CBuffPool
{
private:
    CBuffPool(const CBuffPool& obj);
    CBuffPool& operator=(const CBuffPool& obj);
protected:
    //���캯����ֻ����Ϊ����ʹ��
    CBuffPool();
public:
    ~CBuffPool();

    static CBuffPool& instance();

    enum BUFF_POOL_CONST
    {
        BUFF_HDR_SIZE = sizeof(int),    //������ͷ����С�����ڱ��滺������С��
    };

    /**
     * ����һ������Ļ����������������Ԫ����λ<max_buf_num>, ÿ������Ĵ�СΪ<buf_size>�ֽ�.
     * �ɹ�����0��ʧ�ܷ��ش����롣
     */
    int CreatePool(int max_buf_num, int buf_size);

    /**
     * ��ȡ��СΪ<buf_size>�ֽڵĻ���.
     * �ɹ����ػ���ָ�루�����1�ֽڵĵ�ַ����ʧ�ܷ���NULL��
     */
    char *GetBuff(int buf_size);

    /**
     * �ͷŻ���<buff>.
     */
    void ReleaseBuff(char *buff);
protected:
    static CBuffPool static_instance;

    typedef CList<char *> BUF_LIST_T;

#pragma pack(push, 1)
    struct BuffList
    {
        BUF_LIST_T BufList;   //�����б�
        int BufSize;          //�����С, ��λ�ֽ�
        int MaxNum;           //���Ԫ����
        int BufNum;           //�ѷ�������� 
    };
#pragma pack(pop)

    typedef CMap<int, BuffList *> BUF_MAP_T;

    /**
     * ��������ӳ�䣺����Ϊ�����С��ֵΪ��������.
     */
    BUF_MAP_T m_BuffListMap;

    BP_Thread_Mutex m_Mutex;
};

NAMESPACE_BP_END

#endif //_buff_pool_h


