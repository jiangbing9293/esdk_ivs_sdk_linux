/******************************************************************************
   ��Ȩ���� (C), 2001-2011, ��Ϊ�������޹�˾

 ******************************************************************************
  �� �� �� : rtsp_client_api.h
  ��    �� : y00182336
  �������� : RTSP�ͻ���ͨѶ��
  �޸���ʷ :
    1 ���� : 2011-10-12
      ���� : y00182336
      �޸� : ����

 ******************************************************************************/
#ifndef __RTSP_CLIENT_API_H_
#define __RTSP_CLIENT_API_H_

#include "rtsp_client_datatype.h"


#ifdef __cplusplus
extern "C"
{
#endif

#ifndef WIN32
//#define __stdcall
#define __declspec(...)
#endif

namespace RTSP_SDK{

    /***************************************************************************
     �� �� �� : RTSP_CLIENT_Init
     �������� : ͨѶ���ʼ��
     ������� : ��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_Init();


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SetLog
     �������� : ͨѶ����־�̳߳�ʼ�����������ó�ʼ������־����ӡ
     ������� : unLevel       ��־�ȼ�����Ҫ��RTSP_LOG_LEVEL_Eö����ȡֵ
                bDebugflag    ��ӡDEBUG��Ϣ��־
                pszPath       ��־�ļ�·��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetLog(const unsigned int unLevel, const int iDebugflag, 
                                                         const char* pszPath);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_GetHandle
     �������� : ��ȡ���Ӿ��
     ������� : bBlocked        ͬ��/�첽��ʽ
     ������� : nHandle         �������Ӿ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport)  int RTSPAPI RTSP_CLIENT_GetHandle(const bool bBlocked, 
                                                             long *nHandle);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SetExceptionCallBack
     �������� : �����쳣�ص�����
     ������� : cbExceptionCallBack         �쳣�ص�����
               pUser                       �û��������ص������з���
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetExceptionCallBack(fExceptionCallBack cbExceptionCallBack, 
                                                                       void* pUser);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SetDataCallBack
     �������� : ����ý�����ص�����
     ������� :  nHandle                     ���Ӿ��
                cbDataCallBack              ý�����ص�����
                pUser                       �û��������ص������з���
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetDataCallBack(long nHandle, fDataCallBack cbDataCallBack, 
                                                                  void* pUser);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SendSetupMsg
     �������� : ����RTSP SETUP����
     ������� :  nHandle                    ���Ӿ��
                pstServerAddr               RTSP��������ַ
                sURL                        SETUP��Ϣ��װ�е�URL
                unTimeout                   SETUP��Ϣ��ʱʱ�䣨��λ��S��
                pstLocalMediaAddr           ͬ��ģʽ�£�ý�������ؽ��յ�ַ���첽ģʽ��Ϊ��
     ������� : pstPeerMediaAddr            ͬ��ģʽ�£�ý�������Ͷ˵�ַ���첽ģʽ��Ϊ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport)  int RTSPAPI RTSP_CLIENT_SendSetupMsg(long nHandle , const INET_ADDR* pstServerAddr,
                                                                const char *sURL, unsigned int unTimeout, bool bTcpFlag,
                                                                const MEDIA_ADDR* pstLocalMediaAddr, MEDIA_ADDR *pstPeerMediaAddr);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SendPlayMsg
     �������� : ����RTSP PLAY����
     ������� :  nHandle                    ���Ӿ��
                sURL                        PLAY��Ϣ��װ�е�URL
                dScale                      �ط�/���ز�������
                sRange                      �ط�/����ʱ���
                unTimeout                   PLAY��Ϣ��ʱʱ�䣨��λ��S��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendPlayMsg(long nHandle , const char *sURL, 
                                                              double dScale, const MEDIA_RANGE_S* stRange, 
                                                              unsigned int unTimeout);

    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SendOptionMsg
     �������� : ����RTSP OPTIONS����
     ������� :  nHandle                    ���Ӿ��
                sURL                        OPTIONS��Ϣ��װ�е�URL
                unTimeout                   OPTIONS��Ϣ��ʱʱ�䣨��λ��S��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendOptionMsg(long nHandle,const char *sURL,unsigned int unTimeout, char *strMsg, int &iLength);
    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SendPauseMsg
     �������� : ����RTSP PAUSE����
     ������� :  nHandle                    ���Ӿ��
                sURL                        PAUSE��Ϣ��װ�е�URL
                unTimeout                   PAUSE��Ϣ��ʱʱ�䣨��λ��S��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendPauseMsg(long nHandle , const char *sURL, 
                                                               unsigned int unTimeout);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_SendTeardownMsg
     �������� : ����RTSP TEARDOWN����
     ������� :  nHandle                     ���Ӿ��
                sURL                        TEARDOWN��Ϣ��װ�е�URL
                unTimeout                   PAUSE��Ϣ��ʱʱ�䣨��λ��S��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendTeardownMsg(long nHandle , const char *sURL, 
                                                                  unsigned int unTimeout);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_GetStatus
     �������� : ��ȡ���ӵ�ǰ״̬
     ������� : nHandle                     ���Ӿ��
     ������� : ��
     �� �� ֵ : RTSP_SESSION_STATUS -����״̬
    ***************************************************************************/
    __declspec(dllexport) RTSP_SESSION_STATUS RTSPAPI RTSP_CLIENT_GetStatus(long nHandle);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_FreeHandle
     �������� : �ͷ����Ӿ��
     ������� : nHandle                     ���Ӿ��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_FreeHandle(long nHandle);


    /***************************************************************************
     �� �� �� : RTSP_CLIENT_FreeHandle
     �������� : �ͷ�ͨѶ��
     ������� : ��
     ������� : ��
     �� �� ֵ : RET_OK-�ɹ�     RET_FAIL-ʧ��
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_Cleanup();

    }

#ifdef __cplusplus
}
#endif

#endif //__RTSP_CLIENT_API_H_


