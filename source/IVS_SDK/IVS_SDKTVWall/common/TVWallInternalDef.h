/********************************************************************
	filename	: 	TVWallInternalDef.h
	author		:	
	created		:	2011/10/29	
	description	:	�ṩIVS SDK�в���Ҫ��¶��������ʹ�õ��������ͣ�IVSSDK�ӿ�����Ҫ�õ����������ͣ�
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/29 ��ʼ�汾
*********************************************************************/
#ifndef TV_WALL_INTERNAL_DEF_H
#define TV_WALL_INTERNAL_DEF_H

/*
 *	ɾ��ָ��ĺ궨�� 
 */
#define DELETEP(p)    \
if(NULL != p)         \
{                     \
    delete p;         \
    p = NULL;         \
}                     \

/*
 *	�ж�ָ���Ƿ�Ϊ�� 
 */
#define TVWALL_CHECK_POINTER( pPointer, retValue ) \
if ( NULL == ( pPointer ) )                 \
{                                           \
    IVS_RUN_LOG_ERR( "ָ��ΪNULL");       \
    return retValue;                        \
}                                           \

/*
 *	�ж�ָ���Ƿ�Ϊ��,�޷���ֵ
 */
#define TVWALL_CHECK_POINTER_VOID( pPointer )      \
if ( NULL == ( pPointer ) )                 \
{                                           \
    IVS_RUN_LOG_ERR( "ָ��ΪNULL");       \
    return ;                                \
}                                           \

/*
 *	New����ĺ궨�壬��try catch
 */
#define TVWALL_NEW_TRYCATCH(pPointer, stType, retValue)\
try                                             \
{                                               \
    pPointer = new stType();                    \
}                                               \
catch (...)                                     \
{                                               \
    IVS_RUN_LOG_ERR( "��������ʧ��");         \
    return retValue;                            \
}                                               \

/*
 *	New����ĺ궨�壬��try catch���޷���ֵ
 */
#define TVWALL_NEW_TRYCATCH_VOID(pPointer, stType)     \
try                                             \
{                                               \
    pPointer = new stType();                    \
}                                               \
catch (...)                                     \
{                                               \
    IVS_RUN_LOG_ERR( "��������ʧ��");         \
    return ;                                    \
}                                               \

//��ҳ��ѯͨ������ͷ
#define LOGIN_ID_LEN            64          //�û���¼Ψһ��ʶ����
#define ORG_COD_LEN             10          //��֯���볤��
#define ALARM_NAME_LEN       21          //�澯�����豸���Ƴ���
#define ENABLE_LEN           4           //ʹ�ܳ��ȣ���ON����OFF��
#define  SERVICE_TYPE_LEN 16
#define  CODE_LEN 36
//#define  STREAM_ID_LEN 8
#define  PROTOCOL_LEN 16
#define  IP_LEN 17
#define  PORT_LEN 6
#define  CODE_PROTOCOL_LEN 28
#define  TIME_SPAN_LEN 36
#define  SERVER_IP_LEN 20
#define  SERVER_PORT_LEN 10

/*
 *	ͨ�÷���ֵ�궨��
 */
#define TVWALL_RET_FALSE   -1
#define TVWALL_RET_TRUE    0

/*
 *	ƽ̨������
 */
#define SDK_SOCKET_TIMEOUT       3
#define SDK_SOCKET_INVALID       4
#define SDK_NULL_CALLBACK        5


#endif //_HTTP_DEF_H_