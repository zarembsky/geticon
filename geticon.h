#include <napi.h>

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//
#pragma once
//
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// reference additional headers your program requires here
#include <minwindef.h>
#include <processthreadsapi.h>
#include <securitybaseapi.h>
#include <winnt.h>
#include <sddl.h>

#include <sddl.h>
#include <AccCtrl.h>
#include <Aclapi.h>

#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>

#include <wincrypt.h>

#include <winsock2.h>
#include <WS2tcpip.h>
#include <iphlpapi.h>

#include <tlhelp32.h>

#include <wtsapi32.h>
#include <Knownfolders.h>
#include <Shlobj.h>
#include <shellapi.h>

#include <olectl.h>
#pragma comment(lib, "oleaut32.lib")

namespace geticon {
    std::string geticon(int pid);
    Napi::String GetIconWrapped(const Napi::CallbackInfo& info);
    Napi::Object Init(Napi::Env env, Napi::Object exports);
}