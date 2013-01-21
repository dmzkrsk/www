#ifndef CLinkage
#define CLinkage __declspec (dllimport)
#endif

class CLinkage CStrings:public CStringArray
{
public:
	CStrings (const CStrings& stringSrc);
	CStrings (const CStringArray& stringSrc);
	CStrings();
	CStrings& operator=(const CStrings& stringSrc);
	CStrings& operator=(const CStringArray& stringSrc);
};

#define HTML_A 0
#define HTML_APPLET 1
#define HTML_FORM 2
#define HTML_IMG 3
#define HTML_EMBED 4
#define HTML_FRAME 5
#define HTML_IFRAME 6
#define HTML_XML 7
#define HTML_BGSOUND 8
#define HTML_INPUT 9
#define HTML_LINK 10
#define HTML_TABLE 11
#define HTML_TH 12
#define HTML_TD 13
#define HTML_AREA 14
#define HTML_BODY 15
#define HTML_IMG_DYNSRC 16

CLinkage CStrings extractLinks(const char* htmlText);
CLinkage CStrings extractApplets(const char* htmlText);
CLinkage CStrings extractForms(const char* htmlText);
CLinkage CStrings extractPictures(const char* htmlText);
CLinkage CStrings extractEmbedded(const char* htmlText);
CLinkage CStrings extractFrames(const char* htmlText);
CLinkage CStrings extractInlineFrames(const char* htmlText);
CLinkage CStrings extractXML(const char* htmlText);
CLinkage CStrings extractSounds(const char* htmlText);
CLinkage CStrings extractLinked(const char* htmlText);
CLinkage CStrings extractTablesBgr(const char* htmlText);
CLinkage CStrings extractTableLinesBgr(const char* htmlText);
CLinkage CStrings extractTableHLinesBgr(const char* htmlText);
CLinkage CStrings extractAreas(const char* htmlText);
CLinkage CStrings extractBodyBgr(const char* htmlText);
CLinkage CStrings extractDynamicImage(const char* htmlText);

CLinkage CStrings extractElements(int HTMLTag, const char* htmlText);
CLinkage CStrings extractAll(const char* htmlText);


