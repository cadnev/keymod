#include <iostream>
//#include <windef.h>
//#include <winuser.h>
#include <windows.h>

LRESULT CALLBACK BlockKeys(int iCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ReplaceKey(int iCode, WPARAM wParam, LPARAM lParam);
void PressCtrl(bool keyup = false);

int main(int argc, char **argv)
{
//    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, BlockKeys, NULL, NULL);
    HHOOK hHook = SetWindowsHookEx(WH_KEYBOARD_LL, ReplaceKey, NULL, NULL);
    MSG msg = { 0 };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    UnhookWindowsHookEx(hHook);

    return 0;
}

LRESULT CALLBACK BlockKeys(int iCode, WPARAM wParam, LPARAM lParam)
{
    auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);
    bool bEatKeystroke = false;

    switch (wParam)
    {
        case WM_KEYDOWN:
        case WM_KEYUP:
        {
            bEatKeystroke = ((p->vkCode == VK_ADD));
            break;
        }
    }

    if (bEatKeystroke)
    {
        return 1;
    }
    else
    {
        return CallNextHookEx(NULL, iCode, wParam, lParam);
    }
}

LRESULT CALLBACK ReplaceKey(int iCode, WPARAM wParam, LPARAM lParam)
{
    auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    switch (wParam)
    {
        case WM_KEYDOWN:
        {
            if (p->vkCode == VK_NONAME) // 0xff - fn key on laptops
                PressCtrl();
            else
                return CallNextHookEx(NULL, iCode, wParam, lParam);
            break;
        }
        case WM_KEYUP:
        {
            if (p->vkCode == VK_NONAME) // 0xff - fn key on laptops
                PressCtrl(true);
            else
                return CallNextHookEx(NULL, iCode, wParam, lParam);
            break;
        }
    }

    return 1;

}

void PressCtrl(bool keyup)
{
    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = VK_LCONTROL;
    if (keyup)
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        std::cout << L"SendInput failed: 0x" << HRESULT_FROM_WIN32(GetLastError()) << '\n';
    }
}