/*
 * File: helper.c
 *
 * General helper functions
 */

#ifdef STLINK_HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <sys_time.h>
#endif // STLINK_HAVE_SYS_TIME_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#if defined(_WIN32)
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#endif

#include "helper.h"

uint32_t time_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (uint32_t) (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

int32_t arg_parse_freq(const char *str) {
    char *tail;
    int32_t value = (int32_t) strtol(str, &tail, 10);

    if (tail[0] == 'M' && tail[1] == '\0') {
        value = value*1000;
    } else if ((tail[0] != 'k' || tail[1] != '\0') && tail[0] != '\0') {
        return -1;
    }

    return value;
}

const char* get_chips_path(char* argv0, char* buf, size_t size) {
#if defined(_WIN32)
    (void)argv0; // Unused on Windows
    DWORD len = GetModuleFileNameA(NULL, buf, (DWORD)size);
    if (len == 0 || len >= size) {
        // Fallback or error
        strncpy(buf, ".\\chips", size);
        return buf;
    }
    char* delim = strrchr(buf, '\\');
    if (delim) {
        *(delim + 1) = '\0';
        strncat(buf, "chips", size - strlen(buf) - 1);
    } else {
        strncpy(buf, ".\\chips", size);
    }
#else // POSIX-like systems
    ssize_t len = readlink("/proc/self/exe", buf, size - 1);
    if (len != -1) {
        buf[len] = '\0';
        char* delim = strrchr(buf, '/');
        if (delim) {
            *(delim + 1) = '\0';
            strncat(buf, "chips", size - strlen(buf) - 1);
        } else {
            strncpy(buf, "./chips", size);
        }
    } else {
        // Fallback to argv[0] if /proc/self/exe is not available
        strncpy(buf, argv0, size);
        buf[size - 1] = '\0';
        char* delim = strrchr(buf, '/');
        if (delim) {
            *(delim + 1) = '\0';
            strncat(buf, "chips", size - strlen(buf) - 1);
        } else {
            strncpy(buf, "./chips", size);
        }
    }
#endif
    return buf;
}
