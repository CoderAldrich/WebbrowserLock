#pragma once


class CResourceClientObj : public CCommonControl
{
public:
	CResourceClientObj(void);
	~CResourceClientObj(void);
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
	bool ReadResource(PRESOURCEHANDLE pResourceReadInfo);

	bool InitResourceHander(PRESOURCEREADINFO pResourceObj,PRESOURCEHANDLE pResourceReadInfo);
	bool UnInitResourceHander(PRESOURCEHANDLE pResourceReadInfo,PRESOURCEREADINFO pResourceObj);
	/**
	 * ���ӹر��¼����ص�����
	 * 
	 * ������ʵ�־���Ͽ�����ҵ����
	 * 
	 */
	void OnCloseConnect();
	
protected:
	SOCKET m_Socket; 
	static TCHAR m_szServerIP[MAX_IP4_ADDRESS_LEN+1];
	static USHORT m_uServerPort;
	static BYTE	m_btResourceBag[MAX_BAG_LEN];
	RESOURCEREADINFO m_ResourceObj;
	RESOURCEHANDLE m_ResourceReadInfo;
};

