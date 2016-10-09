/* RLIB - Convenience library for useful things
 * Copyright (C) 2015-2016  Haakon Sporsheim <haakon.sporsheim@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 3.0 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library.
 * See the COPYING file at the root of the source repository.
 */


/*
 * Modified by Kyle Hayes.  I just want the early init stuff.
 */

#ifndef __R_MACROS_H__
#define __R_MACROS_H__

#if defined(__GNUC__)
#define R_INITIALIZER(f)   static void __attribute__((constructor)) f (void)
#define R_DEINITIALIZER(f) static void __attribute__((destructor))  f (void)
#elif defined(_MSC_VER)
#define R_INITIALIZER(f)                                                    \
  static void __cdecl f (void);                                             \
  __pragma(section(".CRT$XCU",read))                                        \
  __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f;           \
  static void __cdecl f (void)
#define R_DEINITIALIZER(f)                                                  \
  static void __cdecl f (void);                                             \
  static void __cdecl f##_atexit (void) { atexit (f); }                     \
  __pragma(section(".CRT$XCU",read))                                        \
  __declspec(allocate(".CRT$XCU")) void (__cdecl*f##_)(void) = f##_atexit;  \
  static void __cdecl f (void)
#else
#error Your compiler does not support initializers/deinitializers
#endif


#endif /* __R_MACROS_H__ */
