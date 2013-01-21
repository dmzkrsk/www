#define cmdpLinkage __declspec (dllexport)
#include "cmdparser.h"

void CCmdParser::LoadLine(int argc, char** argv)
{
	nCmd=argc;
	cmdLine=argv;
}

void CCmdParser::Parse()
{
	if(!cmdLine) return;

	string key, val;
	int param=1;

	for(int i=1;i<nCmd;i++, key.erase(), val.erase())
	{
		switch(GetKind(cmdLine[i]))
		{
		case PARAM_DOUBLEMINUS:
			{
				char* eqpos=strchr(cmdLine[i],'='); //Положение знака равенства
				
				if(!eqpos)//Если нет равенства, то значение ключа булевое
				{
					key.append(cmdLine[i]+2);
					break;
				}
				else (eqpos++)[0]=0;

				//Равно разделяет строку на ключ=значение
				key.append(cmdLine[i]+2);
				val.append(eqpos);
				break;
			}
		case PARAM_SINGLEMINUS:
			{
				//Если это последний параметр в командной строке
				char sym[2]={SplitChars(cmdLine[i]),0};
				if(i==(nCmd-1))
				{
					//Ключ - булевое значение - разбираем по символам
					key.append(sym);
					break;
				}
			
				//Если после него не ключ
				if(GetKind(cmdLine[i+1])==PARAM_UNDEFINED)
				{
					key.append(sym);
					val.append(cmdLine[++i]);
				}
				else
					key.append(sym);

				break;
			}
		case PARAM_UNDEFINED:
			{
				char keyname[128];
				sprintf(keyname,"param%d", param++);
				key.append(keyname);
				val.append(cmdLine[i]);
				break;
			}
		}
		list.insert(strhash::value_type(key, val));
	}
}

char* CCmdParser::GetParam(char* szKey)
{
	strhash::iterator it;

	it=list.find(string(szKey));
	if(it!=list.end())
		return (char* )(*it).second.data();
	else return NULL;
}

char* CCmdParser::GetParam(int n)
{
	strhash::iterator it;

	char keyname[128];
	sprintf(keyname,"param%d", n);
	return GetParam(keyname);
}

int CCmdParser::GetKind(char *szElement)
{
	if(strstr(szElement, "--")==szElement) return PARAM_DOUBLEMINUS;
	else
		if(strstr(szElement, "-")==szElement) return PARAM_SINGLEMINUS;
		else
			return PARAM_UNDEFINED;
}

void CCmdParser::print()
{
	strhash::iterator it=list.begin();

	for (it=list.begin(); it!=list.end();++it)
		printf("%s | %s\n", (char*)(*it).first.data(), (char*)(*it).second.data());
}

int CCmdParser::GetIntParam(char *szKey)
{
	char* strValue=GetParam(szKey);

	if(strValue) return atoi(strValue);
	else return 0;
}

int CCmdParser::GetIntParam(int n)
{
	char* strValue=GetParam(n);

	if(strValue) return atoi(strValue);
	else return 0;
}

char* CCmdParser::GetOneOfParam(char* szKey)
{
	char* val=GetParam(szKey);
	if(val) return val;

	char szSmallKey[2];
	szSmallKey[0]=szKey[0], szSmallKey[1]=0;
	return GetParam(szSmallKey);
}

int CCmdParser::GetOneOfIntParam(char* szKey)
{
	int val=GetIntParam(szKey);
	if(val) return val;

	char szSmallKey[2];
	szSmallKey[0]=szKey[0], szSmallKey[1]=0;
	return GetIntParam(szSmallKey);
}

char CCmdParser::SplitChars(char* curcmdLine)
{
	string key, val;
	int cmdLen=strlen(curcmdLine)-1;
	char sym[2]={0,0};
	for(int j=1;j<=cmdLen; j++, key.erase())
	{
		sym[0]=curcmdLine[j];
		key.append(sym);
		if(j==cmdLen) return sym[0];
		list.insert(strhash::value_type(key, val));
	}

	return sym[0];
}

//New at 2.0
int CCmdParser::GetParamCount()
{
	return list.size();
}

bool CCmdParser::IsParamExists(char* szKey)
{
	return (GetParam(szKey)!=NULL);
}