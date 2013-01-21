#ifndef hpLinkage
#define hpLinkage __declspec (dllimport)
#endif

#include <comdef.h>
#include <mshtml.h>

#define IHTMLDOCUMENT IHTMLDocument2
#define IID_IHTMLDOCUMENT IID_IHTMLDocument2

class hpLinkage CHtmlParser
{
private:
	char** ppValues;
	long lSize;

public:
	void ClearEmpty();
	CHtmlParser();
	~CHtmlParser();

	BOOL ParseFile(char* szFileName, char* szTagName, char* szAttrName);
	BOOL Parse(char* szHtmlBuffer, char* szTagName, char* szAttrName);
	long GetSize();
	char* GetAttribute(long lNum);

private:
	void Clear();
	void AllocPP(long newSize);
	void DeletePP();
	void AllocP(long l, int nSize);
	void DeleteP(long l);
};

class hpLinkage CHtmlElement
{
private:
	IHTMLElement* pElement;
public:
	CHtmlElement();
	~CHtmlElement();

	IHTMLElement* GetElement();
	VOID SetElement(IHTMLElement** ppElement);

	int GetStringAttribute(const char* szAttributeName, char* Attr=NULL);
};

class hpLinkage CHtmlElements
{
private:
	IHTMLElementCollection* pElements;

public:
	CHtmlElements();
	~CHtmlElements();

	IHTMLElementCollection* GetCollection();
	VOID SetCollection(IHTMLElementCollection** ppElements);

	BOOL FindTag(char* szTagName, CHtmlElements** ppElements);
	long ElementsLength();
	BOOL GetElement(long n, CHtmlElement** pElement);
};

class hpLinkage CHtmlDocument
{
private:
	IHTMLDOCUMENT *pDoc;

public:
	CHtmlDocument();
	~CHtmlDocument();

	IHTMLDOCUMENT* GetDocument();
	VOID SetDocument(IHTMLDOCUMENT** ppDoc);
	BOOL LoadFile(char* szFilename);
	BOOL LoadData(char* buffer);
	BOOL GetAll(class CHtmlElements** ppElements);
};
