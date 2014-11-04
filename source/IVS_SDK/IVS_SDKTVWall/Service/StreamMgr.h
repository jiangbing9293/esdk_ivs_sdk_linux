/********************************************************************
 filename    :    StreamMgr.h
 author      :    g00209332
 created     :    2012/12/19
 description :    ͨ�Ź�����
 copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd
 history     :    2012/12/19 ��C30�̳�
*********************************************************************/

#ifndef STREAM_MGR_H
#define STREAM_MGR_H

/*!< Զ�̽���������ͷ�ļ� */
#include "RemoteDecoderMgr.h"

//StreamMgr�ӿ�ʵ����
class CStreamMgr
{
public:

    CStreamMgr();

    virtual ~CStreamMgr();

    /**************************************************************************
    * name       : InitStreamMgr
    * description: ��ʼ��StreamMgr
    * input      : pFunNotifyCallBack  �ص�����ָ��
                   pCallBackUserData   �ص������û�����ָ��
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          InitStreamMgr
    (   IN PCUMW_CALLBACK_NOTIFY   pFunNotifyCallBack,
        IN void*                   pCallBackUserData);

    /**************************************************************************
    * name       : ReleaseStreamMgr
    * description: �ͷ�StreamMgr
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          ReleaseStreamMgr();

    /**************************************************************************
    * name       : InitBusiness
    * description: ��ʼ��ҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          InitBusiness
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : StartBusiness
    * description: ����ҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          StartBusiness
    (   IN CBusiness& objBusiness);

    /**************************************************************************
    * name       : ControlBusiness
    * description: ����ҵ��
    * input      : objBusiness ҵ���������
                   stCtrlPara  ���Ʋ���
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          ControlBusiness
    (   IO CBusiness&                  objBusiness,
        IN CUMW::BUSINESS_CTRL_PARA&   stCtrlPara);

    /**************************************************************************
    * name       : StopBusiness
    * description: ֹͣҵ��
    * input      : objBusiness ҵ���������
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          StopBusiness
    (   IN const CBusiness& objBusiness);

    /**************************************************************************
    * name       : StartRemoteDecoderMgr
    * description: ����Զ�̽���������
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          StartRemoteDecoderMgr();

    /**************************************************************************
    * name       : StopRemoteDecoderMgr
    * description: ֹͣԶ�̽���������
    * input      : NA
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    virtual IVS_INT32          StopRemoteDecoderMgr();

private:

    /**************************************************************************
    * name       : HandleNotifyCallBack
    * description: ����֪ͨ�ص�
    * input      : pstNotifyInfo   ֪ͨ��Ϣָ��
                   pUserData       �û�����ָ��
    * output     : NA
    * return     : long ������
    * remark     : NA
    **************************************************************************/
    static IVS_INT32 __stdcall HandleNotifyCallBack
    (   IN CUMW_NOTIFY_INFO* pstNotifyInfo,
        IN void*             pUserData);

private:

    //֪ͨ�ص�����״̬��
    CStatusNotifyCallBack m_objStatusNotifyCallBack;

    //Զ�̽�����
    CRemoteDecoderMgr* m_pRemoteDecoderMgr;
};

#endif //STREAM_MGR_H
