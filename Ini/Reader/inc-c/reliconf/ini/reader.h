// SPDX-License-Identifier: MPL-2.0

#ifndef RELICONF_INI_READER_H
#define RELICONF_INI_READER_H

#include <stdint.h>
#include <stdio.h>

#include <reliconf/ini/document.h>

IniDocument ini_read_source(const char *source, size_t len_source);
IniDocument ini_read_file(FILE *file, size_t offset, size_t len_data);
IniDocument ini_read_file_at_path(const char *path, size_t offset, size_t len_data);

#endif // RELICONF_INI_READER_H
