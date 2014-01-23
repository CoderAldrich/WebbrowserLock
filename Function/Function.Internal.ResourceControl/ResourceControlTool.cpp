#include "ResourceControlTool.h"

#include <io.h>

CResourceControlTool::CResourceControlTool(void)
{
	m_szFileName[0] = NULL;
}


CResourceControlTool::~CResourceControlTool(void)
{

}


DWORD CResourceControlTool::GetDataFromResource(PBYTE pData, UINT nIDResource, LPCTSTR pszResourceType, HMODULE hModule )
{
	//   ����ԴIDת����Դ���� 
	LPTSTR szResourceName = MAKEINTRESOURCE(LOWORD(nIDResource));
	//   ������Դ��� 
	HRSRC hRes = FindResource(hModule, szResourceName, pszResourceType); 
	//   �����Դ���ݵľ�� 
	HGLOBAL hResData; 
	if(!hRes || !(hResData = LoadResource(hModule, hRes)))return 0;
	//   ��Դ���ݵĴ�С 
	DWORD dwSize = SizeofResource(hModule, hRes);
	if(pData == NULL)return dwSize;
	//   ��Դ���� 
	PBYTE pSrc = (PBYTE)LockResource(hResData);
	if(!pSrc)
	{ 
		FreeResource(hResData);
		return 0;
	}; 
	//   �������� 
	CopyMemory(pData, pSrc, dwSize);
	//   �ͷ���Դ 
	FreeResource(hResData);
	return dwSize;
}

bool CResourceControlTool::CreateFileFromResource(LPCTSTR pszFilePath, UINT nIDResource, LPCTSTR pszResourceType, HMODULE hModule )
{
	if(!pszFilePath)
	{
		if(!MakeFileName())return false;
		pszFilePath = m_szFileName;
	}

	HANDLE hFile = CreateFile(
		pszFilePath,
		GENERIC_READ|GENERIC_WRITE,
		FILE_SHARE_READ|FILE_SHARE_WRITE,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,//FILE_ATTRIBUTE_HIDDEN
		NULL
		);
	if(hFile == INVALID_HANDLE_VALUE)return false;

	DWORD dwSize = GetDataFromResource(NULL, nIDResource, pszResourceType,hModule);
	if(dwSize == 0)return false;

	PBYTE pData = new BYTE[dwSize];
	dwSize = GetDataFromResource(pData, nIDResource, pszResourceType,hModule);

	DWORD dwWrited = 0;
	BOOL bReturn = WriteFile(hFile, pData, dwSize, &dwWrited, NULL);
	CloseHandle(hFile);
	if (NULL != pData)
	{
		delete [] pData;
		pData = NULL;
	}

	if(!bReturn || dwSize != dwWrited)return false;

	return true;
}

bool CResourceControlTool::MakeFileName(LPCTSTR pszPrefix)
{
	DWORD dwReturn = GetTempPath(MAX_PATH, m_szFileName);
	if(!dwReturn || dwReturn > MAX_PATH)return false;

	dwReturn = GetTempFileName(m_szFileName, pszPrefix, 0, m_szFileName);

	return dwReturn?true:false;
}

LPCTSTR CResourceControlTool::GetFileName(LPCTSTR pszPrefix)
{
	if(pszPrefix)
	{
		if(*pszPrefix)
			MakeFileName(pszPrefix);
		else
			MakeFileName();
	}

	return m_szFileName;
}
