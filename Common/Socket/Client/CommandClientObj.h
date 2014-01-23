#pragma once


class CCommandClientObj : public CCommonControl
{
public:
	CCommandClientObj(void);
	~CCommandClientObj(void);
	//////////////////////////////////////////////////////////////////////////
	// Explain ConnectServer() ���ӷ�����
	//     lpServerIP * ������IP
	//     uPort *  �������˿�
	bool ConnectServer(LPCTSTR lpServerIP = m_szServerIP,USHORT uPort = m_uServerPort);
	//////////////////////////////////////////////////////////////////////////
	// Explain Send() ��������
	//     lpMessage * ����ָ��
	//     dwMessageSize *  ���ݳ���
	long Send(PVOID lpMessage, DWORD dwMessageSize);
	//////////////////////////////////////////////////////////////////////////
	// Explain Send() ��������
	//     lpMessage * ����ָ��
	//     dwMessageSize *  ���ݳ���
	bool Recv(PVOID lpMessage, DWORD dwMessageSize,bool bFlags = FALSE);
	//////////////////////////////////////////////////////////////////////////
	// Explain Execution() ��ʼ����
	//     lpMessage * ����ָ��
	//     dwMessageSize *  ���ݳ���
	bool Execution(CResourceClientObj * pResourceClientObj);
	

protected:

	bool InitCommand(PRESOURCEREQUIT pResourceSendInfo);
	
	/**
	 * ���ӹر��¼����ص�����
	 * 
	 * ������ʵ�־���Ͽ�����ҵ����
	 * 
	 */
	void OnCloseConnect();
	/**
	 * ���ݵ���ص�����
	 * 
	 * ������ʵ�־���Ͽ�����ҵ����
	 * 
	 */
	bool ExecuteCommand();

	inline PBYTE GetResourceBag()
	{
		return m_btResourceBag;
	}
	
protected:
	SOCKET m_Socket; 
	static TCHAR m_szServerIP[MAX_IP4_ADDRESS_LEN+1];
	static USHORT m_uServerPort;
	static BYTE	m_btResourceBag[MAX_BAG_LEN];
	RESOURCEREQUIT m_rResourceRequit;
	CResourceClientObj * m_pResourceClientObj;
};

