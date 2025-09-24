// SPDX-License-Identifier: MPL-2.0

#ifndef CONFIGURY_INI_DOCUMENT_H
#define CONFIGURY_INI_DOCUMENT_H

#include <stdbool.h>
#include <stdint.h>

typedef struct IniDocument IniDocument;
typedef struct IniSection IniSection;
typedef struct IniField IniField;
typedef struct IniValue IniValue;

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
        } integer;

        struct IniString
        {
            uint32_t len_text;
            char *text;
        } string;

        struct IniArray
        {
            /// @brief
            ///     Whether this array is represented as a list; with
            ///     each entry in a new line and preceded by a minus.
            ///
            ///     If this is FALSE, the list is encapsulated by square
            ///     brackets, but may also span multiple lines.
            bool is_list;
            uint32_t num_items;
            IniValue *values;

            uint32_t num_indent_spaces;
        } array;
    } specifics;
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

    /// @brief
    ///     To be interpreted as an array of pointers into the array
    ///     `fields` of the `IniDocument` that also contains this section.
    IniField **fields;
};

struct IniDocument
{
    /// @brief
    ///     The only instance of the INI source that the library is
    ///     supposed to have for this document.
    ///
    /// @note
    ///     This source should only be referenced but never copied.
    char *source;

    /// @brief
    ///     Number of allocated sections in the `sections`-array.
    ///     This is often higher than `num_sections` to avoid
    ///     re-allocating unnecessarily often.
    uint32_t sections_capacity;

    /// @brief
    ///     Number of items of the array `sections` that are actually used.
    uint32_t num_sections;

    /// @brief
    ///     Parts of the document, or sections. They start with a
    ///     header consisting of the section's name which is encapsulated
    ///     in the front with an opening square bracket and in the back
    ///     with a closing square bracket.
    ///
    /// @note
    ///     These structures may cross-reference into `fields`.
    ///
    /// @todo
    ///     Make sure that there are no dangling pointers
    ///     from `sections` into `fields`.
    IniSection *sections;

    /// @brief
    ///     Number of allocated fields in the `fields`-array.
    ///     This is often higher than `num_fields` to avoid
    ///     re-allocating unnecessarily often.
    uint32_t fields_capacity;

    /// @brief
    ///     Number of items of the array `fields` that are actually used.
    uint32_t num_fields;

    /// @brief
    ///     Generic INI-field with a name and a value of a given type.
    IniField *fields;
};

#endif // CONFIGURY_INI_DOCUMENT_H
