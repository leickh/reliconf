// SPDX-License-Identifier: MPL-2.0

#include <configury/ini/internals/tokenizer.h>
#include <configury/ini/internals/utility/ascii.h>
#include <configury/ini/internals/utility/string.h>

#include <stdlib.h>
#include <string.h>

bool ini_is_identifier_letter(uint32_t character)
{
    if (ini_is_letter(character))
    {
        return true;
    }
    if (ini_is_digit(character))
    {
        return true;
    }
    if (character == '-' || character == '_')
    {
        return true;
    }
    return false;
}

IniTokenList ini_tokenize(
    const char *source
) {
    IniTokenList token_list;
    token_list.num_tokens = 0;
    token_list.tokens_capacity = 1024;
    token_list.tokens = malloc(token_list.tokens_capacity * sizeof(IniToken));

    uint32_t offset = 0;
    while (source[offset])
    {
        if (ini_is_blank(source[offset]))
        {
            ++offset;
            continue;
        }

        if (token_list.num_tokens >= token_list.tokens_capacity)
        {
            token_list.tokens_capacity *= 2;
            token_list.tokens = realloc(
                token_list.tokens,
                token_list.tokens_capacity * sizeof(IniToken)
            );
        }

        if (source[offset] == '#')
        {
            uint32_t start_offset = offset;
            while (source[offset])
            {
                if (source[offset] == '\n')
                {
                    break;
                }
                ++offset;
            }
            IniToken comment_token;
            comment_token.type = INI_TOKEN_SINGLE_LINE_COMMENT;
            comment_token.offset = start_offset;
            comment_token.length = offset - start_offset;

            token_list.tokens[token_list.num_tokens] = comment_token;
            ++token_list.num_tokens;
        }

        if (ini_is_digit(source[offset]))
        {
            uint32_t start_offset = offset;

            uint8_t number_base = 10;
            if (source[offset] == '0')
            {
                if ( ! ini_is_digit(source[offset + 1]))
                {
                    switch (source[offset + 1])
                    {
                        case 'b':
                            number_base = 2;
                            break;

                        case 'o':
                            number_base = 8;
                            break;

                        case 'd': break;

                        case 'x':
                            number_base = 16;
                            break;

                        default:
                            // @todo: Log error for invalid number base declarator
                            break;
                    }
                }
            }
            if (number_base == 16)
            {
                while (ini_is_hexadecimal_digit(source[offset]))
                {
                    ++offset;
                }
                IniToken integer_token;
                integer_token.type = INI_TOKEN_INTEGER;
                integer_token.offset = start_offset;
                integer_token.length = offset - start_offset;
                integer_token.specifics.integer.number_base = 16;

                token_list.tokens[token_list.num_tokens] = integer_token;
                ++token_list.num_tokens;
                continue;
            }

            if (number_base < 10) {
                while (source[offset])
                {
                    // If the character at the current position is a digit but
                    // a larger one than the number base allows.
                    if (
                        (source[offset] >= ('0' + number_base))
                        && (source[offset] <= '9')
                    ) {
                        // @todo: Log error because of invalid digit for number base
                    }

                    if ( ! ini_is_digit(source[offset]))
                    {
                        break;
                    }
                    ++offset;
                }
                if (source[offset] != '.')
                {
                    IniToken integer_token;
                    integer_token.type = INI_TOKEN_INTEGER;
                    integer_token.offset = start_offset;
                    integer_token.length = offset - start_offset;
                    integer_token.specifics.integer.number_base = number_base;

                    token_list.tokens[token_list.num_tokens] = integer_token;
                    ++token_list.num_tokens;

                    continue;
                }
                // Only decimal reals are supported
                if (number_base != 10)
                {
                    continue;
                }
                // Parse float's decimal portion

                while (ini_is_digit(source[offset]))
                {
                    ++offset;
                }
                IniToken real_token;
                real_token.type = INI_TOKEN_REAL;
                real_token.offset = start_offset;
                real_token.length = offset - start_offset;

                token_list.tokens[token_list.num_tokens] = real_token;
                ++token_list.num_tokens;

                continue;
            }
            // @todo: Log an internal error if it gets here
        }

        if (ini_is_identifier_letter(source[offset]))
        {
            uint32_t start_offset = offset;
            while (ini_is_identifier_letter(source[offset]))
            {
                ++offset;
            }

            IniToken identifier_token;
            identifier_token.type = INI_TOKEN_IDENTIFIER;
            identifier_token.offset = start_offset;
            identifier_token.length = offset - start_offset;

            if (ini_check_string_equality_ignoring_case(
                &source[start_offset],
                "TRUE",
                4
            )) {
                identifier_token.type = INI_TOKEN_BOOLEAN;
                identifier_token.specifics.boolean.value = true;
            }

            if (ini_check_string_equality_ignoring_case(
                &source[start_offset],
                "FALSE",
                5
            )) {
                identifier_token.type = INI_TOKEN_BOOLEAN;
                identifier_token.specifics.boolean.value = false;
            }

            token_list.tokens[token_list.num_tokens] = identifier_token;
            ++token_list.num_tokens;
            continue;
        }

        if ((source[offset] == '"') || (source[offset] == '\''))
        {
            char quotation_mark = source[offset];
            uint32_t initiating_quotation_mark_offset = offset;
            while (source[offset] == quotation_mark)
            {
                ++offset;
            }
            uint32_t num_quotation_marks = offset - initiating_quotation_mark_offset;

            if (num_quotation_marks == 2)
            {
                IniToken string_token;
                string_token.type = INI_TOKEN_INLINE_STRING;
                string_token.offset = initiating_quotation_mark_offset + 1;
                string_token.length = 0;

                token_list.tokens[token_list.num_tokens] = string_token;
                ++token_list.num_tokens;
                continue;
            }

            if (num_quotation_marks >= 3)
            {
                // @todo: Parse multi-line strings
                continue;
            }

            while (source[offset])
            {
                if (source[offset] == quotation_mark)
                {
                    break;
                }
                if (source[offset] == '\\')
                {
                    ++offset;
                }
                ++offset;
            }

            IniToken string_token;
            string_token.type = INI_TOKEN_INLINE_STRING;
            string_token.offset = initiating_quotation_mark_offset + 1;
            string_token.length = offset - string_token.offset;

            token_list.tokens[token_list.num_tokens] = string_token;
            ++token_list.num_tokens;

            ++offset; // Skip the terminating quotation mark
            continue;
        }

        if (ini_is_sign(source[offset]))
        {
            IniToken sign_token;
            sign_token.type = INI_TOKEN_SIGN;
            sign_token.offset = offset;
            sign_token.length = 1;

            token_list.tokens[token_list.num_tokens] = sign_token;
            ++token_list.num_tokens;

            ++offset;
            continue;
        }

        ++offset;
    }
    return token_list;
}

void ini_free_token_list(
    const IniTokenList *token_list
) {
    free(token_list->tokens);
}

char * ini_stringify_token_type(
    IniTokenType type
) {
    switch (type)
    {
        case INI_TOKEN_NONE:
            return "NONE";

        case INI_TOKEN_INTEGER:
            return "INTEGER";

        case INI_TOKEN_REAL:
            return "REAL";

        case INI_TOKEN_IDENTIFIER:
            return "IDENTIFIER";

        case INI_TOKEN_BOOLEAN:
            return "BOOLEAN";

        case INI_TOKEN_INLINE_STRING:
            return "INLINE-STRING";

        case INI_TOKEN_MULTI_LINE_STRING:
            return "MULTI-LINE-STRING";

        case INI_TOKEN_SIGN:
            return "SIGN";

        case INI_TOKEN_SINGLE_LINE_COMMENT:
            return "SINGLE-LINE-COMMENT";

        case INI_TOKEN_INVALID:
            return "INVALID";

        case INI_TOKEN_STREAM_END:
            return "STREAM-END";

        default:
            break;
    }
    return "~INVALID_ENUM~";
}

uint32_t ini_min(uint32_t left, uint32_t right)
{
    if (left < right)
    {
        return left;
    }
    return right;
}

#define INI_TOKEN_STRING_DISPLAY_LENGTH 128

void ini_write_tokens(
    FILE *file,
    IniTokenList token_list,
    const char *source
) {
    char string_buffer[INI_TOKEN_STRING_DISPLAY_LENGTH];

    uint32_t token_index = 0;
    while (token_index < token_list.num_tokens)
    {
        IniToken token = token_list.tokens[token_index];

        uint32_t copied_length = ini_min(
            INI_TOKEN_STRING_DISPLAY_LENGTH - 1,
            token.length
        );

        memcpy(
            string_buffer,
            &source[token.offset],
            copied_length
        );
        string_buffer[copied_length] = 0;

        fprintf(
            file,
            "%03u: [ T=%s, '%s':%u ]\n",
            token_index,
            ini_stringify_token_type(token.type),
            string_buffer,
            token.length
        );
        ++token_index;
    }
}
