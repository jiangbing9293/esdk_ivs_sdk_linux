/********************************************************************
filename    :    bp_obj_pool.h
author      :    j00213308
created     :    2012/05/07
description :    �����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/05/07 ��ʼ�汾
*********************************************************************/
#ifndef _BP_OBJ_POOL_H
#define _BP_OBJ_POOL_H

#include <map>
#include <list>
#include "ace_header.h"
#include "bp_namespace.h"
#include "IObject.h"
#include "bp_def.h"

NAMESPACE_BP_BEGIN

typedef std::list<IObject*>               CListObj;
typedef std::list<IObject*>::iterator     CListObjIter;
typedef std::map<int, CListObj>           CMapObj;
typedef std::map<int, CListObj>::iterator CMapObjIter;
typedef ACE_Recursive_Thread_Mutex         MutexThread;

// ��������
class EXPORT_DLL CObjPool
{
public:
    /**************************************************************************
    * name       : GetInstance
    * description: ��ȡ����صĵ���
    * input      : NA
    * return     : CObjPool* ʵ�� NULL ��������ʧ��
    * remark     : ��̬��������ֱ�ӵ���
    **************************************************************************/
    static CObjPool* GetInstance(void);
    ~CObjPool(void);

    /**************************************************************************
    * name       : AddObjectTemplate
    * description: ������ע���������
    * input      : IObject* ����ĳ���͵�ʵ��
    * return     : OK �ɹ� FAIL ʧ��
    * remark     : �ȴ���ĳ�����͵�ʵ�����������Ϊģ�壬���������Ե�һ��ʵ��Ϊԭ��
	* remark     : ���������Ϊģ���ʵ�������Բ����ֶ��������ػḺ���ͷ�
    **************************************************************************/
    int AddObjectTemplate(IObject*);

    /**************************************************************************
    * name       : GetObject
    * description: �Ӷ�����У����ݶ������ͣ���ȡһ����Ӧ��ʵ��
    * input      : iType: ��������ֵ
    * return     : IObject* �������ָ��
    * remark     : �������ʹ�IObject��GetObjectType()�������
    **************************************************************************/
    IObject* GetObject(int iType);

    /**************************************************************************
    * name       : ReleaseObject
    * description: �ͷŶ���ʵ�����Żس�
    * input      : IObject* ���ͷŵĶ���ָ��
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ReleaseObject(IObject*);

private:
    CObjPool(void);

private:
    static CObjPool*    m_pInstance;        // ��ָ��
    static MutexThread  m_mutex;            // ����

    CMapObj             m_freeObjMap;       // ���õ��������
};

NAMESPACE_BP_END

#endif //_BP_OBJ_POOL_H
