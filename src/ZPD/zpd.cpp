#include "zpd.h"

#include <Windows.h>
#include <direct.h>

void ZPD::changeDir() {

    // Change directory to resource path
    char appPath[256];
    GetCurrentDirectory(sizeof(appPath), appPath);
    ZLOG_P("current directory = %s", appPath);
    ZLOG_P("change to resource dir", appPath);
    _wchdir(L"../../resources/");
    GetCurrentDirectory(sizeof(appPath), appPath);
    ZLOG_P("current directory = %s\n", appPath);

}