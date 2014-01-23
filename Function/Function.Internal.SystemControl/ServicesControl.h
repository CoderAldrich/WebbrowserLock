#pragma once

#include "SystemControl.h"

#include <Function.Internal.RegistryControl/RegistryControlTool.h>
#include "Function.Internal.CommonControl/CommonControlTool.h"

class CServicesControl : CCommonControl
{
public:
	CServicesControl(void);
	~CServicesControl(void);

	//////////////////////////////////////////////////////////////////////////
	// Explain: ���ѡ�����װ,���ذ�װ�ɹ��ķ����� InstallServiceRandom()
	LPCTSTR InstallServiceRandom(LPCTSTR lpServiceDisplayName, LPCTSTR lpServiceDescription,LPCTSTR lpModulePath);
	bool StartService(LPCTSTR lpService);

protected:
	LPTSTR AddsvchostService();


protected:
	TCHAR m_szServiceName[1024];
	CRegistryControlTool m_cRegistryControlTool;
};

