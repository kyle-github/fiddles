/* 
 * Copyright 2022 Kyle Hayes
 * Author: Kyle Hayes
 * License: MPL 2.0
 */


#include "codec.h"
#include <stdarg.h>

#define PLCTAG_ERR_OUT_OF_BOUNDS (-1)

static int encode_int8(char *buf, int buf_size, int offset, int8_t val);
static int encode_int16_le(char *buf, int buf_size, int offset, int16_t val);
static int encode_int32_le(char *buf, int buf_size, int offset, int32_t val);
static int encode_int64_le(char *buf, int buf_size, int offset, int64_t val);
static int encode_uint8(char *buf, int buf_size, int offset, uint8_t val);
static int encode_uint16_le(char *buf, int buf_size, int offset, uint16_t val);
static int encode_uint32_le(char *buf, int buf_size, int offset, uint32_t val);
static int encode_uint64_le(char *buf, int buf_size, int offset, uint64_t val);

int encode_buf(char *buf, int buf_size, const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);


    va_end(args);

}

static void encode_int16_le(char *buf, int buf_size, int offset, int16_t val)
{
    if(offset + 2 > buf_size) {
        return PLCTAG_ERR_OUT_OF_BOUNDS;
    }

    buf[offset]   = (char)(uint8_t)(int8_t)val;
    buf[offset+1] = (char)(uint8_t)(int8_t)(val >> 8);
    
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
    } else {
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_int32_t(unsigned char **bp, uint32_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
    } else {
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}

static void pack_int64_t(unsigned char **bp, uint64_t val, int endian)
{
    if (endian == myendian) {
        *((*bp)++) = val;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 56;
    } else {
        *((*bp)++) = val >> 56;
        *((*bp)++) = val >> 48;
        *((*bp)++) = val >> 40;
        *((*bp)++) = val >> 32;
        *((*bp)++) = val >> 24;
        *((*bp)++) = val >> 16;
        *((*bp)++) = val >> 8;
        *((*bp)++) = val;
    }
}



