#ifndef KEYBOARD_SCAN_KEYS_H
#define KEYBOARD_SCAN_KEYS_H

#include <windows.h>
#include <iostream>


LRESULT CALLBACK ScanKeys(int iCode, WPARAM wParam, LPARAM lParam);
extern "C" __declspec(dllexport) int sethook();

#endif //KEYBOARD_SCAN_KEYS_H
