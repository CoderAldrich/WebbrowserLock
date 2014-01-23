#ifndef SYSTEM_PLUGIN_CONFIG_H
#define SYSTEM_PLUGIN_CONFIG_H

//////////////////////////////////////////////////////////////////////////
// ��������ͷ�ļ�

#include <UtilFunc/PluginInc.h>


#include <Interfaces/Config/IXmlElement.h>

//////////////////////////////////////////////////////////////////////////
// �����ⲿͷ�ļ�

#include <Function.Internal.CommonControl/CommonControlTool.h>

//////////////////////////////////////////////////////////////////////////
// ����ȫ�ֺ�<Guid("EE590B91-A4A5-485E-AD55-798E040A0168")>
static const X3CLSID ClsidConfigdata("EE590B91-A4A5-485E-AD55-798E040A0168");

//////////////////////////////////////////////////////////////////////////
// API����

class CXmlElement
	: public IXmlElement, CCommonControl
{
	X3BEGIN_CLASS_DECLARE(CXmlElement)
		X3DEFINE_INTERFACE_ENTRY(IXmlElement)
		X3END_CLASS_DECLARE()
		//REGISTER_PLUGIN_INFO(L"ϵͳ��Ϣͳ�Ʋ��",L"V1.1 Bate",L"2013-02-06 12:41:00",L"�ҵĹ�˾",L"��Ȩ����")
protected:
	CXmlElement(TiXmlElement * pXmlElement);
	virtual ~CXmlElement();
	

    //! Get a child node of the root node.
    /*!
        \return the child node (Cx_ConfigSection). The node will be created when not exist and autoCreate is true.
        \param name The child node name.\n
            The name may contains slash chars for multiple levels.\n
            Empty string is represented as getting the root node.
        \param autoCreate true if enable to auto create new node.
    */
    virtual Cx_Ptr GetSection(
        const wchar_t* name, bool autoCreate = true) = 0;

protected:
	TiXmlElement * m_pXmlElement;
};

#endif