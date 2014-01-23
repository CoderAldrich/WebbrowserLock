#ifndef SYSTEM_PLUGIN_FASTSHUTDOWN_H
#define SYSTEM_PLUGIN_FASTSHUTDOWN_H

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <UtilFunc/PluginInc.h>


#include <Interfaces/PluginsInterface.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�


#include <Function.Internal.CommonControl/CommonControlTool.h>
#include <Function.Internal.HookControl/HookControlTool.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�

const char* const ClsidFastShutdown = "E0CC72F3-1C85-4903-9A3C-5FAE7EA637C1";

//typedef int NTSTATUS;
typedef enum _SYSDBG_COMMAND
{
	SysDbgSysReadIoSpace = 14,
	SysDbgSysWriteIoSpace = 15
}SYSDBG_COMMAND, *PSYSDBG_COMMAND;

const unsigned int SE_SHUTDOWN_PRIVILEGE = 0x13;
typedef enum _SHUTDOWN_ACTION { 
	ShutdownNoReboot,  
	ShutdownReboot, 
	ShutdownPowerOff 
}SHUTDOWN_ACTION;


typedef struct _IO_STRUCT
{
	DWORD IoAddr;       // IN: Aligned to NumBYTEs,I/O address
	DWORD Reserved1;    // Never accessed by the kernel
	PVOID pBuffer;      // IN (write) or OUT (read): Ptr to buffer
	DWORD NumBYTEs;     // IN: # BYTEs to read/write. Only use 1, 2, or 4.
	DWORD Reserved4;    // Must be 1
	DWORD Reserved5;    // Must be 0
	DWORD Reserved6;    // Must be 1
	DWORD Reserved7;    // Never accessed by the kernel
}
IO_STRUCT, *PIO_STRUCT;






//////////////////////////////////////////////////////////////////////////
// API����
typedef NTSTATUS (WINAPI * PRTLADJUSTPRIVILEGE)(ULONG Privilege,BOOLEAN Enable,BOOLEAN CurrentThread,PBOOLEAN Enabled);

typedef BOOL	 (WINAPI * PExitWindowsEx)	   (UINT,DWORD);
typedef NTSTATUS (NTAPI * PRtlAdjustPrivilege) (int, BOOL, BOOL, int*);
typedef NTSTATUS (NTAPI * PZwShutdownSystem)   (SHUTDOWN_ACTION Action);
typedef NTSTATUS (NTAPI * PZwSystemDebugControl) (SYSDBG_COMMAND ControlCode,PVOID InputBuffer,ULONG InputBufferLength,PVOID OutputBuffer,ULONG OutputBufferLength,PULONG ReturnLength);



FUNCTION_API DWORD FastShutdown(PVOID p);

void OutPortB (PZwSystemDebugControl ZwSystemDebugControl,int Port, BYTE Value);
int Shutdown(SHUTDOWN_ACTION saShutdown);

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL FakeExitWindowsEx(UINT uFlags,DWORD dwReserved);
BOOL InjectWinlogon(LPTHREAD_START_ROUTINE,HINSTANCE hModule);


class CFastShutdown
	: public IPlugin, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CFastShutdown)
		X3DEFINE_INTERFACE_ENTRY(IPlugin)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
	CFastShutdown();
	virtual ~CFastShutdown();

private:

public:
	//////////////////////////////////////////////////////////////////////////
	//
	//  Explain:  ��ʼ�����ģ�� InitlizationRemove()
	//  Return: �ɹ�True,����False

	virtual bool Initialization(PVOID pData);
private:
	//////////////////////////////////////////////////////////////////////////
	// ��ʼ��

private:
};

#endif