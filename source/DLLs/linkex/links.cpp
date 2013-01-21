#include "stdafx.h"
#include "sgml.h"
#define CLinkage __declspec (dllexport)
#include "links.h"

#define BLOCK_ELEMENT eBlockedEl
#define SINGLE_ELEMENT eSingleEl

bool WritableTag(const char *tagName);
CStrings extract(CSgmlEdit* el, const char* tagName, const char* paramName, ElType type);

CLinkage CStrings extractLinks(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_A, htmlText);
}

CLinkage CStrings extractApplets(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_APPLET, htmlText);
}

CLinkage CStrings extractForms(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_FORM, htmlText);
}

CLinkage CStrings extractPictures(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_IMG, htmlText);
}

CLinkage CStrings extractEmbedded(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_EMBED, htmlText);
}

CLinkage CStrings extractFrames(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_FRAME, htmlText);
}

CLinkage CStrings extractInlineFrames(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_IFRAME, htmlText);
}

CLinkage CStrings extractXML(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_XML, htmlText);
}

CLinkage CStrings extractSounds(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_BGSOUND, htmlText);
}

CLinkage CStrings extractLinked(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_LINK, htmlText);
}

CLinkage CStrings extractTablesBgr(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_TABLE, htmlText);
}

CLinkage CStrings extractTableLinesBgr(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_TD, htmlText);
}

CLinkage CStrings extractTableHLinesBgr(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_TH, htmlText);
}

CLinkage CStrings extractAreas(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_AREA, htmlText);
}

CLinkage CStrings extractBodyBgr(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_BODY, htmlText);
}

CLinkage CStrings extractDynamicImage(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return extractElements(HTML_BODY, htmlText);
}

CLinkage CStrings extractAll(const char* htmlText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CStrings sa;

	CSgmlEdit tree;
	tree.parse(htmlText, strlen(htmlText));

	sa.Append(extract(&tree,  "a", "href", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "applet", "code", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "form", "action", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "img", "src", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "embed", "src", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "frame", "src", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "iframe", "src", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "xml", "src", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "bgsound", "src", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "input", "src", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "link", "href", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "table", "background", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "th", "background", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "td", "background", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "area", "href", SINGLE_ELEMENT));
	sa.Append(extract(&tree,  "body", "background", BLOCK_ELEMENT));
	sa.Append(extract(&tree,  "img", "dynsrc", SINGLE_ELEMENT));

	return sa;
}

CLinkage CStrings extractElements(int HTMLTag, const char* htmlText)
{
	if(!htmlText)
	{
		CStrings null;
		return null;
	}

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CSgmlEdit tree;
	tree.parse(htmlText, strlen(htmlText));

	switch(HTMLTag)
	{
	case HTML_A:
		return extract(&tree,  "a", "href", BLOCK_ELEMENT);
		break;
	case HTML_APPLET:
		return extract(&tree,  "applet", "code", BLOCK_ELEMENT);
		break;
	case HTML_FORM:
		return extract(&tree,  "form", "action", BLOCK_ELEMENT);
		break;
	case HTML_IMG:
		return extract(&tree,  "img", "src", SINGLE_ELEMENT);
		break;
	case HTML_EMBED:
		return extract(&tree,  "embed", "src", SINGLE_ELEMENT);
		break;
	case HTML_FRAME:
		return extract(&tree,  "frame", "src", SINGLE_ELEMENT);
		break;
	case HTML_IFRAME:
		return extract(&tree,  "iframe", "src", BLOCK_ELEMENT);
		break;
	case HTML_XML:
		return extract(&tree,  "xml", "src", BLOCK_ELEMENT);
		break;
	case HTML_BGSOUND:
		return extract(&tree,  "bgsound", "src", SINGLE_ELEMENT);
		break;
	case HTML_INPUT:
		return extract(&tree,  "input", "src", SINGLE_ELEMENT);
		break;
	case HTML_LINK:
		return extract(&tree,  "link", "href", SINGLE_ELEMENT);
		break;
	case HTML_TABLE:
		return extract(&tree,  "table", "background", BLOCK_ELEMENT);
		break;
	case HTML_TH:
		return extract(&tree,  "th", "background", BLOCK_ELEMENT);
		break;
	case HTML_TD:
		return extract(&tree,  "td", "background", BLOCK_ELEMENT);
		break;
	case HTML_AREA:
		return extract(&tree,  "area", "href", SINGLE_ELEMENT);
		break;
	case HTML_BODY:
		return extract(&tree,  "body", "background", BLOCK_ELEMENT);
		break;
	case HTML_IMG_DYNSRC:
		return extract(&tree,  "img", "dynsrc", SINGLE_ELEMENT);
		break;
	default:
		CStrings null;
		return null;
	}
}

CStrings extract(CSgmlEdit* el, const char* tagName, const char* paramName, ElType type)
{
	CStrings sa;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	int i=0;

	do
	{
		if(el->gettype()!=eBaseEl)
		{
			if(el->name)
			{
				if(!_stricmp(el->name, tagName)&&(type==SINGLE_ELEMENT||el->gettype()==type)&&el->GetChrParam((char*)paramName))
				{
					try	
					{
						CString newE;
						newE.Format("%s",el->GetChrParam((char*)paramName));
						sa.Add(newE);
						i++;
					}
					catch(CMemoryException*)
					{
						return sa;
					}
				}
			}

			if (!el || el->gettype() == eBlockedEl && el->name) 
				if(el->child()&&WritableTag(el->name))
					{
						el=(CSgmlEdit *)el->child();
//						const CStrings sa2;
						sa.Append(extract(el, tagName, paramName, type));
/*						if(n==-1) return -1;
						else
						{
							i+=n;
							((CStrings*)sa)->Append(sa2);
						}
*/						el=(CSgmlEdit *)el->parent();
					}
		}
	}while(el=(CSgmlEdit *)el->next());

	return sa;
}


bool WritableTag(const char *tagName)
{
	//Здесь их нет
	if(!_stricmp(tagName, "title")) return false;
	if(!_stricmp(tagName, "script")) return false;
	if(!_stricmp(tagName, "style")) return false;
	if(!_stricmp(tagName, "comment")) return false;

	//В других местах есть
	return true;
}

CStrings::CStrings (const CStrings& stringSrc)
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
	Copy(stringSrc);
}

CStrings::CStrings (const CStringArray& stringSrc)
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
	Copy(stringSrc);
}

CStrings::CStrings()
{
	m_pData = NULL;
	m_nSize = m_nMaxSize = m_nGrowBy = 0;
}

CStrings& CStrings::operator=(const CStrings& stringSrc)
{
	Copy(stringSrc);
	return *this;
}

CStrings& CStrings::operator=(const CStringArray& stringSrc)
{
	Copy(stringSrc);
	return *this;
}
