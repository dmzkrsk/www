#ifndef uLinkage
#define uLinkage __declspec(dllimport)
#endif

uLinkage CString FollowLink(const char* base, char* newlink);
