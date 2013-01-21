#include <stdio.h>
#include <windows.h>
#include "b64.h"

void ShowHelp();

void main(int argc, char*argv[])
{
	printf("Base64 Coding-Encoding tool. Dead_Morozz (c)2003\n\thellas@krasu.ru\ntype base64 -? for help.\n");
	BOOL Encode=TRUE;
	
	if(argc>=2)
		if(!strcmp(argv[1],"-?"))
		{
			ShowHelp();
			return;
		}

	if(argc!=3 && argc!=4)
	{
		printf("Bad syntax\n");
		ShowHelp();
		return;
	}

	char* input, *output;
	int offset=0;
	switch(argc)
	{
	case 4:
		offset=1;
		if(strcmp(argv[1],"-d"))
		{
			printf("Bad syntax\n");
			ShowHelp();
			return;
		}
		Encode=FALSE;
	case 3:
		input=argv[offset+1];
		output=argv[offset+2];
		break;
	}

	HANDLE in=CreateFile(input, GENERIC_READ, FILE_SHARE_WRITE,NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	HANDLE out=CreateFile(output, GENERIC_WRITE, FILE_SHARE_READ,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,NULL);

	if(in==INVALID_HANDLE_VALUE)
	{
		printf("Cannot open file %s for reading", input);
		return;
	}
	if(out==INVALID_HANDLE_VALUE)
	{
		printf("Cannot open file %s for writing", output);
		return;
	}

	DWORD insize=GetFileSize(in,NULL), read;
	char* inbuffer=new char[insize];
	if(!ReadFile(in, inbuffer, insize, &read, NULL))
	{
		printf("Error while reading file...");
		delete inbuffer;
		return;
	}

	int outsize;
	char *outbuffer;
	int realsize;

	if(Encode)
	{
		outsize=Base64BufferSize(insize);
		outbuffer=new char[outsize];

		if(!EncodeBase64(inbuffer, insize, outbuffer, outsize, &realsize))
		{
			printf("Error while encoding...");
			delete outbuffer;
			delete inbuffer;
			return;
		}
	}
	else
	{
		outsize=UnBase64BufferSize(insize);
		outbuffer=new char[outsize];

		if(!DecodeBase64(inbuffer, insize, outbuffer, outsize, &realsize))
		{
			printf("Error while decoding...");
			delete outbuffer;
			delete inbuffer;
			return;
		}
	}

	delete inbuffer;
	if(!WriteFile(out, outbuffer, realsize, &read,NULL))
	{
		printf("Error while writing file...");
		delete outbuffer;
		return;
	}

	delete outbuffer;

	printf("File %s was encoded succesfully an saved in %s\n",input, output);
}

void ShowHelp()
{
	printf("Usage:\n\tbase64[.exe] [-?d] input output\n\t-?\tShows this help\n\t-d\tDecode from Base64\n");
}

