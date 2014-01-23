#if !defined(__SMARTDB_)
#define __SMARTDB_

#if _MSC_VER > 1000
#pragma once
#endif

// SmartDB.h : header file
//

#define ASSERT

#define RSOPEN_SUCCESS			0
#define RSOPEN_NOLIBLOADED		1
#define RSOPEN_NOCONNECT		2
#define RSOPEN_ALREADYOPENED	3
#define RSOPEN_INVALIDQRY		4

#include <vector>
#include <string>
#include "sqlite3.h"
#include <Function.Internal.CommonControl\CommonControlTool.h>

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CSmartDB thread

class CSmartDB : CCommonControl
{
	// Attributes
	public:
		CSmartDB();           // constructor and distructor
		~CSmartDB();

	// Operations
	public:
		BOOL Connect (LPCTSTR strDBPathName, LPCTSTR strParamString = NULL);
		BOOL InitInstance();
		BOOL Close (void);
		BOOL IsConnected (void);
		UINT Execute (LPCTSTR strQuery, UINT * nRecEfected = NULL);
		BOOL IsLibLoaded (void);

		sqlite3 *db;

	// Implementation
	private:
		BOOL InitLibrary(void);

		BOOL m_bLibLoaded;
		BOOL m_bConnected;
};


/////////////////////////////////////////////////////////////////////////////
// CSmartDBRecordSet thread

class CSmartDBRecordSet : CCommonControl
{
public:
	static LPCTSTR Type[];
	// Attributes
	public:
		CSmartDBRecordSet();           // protected constructor used by dynamic creation
		virtual ~CSmartDBRecordSet();

	// Operations
	public:
		BOOL Close (void);
		UINT Open (LPCTSTR strQueryString, CSmartDB * ptrConn, LONG nRSType = NULL);
		BOOL InitInstance();



		//////////////////////////////////////////////////////////////////////////
		//	Explain: �õ������� FieldsCount (void)
		//	Return: ���ص�ǰ�����е�������

		UINT FieldsCount (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: �õ������� GetFieldName (LONG)
		//	Parameter:
		//		nIndex ȡ�����Ƶ�������
		//	Return: �����е�����

		LPCTSTR GetFieldName (LONG nIndex);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: �õ����������� GetFieldType (LONG)
		//	Parameter:
		//		nIndex ȡ���������͵�������
		//	Return: ����������������

		INT GetFieldType (LONG nIndex);



		
		//////////////////////////////////////////////////////////////////////////
		//	Explain: �õ���¼���� RecordCount (void)
		//	Return: ���ص�ǰ��¼��������

		LONG RecordCount (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: ת����һ����¼ MoveNext (void)
		//	Return: �޷���

		void MoveNext (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: ת����һ����¼ MovePrevious (void)
		//	Return: �޷���

		void MovePrevious (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: ת�����һ����¼ MoveLast (void)
		//	Return: �޷���

		void MoveLast (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: ת����һ����¼ MoveFirst (void)
		//	Return: �޷���

		void MoveFirst (void);




		//////////////////////////////////////////////////////////////////////////
		//	Explain: �ж��Ƿ���� IsEOF (void)
		//	Return: ����������,�������ؼ�

		BOOL IsEOF (void);

		//////////////////////////////////////////////////////////////////////////
		//	Explain: �õ���ǰ��¼ָ���е����� GetColumnString (LONG)
		//	Parameter:
		//		nIndex ȡ�����ݵ�������
		//	Return: �����е�����

		LPCTSTR GetColumnString (LONG nIndex);

	// Implementation
	private:
		BOOL m_bIsOpen;
		vector<string>  m_strFieldsList;
		vector<int> m_nFieldsType;
		vector<vector<string> * > orsRows;
		LONG m_nFieldsCount;
		BOOL m_bIsProcessing;
		LONG m_nCurrentPage;
		LONG m_nPageCount;
		LONG m_nPageSize;
		LONG m_nCurrentRecord;
		LONG m_nRecordCount;

#ifdef UNICODE
		wstring m_strRet;
#else
		string m_strRet;
#endif
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__SMARTDB_)
