#include "HookTool.h"

namespace Hook
{


//////////////////////////////////////////////////////////////////////////
//
//  Hook mswsock.dll�������Ntdll!NtDeviceIoControlFile
//  ���������TDI Cilent�����������˷��
//  �ȶ������Σ�RING3����ײ�İ�����~
//
//////////////////////////////////////////////////////////////////////////
PVOID HookIAT(LPCTSTR lpIDTDllName,LPCTSTR lpHookDllName,LPCTSTR lpHookFunction,ULONG NewFunction)
{
	HMODULE hMod;
	hMod = LoadLibrary(lpIDTDllName);//�õ���ģ��lpHookDllName��ַ	
	if (!hMod)
	{
		FCHK(0x36005101,"[HookAPIForIDT] hMod = LoadLibrary(lpIDTDllName) lpIDTDllName=%s hMod=%c failed.\n",lpIDTDllName,hMod);
	}
	PIMAGE_DOS_HEADER pDosHeader = (PIMAGE_DOS_HEADER)hMod ; //�õ�DOSͷ	
	if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)//���DOSͷ��Ч	
	{
		FCHK(0x32005101,"[HookAPIForIDT] pDosHeader->e_magic != IMAGE_DOS_SIGNATURE pDosHeader->e_magic=0x%x IMAGE_DOS_SIGNATURE=0x%x failed.\n",pDosHeader->e_magic,IMAGE_DOS_SIGNATURE);
	}
	PIMAGE_NT_HEADERS pNtHeaders = (PIMAGE_NT_HEADERS)((ULONG)hMod + pDosHeader->e_lfanew);//�õ�NTͷ	
	if (pNtHeaders->Signature != IMAGE_NT_SIGNATURE)//���NTͷ��Ч
	{
		FCHK(0x32005102,"[HookAPIForIDT] pNtHeaders->Signature != IMAGE_NT_SIGNATURE pNtHeaders->Signature=0x%x IMAGE_NT_SIGNATURE=0x%x failed.\n",pNtHeaders->Signature,IMAGE_NT_SIGNATURE);
	}
	
	if (pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||
		pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)//������������Ŀ¼�Ƿ����
	{
		FCHK(0x32005103,"[HookAPIForIDT] (pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||\
								   pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0) \
								   IMAGE_DIRECTORY_ENTRY_IMPORT=%c failed.\n",IMAGE_DIRECTORY_ENTRY_IMPORT);
	}
	//�õ����������ָ��
	PIMAGE_IMPORT_DESCRIPTOR ImportDescriptor = (PIMAGE_IMPORT_DESCRIPTOR)((ULONG)hMod + pNtHeaders->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

	PIMAGE_THUNK_DATA ThunkData ; 

	//���ÿ��������
	while(ImportDescriptor->FirstThunk)
	{
		const char *dllname = (const char*)((ULONG)hMod + ImportDescriptor->Name);
//		if (_tcsicmp(dllname ,lpHookDllName) !=0)//�����������Ƿ�ΪlpHookDllName,������ǣ���������һ������---------------------------------------------
		{
			ImportDescriptor ++ ; 
			continue;
		}

		ThunkData = (PIMAGE_THUNK_DATA)((ULONG)hMod + ImportDescriptor->OriginalFirstThunk);

		int no = 1;
		while(ThunkData->u1.Function)
		{
			//��麯���Ƿ�ΪNtDeviceIoControlFile

			const char* functionname = (char*)((ULONG)hMod + ThunkData->u1.AddressOfData + 2);
//			if (_tcsicmp(functionname , lpHookFunction) == 0 )---------------------------------------------------------------------------------
			{
				//
				//����ǣ���ô��¼ԭʼ������ַ
				//HOOK���ǵĺ�����ַ
				//
				ULONG btw ; 
				PDWORD lpAddr = (DWORD *)((ULONG)hMod + (DWORD)ImportDescriptor->FirstThunk) +(no-1);
				PVOID pOldFunction =(PVOID)(*(ULONG*)lpAddr);
				if (!WriteProcessMemory(GetCurrentProcess() , lpAddr , &NewFunction , sizeof(ULONG), &btw ))
				{
					FCHK(0x31005401,"[HookAPIForIDT] WriteProcessMemory(GetCurrentProcess() , lpAddr , &NewFunction , sizeof(ULONG), &btw ) failed.\n");
				}

				return pOldFunction;
			}

			no++;
			ThunkData ++;
		}
		ImportDescriptor ++;
	}
	return NULL; 
}

}