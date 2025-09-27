// SPDX-License-Identifier: MPL-2.0

#ifndef RELICONF_INI_STRING_UTILITY_H
#define RELICONF_INI_STRING_UTILITY_H

#include <stdbool.h>
#include <stdint.h>

bool ini_check_string_equality_ignoring_case(
    const char *string_1,
    const char *string_2,
    uint32_t maximum_length
);

bool ini_check_string_equality(
    const char *string_1,
    const char *string_2,
    uint32_t maximum_length
);

#endif // RELICONF_INI_STRING_UTILITY_H
