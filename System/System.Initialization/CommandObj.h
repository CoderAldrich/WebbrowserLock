#pragma once

//////////////////////////////////////////////////////////////////////////
// �����

//////////////////////////////////////////////////////////////////////////
// API����
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

//////////////////////////////////////////////////////////////////////////

#define READ_BUFFER_COMMAND				3		//��������
#define READ_BUFFER_DATAFILE			0		//�����ļ�
#define READ_BUFFER_SHAREMEMORY			1		//�����ڴ�
#define READ_BUFFER_FUNCTIONFILE		2		//��ִ���ļ�

//////////////////////////////////////////////////////////////////////////

class CCommandObj : CCommonControl
{
public:
	CCommandObj(void);
	CCommandObj(LPCTSTR lpLogFileName);
	~CCommandObj(void);

	//////////////////////////////////////////////////////////////////////////
	// 
	// Explain:  ��ʼ������Initlization()
	// Return:  �ɹ�True,����False
	bool Initialization(HMODULE hModule);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ����Ŀ��Ϣ InternalInitializationClient()
	//	Return: �޷���    

	void InternalInitializationClient(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ��������Ϣ InternalInitializationConfig()
	//	Return: �޷���    

	void InternalInitializationConfig(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ����ȫ������Ϣ InternalInitializationSecurity()
	//	Return: �޷���    

	void InternalInitializationSecurity(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ��ҵ����Ϣ InternalInitializationBusiness()
	//	Return: �޷���    

	void InternalInitializationBusiness(TiXmlHandle & cConfigObj);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�������ڴ�ģ�� InitlizationShareMemory()
	//  Return: �ɹ�True,����False 

	bool InitlizationShareMemory(LPCWSTR lpConfigFileFullPath);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ������ͨѶģ�� InitializationSocketClient(LPCTSTR lpServerAddress = NULL,USHORT usServerPort = DEFAULT_SERVER_PORT)
	//	Parameter:	lpServerAddress = ������IP������Ϊ�� 
	//	Parameter:	usServerPort = �������˿ڣ�����Ϊ��
	//	Return: �ɹ�True,����False     

	bool InitializationSocketClient();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ���������ģ�� InitlizationPlugin()
	//	Return: �ɹ�True,����False     

	bool InitlizationPluginManager(bool bLoadCore);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ���ز��ģ�� LoadPlugin()
	//	Return: �ɹ�True,����False     

	bool LoadPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ�����ģ�� InitlizationPlugin()
	//	Return: �ɹ�True,����False     

	bool InitlizationPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��ʼ�����ģ�� InitlizationPlugin()
	//	Return: �ɹ�True,����False     

	ULONG ExecutePlugin(LPCSTR pszPluginClass,PVOID pDataAddress);

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ������������ UnInitlizationPlugin()
	//	Return: �ɹ�True,����False     

	void UnInitlizationPlugin();

	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:  ��������������� Send(GUID guid,PVOID pData,DWORD dwDataLen,DWORD dwDataType = READ_BUFFER_COMMAND)
	//	Parameter:	guid = �������ݲ��Guid 
	//	Parameter:	pData = ����ָ��
	//	Parameter:	dwDataLen = ���ݳ���
	//	Parameter:	dwDataType = ��������
	//	Return: �ɹ�True,����False

	BOOL Send(GUID guid,PVOID pData,DWORD dwDataLen,DWORD dwDataType = READ_BUFFER_COMMAND);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain: ͨ����������ȡ����ID GetProcessIDForProcessName(LPTSTR lpProcessName)
	//  Parameter: lpProcessName ������
	//  Return: �ɹ����ؽ���Id��ʧ�ܷ���0

	DWORD GetProcessIDForProcessName(LPCTSTR lpProcessName);
private:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�������ڴ�ģ�� InitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL)
	//	Parameter:	lpszCurrentPath = ����·�� 
	//	Parameter:	lpURL = ��ҳ��ַ
	//	Return: �ɹ�True,����False     

	bool InternalInitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL);
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ƴװ·��  InternalCopyPath(LPTSTR Destination,rsize_t Size,LPCTSTR SourceAddress,LPTSTR String);
	//	Parameter:	Destination = ���ձ��������ַ 
	//	Parameter:	Size = ���ձ��������С
	//	Parameter:	SourceAddress = ·��Ƭ��1 
	//	Parameter:	String = ·��Ƭ��2
	//	Return: �ɹ�True,����False     

	bool InternalCopyPath(LPTSTR Destination,rsize_t Size,LPCTSTR SourceAddress,LPTSTR String);

	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ȡ������ InitShareMemory(LPCTSTR lpszCurrentPath,LPCTSTR lpURL)
	//	Parameter:	lpszCurrentPath = ����·�� 
	//	Parameter:	lpURL = ��ҳ��ַ
	//	Return: �ɹ�True,����False     
	static unsigned WINAPI InternalExecutePlugin(PVOID pPluginInfo);

private:

	//////////////////////////////////////////////////////////////////////////
	// ִ�в��

	typedef struct _EXECUTE_PLUGIN_INFO
	{
		ULONG Return;
		ULONG StartTime;
		PVOID DataAddress;
		LPCSTR PluginClassName;
	}EXECUTE_PLUGIN_INFO, * PEXECUTE_PLUGIN_INFO;

private:
	static CPluginManager m_Plugin;
	//////////////////////////////////////////////////////////////////////////
	//�����ڴ�
	CShareMemory<SHAREMEMORY_PROGRAM> m_UserPool;

	PVOID m_pReadBuf;
	DWORD m_dwBufLen;

public:
	PSHAREMEMORY_PROGRAM m_pUserPool;
	Ix_PluginLoader* m_PluginLoader;
	vector<LPCSTR> m_Plugins;
	map<string,PPLUGIN_INIT_DATA> m_PluginInitData;
 	CResourceClientObj m_cResourceClientObj;
 	CCommandClientObj m_cCommandClientObj;
};

