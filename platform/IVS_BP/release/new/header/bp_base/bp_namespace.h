/********************************************************************
filename	: 	bp_namespace.h
author		:	s00191067
created		:	2012/03/27
description	:	�ṩivs �����ռ�궨��
copyright	:	Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history		:	2012/03/27 ��ʼ�汾
*********************************************************************/
#ifndef BP_NAMESPACE_H
#define BP_NAMESPACE_H

namespace BP 
{//lint !e129 !e10
    // �����ռ�������ʼ
    #define NAMESPACE_BP_BEGIN  \
    namespace BP{               \

    // �����ռ���������
    #define NAMESPACE_BP_END    \
    }                           \

    // ���������ռ�
    #define USING_NAMESPACE_BP using namespace BP;

}
#endif //BP_NAMESPACE_H
