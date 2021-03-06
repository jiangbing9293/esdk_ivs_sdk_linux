/******************************************************************************
   版权所有 (C), 2001-2011, 华为技术有限公司

 ******************************************************************************
  文 件 名 : rtsp_client_api.h
  作    者 : y00182336
  功能描述 : RTSP客户端通讯库
  修改历史 :
    1 日期 : 2011-10-12
      作者 : y00182336
      修改 : 创建

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
     函 数 名 : RTSP_CLIENT_Init
     功能描述 : 通讯库初始化
     输入参数 : 无
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_Init();


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SetLog
     功能描述 : 通讯库日志线程初始化，若不调用初始化，日志不打印
     输入参数 : unLevel       日志等级，需要从RTSP_LOG_LEVEL_E枚举中取值
                bDebugflag    打印DEBUG消息标志
                pszPath       日志文件路径
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetLog(const unsigned int unLevel, const int iDebugflag, 
                                                         const char* pszPath);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_GetHandle
     功能描述 : 获取连接句柄
     输入参数 : bBlocked        同步/异步方式
     输出参数 : nHandle         创建连接句柄
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport)  int RTSPAPI RTSP_CLIENT_GetHandle(const bool bBlocked, 
                                                             long *nHandle);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SetExceptionCallBack
     功能描述 : 设置异常回调函数
     输入参数 : cbExceptionCallBack         异常回调函数
               pUser                       用户参数，回调函数中返回
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetExceptionCallBack(fExceptionCallBack cbExceptionCallBack, 
                                                                       void* pUser);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SetDataCallBack
     功能描述 : 设置媒体流回调函数
     输入参数 :  nHandle                     连接句柄
                cbDataCallBack              媒体流回调函数
                pUser                       用户参数，回调函数中返回
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SetDataCallBack(long nHandle, fDataCallBack cbDataCallBack, 
                                                                  void* pUser);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SendSetupMsg
     功能描述 : 发送RTSP SETUP请求
     输入参数 :  nHandle                    连接句柄
                pstServerAddr               RTSP服务器地址
                sURL                        SETUP消息封装中的URL
                unTimeout                   SETUP消息超时时间（单位：S）
                pstLocalMediaAddr           同步模式下，媒体流本地接收地址；异步模式下为空
     输出参数 : pstPeerMediaAddr            同步模式下，媒体流发送端地址；异步模式下为空
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport)  int RTSPAPI RTSP_CLIENT_SendSetupMsg(long nHandle , const INET_ADDR* pstServerAddr,
                                                                const char *sURL, unsigned int unTimeout, bool bTcpFlag,
                                                                const MEDIA_ADDR* pstLocalMediaAddr, MEDIA_ADDR *pstPeerMediaAddr);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SendPlayMsg
     功能描述 : 发送RTSP PLAY请求
     输入参数 :  nHandle                    连接句柄
                sURL                        PLAY消息封装中的URL
                dScale                      回放/下载播放速率
                sRange                      回放/下载时间段
                unTimeout                   PLAY消息超时时间（单位：S）
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendPlayMsg(long nHandle , const char *sURL, 
                                                              double dScale, const MEDIA_RANGE_S* stRange, 
                                                              unsigned int unTimeout);

    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SendOptionMsg
     功能描述 : 发送RTSP OPTIONS请求
     输入参数 :  nHandle                    连接句柄
                sURL                        OPTIONS消息封装中的URL
                unTimeout                   OPTIONS消息超时时间（单位：S）
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendOptionMsg(long nHandle,const char *sURL,unsigned int unTimeout, char *strMsg, int &iLength);
    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SendPauseMsg
     功能描述 : 发送RTSP PAUSE请求
     输入参数 :  nHandle                    连接句柄
                sURL                        PAUSE消息封装中的URL
                unTimeout                   PAUSE消息超时时间（单位：S）
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendPauseMsg(long nHandle , const char *sURL, 
                                                               unsigned int unTimeout);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_SendTeardownMsg
     功能描述 : 发送RTSP TEARDOWN请求
     输入参数 :  nHandle                     连接句柄
                sURL                        TEARDOWN消息封装中的URL
                unTimeout                   PAUSE消息超时时间（单位：S）
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_SendTeardownMsg(long nHandle , const char *sURL, 
                                                                  unsigned int unTimeout);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_GetStatus
     功能描述 : 获取连接当前状态
     输入参数 : nHandle                     连接句柄
     输出参数 : 无
     返 回 值 : RTSP_SESSION_STATUS -连接状态
    ***************************************************************************/
    __declspec(dllexport) RTSP_SESSION_STATUS RTSPAPI RTSP_CLIENT_GetStatus(long nHandle);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_FreeHandle
     功能描述 : 释放连接句柄
     输入参数 : nHandle                     连接句柄
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_FreeHandle(long nHandle);


    /***************************************************************************
     函 数 名 : RTSP_CLIENT_FreeHandle
     功能描述 : 释放通讯库
     输入参数 : 无
     输出参数 : 无
     返 回 值 : RET_OK-成功     RET_FAIL-失败
    ***************************************************************************/
    __declspec(dllexport) int RTSPAPI RTSP_CLIENT_Cleanup();

    }

#ifdef __cplusplus
}
#endif

#endif //__RTSP_CLIENT_API_H_


