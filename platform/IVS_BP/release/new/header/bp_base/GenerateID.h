/********************************************************************
filename    :    GenerateID.h 
author      :    lkf72560
created     :    2012/11/26
description :    �����û���Ψһ��ʾ
copyright   :    Copyright (c) 2012-2016 Huawei Tech.Co.,Ltd 
history     :    2012/11/26 ��ʼ�汾
*********************************************************************/

#ifndef BP_GENERATE_ID_H
#define BP_GENERATE_ID_H

#include <string>
#include "bp_namespace.h"
#include "ace/Truncate.h"
#include "ace/UUID.h"

using namespace std;
NAMESPACE_BP_BEGIN // namespace begin

/**********************************************************************
    * name           : GenerateUUID
    * description    : �����û���Ψһ��ʾ
    * input          : NA
    * return         : string - �û���Ψһ��ʾ
    * remark         : NA   
*********************************************************************/
static string GenerateUUID()
{
    // �����û���Ψһ��ʾ
    ACE_Utils::UUID_Generator uuidout;
    uuidout.init();
    ACE_Utils::UUID uuID;
    uuidout.generate_UUID(uuID);
    string strUUID(uuID.to_string()->c_str ());
    transform(strUUID.begin(), strUUID.end(), strUUID.begin(), ::toupper);
    return strUUID;
}

NAMESPACE_BP_END   // namespace end
#endif
