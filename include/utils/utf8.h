#pragma once

// TODO: This is terrible
#define UTF8(character)                                                                                      \
    (((unsigned int)(unsigned char)(character[0])) | (((unsigned int)(unsigned char)(character[1])) << 8U) | \
     (((unsigned int)(unsigned char)(character[2])) << 16U) | (((unsigned int)(unsigned char)(character[3])) << 24U))
