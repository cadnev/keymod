#ifndef KEYBOARD_REPLACE_H
#define KEYBOARD_REPLACE_H

#include <iostream>

#include <windows.h>

LRESULT CALLBACK ReplaceKey(int iCode, WPARAM wParam, LPARAM lParam);
void PressCtrl(bool keyup = false);

#endif //KEYBOARD_REPLACE_H
