#ifdef WIN32
#include "../../stdafx.h"
#endif
#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif /* __cpluscplus */
#endif /* __cpluscplus */

#include "vos.h"



VOS_Mutex *VOS_CreateMutex( VOID )
{
	ULONG ulResult = VOS_OK ;

	VOS_Mutex *pstMutex = VOS_NULL ;

	pstMutex = (VOS_Mutex *) VOS_malloc(sizeof(VOS_Mutex));//lint !e838
	if ( NULL == pstMutex  )
	{
		return VOS_NULL ;
	}
#if VOS_APP_OS == VOS_OS_LINUX
	ulResult = (ULONG)pthread_mutex_init( &pstMutex->mutex, 0);
	if( VOS_OK != ulResult )
	{
		VOS_free( pstMutex );        
		return VOS_NULL ;    
	}
#elif VOS_APP_OS == VOS_OS_WIN32
	pstMutex->mutex = CreateMutex(NULL,0,NULL);
	if (NULL == pstMutex->mutex)
	{
		VOS_free( pstMutex );        
		return VOS_NULL ;    
	}
	(void)ulResult; //��PCLINT
#endif
	return pstMutex ;
}


ULONG VOS_DestroyMutex( VOS_Mutex *pstMutex )
{  
	ULONG ulResult = VOS_OK;
	if (NULL == pstMutex)
	{
		return VOS_ERR;
	}
#if VOS_APP_OS == VOS_OS_LINUX
	pthread_mutex_destroy( &pstMutex->mutex );
#elif VOS_APP_OS == VOS_OS_WIN32
	(void)CloseHandle(pstMutex->mutex);/*lint !e613*/ //�޴�����
#endif
	VOS_free( pstMutex );

	return ulResult ;
}


ULONG VOS_MutexLock( VOS_Mutex *pstMutex )
{
	ULONG ulResult = VOS_OK;

	if(NULL == pstMutex)
	{
		return VOS_ERR;
	}

#if VOS_APP_OS == VOS_OS_LINUX 
	ulResult = (ULONG)pthread_mutex_lock(&pstMutex->mutex);
	if( VOS_OK != ulResult )
	{
		return ulResult ;
	}
#elif VOS_APP_OS == VOS_OS_WIN32
	ulResult = WaitForSingleObject(pstMutex->mutex,INFINITE);//lint !e838
	if(WAIT_OBJECT_0 != ulResult)//lint !e835
	{
		return VOS_ERR;
	}
#endif
	return VOS_OK ;
}/*lint !e818*///ʹ�ù���ƽ̨Դ���룬�Ƿ�Const����Ҫ��

ULONG VOS_MutexUnlock( VOS_Mutex *pstMutex )
{    
	ULONG ulResult = VOS_OK;

	//BEGIN V100R001C05 ADD 2010-02-08 ligengqiang l00124479 for ��Ƶ���жϺ�ͻ�����ʧ
	if(NULL == pstMutex)
	{
		return VOS_ERR;
	}
	//END   V100R001C05 ADD 2010-02-08 ligengqiang l00124479 for ��Ƶ���жϺ�ͻ�����ʧ

#if VOS_APP_OS == VOS_OS_LINUX
	ulResult = (ULONG)pthread_mutex_unlock(&pstMutex->mutex);
	if( VOS_OK != ulResult )
	{
		return ulResult ;
	}
#elif VOS_APP_OS == VOS_OS_WIN32
	if((NULL == pstMutex)/*lint !e774*/
		|| (TRUE != ReleaseMutex(pstMutex->mutex)))
	{
		ulResult = VOS_ERR;
		return ulResult;
	}
#endif
	return VOS_OK ;
}/*lint !e818*/ //�޴�����

#ifdef __cplusplus
#if __cplusplus
}
#endif /* __cpluscplus */
#endif /* __cpluscplus */


