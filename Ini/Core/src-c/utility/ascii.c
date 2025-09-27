// SPDX-License-Identifier: MPL-2.0

#include <reliconf/ini/utility/ascii.h>

bool ini_is_lowercase(uint32_t character)
{
    if (character < 'a')
    {
        return false;
    }
    if (character > 'z')
    {
        return false;
    }
    return true;
}

bool ini_is_uppercase(uint32_t character)
{
    if (character < 'A')
    {
        return false;
    }
    if (character > 'Z')
    {
        return false;
    }
    return true;
}

bool ini_is_letter(uint32_t character)
{
    if (ini_is_lowercase(character))
    {
        return true;
    }
    if (ini_is_uppercase(character))
    {
        return true;
    }
    return false;
}

bool ini_is_digit(uint32_t character)
{
    if (character < '0')
    {
        return false;
    }
    if (character > '9')
    {
        return false;
    }
    return true;
}

bool ini_is_hexadecimal_digit(uint32_t character)
{
    if (ini_is_digit(character))
    {
        return true;
    }
    if ((character >= 'a') && (character <= 'z'))
    {
        return true;
    }
    if ((character >= 'A') && (character <= 'Z'))
    {
        return true;
    }
    return false;
}

bool ini_is_sign_of_block_1(uint32_t character)
{
    if (character < 0x21)
    {
        return false;
    }
    if (character > 0x2f)
    {
        return false;
    }
    return true;
}

bool ini_is_sign_of_block_2(uint32_t character)
{
    if (character < 0x3a)
    {
        return false;
    }
    if (character > 0x40)
    {
        return false;
    }
    return true;
}

bool ini_is_sign_of_block_3(uint32_t character)
{
    if (character < 0x5b)
    {
        return false;
    }
    if (character > 0x60)
    {
        return false;
    }
    return true;
}

bool ini_is_sign_of_block_4(uint32_t character)
{
    if (character < 0x7b)
    {
        return false;
    }
    if (character > 0x7e)
    {
        return false;
    }
    return true;
}

bool ini_is_sign(uint32_t character)
{
    if (ini_is_sign_of_block_1(character))
    {
        return true;
    }
    if (ini_is_sign_of_block_2(character))
    {
        return true;
    }
    if (ini_is_sign_of_block_3(character))
    {
        return true;
    }
    if (ini_is_sign_of_block_4(character))
    {
        return true;
    }
    return false;
}

bool ini_is_blank(uint32_t character)
{
    if (character == ' ')
    {
        return true;
    }
    if (character == '\t')
    {
        return true;
    }
    return false;
}
