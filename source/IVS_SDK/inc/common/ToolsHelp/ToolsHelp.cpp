/********************************************************************
	filename	: 	ToolsHelp.cpp
	author		:	s00191067
	created		:	2011/10/29	
	description	:	�ṩ����ģ�鹫�������Ĺ����ļ�
	copyright	:	Copyright (C) 2011-2015
	history		:	2011/10/29 ��ʼ�汾
*********************************************************************/

//#include "ivs_error.h"

/*!< ����ͷ�ļ� */
#include "ToolsHelp.h"
#include "vos_config.h"
#include "nss_mt.h"
#include <stdarg.h>
#ifdef WIN32
	#include <time.h> 
	#include <io.h>
#else
	#include <sys/time.h>
	#include <sys/io.h>
	#include <unistd.h> 
	#include <iconv.h>
#endif
#include "SDKDef.h"

/*!< �궨�� */
//#define BUF_SIZE				4096
const double EPSINON = 0.00001;  // doubleֵ����Χ;

struct SEND_SPEED
{
    const float fUserScale; // �û�Ҫ����ٶ�
    const float fSendScale; // ���͸���������ʵ���ٶ�
};

// ���������ı�
static SEND_SPEED s_SpeedNames[] =
{
    /*01*/	{ 1,            2},	// �û�1����ȫ����ʵ��2����ȫ��
    /*02*/	{ 2,            4},	// �û�2����ȫ����ʵ��4����ȫ��
    /*03*/	{ 4,			4}, // �û�4����ȫ����ʵ��8����ȫ��ǰ��
    /*04*/	{ 8,           16},	// �û�8����ȫ����ʵ��16����ֻ��I֡
    /*05*/	{ 16,          32},	// �û�16����ȫ����ʵ��32����ֻ��I֡
    /*06*/	{ -1,         253},	// �û�1����ȫ�����ˣ�ʵ��2����ȫ������
    /*07*/	{ -2,          -4},	// �û�2����ֻ��I֡���ˣ�ʵ��4����ֻ��I֡
    /*08*/	{ -4,          -8},	// �û�4����ֻ��I֡���ˣ�ʵ��8����ֻ��I֡
    /*09*/	{ -8,         -16},	// �û�8����ֻ��I֡���ˣ�ʵ��16����ֻ��I֡
    /*10*/	{ -16,        -32},	// �û�16����ֻ��I֡���ˣ�ʵ��32����ֻ��I֡
    /*11*/	{ 0.5,          1},	// �û�1/2����ȫ����ʵ��1����ȫ��
    /*12*/	{ 0.25,			1},	// �û�1/4����ȫ����ʵ��1����ȫ��
    /*13*/	{ 0.125,		1},	// �û�1/8����ȫ����ʵ��1����ȫ��
    /*14*/	{ 0.0625,		1},	// �û�1/16����ȫ����ʵ��1����ȫ��
    /*15*/	{ 0.03125,		1},	// �û�1/32����ȫ����ʵ��1����ȫ��
    /*16*/	{ -0.5,       252},	// �û�1/2����ȫ�����ˣ�ʵ��1����ȫ������
    /*17*/	{ -0.25,      252},	// �û�1/4����ȫ�����ˣ�ʵ��1����ȫ������
};

//������ת��Ϊ�ַ���
std::string CToolsHelp::Int2Str ( int iValue, bool bHex /*= false*/  )
{
    char szTmp[STR_LENGTH_SIZE];
#if VOS_APP_OS == VOS_OS_WIN32
	if ( bHex )
    {
        (void)_itoa_s( iValue, szTmp, INT_FORMAT_X );
    }
    else
    {
        (void)_itoa_s( iValue, szTmp, INT_FORMAT_D );
    }
#elif VOS_APP_OS == VOS_OS_LINUX //modi by zkf71716 at 2012-8-17
	if(bHex)
	{
		(void)sprintf(szTmp,"%x",iValue);
	}
	else
	{
		if(0 > ItoaEX(iValue,szTmp))
		{
			return "";
		}
	}
#endif
    return std::string( szTmp );
}

std::string CToolsHelp::Long2Str ( long lValue, bool bHex)
{
	char szTmp[STR_LENGTH_SIZE];
#if VOS_APP_OS == VOS_OS_WIN32
	if ( bHex )
	{
		(void)_ltoa_s( lValue, szTmp, INT_FORMAT_X );
	}
	else
	{
		(void)_ltoa_s( lValue, szTmp, INT_FORMAT_D );
	}
#elif VOS_APP_OS == VOS_OS_LINUX //modi by zkf71716 at 2012-8-17
	if(bHex)
	{
		(void)sprintf(szTmp,"%x",lValue);
	}
	else
	{
		if(0 > ItoaEX(lValue,szTmp))
		{
			return "";
		}
	}
#endif
	return std::string( szTmp );
}

 int CToolsHelp::ItoaEX(int iValue, char *buff )//add by zkf71716 at 2012-8-17
 {
	 const unsigned int radix = 10;
	 char* p;
	 unsigned int a;        //every digit
	 char* b;             //start of the digit char
	 char temp;
	 unsigned int u;
	 if(NULL == buff)  //lint !e830
	 {
		return -1;
	 }
	 p = buff;

	 if (iValue < 0)
	 {
		 *p++ = '-';
		 iValue = 0 - iValue;
	 }
	 u = (unsigned int)iValue;

	 b = p;
	 do
	 {
		 a = u % radix;
		 u /= radix;
		 *p++ = a + '0';//lint !e734
	 } while (u > 0);

     int len = 0;
	 len = (int)(p - buff);
	 *p-- = 0;
	 //swap
	 do
	 {
		 temp = *p;
		 *p = *b;
		 *b = temp;
		 --p;
		 ++b;

	 } while (b < p);

	 return len;
 }

// ������ת��Ϊ�ַ���
int CToolsHelp::StrToInt( const std::string& strSrc, bool bHex /*= false*/ )
{
     int iRet = 0;	

     sscanf( strSrc.c_str(), bHex ? STR_FORMAT_X : STR_FORMAT_D, &iRet);

     return iRet;
}

std::string CToolsHelp::GetSCCHead()
{
	return "";
}
int CToolsHelp::ResultCodeToErrorCode( int iResultCode )
{
	//�����������ת�ɸ���
	if (iResultCode > 0)
	{
		int iErrorCode = ~iResultCode;//lint !e502
		iErrorCode = iErrorCode + 1;

		return iErrorCode;
	}

	return iResultCode;
}

bool CToolsHelp::IsNum( const std::string &str )
{
    std::string::const_iterator it = str.begin();
    for (; it != str.end(); ++it)
    {
        if(!isdigit(*it))
        {
            return false;
        }
    }
    return true;
}

/**************************************************************************
* name       : TranslateToSer
* description: �����������֮������ݷ���
* input      : iNum		���������
*			   pscAgree	���϶��ʵ��ַ���
			   pscDisAgree ���񶨴ʵ��ַ���
* output     : NA
* return     : std::string
* remark     : NA
**************************************************************************/
const char*  CToolsHelp::TranslateToSer(const char *pscAgree, const char *pscDisAgree, int &iNum)
{
		if( 0 > iNum || iNum > 1)
		{
			return "";
		}
		if(iNum)
		{
			return pscAgree;
		}
		else
		{
			return pscDisAgree;
		}
}//lint !e1764

/**************************************************************************
* name       : TranslateToSer
* description: �����������֮������ݷ���
* input      : pscTemp		������Ĵ�
	output     : iNum ��������
* return     : �ɹ����ط�����������
* remark     : NA
**************************************************************************/
int CToolsHelp::TranslateFromSer(const char *pscTemp, int &iNum)
{
	if(NULL == pscTemp)
	{
		return 0;
	}
	if(!strcmp("ON",pscTemp))
	{
		iNum = 1;
	}
	if(!strcmp("OFF",pscTemp))
	{
		iNum = 0;
	}
	return 0;
}

int CToolsHelp::GetHostName(char* pHostName)
{
	if (NULL == pHostName)
	{
		return 1;
	}
#ifdef WIN32
    //��ʼ��:�������ʼ�������´��뽫�޷�ִ��
    WSAData data;
    if(WSAStartup(MAKEWORD(1,1),&data)!=0)
    {
        return 1;
    }

	char host[255] = {0};
    //��ȡ������:Ҳ����ʹ��GetComputerName()�������
    if(gethostname(host,sizeof(host))==SOCKET_ERROR)
    {
        WSACleanup(); 
        return 1;
    }
	if (!Memcpy(pHostName, 255, host, strlen(host)))
	{
		return 1;
	}
	
    WSACleanup();      
#else
	gethostname(pHostName,IVS_MACHINE_NAME_LEN);

#endif
    return 0;    
}

// ��ȡ�������ǰ��¼�û���
int CToolsHelp::GetCurrentUserName(char* pCurrentUserName)
{
	if (NULL == pCurrentUserName)
	{
		return 1;
	}
#ifdef WIN32
	DWORD size= 255;  // ���ʼ��ֵ
	#ifdef UNICODE
	    wchar_t name[255] = {0};
	#else
	    char name[255] = {0};
	#endif // !UNICODE
	GetUserName(name,&size);
	if (!Memcpy(pCurrentUserName, 255,name, size))
	{
		return 1;
	}
#else
	pCurrentUserName = getlogin();

#endif
	return 0;
}

// ��ȡ��������Ŀ¼�ĸ�Ŀ¼
int CToolsHelp::get_exe_parent_dir(char *buf)
{
#ifdef WIN32
    const char separator = '\\';
#else
    const char separator = '/';
#endif

    //ȥ��������
    for(int i = 0; i < 2; ++i) //ѭ��2��: ��1���ҵ���������Ŀ¼����2���ҵ���������Ŀ¼�ĸ�Ŀ¼
    {
        char *pch = strrchr(buf, separator);
        if (NULL == pch)
        {
            printf("Program no directory.\n");
            return IVS_FAIL;
        }
    
        if (pch == buf)
        {
            printf("Program in root directory.\n");
            return IVS_FAIL;
        }

        *pch = '\0';
        //pch = NULL;
    }
    return IVS_SUCCEED;
}

// ��ȡ��Ŀ¼
int CToolsHelp::get_home_directory(std::string& dir)
{
	int nRet;
	char buf[1024] = {0};
#ifdef WIN32
	    //int nRet = ::GetCurrentDirectoryA(sizeof(buf) - 1, buf);
	    //printf("GetCurrentDirectory: %s\n", buf);

        nRet = GetModuleFileNameA(NULL, buf, sizeof(buf) - 1);//lint !e713
	    if (0 == nRet)
	    {
	        printf("Can't get directory in which process is.\n");
	        return IVS_FAIL;
	    }
#else
		/*
		linuxͨ��������readlink("/proc/pid/exe", buf, buf_size)����ȡӦ�ó���ľ���·��������������
		*/
		readlink("/proc/self/exe", buf, sizeof(buf) - 1);
#endif

        nRet = get_exe_parent_dir(buf);
		if (IVS_SUCCEED != nRet)
		{
			return nRet;
		}

		dir = buf;

	return IVS_SUCCEED;
}

#ifndef WIN32
/*
 * GetModuleFilePath
 * ��ȡָ�����ļ�����Ŀ¼���ϼ�Ŀ¼
 * sModuleName [in] ���ļ����ƣ���"libIVS_Player.so"
 * sPath [out] ����Ŀ¼
 * unSize [int] sPath�ĳ���
 * added by zhangrui/233055
 */
int CToolsHelp::GetModuleFilePath(const char* sModuleName, char* sPath, unsigned int unSize)
{
	int iRet = IVS_FAIL;
	char sLine[1024] = {0};
	FILE *fp = NULL;
	char *pTmpFullDir = NULL;
	char *pTmpModuleDir = NULL;

	fp = fopen ("/proc/self/maps", "r");
	if (NULL == fp)
	{
		return iRet;
	}

	while (0 == feof(fp))
	{
		if (NULL == fgets(sLine, sizeof(sLine), fp))
		{
			continue;
		}

		pTmpFullDir = strchr(sLine, '/');
		if (NULL == strstr(sLine, "r-xp") ||  NULL == pTmpFullDir || NULL == strstr(sLine, sModuleName))//r-xp:˽��ֻ����ִ������
		{
			continue;
		}

		//������ҵ�ģ��ֱ�ӷ��ڸ�Ŀ¼�£�����ʧ��
		pTmpModuleDir = strrchr(pTmpFullDir, '/');            
		if (pTmpModuleDir == pTmpFullDir)
		{
			break;
		}

		*pTmpModuleDir = '\0';
		//���ҵ�ģ��������libĿ¼�£���lib/libXXX.so
		pTmpModuleDir = strrchr(pTmpFullDir, '/'); 
		if (pTmpModuleDir == pTmpFullDir)
		{
			iRet = IVS_SUCCEED;
			strcpy(sPath, "/");
			break;
		}

		*pTmpModuleDir = '\0';            

		if (strlen(pTmpFullDir) >= unSize)
		{
			break;
		}

		iRet = IVS_SUCCEED;
		strncpy(sPath, pTmpFullDir, strlen(pTmpFullDir) + 1);
		break;
	}

	return iRet;

}
#endif

// ��ȡconfigĿ¼
int CToolsHelp::get_config_directory(std::string& dir)
{
    int nRet;
	char buf[1024] = {0};
#ifdef WIN32
    const char separator = '\\';
    nRet = GetModuleFileNameA(NULL, buf, sizeof(buf) - 1);//lint !e713
	if (0 == nRet)
	{
	    printf("Can't get directory in which process is.\n");
	    return IVS_FAIL;
	}
#else
	const char separator = '/';
	nRet = GetModuleFilePath("libIVS_SDK.so", buf, sizeof(buf));
	if (IVS_SUCCEED != nRet)
	{
		BP_RUN_LOG_ERR(nRet,"GetModuleFileName Failed ", "failed.");
		return nRet;
	}
	strcat(buf, "/");//Ĭ��ĩβ����'/'
#endif
    //ȥ��������
    char *pch = strrchr(buf, separator);
    if (NULL == pch)
    {
        printf("Program no directory.\n");
        return IVS_FAIL;
    }
    if (pch == buf)
    {
        printf("Program in root directory.\n");
        return IVS_FAIL;
    }
    *pch = '\0';
	dir = buf;

#ifdef WIN32
    dir += "\\config";
#else
    dir += "/config";
#endif
    return IVS_SUCCEED;
}

void CToolsHelp::LTrim(std::string& strSrc )
{
	if ( !strSrc.empty() )
	{
		std::string::const_iterator iter = strSrc.begin();
		do {} while(strSrc.end() != iter && isspace(*(iter++)));

		size_t len = (static_cast<unsigned int>(iter - strSrc.begin()) - 1);
		if ( iter == strSrc.end() && isspace(*(--iter)) )
		{
			len = std::string::npos;
		}

		// ȥ��ǰ�ո��
		strSrc.erase(0, len);
	}
}

void CToolsHelp::RTrim(std::string& strSrc )
{
	if ( !strSrc.empty() )
	{
		std::string::reverse_iterator iter = strSrc.rbegin();
		do {} while(strSrc.rend() != iter && isspace(*(iter++)));

		size_t pos = (static_cast<unsigned int>(strSrc.rend() - iter) + 1);
		if ( iter == strSrc.rend() && isspace(*(--iter)) )
		{
			pos = 0;
		}

		// ȥ����ո��
		strSrc.erase(pos);
	}
}

std::string CToolsHelp::Trim(std::string& strSrc )
{
	LTrim(strSrc);
	RTrim(strSrc);
	return strSrc;
}
#ifdef WIN32
wchar_t* CToolsHelp::ANSIToUnicode(const char* str)
{
	unsigned int textlen = 0;
	wchar_t * result = NULL;
	textlen = static_cast<IVS_UINT32>(MultiByteToWideChar( CONV_CODE_PAGE, 0, str,-1, NULL,0 ));
	try
	{
		result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));
        if (NULL == result)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
            return NULL;
        }
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
		//result = NULL;
		return NULL;
	}
	
	memset(result,0,(textlen+1)*sizeof(wchar_t));
	MultiByteToWideChar(CONV_CODE_PAGE, 0,str,-1,(LPWSTR)result,textlen );//lint !e713
	return result;
}

char* CToolsHelp::UnicodeToUTF8( const wchar_t* str )
{
	char* result = NULL;
	unsigned int textlen = 0;
	textlen = static_cast<IVS_UINT32>(WideCharToMultiByte( CP_UTF8, 0, str, -1, NULL, 0, NULL, NULL ));
	try
	{
		result =(char *)malloc((textlen+1)*sizeof(char));
        if (NULL == result)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
            return NULL;
        }
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
		//result = NULL;
		return NULL;
	}
	
	memset(result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CP_UTF8, 0, str, -1, result, textlen, NULL, NULL );//lint !e713
	return result;
}

char* CToolsHelp::UnicodeToANSI( const wchar_t* str )
{
	char* result = NULL;
	unsigned int textlen = 0;
	textlen = static_cast<IVS_UINT32>(WideCharToMultiByte( CONV_CODE_PAGE, 0, str, -1, NULL, 0, NULL, NULL ));
	try
	{
		result =(char *)malloc((textlen+1)*sizeof(char));
        if (NULL == result)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
            return NULL;
        }
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
		//result = NULL;
		return NULL;
	}
	
	memset( result, 0, sizeof(char) * ( textlen + 1 ) );
	WideCharToMultiByte( CONV_CODE_PAGE, 0, str, -1, result, textlen, NULL, NULL );//lint !e713
	return result;
}

wchar_t* CToolsHelp::UTF8ToUnicode( const char* str )
{
	unsigned int textlen = 0;
	wchar_t * result = NULL;
	textlen = static_cast<IVS_UINT32>(MultiByteToWideChar( CP_UTF8, 0, str,-1, NULL,0 ));
	try
	{
		result = (wchar_t *)malloc((textlen+1)*sizeof(wchar_t));
        if (NULL == result)
        {
            BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
            return NULL;
        }
	}
	catch (...)
	{
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "memory is not enough", "NA");
		//result = NULL;
		return NULL;
	}
	
	memset(result,0,(textlen+1)*sizeof(wchar_t));
	MultiByteToWideChar(CP_UTF8, 0,str,-1,(LPWSTR)result,textlen );//lint !e713
	return result;
}
#endif
char* CToolsHelp::UTF8ToANSI(const char* str)
{
#ifdef WIN32
	wchar_t* unicode = UTF8ToUnicode(str);
	if (NULL == unicode)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "UTF8 To Unicode error", "NA");
		return NULL;
	}
	char* ansi = UnicodeToANSI(unicode);
	free(unicode);
	unicode = NULL;
	return ansi;
#else
	char* pout = NULL;
	if (NULL == str)
	{
		pout = (char*)malloc(sizeof(char));
		if (NULL == pout)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "UTF8ToANSI", "UTF8ToANSI str is NULL and malloc failed");
			return NULL;
		}
		memset(pout, 0, sizeof(char));
		BP_RUN_LOG_INF("UTF8ToANSI", "UTF8ToANSI str is NULL");
		return pout;
	}
	BP_RUN_LOG_INF("UTF8ToANSI", "UTF8ToANSI str = %s", str);
	int outlen = strlen(str);
	pout = (char*)malloc(sizeof(char)*(outlen+1));
	if (NULL == pout)
	{
		pout = (char*)malloc(sizeof(char));
		if (NULL == pout)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "UTF8ToANSI", "malloc failed");
			return NULL;
		}
		memset(pout, 0, sizeof(char));
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "UTF8ToANSI", "malloc failed");
		return pout;
	}
	strcpy(pout,str);
	*(pout+outlen)=0;
	return pout;

#endif
}

char* CToolsHelp::ANSIToUTF8(const char* str)
{

#ifdef WIN32
	wchar_t* unicode = ANSIToUnicode(str);
	if (NULL == unicode)
	{
		BP_RUN_LOG_ERR(IVS_FAIL, "ANSI To Unicode error", "NA");
		return NULL;
	}
	char* utf8 = UnicodeToUTF8(unicode);
	free(unicode);
	unicode = NULL;
	return utf8;
#else
	char* pout = NULL;
	if (NULL == str)
	{
		pout = (char*)malloc(sizeof(char));
		if (NULL == pout)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "ANSIToUTF8", "ANSIToUTF8 str is NULL and malloc failed");
			return NULL;
		}
		memset(pout, 0, sizeof(char));
		BP_RUN_LOG_INF("ANSIToUTF8", "ANSIToUTF8 str is NULL");
		return pout;
	}
	BP_RUN_LOG_INF("ANSIToUTF8", "ANSIToUTF8 str = %s", str);
	int outlen = strlen(str);
	pout = (char*)malloc(sizeof(char)*(outlen+1));
	if (NULL == pout)
	{
		pout = (char*)malloc(sizeof(char));
		if (NULL == pout)
		{
			BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "ANSIToUTF8", "malloc failed");
			return NULL;
		}
		memset(pout, 0, sizeof(char));
		BP_RUN_LOG_ERR(IVS_ALLOC_MEMORY_ERROR, "ANSIToUTF8", "malloc failed");
		return pout;
	}
	strcpy(pout,str);
	*(pout+outlen)=0;
	return pout;
#endif
}

int CToolsHelp::SplitCameraDomainCode(const std::string& strCameraDomainCode, 
                                      std::string& strCameraCode, 
                                      std::string& strDomainCode)
{
    if (strCameraDomainCode.empty())
    {
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"CToolsHelp", "strCameraDomainCode is empty.");
        return IVS_PARA_INVALID;
    }

    unsigned int uiPos = strCameraDomainCode.rfind("#");
    if (std::string::npos == uiPos || 0 == uiPos)
    {
        strCameraCode = strCameraDomainCode;// ����û��#ƴװ�����;
        BP_RUN_LOG_ERR(IVS_PARA_INVALID,"CToolsHelp", "strCameraDomainCode(%s) invalid.", strCameraDomainCode.c_str());
        return IVS_SUCCEED;
    }

    strCameraCode = strCameraDomainCode.substr(0, uiPos);

    if (uiPos + 1 >= strCameraDomainCode.length())
    {
        BP_RUN_LOG_ERR(IVS_SUCCEED,"CToolsHelp", "SplitCameraDomainCode do not have DomainCode.");
        return IVS_SUCCEED;// ���ݲ��ֲ��������������;
    }

    strDomainCode = strCameraDomainCode.substr(uiPos + 1, strCameraDomainCode.length());

    return IVS_SUCCEED;
}

// ������Ҫ�ض������������
unsigned short m_needRedirectReqIDs[7] = {NSS_GET_URL_REQ,NSS_GET_RECORD_LIST_REQ,NSS_PTZ_CONTROL_REQ,
											NSS_CLUSTER_STAT_CAMERA_TRACE_REQ,NSS_CLUSTER_GET_CAMERA_TRACE_REQ,
											NSS_GET_VOICE_INFO_REQ,NSS_GET_CAMERA_CFG_REQ};

bool CToolsHelp::IsNeedReDirect(unsigned short usReqID)
{
    bool bRet = false;
 
	// ����ӳ������洢��Ҫ��������󡣲ο�MAU
	int iSize = sizeof(m_needRedirectReqIDs) / sizeof(unsigned short);
	for ( int i = 0; i < iSize; i++ )
	{
		if ( m_needRedirectReqIDs[i] == usReqID )
		{
			bRet = true;
			break;
		}
	}

    return bRet;
}

TiXmlElement* CToolsHelp::FindElementByPath(TiXmlNode* pParentEle,std::string strPath)
{
	TiXmlNode* pChildElem = NULL;
	if(NULL != pParentEle)
	{
		std::string tmpPath = CToolsHelp::Trim(strPath);

		// ���ҷָ���
		std::string::size_type siPosIndex = tmpPath.find('/');

		// ����ҵ��ĵ�һ���ַ����� / ������������Ѱ����һ��
		while (0 == siPosIndex)
		{
			tmpPath = tmpPath.substr(1);
			tmpPath = CToolsHelp::Trim(tmpPath);
			siPosIndex = tmpPath.find('/');
		}
		// ����ǿյ��򷵻ر���
		if (tmpPath.empty())
		{
			pChildElem = pParentEle;
		}
		else if(std::string::npos == siPosIndex )
		{
			pChildElem = pParentEle->FirstChildElement(tmpPath.c_str());
		}
		else
		{
			std::string strElemName = tmpPath.substr(0,siPosIndex);
			std::string strChildPath = tmpPath.substr(siPosIndex + 1);

			strChildPath = CToolsHelp::Trim(strChildPath);

			pChildElem = pParentEle->FirstChildElement(strElemName.c_str());
			if(NULL != pChildElem && !strChildPath.empty())
			{
				pChildElem = CToolsHelp::FindElementByPath(pChildElem,strChildPath); //lint !e1036
			}
		}
	}
	return (NULL == pChildElem) ?NULL: pChildElem->ToElement();
}

bool CToolsHelp::IsSpeedVaild(float fScale)
{
    bool bRet = false;

    float fScaleAbs = abs(fScale);

    if ((fScaleAbs - 32 >= - EPSINON) && (fScaleAbs - 32 <= EPSINON))
    {
        bRet = true;
    }
    else if ((fScaleAbs - 16 >= - EPSINON) && (fScaleAbs - 16 <= EPSINON))
    {
        bRet = true;
    }
    else if ((fScaleAbs - 8 >= - EPSINON) && (fScaleAbs - 8 <= EPSINON))
    {
        bRet = true;
    }
    else if ((fScaleAbs - 4 >= - EPSINON) && (fScaleAbs - 4 <= EPSINON))
    {
        bRet = true;
    }
    else if ((fScaleAbs - 2 >= - EPSINON) && (fScaleAbs - 2 <= EPSINON))
    {
        bRet = true;
    }
    else if ((fScaleAbs - 1 >= - EPSINON) && (fScaleAbs - 1 <= EPSINON))
    {
        bRet = true;
    }
	else if ((fScaleAbs - 0.5 >= - EPSINON) && (fScaleAbs - 0.5 <= EPSINON))
	{
		bRet = true;
	}
	else if ((fScaleAbs - 0.25 >= - EPSINON) && (fScaleAbs - 0.25 <= EPSINON))
	{
		bRet = true;
	}
	else if ((fScaleAbs - 0.125 >= - EPSINON) && (fScaleAbs - 0.125 <= EPSINON))
	{
		bRet = true;
	}
	else if ((fScaleAbs - 0.0625 >= - EPSINON) && (fScaleAbs - 0.0625 <= EPSINON))
	{
		bRet = true;
	}
	else if ((fScaleAbs - 0.03125 >= - EPSINON) && (fScaleAbs - 0.03125 <= EPSINON))
	{
		bRet = true;
	}

    return bRet;
}

// ���û������ٶ�ת��Ϊ����������͵Ĳ����ٶ�;
bool CToolsHelp::GetSendScale(IVS_SERVICE_TYPE enServiceType, float fUserScale, float& fSendScale)
{
#ifndef _MSC_VER  // linux�����£��ṩ�������̵��ã�ȥ������
    return true;
#endif 

    bool bRet = false;
    int iSize = sizeof(s_SpeedNames) / sizeof(SEND_SPEED);

    for ( int i = 0; i < iSize; i++ )
    {
        if ( s_SpeedNames[i].fUserScale == fUserScale )
        {
            fSendScale = s_SpeedNames[i].fSendScale;
            bRet = true;
            break;
        }
    }

	// ���ǰ�˻طź�ǰ�����������⴦��
	// ǰ�˻طź�ǰ������4���ٿ���ʱ����4���ٷ���
	unsigned int uiUserScale = (unsigned int)fUserScale;
	if ((SERVICE_TYPE_PU_PLAYBACK == enServiceType || SERVICE_TYPE_PU_DOWNLOAD == enServiceType)
		&& 4 == uiUserScale)
	{
		fSendScale = 4;
	}

    return bRet;
}

// ��װ���Memcpy����
bool CToolsHelp::Memcpy(void* pDst,unsigned int uiMaxLen,const void* pSrc, unsigned int uiCopyLen)
{
    bool bRet = false;
    if (NULL == pDst)
    {
        return bRet;
    }
    
    if (uiCopyLen > uiMaxLen)
    {
        return bRet;
    }
    else
    {
        memcpy(pDst, pSrc, uiCopyLen);
        bRet = true;
    }
    return bRet;
}

// ��װ���strncpy����,�����ɹ����Զ����ַ�����β���'\0��
bool CToolsHelp::Strncpy(char* pDst, unsigned int uiMaxLen, const char* pSrc, unsigned int uiCopyLen)
{
    if (NULL == pSrc)
    {
        return false;
    }

    if (uiCopyLen >= uiMaxLen)
    {
        return false;
    }
    else
    {
        strncpy(pDst, pSrc, uiCopyLen);
        pDst[uiCopyLen] = '\0';
        return true;
    }
}

// ��װ���snprintf�����������ɹ����Զ����ַ�����β���'\0��
bool CToolsHelp::Snprintf(char* pBuffer, unsigned int uiMaxLen, const char* pFormat, ...)
{
    va_list ap;
    va_start (ap, pFormat);
    int iResult = ::vsnprintf (pBuffer, uiMaxLen - 1, pFormat, ap);//lint !e838
    va_end (ap);
    if ( -1 == iResult )
    {
        return false;
    }
    else
    {
        pBuffer[iResult] = '\0';
        return true;
    }
}//lint !e838 !e438

// ��װ���strncat����
bool CToolsHelp::Strncat(char* pDst, unsigned int uiMaxLen,const char* pSrc)
{
    if (uiMaxLen < (strlen(pDst) + strlen(pSrc) + 1))
    {
        return false;
    }
    else
    {
        strncat( pDst, pSrc, strlen(pSrc) );
        return true;
    }
}

// �滻����ָ�����ַ������磺��"12212"����ַ���������"12"���滻��"21"��21221
std::string& CToolsHelp::replace_all_distinct(std::string& str,const std::string& old_value,const std::string& new_value)    
{    
    for(std::string::size_type   pos(0);   pos!=std::string::npos;   pos+=new_value.length()) {    
        if((pos=str.find(old_value,pos))!=std::string::npos)    
            str.replace(pos,old_value.length(),new_value);    
        else   
            break;    
    }
    return str;    
} 

// �滻����ָ�����ַ������磺��"12212"����ַ���������"12"���滻��"21"��22211
std::string& CToolsHelp::replace_all(std::string& str,const std::string& old_value,const std::string& new_value)   
{    
    while(true) //lint !e716
    {    
        std::string::size_type pos(0);    
        if((pos=str.find(old_value)) != std::string::npos)    
            str.replace(pos,old_value.length(),new_value);    
        else 
            break;    
    }    
    return str;    
}
