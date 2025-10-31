#pragma once

#ifdef linux
#include "linux/linux_chronometer.h"
#endif

#ifdef _WIN32
#include "windows/windows_chronometer.h"
#endif
