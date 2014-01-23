
#include "CommonControlTool.h"



BOOL  EnableAccountPrivilege (LPCTSTR pszPath, LPCTSTR pszAccount ,DWORD AccessPermissions /* = GENERIC_READ | GENERIC_EXECUTE  */,ACCESS_MODE AccessMode /* = DENY_ACCESS  */,SE_OBJECT_TYPE dwType)
{
	BOOL bSuccess = TRUE;
	PACL pNewDacl = NULL, pOldDacl = NULL;
	EXPLICIT_ACCESS ea;
	do
	{
		// ��ȡ�ļ�(��)��ȫ�����DACL�б�
		if (ERROR_SUCCESS != GetNamedSecurityInfo (pszPath,(SE_OBJECT_TYPE) dwType, DACL_SECURITY_INFORMATION, NULL, NULL, &pOldDacl, NULL, NULL))
		{
			bSuccess  =  FALSE;
			break;
		}

		// �˴�����ֱ����AddAccessAllowedAce����,��Ϊ���е�DACL�����ǹ̶�,�������´���һ��DACL����
		// ����ָ���û��ʻ��ķ��ʿ�����Ϣ(����ָ���ܾ�ȫ���ķ���Ȩ��)
		switch (dwType)
		{
		case SE_REGISTRY_KEY:
			::BuildExplicitAccessWithName (&ea, (LPTSTR)pszAccount, AccessPermissions, AccessMode, SUB_CONTAINERS_AND_OBJECTS_INHERIT);
			break;
		case SE_FILE_OBJECT:
			::BuildExplicitAccessWithName (&ea, (LPTSTR)pszAccount, AccessPermissions, AccessMode, SUB_CONTAINERS_AND_OBJECTS_INHERIT);
			break;
		default:
			return FALSE;
		}

		// �����µ�ACL����(�ϲ����е�ACL����͸����ɵ��û��ʻ����ʿ�����Ϣ)
		if (ERROR_SUCCESS != ::SetEntriesInAcl(1, &ea, pOldDacl, &pNewDacl))
		{
			bSuccess   =  FALSE;
			break;
		}

		// �����ļ�(��)��ȫ�����DACL�б�
		if (ERROR_SUCCESS != ::SetNamedSecurityInfo ((LPTSTR)pszPath,(SE_OBJECT_TYPE) dwType, DACL_SECURITY_INFORMATION, NULL, NULL, pNewDacl, NULL))
		{
			bSuccess   =  FALSE;
		}
	} while (FALSE);

	// �ͷ���Դ
	if (pNewDacl != NULL) LocalFree(pNewDacl);
	return bSuccess;
}