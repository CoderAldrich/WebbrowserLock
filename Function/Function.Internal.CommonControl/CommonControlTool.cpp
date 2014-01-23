#include <Windows.h>
#include <tchar.h>
#include "CommonControlTool.h"

 HANDLE	CCommonControl::m_hMutex = NULL;

CCommonControl::CCommonControl(void) : CDebugTool(_T("Default"))
{
}

CCommonControl::CCommonControl(LPCTSTR lpLogFileName) : CDebugTool(lpLogFileName)
{

}

CCommonControl::~CCommonControl(void)
{
}



//////////////////////////////////////////////////////////////////////////
//
//	Explain:  ����Ӧ�ü�Ȩ��EnablePrivilege()
//	Parameter:	hProcessID = ����PID 
//	Parameter:	szPrivName = Ȩ������
//	Parameter:	fEnable = �Ƿ���
//	Return: �ɹ�True,����False     
//
//////////////////////////////////////////////////////////////////////////
BOOL CCommonControl::EnableDebugPriv(LPCTSTR name)
{
	BOOL bRet = FALSE;
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//�򿪽������ƻ�
	bRet = OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken);
	//��ý��̱���ΨһID
	bRet = LookupPrivilegeValue(NULL, name, &luid);

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;

	//����Ȩ��
	bRet = AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL);
	return bRet;
} 

bool CCommonControl::HaveAnother(LPCTSTR pszMutexName,bool bIsGlobal)
{
	TCHAR szFilePath[MAX_PATH + 1];
	TCHAR szFileName[MAX_PATH + 1];
	
	if(!pszMutexName)
	{
		::GetModuleFileName(NULL, szFilePath, MAX_PATH);
		szFilePath[MAX_PATH] = NULL;
		_tsplitpath(szFilePath, NULL, NULL, szFileName, NULL);
		pszMutexName = szFileName;
	}

	if(bIsGlobal)
		_stprintf(szFilePath, _T("Global\\%s"), pszMutexName);
	else
		_stprintf(szFilePath, _T("Local\\%s"), pszMutexName);
	
	m_hMutex = ::CreateMutex(NULL, FALSE, szFilePath);
	
	DWORD dwError = GetLastError();
	
	if(!m_hMutex)return dwError;
	
	return (dwError == ERROR_ALREADY_EXISTS)?-1:0;
}
