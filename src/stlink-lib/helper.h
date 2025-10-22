/*
 * File: helper.h
 *
 * General helper functions
 */

#ifndef HELPER_H
#define HELPER_H

#include <stddef.h>

uint32_t time_ms();
int32_t arg_parse_freq(const char *str);
const char* get_chips_path(char* argv0, char* buf, size_t size);

#endif // HELPER_H
