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
        pch1 = std::strstr(arg, "-c");
        pch2 = std::strstr(arg, "--configure");
        if ((pch1 != nullptr) || (pch2 != nullptr))
        {
            isConfigure = true;
            break;
        }
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
        std::cout << "[3] Print configuration\n";
        std::cout << "[4] Write configuration to file\n";
        std::cout << "[5] Exit from configuration mode\n\n";
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
                print_configuration();
                break;
            case 4:
                write_configuration();
                break;
            case 5:
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

    static HINSTANCE hinstDLL = LoadLibrary(TEXT("libscanKeys.dll"));
    if (hinstDLL == nullptr)
        std::cerr << "Some error while LoadLibrary: " << GetLastError() << '\n';

    HOOKPROC sethook = (HOOKPROC) GetProcAddress(hinstDLL, "sethook");
    if (sethook == nullptr)
        std::cerr << "Some error while GetProcAddress: " << GetLastError() << '\n';

    std::cout << "Press the key to block: ";

    current_vkCode = sethook(0, 0, 0);
    block_vkCode.push_back(current_vkCode);
}

void add_reassignment()
{
    int replace1;
    int replace2;

    static HINSTANCE hinstDLL = LoadLibrary(TEXT("libscanKeys.dll"));
    if (hinstDLL == nullptr)
        std::cerr << "Some error while LoadLibrary: " << GetLastError() << '\n';

    HOOKPROC sethook = (HOOKPROC) GetProcAddress(hinstDLL, "sethook");
    if (sethook == nullptr)
        std::cerr << "Some error while GetProcAddress: " << GetLastError() << '\n';

    std::cout << "Replace key: ";
    replace1 = sethook(0, 0, 0);
    std::cout << "With key: ";
    replace2 = sethook(0, 0, 0);
    reassign_vkCode.push_back(toml::inserter{toml::array{replace1, replace2}});
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

        if (!block_vkCode.empty())
            config.insert_or_assign("block", block_vkCode);

        if (!reassign_vkCode.empty())
            config.insert_or_assign("reassign", reassign_vkCode);

        out << config;
        out.close();

        std::cout << "Configuration saved.";
    }
    else
    {
        std::cerr << "Can't create configuration file.";

        return;
    }
}

void daemon_mode()
{
    
}