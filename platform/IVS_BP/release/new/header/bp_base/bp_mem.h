#ifndef _IVS_BP_MEM_H
#define _IVS_BP_MEM_H

enum BP_MEM_OBJ_TYPE
{
    BP_MEM_NONE     = 0, //Ԥ��
    BP_MEM_LOG_BUF  = 1, //��־������
    BP_MEM_DBCONN   = 2, //���ݿ�����
	MEMID_XMLBUFF   = 3, //XML����
	MEMID_CmdID     = 4, //����ID
	MEMID_MsgBlock  = 5, //��Ϣ��

    //0 ~ 100��BP�ڲ�ʹ��, 101 ~ 200��ҵ��ģ����, ��ҵ��ģ����Զ�����ͬ��ֵ
    MEMID_BP_MAX = 100,

	//���ڴ�֮ǰ����������.
	// �趨�ڴ��������ID�ţ��ݶ�200
    BP_MEM_ID_MAX = 200
};

#ifdef WIN32 

#ifdef BASE_DLL
extern __declspec(dllexport) int g_bp_memstat[];
#else
extern __declspec(dllimport) int g_bp_memstat[];
#endif

#else
extern int g_bp_memstat[];
#endif

/**************************************************************************
* name       : BP_NEW_P
* description: �������ʹ������󣬲�������ID�����м�¼��
* input      : ObjectID ����ID���û��Զ��塣
* output     : NA
* remark     : ����ID���ݶ����200
**************************************************************************/
#define BP_NEW_P(ObjectID, ObjectPtr, Type) \
    do {\
        (ObjectPtr) = NULL;\
        if ((BP_MEM_ID_MAX < ObjectID) || (0 > ObjectID)) break;\
        try {\
            (ObjectPtr) = new Type;\
            if(NULL != (ObjectPtr)){++g_bp_memstat[ObjectID];}\
        }\
        catch (...) {\
            (ObjectPtr) = NULL;\
        }\
    } while (0)

/**************************************************************************
* name       : BP_DEL_P
* description: ���������ͷŶ��󣬲�������ID������������
* input      : ObjectID ����ID���û��Զ��塣
* output     : NA
* remark     : ����ID���ݶ����200
**************************************************************************/
#define BP_DEL_P(ObjectID, ObjectPtr) \
    do {\
        if(NULL != ObjectPtr) \
        {  \
            delete (ObjectPtr);\
            (ObjectPtr) = NULL;\
            --g_bp_memstat[ObjectID];\
        }  \
    } while (0)

/**************************************************************************
* name       : BP_NEW_A
* description: �������ʹ����������飬��������ID�����м�¼��
* input      : ObjectID ����ID���û��Զ��塣
* output     : NA
* remark     : ����ID���ݶ����200
**************************************************************************/
#define BP_NEW_A(ObjectID, ObjectPtr, Type, Size)\
    do {\
        if ((BP_MEM_ID_MAX < ObjectID) || (0 > ObjectID)) break;\
        try {\
            (ObjectPtr) = new Type[Size];\
            if(NULL != (ObjectPtr)){++g_bp_memstat[ObjectID];}\
        }\
        catch (...) {\
            (ObjectPtr) = NULL;\
        }\
    } while (0)

/**************************************************************************
* name       : BP_NEW_A
* description: ���������ͷŶ������飬��������ID������������
* input      : ObjectID ����ID���û��Զ��塣
* output     : NA
* remark     : ����ID���ݶ����200
**************************************************************************/
#define BP_DEL_A(ObjectID, ObjectPtr)\
    do {\
        if(NULL != ObjectPtr) \
        {  \
            delete [] (ObjectPtr);    \
            (ObjectPtr) = NULL;       \
            --g_bp_memstat[ObjectID]; \
        }  \
    } while (0)

// ����һ���ڴ������⸲�Ǻ궨�壬ʹ��Ĭ��ID����Ϊpclint�����ò�����ճ���������
#define HW_NEW(var, Type) \
    do {\
    (var) = NULL;\
    try {\
    (var) = new Type;\
    if(NULL != (var)){++g_bp_memstat[BP_MEM_ID_MAX];}\
    }\
    catch (...) {\
    (var) = NULL;\
    }\
    } while (0)

#define HW_DELETE(var) \
    do {\
    if(NULL != var) \
    {\
    delete (var);\
    (var) = NULL;\
    --g_bp_memstat[BP_MEM_ID_MAX];\
    }\
    } while (0)

#define HW_NEW_A(var, type, size)\
    do {\
    try {\
        (var) = new type[size];\
        if(NULL != (var)){++g_bp_memstat[BP_MEM_ID_MAX];}\
    }\
    catch (...) {\
    (var) = NULL;\
    }\
    } while (0)

#define HW_DELETE_A(var)\
    do {\
    if(NULL != var) \
    {  \
    delete [] (var);    \
    (var) = NULL;       \
    --g_bp_memstat[BP_MEM_ID_MAX]; \
    }  \
    } while (0)

#define SAFE_RELEASE_MB(var) \
    do {\
    if (NULL != var) \
    { \
        if (var->reference_count() > 1) \
        { \
            var->release(); \
        } \
        var->reset(); \
        HW_DELETE(var); \
    } \
    } while (0)
/**************************************************************************
* name       : BP_CheckMemLeak
* description: �������ڴ��Ƿ�й¶��
* input      : NA
* output     : й¶��ID����
* remark     : NA
**************************************************************************/
#ifdef WIN32
extern "C" __declspec(dllexport) int BP_CheckMemLeak(void);
#else
extern "C" int BP_CheckMemLeak(void);
#endif

#endif //_IVS_BP_MEM_H
