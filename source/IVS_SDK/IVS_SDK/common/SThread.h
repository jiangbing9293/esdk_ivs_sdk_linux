#ifndef _CTHREADHEADER_
#define _CTHREADHEADER_
#include "vos.h"
#include "hwsdk.h"
#include "ivs_error.h"

//lint -e601 -e10 -e40 -e1013 -e63
class CSThread 
{
public:
	CSThread ();
	virtual ~CSThread ();

	// �����߳�
	IVS_INT32 CreateThread();

	// �ر��߳�
	void CloseThread();

	// �Ƿ�����߳�
	int	IsCreated()const { return (m_pVosThread != VOS_NULL);}

protected:

	// ִ���¼�
	virtual void Process ();	

	static void *invoke(void *argc)
	{
		CSThread *pThis = (CSThread *)argc;
		pThis->Process();
		VOS_pthread_exit(NULL);
		pThis->m_pVosThread = VOS_NULL;
		return NULL;
	};

public:
	
	 VOS_Thread *m_pVosThread;
	
};
 //lint +e601 +e10 +e40 +e1013 +e63

#endif // _CTHREADHEADER_
