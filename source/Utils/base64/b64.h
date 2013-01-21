#ifndef Linkage
#define Linkage extern "C" __declspec(dllimport)
#endif

#define BASE64_MAXLINE  76
#define EOL  "\r\n"

Linkage BOOL EncodeBase64(const char* pszIn, int nInLen, char* pszOut, int nOutSize, int* nOutLen);
Linkage int Base64BufferSize(int nInputSize);
Linkage BOOL DecodeBase64(const char* pszIn, int nInLen, char* pszOut, int nOutSize, int* nOutLen);
Linkage int UnBase64BufferSize(int nInputSize);
