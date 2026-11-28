/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        macros.h                        */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 02:23:33    */
/*                                                             */
/*   updated: 2026/11/27 06:21:57                              */
/*                                                             */
/* *********************************************************** */

#pragma once

#if defined(__APPLE__)
#    define APPLE_MACHINE 1
#else
#    define APPLE_MACHINE 0
#endif

#if defined(__linux__)
#    define LINUX_MACHINE 1
#else
#    define LINUX_MACHINE 0
#endif

#if defined(_WIN32)
#    define WINDOWS_MACHINE 1
#	 include <windows.h>
#else
#    define WINDOWS_MACHINE 0
#endif

#ifndef bool
#    define bool  int
#    define true  1
#    define false 0
#endif

#define FILE __FILE__
#define LINE __LINE__
#define FUNC __func__
#define TAB  4

#ifndef DEBUG
#    define DEBUG 1
#endif

#if defined(__GNUC__) || defined(__clang__)
#  define PRINTF_ATTR(f, a) __attribute__((__format__(__printf__, f, a)))
#else
#  define PRINTF_ATTR(f, a)
#endif

#define print(...)  (DEBUG ? fputs(format(__VA_ARGS__), stdout) : 0)
#define eprint(...) fputs(format(__VA_ARGS__), stderr)
#define help(...) fputs(format(__VA_ARGS__), stdout)
// DYNAMIC ARRAY
#define expand(T, e)  \
    T *e;             \
    size_t e##_count; \
    size_t e##_size;
#define zero_init(e) \
    e = NULL;        \
    e##_count = 0;   \
    e##_size = 0;
#define append(p, c)                                              \
    {                                                             \
        __typeof__(*p) appended = (c);                            \
        if (p##_size == 0)                                        \
            p = arena_alloc((p##_size = 10), sizeof(*p));         \
        else if (p##_count + 2 == p##_size) {                     \
            void *tmp = arena_alloc((p##_size *= 2), sizeof(*p)); \
            memcpy(tmp, p, (p##_count * sizeof(*p)));             \
            p = tmp;                                              \
        }                                                         \
        p[p##_count++] = appended;                                \
    }

// TEXT COLORS
#define RESET      "\033[0m"
#define BOLD       "\033[1m"
#define GREEN(fmt) BOLD "\033[0;32m" fmt RESET
#define RED(fmt)   BOLD "\033[0;31m" fmt RESET
#define CYAN(fmt)  BOLD "\033[0;36m" fmt RESET
#define DIM(fmt)   "\033[2m" fmt RESET

#if APPLE_MACHINE
#    include <mach-o/dyld.h>
#elif LINUX_MACHINE
typedef struct _IO_FILE *ioFile;
#endif
