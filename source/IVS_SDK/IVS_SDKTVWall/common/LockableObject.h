/********************************************************************
    filename	: 	LockableObject.h
    author		:	wx58056
    created		:	2011/10/28
    description	:	�����������������
    copyright	:	Copyright (C) 2011-2015
    history		:	2011/10/28 ��ʼ�汾
 *********************************************************************/

#ifndef LOCKABLE_OBJECT_H
#define LOCKABLE_OBJECT_H

///////////////////////////////////////////////////////////////////////////////
/// @namespace CUMW
/// @brief �ͻ����м��ר�������ռ�
///
/// CUMW֮����ض���
///////////////////////////////////////////////////////////////////////////////
namespace CUMW
{
    ///////////////////////////////////////////////////////////////////////////
    /// @brief �����������
    ///
    /// �ṩ�������ͨ�ýӿ�
    ///////////////////////////////////////////////////////////////////////////
    class CLockableObject
    {
public:
        CLockableObject(){};
        virtual ~CLockableObject(){};

        virtual void Lock(){};

        virtual void Unlock(){};
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ����
    ///
    /// �Զ���������?
    ///////////////////////////////////////////////////////////////////////////
    class CLock
    {
public:
        CLock(CLockableObject& obj) : m_objLockabelObject(obj)
        {
            m_objLockabelObject.Lock();
        };

        ~CLock()
        {
            m_objLockabelObject.Unlock();
        };

private:
        CLockableObject & m_objLockabelObject;
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief �ٽ�������
    ///
    /// ��װ�ٽ�����Ŀ�����
    ///////////////////////////////////////////////////////////////////////////
    class CCriticalSection : public CLockableObject
    {
public:
        CCriticalSection()
        {
            InitializeCriticalSection(&m_CriticalSection);
        };

        virtual ~CCriticalSection()
        {
            DeleteCriticalSection(&m_CriticalSection);
        };

        virtual void Lock()
        {
            EnterCriticalSection(&m_CriticalSection);
        };

        virtual void Unlock()
        {
            LeaveCriticalSection(&m_CriticalSection);
        };

private:
        CRITICAL_SECTION m_CriticalSection;
    };

    ///////////////////////////////////////////////////////////////////////////
    /// @brief ��������
    ///
    /// ��װ������Ŀ�����
    ///////////////////////////////////////////////////////////////////////////
    class CMutex : public CLockableObject
    {
public:
        CMutex()
        {
            m_hMutex = CreateMutex(NULL, FALSE, NULL);
        };

        virtual ~CMutex()
        {
            if (NULL != m_hMutex)
            {
                CloseHandle(m_hMutex);
            }
        };

        virtual void Lock()
        {
            if (NULL != m_hMutex)
            {
                WaitForSingleObject(m_hMutex, INFINITE);
            }
        };

        virtual void Unlock()
        {
            if (NULL != m_hMutex)
            {
                ReleaseMutex(m_hMutex);
            }
        };

private:
        HANDLE m_hMutex;
    };
}

#endif //LOCKABLE_OBJECT_H
