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

	bool InitializationSocketClient(LPCSTR lpServerAddress,USHORT usResourceServerPort,USHORT usCommandServerPort);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  ���� Execute()
	// Return:  �ɹ�True,����False
	bool Execute(__in LPTSTR lpCmdLine);

private:

	static bool GetBhoListType(const char * szType,bool DefType = true)
	{
		switch (szType[0])
		{
		case  0 :
			return  DefType;
		case 'U':
			return  true;
		case 'u':
			return  true;
		case 'T':
			return  false;
		case 't':
			return  false;
		default:
			return DefType;
		}
		return DefType;
	}

private:

	//////////////////////////////////////////////////////////////////////////
	// BHO Data

	// BHO
	typedef struct _DATA_BHO
	{
		WCHAR CLSID[MAX_CLSID_LEN+1];
	}DATA_BHO, * PDATA_BHO;
	typedef struct _DATA_SECURITY_BHO
	{
		bool Type;
		ULONG ArraySize;
		ULONG ArrayOffset;
		PDATA_BHO BHO;
	}DATA_SECURITY_BHO, * PDATA_SECURITY_BHO;

private:

	typedef BOOL (* INITIALIZATION) (HINSTANCE);
	INITIALIZATION m_pInitialization;
	DATA_SECURITY_BHO m_SecurityData;
	CShareMemory<DATA_SECURITY_BHO> m_ShareMemoryBHO;
	CFileControlTool m_cFileControlTool;
	INFORMATION_CONFIG m_ConfigCommon;

};

