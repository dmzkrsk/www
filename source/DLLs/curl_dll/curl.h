#ifndef urlLinkage
#define urlLinkage __declspec (dllimport)
#endif

#include <afxinet.h>

#define PROTO_FTP AFX_INET_SERVICE_FTP
#define PROTO_HTTP AFX_INET_SERVICE_HTTP
#define PROTO_HTTPS AFX_INET_SERVICE_HTTPS
#define PROTO_GOPHER AFX_INET_SERVICE_GOPHER
#define PROTO_FILE AFX_INET_SERVICE_FILE
#define PROTO_MAIL AFX_INET_SERVICE_MAILTO
#define PROTO_NEWS AFX_INET_SERVICE_NEWS
#define PROTO_NNTP AFX_INET_SERVICE_NNTP
#define PROTO_TELNET AFX_INET_SERVICE_TELNET
#define PROTO_WAIS AFX_INET_SERVICE_WAIS
#define PROTO_MIDI AFX_INET_SERVICE_MID
#define PROTO_CID AFX_INET_SERVICE_CID
#define PROTO_PROSPERO AFX_INET_SERVICE_PROSPERO
#define PROTO_AFS AFX_INET_SERVICE_AFS
#define PROTO_UNKNOWN AFX_INET_SERVICE_UNK

#define MAX_URL 2048

typedef enum
{
	L_RELATIVE=0, 
	L_FROMROOT=1, 
	L_FOREIGN=2,
	L_CURRENT_B=3,
	L_CURRENT_G=4
} LINKTYPE;

class urlLinkage CUrl
{
private:
	unsigned long protocol;
	unsigned short port;
	CString szHost;
	CString szPath;
	CString szFile;
	CString szBookmark;
	CString GETParams;

	char szUrl[MAX_URL];

public:
	CUrl();
	CUrl(char* szUrl);
	CUrl(CString szUrl);
	CUrl(CUrl& url);
	CUrl(CUrl* url);
	~CUrl();

	CUrl operator=(char* szUrl);
	CUrl operator=(CUrl url);
	
	CUrl operator+(char* szUrl);
	CUrl operator+(CString Url);
	
	CUrl operator+=(char* szUrl);
	CUrl operator+=(CString Url);

	operator char*();
	operator CString();
	
	CString ToString();

	void SetProtocol(unsigned long nProto);
	void SetPort(unsigned short nPort);
	void SetHost(CString nHost);
	void SetFile(CString nFile);
	void SetPath(CString nPath);
	void SetBookmark(CString nBookmark);
	void SetGETParams(CString nGetParams);

	void SetURL(char* szUrl);

	unsigned long GetProtocol();
	unsigned short GetPort();
	CString GetHost();
	CString GetFile();
	CString GetPath();
	CString GetBookmark();
	CString GetGETParams();

	BOOL FollowUrl(char* szUrl);
	BOOL FollowUrl(CString url);

	static LINKTYPE URLType(char* szUrl);
	static LINKTYPE URLType(CString Url);
//	static LINKTYPE UrlType(char* szUrl);
	static BOOL IsValidURL(char* szURL);
	static BOOL IsValidURL(CString szURL);
	
	CString Localize(char* szToLocalizeUrl);
	CString Localize(CString ToLocalizeUrl);
	CString Localize(CUrl Url);

private:
	CString GetFilenameWithoutProtocol();
	CString GetFullWithoutProtocol();
	CString GetPathToRoot();
	CString GetProtocolByID(int id);
	void Setup(char* szUrl);
	void Setup(CUrl* url);
};

