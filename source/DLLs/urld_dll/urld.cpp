#include "StdAfx.h"
#define DULinkage __declspec (dllexport)
#define CompileDU
#include "urld.h"
#undef CompileDU
#include "../../lib/filesys.h"
#include "../../lib/b64.h"

#define DEFAULT_BUFFER_SIZE 2048

CDUrl::CDUrl(CUrl newURL, char* szNewFileName, AccessTypes eType, char* szNewPath)
{
	Set(newURL, szNewFileName, eType, szNewPath);
}

CDUrl::CDUrl(CUrl newURL, CString sNewFileName, AccessTypes eType, CString sNewPath)
{
	Set(newURL, sNewFileName.GetBuffer(sNewFileName.GetLength()), eType, sNewPath.GetBuffer(sNewPath.GetLength()));
}

CDUrl::CDUrl(CUrl newURL, AccessTypes eType, BOOL bGenerateFileName)
{
	if(Set(newURL, "", eType, ""))
		if(bGenerateFileName) GenerateFilename();
}
	
CDUrl::~CDUrl()
{
	if(pPwdAuth) delete pPwdAuth;
}

BOOL CDUrl::Set(CUrl newURL, char* szNewFileName, AccessTypes eType, char* szNewPath)
{
	pPwdAuth=NULL;
	bEnableMIME=FALSE;
	shHeaders.StatusCode=-1;
	shHeaders.ContentLength=-1;
	pOError=NULL, pONormal=NULL;
	dwBufferSize=DEFAULT_BUFFER_SIZE;
	pPwdAuth=new PWD_STRUCT;
	URL=newURL;
	dwAccessType=eType;

	if(!CUrl::IsValidURL((char*)newURL)) return FALSE;

	sFileName=szNewFileName;
	sPath=szNewPath;

	return TRUE;
}

char* CDUrl::GetFilename()
{
	return (char*)sFileName.data();
}


CUrl& CDUrl::GetURL()
{
	return URL;
}

char* CDUrl::GetPath()
{
	return (char*)sPath.data();
}

BYTE* CDUrl::GetBuffer()
{
	return cbaBuffer.GetData();
}

char* CDUrl::GenerateFilename()
{
	CString sTemp=URL.GetHost()+URL.GetPath();
	sTemp+=((URL.GetFile().IsEmpty())?DEFAULT_FILENAME:URL.GetFile());
	sTemp=CFileSystem::CollapseFilename(sTemp);
	sFileName=sTemp.GetBuffer(sTemp.GetLength());
	return (char*)sFileName.data();
}
	
void CDUrl::SetFileName(char* szNewFileName)
{
	sFileName=szNewFileName;
}

void CDUrl::SetURL(CUrl newURL)
{
	URL=newURL;
}

void CDUrl::SetPath(char* szNewPath)
{
	sPath=szNewPath;
}

void CDUrl::SetFileName(CString sNewFileName)
{
	sFileName=sNewFileName.GetBuffer(sNewFileName.GetLength());
}

void CDUrl::SetPath(CString sNewPath)
{
	sPath=sNewPath.GetBuffer(sNewPath.GetLength());
}

void CDUrl::SetRights(PWD_STRUCT* pNewPwd)
{
	pPwdAuth->EnableAuth=pNewPwd->EnableAuth;
	pPwdAuth->log=pNewPwd->log;
	pPwdAuth->password=pNewPwd->password;
	pPwdAuth->EnableProxyAuth=pNewPwd->EnableProxyAuth;
	pPwdAuth->proxyUser=pNewPwd->proxyUser;
	pPwdAuth->proxyPassword=pNewPwd->proxyPassword;
}

BOOL CDUrl::EnableMIME(BOOL bState)
{
	BOOL bOld=bEnableMIME;
	bEnableMIME=bState;

	return bOld;
}

void CDUrl::SetMIME(CMIMETypes* newMIME)
{
	if(newMIME)
	{
		cMIME=*newMIME;
		bEnableMIME=TRUE;
	}
	else bEnableMIME=FALSE;
}

CMIMETypes& CDUrl::GetMIME()
{
	return cMIME;
}

void CDUrl::ValidatePathAndFile()
{
	CString sFullPath;
	sFullPath.Format("%s%s",(char*)sPath.data(), (char*)sFileName.data());
	sFullPath=CFileSystem::CollapseFilename(sFullPath);
	SPLITINFO si=CFileSystem::SplitFileName(sFullPath.GetBuffer(sFullPath.GetLength()));
	sFileName=si.FilenameWithExtension;
	sPath=string(si.DriveLetter)+string(si.FullPath);
	
}

DWORD CDUrl::DownloadToFile()
{
	return DownloadRoutine(FALSE, TRUE);
}

DWORD CDUrl::DownloadToBuffer()
{
	return DownloadRoutine(TRUE, TRUE);
}

DWORD CDUrl::DownloadRoutine(BOOL bEnableBufferize, BOOL bEnableFilesave)
{
	CInternetSession session (_T((char*)sClientName.data()), 1, dwAccessType, (dwAccessType==accessProxy)?GetProxy():NULL, NULL, INTERNET_FLAG_DONT_CACHE);
	CString csErrorMsgBuffer;

	CHttpFile* pFile = NULL;
	CString csHead("Accept: */*\r\n\r\n");

	//Подготовка строки аутентификации
	if(pPwdAuth->EnableAuth)
	{
		char* szLog=pPwdAuth->log.GetBuffer(pPwdAuth->log.GetLength());
		char* szPwd=pPwdAuth->password.GetBuffer(pPwdAuth->password.GetLength());
		int nBufferSize=::MIMEAuthBufferSize(szLog, szPwd);
		char* szAuthBuffer=new char[nBufferSize];
		::MIMEAuth(szLog, szPwd, szAuthBuffer, nBufferSize);

		CString csAuth, csProxyAuth;
		csAuth.Format("Authorization: Basic %s\r\n", szAuthBuffer);
		delete szAuthBuffer;
		
		csHead+=csAuth;
	}

	//Подготовка строки аутентификации для прокси
	if(pPwdAuth->EnableProxyAuth)
	{
		char* szLog=pPwdAuth->proxyUser.GetBuffer(pPwdAuth->proxyUser.GetLength());
		char* szPwd=pPwdAuth->proxyPassword.GetBuffer(pPwdAuth->proxyPassword.GetLength());
		int nBufferSize=::MIMEAuthBufferSize(szLog, szPwd);
		char* szAuthBuffer=new char[nBufferSize];
		::MIMEAuth(szLog, szPwd, szAuthBuffer, nBufferSize);

		CString csAuth, csProxyAuth;
		csAuth.Format("Proxy-Authorization: Basic %s\r\n", szAuthBuffer);
		delete szAuthBuffer;
		
		csHead+=csAuth;
	}

	//Заголовок готов
	csHead+="\r\n";

	ValidatePathAndFile(); //Убираем из имени файла все куски пути

	DWORD dwnd=0;
	if(CFileSystem::CreateDirectories(sPath.data()))
	{
		try
		{
			pFile = (CHttpFile*)session.OpenURL ((char*)URL, 1, INTERNET_FLAG_RELOAD|INTERNET_FLAG_TRANSFER_BINARY, csHead.GetBuffer(csHead.GetLength()), -1L);
		}
		catch (CInternetException *pEx)
		{
			csErrorMsgBuffer.Format("Cannot open URL %s %s\n", (char*)URL, GetErrorText(pEx-> m_dwError, TRANSLATE_OEM));
			flushErr(csErrorMsgBuffer);
			return 0;
		}

		SetHeaders(pFile);

		if(bEnableMIME)
		{
			char *szCt=shHeaders.ContentType.GetBuffer(shHeaders.ContentType.GetLength());
			int nExt=sFileName.rfind('.');
			string sExt;
			if(nExt<0) sExt="";
			else sExt=sFileName.substr(++nExt);
			char* szExt=(char*)sExt.data();
			
			if(!cMIME.IsExtension(szCt, szExt))
			{
				char* szNewExt=cMIME.FindExtension(szCt);

				if(!cMIME.IsError) 
					if(nExt<0)
					{
						sFileName.append(".");
						sFileName.append(szNewExt);
					}
					else sFileName.replace(nExt, sFileName.length()-nExt, szNewExt);

				csErrorMsgBuffer.Format("Content Control found extension discrepancy. '%s' changed on '%s'\n", szExt, szNewExt);
				flushErr(csErrorMsgBuffer);
			}
		}

		CFile myFile;
		CFileException fileException;
	
		if(bEnableFilesave)
		{
			char* szFullName=(char*)(sPath+sFileName).data();
			if (!myFile.Open ((char*)(sPath+sFileName).data(), CFile::modeCreate | CFile::modeReadWrite | CFile::shareExclusive , &fileException ))
			{
				csErrorMsgBuffer.Format("Can't open file: %s - %s\n", szFullName, GetErrorText(fileException.m_cause, TRANSLATE_OEM));
				flushErr(csErrorMsgBuffer);
				return 0;
			}
		}

		if(bEnableBufferize) cbaBuffer.RemoveAll();
		BYTE *szInetBuffer=new BYTE[dwBufferSize];
		DWORD nRead;
		do
		{
			if(nRead = pFile->Read(szInetBuffer, dwBufferSize-1))
			{
				if(bEnableFilesave)
					myFile.Write (szInetBuffer, nRead);
				if(bEnableBufferize)
					for(int nIndex=0;nIndex<nRead;nIndex++) cbaBuffer.Add(szInetBuffer[nIndex]);
			}
			dwnd+=nRead;
		}while (nRead);

		delete szInetBuffer;

		if(bEnableFilesave) myFile.Close();
		pFile->Close();
		if (pFile != NULL) delete pFile;
		session.Close();

		//csErrorMsgBuffer.Format("\t\tdownloaded %s...\n", (char*)sFileName.data());
		csErrorMsgBuffer.Format("\t\tdownloaded %s...\n", (char*)URL);
		flushOut(csErrorMsgBuffer);
	}
	else
	{
		csErrorMsgBuffer.Format("Cannot create directory %s - %s\n",sPath.data(), GetErrorText(GetLastError(), TRANSLATE_OEM));
		flushErr(csErrorMsgBuffer);
		return 0;
	}

	return dwnd;
}

void CDUrl::SetHeaders(CHttpFile* pFile)
{
	pFile->QueryInfo(HTTP_QUERY_ACCEPT, shHeaders.Accept);
	pFile->QueryInfo(HTTP_QUERY_ACCEPT_CHARSET, shHeaders.AcceptCharset);
	pFile->QueryInfo(HTTP_QUERY_ACCEPT_ENCODING, shHeaders.AcceptEncoding);
	pFile->QueryInfo(HTTP_QUERY_ACCEPT_LANGUAGE, shHeaders.Language); 
	pFile->QueryInfo(HTTP_QUERY_ACCEPT_RANGES, shHeaders.AcceptRanges);
	pFile->QueryInfo(HTTP_QUERY_AGE, &shHeaders.Age);
	pFile->QueryInfo(HTTP_QUERY_ALLOW, shHeaders.AllowedMethods);  
	pFile->QueryInfo(HTTP_QUERY_AUTHORIZATION, shHeaders.Authorization);
	pFile->QueryInfo(HTTP_QUERY_CACHE_CONTROL, shHeaders.CacheControl);
	pFile->QueryInfo(HTTP_QUERY_CONNECTION, shHeaders.Connection);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_BASE, shHeaders.ContentBase);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_DESCRIPTION, shHeaders.ContentDescription); 
	pFile->QueryInfo(HTTP_QUERY_CONTENT_DISPOSITION, shHeaders.ContentDisposition);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_ENCODING, shHeaders.ContentEncoding);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_ID , shHeaders.ContentID); 
	pFile->QueryInfo(HTTP_QUERY_CONTENT_LANGUAGE, shHeaders.ContentLanguage);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, shHeaders.ContentLength);  
	pFile->QueryInfo(HTTP_QUERY_CONTENT_LOCATION, shHeaders.ContentLocation);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_MD5, shHeaders.ContentMd5);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_RANGE, shHeaders.ContentRange);
	pFile->QueryInfo(HTTP_QUERY_CONTENT_TRANSFER_ENCODING, shHeaders.ContentTransferEncoding); 
	pFile->QueryInfo(HTTP_QUERY_CONTENT_TYPE, shHeaders.ContentType);
	pFile->QueryInfo(HTTP_QUERY_COOKIE, shHeaders.Cookie);
	pFile->QueryInfo(HTTP_QUERY_COST, shHeaders.Cost); 
	pFile->QueryInfo(HTTP_QUERY_DATE, &shHeaders.Date); 
	pFile->QueryInfo(HTTP_QUERY_DERIVED_FROM, shHeaders.DerivedFrom); 
	pFile->QueryInfo(HTTP_QUERY_ETAG, shHeaders.Etag);
	pFile->QueryInfo(HTTP_QUERY_EXPIRES, &shHeaders.Expires); 
	pFile->QueryInfo(HTTP_QUERY_FORWARDED, shHeaders.Forwarded);
	pFile->QueryInfo(HTTP_QUERY_FROM, shHeaders.From);
	pFile->QueryInfo(HTTP_QUERY_HOST, shHeaders.Host);
	pFile->QueryInfo(HTTP_QUERY_IF_MATCH, shHeaders.IfMatch);
	pFile->QueryInfo(HTTP_QUERY_IF_MODIFIED_SINCE, &shHeaders.IfModifiedSince);
	pFile->QueryInfo(HTTP_QUERY_IF_NONE_MATCH, shHeaders.IfNoneMatch);
	pFile->QueryInfo(HTTP_QUERY_IF_RANGE, shHeaders.IfRange);
	pFile->QueryInfo(HTTP_QUERY_IF_UNMODIFIED_SINCE, &shHeaders.IfUnmodifiedSince);
	pFile->QueryInfo(HTTP_QUERY_LAST_MODIFIED, &shHeaders.LastModified); 
	pFile->QueryInfo(HTTP_QUERY_LINK, shHeaders.Link); 
	pFile->QueryInfo(HTTP_QUERY_LOCATION, shHeaders.Location);
	pFile->QueryInfo(HTTP_QUERY_MAX_FORWARDS, shHeaders.MaxForwards);
	pFile->QueryInfo(HTTP_QUERY_MESSAGE_ID, shHeaders.MessageID); 
	pFile->QueryInfo(HTTP_QUERY_MIME_VERSION, shHeaders.MIMEVersion);
	pFile->QueryInfo(HTTP_QUERY_ORIG_URI, shHeaders.OrigUri);
	pFile->QueryInfo(HTTP_QUERY_PRAGMA, shHeaders.Pragma); 
	pFile->QueryInfo(HTTP_QUERY_PROXY_AUTHENTICATE, shHeaders.ProxyAuthenticate);
	pFile->QueryInfo(HTTP_QUERY_PROXY_AUTHORIZATION, shHeaders.ProxyAuthorization);
	pFile->QueryInfo(HTTP_QUERY_PUBLIC, shHeaders.PublicMethods);  
	pFile->QueryInfo(HTTP_QUERY_RANGE, shHeaders.Range);
	pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, shHeaders.RawHeaders); 
	pFile->QueryInfo(HTTP_QUERY_REFERER, shHeaders.Referer);
	pFile->QueryInfo(HTTP_QUERY_REFRESH, shHeaders.Refresh);
	pFile->QueryInfo(HTTP_QUERY_REQUEST_METHOD, shHeaders.RequestMethod);
	pFile->QueryInfo(HTTP_QUERY_RETRY_AFTER, shHeaders.RetryAfter);
	pFile->QueryInfo(HTTP_QUERY_SERVER, shHeaders.Server);
	pFile->QueryInfo(HTTP_QUERY_SET_COOKIE, shHeaders.SetCookie);
	pFile->QueryInfo(HTTP_QUERY_STATUS_CODE, shHeaders.StatusCode); 
	pFile->QueryInfo(HTTP_QUERY_STATUS_TEXT, shHeaders.StatusText); 
	pFile->QueryInfo(HTTP_QUERY_TITLE, shHeaders.Title);
	pFile->QueryInfo(HTTP_QUERY_TRANSFER_ENCODING, shHeaders.TransferEncoding);
	pFile->QueryInfo(HTTP_QUERY_UPGRADE, shHeaders.Upgrade);
	pFile->QueryInfo(HTTP_QUERY_URI, shHeaders.URI); 
	pFile->QueryInfo(HTTP_QUERY_USER_AGENT, shHeaders.UserAgent);
	pFile->QueryInfo(HTTP_QUERY_VARY, shHeaders.Vary);
	pFile->QueryInfo(HTTP_QUERY_VERSION, shHeaders.Version); 
	pFile->QueryInfo(HTTP_QUERY_VIA, shHeaders.Via);
	pFile->QueryInfo(HTTP_QUERY_WARNING, shHeaders.Warning);
	pFile->QueryInfo(HTTP_QUERY_WWW_AUTHENTICATE, shHeaders.WwwAuthenticate);

}

CString DULinkage GetErrorText(DWORD dwLastError, int char_or_oem)
{
    LPSTR szMessageBuffer;
    DWORD dwBufferLength;
	CString csRet;

    if(dwBufferLength = FormatMessageA(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS |
        FORMAT_MESSAGE_FROM_SYSTEM , NULL,
        dwLastError,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // default language
        (LPSTR) &szMessageBuffer,
        0, NULL
        ))
    {
		int n=strlen(szMessageBuffer);
		szMessageBuffer[n-2]=0;
		char* szTranslatedBuffer=new char[n+10];
		char error[10];
		sprintf(error, " (%d)", dwLastError);
		if(char_or_oem==TRANSLATE_OEM)
		{
			CharToOem(szMessageBuffer, szTranslatedBuffer);
		}
		else
			strcpy(szTranslatedBuffer, szMessageBuffer);

		strcat(szTranslatedBuffer,error);
		csRet.Format("%s", szTranslatedBuffer);
		delete szTranslatedBuffer;
        LocalFree(szMessageBuffer);
    }

	return csRet;
}

void CDUrl::SetOutputs(FLUSH_FUNC pNormal, FLUSH_FUNC pError)
{
	pONormal=pNormal;
	pOError=pError;
}

void CDUrl::flushErr(CString text)
{
	if(pOError) pOError(text);
}

void CDUrl::flushOut(CString text)
{
	if(pONormal) pONormal(text);
}

void CDUrl::SetInnerBufferSize(DWORD dwNewBufferSize)
{
	dwBufferSize=dwNewBufferSize;
}

DWORD CDUrl::DownloadToFile(char* szFileToDownload)
{
	SetFileName(szFileToDownload);
	return DownloadToFile();
}

DWORD CDUrl::DownloadToFile(CString csFileToDownload)
{
	SetFileName(csFileToDownload);
	return DownloadToFile();
}

void CDUrl::SetProxy(char* szProxy)
{
	sProxy=szProxy;
}