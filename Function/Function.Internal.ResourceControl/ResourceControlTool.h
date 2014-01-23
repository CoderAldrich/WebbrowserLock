#pragma once

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>



//////////////////////////////////////////////////////////////////////////
// API����
typedef bool (* PFINDFILE)(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pParameter);
typedef bool (* PFINDFOLDER)(LPCTSTR lpFolderPath,LPCTSTR lpFolderName,PVOID pParameter);

class CResourceControlTool
{
public:
	CResourceControlTool(void);
	~CResourceControlTool(void);	

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ����Դ��ȡ���� GetDataFromResource����
	//  pData���������ݵĻ�����ָ��
	//  nIDResource����ԴID
	//  pszResourceType����Դ����
	//  hModule��ģ����
	//  Return�����س���
	DWORD GetDataFromResource(PBYTE pData, UINT nIDResource, LPCTSTR pszResourceType = _T("BIN"), HMODULE hModule = NULL);
	

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ����Դд���ļ� CreateFileFromResource����
	//  lpszFilePath���ļ������·��
	//  nIDResource����ԴID
	//  pszResourceType����Դ����
	//  hModule��ģ����
	//  Return���ɹ�������
	bool CreateFileFromResource(LPCTSTR lpszFilePath, UINT nIDResource, LPCTSTR pszResourceType = _T("BIN"), HMODULE hModule = NULL);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ���һ����ʱ�ļ����� GetFileName����
	//  pszPrefix��·���Ļ�����ָ��(��Ϊ��)
	//  Return���ɹ�����·��
	LPCTSTR GetFileName(LPCTSTR pszPrefix = NULL);
private:

	bool MakeFileName(LPCTSTR pszPrefix = _T("~"));
	TCHAR m_szFileName[MAX_PATH + 1];
};
