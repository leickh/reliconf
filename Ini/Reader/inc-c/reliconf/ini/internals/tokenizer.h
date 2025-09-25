// SPDX-License-Identifier: MPL-2.0

#ifndef RELICONF_INI_TOKENIZER_H
#define RELICONF_INI_TOKENIZER_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

typedef struct IniTokenList IniTokenList;
typedef struct IniToken IniToken;

typedef enum
{
    /// @brief Placeholder value for a nonexistent token or a value
    ///        that is not a token. This doesn't occur in any valid
    ///        INI token list.
    INI_TOKEN_NONE = 0,

    INI_TOKEN_INTEGER,
    INI_TOKEN_REAL,
    INI_TOKEN_IDENTIFIER,
    INI_TOKEN_BOOLEAN,
    INI_TOKEN_INLINE_STRING,
    INI_TOKEN_MULTI_LINE_STRING,

    INI_TOKEN_SIGN,

    // @todo: Multi-line comments
    INI_TOKEN_SINGLE_LINE_COMMENT,

    /// @brief Occurrs in a token list if a piece of text could not
    ///        be identified as being any token.
    INI_TOKEN_INVALID = 254,
    INI_TOKEN_STREAM_END = 255,

} IniTokenType;

struct IniToken
{
    uint32_t offset;
    uint32_t length;

    IniTokenType type:16;
    uint16_t column;
    uint32_t line;

    union IniTokenSpecifics
    {
        struct IniIntegerTokenSpecifics
        {
            uint8_t number_base;
        } integer;

        struct IniIntegerBooleanSpecifics
        {
            bool value;
        } boolean;

    } specifics;
};

struct IniTokenList
{
    uint32_t tokens_capacity;
    uint32_t num_tokens;
    IniToken *tokens;
};

IniTokenList ini_tokenize(
    const char *source
);

void ini_write_tokens(
    FILE *file,
    IniTokenList token_list,
    const char *source
);

#endif // RELICONF_INI_TOKENIZER_H
