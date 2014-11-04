/********************************************************************
filename    :    IObject.h 
author      :    b00209723
created     :    2012/04/12
description :    ý��Ự�ӿ�
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/04/12 ��ʼ�汾
*********************************************************************/
#ifndef  I_BP_OBJECT_H
#define  I_BP_OBJECT_H

class IObject
{
public:
    virtual ~IObject(){};

    /**********************************************************************
    * name           : GetObjectType
    * description    : ��ȡ����ֵ
    * input          : NA
    * output         : NA
    * return         : ��������ֵ
    * remark         : 
    *********************************************************************/
    int GetObjectType() const {return m_iType;}

    /**********************************************************************
    * name           : NewSelf
    * description    : ����һ���ö����ʵ��
    * input          : NA
    * output         : NA
    * return         : IObject* ����ʵ��
    * remark         : ���麯����������ʵ����δ����ö����һ������Ʒ
    *********************************************************************/
    virtual IObject* NewSelf() = 0;

    /**********************************************************************
    * name           : Reset
    * description    : ���ö���״̬
    * input          : NA
    * output         : NA
    * return         : NA
    * remark         : ���麯����������ʵ�־��幦��
    *********************************************************************/
    virtual void Reset() = 0;

protected:
    int     m_iType;         // �洢��������ֵ
};

#endif //I_BP_OBJECT_H
