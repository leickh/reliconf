// SPDX-License-Identifier: MPL-2.0

#ifndef RELICONF_INI_ASCII_UTILITY_H
#define RELICONF_INI_ASCII_UTILITY_H

#include <stdbool.h>
#include <stdint.h>

bool ini_is_lowercase(uint32_t character);
bool ini_is_uppercase(uint32_t character);
bool ini_is_letter(uint32_t character);
bool ini_is_hexadecimal_digit(uint32_t character);
bool ini_is_digit(uint32_t character);
bool ini_is_sign(uint32_t character);
bool ini_is_blank(uint32_t character);

#endif // RELICONF_INI_ASCII_UTILITY_H
