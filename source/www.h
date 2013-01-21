#if !defined(AFX_WWW_H__50C140E1_20ED_4AF1_AF53_459D9C7D7561__INCLUDED_)
#define AFX_WWW_H__50C140E1_20ED_4AF1_AF53_459D9C7D7561__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

#endif // !defined(AFX_WWW_H__50C140E1_20ED_4AF1_AF53_459D9C7D7561__INCLUDED_)

#include "lib/murl.h"
//#include "lib/links.h"
#include "lib/mime.h"
#include "lib/urld.h"

#pragma warning(disable:4786)

#ifdef _DEBUG
#include <conio.h>
#endif

#define PRNAME "WWW Console Downloader"
#define PRVERSION "v2.0 alpha.2 (www++)"
#define PRDATE "Oct 31, 2003"
#define PRAUTHOR "Dead_Morozz"
#define PREMAIL "hellas@krasu.ru, trofimov@kspu.ru"
#define PRADD "This program uses RegExp library by Igor Ruskih. Read help for more info.\n"

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

using namespace std;

//Downloadable objects;
typedef struct
{
	char name[32];
	int Tag;
	void Set(char* nname, int tTag)
	{
		strcpy(name, nname);
		Tag=tTag;
	}
}DOBJECT;

//Thread variables;

#define DO_COUNT 17
DOBJECT objects[DO_COUNT];

CRITICAL_SECTION flush_cs;

int StartFile(CUrl url, char* szDownloadRoot, char* file, DWORD dwAccess, char* szProxyName, CMIMETypes* pcmtMIME, PWD_STRUCT* ppsPwd);

void ShowHelp();
void InitTypes();
bool OneOfDownloadableTypes(char *param, int* type=NULL);

void fErr(CString text);
void fOut(CString text);

typedef int(*START_FUNC)(char*);
