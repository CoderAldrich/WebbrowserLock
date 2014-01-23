#pragma once


//////////////////////////////////////////////////////////////////////////
// �����

class CCommandObj : CCommonControl
{
public:
	CCommandObj(void);
	CCommandObj(LPCTSTR lpLogFileName);
	~CCommandObj(void);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ�������ļ� InitializationConfig()
	//	Return: �޷���    

	void InitializationConfig(LPCWSTR lpConfigFileFullPath);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  ��ʼ������ Download()
	// Return:  �ɹ�True,����False
	bool Download( __in LPTSTR lpCmdLine);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ���ز��ģ�� DownloadPluginFile()
	//	Return: �ɹ�True,����False     

	bool DownloadPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ������ͨѶģ�� InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = ������IP������Ϊ�� 
	//	Parameter:	usServerPort = �������˿ڣ�����Ϊ��
	//	Return: �ɹ�True,����False     

	bool InitializationSocketClient(LPCTSTR lpServerAddress,USHORT usResourceServerPort,USHORT usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  ���� Execute()
	// Return:  �ɹ�True,����False
	bool Execute(__in LPTSTR lpCmdLine);
private:

	typedef BOOL (* INITIALIZATION) (HINSTANCE);
	INITIALIZATION m_pInitialization;

	INFORMATION_CONFIG m_ConfigCommon;
	CResourceClientObj m_cResourceClientObj;

};

