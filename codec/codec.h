/*
 * Copyright 2022 Kyle Hayes
 * Author: Kyle Hayes
 * License: MPL 2.0
 */

#pragma once

/*
 *
 * Format characters.  Number after character is the size.  "i4" is a four byte signed integer.
 *
 * Space is ignored.
 *
 *  in - signed integer, n bytes
 *  un - unsigned integer, n bytes
 *
 * Examples i1, i4, i2, i8
 *          f4, f8
 *
 *  f4 - 32-bit float
 *  f8 - 64-bit float
 *
 *
 */

extern int bug_put_byte(char *buf, int buf_size, int *cursor, uint8_t val);
extern int buf_get_byte(char *buf, int buf_size, int *cursor, uint8_t *val)
extern int buf_put_fmt(char *buf, int buf_size, int *cursor, const char *fmt, ...);
extern int buf_get_fmt(char *buf, int buf_size, int *cursor, const char *fmt, ...);
