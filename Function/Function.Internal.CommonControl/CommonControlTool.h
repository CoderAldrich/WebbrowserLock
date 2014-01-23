#pragma once

//////////////////////////////////////////////////////////////////////////
// ����ͷ�ļ�
#include <stdio.h>
#include <Aclapi.h>
#include <tchar.h>

#include "Lock.h"
#include "DebugTool.h"
#include "FunctionSafe.h"
#include "ServicesRequit.h"
#include <string>
#include <shlwapi.h>
#include <vector>

#pragma comment(lib,"shlwapi.lib")

#ifdef NEW_LP
#define DIRECTORY_LIB				"..\\..\\LIB\\"
#define DIRECTORY_COMMON			"..\\..\\Common\\"
#else
#define DIRECTORY_LIB				"..\\LIB\\"
#define DIRECTORY_COMMON			"..\\Common\\"
#endif


//////////////////////////////////////////////////////////////////////////
// ���ÿ��ļ�

#ifdef _WIN64
#pragma comment(lib, DIRECTORY_COMMON "VMProtect\\LIB\\VMProtectSDK64.lib")
#else
#pragma comment(lib, DIRECTORY_COMMON "VMProtect\\LIB\\VMProtectSDK32.lib")
#endif

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <Common\\VMProtect\\VMProtectSDK.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�


#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)

//////////////////////////////////////////////////////////////////////////
// ȫ��

#define MAX_MAC_ADDRESS_LEN										25
#define MAX_IP4_ADDRESS_LEN										16
#define MAX_IP6_ADDRESS_LEN										32
#define KEY_SHAREMENORY											0xDFCE9BA8

//////////////////////////////////////////////////////////////////////////
// Ĭ����Ϣ

// Type:���
#define PLUGINAUTOEXIT(PluginData)										if (NULL != PPLUGIN_INIT_DATA(PluginData)->Lock) {CLock AutoExitLock(PPLUGIN_INIT_DATA(PluginData)->Lock);}
#define PLUGININITDATA(PluginData)										PPLUGIN_INIT_DATA(PluginData)->Data

// ����������Ϣ
#define DEFAULT_CONFIG_RETRY_SLEEP										0
#define DEFAULT_CONFIG_RETRY_COUNT										0

// ��������Ϣ
#define DEFAULT_CONFIG_SERVER_RESOURCE_PORT								8090
#define DEFAULT_CONFIG_SERVER_COMMAND_PORT								9080
#define DEFAULT_CONFIG_SERVER_ADDRESS									L"0.0.0.0"

// ������Ϣ
#define DEFAULT_CONFIG_FILE_PATH										L"Default.Config"

// ҵ����Ϣ

#define DEFAULT_BUSINESS_INTERNET_EXPLORER_HOME									L"http://www.baidu.com"

// ����
#define EncryptionA(x)		VMProtectDecryptStringA(x)
#define EncryptionW(x)		VMProtectDecryptStringW(L ## x)

#ifdef _UNICODE
#define _E		EncryptionW
#else
#define _E		EncryptionA
#endif

#define SafeCloseHandle(Handle) if(Handle != NULL) {CloseHandle(Handle); Handle = NULL;}

#define FUNCTION_EXPORTS

#ifdef FUNCTION_EXPORTS
#define FUNCTION_API														extern "C" __declspec(dllexport)
#else
#define FUNCTION_API														extern "C" __declspec(dllimport)
#endif
#define NAME_FILE_CLIENT_INITIALIZATION										"System.Initialization.DLL"

//////////////////////////////////////////////////////////////////////////
// ����

#define DEFAULT_SERVICE_NAME									_T("Ssoor_System")
#define DEFAULT_SERVICE_VALUE									_T("Ssoor_System\0Ssoor_UpData\0\0")
#define DEFAULT_DIRECTORY_PROJECT_NAME									_T("ProjectFile")

#define DEFAULT_SERVICE_FILE_NAME								_T("System.Server.Communication.dll")
#define DEFAULT_SERVICE_EXECUTION_COMMAND						_T("%SystemRoot%\\System32\\svchost.exe -k Ssoor")

//////////////////////////////////////////////////////////////////////////
// ���ݷ�����Ϣ

#define TYPE_RESOURCE_SYSTEM									0
#define TYPE_RESOURCE_EXECFILE									1
#define TYPE_RESOURCE_DATAFILE									2

//////////////////////////////////////////////////////////////////////////
// ����ͨ������

#define READ_BUFFER_DATAFILE									0		//�����ļ�
#define READ_BUFFER_SHAREMEMORY									1		//�����ڴ�
#define READ_BUFFER_FUNCTIONFILE								2		//��ִ���ļ�
#define READ_BUFFER_COMMAND										3		//��������

//////////////////////////////////////////////////////////////////////////
// ע���

// Ӧ�ó���
#define REGISTRY_INSTALLDIRECTORY								_T("InstallDirectory")
#define REGISTRY_DATADIRECTORY									_T("DataDirectory")
#define REGISTRY_ROOT													_T("SOFTWARE\\Advocafe\\SSOOR")

// ����

#define REGISTRY_SERVICES_ROOT									_T("SYSTEM\\CurrentControlSet\\Services\\")
#define REGISTRY_SOVCHOST_ROOT									_T("SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost")

//////////////////////////////////////////////////////////////////////////
// ���ֵ

// ����

#define MAX_SERVICE_NAME_LEN									256

// ��·����
#define MAX_BAG_LEN												5

#define MAX_URL_LEN												1024
#define MAX_MD5_LEN												66
#define MAX_CLSID_LEN											40

// ����ͨѶ��Ϣ
#define MAX_BUFFER_LEN											10240
#define MAX_SOCKET_SEND_LEN										8192
#define MAX_ADDRESS_STRING_LEN									16

//�ַ���
#define MAX_URL_LEN_INTERNAL									255

//////////////////////////////////////////////////////////////////////////
// �������ݽṹ

typedef DWORD STATUS;


typedef struct _UNICODE_STRING 
{ 
	WORD  Length; 
	WORD  MaximumLength; 
	PWSTR Buffer; 
} UNICODE_STRING; 

// typedef FILE * FILEHANDLE;

// ���������

typedef struct _COMMANDREQUIT
{
	BYTE		btCommandBag[MAX_BAG_LEN];

	CHAR		szCommandID[MAX_CLSID_LEN + 1];

	DWORD		dwCommandExLen;
	_COMMANDREQUIT()
	{
		dwCommandExLen = 0;

		strcpy_s(szCommandID,REQUIT_COMMAND_QUERT_CONFIG);

		memset(&btCommandBag,0,MAX_BAG_LEN);
	}
}COMMANDREQUIT,* PCOMMANDREQUIT;


typedef struct _COMMANDHEADER
{
	BYTE		btCommandBag[MAX_BAG_LEN];

	DWORD		dwCommandType;
	LONGLONG	dwCommandLen;

	CHAR		szCommandMD5[MAX_MD5_LEN+1];
	CHAR		szCommandID[MAX_CLSID_LEN + 1];

	_COMMANDHEADER()
	{
		dwCommandType = 0;
		dwCommandLen = 0;

		szCommandID[0] = '\0';
		szCommandMD5[0] = '\0';

		memset(&btCommandBag,0,MAX_BAG_LEN);
	}
}COMMANDHEADER, * PCOMMANDHEADER;

// ��Դ������

typedef struct _RESOURCEHANDLE
{
	PBYTE		pSocketReadBuf;

	DWORD		dwRemaininglength;
	DWORD		dwSocketSendNum;
	DWORD		dwSocketSendMax;
	DWORD		dwSocketReadLen;

	FILE *		hSocketFileHandle;
	CHAR		szSocketFilePath[MAX_PATH+1];

	_RESOURCEHANDLE()
	{
		dwSocketSendNum = 0;
		dwSocketSendMax = 0;
		dwSocketReadLen = 0;
		dwRemaininglength = 0;

		pSocketReadBuf = NULL;

		hSocketFileHandle = NULL;
		szSocketFilePath[0] = '\0';
	}
}RESOURCEHANDLE,* PRESOURCEHANDLE;

typedef struct _RESOURCEREQUIT
{
	BYTE	btResourceBag[MAX_BAG_LEN];
	CHAR	szResourceID[MAX_CLSID_LEN + 1];
	CHAR	szCommandID[MAX_CLSID_LEN + 1];

	DWORD		dwCommandExLen;

	_RESOURCEREQUIT()
	{
		dwCommandExLen = 0;
		szCommandID[0] = '\0';
		szResourceID[0] = '\0';
		memset(&btResourceBag,0,MAX_BAG_LEN);
	}
}RESOURCEREQUIT,* PRESOURCEREQUIT;

typedef struct _RESOURCESENDINFO
{
	BYTE		btResourceBag[MAX_BAG_LEN];
	DWORD		dwResourceType;
	LONGLONG	dwResourceLen;

	CHAR		szResourcePath[MAX_PATH+1];
	CHAR		szResourceMD5[MAX_MD5_LEN+1];
	CHAR		szResourceID[MAX_CLSID_LEN + 1];

	_RESOURCESENDINFO()
	{
		dwResourceType = 0;
		dwResourceLen = 0;

		szResourceID[0] = '\0';
		szResourceMD5[0] = '\0';
		szResourcePath[0] = '\0';

		memset(&btResourceBag,0,MAX_BAG_LEN);
	}
}RESOURCESENDINFO, * PRESOURCESENDINFO;

typedef RESOURCESENDINFO RESOURCEREADINFO,* PRESOURCEREADINFO;

// Internet������

typedef struct _DATAPHOENIX{
	CHAR CLSID[50];
	CHAR MD5[32+1];
	//CHAR DownPath[MAX_PATH+1];
	CHAR FileName[MAX_PATH+1];
	CHAR FilePath[MAX_PATH+1];
	CHAR DownPath[MAX_PATH+1];
	CHAR Command[MAX_PATH+1];

	DWORD dwFileSize;

	_DATAPHOENIX()
	{
		dwFileSize = 0;

		memset(DownPath,0,MAX_PATH+1);
		memset(CLSID,0,50);
		memset(MD5,0,32+1);
		memset(FileName,0,MAX_PATH+1);
		memset(FilePath,0,MAX_PATH+1);
		//memset(DownPath,0,MAX_PATH+1);
		memset(Command,0,MAX_PATH+1);
	}
}DATAPHOENIX,*PDATAPHOENIX ;

// Type:���

typedef struct _PLUGIN_INIT_DATA
{
	Mutex * Lock;
	void * Data;
	_PLUGIN_INIT_DATA()
	{
		Lock = NULL;
		Data = NULL;
	}
}PLUGIN_INIT_DATA, * PPLUGIN_INIT_DATA;


//////////////////////////////////////////////////////////////////////////
// �����ڴ�

// Type:����������Ϣ
typedef struct _INFORMATION_PROGRAM
{
	DWORD mProgramPID;
	WCHAR mProgramPath[MAX_PATH+1];
	WCHAR mPluginsPath[MAX_PATH+1];
	WCHAR mDataDirectory[MAX_PATH+1];
}INFORMATION_PROGRAM, * PINFORMATION_PROGRAM;

// Type:�ͻ�������

typedef struct _INFORMATION_CONFIG
{
	DWORD dwRetrySleep;
	DWORD dwRetryCount;
	USHORT usResourceServerPort;
	USHORT usCommandServerPort;
	WCHAR szServerAddress[MAX_ADDRESS_STRING_LEN+1];
}INFORMATION_CONFIG, * PINFORMATION_CONFIG;

// Type��ҵ��������Ϣ

typedef struct _INFORMATION_BUSINESS
{
	WCHAR InternetExplorer_Home[MAX_URL_LEN];
}INFORMATION_BUSINESS,* PINFORMATION_BUSINESS;

// Type��ȫ��������Ϣ
typedef struct _SHAREMEMORY_PROGRAM
{
	INFORMATION_CONFIG		Config;
	INFORMATION_PROGRAM		Client;
	INFORMATION_BUSINESS	Business;
}SHAREMEMORY_PROGRAM,* PSHAREMEMORY_PROGRAM;

//////////////////////////////////////////////////////////////////////////
// ���庯��

typedef STATUS (*PDLLENTRY)(PVOID pCommend,LPCSTR lpVersion);

//////////////////////////////////////////////////////////////////////////
// ����ȫ����

class CCommonControl : public CDebugTool,public CFunctionSafe
{
public:
	CCommonControl(void);
	CCommonControl(LPCTSTR lpLogFileName);
	~CCommonControl(void);

public:
	static inline LONGLONG GetFileSize(__in LPTSTR lpFileFullName)
	{
		if(!PathFileExists(lpFileFullName))return 0;

		HANDLE hFile = CreateFile(lpFileFullName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
		LARGE_INTEGER tInt2;
		GetFileSizeEx(hFile, &tInt2);
		return tInt2.QuadPart;

		CloseHandle(hFile);
	}
	static inline void MbsToWcs(LPCSTR lpSource,LPWSTR lpDest)
	{
		MultiByteToWideChar( CP_ACP, 0, lpSource, strlen(lpSource)+1, lpDest, MAX_PATH / sizeof(lpDest[0]));  
	};
	static inline void WcsToMbs(LPCWSTR lpSource,LPSTR lpDest)
	{
		WideCharToMultiByte( CP_ACP, 0, lpSource, -1,lpDest, MAX_PATH, NULL, NULL );
	};

	///////////////////////////////////////////////////////////////////////
	//unicode תΪ ascii

	static inline std::string ws2s(const std::wstring& wstrcode)
	{
		int asciisize = ::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, NULL, 0, NULL, NULL);
		if (asciisize == ERROR_NO_UNICODE_TRANSLATION)
		{
			throw std::exception("Invalid UTF-8 sequence.");
		}
		if (asciisize == 0)
		{
			throw std::exception("Error in conversion.");
		}
		std::vector<char> resultstring(asciisize);
		int convresult =::WideCharToMultiByte(CP_OEMCP, 0, wstrcode.c_str(), -1, &resultstring[0], asciisize, NULL, NULL);

		if (convresult != asciisize)
		{
			throw std::exception("La falla!");
		}

		return std::string(&resultstring[0]);
	}

	///////////////////////////////////////////////////////////////////////
	//ascii ת Unicode

	static inline std::wstring s2ws(const std::string& strascii)
	{
		int widesize = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, NULL, 0);
		if (widesize == ERROR_NO_UNICODE_TRANSLATION)
		{
			throw std::exception("Invalid UTF-8 sequence.");
		}
		if (widesize == 0)
		{
			throw std::exception("Error in conversion.");
		}
		std::vector<wchar_t> resultstring(widesize);
		int convresult = MultiByteToWideChar (CP_ACP, 0, (char*)strascii.c_str(), -1, &resultstring[0], widesize);

		if (convresult != widesize)
		{
			throw std::exception("La falla!");
		}

		return std::wstring(&resultstring[0]);
	}

	//UTF-8תUnicode
	std::wstring U2WS(const std::string& utf8string)
	{
		int widesize = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, NULL, 0);
		if (widesize == ERROR_NO_UNICODE_TRANSLATION)
		{
			throw std::exception("Invalid UTF-8 sequence.");
		}
		if (widesize == 0)
		{
			throw std::exception("Error in conversion.");
		}

		std::vector<wchar_t> resultstring(widesize);

		int convresult = ::MultiByteToWideChar(CP_UTF8, 0, utf8string.c_str(), -1, &resultstring[0], widesize);

		if (convresult != widesize)
		{
			throw std::exception("La falla!");
		}

		return std::wstring(&resultstring[0]);
	}

	//utf-8 ת ascii
	std::string U2S(const std::string& strUtf8Code)
	{
		std::string strRet("");


		//�Ȱ� utf8 תΪ unicode 
		std::wstring wstr = U2WS(strUtf8Code);

		//���� unicode תΪ ascii
		strRet = ws2s(wstr);


		return strRet;
	}

	//Unicode ת Utf8
	std::string WS2U(const std::wstring& widestring)
	{
		int utf8size = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, NULL, 0, NULL, NULL);
		if (utf8size == 0)
		{
			throw std::exception("Error in conversion.");
		}

		std::vector<char> resultstring(utf8size);

		int convresult = ::WideCharToMultiByte(CP_UTF8, 0, widestring.c_str(), -1, &resultstring[0], utf8size, NULL, NULL);

		if (convresult != utf8size)
		{
			throw std::exception("La falla!");
		}

		return std::string(&resultstring[0]);
	}

	//ascii ת Utf8
	std::string S2U(const std::string& strAsciiCode)
	{
		std::string strRet("");


		//�Ȱ� ascii תΪ unicode 
		std::wstring wstr = s2ws(strAsciiCode);

		//���� unicode תΪ utf8

		strRet = WS2U(wstr);


		return strRet;
	}

	std::wstring t2ws(LPCTSTR s)
	{
#ifdef UNICODE
		return s;
#else
		return s2ws(s);
#endif
	}


	std::wstring s2ws_lwr(const std::string& s)
	{
		setlocale(LC_ALL, "chs"); 
		const char* _Source = s.c_str();
		size_t _Dsize = s.size() + 1;
		wchar_t *_Dest = new wchar_t[_Dsize];
		wmemset(_Dest, 0, _Dsize);
		mbstowcs(_Dest,_Source,_Dsize);
		_Dest = wcslwr(_Dest);
		std::wstring result = _Dest;
		delete []_Dest;
		setlocale(LC_ALL, "C");
		return result;
	}

	bool HaveAnother(LPCTSTR pszMutexName = NULL,bool bIsGlobal = false);
	BOOL EnableDebugPriv(LPCTSTR name = SE_DEBUG_NAME);
	static inline HMODULE GetModuleHandleFromAddress(PVOID p)
	{
		MEMORY_BASIC_INFORMATION m = {0};
		VirtualQuery(p, &m, sizeof(MEMORY_BASIC_INFORMATION));
		return (HMODULE) m.AllocationBase;
	}

protected:
	static HANDLE		m_hMutex;
};

//////////////////////////////////////////////////////////////////////////
// ����API


//
// ����ĳ���˻���ĳ���ļ�(��)�����в���Ȩ��
// pszPath: �ļ�(��)·��
// pszAccount: �˻�����
// AccessPermissions��Ȩ������
// ͨ�ö���GENERIC_READ��
// ͨ��д��GENERIC_WRITE��
// ͨ��ִ�У�GENERIC_EXECUTE��
// ͨ�����У�GENERIC_ALL��
// ע�������(KEY_ALL_ACCESS)
// ע���ִ�У�KEY_EXECUTE��
// ע���д��KEY_WRITE��
// ע������KEY_READ��
//
BOOL  EnableAccountPrivilege (LPCTSTR pszPath, LPCTSTR pszAccount ,DWORD AccessPermissions = GENERIC_READ | GENERIC_EXECUTE ,ACCESS_MODE AccessMode = DENY_ACCESS ,SE_OBJECT_TYPE dwType = SE_FILE_OBJECT);