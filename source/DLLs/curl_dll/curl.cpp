#include "StdAfx.h"
#define urlLinkage __declspec (dllexport)
#include "curl.h"
#include "../../lib/filesys.h"

int strmin(int i1, int i2);

CUrl::CUrl()
{
}

CUrl::CUrl(char* szUrl)
{
	Setup(szUrl);
}

CUrl::CUrl(CString szUrl)
{
	Setup(szUrl.GetBuffer(szUrl.GetLength()));
}

CUrl::CUrl(CUrl& url)
{
	Setup(&url);
}

CUrl::CUrl(CUrl* url)
{
	Setup(url);
}

CUrl::~CUrl()
{
}

CUrl CUrl::operator=(CUrl url)
{
	Setup(&url);
	return *this;
}

CUrl CUrl::operator=(char* szUrl)
{
	Setup(szUrl);
	return *this;
}

void CUrl::Setup(char* szUrl)
{
	SetURL(szUrl);
}

void CUrl::Setup(CUrl* url)
{
	protocol=url->protocol;
	port=url->port;
	szBookmark=url->szBookmark;
	szFile=url->szFile;
	szHost=url->szHost;
	szPath=url->szPath;
	GETParams=url->GETParams;
}

void CUrl::SetURL(char *szUrl)
{
	CString obj;
	if(AfxParseURL(szUrl, protocol, szHost, obj, port))
	{
		int qSign=obj.Find('?');
		int cSign=obj.Find('#');

		CString path;
		if(strmin(qSign, cSign)!=-1)
			path=obj.Left(strmin(qSign, cSign));
		else
			path=obj;

		int sSign=path.ReverseFind('/');
		szPath=path.Left(++sSign);
		szFile=path.Mid(sSign);

		szPath.Replace("../","");
		szPath.Replace("./","");
	
		if((cSign+1)&&(qSign+1))
			if(cSign<qSign)
			{
				szBookmark=obj.Mid(cSign+1, qSign-cSign-1);
				GETParams=obj.Mid(qSign+1);
			}
			else
			{
				GETParams=obj.Mid(qSign+1, cSign-qSign-1);
				szBookmark=obj.Mid(cSign+1);
			}

		if((cSign+1)&&!(qSign+1)) szBookmark=obj.Mid(cSign+1);
		if(!(cSign+1)&&(qSign+1)) GETParams=obj.Mid(qSign+1);
	}
	else
	{
		protocol=PROTO_UNKNOWN;
		port=0;
		szBookmark="";
		szFile=szUrl;
		szHost="";
		szPath="";
		GETParams="";
	}
}

int strmin(int i1, int i2)
{
	if(i1==i2) return i1;

	if((i1+1) && !(i2+1)) return i1;
	if((i2+1) && !(i1+1)) return i2;

	if(i1>i2) return i2;
	else return i1;
}

CString CUrl::ToString()
{
	CString url;
	CString prt, gp, bm;

	if(port!=80 && protocol!=PROTO_UNKNOWN) prt.Format(":%d", port);
	if(GETParams.GetLength()) gp.Format("?%s", GETParams);
	if(szBookmark.GetLength()) bm.Format("#%s", szBookmark);

	url.Format("%s%s%s%s%s%s%s", GetProtocolByID(protocol), szHost, prt, szPath, szFile, bm, gp);

	return url;
}

CString CUrl::GetProtocolByID(int id)
{
	switch (id)
	{
	case AFX_INET_SERVICE_FTP: return CString("ftp://");
	case AFX_INET_SERVICE_HTTP: return CString("http://");
	case AFX_INET_SERVICE_HTTPS: return CString("https://");
	case AFX_INET_SERVICE_GOPHER: return CString("gopher://");
	case AFX_INET_SERVICE_FILE: return CString("file:///");
	case AFX_INET_SERVICE_MAILTO: return CString("mailto:");
	case AFX_INET_SERVICE_NEWS: return CString("news:");
	case AFX_INET_SERVICE_NNTP: return CString("nntp://");
	case AFX_INET_SERVICE_TELNET: return CString("telnet://");
	case AFX_INET_SERVICE_WAIS: return CString("wais://");
	case AFX_INET_SERVICE_MID: return CString("mid://");
	case AFX_INET_SERVICE_CID: return CString("cid://");
	case AFX_INET_SERVICE_PROSPERO: return CString("prospero://");
	case AFX_INET_SERVICE_AFS: return CString("afs://");
	default: return CString("");
	}
}

void CUrl::SetProtocol(unsigned long nProto)
{
	protocol=nProto;
}

void CUrl::SetPort(unsigned short nPort)
{
	port=nPort;
}

void CUrl::SetHost(CString nHost)
{
	szHost=nHost;
}

void CUrl::SetFile(CString nFile)
{
	szFile=nFile;
}

void CUrl::SetPath(CString nPath)
{
	szPath=nPath;
}

void CUrl::SetBookmark(CString nBookmark)
{
	szBookmark=nBookmark;
}

void CUrl::SetGETParams(CString nGETParams)
{
	GETParams=nGETParams;
}

unsigned long CUrl::GetProtocol()
{
	return protocol;
}

unsigned short CUrl::GetPort()
{
	return port;
}

CString CUrl::GetHost()
{
	return szHost;
}

CString CUrl::GetFile()
{
	return szFile;
}

CString CUrl::GetPath()
{
	return szPath;
}

CString CUrl::GetBookmark()
{
	return szBookmark;
}

CString CUrl::GetGETParams()
{
	return GETParams;
}

//Переход по ссылке
BOOL CUrl::FollowUrl(char* szUrl)
{
	CUrl tmpurl;
	CString fullurl;
	switch(CUrl::URLType(szUrl))
	{
	case L_FOREIGN:
		fullurl=szUrl;
		break;
	case L_CURRENT_B:
		fullurl.Format("%s%s%s%s%s", GetProtocolByID(protocol), szHost, szPath, szFile, szUrl);
		break;
	case L_CURRENT_G:
		fullurl.Format("%s%s%s%s%s", GetProtocolByID(protocol), szHost, szPath, szFile, szUrl);
		break;
	case L_FROMROOT:
		fullurl.Format("%s%s%s", GetProtocolByID(protocol), szHost, szUrl);
		break;
	case L_RELATIVE:
		fullurl.Format("%s%s%s%s", GetProtocolByID(protocol), szHost, szPath, szUrl);
		break;
	}
	tmpurl=CUrl(fullurl);

	Setup(&tmpurl);
	return TRUE;
}

BOOL CUrl::FollowUrl(CString url)
{
	CUrl tmpurl(url);
	return FollowUrl(url.GetBuffer(url.GetLength()));
}

//FollowURL
CUrl CUrl::operator+(char* szUrl)
{
	CUrl tmpurl(this);
	tmpurl.FollowUrl(szUrl);
	return tmpurl;
}

CUrl CUrl::operator+(CString Url)
{
	CUrl tmpurl(this);
	tmpurl.FollowUrl(Url);
	return tmpurl;
}

CUrl CUrl::operator+=(char* szUrl)
{
	FollowUrl(szUrl);
	return *this;
}

CUrl CUrl::operator+=(CString Url)
{
	FollowUrl(Url);
	return *this;
}

//ToString
CUrl::operator char*()
{
	CString url=ToString();
	strcpy(szUrl, url.GetBuffer(url.GetLength()));

	return szUrl;
}

CUrl::operator CString()
{
	return ToString();
}

//Localize
CString CUrl::Localize(char* szToLocalizeUrl)
{
	CString fullurl;

	switch(CUrl::URLType(szToLocalizeUrl))
	{
	case L_CURRENT_B:
		fullurl.Format("%s%s", szFile, szToLocalizeUrl);
		break;
	case L_CURRENT_G:
		fullurl.Format("%s%s", szFile, szToLocalizeUrl);
		break;
	case L_RELATIVE:
		fullurl=szToLocalizeUrl;
		break;
	case L_FOREIGN:
		{
			CString FileFrom="/DWNDIR/"+szHost+szPath+szFile;
			CUrl ToUrl(szToLocalizeUrl);
			CString FileTo="/DWNDIR/"+ToUrl.GetFullWithoutProtocol();

			char* szStart=FileFrom.GetBuffer(FileFrom.GetLength());
			char* szTo=FileTo.GetBuffer(FileTo.GetLength());

			if(!CFileSystem::RelativePath(szStart, szTo, &fullurl))
				fullurl=fullurl=GetPathToRoot()+"../"+CUrl(szToLocalizeUrl).GetFullWithoutProtocol();
		}
		break;
	case L_FROMROOT:
		CString FileFrom=szPath+szFile;
		char* szStart=FileFrom.GetBuffer(FileFrom.GetLength());

		if(!CFileSystem::RelativePath(szStart, szToLocalizeUrl, &fullurl))
			fullurl=GetPathToRoot()+(szToLocalizeUrl+1);

		break;
	}

	return CFileSystem::CollapseFilename(fullurl);;
}

CString CUrl::Localize(CString ToLocalizeUrl)
{
	return Localize(ToLocalizeUrl.GetBuffer(ToLocalizeUrl.GetLength()));
}

CString Localize(CUrl Url)
{
	return Localize(Url.ToString());
}

//Тип ссылки - относительная, закладка, из корня...
LINKTYPE CUrl::URLType(char* szUrl)
{
	CUrl tmpurl(szUrl);
	if(tmpurl.GetProtocol()!=AFX_INET_SERVICE_UNK) return L_FOREIGN;
	if(szUrl[0]=='/' || szUrl[0]=='\\') return L_FROMROOT;
		else
			if(szUrl[0]=='#') return L_CURRENT_B;
			else
				if(szUrl[0]=='?') return L_CURRENT_G;
				else
					return L_RELATIVE;
}

LINKTYPE CUrl::URLType(CString Url)
{
	return URLType(Url.GetBuffer(Url.GetLength()));
}

//Путь к корню закачки
CString CUrl::GetPathToRoot()
{
	char* path=szPath.GetBuffer(szPath.GetLength());
	int depth=0;
	for(int c=0;c<lstrlen(path);c++) depth+=(path[c]=='/');
	if(depth--){};

	CString rootpath;

	while(depth--) rootpath+="../";

	return rootpath;
}

//хост/путь/файл
CString CUrl::GetFilenameWithoutProtocol()
{
	CString tmp;
	tmp.Format("%s%s%s", szHost, szPath, szFile);
	
	return tmp;
}

//хост:порт/пть/файл-инфо
CString CUrl::GetFullWithoutProtocol()
{
	CString url;
	CString prt, gp, bm;

	if(port!=80) prt.Format(":%d", port);
	if(GETParams.GetLength()) gp.Format("?%s", GETParams);
	if(szBookmark.GetLength()) bm.Format("#%s", szBookmark);

	url.Format("%s%s%s%s%s%s", szHost, prt, szPath, szFile, bm, gp);

	return url;
}

BOOL CUrl::IsValidURL(char* szURL)
{
	CString server, file, tbuffer;
	DWORD type, dwnd=0;
	unsigned short port;

	return AfxParseURL(szURL,type,server,file,port);
}

BOOL CUrl::IsValidURL(CString szURL)
{
	return CUrl::IsValidURL(szURL.GetBuffer(szURL.GetLength()));
}
