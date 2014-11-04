/********************************************************************
filename    :    ivs_memory.h
author      :    yWX150774
created     :    2012/12/11
description :    CBB ģ�����õ����ڴ�����
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
history     :    2012/12/11 ��ʼ�汾
*********************************************************************/

#ifndef CBB_BASE_MEMORY_H
#define CBB_BASE_MEMORY_H


#define CBB_NEW(var, Type)               \
    do                                   \
    {                                    \
        try                              \
        {                                \
            (var) = new Type;            \
        }                                \
        catch (...)                      \
        {                                \
            (var) = NULL;                \
        }                                \
    } while (false)

#define CBB_DELETE(var)                  \
    do                                   \
    {                                    \
        if(NULL == var)                  \
        {                                \
            break;                       \
        }                                \
        try                              \
        {                                \
            delete (var);                \
        }                                \
        catch (...)                      \
        {                                \
                                         \
        }                                \
        (var) = NULL;                    \
    } while (false)

#define CBB_NEW_A(var, Type, size)       \
    do                                   \
    {                                    \
        try                              \
        {                                \
            (var) = new Type[size];      \
        }                                \
        catch (...)                      \
        {                                \
            (var) = NULL;                \
        }                                \
    } while (false)

#define CBB_DELETE_A(var)                \
    do                                   \
    {                                    \
        if(NULL == var)                  \
        {                                \
            break;                       \
        }                                \
        try                              \
        {                                \
            delete [] (var);             \
        }                                \
        catch (...)                      \
        {                                \
                                         \
        }                                \
        (var) = NULL;                    \
    } while (false)

// �ж�ָ���Ƿ�Ϊ�� 
#define CBB_CHECK_POINTER( pPoint, retValue )   \
    if ( NULL == ( pPoint ) )                   \
    {                                           \
        return retValue;                        \
    }

// �ж�ָ���Ƿ�Ϊ��,�޷���ֵ,���ܻ���ʾ���˷ֺ�
#define CBB_CHECK_POINTER_VOID( pPoint )        \
    if ( NULL == ( pPoint ) )                   \
    {                                           \
        return ;                                \
    }

#ifdef WIN32
#define STRTOK  strtok_s
#else
#define STRTOK  strtok_r
#endif

#endif // CBB_BASE_MEMORY_H
