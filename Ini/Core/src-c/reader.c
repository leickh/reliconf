// SPDX-License-Identifier: MPL-2.0

#include <reliconf/ini/reader.h>

IniDocument ini_read_source(const char *source, size_t len_source);
IniDocument ini_read_file(FILE *file, size_t offset, size_t len_data);
IniDocument ini_read_file_at_path(const char *path, size_t offset, size_t len_data);


