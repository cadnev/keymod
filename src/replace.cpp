#include "replace.h"

LRESULT CALLBACK ReplaceKey(int iCode, WPARAM wParam, LPARAM lParam)
{
    auto p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

    switch (wParam)
    {
        case WM_KEYDOWN:
        {
            if (p->vkCode == VK_LWIN)
                PressCtrl();
            else
                return CallNextHookEx(NULL, iCode, wParam, lParam);
            break;
        }
        case WM_KEYUP:
        {
            if (p->vkCode == VK_LWIN)
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