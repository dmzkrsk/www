#include <stdio.h>
#include <windows.h>
#include "../../lib/hparser.h"

#define VERSION "1.0"

void main(int argc, char** argv)
{
	printf("Extractor v%s - HTMLParser Simple Shell\n\tTrofimov Alexey 2003\n\tEssential part of www-downloader\n", VERSION);
	if(argc>=4)
	{
		CoInitialize(NULL);
		CHtmlParser parser;
		parser.ParseFile(argv[1], argv[2], argv[3]);
		if(argc==5) if(!strcmp(argv[4],"-c"))parser.ClearEmpty();
		printf("-----------------------\n");
		for(long i=0;i<parser.GetSize();i++)
			printf("%s\n",parser.GetAttribute(i));
		if (parser.GetSize()==0) printf("No elements found!\n");
		printf("-----------------------\n");
		CoUninitialize();
	}
	else
	{
		printf("Bad arguments!\n\tUsing extract[.exe] filename tagname attributename [-c]\n\to\tDelete empty fields\n\n");
	}
}
