#include <comutil.h>
#define hpLinkage __declspec (dllexport)
#include "hparser.h"

CHtmlElements::CHtmlElements()
{
	pElements=NULL;
}

CHtmlElements::~CHtmlElements()
{
	if(pElements) pElements->Release();
}

IHTMLElementCollection* CHtmlElements::GetCollection()
{
	return pElements;
}

VOID CHtmlElements::SetCollection(IHTMLElementCollection** ppElements)
{
	if(pElements) pElements->Release();

	pElements=*ppElements;
}

long CHtmlElements::ElementsLength()
{
	if(!pElements) return -1;

	long l;
	if(pElements->get_length(&l)==S_OK) return l;
	else return -1;
}

BOOL CHtmlElements::FindTag(char* szTagName, CHtmlElements** ppElements)
{
	if(!pElements) return FALSE;

	BOOL bValue=FALSE;

	VARIANT vTagName;
	bstr_t bsData = (LPCTSTR)szTagName;
	vTagName.vt = VT_BSTR;
	vTagName.bstrVal = (BSTR)bsData;

	bstr_t bTagName=szTagName;

	struct IHTMLElementCollection* pElements;
	IDispatch* pDisp;
	if(bValue=(this->pElements->tags(vTagName, &pDisp)==S_OK))
	{
		if(pDisp)pDisp->QueryInterface(IID_IHTMLElementCollection, (void**)&pElements);

		(*ppElements)->SetCollection(&pElements);
	}

	return bValue;
}

BOOL CHtmlElements::GetElement(long n, CHtmlElement** pElement)
{
	BOOL bValue=FALSE;

	if(pElements)
	{
		VARIANT id, index;

		V_VT(&id)=VT_I4;
		V_I4(&id)=n;
		V_VT(&index)=VT_I4;

		IDispatch* pDisp;
		IHTMLElement* pElem;

		V_I4(&index)=0;
		pElements->item(id, index, &pDisp);

		if(pDisp)
		{
			pDisp->QueryInterface(IID_IHTMLElement, (void**)&pElem);

			if(pElem)
			{
				(*pElement)->SetElement(&pElem);
				bValue=TRUE;
			}
		}
	}

	return bValue;
}
