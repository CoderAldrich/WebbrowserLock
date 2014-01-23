#pragma once
#include <Windows.h>
#include <tchar.h>
#include <Tlhelp32.h>

#include <Function.Internal.CommonControl\\CommonControlTool.h>

#define SE_DEBUG_PRIVILEGE 0x13
#define OB_TYPE_INDEX_TYPE              1 // [ObjT] "Type"
#define OB_TYPE_INDEX_DIRECTORY         2 // [Dire] "Directory"
#define OB_TYPE_INDEX_SYMBOLIC_LINK     3 // [Symb] "SymbolicLink"
#define OB_TYPE_INDEX_TOKEN             4 // [Toke] "Token"
#define OB_TYPE_INDEX_PROCESS           5 // [Proc] "Process"
#define OB_TYPE_INDEX_THREAD            6 // [Thre] "Thread"
#define OB_TYPE_INDEX_JOB               7 // [Job ] "Job"
#define OB_TYPE_INDEX_EVENT             8 // [Even] "Event"
#define OB_TYPE_INDEX_EVENT_PAIR        9 // [Even] "EventPair"
#define OB_TYPE_INDEX_MUTANT           10 // [Muta] "Mutant"
#define OB_TYPE_INDEX_CALLBACK         11 // [Call] "Callback"
#define OB_TYPE_INDEX_SEMAPHORE        12 // [Sema] "Semaphore"
#define OB_TYPE_INDEX_TIMER            13 // [Time] "Timer"
#define OB_TYPE_INDEX_PROFILE          14 // [Prof] "Profile"
#define OB_TYPE_INDEX_WINDOW_STATION   15 // [Wind] "WindowStation"
#define OB_TYPE_INDEX_DESKTOP          16 // [Desk] "Desktop"
#define OB_TYPE_INDEX_SECTION          17 // [Sect] "Section"
#define OB_TYPE_INDEX_KEY              18 // [Key ] "Key"
#define OB_TYPE_INDEX_PORT             19 // [Port] "Port"
#define OB_TYPE_INDEX_WAITABLE_PORT    20 // [Wait] "WaitablePort"
#define OB_TYPE_INDEX_ADAPTER          21 // [Adap] "Adapter"
#define OB_TYPE_INDEX_CONTROLLER       22 // [Cont] "Controller"
#define OB_TYPE_INDEX_DEVICE           23 // [Devi] "Device"
#define OB_TYPE_INDEX_DRIVER           24 // [Driv] "Driver"
#define OB_TYPE_INDEX_IO_COMPLETION    25 // [IoCo] "IoCompletion"
#define OB_TYPE_INDEX_FILE             26 // [File] "File"
#define OB_TYPE_INDEX_WMI_GUID         27 // [WmiG] "WmiGuid" 

typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             // obsolete...delete
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  // MaxSystemInfoClass should always be the last enum
} SYSTEM_INFORMATION_CLASS; 

//�ڴ��д���ݽṹ
typedef struct _MEMORY_CHUNKS {

	ULONG Address;

	PVOID Data;

	ULONG Length;

}MEMORY_CHUNKS, *PMEMORY_CHUNKS;

//�ڴ��д���������ݽṹ
typedef enum _SYSDBG_COMMAND {
	//���ں˿ռ俽�����û��ռ䣬���ߴ��û��ռ俽�����û��ռ�
	//���ǲ��ܴ��û��ռ俽�����ں˿ռ�
	SysDbgCopyMemoryChunks_0 = 8,
	//SysDbgReadVirtualMemory = 8,

	//���û��ռ俽�����ں˿ռ䣬���ߴ��û��ռ俽�����û��ռ�
	//���ǲ��ܴ��ں˿ռ俽�����û��ռ�
	SysDbgCopyMemoryChunks_1 = 9,
	//SysDbgWriteVirtualMemory = 9,
} SYSDBG_COMMAND, *PSYSDBG_COMMAND;
typedef struct _PROCESSINFO
{
	DWORD dwProcessId;
	HANDLE hProcess;
	TCHAR sExeFile[MAX_PATH];
}PROCESSINFO,*PPROCESSINFO;
//������Ϣ���ݽṹ
typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG ProcessID; //���̵ı�ʶID
	UCHAR ObjectTypeNumber; //��������
	UCHAR Flags; //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
	USHORT Handle; //����������ֵ
	PVOID Object; //��������ָ���ں˶����ַ
	ACCESS_MASK GrantedAccess; //�������ʱ��׼��Ķ���ķ���Ȩ
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

//ZwSystemDebugControl()��������
typedef NTSTATUS (NTAPI *PZWSYSTEMDEBUGCONTROL)(SYSDBG_COMMAND,PVOID,ULONG,PVOID,ULONG,PULONG);
//NtQuerySystemInforamation()��������
typedef NTSTATUS (NTAPI *PNTQUERYSYSTEMINFORMATION)(SYSTEM_INFORMATION_CLASS,PVOID,ULONG,PULONG);
//ZwAllocateVirtualMemory()��������
typedef NTSTATUS (NTAPI *PZWALLOCATEVIRTUALMEMORY)(HANDLE, PVOID *, ULONG, PULONG, ULONG, ULONG);
//ZwFreeVirtualMemory()��������
typedef NTSTATUS (NTAPI *PZWFREEVIRTUALMEMORY)(HANDLE, PVOID *, PDWORD, ULONG);
//���̿�����
class CProcess : CCommonControl {
public:
	CProcess();
	CProcess(HANDLE hProcessHand);
	~CProcess();
	//////////////////////////////////////////////////////////////////////////
	//
	//�����ĺ��壺
	//	cProcessPath [In].                         
	//	��Ҫִ�еĳ���·��������\��\\�滻
	//
	//	cRunCommendLink [In] . 
	//	ִ�г����������.
	//
	//////////////////////////////////////////////////////////////////////////
	PROCESS_INFORMATION ProcessCreate(PTCHAR cProcessPath,PTCHAR cRunCommendLink);
	HANDLE GetProcessHand();
	HANDLE GetThreadHand();
	DWORD GetProcessID();
	DWORD GetThreadID();
	BOOL HideProcess();
	BOOL EnableDebugPriv(PTCHAR name);
	STATUS GetCurrentProcessPath(LPSTR lpProcessPath);
	BOOL HaveProcess (PPROCESSINFO pProcess);
	BOOL PrivilegeControl(ULONG uPrivilege,BOOLEAN bEnable,BOOLEAN bCurrentThread);
protected:

	BOOL InitializeFunction();
	BOOL SystemDebugControl(ULONG uAddress,PVOID pvData,ULONG usize,SYSDBG_COMMAND command);
	BOOL OperateSystemMemory(MEMORY_CHUNKS datas,SYSDBG_COMMAND command);
protected:
	PZWSYSTEMDEBUGCONTROL ZwSystemDebugControl;
	PNTQUERYSYSTEMINFORMATION NtQuerySystemInformation;
	PZWALLOCATEVIRTUALMEMORY ZwAllocateVirtualMemory;
	PZWFREEVIRTUALMEMORY ZwFreeVirtualMemory;
	PROCESS_INFORMATION m_ProcInfo;//�ñ��������½����̵ı�־��Ϣ
private:

};

//////////////////////////////////////////////////////////////////////////
//
//�����ĺ��壺
//	uPrivailege [In] Privilege index to change.                         
//	 ����Ҫ��Ȩ�����ƣ����Ե�MSDN���ҹ���Process Token & Privilege���ݿ��Բ鵽
//
//	bEnable [In] If TRUE, then enable the privilege otherwise disable. 
//	 ���ΪTrue ���Ǵ���ӦȨ�ޣ����ΪFalse ���ǹر���ӦȨ��
//
//	bCurrentThread [In] If TRUE, then enable in calling thread, otherwise process. 
//	 ���ΪTrue ���������ǰ�߳�Ȩ�ޣ����������������̵�Ȩ��
//
///////////////////////////////////////////////////////////////////////////
BOOL PrivilegeControl(const char *uPrivailege,BOOLEAN bEnable,BOOLEAN bCurrentThread);