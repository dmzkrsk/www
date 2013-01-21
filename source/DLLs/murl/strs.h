#ifndef sLinkage
#define sLinkage extern "C" __declspec(dllexport)
#endif

sLinkage char *stristr(const char *string, const char *strCharSet);