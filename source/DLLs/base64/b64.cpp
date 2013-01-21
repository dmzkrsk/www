#include <windows.h>
#define Linkage extern "C" __declspec(dllexport)
#include "b64.h"
#include <stdio.h>

int DeBase64Tab(char cSym);
char m_base64tab[];

Linkage BOOL EncodeBase64(const char* pszIn, int nInLen, char* pszOut, int nOutSize, int* nOutLen)
{/*
  //Input Parameter validation
  ASSERT(pszIn);
  ASSERT(pszOut);
  ASSERT(nOutSize);*/
//  ASSERT(nOutSize >= Base64BufferSize(nInLen));
	if (nOutSize < Base64BufferSize(nInLen)) return FALSE;

#ifndef _DEBUG
  //justs get rid of "unreferenced formal parameter"
  //compiler warning when doing a release build
  nOutSize;
#endif

  //Set up the parameters prior to the main encoding loop
  int nInPos  = 0;
  int nOutPos = 0;
  int nLineLen = 0;

  // Get three characters at a time from the input buffer and encode them
  for (int i=0; i<nInLen/3; ++i) 
  {
    //Get the next 2 characters
    int c1 = pszIn[nInPos++] & 0xFF;
    int c2 = pszIn[nInPos++] & 0xFF;
    int c3 = pszIn[nInPos++] & 0xFF;

    //Encode into the 4 6 bit characters
    pszOut[nOutPos++] = m_base64tab[(c1 & 0xFC) >> 2];
    pszOut[nOutPos++] = m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
    pszOut[nOutPos++] = m_base64tab[((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
    pszOut[nOutPos++] = m_base64tab[c3 & 0x3F];
    nLineLen += 4;

    //Handle the case where we have gone over the max line boundary
    if (nLineLen >= BASE64_MAXLINE-3) 
    {
      char* cp = EOL;
      pszOut[nOutPos++] = *cp++;
      if (*cp) 
        pszOut[nOutPos++] = *cp;
      nLineLen = 0;
    }
  }

  // Encode the remaining one or two characters in the input buffer
  char* cp;
  switch (nInLen % 3) 
  {
    case 0:
    {
      cp = EOL;
      pszOut[nOutPos++] = *cp++;
      if (*cp) 
        pszOut[nOutPos++] = *cp;
      break;
    }
    case 1:
    {
      int c1 = pszIn[nInPos] & 0xFF;
      pszOut[nOutPos++] = m_base64tab[(c1 & 0xFC) >> 2];
      pszOut[nOutPos++] = m_base64tab[((c1 & 0x03) << 4)];
      pszOut[nOutPos++] = '=';
      pszOut[nOutPos++] = '=';
      cp = EOL;
      pszOut[nOutPos++] = *cp++;
      if (*cp) 
        pszOut[nOutPos++] = *cp;
      break;
    }
    case 2:
    {
      int c1 = pszIn[nInPos++] & 0xFF;
      int c2 = pszIn[nInPos] & 0xFF;
      pszOut[nOutPos++] = m_base64tab[(c1 & 0xFC) >> 2];
      pszOut[nOutPos++] = m_base64tab[((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
      pszOut[nOutPos++] = m_base64tab[((c2 & 0x0F) << 2)];
      pszOut[nOutPos++] = '=';
      cp = EOL;
      pszOut[nOutPos++] = *cp++;
      if (*cp) 
        pszOut[nOutPos++] = *cp;
      break;
    }
    default: 
    {
//      ASSERT(FALSE); 
	  return FALSE;
      break;
    }
  }
//  pszOut[nOutPos] = 0;
  if(nOutLen)*nOutLen = nOutPos;
  return TRUE;
}

Linkage BOOL DecodeBase64(const char* pszIn, int nInLen, char* pszOut, int nOutSize, int* nOutLen)
{
	int nInPos=0;
	int nOutPos=0;
	int nLineLen=0;

	char* pszTmp=new char[nInLen];
	int nTmpPos=0;
	int nCut=0;
	for(int nCnt=0;nCnt<nInLen;nCnt++)
		if(pszIn[nCnt]=='=' || pszIn[nCnt]=='\n' || pszIn[nCnt]=='\r') {nCut++;continue;}
		else pszTmp[nInPos++]=pszIn[nCnt];

	nInPos=0;
	nInLen-=nCut;

	for (int i=0; i<nInLen/4; ++i) 
	{
		int c1 = DeBase64Tab(pszTmp[nInPos++]);
		int c2 = DeBase64Tab(pszTmp[nInPos++]);
		int c3 = DeBase64Tab(pszTmp[nInPos++]);
		int c4 = DeBase64Tab(pszTmp[nInPos++]);

		pszOut[nOutPos++]=(c1<<2)|((c2&0x30)>>4);
		pszOut[nOutPos++]=((c2&0x0F)<<4)|((c3&0x3C)>>2);
		pszOut[nOutPos++]=((c3&0x03)<<6)|c4;

	}

	switch(nInLen%4)
	{
	case 0:
		break;
	case 1:
		delete pszTmp;
		return FALSE;
		break;
	case 2:
	{
		int c1 = DeBase64Tab(pszTmp[nInPos++]);
		int c2 = DeBase64Tab(pszTmp[nInPos++]);
		pszOut[nOutPos++]=(c1<<2)|((c2&0x30)>>4);
		break;
	}
	case 3:
	{
		int c1 = DeBase64Tab(pszTmp[nInPos++]);
		int c2 = DeBase64Tab(pszTmp[nInPos++]);
		int c3 = DeBase64Tab(pszTmp[nInPos++]);
		pszOut[nOutPos++]=(c1<<2)|((c2&0x30)>>4);
		pszOut[nOutPos++]=((c2&0x0F)<<4)|((c3&0x3C)>>2);
		break;
	}
	default:
		delete pszTmp;
		return FALSE;
		break;
	}
	
//	pszOut[nOutPos] = 0;
	if(nOutLen)*nOutLen = nOutPos;
	delete pszTmp;
	return TRUE;
}

Linkage int Base64BufferSize(int nInputSize)
{
  int nOutSize = (nInputSize+2)/3*4;                    // 3:4 conversion ratio
  nOutSize += strlen(EOL)*nOutSize/BASE64_MAXLINE + 3;  // Space for newlines and NUL
  return nOutSize;
}

Linkage int UnBase64BufferSize(int nInputSize)
{
	int l=nInputSize/(BASE64_MAXLINE+strlen(EOL))+1;
	int nOutSize=(nInputSize-l*strlen(EOL))*3/4;

	return nOutSize;
}

int DeBase64Tab(char cSym)
{
	if(cSym>=65 && cSym<=90) return cSym-65;
	if(cSym>=97 && cSym<=122) return cSym-71;
	if(cSym>=48 && cSym<=57) return cSym+4;
	if(cSym==43) return 62;
	if(cSym==47) return 63;

	return -1;
}
//                    01234567890123456789012345
char m_base64tab[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                                      "abcdefghijklmnopqrstuvwxyz0123456789+/";
//                                     67890123456789012345678901234567890123

Linkage int MIMEAuthBufferSize(char* szLog, char* szPassword)
{
	int nLen=strlen(szLog)+strlen(szPassword);
	return Base64BufferSize(nLen)+3;
}

Linkage void MIMEAuth(char* szLog, char* szPassword, char* szAuthString, int nAuthStringSize)
{
	int nLen=strlen(szLog)+strlen(szPassword)+1;
	char* szLogPwd=new char[nLen+1];
	sprintf(szLogPwd,"%s:%s", szLog, szPassword);

	int realsize;

	EncodeBase64(szLogPwd, nLen, szAuthString, nAuthStringSize, &realsize);
	
	szAuthString[realsize]=0;
	delete szLogPwd;
}
