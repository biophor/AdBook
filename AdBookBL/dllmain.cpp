// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com


// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

HMODULE moduleHandle;

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID /*lpReserved*/
)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        moduleHandle = hModule;
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

