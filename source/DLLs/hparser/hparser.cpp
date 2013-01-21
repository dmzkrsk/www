#include <comutil.h>
#define hpLinkage __declspec (dllexport)
#include "hparser.h"

CHtmlParser::CHtmlParser()
{
	ppValues=NULL;
	AllocPP(0);
}

CHtmlParser::~CHtmlParser()
{
	DeletePP();
}

BOOL CHtmlParser::ParseFile(char* szFileName, char* szTagName, char* szAttrName)
{
	BOOL bValue=FALSE;
	HANDLE file=CreateFile(szFileName,GENERIC_READ,FILE_SHARE_WRITE,NULL,OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL,NULL);
	if(file!=INVALID_HANDLE_VALUE)
	{
		DWORD dwBytes;
		DWORD nFileLen=GetFileSize(file, NULL);
		char* szHtml=new char[nFileLen];
		ReadFile(file, szHtml, nFileLen, &dwBytes, NULL);
		CloseHandle(file);

		bValue=Parse(szHtml, szTagName, szAttrName);

		delete szHtml;
	}
	return bValue;
}

BOOL CHtmlParser::Parse(char* szHtmlBuffer, char* szTagName, char* szAttrName)
{
	CHtmlDocument *doc=new CHtmlDocument;

	if(!doc->LoadData(szHtmlBuffer))
	{
		delete doc;
		return FALSE;
	}

	CHtmlElements* els=new CHtmlElements;

	if(!doc->GetAll(&els))
	{
		delete doc;
		delete els; 
		return FALSE;
	}

	CHtmlElements* links=new CHtmlElements;
	if(!els->FindTag(szTagName, &links))
	{
		delete links;
		delete doc;
		delete els; 
		return FALSE;
	}
	
	long count=links->ElementsLength();
	long lNum=0;
	AllocPP(count);

	CHtmlElement *el=new CHtmlElement;
	for(long i=0;i<count;i++)
	{
		if(links->GetElement(i, &el))
		{
			long lAttr=el->GetStringAttribute(szAttrName);
			if(lAttr>=0)
			{
				AllocP(lNum,lAttr+1);
				el->GetStringAttribute(szAttrName, ppValues[lNum++]);
			}
		}
	}

	Clear();
	
	delete el;
	delete links;
	delete els;
	delete doc;

	return TRUE;
}

long CHtmlParser::GetSize()
{
	return lSize;
}

char* CHtmlParser::GetAttribute(long lNum)
{
	if(ppValues && lSize>lNum)
		return ppValues[lNum];
	else
		return NULL;
}

void CHtmlParser::AllocPP(long newSize)
{
	if(ppValues) DeletePP();

	ppValues=new char*[newSize];

	for(lSize=0;lSize<newSize;lSize++)
		ppValues[lSize]=NULL;
}

void CHtmlParser::DeletePP()
{
	while(lSize--) DeleteP(lSize);
	if(ppValues) delete ppValues;

}

void CHtmlParser::AllocP(long l, int nSize)
{
	if(ppValues && lSize>l)
		ppValues[l]=new char[nSize];
}

void CHtmlParser::DeleteP(long l)
{
	if(ppValues) delete ppValues[l];
}


void CHtmlParser::Clear()
{
	for(long l=lSize-1;l>=0 && !ppValues[l];l--);
	lSize=l+1;
}

void CHtmlParser::ClearEmpty()
{
	long i,j;

	for(i=0;i<lSize;i++)
		for(j=i+1;j<lSize;j++)
			if(strlen(ppValues[i])<strlen(ppValues[j]))
			{
				char* tmp=ppValues[i];
				ppValues[i]=ppValues[j];
				ppValues[j]=tmp;
			}

	for(long l=lSize-1;l>=0 && ppValues[l];l--)
	{
		if(strlen(ppValues[l])) break;
		DeleteP(l);
	}
	lSize=l+1;
}
