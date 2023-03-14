#include "daemon_hooks.h"

void set_daemon_hooks()
{
    parse_configuration();

    for (int i = 0; i < hooks_vkCodes::block_vkCode.size(); ++i)
    {
        SetWindowsHookEx(WH_KEYBOARD_LL, &BlockKeys, 0, 0);
    }
    /*if (!reassign_vkCode.empty())
        static HHOOK hHookReassignKeys = SetWindowsHookEx(WH_KEYBOARD_LL, &ReplaceKey, 0, 0);*/

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