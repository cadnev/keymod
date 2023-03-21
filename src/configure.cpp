#include "configure.h"

toml::array block_vkCode;
toml::array reassign_vkCode;

void conf_mode(int argc, char **argv)
{
    bool isConfigure = false;

    for (int i = 0; i < argc; ++i)
    {
        auto arg = argv[i];
        std::cout << arg;
        char* pch1;
        char* pch2;
        char* pch3;
        pch1 = std::strstr(arg, "-c");
        pch2 = std::strstr(arg, "--configure");
        pch3 = std::strstr(arg, "--autorun");
        if ((pch1 != nullptr) || (pch2 != nullptr))
        {
            isConfigure = true;
            break;
        }
        else if (pch3 != nullptr)
            autorun();
        else
            continue;
    }

    if (!isConfigure)
        return;

    std::cout << "\nKey modifier\n";

    while (true)
    {
        std::cout << "\nConfiguration mode:\n";
        std::cout << "[1] Block the key\n";
        std::cout << "[2] Reassign the key\n";
        std::cout << "[3] Enable\\Disable autorun\n";
        std::cout << "[4] Print configuration\n";
        std::cout << "[5] Write configuration to file\n";
        std::cout << "[6] Exit from configuration mode\n\n";
        std::cout << "> ";
        int choice;
        std::cin >> choice;
        std::cin.sync();

        switch (choice) {
            case 1:
                add_blocking();
                break;
            case 2:
                add_reassignment();
                break;
            case 3:
                ShellExecuteA(nullptr, "runas", "keymod.exe", "--autorun", nullptr, SW_NORMAL);
                break;
            case 4:
                print_configuration();
                break;
            case 5:
                write_configuration();
                break;
            case 6:
                std::cout << "Goodbye.";
                return;
            default:
                std::cout << "Wrong option.\n";
                std::cin.clear();
                std::cin.sync();
        }

    }
}

void add_blocking()
{
    int current_vkCode;

    static HINSTANCE hinstDLL = LoadLibrary(TEXT("hooks.dll"));
    if (hinstDLL == nullptr)
        LOGF << "Some error while LoadLibrary: " << GetLastError() << '\n';

    HOOKPROC sethook = (HOOKPROC) GetProcAddress(hinstDLL, "sethook");
    if (sethook == nullptr)
        LOGF << "Some error while GetProcAddress: " << GetLastError() << '\n';

    std::cout << "Press the key to block: ";

    current_vkCode = sethook(0, 0, 0);
    block_vkCode.push_back(current_vkCode);
}

void add_reassignment()
{
    int replace1;
    int replace2;

    static HINSTANCE hinstDLL = LoadLibrary(TEXT("hooks.dll"));
    if (hinstDLL == nullptr)
        LOGF << "Some error while LoadLibrary: " << GetLastError() << '\n';

    HOOKPROC sethook = (HOOKPROC) GetProcAddress(hinstDLL, "sethook");
    if (sethook == nullptr)
        LOGF << "Some error while GetProcAddress: " << GetLastError() << '\n';

    std::cout << "Replace key: ";
    replace1 = sethook(0, 0, 0);
    std::cout << "With key: ";
    replace2 = sethook(0, 0, 0);
    reassign_vkCode.push_back(toml::inserter{toml::array{replace1, replace2}});
}

void autorun()
{
    HKEY hKey;
    LONG lresult = 0;

    char value[0x100];
    DWORD buffersize = sizeof(value);

    char szPath[0x100];
    GetModuleFileName(nullptr, szPath, sizeof(szPath));

    lresult = RegGetValueA(
            HKEY_LOCAL_MACHINE,
            R"(Software\Microsoft\Windows\CurrentVersion\Run)",
            "keymod",
            RRF_RT_REG_SZ,
            nullptr,
            (PVOID)&value,
            &buffersize);

    if (lresult == ERROR_FILE_NOT_FOUND)
    {
        std::cout << "\n[*] Registry value not found.\n";

        lresult = RegCreateKeyExA(
                HKEY_LOCAL_MACHINE,
                R"(Software\Microsoft\Windows\CurrentVersion\Run)",
                0,
                "",
                REG_OPTION_NON_VOLATILE,
                KEY_SET_VALUE,
                nullptr,
                &hKey,
                nullptr);

        if (hKey && (lresult == ERROR_SUCCESS))
        {
            std::cout << "[*] Key created.\n";

            lresult = RegSetValueEx(
                    hKey,
                    "keymod",
                    0,
                    REG_SZ,
                    (LPBYTE)szPath,
                    strlen(szPath));
            RegCloseKey(hKey);

            if (lresult == ERROR_SUCCESS)
                std::cout << "[+] Registry value written!\nAutorun enabled.\n";
            else
                LOGF << "[x] Failure while setting value: " << lresult << '\n';
        }
        else
            LOGF << "[x] Failure while creating key: " << lresult << '\n';
    }
    else if (lresult == ERROR_SUCCESS)
    {
        std::cout << "\n[*] Registry value found.\n";

        lresult = RegDeleteKeyValueA(
                HKEY_LOCAL_MACHINE,
                R"(Software\Microsoft\Windows\CurrentVersion\Run)",
                "keymod"
                );

        if (lresult == ERROR_SUCCESS)
            std::cout << "[+] Registry value deleted!\nAutorun disabled.\n";
        else
            LOGF << "[x] Failure while deleting key value.\n";
    }

    system("pause");
}

void print_configuration()
{
    std::cout << "Current configuration\n";

    if (!block_vkCode.empty())
    {
        std::cout << "Keys to block:\n";

        for (int i = 0; i < block_vkCode.size(); ++i)
        {
            std::cout << '\t' << **block_vkCode.get_as<int64_t>(i) << '\n';
        }
    }

    if (!reassign_vkCode.empty())
    {
        std::cout << "Keys to reassign:\n";

        for (int i = 0; i < reassign_vkCode.size(); ++i)
        {
            std::cout << '\t' << *reassign_vkCode.get_as<toml::array>(i)->get_as<int64_t>(0) << " -> "
                << *reassign_vkCode.get_as<toml::array>(i)->get_as<int64_t>(1) << '\n';
        }
    }

    std::cout << "\n\n";
}

void write_configuration()
{
    std::ofstream out;

    out.open("configuration.toml");

    if (out.is_open())
    {
        toml::table config;

        config.insert_or_assign("block", block_vkCode);
        config.insert_or_assign("reassign", reassign_vkCode);

        out << config;
        out.close();

        std::cout << "Configuration saved.\n";
    }
    else
    {
        LOGF << "Can't create configuration file.\n";

        return;
    }
}

void daemon_mode()
{
    static HINSTANCE hinstDLL = LoadLibrary(TEXT("hooks.dll"));
    if (hinstDLL == nullptr)
        LOGF << "Some error while LoadLibrary:" << GetLastError() << '\n';

    HOOKPROC set_daemon_hooks = (HOOKPROC) GetProcAddress(hinstDLL, "set_daemon_hooks");
    if (set_daemon_hooks == nullptr)
        LOGF << "Some error while GetProcAddress: " << GetLastError() << '\n';

    set_daemon_hooks(0, 0, 0);
}
