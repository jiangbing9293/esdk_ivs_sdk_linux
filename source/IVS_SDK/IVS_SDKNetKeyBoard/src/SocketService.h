/********************************************************************
	filename	: 	SocketService.h
	author		:	s00194833
	created		:	2011/12/07	
	description	:	SocketͨѶ��
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/12/07 ��ʼ�汾
*********************************************************************/
#ifndef SOCKETSERVICE_H
#define SOCKETSERVICE_H

/*!< ϵͳԤ����ͷ*/
#include <WinSock2.h>

class CSocketService
{
public:
    CSocketService();
    ~CSocketService();

    /**************************************************************************
    * name       : InitSock
    * description: ��ʼ��SocketͨѶ��
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED �ɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    static int InitSock();

    /**************************************************************************
    * name       : FreeSock
    * description: �ͷ�SocketͨѶ����Դ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    static void FreeSock();

    /**************************************************************************
    * name       : CreateSocket
    * description: �������ֽ�
    * input      : iAF��ַ�� ֻ֧��AF_INET
                   iType Socket���� ֧��SOCK_STREAM��SOCK_DGRAM
                   iRrotocol Э�鷽�� IPPROTO_TCP��IPPROTO_UDP
    * output     : NA
    * return     : IVS_SUCCEED �ɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    int CreateSocket(int iAF =  AF_INET,int iType = SOCK_STREAM,int iProtocol = IPPROTO_TCP);

    /**************************************************************************
    * name       : SetServerAddr
    * description: ���÷�����IP�Ͷ˿ں�
    * input      : iPort �������˿ں�
                   pszIP ������IP
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void SetServerAddr(u_short iPort,const char *pszIP = NULL);

    int Connect(int iTimeout);
    /**************************************************************************
    * name       : Listen
    * description: �����˿�
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED �ɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    int Listen();
    /**************************************************************************
    * name       : UDPListen
    * description: �����˿�
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED �ɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    int UDPListen();

    /**************************************************************************
    * name       : AcceptClientRequst
    * description: �ȴ��ͻ�����
    * input      : iTimeout ���ӳ�ʱʱ�� ��λs
    * output     : NA
    * return     : IVS_SUCCEED �ɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    int AcceptClientRequst(int iTimeout = 4);

    /**************************************************************************
    * name       : SendData
    * description: �������� 
    * input      : pszSendData �������ݻ�����
                   iLength �������ݳ���
                   iTimeout ��ʱʱ��
    * output     : NA
    * return     : totalSendBytes  �ѷ��͵����ݳ���
    * remark     : NA
    **************************************************************************/
    int Send(const char *pszSendData,int iLength,int iTimeout);

    /**************************************************************************
    * name       : ReceiveData
    * description: ��������
    * input      : iTimeout ��ʱʱ��
    * output     : pszReceiveData �������ݻ�����
                   iLength �������ݻ���������
    * return     : iReceiveLength ʵ�ʽ������ݳ���
    * remark     : NA
    **************************************************************************/
    int Receive(char *pszReceiveData,u_int iLength,int iTimeout);

    /**************************************************************************
    * name       : ReceiveData(UDP)
    * description: ��������
    * input      : iTimeout ��ʱʱ��
    * output     : pszReceiveData �������ݻ�����
                   iLength �������ݻ���������
    * return     : iReceiveLength ʵ�ʽ������ݳ���
    * remark     : NA
    **************************************************************************/
    int UDPReceive(char *pszReceiveData,u_int iLength, int iTimeout);

    /**************************************************************************
    * name       : CloseConnectSocket
    * description: �ر��������ֽ�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void CloseConnectSocket();

    /**************************************************************************
    * name       : CloseSocket
    * description: �ر����ֽ�
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void CloseSocket();
private:

    SOCKET m_hSocket;               //���������ֽ�
    SOCKET m_hConnectSocket;              //�������ֽ�


    SOCKADDR_IN m_ServerAddr;       //�������˵�ַ
    SOCKADDR_IN m_ClientAddr;       //�ͻ��˵�ַ

    static bool m_bInitLabel;       //��־ͨѶ���Ƿ��ʼ��
    static int m_iInUse;            //ͳ���ж����û�ʹ�ø�ͨѶ��
};
#endif //SOCKETSERVICE_H
