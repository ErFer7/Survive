#pragma once

#include <stdint.h>

// NOTE: This is terrible
#define UTF8(character)                                                            \
    (((uint32_t)(byte)(character[0])) | (((uint32_t)(byte)(character[1])) << 8U) | \
     (((uint32_t)(byte)(character[2])) << 16U) | (((uint32_t)(byte)(character[3])) << 24U))
