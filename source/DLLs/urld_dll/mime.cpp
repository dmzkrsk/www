#include "StdAfx.h"
#include "mime.h"

int CMIMETypes::Load(char* szFileName)
{
	IsError=FALSE;
	int r=0, ir=0;
	CFile mime_cfg;
	CFileException exc;

	if(mime_cfg.Open(szFileName,CFile::modeRead, &exc))
	{
		DWORD lLen=mime_cfg.GetLength();
		char* szFileBuf=new char[lLen+1];
		mime_cfg.ReadHuge(szFileBuf, lLen);
		szFileBuf[lLen]=0;
		mime_cfg.Close();

		char* pos=szFileBuf;
		char* npos;
//		flushOut("Reading mime.cfg...");
		CString ContentType, Extension;
		while(npos=strstr(pos,"\n"))
		{
			if(npos[1]!='#')
			{
				char *mpos;
				mpos=strchr(pos,'=');
				if(mpos>pos &&mpos<npos)
				{
					(mpos++)[0]=0;
					(npos-1)[0]=0;
					(npos++)[0]=0;

					ContentType=pos;
					ContentType.TrimLeft();
					ContentType.TrimRight();
					Extension=mpos;
					Extension.TrimLeft();
					Extension.TrimRight();

					string key(ContentType.GetBuffer(ContentType.GetLength()));
					string val(Extension.GetBuffer(Extension.GetLength()));

					MIMETypeArray.insert(mimetype::value_type(key, val));

					r++;
					npos--;
				}
				pos=++npos;
			}
			else pos=++npos, ir++;

		}

		delete szFileBuf;
	}
	else
	{
		IsError=TRUE;
//		CString errmsg;
//		errmsg.Format("Cannot open %s - %s\n",szFileName, GetErrorText(exc.m_cause, TRANSLATE_OEM));
//		flushErr(errmsg);
	}

	CString msg;
	msg.Format("%d type%s read (%d ignored)\n", r, (r>1)?"s":"", ir);
//	flushOut(msg);
	return r;
}

char*  CMIMETypes::FindExtension(char* type)
{
	IsError=FALSE;
	mimetype::iterator it;
	char* szTemp=NULL;

	it=MIMETypeArray.find(string(type));
	if(it!=MIMETypeArray.end())	szTemp=(char* )(*it).second.data();
	else IsError=TRUE;

	return szTemp;
}

BOOL CMIMETypes::IsExtension(char* type, char* ext)
{
	if(!type || !ext) return FALSE;
	IsError=FALSE;
	mimetype::iterator it;
	it=MIMETypeArray.find(string(type));
	if(it!=MIMETypeArray.end())
		return !stricmp(ext, (char* )(*it).second.data());
	else return FALSE;
}

CMIMETypes& CMIMETypes::operator=(CMIMETypes& newMIME)
{
	MIMETypeArray=newMIME.MIMETypeArray;
	return *this;
}
