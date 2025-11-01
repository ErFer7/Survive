#pragma once

#ifdef linux
#include "linux/linux_terminal.h"
#endif

#ifdef _WIN32
#include "windows/windows_terminal.h"
#endif
