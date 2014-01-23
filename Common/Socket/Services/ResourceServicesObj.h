#pragma once

class CResourceServicesObj : public mdk::NetServer   , CCommonControl
{
public:
	CResourceServicesObj(void);
	~CResourceServicesObj(void);
	
protected:
	void DispatchResource(mdk::NetHost &host,PRESOURCEREQUIT pResourceRequit);
	bool SendResource(mdk::NetHost &host,PRESOURCESENDINFO pResourceObj,LPCTSTR lpFileFullName);

	bool FormatResourceHander(PRESOURCEREQUIT pResourceRequit,PRESOURCESENDINFO pResourceObj,__out LPTSTR lpFileFullName);

	void FormatResourceFullName(LPCSTR lpCLSID,LPTSTR lpBuffer,DWORD dwBufLen);
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
	CRegistryControlTool m_cRegistryObj;
	static BYTE	m_btResourceBag[MAX_BAG_LEN];
};

