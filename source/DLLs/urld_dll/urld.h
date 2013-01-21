#pragma warning(disable:4786)
#ifndef DULinkage
#define DULinkage __declspec (dllimport)
#endif

#include <string>

#ifdef CompileDU
#include "../../lib/curl.h"
#include "../../lib/mime.h"
#include "../../lib/links.h"
#else
#include "lib/curl.h"
#include "lib/mime.h"
#include "lib/links.h"
#endif

using std::string;

#define DEFAULT_FILENAME "index.html"
#define CLIENT_NAME _T("Schmozilla Platinum (not-compatible; MSUX 6.0; Linux XP;)")

#define MAX_THREADS 5

#define MSIE_NAME _T("Mozilla/6.0 (compatible; MSIE 6.0; Windows NT;)")
#define NSNV_NAME _T("Mozilla/5.0 * (WinNT; I ;Nav)")
#define OPER_NAME _T("Opera/4.02 (Windows NT 5.0; U) *")

typedef void(*FLUSH_FUNC)(CString);
typedef struct 
{
	CString Accept;
	CString AcceptCharset;
	CString AcceptEncoding;
	CString AcceptRanges;
	SYSTEMTIME Age;
	CString AllowedMethods;
	CString Authorization;
	CString CacheControl;
	CString Connection;
	CString ContentBase;
	CString ContentDescription;
	CString ContentDisposition;
	CString ContentEncoding;
	CString ContentID;
	CString ContentLanguage;
	DWORD ContentLength;
	CString ContentLocation;
	CString ContentMd5;
	CString ContentRange;
	CString ContentTransferEncoding;
	CString ContentType;
	CString Cookie;
	CString Cost;
	SYSTEMTIME Date;
	CString DerivedFrom;
	CString Etag;
	SYSTEMTIME Expires;
	CString Forwarded;
	CString From;
	CString Host;
	CString IfMatch;
	SYSTEMTIME IfModifiedSince;
	CString IfNoneMatch;
	CString IfRange;
	SYSTEMTIME IfUnmodifiedSince;
	CString Language;
	SYSTEMTIME LastModified;
	CString Link;
	CString Location;
	CString MaxForwards;
	CString MessageID;
	CString MIMEVersion;
	CString OrigUri;
	CString Pragma;
	CString ProxyAuthenticate;
	CString ProxyAuthorization;
	CString PublicMethods;
	CString Range;
	CString RawHeaders;
	CString Referer;
	CString Refresh;
	CString RequestMethod;
	CString RetryAfter;
	CString Server;
	CString SetCookie;
	DWORD StatusCode;
	CString StatusText;
	CString Title;
	CString TransferEncoding;
	CString Upgrade;
	CString URI;
	CString UserAgent;
	CString Vary;
	CString Version;
	CString Via;
	CString Warning;
	CString WwwAuthenticate;

} HTTPHEADER;

typedef struct
{
	CString log;
	CString password;
	BOOL EnableAuth;

	CString proxyUser;
	CString proxyPassword;
	BOOL EnableProxyAuth;
}PWD_STRUCT;

class DULinkage CDUrl
{
private:
	CUrl URL;
	string sFileName;
	string sPath;
	CByteArray cbaBuffer;
	string sClientName;
	PWD_STRUCT* pPwdAuth;
	CMIMETypes cMIME;
	BOOL bEnableMIME;

	FLUSH_FUNC pOError;
	FLUSH_FUNC pONormal;

	DWORD dwAccessType;
	string sProxy;

	DWORD dwBufferSize;
public:

	typedef enum
	{
		accessDirect=INTERNET_OPEN_TYPE_DIRECT,
		accessPreConfig=INTERNET_OPEN_TYPE_PRECONFIG,
		accessProxy=INTERNET_OPEN_TYPE_PROXY 
	} AccessTypes;

	HTTPHEADER shHeaders;
public:
	CDUrl(CUrl newURL, char* szNewFileName, AccessTypes eType=accessPreConfig, char* szNewPath="\\");
	CDUrl(CUrl newURL, AccessTypes eType=accessPreConfig, BOOL bGenerateFileName=FALSE);
	CDUrl(CUrl newURL, CString sNewFileName, AccessTypes eType=accessPreConfig, CString sNewPath="\\");
	
	~CDUrl();
private:
	BOOL Set(CUrl newURL, char* szNewFileName, AccessTypes eType, char* szNewPath);
public:
	char* GetFilename();
	CUrl& GetURL();
	char* GetPath();
	char* GetProxy() {return (char*)sProxy.data();};
	DWORD GetAccessType() const {return dwAccessType;};

	BYTE* GetBuffer();
	CByteArray& GetBufferAsArray() {return cbaBuffer;};
	DWORD GetInnerBufferSize() const {return dwBufferSize;};
	FLUSH_FUNC GetNormalOutput() const {return pONormal;};
	FLUSH_FUNC GetErrorOutput() const {return pOError;};
	CMIMETypes& GetMIME();
	PWD_STRUCT* GetRights() const {return pPwdAuth;};

	char* GenerateFilename();
	
	void SetFileName(char* szNewFileName);
	void SetURL(CUrl newURL);
	void SetPath(char* szNewPath);
	void SetProxy(char* szProxy);
	
	void SetOutputs(FLUSH_FUNC pNormal, FLUSH_FUNC pError);
	void SetInnerBufferSize(DWORD dwNewBufferSize);
	
	void SetFileName(CString sNewFileName);
	void SetPath(CString sNewPath);

	void SetRights(PWD_STRUCT* pNewPwd);
	void SetMIME(CMIMETypes* newMIME);

	BOOL EnableMIME(BOOL bState=TRUE);

	DWORD DownloadToFile();
	DWORD DownloadToFile(char* szFileToDownload);
	DWORD DownloadToFile(CString csFileToDownload);
	DWORD DownloadToBuffer();

	void ValidatePathAndFile();

	static CString MIMEAuth(CString log, CString password);
private:
//	void CutOffUrl(char* url, BOOL bPreservePound=FALSE);
	void SetHeaders(CHttpFile* pFile);

	void flushOut(CString text);
	void flushErr(CString text);

	DWORD DownloadRoutine(BOOL bEnableBufferize, BOOL bEnableFilesave);
};
/*
void flushOut(CString text);
void flushErr(CString text);
void flush(CString text, char* szFuncName);
*/
#define TRANSLATE_OEM 1
#define TRANSLATE_ANSI 0

CString DULinkage GetErrorText(DWORD dwLastError, int char_or_oem=TRANSLATE_ANSI);

typedef map<string, int, less<string>, allocator<int> > urlcond;
class DULinkage CUrlManager
{
#define URL_READY		93800
#define URL_LOADING		93801
#define URL_DOWNLOADED	93802
#define URL_ERROR		93803
private:
	CString csClientName;
	CString csDownloadRoot;
	CString csProxyName;

	DWORD dwAccess;

	unsigned int nThreads;
	CMIMETypes* pcmtTypes;
	PWD_STRUCT* ppsPwd;
	urlcond ucUrlArray;
	CUrl cuStartUrl;
	
	CByteArray cbaData;

	FLUSH_FUNC pOError;
	FLUSH_FUNC pONormal;

	HANDLE hThCount;
	HANDLE copyEvent;

	DWORD dwThreads;

	DWORD dwStartTime;
	DWORD dwFinishTime;
public:
	DWORD dwTotalBytes;
	double dTotalTime;
public:
	CUrlManager(CString csNewClientName, CString csNewDownloadRoot, int nNewThreads, DWORD dwAccessType=CDUrl::accessPreConfig, CString csNewProxyName="", CMIMETypes* pcmtNewTypes=NULL, PWD_STRUCT *ppsNewPwd=NULL);
	CUrlManager::~CUrlManager();

	CString GetClientName() const {return csClientName;};
	int GetThreads() const {return nThreads;};
	CMIMETypes* GetMIME() const {return pcmtTypes;};
	PWD_STRUCT * GetAuthority() const {return ppsPwd;};

	BOOL StartWithUrl (CUrl cuStartUrl);
	BOOL CUrlManager::StartWithList(CStrings &csLinksList);
	BOOL ContinueAll();
	BOOL ContinueWith(CWordArray &cwaTypes);

	FLUSH_FUNC GetNormalOutput() const {return pONormal;};
	FLUSH_FUNC GetErrorOutput() const {return pOError;};

	void SetOutputs(FLUSH_FUNC pNormal, FLUSH_FUNC pError);

	void RunTimer();
	void StopTimer();
	void ShowStat();

private:
	BOOL DownloadList ();
};

typedef struct
{
	CDUrl* pCDUrl;
	DWORD* pdwTotalBytes;
	DWORD* pdwThreads;
	HANDLE* pCopyEvent;
	HANDLE* pThreadCount;
}DOWNLOAD_PACK;

DWORD threadRoutine(void* pSDP);
