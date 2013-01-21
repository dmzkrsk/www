@echo off

REM Update www_d.dll files
REM copy DLLs\www_d\Debug\www_d.lib lib\www_d.lib >nul
REM copy DLLs\www_d\Release\www_d.lib librel\www_d.lib >nul
REM copy lib\www_d.dll www_d.dll >nul
REM copy DLLs\www_d\wwwfunc.h lib\wwwfunc.h >nul

if not exist lib\ mkdir lib\
if not exist librel\ mkdir librel\

REM Update linkex.dll files
copy DLLs\linkex\Debug\linkex.lib lib\linkex.lib >nul
copy DLLs\linkex\Release\linkex.lib librel\linkex.lib >nul
copy lib\linkex.dll linkex.dll >nul
copy DLLs\linkex\links.h lib\links.h >nul

REM Update murl.dll files
copy DLLs\murl\Debug\murl.lib lib\murl.lib >nul
copy DLLs\murl\Release\murl.lib librel\murl.lib >nul
copy lib\murl.dll murl.dll >nul
copy DLLs\murl\murl.h lib\murl.h >nul

REM Update b64.dll files
copy DLLs\base64\Debug\b64.lib lib\b64.lib >nul
copy DLLs\base64\Release\b64.lib librel\b64.lib >nul
copy lib\b64.dll b64.dll >nul
copy DLLs\base64\b64.h lib\b64.h >nul

REM Update cmdparser.dll files
copy DLLs\cmdparser\Debug\cmdparser.lib lib\cmdparser.lib >nul
copy DLLs\cmdparser\Release\cmdparser.lib librel\cmdparser.lib >nul
copy lib\cmdparser.dll cmdparser.dll >nul
copy DLLs\cmdparser\cmdparser.h lib\cmdparser.h >nul

REM Update hparser.dll files
copy DLLs\hparser\Debug\hparser.lib lib\hparser.lib >nul
copy DLLs\hparser\Release\hparser.lib librel\hparser.lib >nul
copy lib\hparser.dll hparser.dll >nul
copy DLLs\hparser\hparser.h lib\hparser.h >nul

REM Update curl.dll files
copy DLLs\curl_dll\Debug\curl.lib lib\curl.lib >nul
copy DLLs\curl_dll\Release\curl.lib librel\curl.lib >nul
copy lib\curl.dll curl.dll >nul
copy DLLs\curl_dll\curl.h lib\curl.h >nul

REM Update fsys.dll files
copy DLLs\fsys_dll\Debug\fsys.lib lib\fsys.lib >nul
copy DLLs\fsys_dll\Release\fsys.lib librel\fsys.lib >nul
copy lib\fsys.dll fsys.dll >nul
copy DLLs\fsys_dll\filesys.h lib\filesys.h >nul

REM Update mime.dll files
copy DLLs\mime_dll\Debug\mime.lib lib\mime.lib >nul
copy DLLs\mime_dll\Release\mime.lib librel\mime.lib >nul
copy lib\mime.dll mime.dll >nul
copy DLLs\mime_dll\mime.h lib\mime.h >nul

REM Update urld.dll files
copy DLLs\urld_dll\Debug\urld.lib lib\urld.lib >nul
copy DLLs\urld_dll\Release\urld.lib librel\urld.lib >nul
copy lib\urld.dll urld.dll >nul
copy DLLs\urld_dll\urld.h lib\urld.h >nul