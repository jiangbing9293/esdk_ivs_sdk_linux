/********************************************************************
	filename	: 	NetKeyBoardService.h
	author		:	s00194833
	created		:	2011/12/07	
	description	:	����������ݽ���
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/12/07 ��ʼ�汾
*********************************************************************/
#ifndef NET_KEYBOARD_H
#define NET_KEYBOARD_H

/*!< Socketͷ�ļ����߳�����socketͨѶʹ��*/
#include "SocketService.h"
/*!< �����ꡢ�ṹ��ͷ�ļ�,�ص�����ʹ��*/
#include "KBInternalDef.h"
#include "bp_os.h"
#include "NVSTimer.h"


#ifdef _UT_TEST_
#define  protected public
#define  private public
#endif

//������̴��������
typedef struct stNKBData
{
    unsigned char btKeyValue;                       //��ֵ
    unsigned char btKeyLabel;                       //��ʶ
    unsigned char btReservedFirst;                  //����1
    unsigned char btReservedSecond;                 //����2
    stNKBData()
    {
        btKeyValue = 0;
        btKeyLabel = 0;
        btReservedFirst = 0;
        btReservedSecond = 0;
    }
    ~stNKBData()
    {
        btKeyValue = 0;
        btKeyLabel = 0;
        btReservedFirst = 0;
        btReservedSecond = 0;
    }
}ST_NKB_DATA;

//������̴��������(���ΰҵ)
typedef struct stTDNKBData
{
    unsigned char btKeyValue;                       //��ֵ
    unsigned char btKeyLabel;                       //��ʶ
    unsigned char btReservedFirst;                  //����1(ҡ��:���Ʒ�ʽλ+����������+�����ٶ�+�����ٶ�+�䱶�ٶ�)
    unsigned char btReservedSecond;                 //����2
    unsigned char btReservedThird;                  //����3
    stTDNKBData()
    {
        btKeyValue = 0;
        btKeyLabel = 0;
        btReservedFirst = 0;
        btReservedSecond = 0;
        btReservedThird = 0;
    }
    ~stTDNKBData()
    {
        btKeyValue = 0;
        btKeyLabel = 0;
        btReservedFirst = 0;
        btReservedSecond = 0;
        btReservedThird = 0;
    }
}ST_TD_NKB_DATA;

class CNetKeyBoardService;
class CKBTrigger : public ITrigger
{
public:
	CKBTrigger(void){m_lpCallBack = NULL; m_pNetKeyBoardService = NULL;};
	~CKBTrigger(void){};
	virtual void onTrigger(void *pArg, ULONGLONG ullScales, TriggerStyle enStyle);
	//���õ���ǽ�������
	void SetNetKeyBoardService(CNetKeyBoardService* pNetKeyBoardService, LPCallBack lpCallBack)
	{
		m_lpCallBack = lpCallBack;
		m_pNetKeyBoardService = pNetKeyBoardService;
	};
private:
	LPCallBack m_lpCallBack;                          //�ص�����ָ��
	CNetKeyBoardService* m_pNetKeyBoardService;		//������̶���
};






class CNetKeyBoardService
{
public:
    static CNetKeyBoardService* GetInstance();
    static void ReleaseInstance();

    /**************************************************************************
    * name       : ListenSocket
    * description: �˿ڼ����߳�
    * input      : lpParameter �ص�������ַ
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    static DWORD __stdcall ListenSocket(LPVOID lpParameter);

    /**************************************************************************
    * name       : ListenSocket
    * description: �˿ڼ����߳�
    * input      : lpParameter �ص�������ַ
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    static DWORD __stdcall ListenUDPSocket(LPVOID lpParameter);

    /**************************************************************************
    * name       : ReleaseListenThread
    * description: �ͷż����߳�
    * input      : hRleaseThread �����߳̾��
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    static void ReleaseListenThread(HANDLE hRleaseThread);

    /**************************************************************************
    * name       : IniTcpServer
    * description: ��ʼ��������̵�TCP�����������׽���
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ�,���� - ʧ��
    * remark     : NA
    **************************************************************************/
    int IniTcpServer();

    /**************************************************************************
    * name       : IniUdpServer
    * description: ��ʼ��������̵�UDP�����������׽���
    * input      : NA
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ�,���� - ʧ��
    * remark     : NA
    **************************************************************************/
    int IniUdpServer();

    /**************************************************************************
    * name       : ReleaseListenServer
    * description: �ͷż����׽��֣������������߳�ʧ��ʱʹ��
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void ReleaseListenServer();

	//��CU����ֹͣҡ�˵���Ϣ
	void StopPTZDirectionControlUDP(LPCallBack lpCallBack);

private:

    CNetKeyBoardService();

    ~CNetKeyBoardService();

    /**************************************************************************
    * name       : IndentifyAction
    * description: �б�������̰���
    * input      : pszRecieveData ���յ�����
                   iLength �������ݳ���
                   lpCallBack �ص�����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    int IndentifyAction(char *pszRecieveData,int iLength,LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PraseData
    * description: ������������Ƿ����Э��Ҫ��
    * input      : pszRecieveData ���յ����ݣ�iLength �������ݳ���
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    int PraseData(char *pszRecieveData,int iLength);

    /**************************************************************************
    * name       : DealLogData
    * description: �����¼�����Ϣ
    * input      : pszRecieveData ������̷��͵�������
                   iLength ����������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void DisposeLogOnData(char *pszRecieveData, int iLength);

    /**************************************************************************
    * name       : IsLogin
    * description: �ж��Ƿ�Ϊ��¼��Ϣ
    * input      : NA
    * output     : NA
    * return     : true �ǵ�¼��Ϣ��false ���ǵ�¼��Ϣ
    * remark     : NA
    **************************************************************************/
    bool IsLogin();

    /**************************************************************************
    * name       : IsLogout
    * description: �ж��Ƿ�Ϊע����Ϣ
    * input      : NA
    * output     : NA
    * return     : true ��ע����Ϣ��false ����ע����Ϣ
    * remark     : NA
    **************************************************************************/
    bool IsLogout();

    /**************************************************************************
    * name       : LoginSuccess
    * description: ������̵�¼�ɹ����ݴ���
    * input      : pszRecieveData ������̷��͵�������
                   iLength ����������
    * output     : NA
    * return     : IVS_SUCCEED �������ݳɹ������� ʧ��
    * remark     : NA
    **************************************************************************/
    int LoginSuccess(char *pszRecieveData, int iLength);

    /**************************************************************************
    * name       : LoginFail
    * description: ������̵�¼ʧ�����ݴ���
    * input      : pszRecieveData ������̷��͵�������
                   iLength ����������
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void LoginFail(char *pszRecieveData, int iLength);

    /**************************************************************************
    * name       : IsDirectionControl
    * description: �ж��Ƿ�����̨�������
    * input      : NA
    * output     : NA
    * return     : true ����̨������ƣ�false ������̨�������
    * remark     : NA
    **************************************************************************/
    int IsDirectionControl();
    /**************************************************************************
    * name       : PTZDirectionControl
    * description: ���������̨����������ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZDirectionControl(LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PTZOtherControl
    * description: ���������̨��Ȧ���佹���䱶�������ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZOtherControl(LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PTZVideoExchange
    * description: ���������Ƶ�л����ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZVideoExchange(LPCallBack lpCallBack);

	/**************************************************************************
    * name       : UpdateNum
    * description: ����ͨ��ֵ
    * input      : NA
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void UpdateNum();

	/**************************************************************************
    * name       : PTVideoZCancel
    * description: ȡ��������ǽ
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void PTVideoZCancel(LPCallBack lpCallBack);

    void DealAction(char *pszRecieveData, int iLength, int i, LPCallBack lpCallBack);

    void DealOtherAction();

    void DealPTZDirection(LPCallBack lpCallBack);

    void DealPTZDirectionCallBack(int iCallBackKeyValue, LPCallBack lpCallBack);

    /************************���ΰҵЭ�鱨�Ĵ���*****************************/
    /**************************************************************************
    * name       : IndentifyActionUDP
    * description: �б�������̰���
    * input      : pszRecieveData ���յ�����
                   iLength �������ݳ���
                   lpCallBack �ص�����
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    int IndentifyActionUDP(char *pszRecieveData,int iLength,LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PraseDataUDP
    * description: ������������Ƿ����Э��Ҫ��
    * input      : pszRecieveData ���յ����ݣ�iLength �������ݳ���
    * output     : NA
    * return     : IVS_SUCCEED - �ɹ������� - ʧ��
    * remark     : NA
    **************************************************************************/
    int PraseDataUDP(char *pszRecieveData,int iLength);

    /**************************************************************************
    * name       : PTZOtherControlUDP
    * description: ���������̨��Ȧ���佹���䱶�������ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZOtherControlUDP(LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PTZDirectionControlUDP
    * description: ���������̨����������ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZDirectionControlUDP(LPCallBack lpCallBack);

    /**************************************************************************
    * name       : PTZVideoExchangeUDP
    * description: ���������Ƶ�л����ݴ���
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
    void PTZVideoExchangeUDP(LPCallBack lpCallBack);

	/**************************************************************************
    * name       : PTVideoZCancelUDP
    * description: ȡ��������ǽ
    * input      : lpCallBack �ص�����
    * output     : NA
    * return     : NA
    * remark     : NA
    **************************************************************************/
	void PTVideoZCancelUDP(LPCallBack lpCallBack);

	/**************************************************************************
	* name       : PresetControlUDP
	* description: �������Ԥ��λ�������ݴ���
	* input      : lpCallBack �ص�����
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void PresetControlUDP(LPCallBack lpCallBack);

	/**************************************************************************
	* name       : PresetCruiseControlUDP
	* description: �������Ԥ��λѲ�����ÿ������ݴ���
	* input      : lpCallBack �ص�����
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void PresetCruiseControlUDP(LPCallBack lpCallBack);

	/**************************************************************************
	* name       : PresetCruiseStopControlUDP
	* description: �������Ԥ��λѲ��ֹͣ�������ݴ���
	* input      : lpCallBack �ص�����
	* output     : NA
	* return     : NA
	* remark     : NA
	**************************************************************************/
	void PresetCruiseStopControlUDP(LPCallBack lpCallBack);


    void NoteLastBeatTime();
    time_t GetCurTime();

	void RegisterTimer(LPCallBack lpCallBack);
	void CancelTimer();

public:
    CSocketService m_SocketService;             //������̼���Socket��������
    CSocketService m_UDPSocketService;          //������̼���Socket��������(���ΰҵΪUDPЭ��) 

	CNVSTimer m_timer;                          //�������ҡ�˶�ʱ��
	CKBTrigger m_KBTigger;                      //�������ҡ�˴�����
	bool m_bTimerReg;                           //��ʱ���Ƿ��Ѿ�ע����
	bool m_bTimerRun;                           //��ʱ���Ƿ�������
	
private:
    ST_NKB_DATA m_stNKBData;                    //������̵İ�����Ϣ����
    bool m_bIsListen;                           //������־λ
    bool m_bIsInvalidKey;                       //�ж��Ƿ�Ϊ��Ч����������ֹͣʱ
    int m_iCurrentMaxUser;                      //��ǰ��¼�����Ȩ���û�����������̵�¼ʧ��ʱʹ��
    int m_iVideoLabel;                          //��Ƶ�л���־λ
    int m_iVideoNum;                            //��Ƶͨ����
    int m_iDecodeNum;                           //����ͨ����
    bool m_bTvWallPTZ;                          //������ǽ�����������̨����
    int m_iWinNum;                              //������ǽ�Ĵ����
    int m_iScreenNum;                           //��Ļ�ţ�����ǽ��

    ST_TD_NKB_DATA m_stTDNKBData;               //������̵İ�����Ϣ����(���ΰҵ)
    bool m_bIsTVWall;                           //�Ƿ�ΪӲ��
    int m_iCameraID;                            //�������ţ���Ƶͨ����
    int m_iWinID;                               //����ID��ͨ����

    static CNetKeyBoardService *m_pNetKeyBoardService; //������̾�̬ʵ��

    time_t m_LastBeatTime;                      //��¼�����һ����������ʱ��

    int m_iOldCallBackKeyValue;                 //��һ��������̷�����̨��������
    int m_iOldCallBackTeleWideKeyValue;         //��һ��������̷�����̨�䱶����
    char m_cOldSpeed;                           //��һ����̨�����ٶ�
	char m_cOldTeleWideSpeed;                   //��һ����̨�䱶�ٶ�

	int m_iOldOtherKeyValue;                    //��һ�����������̨��Ȧ���佹���䱶�������ݵ�����
};
#endif //NET_KEYBOARD_H
