# Keyboard modificator
---
In case of mechanical damage or while cleaning the keyboard, the key may stick or stop working.  In this case, you can use this small program for Windows.
The keyboard modifier can **reassign** or **block** almost any keys on the keyboard (the only restriction is that a [Virtual-Key Code](https://learn.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes) defined for the key).
`Keymod` is written on C++ using WinAPI as well as some other open-source libraries, which will be discussed later.

### Table of contents

1. [Installation](#installation)
2. [Usage](#usage)
3. [How it works](#how-it-works)

## Installation
1. Clone this repository;
2. Compile project using `CMakeLists.txt`;
3. Copy `cmake-build-debug\keymod.exe` and `cmake-build-debug\hooks.dll` wherever you want (optional).

## Usage
1. First you need to configure `keymod`. To do this, open `powershell` (or `cmd`) and run the executable with a special flag:
`.\keymod.exe -c` or `.\keymod.exe --configure`
2. Select an action. For example:
```
Key modifier

Configuration mode:
[1] Block the key
[2] Reassign the key
[3] Enable\Disable autorun
[4] Print configuration
[5] Write configuration to file
[6] Exit from configuration mode

> 2
```
3. Follow the prompts on the screen. First, press the replacement key. Second, press on what to replace:
```
> 2
Replace key: vkCode = 162
With key: vkCode = 91
```
> Reassign left CTRL key (vkCode = 162) to left windows key (vkCode = 91).
4. Select other actions if necessary. Then select `5` to write configuration to file and  `6` to exit from configuration mode;
5. Now you can run the program in the background and close the terminal:
`.\keymod.exe --daemon`
> Also, using the CLI, you can enable/disable autorun by adding an key to the registry.

## How it works
The project consists of two components:
- executable file that contains the CLI functions;
- shared library that contains callback functions and hooks setup.

Executable file writes the settings to the configuration file (`configuration.toml` by default). Then shared library parses configuration and install hooks into windows hook chain.
The configuration file is written in [TOML](https://toml.io/en/) format.

---
Special thanks to:
- [@temikfart](https://github.com/temikfart) for simple [logger](https://github.com/temikfart/logger) and prompt feedback;
- [@marzer](https://github.com/marzer) for [tomlplusplus](https://github.com/marzer/tomlplusplus).