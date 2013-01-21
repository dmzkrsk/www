// www.cpp : Defines the entry point for the console application.
//

#include "StdAfx.h"
#include "www.h"
#include "lib/cmdparser.h"
#include "lib/filesys.h"
#include <shlwapi.h>

#include <math.h>

#ifdef _DEBUG

#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	InitializeCriticalSection(&flush_cs);
	int nRetCode = 0;

	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: change error code to suit your needs
		cerr << _T("Fatal Error: MFC initialization failed") << endl;
		nRetCode = 1;
	}
	else
	{
		PWD_STRUCT pwd;
		BOOL EnableThreadControl=FALSE;
		BOOL EnableContentControl=TRUE;
		BOOL EnableStat=FALSE;
		BOOL EnableCustomDirectory=FALSE;
		int threads=MAX_THREADS;
		CString Directory;
		char* ClientName;

		CString pinfo;
		pinfo.Format("\n\n%s %s (Released %s) by %s.\n\tE-mail: %s\n%s\n", PRNAME, PRVERSION, PRDATE, PRAUTHOR, PREMAIL, PRADD);
		fOut(pinfo);
#ifdef _DEBUG
		fErr("\n!!DEBUG VERSION!!\n");
#endif
		InitTypes();
		int plus_flags=0;

		CMIMETypes cmtMIME;
		cmtMIME.Load("mime.cfg");

		CCmdParser parser;
		parser.LoadLine(argc, argv);
		parser.Parse();

		if(parser.GetParam("?") || parser.GetParam("h") || parser.GetParam("help"))
		{
			ShowHelp();
			DeleteCriticalSection(&flush_cs);
			return 0;
		}

		if(threads=parser.GetOneOfIntParam("thread"))
		{
			EnableThreadControl=TRUE;
			printf("Thread control enabled\n");
		}
		else threads=MAX_THREADS;

		pwd.EnableAuth=FALSE;
		char *log, *pswd;
		if(log=parser.GetOneOfParam("user"))
		{
			pwd.log=log;
			pwd.EnableAuth=TRUE;
			if(pswd=parser.GetOneOfParam("password"))
				pwd.password=pswd;
		}
		else
		{
			pwd.log="anonymous";
			pwd.password="user@usermail.com";
		}

		pwd.EnableProxyAuth=FALSE;
		char *proxyuser, *proxypwd;
		if(proxyuser=parser.GetParam("proxyuser"))
		{
			pwd.proxyUser=proxyuser;
			pwd.EnableProxyAuth=TRUE;
			if(proxypwd=parser.GetParam("proxypassword"))
				pwd.proxyPassword=proxypwd;
		}
		
		char* szProxy=parser.GetParam("proxy");
		bool szDirect=parser.IsParamExists("direct");
		if(szProxy && szDirect)
		{
			fErr("Please select between proxy and direct connection!\n");
			DeleteCriticalSection(&flush_cs);
			return 0;
		}

		DWORD dwAccess=CDUrl::accessPreConfig;
		if(szProxy)
		{
			dwAccess=CDUrl::accessProxy;
			printf("Making connection through %s\n", szProxy);
		}
		else
			if(szDirect)
			{
				dwAccess=CDUrl::accessDirect;
				printf("Making direct internet connection\n");
			}
			else printf("Using standart settings for internet connection\n");

		if(pwd.EnableAuth) printf("Authorization enabled\n");
		if(pwd.EnableProxyAuth) printf("Proxy authorization enabled\n");
		else printf("Using anonymous connection\n");

		char* dir;
		if(dir=parser.GetParam("dir"))
		{
			Directory=dir;
			EnableCustomDirectory=TRUE;
		}
		else Directory="Download";

		char endc=Directory[Directory.GetLength()-1];
		if(endc!='\\' && endc!='/') Directory+='\\';
	
		printf("Using directory %s as download root\n", Directory);

		if(ClientName=parser.GetOneOfParam("name"))
		{
			if(!strcmp(ClientName,"__msie__")) ClientName=MSIE_NAME;
			if(!strcmp(ClientName,"__navigator__")) ClientName=NSNV_NAME;
			if(!strcmp(ClientName,"__opera__")) ClientName=OPER_NAME;
		}
		else ClientName=CLIENT_NAME;

		printf("Start downloading as %s\n", ClientName);

		if(parser.GetParam("s")) EnableStat=TRUE;
		if(parser.GetParam("d"))
		{
			EnableContentControl=FALSE;
			printf("!!Content Control disabled\n");
		}

		char* dfile;

		printf("\n");

		CUrlManager cumStarter(ClientName, Directory, threads, dwAccess, szProxy, EnableContentControl?&cmtMIME:NULL, &pwd);
		cumStarter.SetOutputs(fOut, fErr);

		if(dfile=parser.GetParam("list"))
		{
			if(lstrlen(dfile))
			{
				printf("Starting miltidownloading using file %s\n", dfile);

				cumStarter.RunTimer();
				CStrings csLines;
				CFileSystem::GetFile(dfile, &csLines);
				cumStarter.StartWithList(csLines);
				cumStarter.StopTimer();
				if(EnableStat) cumStarter.ShowStat();
				TRACE ("Work is done!\n");
			}
			else fErr("Please input URL with 'list' (Did you forget '='?)!\n");

			DeleteCriticalSection(&flush_cs);
			return 0;
		}

		if(dfile=parser.GetParam("all"))
		{
			if(lstrlen(dfile))
			{
				printf("Downloading all from %s\n", dfile);
				cumStarter.RunTimer();
				cumStarter.StartWithUrl(CUrl(dfile));
				cumStarter.ContinueAll();
				cumStarter.StopTimer();
				if(EnableStat) cumStarter.ShowStat();
				TRACE ("Work is done!\n");
			}
			else fErr("Please input URL with 'all' (Did you forget '='?)!\n");

			DeleteCriticalSection(&flush_cs);
			TRACE ("Gonna shutdown app now!\n");
			return 0;
		}

		CWordArray cwaTypes;
		if(dfile=parser.GetParam(1))
		{
			for(int i=0;i<DO_COUNT;i++)
			{
				if(parser.IsParamExists(objects[i].name))
					cwaTypes.Add(objects[i].Tag);
			}

			if(cwaTypes.GetSize())
			{
				printf("Downloading different objects from %s\n", dfile);
				cumStarter.RunTimer();
				cumStarter.StartWithUrl(CUrl(dfile));
				cumStarter.ContinueWith(cwaTypes);
				cumStarter.StopTimer();
				if(EnableStat) cumStarter.ShowStat();
				DeleteCriticalSection(&flush_cs);
				return 0;
			}
		}
		else fErr("Please input starting URL!\n");

		int i;

		CStrings elements;
		int n;
		for(i=0,n=0;i<DO_COUNT;i++)
			if(parser.GetParam(objects[i].name))
				elements.Add(objects[i].name);

		if(elements.GetSize())
		{
//			CreateThreadSemaphore(threads);
//			DownloadTypes(elements, parser.GetParam(1));
			DeleteCriticalSection(&flush_cs);
			return 0;
		}

		char *from=parser.GetParam(1);
		char *to=parser.GetParam(2);

		if(from)
		{
			StartFile(CUrl(from), Directory.GetBuffer(Directory.GetLength()), to, dwAccess, szProxy, &cmtMIME, &pwd);
			DeleteCriticalSection(&flush_cs);
			return 0;
		}

		fErr("Wrong number of parameters");
	}

	DeleteCriticalSection(&flush_cs);
	return nRetCode;
}

int StartFile(CUrl url, char* szDownloadRoot, char* file, DWORD dwAccess, char* szProxyName, CMIMETypes* pcmtMIME, PWD_STRUCT* ppsPwd)
{
	if(!szDownloadRoot) return -1;

	CDUrl cduUrl(url, (CDUrl::AccessTypes)dwAccess);
	if(dwAccess==CDUrl::accessProxy) cduUrl.SetProxy(szProxyName);
	cduUrl.SetOutputs(fOut, fErr);
	if(file)
	{
		cduUrl.SetPath(".\\");
		cduUrl.SetFileName(file);
	}
	else
	{
		cduUrl.SetPath(szDownloadRoot);
		cduUrl.GenerateFilename();
	}
	if(pcmtMIME) cduUrl.SetMIME(pcmtMIME);
	if(ppsPwd) cduUrl.SetRights(ppsPwd);

	cduUrl.DownloadToFile();
	return 0;
}

void ShowHelp()
{
	if((int)ShellExecute(NULL, "open", "help\\wcd_help.chm",  NULL, NULL,SW_SHOW )<=32)
		fErr("Cannot show help file!\n");
}

void InitTypes()
{
	//Don't forget increase DO_COUNT when Add new element!!;

	objects[0].Set("links", HTML_A);
	objects[1].Set("applets", HTML_APPLET);
	objects[2].Set("formactions", HTML_FORM);
	objects[3].Set("images", HTML_IMG);
	objects[4].Set("embeded", HTML_EMBED);
	objects[5].Set("frameparts", HTML_FRAME);
	objects[6].Set("iframes", HTML_IFRAME);
	objects[7].Set("xmls", HTML_XML);
	objects[8].Set("sounds", HTML_BGSOUND);
	objects[9].Set("inputimages", HTML_INPUT);
	objects[10].Set("rels", HTML_LINK);
	objects[11].Set("bodybgs", HTML_BODY);
	objects[12].Set("tablebgs", HTML_TABLE);
	objects[13].Set("thbgs", HTML_TH);
	objects[14].Set("tdbgs", HTML_TD);
	objects[15].Set("arealinks", HTML_AREA);
	objects[16].Set("movies", HTML_IMG_DYNSRC);

}

bool OneOfDownloadableTypes(char *param, int* type)
{
	for(int i=0;i<DO_COUNT;i++)
		if(!stricmp(param,  objects[i].name))
		{
			if(type) *type=objects[i].Tag;
			return true;
		}

	return 0;
}

void fErr(CString text)
{
		EnterCriticalSection(&flush_cs);
		CONSOLE_SCREEN_BUFFER_INFO ConsoleInfo;
		HANDLE hConsole=GetStdHandle(STD_OUTPUT_HANDLE); 
		GetConsoleScreenBufferInfo(hConsole, &ConsoleInfo);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED|FOREGROUND_INTENSITY);
		fprintf(stderr, "%s",text.GetBuffer(text.GetLength()));
		SetConsoleTextAttribute(hConsole, ConsoleInfo.wAttributes);
		LeaveCriticalSection(&flush_cs);
}

void fOut(CString text)
{
		EnterCriticalSection(&flush_cs);
		fprintf(stdout, "%s",text.GetBuffer(text.GetLength()));
		LeaveCriticalSection(&flush_cs);
}
