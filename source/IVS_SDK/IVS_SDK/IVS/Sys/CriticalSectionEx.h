#ifndef __CCRITICAL_SECTIONEX_H__
#define __CCRITICAL_SECTIONEX_H__

#include "vos.h"

//�����Լ����ٽ����ֱ࣬����init��ʹ��CCRITICAL_SECTION�����ɱ����γ�ʼ��;
class CCRITICAL_SECTIONEX
{
public:
	CCRITICAL_SECTIONEX();
	virtual ~CCRITICAL_SECTIONEX();
	void EnterCriticalSectionEx();
	void LeaveCriticalSectionEx();
protected:

	VOS_Mutex *m_pMutex;
};

#endif