#include "block.h"

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
