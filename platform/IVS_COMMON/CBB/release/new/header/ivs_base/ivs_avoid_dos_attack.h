/********************************************************************
filename    :    ivs_avoid_dos_attack.h
author      :    yWX150774
created     :    2012/12/04
description :    ��ֹDoS�����ĵ���
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/04 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_AVOID_DOS_ATTACK_H
#define CBB_BASE_AVOID_DOS_ATTACK_H


#include "ivs_namespace.h"
#include "ivs_uncopy.hpp"

#include "bp_stdint.h"

#include "ace/Thread_Mutex.h"

#include <map>
#include <utility>

NAMESPACE_CBB_BEGIN

/*************************************************************
 *
 * ��ֹDoS��������, ��һ�����ɿ�������, ����, ʵ��Ϊ����ģʽ
 *
 ************************************************************/
class EXPORT_DLL CAvoidDoSAttack : private CBB::CUnCopy
{
private:
    CAvoidDoSAttack();
    ~CAvoidDoSAttack();

public:
    /*********************************************************
     * ��ȡʵ��
     ********************************************************/
    static CAvoidDoSAttack & GetInstance();

public:
    /*********************************************************
     * ���˷���IP�Ƿ��ǰ�ȫ��
     * Ƶ�����ʵ�IP����������, ����һ��ʱ����Ӻ������Ƴ�
     * ����Ϊ����IP, �ɹ����� IVS_SUCCEED, ʧ�ܷ��� IVS_FAIL
     ********************************************************/
    int Check(uint32_t uiCheckIP);

protected:
    static const uint32_t  LOCK_UP_TIME_SLICE;               // ������������ʱ��
    static const uint32_t  MAX_ACCESS_TIMES_PER_TIME_SLICE;  // ��λʱ��Ƭ�����ķ��ʴ���

    static CAvoidDoSAttack s_aAvoidDoSAttack;

private:
    std::map<
             uint32_t, 
             std::pair<
                       time_t, 
                       uint32_t
                      >
            >                    m_mapIP2CheckTimeAndTimes;  // IP�����ʱ�估��������һһӳ��

    ACE_Thread_Mutex             m_aMapThreadMutex;          // �����map��Ӧ���̻߳�����

    typedef std::map<
                     uint32_t, 
                     std::pair<
                               time_t, 
                               uint32_t
                              >
                    >::iterator  IP2TimeAndTimesMapIterator;
};

NAMESPACE_CBB_END


#endif // CBB_BASE_AVOID_DOS_ATTACK_H
