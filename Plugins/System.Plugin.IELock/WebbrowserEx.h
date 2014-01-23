// WebbrowserEx.h : CWebbrowserEx ������

#pragma once
#include "resource.h"       // ������



#include "SystemPluginIELock_i.h"


#include "exdispid.h"  
#include "shlguid.h"  


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;


// CWebbrowserEx

class ATL_NO_VTABLE CWebbrowserEx :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebbrowserEx, &CLSID_WebbrowserEx>,
	public IObjectWithSiteImpl<CWebbrowserEx>,
	public IDispatchImpl<IWebbrowserEx, &IID_IWebbrowserEx, &LIBID_SystemPluginIELockLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IDispEventImpl<1,CWebbrowserEx,&DIID_DWebBrowserEvents2,&LIBID_SHDocVw,1,1>   //����������IDispEventImpl
{
public:
	CWebbrowserEx()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WEBBROWSEREX)


BEGIN_COM_MAP(CWebbrowserEx)
	COM_INTERFACE_ENTRY(IWebbrowserEx)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()

	DECLARE_PROTECT_FINAL_CONSTRUCT()


	
/* 
 * SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE, OnDocumentComplete)
 * IDispEventImpl���¼�·�ɱ���Ϲ��������Խ��¼�·�ɵ���Ӧ�Ĵ��������
 * SINK_ENTRY_EX(1,...)�е�"1"��ӿ������е�IDispEventImpl<1,....>�Ƕ�Ӧ��,�ڱ�Ҫʱ���������������Բ�ͬ�ӿڵ��¼�
 */
	BEGIN_SINK_MAP(CWebbrowserEx)   
		//SINK_ENTRY_EX(1, DIID_DWebBrowserEvents2, DISPID_DOCUMENTCOMPLETE , OnDocumentComplete) 
	END_SINK_MAP()  



	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	STDMETHOD(SetSite)(/*[in]*/ IUnknown *pUnkSite);
	STDMETHOD(Invoke)(DISPID, REFIID, LCID, WORD, DISPPARAMS*, VARIANT*, EXCEPINFO*, UINT*);

private:
	CComPtr<IWebBrowser2> m_spWebBrowser;//����Browserָ���˽�б���  

	BOOL m_fAdvised;   //�¼�ӳ��Ĵ������ 
	LONGLONG m_StartTime;
	IUnknown* m_spWebBrowser2;

};

OBJECT_ENTRY_AUTO(__uuidof(WebbrowserEx), CWebbrowserEx)
