#include <comutil.h>
#define hpLinkage __declspec (dllexport)
#include "hparser.h"

CHtmlDocument::CHtmlDocument()
{
	pDoc=NULL;
	if(CoCreateInstance(CLSID_HTMLDocument, NULL, CLSCTX_INPROC_SERVER, IID_IHTMLDOCUMENT, (void**)&pDoc)!=S_OK)
		pDoc=NULL;
}

CHtmlDocument::~CHtmlDocument()
{
	if(pDoc) pDoc->Release();
}

IHTMLDOCUMENT* CHtmlDocument::GetDocument()
{
	return pDoc;
}

VOID CHtmlDocument::SetDocument(IHTMLDOCUMENT** ppDoc)
{
	if(pDoc) pDoc->Release();

	pDoc=*ppDoc;
}

BOOL CHtmlDocument::LoadFile(char* szFilename)
{
	if(!pDoc) return FALSE;

	BOOL bValue=FALSE;
	HANDLE file=CreateFile(szFilename,GENERIC_READ,FILE_SHARE_WRITE,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if(file!=INVALID_HANDLE_VALUE)
	{
		DWORD dwBytes;
		DWORD nFileLen=GetFileSize(file, NULL);
		char* szHtml=new char[nFileLen];
		ReadFile(file, szHtml, nFileLen, &dwBytes, NULL);
		CloseHandle(file);

		bValue=LoadData(szHtml);

		delete szHtml;
	}

	return bValue;
}

BOOL CHtmlDocument::LoadData(char* buffer)
{
	if(!pDoc) return FALSE;

	BOOL bValue=FALSE;

/*	BSTR bstr = SysAllocString((const unsigned short *)buffer);

	// Creates a new one-dimensional array
	SAFEARRAY  *sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT* param;
	
	if (sfArray && pDoc)
	{
		SafeArrayAccessData(sfArray,(LPVOID*) & param);
		param->vt = VT_BSTR;
		param->bstrVal = bstr;
		SafeArrayUnaccessData(sfArray);
		if(bValue=(pDoc->write(sfArray)==S_OK))
			pDoc->close();
	}
*/
	SAFEARRAY* psa = SafeArrayCreateVector(VT_VARIANT, 0, 1);
	VARIANT *param;
	_bstr_t bsData = (LPCTSTR)buffer;
	SafeArrayAccessData(psa, (LPVOID*)&param);
	param->vt = VT_BSTR;
	param->bstrVal = (BSTR)bsData;
	SafeArrayUnaccessData(psa);

	if(bValue=(pDoc->write(psa)==S_OK))
		pDoc->close();


	return bValue;
}

BOOL CHtmlDocument::GetAll(CHtmlElements** ppElements)
{
	if(!pDoc) return FALSE;

	BOOL bValue=FALSE;

	struct IHTMLElementCollection* pElements;
	if(bValue=(pDoc->get_all(&pElements)==S_OK))
		(*ppElements)->SetCollection(&pElements);

	return bValue;
}
