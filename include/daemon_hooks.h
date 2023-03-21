#ifndef KEYBOARD_DAEMON_HOOKS_H
#define KEYBOARD_DAEMON_HOOKS_H

#include <windows.h>
#include <iostream>
#include <filesystem>

#include "toml++/toml.h"
#include "logger/log.hpp"
#include "logger/initializers/console_appender_initializer.hpp"

namespace hooks_vkCodes
{
    toml::array block_vkCode;
    toml::array reassign_vkCode;
}

extern "C" __declspec(dllexport) void set_daemon_hooks();
void parse_configuration();
LRESULT CALLBACK BlockKeys(int iCode, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK ReplaceKey(int iCode, WPARAM wParam, LPARAM lParam);
void PressKey(int vkCode, WPARAM wParam);

#endif //KEYBOARD_DAEMON_HOOKS_H
