#include <comutil.h>
#define hpLinkage __declspec (dllexport)
#include "hparser.h"

CHtmlElement::CHtmlElement()
{
	pElement=NULL;
}

CHtmlElement::~CHtmlElement()
{
	if(pElement)pElement->Release();
}

IHTMLElement* CHtmlElement::GetElement()
{
	return pElement;
}

VOID CHtmlElement::SetElement(IHTMLElement** ppElement)
{
	if(pElement)pElement->Release();
		
	pElement=*ppElement;
}

int CHtmlElement::GetStringAttribute(const char* szAttributeName, char* buffer)
{
	if(!pElement) return -1;

	VARIANT vval;
	V_BSTR(&vval);

	bstr_t bstrName = szAttributeName;
	UINT strl=-1;
	if(pElement->getAttribute(bstrName, 2, &vval)==S_OK)
		if(vval.vt==VT_BSTR)// && vval.pbstrVal)
		{
			_bstr_t tmpbstr=vval;
			strl=SysStringLen(tmpbstr);

			if(buffer)strcpy(buffer, (char*)tmpbstr);
		}
	
	return strl;
}
