#ifndef KEYBOARD_DAEMON_HOOKS_H
#define KEYBOARD_DAEMON_HOOKS_H

#include <windows.h>

#include "toml++/toml.h"

#include "replace.h"

namespace hooks_vkCodes
{
    toml::array block_vkCode;
    toml::array reassign_vkCode;
}

extern "C" __declspec(dllexport) void set_daemon_hooks();
void parse_configuration();
LRESULT CALLBACK BlockKeys(int iCode, WPARAM wParam, LPARAM lParam);

#endif //KEYBOARD_DAEMON_HOOKS_H
