// SPDX-License-Identifier: MPL-2.0

#include <configury/ini/internals/utility/string.h>
#include <configury/ini/internals/utility/ascii.h>

bool ini_check_string_equality_ignoring_case(
    const char *string_1,
    const char *string_2,
    uint32_t maximum_length
) {
    // If either one of the two pointers is NULL, return false. In the
    // case that both are NULL, return true, because they are equal.
    if ( ! string_1)
    {
        if ( ! string_2)
        {
            return true;
        }
        return false;
    }
    if ( ! string_2)
    {
        return false;
    }

    // Go through the whole string and compare both.

    uint32_t offset = 0;
    while (offset < maximum_length)
    {
        char character_1 = string_1[offset];
        if ((character_1 >= 'a') && (character_1 <= 'z'))
        {
            character_1 -= 0x20;
        }

        char character_2 = string_2[offset];
        if ((character_2 >= 'a') && (character_2 <= 'z'))
        {
            character_2 -= 0x20;
        }

        if (character_1 != character_2)
        {
            return false;
        }

        // If the current offset of this string (and thus of the other
        // as well) contains a NUL, terminating the string.
        if ( ! string_1[offset])
        {
            break;
        }
        ++offset;
    }
    return true;
}

bool ini_check_string_equality(
    const char *string_1,
    const char *string_2,
    uint32_t maximum_length
) {
    // If either one of the two pointers is NULL, return false. In the
    // case that both are NULL, return true, because they are equal.
    if ( ! string_1)
    {
        if ( ! string_2)
        {
            return true;
        }
        return false;
    }
    if ( ! string_2)
    {
        return false;
    }

    // Go through the whole string and compare both.

    uint32_t offset = 0;
    while (offset < maximum_length)
    {
        if (string_1[offset] != string_2[offset])
        {
            return false;
        }

        // If the current offset of this string (and thus of the other
        // as well) contains a NUL, terminating the string.
        if ( ! string_1[offset])
        {
            break;
        }
        ++offset;
    }
    return true;
}
