// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h";
#include "stdio.h";
#include <iostream>;
#include <fstream>;
#define EXPORTING_DLL
#include "dllmain.h";
#include <iostream>
using namespace std;

const int PROTECTED_BYTES = 7;

BOOL WINAPI DllMain(
    HINSTANCE hDllHandle,
    DWORD     nReason,
    LPVOID    Reserved)
{   
    BOOL bSuccess = TRUE;
    switch (nReason)
    {
    case DLL_PROCESS_ATTACH:
        hookingFunction();
        break;
    case DLL_PROCESS_DETACH:
        break;
    case DLL_THREAD_ATTACH:
        break;
    case DLL_THREAD_DETACH:
        break;
    }

    return bSuccess;

}


void maliciousCode() {
    MessageBox(NULL, TEXT("Malicious code"),
        TEXT("Malicious code"), MB_OK);
}

int setJMP(LPVOID* from, LPVOID* to) {
    DWORD protection;
    if (!VirtualProtect(from, PROTECTED_BYTES, PAGE_EXECUTE_READWRITE, &protection)) { // We must be able to write to it (don't necessarily need execute and read)
        return 0;
    }

    *(char*)from = 0xE9; // jmp opcode
    *(int*)(from + 1) = (int)((int)to - (int)from - PROTECTED_BYTES); // relative addr

    return VirtualProtect(from, PROTECTED_BYTES, protection, &protection); // Restore original protection
}


void hookingFunction() {
    MessageBox(NULL, TEXT("Hooking code"),
        TEXT("Hooking code"), MB_OK);

    LPVOID origRealFileAddress = (LPVOID)GetProcAddress(LoadLibrary(L"kernel32"), "ReadFile");
    setJMP(&origRealFileAddress, (LPVOID*)&maliciousCode);
 

}