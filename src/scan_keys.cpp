#include "scan_keys.h"

bool SkipFirstKey = true;
int current_vkCode;

static HHOOK hHookScanKeys;

LRESULT CALLBACK ScanKeys(int iCode, WPARAM wParam, LPARAM lParam)
{
    auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    switch (wParam)
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            if (SkipFirstKey)
            {
                SkipFirstKey = false;
                break;
            }
            else
            {
                std::cout << "vkCode = " << p->vkCode << '\n';
                current_vkCode = p->vkCode;
                PostMessage(NULL, WM_QUIT, 0, 0);
                UnhookWindowsHookEx(hHookScanKeys);
                SkipFirstKey = true;
                break;
            }
        }
    }

//    return CallNextHookEx(NULL, iCode, wParam, lParam);
    return 1;
}

int sethook()
{
    hHookScanKeys = SetWindowsHookEx(WH_KEYBOARD_LL, &ScanKeys, 0, 0);

    MSG msg;
    BOOL bRet;
    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    {
        if (bRet == -1)
        {
            std::cerr << "bRet = -1\n";
        }
        else
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return current_vkCode;
}
