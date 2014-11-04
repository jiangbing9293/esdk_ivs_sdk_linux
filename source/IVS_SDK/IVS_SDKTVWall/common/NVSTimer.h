/********************************************************************
 filename    :    NVSTimer.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ��ʱ��
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#ifndef CTIMER_H_INCLUDE
#define CTIMER_H_INCLUDE

#ifdef WIN32
#pragma warning(disable: 4786)
#endif

#include <list>
#include <map>

#include "vos_config.h"
#include "vos_basetype.h"
#include "vos_common.h"
#include "vos.h"

const ULONG DefaultTimerScale = 100; //ȱʡ��ʱ����Ϊ100ms
const ULONG MinTimerScale = 1; //��ʱ������СΪ1ms

class ITrigger;
class CTimerItem;

class CCmpTimeOut
{
  public:
    bool operator()(const ULONGLONG ullTimerOut1, const ULONGLONG ullTimerOut2) const
    {
        return (ullTimerOut1 < ullTimerOut2);
    };
};
typedef std::multimap<ULONGLONG, CTimerItem *, CCmpTimeOut> ListOfTrigger;
typedef std::pair<ULONGLONG const, CTimerItem *> ListOfTriggerPair;
typedef ListOfTrigger::iterator ListOfTriggerIte;

typedef enum tagTriggerStyle
{
    enOneShot = 0,
    enRepeated = 1
} TriggerStyle;

class ITrigger 
{
  public:
    ITrigger() : m_pTimerItem(NULL)
    {

    };
    virtual ~ITrigger(){};
    
  public:
    virtual void onTrigger(void *pArg, ULONGLONG ullScales, TriggerStyle enStyle);
        
  public:
    CTimerItem *m_pTimerItem;
};

class CTimerItem
{
  public:
    CTimerItem() : m_pTrigger(NULL),m_pArg(NULL),m_bRemoved(VOS_FALSE),m_ulInitialScales(0),m_ullCurScales(0),m_enStyle(enOneShot),m_ulBaseTimeTicks(0)
    {
    };
    
  public:
    ITrigger *m_pTrigger;
    void *m_pArg;
    ULONG m_ulInitialScales; 
    ULONGLONG m_ullCurScales; 
    TriggerStyle m_enStyle;
    VOS_BOOLEAN m_bRemoved;

    //BEGIN V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231 
    ULONG m_ulBaseTimeTicks;    //��ǰ��׼ʱ�� 
    //END   V100R001C05 ADD 2010-02-03 ligengqiang l00124479 for �������������Ļ�ʯ�������������� NP-231
};

// 4����־����
#define    TIMER_OPERATOR_LOG    16
#define    TIMER_RUN_LOG         17
#define    TIMER_SECURITY_LOG    20
#define    TIMER_USER_LOG        19

// 4����־����
enum TIMER_LOG_LEVEL
{
    TIMER_EMERGENCY = 0,
    TIMER_ERROR = 3,
    TIMER_WARNING = 4,
    TIMER_DEBUG = 7
};

class CNVSTimer  
{
  public:
	  CNVSTimer() : m_plistTrigger(NULL)
					,m_ulTimerScale(DefaultTimerScale)
					,m_ullRrsAbsTimeScales(0)
					,m_pMutexListOfTrigger(NULL)
					,m_pVosThread(NULL)
					,m_bExit(VOS_FALSE)
    {
    }

    ~CNVSTimer();
    
  public:
    virtual long init(ULONG ulTimerScale);
    virtual long run();
    void exit();

  public:
     virtual long registerTimer(ITrigger *pRrsTrigger, void *pArg, ULONG nScales, 
        TriggerStyle enStyle);
     virtual long cancelTimer(ITrigger *pRrsTrigger);

    void clearTimer( );

  public:
    static void *invoke(void *argc)
    {
        CNVSTimer *pTimer = (CNVSTimer *)argc;
        pTimer->mainLoop();
        VOS_pthread_exit(NULL);
        return NULL;
    };

    void mainLoop();

  private:
    ULONG m_ulTimerScale;
    ULONGLONG m_ullRrsAbsTimeScales;
    ListOfTrigger *m_plistTrigger;
    VOS_Mutex *m_pMutexListOfTrigger;
    VOS_Thread *m_pVosThread;
    volatile VOS_BOOLEAN m_bExit;    
};


#endif //CTIMER_H_INCLUDE


