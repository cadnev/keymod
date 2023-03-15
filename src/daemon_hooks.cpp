#include "daemon_hooks.h"

void set_daemon_hooks()
{
    parse_configuration();

    if (!hooks_vkCodes::block_vkCode.empty())
        SetWindowsHookEx(WH_KEYBOARD_LL, &BlockKeys, 0, 0);

    if (!hooks_vkCodes::reassign_vkCode.empty())
        SetWindowsHookEx(WH_KEYBOARD_LL, &ReplaceKey, 0, 0);

    MSG msg;
    BOOL bRet;

    while( (bRet = GetMessage( &msg, NULL, 0, 0 )) != 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void parse_configuration()
{
    toml::parse_result config;

    try
    {
        config = toml::parse_file( "configuration.toml" );

    }
    catch (toml::v3::ex::parse_error&)
    {
        std::cerr << "Configuration file was not found. To enter configuration mode, run .\\keyboard.exe -c or "
                  << ".\\keyboard.exe --configure";
    }

    hooks_vkCodes::block_vkCode = *config["block"].as<toml::array>();
    hooks_vkCodes::reassign_vkCode = *config["reassign"].as<toml::array>();

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
            for (int i = 0; i < hooks_vkCodes::block_vkCode.size(); ++i)
            {
                int target_vkCode = **hooks_vkCodes::block_vkCode.get_as<int64_t>(i);
                bEatKeystroke = bEatKeystroke || ((p->vkCode == target_vkCode));
            }
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
        case WM_KEYUP:
        {
            for (int i = 0; i < hooks_vkCodes::reassign_vkCode.size(); ++i)
            {
                int target_vkCode = **hooks_vkCodes::reassign_vkCode.get_as<toml::array>(i)->get_as<int64_t>(0);
                int reassign_vkCode = **hooks_vkCodes::reassign_vkCode.get_as<toml::array>(i)->get_as<int64_t>(1);

                if (p->vkCode == target_vkCode)
                    PressKey(reassign_vkCode, wParam);
                else
                    return CallNextHookEx(NULL, iCode, wParam, lParam);

                break;
            }
        }
    }

    return 1;
}

void PressKey(int vkCode, WPARAM wParam)
{
    INPUT inputs[1] = {};
    ZeroMemory(inputs, sizeof(inputs));

    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = vkCode;
    if (wParam == WM_KEYUP)
        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP;

    UINT uSent = SendInput(ARRAYSIZE(inputs), inputs, sizeof(INPUT));
    if (uSent != ARRAYSIZE(inputs))
    {
        std::cerr << L"SendInput failed: 0x" << HRESULT_FROM_WIN32(GetLastError()) << '\n';
    }
}
