#include "StdAfx.h"
#define DULinkage __declspec (dllexport)

#define CompileDU
#include "urld.h"
#undef CompileDU

#define _CRTDBG_MAP_ALLOC

//#include "../../lib/filesys.h"
//#include "../../lib/b64.h"

#include <math.h>
CString makeDigits(int number);

CUrlManager::CUrlManager(CString csNewClientName, CString csNewDownloadRoot, int nNewThreads, DWORD dwAccessType, CString csNewProxyName, CMIMETypes* pcmtNewTypes, PWD_STRUCT *ppsNewPwd)
{
	csClientName=csNewClientName;
	csDownloadRoot=csNewDownloadRoot;
	csProxyName=csNewProxyName;
	dwAccess=dwAccessType;

	dwTotalBytes=0;
	dwThreads=0;

	nThreads=nNewThreads;
	hThCount=CreateSemaphore(NULL, (nNewThreads)?nNewThreads:MAX_THREADS, nNewThreads, NULL);
	copyEvent=CreateEvent(NULL,TRUE, FALSE, NULL);
	
	pcmtTypes=pcmtNewTypes;
	ppsPwd=ppsNewPwd;

	pONormal=NULL;
	pOError=NULL;

	dwStartTime=NULL;
	dwFinishTime=NULL;
	dTotalTime=0.0;

	cbaData.RemoveAll();
}

CUrlManager::~CUrlManager()
{
	TRACE ("\t\tPrepare to shutdown\n");
	TRACE ("\t\tClosing handles\n");
	CloseHandle(copyEvent);
	CloseHandle(hThCount);
}

BOOL CUrlManager::StartWithUrl (CUrl cuStartUrl)
{
	CFile cfTryFile;
	
	if(cfTryFile.Open(cuStartUrl.GetFile(), CFile::modeRead|CFile::shareDenyWrite))
	{
		DWORD dwFileLen=cfTryFile.GetLength();
		BYTE* byteFileArray=new BYTE[dwFileLen];
		DWORD dwRead=cfTryFile.ReadHuge(byteFileArray, dwFileLen);

		for(DWORD i=0;i<dwFileLen;i++)
			cbaData.Add(byteFileArray[i]);
		delete byteFileArray;

		CString csMessage;
		csMessage.Format("File %s was succesfully read\n", cuStartUrl.GetFile());
		pONormal(csMessage);
	}
	else
	{
		this->cuStartUrl=cuStartUrl;
		CDUrl cduDownloader(cuStartUrl,(CDUrl::AccessTypes)dwAccess,TRUE);
		if(dwAccess==CDUrl::accessProxy)	cduDownloader.SetProxy(csProxyName.GetBuffer(csProxyName.GetLength()));
		cduDownloader.SetPath(csDownloadRoot);
		cduDownloader.SetRights(ppsPwd);
		cduDownloader.SetMIME(pcmtTypes);
		cduDownloader.SetOutputs(pONormal, pOError);

		dwTotalBytes+=cduDownloader.DownloadToBuffer();
		cbaData.Append(cduDownloader.GetBufferAsArray());
	}

	return TRUE;
}

BOOL CUrlManager::StartWithList(CStrings &csLinksList)
{
	for(int n=0;n<csLinksList.GetSize();n++)
	{
		char* szUrl=csLinksList[n].GetBuffer(csLinksList[n].GetLength());
		ucUrlArray.insert(urlcond::value_type(string(szUrl), URL_READY));
	}

	return DownloadList();
};

/*
BOOL CUrlManager::StartWithList (CStrings csLinksList)
{
	CString csMessage;
	csMessage.Format("Starting miltidownloading using file %s\n", linksFile);
	fONormal(csMessage);

	CFile file;
	if(file.Open(linksFile, CFile::modeRead|CFile::shareExclusive))
	{
		int len=file.GetLength();
		char* buffer=new char[len+1];
		file.ReadHuge(buffer, len);
		buffer[len]=0;
		CString links=buffer;
		delete buffer;
		file.Close();

		int start=0, ostart;
		CString cLink;
		CStrings List;

		while((ostart=links.Find("\n", start))!=-1)
		{
			cLink=links.Mid(start, ostart-start-1);
			List.Add(cLink);
			start=ostart+1;
		}
		cLink=links.Right(links.GetLength()-start);
		if(cLink.GetLength()) 
			List.Add(cLink);

		CString tbuffer;
		DownloadList(List);

	}
	else
	{
		fOut(GetErrorText(GetLastError(), TRANSLATE_OEM));
	}
	return 0;
}
*/

BOOL CUrlManager::ContinueAll()
{
	if(!cbaData.GetData()) return FALSE;
	CStrings csAll=extractAll((const char*)cbaData.GetData());

	for(int n=0;n<csAll.GetSize();n++)
	{
		char* szUrl=csAll[n].GetBuffer(csAll[n].GetLength());
		ucUrlArray.insert(urlcond::value_type(string(szUrl), URL_READY));
	}

	return DownloadList();
}

BOOL CUrlManager::ContinueWith(CWordArray &cwaTypes)
{
	if(!cbaData.GetData()) return FALSE;
	CStrings csEls;
	
	for(int i=0;i<cwaTypes.GetSize();i++)
		csEls.Append(extractElements(cwaTypes[i], (char*)cbaData.GetData()));

	for(int n=0;n<csEls.GetSize();n++)
	{
		char* szUrl=csEls[n].GetBuffer(csEls[n].GetLength());
		ucUrlArray.insert(urlcond::value_type(string(szUrl), URL_READY));
	}

	return DownloadList();
}

void CUrlManager::SetOutputs(FLUSH_FUNC pNormal, FLUSH_FUNC pError)
{
	pONormal=pNormal;
	pOError=pError;
}

BOOL CUrlManager::DownloadList ()
{
	CString csMessage;
	urlcond::iterator it;

	for (it=ucUrlArray.begin(); it!=ucUrlArray.end();++it)
	{
		CUrl cuCurrentUrl=cuStartUrl;
		char* szNewlink=(char*)(*it).first.data();
		cuCurrentUrl.FollowUrl(szNewlink);
		csMessage.Format("Retrieving %s\n", szNewlink);
		pONormal(csMessage);

		DWORD dwThreadId;
		HANDLE hThread;
		ResetEvent(copyEvent);
		if(WaitForSingleObject(hThCount, INFINITE)==WAIT_OBJECT_0)
		{
			CDUrl cduDownloader(cuCurrentUrl, (CDUrl::AccessTypes)dwAccess, TRUE);
			if(dwAccess==CDUrl::accessProxy)	cduDownloader.SetProxy(csProxyName.GetBuffer(csProxyName.GetLength()));
			cduDownloader.SetPath(csDownloadRoot);
			cduDownloader.SetRights(ppsPwd);
			cduDownloader.SetMIME(pcmtTypes);
			cduDownloader.SetOutputs(pONormal, pOError);

			DOWNLOAD_PACK dp;
			dp.pCDUrl=&cduDownloader;
			dp.pCopyEvent=&copyEvent;
			dp.pdwTotalBytes=&dwTotalBytes;
			dp.pThreadCount=&hThCount;
			dp.pdwThreads=&dwThreads;

			hThread=CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)::threadRoutine, (void*)&dp, 0, &dwThreadId);
			WaitForSingleObject(copyEvent, INFINITE);
			CloseHandle(hThread);
		}
		else
		{
			csMessage.Format("Cannot start download routine for %s: %s\n", szNewlink, GetErrorText(GetLastError(), TRANSLATE_OEM));
			pOError(csMessage);
		}
	}
	while(dwThreads) TRACE("\tWaiting %d threads to shut down\n", dwThreads);
	return TRUE;
}

DWORD threadRoutine(void* pSDP)
{
	DOWNLOAD_PACK* pPack=(DOWNLOAD_PACK*)pSDP;
	InterlockedIncrement((long*)pPack->pdwThreads);

	CDUrl* pDownloader=pPack->pCDUrl;
	CDUrl cduDownloader(pDownloader->GetURL(), (CDUrl::AccessTypes)pDownloader->GetAccessType(), TRUE);
	if(pDownloader->GetAccessType()==CDUrl::accessProxy)	cduDownloader.SetProxy(pDownloader->GetProxy());
	cduDownloader.SetPath(pDownloader->GetPath());
	cduDownloader.SetRights(pDownloader->GetRights());
	cduDownloader.SetMIME(&pDownloader->GetMIME());
	cduDownloader.SetOutputs(pDownloader->GetNormalOutput(), pDownloader->GetErrorOutput());

	SetEvent(*(pPack->pCopyEvent));
	*(pPack->pdwTotalBytes)+=cduDownloader.DownloadToFile();
	ReleaseSemaphore(*(pPack->pThreadCount), 1, NULL);

	InterlockedDecrement((long*)pPack->pdwThreads);
	return NO_ERROR;
}

void CUrlManager::RunTimer()
{
	dwStartTime=GetTickCount();
}

void CUrlManager::StopTimer()
{
	dwFinishTime=GetTickCount();
	dTotalTime=(double)(dwFinishTime-dwStartTime)/1000;
}

void CUrlManager::ShowStat()
{
	printf("\n\tTime elapsed...\t\t%.2fs",dTotalTime);
	printf("\n\tDownloaded size...\t%s bytes",::makeDigits(dwTotalBytes));
	printf("\n\tAverage speed...\t%.1f Kb/s",(double)dwTotalBytes/(dTotalTime*1000));
}

CString makeDigits(int number)
{
	if(number==0) return CString("0");

	CString ret="", dig;
	int csize=log(number)/log(10)+2;
	char* cnum=new char[csize+1];

	itoa(number, cnum, 10);
	dig=strrev(cnum);

	for(int i=0;i<csize-3;i+=3)
		ret+=(dig.Mid(i,3)+" ");

	ret+=dig.Mid(i);

	ret.MakeReverse();
	
	delete cnum;

	if(ret[0]==' ') return ret.Mid(1);
	else return ret;
}
