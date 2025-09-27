// SPDX-License-Identifier: MPL-2.0

#include <reliconf/ini/internals/reader.h>
#include <reliconf/ini/internals/tokenizer.h>

#include <stdlib.h>
#include <string.h>

typedef enum
{
    INI_TOP_LEVEL_UNKNOWN = 0,

    INI_TOP_LEVEL_VALUE_ASSIGNMENT,
    INI_TOP_LEVEL_SECTION_HEADER,

    INI_TOP_LEVEL_INVALID = 255

} IniTopLevelStatementType;

bool ini_check_token(
    const char *source,
    const IniTokenList *token_list,
    uint32_t cursor,
    IniTokenType reference_type,
    const char *reference_value
) {
    IniToken token = token_list->tokens[cursor];

    if (reference_type == INI_TOKEN_NONE)
    {
        if (token.type != reference_type)
        {
            return false;
        }
    }

    if (reference_value != NULL)
    {
        uint32_t len_reference_value = strlen(reference_value);
        if (token.length != len_reference_value)
        {
            return false;
        }
        if ( ! memcmp(&source[token.offset], reference_value, len_reference_value))
        {
            return false;
        }
    }
    return true;
}

IniDocument ini_parse_tokens(
    const char *source,
    const IniTokenList *token_list
) {
    uint32_t num_tokens = token_list->num_tokens;

    uint32_t cursor = 0;
    while (cursor < num_tokens)
    {
        // @todo: Parse INI

        ++cursor;
    }
}

IniDocument ini_read_source(const char *source, size_t len_source)
{
    IniTokenList token_list = ini_tokenize(source);
    ini_parse_tokens(source, &token_list);
}

IniDocument ini_read_from_file(FILE *file, size_t offset, size_t len_data)
{
    size_t start_position = ftell(file);
    fseek(file, offset, SEEK_SET);
    char *source = malloc(len_data);
    fread(source, 1, len_data, file);

    IniDocument document = ini_read_source(source, len_data);

    free(source);
    fseek(file, start_position, SEEK_SET);
    return document;
}

IniDocument ini_read_file_at_path(const char *path, size_t offset, size_t len_data)
{
    FILE *file = fopen(path, "r");
    if ( ! file)
    {
        IniDocument null_document;

        null_document.source = NULL;

        null_document.num_fields = 0;
        null_document.fields_capacity = 0;
        null_document.fields = NULL;

        null_document.num_sections = 0;
        null_document.sections_capacity = 0;
        null_document.sections = NULL;

        return null_document;
    }
    IniDocument document = ini_read_from_file(file, offset, len_data);
    fclose(file);
    return document;
}
