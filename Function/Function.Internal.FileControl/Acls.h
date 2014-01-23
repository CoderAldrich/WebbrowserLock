#pragma once

//////////////////////////////////////////////////////////////////////////
//ʹ��Windows��HeapAlloc�������ж�̬�ڴ����

#define myheapalloc(x) (HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, x))
#define myheapfree(x)  (HeapFree(GetProcessHeap(), 0, x))

//////////////////////////////////////////////////////////////////////////
// ����API

typedef BOOL (WINAPI *SetSecurityDescriptorControlFnPtr)(IN PSECURITY_DESCRIPTOR pSecurityDescriptor,IN SECURITY_DESCRIPTOR_CONTROL ControlBitsOfInterest,IN SECURITY_DESCRIPTOR_CONTROL ControlBitsToSet);

typedef BOOL (WINAPI *AddAccessAllowedAceExFnPtr)(PACL pAcl,DWORD dwAceRevision,DWORD AceFlags,DWORD AccessMask,PSID pSid);

class CAcls
{
public:
	CAcls(void);
	~CAcls(void);


	//////////////////////////////////////////////////////////////////////////
	//
	//	Explain:���ACLȨ��AddAccessRights()
	//		lpszFileName	�����ACL���ļ�·��
	//		lpszAccountName	����ӵ�ACL�û���
	//		dwAccessMask	����ӵ�Ȩ��

	BOOL AddAccessRights(LPCTSTR lpszFileName, LPCTSTR lpszAccountName,DWORD dwAccessMask);
};

