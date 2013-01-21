#pragma once
#pragma warning(disable:4786)
#include <map>
#include <string>

#define MAX_MIME 32767
using namespace std;

typedef map<string, string, less<string>, allocator<string> > mimetype;

class CMIMETypes
{
private:
	mimetype MIMETypeArray;
public:
	BOOL IsError;
public:
	CMIMETypes& operator=(CMIMETypes& newMIME);
	int Load(char* szFileName);
	char* FindExtension(char* type);
	BOOL IsExtension(char* type, char* ext);
};