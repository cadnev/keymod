#ifndef KEYBOARD_CONFIGURE_H
#define KEYBOARD_CONFIGURE_H

#include <cstring>
#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>

#include "toml++/toml.h"
#include "logger/log.hpp"

void conf_mode(int argc, char** argv);
void daemon_mode();
void autorun();
void start_no_window();
void add_blocking();
void add_reassignment();
void print_configuration();
void write_configuration();

#endif //KEYBOARD_CONFIGURE_H
