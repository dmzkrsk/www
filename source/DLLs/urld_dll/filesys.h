typedef struct
{
	char DriveLetter[_MAX_DRIVE];
	char FullPath[_MAX_PATH];
	char FilenameWithoutExtension[_MAX_FNAME];
	CString FilenameWithExtension;
	CString Data;
	char Extension[_MAX_EXT];
}SPLITINFO;

class CFileSystem
{
public:
	static BOOL CreateDirectories(const char* dirpath);
	static SPLITINFO SplitFileName(char* filename);
	static CString CollapseFilename(CString String);
	static BOOL RelativePath(char* szStart, char* To, CString* Path);
};

class CModFileSystem:public CFileSystem
{
public:
	static SPLITINFO splitRelFileName(char* filename, char* server, char *dir_root);
};
