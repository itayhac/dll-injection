// File: SampleDLL.h
//
#ifndef INDLL_H
#define INDLL_H

#ifdef EXPORTING_DLL
extern __declspec(dllexport) void hookingFunction();
#else
extern __declspec(dllimport) void hookingFunction();
#endif

#endif