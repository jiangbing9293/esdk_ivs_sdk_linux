/********************************************************************
filename    :    ivs_uncopy.hpp
author      :    yWX150774
created     :    2012/11/05
description :    ���岻�ܿ����Ļ���CUnCopy
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/11/05 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_UNCOPY_H
#define CBB_BASE_UNCOPY_H


#include "ivs_namespace.h"

NAMESPACE_CBB_BEGIN

/*********************************************
 * ���岻�ܿ�������CUnCopy
 * ֻҪ�̳��������಻ʵ�ֿ�������(�������졢������ֵ)
 * Ҳ���ǲ��ܿ�������
*********************************************/
class EXPORT_DLL CUnCopy
{
protected:
    CUnCopy()
    {

    }

    ~CUnCopy()
    {

    }

private:
    CUnCopy(const CUnCopy &);
    CUnCopy & operator = (const CUnCopy &);
};

NAMESPACE_CBB_END


#endif // CBB_BASE_UNCOPY_H
