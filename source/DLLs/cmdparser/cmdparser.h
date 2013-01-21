#ifndef cmdpLinkage
#define cmdpLinkage __declspec (dllimport)
#endif

#pragma warning(disable:4786)

#include "map"
#include "string"

using namespace std;

typedef map<string, string, less<string>, allocator<string> > strhash;

#define PARAM_DOUBLEMINUS 2
#define PARAM_SINGLEMINUS 1
#define PARAM_UNDEFINED 0

class cmdpLinkage CCmdParser
{
private:
	char SplitChars(char* curcmdLine);
	static int GetKind(char* szElement);
	strhash list;
	int nCmd;
	char** cmdLine;
public:
	int GetParamCount();
	int GetIntParam(int n);
	int GetIntParam(char * szKey);
	void print();
	void LoadLine(int argc, char** argv);
	void Parse();
	char* GetParam(char* szKey);
	char* GetParam(int n);
	char* GetOneOfParam(char* szKey);
	int GetOneOfIntParam(char* szKey);
	bool IsParamExists(char* szKey);
};
