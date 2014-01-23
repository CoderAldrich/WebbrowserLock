#pragma once
class CCommandObj : CCommonTool
{
public:
	CCommandObj(void);
	~CCommandObj(void);

public:

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  ��ʼ������Initlization()
	// Return:  �ɹ�True,����False
	bool Initialization(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	// ���������

	void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);

	bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);

	bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

	bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);

	//////////////////////////////////////////////////////////////////////////
	// ��Դ������

	void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);

	bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

	bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

	void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);

private:
	//////////////////////////////////////////////////////////////////////////
	// ��Դ������
	CRegistry m_cRegistryObj;
};

