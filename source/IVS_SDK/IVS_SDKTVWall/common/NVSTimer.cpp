#include "vos_config.h"
#include "vos_basetype.h"
#include "vos_common.h"
#include "vos.h"
#include "CumwCommon.h"
#include "NVSTimer.h"

#ifndef WIN32   //��PC-LINT
#define TIMER_SECOND_IN_MS 1000
#define TIMER_MS_IN_US 1000
#endif

#define _TIMER_FL_ "CNVSTimer.cpp", __LINE__
//lint -e526
void ITrigger::onTrigger(void *pArg, ULONGLONG ullScales, TriggerStyle enStyle)
{
}

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
            IVS_DBG_LOG("FILE(%s)LINE(%d): CNVSTimer::~CNVSTimer: thread = %u", 
                _TIMER_FL_, VOS_pthread_self());
            while(itListOfTrigger != m_plistTrigger->end())
            {
				CTimerItem* pTimerItem = (*itListOfTrigger).second;//lint !e64
                VOS_DELETE(pTimerItem);
				pTimerItem = NULL;
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
        IVS_RUN_LOG_ERR( "NVSTimer init: create m_pMutexListOfTrigger fail.");
        return VOS_FAIL;
    }
    
    m_ullRrsAbsTimeScales = VOS_GetTicks()/m_ulTimerScale;

    (void)VOS_NEW( m_plistTrigger );
    if( NULL == m_plistTrigger )
    {
        IVS_RUN_LOG_ERR("NVSTimer init: m_plistTrigger is NULL.");
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
        IVS_RUN_LOG_ERR("Create timer thread failed. error(%d):%s", errno, strerror(errno));
        return VOS_FAIL;
    };
    IVS_DBG_LOG("FILE(%s)LINE(%d): VOS_CreateThread: create timer thread(%d) OK.", 
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
        IVS_RUN_LOG_ERR("NVSTimer exit: m_pVosThread is null");
        return;
    }

    //��ֹͣ�߳�
    this->m_bExit = VOS_TRUE;
    
    errno = 0;   
    long ret_val = VOS_ThreadJoin(m_pVosThread);
    if (ret_val != VOS_OK)
    {
        IVS_RUN_LOG_ERR("Wait timer thread exit failed. ret_val(%ld). error(%d):%s", ret_val, errno, strerror(errno));
    }

    //�ͷ��ڴ�
    VOS_free(m_pVosThread);
    m_pVosThread = VOS_NULL;

    //���ͷ���Դ
    clearTimer();

    //�ͷ���
    (void)VOS_DestroyMutex(m_pMutexListOfTrigger);
    m_pMutexListOfTrigger = VOS_NULL;

    IVS_DBG_LOG("FILE(%s)LINE(%d): CNVSTimer::exit: exit complete.", _TIMER_FL_);
  
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
        IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: pTrigger is NULL");
        return VOS_FAIL;
    }

    if (0 == nScales )
    {
        IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: nScales is zero");
        return VOS_FAIL;
    }    

    if(  VOS_TRUE == m_bExit)
    {
        IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: bExit is VOS_TRUE, thread exit");
        return VOS_FAIL;
    }
    
    CTimerItem *pTimerItem = NULL;
    (void)VOS_NEW(pTimerItem);
    if (NULL == pTimerItem )
    {
        IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: new pTimerItem fail");
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
            IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: get lock failed");
        }
        else
        {
            bLocked = VOS_TRUE;
        }
    }
   
    (void)(m_plistTrigger->insert(ListOfTriggerPair(pTimerItem->m_ullCurScales, pTimerItem)));

    //����
    if(VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfTrigger))
        {
            IVS_RUN_LOG_ERR("NVSTimer RegisterTimer: release lock failed",
                _TIMER_FL_);
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
    ListOfTriggerIte itListOfTrigger = m_plistTrigger->begin();
    ListOfTriggerIte itCurrentTrigger = m_plistTrigger->begin();
    while(itListOfTrigger != m_plistTrigger->end())
    {
        pTimerItem = (*itListOfTrigger).second;//lint !e64
        itCurrentTrigger = itListOfTrigger;
        ++itListOfTrigger;
        (void)(m_plistTrigger->erase(itCurrentTrigger));

        IVS_RUN_LOG_ERR("ClearTimer erase pTimerItem(0x%x)", pTimerItem);
            
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
        IVS_RUN_LOG_ERR("NVSTimer CancelTimer: pTrigger is NULL");
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
            IVS_RUN_LOG_ERR("NVSTimer CancelTimer: get lock failed");
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

    IVS_DBG_LOG("NVSTimer CancelTimer set m_bRemoved=VOS_TRUE"
                "  pTimerItem(0x%x) pTrigger(0x%x) .\n", pTrigger->m_pTimerItem, pTrigger);

    
    //����(�������ͬһ�߳�)
    if(VOS_TRUE == bLocked)
    {
        if (VOS_OK != VOS_MutexUnlock(m_pMutexListOfTrigger))
        {
            IVS_RUN_LOG_ERR("NVSTimer CancelTimer: release lock failed");
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
        ListOfTriggerIte itListOfTrigger = m_plistTrigger->begin();
        ListOfTriggerIte itCurrentTrigger = m_plistTrigger->begin();
        while(itListOfTrigger != m_plistTrigger->end())
        {
            pTimerItem = (*itListOfTrigger).second;//lint !e64
            if(NULL == pTimerItem)
            {
                IVS_RUN_LOG_ERR("pTimerItem is NULL.");
                itCurrentTrigger = itListOfTrigger;
                ++itListOfTrigger;
                (void)(m_plistTrigger->erase(itCurrentTrigger));
                continue;
            }
            
            pTrigger = (ITrigger *)pTimerItem->m_pTrigger;
            
            //�����ʱ����ע����ɾ����ʱ��������������һ��
            if((NULL == pTrigger) || (VOS_TRUE == pTimerItem->m_bRemoved))
            {
                IVS_RUN_LOG_ERR("Timer(0x%x) removed.", pTimerItem);
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
                IVS_RUN_LOG_ERR("Timer(0x%x) remove trigger once timer.",pTimerItem);
                //��ֹpTrigger->onTrigger��ɾ����ʱ��
                pTrigger->m_pTimerItem = NULL;
                VOS_DELETE(pTimerItem);
                continue;
            }

            //�����ظ�������ʱ������Ҫ�޸��´γ�ʱ�¼������¼���multimap
            pTimerItem->m_ullCurScales = ullCurrentScales 
                + pTimerItem->m_ulInitialScales;
            (void)(m_plistTrigger->insert(ListOfTriggerPair(pTimerItem->m_ullCurScales, 
                pTimerItem)));
        };
        (void)VOS_MutexUnlock(m_pMutexListOfTrigger);
    }

    return;
}
