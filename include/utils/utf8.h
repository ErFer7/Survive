#pragma once

#include <stdint.h>

// This is so ugly
#define UTF8(string)                                                                                     \
    ((uint32_t)(byte)((u8"" string "\0\0\0")[0]) | ((uint32_t)(byte)((u8"" string "\0\0\0")[1]) << 8U) | \
     ((uint32_t)(byte)((u8"" string "\0\0\0")[2]) << 16U) | ((uint32_t)(byte)((u8"" string "\0\0\0")[3]) << 24U))
