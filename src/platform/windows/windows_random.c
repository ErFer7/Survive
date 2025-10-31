#include "../../../include/platform/windows/windows_random.h"

float randomf(int min, int max) { return (float)((rand() % (max - min + 1)) + min); }
