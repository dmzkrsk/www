#include "stdafx.h"
#include <afxinet.h>
#define uLinkage __declspec(dllexport)
#include "murl.h"

uLinkage CString FollowLink(const char* base, char* newlink)
{
	CString link;
	if(strstr(_strlwr(newlink), "http://"))
	{
		link.Format("%s", newlink);
		return link;
	}

	unsigned short port;
	DWORD ser;
	if(strstr(newlink, "/")==newlink)
	{
		CString server, path;
		AfxParseURL(base, ser, server, path,  port);
		link.Format("http://%s%s", server, newlink);
		return link;
	}
	
	if(strstr(newlink, "#")==newlink||strstr(newlink, "?")==newlink)
	{
		link.Format("%s", base);
		return link;
	}

	CString server, path;
	AfxParseURL(base, ser, server, path, port);
	CString rpath=path.Left(path.ReverseFind('/')+1);
	link.Format("http://%s%s%s", server, rpath, newlink);
	return link;
}
