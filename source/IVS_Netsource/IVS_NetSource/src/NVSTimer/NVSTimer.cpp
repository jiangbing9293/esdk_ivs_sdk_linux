/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �ļ���          : CNVSTimer.cpp
  �汾��          : 1.0
  ����            : duijiying 24362
  ��������        : 2007-4-02
  ����޸�        : 
  ��������        : 
  �����б�        : 
  �޸���ʷ        : 
  1 ����          : 2007-4-02
    ����          : duijiying
    �޸�����      : ����
*******************************************************************************/
#include <stdarg.h>

#include "vos_config.h"
#include "vos_basetype.h"
#include "vos_common.h"
#include "vos.h"
#include "NVSTimer.h"
#include "IVSCommon.h"
#include "ivs_log.h"

ITimerLog *g_pTimerLog = NULL;

#ifndef WIN32   //��PC-LINT
#define TIMER_SECOND_IN_MS 1000
#define TIMER_MS_IN_US 1000
#endif

//NVSTimer
// 4����־����
//#define    TIMER_OPERATOR_LOG    16
#define    TIMER_RUN_LOG         17
//#define    TIMER_SECURITY_LOG    20
//#define    TIMER_USER_LOG        19
#define MAX_TIMER_LOG_LENTH 512

#define _TIMER_FL_ "NVSTimer.cpp", __LINE__
/*******************************************************************************
  Function:       TIMER_WRITE_LOG()
  Description:    ��־��ӡ����
  Calls:            
  Called By:      
  Input:          ��printfһ��
  Output:         �� 
  Return:         ��
*******************************************************************************/
void TIMER_WRITE_LOG(long lLevel, const char *format, ...)
{
    if(NULL == g_pTimerLog)
    {
        return;
    };
    
    char buff[MAX_TIMER_LOG_LENTH + 1];
    buff[0] = '\0';
#ifdef WIN32   
    va_list args = NULL;
#else
	va_list args;
#endif
    va_start (args, format);
    long lPrefix = snprintf (buff, MAX_TIMER_LOG_LENTH, "errno:%d.thread(%u):", 
        errno, (long)VOS_pthread_self());
    if(lPrefix < MAX_TIMER_LOG_LENTH)
    {
        (void)vsnprintf (buff + lPrefix,
            (unsigned long)(MAX_TIMER_LOG_LENTH - lPrefix), format, args);
    }
    buff[MAX_TIMER_LOG_LENTH] = '\0';
    g_pTimerLog->writeLog(TIMER_RUN_LOG, lLevel, buff, (long)strlen(buff));
    va_end (args);
    
};

/*******************************************************************************
  Function:       CNVSTimer::~CNVSTimer()
  Description:    ��������
  Calls:            
  Called By:      
  Input:          ��
  Output:         �� 
  Return:         ��
*******************************************************************************/
CNVSTimer::~CNVSTimer()
{
    try
    {
        if(NULL != m_plistTrigger)
        {
            ListOfTriggerIte itListOfTrigger = m_plistTrigger->begin();
            TIMER_WRITE_LOG(TIMER_DEBUG, "FILE(%s)LINE(%d): CNVSTimer::~CNVSTimer: thread = %u", 
                _TIMER_FL_, VOS_pthread_self());
            while(itListOfTrigger != m_plistTrigger->end())
            {
                VOS_DELETE((*itListOfTrigger).second); //lint !e64 !e1025 !e1514
                ++itListOfTrigger;
            };
            m_plistTrigger->clear();
            VOS_DELETE(m_plistTrigger);
            m_plistTrigger = NULL;
        }
    
        if(m_pVosThread != NULL)
        {
            VOS_free(m_pVosThread);
        }
        m_pVosThread = NULL;

        if(m_pMutexListOfTrigger != NULL)
        {
            (void)VOS_DestroyMutex(m_pMutexListOfTrigger);
        }
        m_pMutexListOfTrigger = NULL;
    }
    catch (...)
    {
    }
};

/*******************************************************************************
  Function:       CNVSTimer::init()
  Description:    ��ʼ������
  Calls:            
  Called By:      
  Input:          ulTimerScale: ��ʱ������
  Output:         �� 
  Return:         
  VOS_SUCCESS: init success
  VOS_FAIL: init fail 
*******************************************************************************/
long CNVSTimer::init(ULONG ulTimerScale)
{
    //��ʼ��ʱ������Ϊ���˳�״̬�������ظ���ͣ��ʧ��
    m_bExit = VOS_FALSE;

    if (ulTimerScale < MinTimerScale)
    {
        m_ulTimerScale = MinTimerScale;
    }
    else
    {
        m_ulTimerScale = ulTimerScale;
    }

    m_pMutexListOfTrigger = VOS_CreateMutex();
    if(NULL == m_pMutexListOfTrigger)
    {
		IVS_LOG(IVS_LOG_ERR, "init falied","create m_pMutexListOfTrigger fail");
        return VOS_FAIL;
    }
    
    m_ullRrsAbsTimeScales = VOS_GetTicks()/m_ulTimerScale;

    (void)VOS_NEW( m_plistTrigger );
    if( NULL == m_plistTrigger )
    {
        if(m_pMutexListOfTrigger != NULL) //lint !e774
        {
            (void)VOS_DestroyMutex(m_pMutexListOfTrigger);
        }
        m_pMutexListOfTrigger = NULL;

		IVS_LOG(IVS_LOG_ERR, "m_plistTrigger is NULL","NA");
        return VOS_FAIL;
    }

    return VOS_SUCCESS;
};

/*******************************************************************************
  Function:       CNVSTimer::run()
  Description:    ������ʱ����߳�
  Calls:            
  Called By:      
  Input:          �� 
  Output:         �� 
  Return:         
  VOS_SUCCESS: init success
  VOS_FAIL: init fail 
*******************************************************************************/
long CNVSTimer::run()
{
    errno = 0;
    if (VOS_OK != VOS_CreateThread((VOS_THREAD_FUNC)invoke, (void *)this, 
                                    &m_pVosThread, VOS_DEFAULT_STACK_SIZE))
    {
        TIMER_WRITE_LOG(TIMER_ERROR, 
            "FILE(%s)LINE(%d): Create timer thread failed. error(%d):%s", 
            _TIMER_FL_, errno, strerror(errno));
        return VOS_FAIL;
    };
    TIMER_WRITE_LOG(TIMER_DEBUG, 
        "FILE(%s)LINE(%d): VOS_CreateThread: create timer thread(%d) OK.", 
        _TIMER_FL_, m_pVosThread->pthead);

    return VOS_SUCCESS;
};

/*******************************************************************************
  Function:       CNVSTimer::exit()
  Description:    �˳���ʱ����߳�
  Calls:            
  Called By:      
  Input:          �� 
  Output:         �� 
  Return:         �� 
*******************************************************************************/
void CNVSTimer::exit()
{
    if(NULL == m_pVosThread)
    {
		IVS_LOG(IVS_LOG_ERR, "m_pVosThread is null","NA");
        return;
    }

    //��ֹͣ�߳�
    this->m_bExit = VOS_TRUE;
    
    errno = 0;   
    long ret_val = VOS_ThreadJoin(m_pVosThread);
    if (ret_val != VOS_OK)
    {
		IVS_LOG(IVS_LOG_INFO, "Wait timer thread exit failed","NA");
    }

    //�ͷ��ڴ�
    VOS_free(m_pVosThread);
    m_pVosThread = VOS_NULL;

    //���ͷ���Դ
    clearTimer();

    //�ͷ���
    if (NULL == m_pMutexListOfTrigger)
    {
        (void)VOS_DestroyMutex(m_pMutexListOfTrigger);
    }
    m_pMutexListOfTrigger = VOS_NULL;
  
    return;
};

/*******************************************************************************
  Function:       CNVSTimer::registerTimer()
  Description:    ע�ᶨʱ��
  Calls:            
  Called By:      
  Input:          pTrigger: ��ʱ�������ʵ��, void *pArg: ��ʱ�������, 
                  nScales: ��ʱ�¼�(��initʱָ����scaleΪ��λ)
                  enStyle: �������    enOneShot: ����һ��, enRepeated: ѭ������
  Output:         �� 
  Return:         
  VOS_SUCCESS: init success
  VOS_FAIL: init fail 
*******************************************************************************/
long CNVSTimer::registerTimer(ITrigger *pTrigger, void *pArg, ULONG nScales, 
    TriggerStyle enStyle)
{
    if (NULL == pTrigger )
    {
		IVS_LOG(IVS_LOG_ERR, "pTrigger is NULL", "NA");
        return VOS_FAIL;
    }

    if (0 == nScales )
    {
		IVS_LOG(IVS_LOG_ERR, "nScales is zero", "NA");
        return VOS_FAIL;
    }    

    if(  VOS_TRUE == m_bExit)
    {
		IVS_LOG(IVS_LOG_ERR, " thread exit", "NA");
        return VOS_FAIL;
    }
    
    CTimerItem *pTimerItem = NULL;
    (void)VOS_NEW(pTimerItem);
    if (NULL == pTimerItem )
    {
		IVS_LOG(IVS_LOG_ERR, " new pTimerItem fail", "NA");
        return VOS_FAIL;
    }
    
    pTrigger->m_pTimerItem = pTimerItem;

    pTimerItem->m_pTrigger = pTrigger;
    pTimerItem->m_pArg = pArg;
    pTimerItem->m_ulInitialScales = nScales;
    pTimerItem->m_ullCurScales = m_ullRrsAbsTimeScales + nScales;
    pTimerItem->m_enStyle = enStyle;
    //BEGIN V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231 
    pTimerItem->m_ulBaseTimeTicks = VOS_GetTicks();
    //END   V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231 

    //����(�����mainloop����ͬһ�̲߳���Ҫ����)
    VOS_BOOLEAN bNeedLock = VOS_FALSE;
    VOS_BOOLEAN bLocked = VOS_FALSE;

	////����simian��������
	//if (NULL != m_pVosThread)
	//{
	//	if(VOS_pthread_self() != m_pVosThread->pthead)
	//	{
	//		bNeedLock = VOS_TRUE;
	//	}
	//}
	//else
	//{
	//	bNeedLock = VOS_TRUE;
	//}
    if (NULL == m_pVosThread)
    {
        bNeedLock = VOS_TRUE;
    }
    else
    {
        if(VOS_pthread_self() != m_pVosThread->pthead)
        {
            bNeedLock = VOS_TRUE;
        }
    }
    
    if(VOS_TRUE == bNeedLock)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfTrigger))
        {
            IVS_LOG(IVS_LOG_ERR, " get lock failed", "NA");
        }
        else
        {
            bLocked = VOS_TRUE;
        }
    }
   
    if (NULL != m_plistTrigger)
    {
        (void)(m_plistTrigger->insert(ListOfTriggerPair(pTimerItem->m_ullCurScales, pTimerItem)));
    }
    
    //����
    if(VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfTrigger))
        {
            IVS_LOG(IVS_LOG_ERR, " release lock failed", "NA");
        }
    } 
    
    return VOS_SUCCESS;
};

/*******************************************************************************
  Function:       CNVSTimer::clearTimer
  Description:    ɾ����ʱ���������е�TimerItem
  Calls:            
  Called By:      
  Input:         �� 
  Output:         �� 
  Return:         
*******************************************************************************/
void CNVSTimer::clearTimer( )
{
    CTimerItem *pTimerItem = NULL;

    if(VOS_OK != VOS_MutexLock(m_pMutexListOfTrigger))
    {
        return;
    };

    if (NULL == m_plistTrigger)
    {
        (void)VOS_MutexUnlock(m_pMutexListOfTrigger);
        return;
    }

    ListOfTriggerIte itListOfTrigger = m_plistTrigger->begin();
    ListOfTriggerIte itCurrentTrigger = m_plistTrigger->begin();
    while(itListOfTrigger != m_plistTrigger->end())
    {
        pTimerItem = (*itListOfTrigger).second;  //lint !e64
        itCurrentTrigger = itListOfTrigger;
        ++itListOfTrigger;
        (void)(m_plistTrigger->erase(itCurrentTrigger));

        TIMER_WRITE_LOG(TIMER_DEBUG, 
            "FILE(%s)LINE(%d): clearTimer erase pTimerItem(0x%x) .\n",
                        _TIMER_FL_, pTimerItem);
            
        continue;
    }

    VOS_DELETE(m_plistTrigger);
    (void)VOS_MutexUnlock(m_pMutexListOfTrigger);
    
}

/*******************************************************************************
  Function:       CNVSTimer::cancelTimer()
  Description:    ע����ʱ��
  Calls:            
  Called By:      
  Input:          pTrigger: ��ʱ�������ʵ��
  Output:         �� 
  Return:         
  VOS_SUCCESS: init success
  VOS_FAIL: init fail 
*******************************************************************************/
long CNVSTimer::cancelTimer(ITrigger *pTrigger)
{
    if(NULL == pTrigger)
    {
        TIMER_WRITE_LOG(TIMER_ERROR, 
            "FILE(%s)LINE(%d): CNVSTimer::cancelTimer: pTrigger is NULL",
            _TIMER_FL_);
        return VOS_FAIL;
    };

    //����(�����mainloop����ͬһ�̲߳���Ҫ����)
    VOS_BOOLEAN bNeedLock = VOS_FALSE;
    VOS_BOOLEAN bLocked = VOS_FALSE;
    if (NULL == m_pVosThread)
    {
        bNeedLock = VOS_TRUE;
    }
    else
    {
        if(VOS_pthread_self() != m_pVosThread->pthead)
        {
            bNeedLock = VOS_TRUE;
        }
    }
    
    if(VOS_TRUE == bNeedLock)
    {
        if (VOS_OK != VOS_MutexLock(m_pMutexListOfTrigger))
        {
			IVS_LOG(IVS_LOG_INFO, "cancelTimer set m_bRemoved=VOS_TRUE", "NA");
        }
        else
        {
            bLocked = VOS_TRUE;
        }
    }
 
    if(pTrigger->m_pTimerItem != NULL)
    {
        pTrigger->m_pTimerItem->m_bRemoved = VOS_TRUE;
        pTrigger->m_pTimerItem->m_pTrigger = NULL;
        pTrigger->m_pTimerItem = NULL;
    }

	//IVS_LOG(LOG_INFO, "cancelTimer set m_bRemoved=VOS_TRUE");

    
    //����(�������ͬһ�߳�)
    if(VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfTrigger))
        {
			IVS_LOG(IVS_LOG_ERR, "cancelTimer set m_bRemoved=VOS_TRUE", "NA");
        }
    }        
    
    return VOS_SUCCESS;
};

/*******************************************************************************
  Function:       CNVSTimer::mainLoop()
  Description:    �߳�ִ�еĶ�ʱ�����ѭ��
  Calls:            
  Called By:      
  Input:          ��
  Output:         �� 
  Return:         ��
*******************************************************************************/
void CNVSTimer::mainLoop()
{
    ULONGLONG ullCurrentScales = 0;
    while(VOS_FALSE == m_bExit)
    {
#ifdef WIN32
        Sleep(m_ulTimerScale);    
#else
        struct timeval tv;
        tv.tv_sec = (long)(m_ulTimerScale/TIMER_SECOND_IN_MS);
        tv.tv_usec = (m_ulTimerScale%TIMER_SECOND_IN_MS)*TIMER_MS_IN_US;
        (void)select(0, VOS_NULL, VOS_NULL, VOS_NULL, &tv);
#endif        
        
        //����       
        CTimerItem *pTimerItem = NULL;
        ITrigger *pTrigger = NULL;
        ++m_ullRrsAbsTimeScales ;//�ڲ�ʱ���׼����һ���̶�
        ullCurrentScales = m_ullRrsAbsTimeScales;

        if(VOS_OK != VOS_MutexLock(m_pMutexListOfTrigger))
        {
            break;
        };
        // modi by wKF71044 at [2012-8-16 11:33:58]ָ���жϿ�
        if (NULL == m_plistTrigger)
        {
			(void)VOS_MutexUnlock(m_pMutexListOfTrigger);
            continue;
        }

        ListOfTriggerIte itListOfTrigger = m_plistTrigger->begin();
        ListOfTriggerIte itCurrentTrigger = m_plistTrigger->begin();
        while(itListOfTrigger != m_plistTrigger->end())    //lint !e613
        {
            pTimerItem = (*itListOfTrigger).second;  //lint !e64
            if(NULL == pTimerItem)
            {
				//IVS_LOG(LOG_INFO, " pTimerItem is NULL.");
                itCurrentTrigger = itListOfTrigger;
                ++itListOfTrigger;
                (void)(m_plistTrigger->erase(itCurrentTrigger));
                continue;
            }
            
            pTrigger = (ITrigger *)pTimerItem->m_pTrigger;
            
            //�����ʱ����ע����ɾ����ʱ��������������һ��
            if((NULL == pTrigger) || (VOS_TRUE == pTimerItem->m_bRemoved))
            {
				//IVS_LOG(LOG_INFO, "Timer removed.");
                itCurrentTrigger = itListOfTrigger;
                ++itListOfTrigger;
                (void)(m_plistTrigger->erase(itCurrentTrigger));
                VOS_DELETE(pTimerItem);
                continue;
            };

            //BEGIN V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231 
            ULONG ulTimeTickInterval = VOS_GetTicks() - pTimerItem->m_ulBaseTimeTicks;
            if (ulTimeTickInterval < pTimerItem->m_ulInitialScales * m_ulTimerScale)
            {
                ++itListOfTrigger;
                continue;
            }
            pTimerItem->m_ulBaseTimeTicks = VOS_GetTicks();
            //END   V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231 
            
            itCurrentTrigger = itListOfTrigger;
            ++itListOfTrigger;
            (void)(m_plistTrigger->erase(itCurrentTrigger));

            //����trigger��onTrigger����
            pTrigger->onTrigger(pTimerItem->m_pArg, 
                ullCurrentScales, pTimerItem->m_enStyle);

            //���ֻ����һ�Σ�ɾ����ʱ��
            if(enOneShot == pTimerItem->m_enStyle)
            {
				//IVS_LOG(LOG_INFO, " Timer remove trigger once timer.");
                //��ֹpTrigger->onTrigger��ɾ����ʱ��
                pTrigger->m_pTimerItem = NULL;
                VOS_DELETE(pTimerItem);
                continue;
            }

            //�����ظ�������ʱ������Ҫ�޸��´γ�ʱ�¼������¼���multimap
            pTimerItem->m_ullCurScales = ullCurrentScales + pTimerItem->m_ulInitialScales;

			if (NULL != m_plistTrigger )
			{
				itListOfTrigger = m_plistTrigger->insert( ListOfTriggerPair(pTimerItem->m_ullCurScales, pTimerItem));
			}
			
        };
        (void)VOS_MutexUnlock(m_pMutexListOfTrigger);
    }

    return;
}


