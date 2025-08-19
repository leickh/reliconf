// SPDX-License-Identifier: MPL-2.0

#ifndef CONFIGURY_INI_H
#define CONFIGURY_INI_H

#include <stdint.h>

typedef struct IniDocument IniDocument;

typedef enum
{
    INI_VALUE_INTEGER,
    INI_VALUE_REAL,
    INI_VALUE_STRING,
    INI_VALUE_ARRAY,
    INI_VALUE_LIST,

} IniValueType;

struct IniValue
{
    IniValueType type;
    union IniValueSpecifics
    {
        struct IniInteger
        {
            uint8_t number_base;
            uint32_t num_digits;
            char *text;
        };

        struct IniHexadecimalInteger
        {
            uint32_t num_digits;
            char *text;
        };

        struct IniString
        {
            uint32_t len_text;
            char *text;
        };

        struct IniArray
        {
            /// @brief Whether this array is represented as a list; with
            ///        each entry in a new line and preceded by a minus.
            bool is_list;
            uint32_t num_items;
            IniValue *values;

            uint32_t num_indent_spaces;
        };
    };
};

struct IniField
{
    uint32_t line_offset;

    char *key;
    IniValue *value;
};

struct IniSection
{
    uint32_t num_occurrences;
    uint32_t *line_offsets;

    uint32_t fields_capacity;
    uint32_t num_fields;
    IniField **fields;
};

struct IniDocument
{
    /// @brief The only instance of the INI source that the library is
    ///        supposed to have for this document.
    ///
    /// @note This source should be referenced but never copied.
    char *source;

    uint32_t sections_capacity;
    uint32_t num_sections;
    IniSection *sections;

    uint32_t fields_capacity;
    uint32_t num_fields;
    IniField *fields;
};

#endif // CONFIGURY_INI_H
