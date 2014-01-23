#pragma once

#define COMMAND_SERVICES_CLSID													"{00000000-0000-0000-0000-000000000001}"	//Ĭ��Client��ʼ��CLSID
class CCommandServicesObj : public mdk::NetServer   , CCommonControl
{
public:
	CCommandServicesObj(void);
	~CCommandServicesObj(void);
	
protected:
	void DispatchCommand(mdk::NetHost &host,PCOMMANDREQUIT pResourceRequit);




	bool InitCommand(mdk::NetHost &host,PCOMMANDREQUIT pCommandRequit,PRESOURCESENDINFO pResourceObj);
	bool UnInitCommand(PRESOURCESENDINFO pResourceObj);

	static bool FormatCommandInfo(LPCTSTR lpFileName,mdk::NetHost & nClientHost);
	/*
	 *	���������߳�
	 */
	void* Main(void* pParam);
	
	/**
	 * �������¼��ص�����
	 * 
	 * ������ʵ�־�������ҵ����
	 * 
	 */
	void OnConnect(mdk::NetHost& host);
	/**
	 * ���ӹر��¼����ص�����
	 * 
	 * ������ʵ�־���Ͽ�����ҵ����
	 * 
	 */
	void OnCloseConnect(mdk::NetHost& host);
	/**
	 * ���ݵ���ص�����
	 * 
	 * ������ʵ�־���Ͽ�����ҵ����
	 * 
	*/
	void OnMsg(mdk::NetHost& host);
	
protected:
	TCHAR m_szDataDirectory[MAX_PATH+1];
	CRegistryControlTool m_cRegistryObj;
	static BYTE	m_btCommandBag[MAX_BAG_LEN];
};

