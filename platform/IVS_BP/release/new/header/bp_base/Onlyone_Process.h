/*************************************************************************
Copyright (C), 1988-2008, Huawei Tech. Co., Ltd.
File Name       : NSS_Onlyone_Process.h
Author          :
Description     : ����Onlyone_Process��
Others          :
Function List   :
History:        : // ��ʷ�޸ļ�¼
<author>    <ID>        <time>      <version>   <desc>
�ܲ�ƽ    ZKF14440     2009-5-12               �޸ĳ�Ա����ע�͡�
*************************************************************************/

#ifndef _Onlyone_Process_h
#define _Onlyone_Process_h

#include "bp_namespace.h"
#include "bp_def.h"

#define FILE_LOCK_TYPE
#define  MAX_FILENAME_LEN 512

NAMESPACE_BP_BEGIN
const int SEM_PRMS = 0644; //�ź�������Ȩ�ޣ�0644�����û�(����)�ɶ�д�����Ա��������Ա�ɶ�����д

/**
 * @class Onlyone_Process
 * @brief ����Ψһ�Լ����
 *        LINUX: �����ļ���ʵ��
 *        WIN32: ��������EVENTʵ��.
 */
class EXPORT_DLL Onlyone_Process
{
private:
    Onlyone_Process(const Onlyone_Process& obj);
    Onlyone_Process& operator =(const Onlyone_Process& obj);
protected:
    Onlyone_Process();
    ~Onlyone_Process();
public:
    /**
     * �������Ƿ�����.
     * �Ѿ����з���false, ���򷵻�true.
     */
    static bool onlyone(const char *strFileName);

#ifndef WIN32
protected:    
    bool  lockfile(const char *strcfgfile);
#endif  //WIN32
};
NAMESPACE_BP_END

#endif //_NSS_Onlyone_Process_h
