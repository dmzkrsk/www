#include "StdAfx.h"
#include "filesys.h"
#include <Shlwapi.h>

BOOL CFileSystem::CreateDirectories(const char* dirpath)
{
	CString dir;
	dir.Format("%s", dirpath);
	int fslash, bslash;
	int start=0;
	BOOL bError=TRUE;
	CString path;

	do
	{
		fslash=dir.Find('/',start);
		bslash=dir.Find('\\',start);

		if(fslash==-1) fslash=dir.GetLength()+1;
		if(bslash==-1) bslash=dir.GetLength()+1;
		if(fslash==bslash) break;

		start=(fslash<bslash)?fslash:bslash;
		path=dir.Left(start);
		start++;
		if(path.GetAt(path.GetLength()-1)!=':' && path.GetAt(path.GetLength()-2)!=':') //Варианты "c:" или "c:\"
			if(!(bError=CreateDirectory(path.GetBuffer(path.GetLength()),NULL)))
				if(GetLastError()==ERROR_ALREADY_EXISTS)
					bError=TRUE;
	}while(bError);

	return bError;

}

SPLITINFO CFileSystem::SplitFileName(char* filename)
{
	SPLITINFO si;
	_splitpath(filename, si.DriveLetter, si.FullPath, si.FilenameWithoutExtension,  si.Extension);
/*	if (!strlen(si.FilenameWithoutExtension))
		strcpy(si.FilenameWithoutExtension,DEFAULT_FILENAME);
*/	si.FilenameWithExtension.Format("%s%s",si.FilenameWithoutExtension,si.Extension);
	return si;
}

//Сборка ../, ./
CString CFileSystem::CollapseFilename(CString String)
{
		String.Replace('\\','/');
		String.Replace("/./","/");
		if(String.Find("./")==0) String=String.Mid(2);

		CUIntArray Slashes;
		UINT s=0;

		while(1)
		{
			Slashes.RemoveAll();
			while(s=String.Find('/', s)+1) Slashes.Add(s);
			Slashes.Add(String.GetLength());

			if(Slashes.GetSize()<4) break;
			for(int i=0;i<Slashes.GetSize()-3;i++)
				if(!String.Mid(Slashes.GetAt(i+1), Slashes.GetAt(i+2)-Slashes.GetAt(i+1)).Compare("../") && String.Mid(Slashes.GetAt(i), Slashes.GetAt(i+1)-Slashes.GetAt(i)).Compare("../"))
				{
					CString pattern=String.Mid(Slashes.GetAt(i), Slashes.GetAt(i+2)-Slashes.GetAt(i));
					String.Replace(pattern, "");
					break;
				}
			
			if(i==Slashes.GetSize()-3) break;
			else continue;
		}

		return String;

}

//Поиск относительного пути
BOOL CFileSystem::RelativePath(char* szStart, char* szTo, CString* Path)
{
	BOOL bVal=FALSE;
	CString Start=szStart;
	CString To=szTo;
	Start.Replace('/','\\');
	To.Replace('/','\\');

	char* szPath=new char[_MAX_PATH];
	if(::PathRelativePathTo(szPath, Start.GetBuffer(Start.GetLength()), FILE_ATTRIBUTE_NORMAL, To.GetBuffer(To.GetLength()), FILE_ATTRIBUTE_NORMAL))
	{
		*Path=CString(szPath);
		bVal=TRUE;
	}
	delete szPath;
	return bVal;
}
