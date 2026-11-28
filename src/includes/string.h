/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        string.h                        */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 02:37:56    */
/*                                                             */
/*   updated: 2026/11/26 03:54:22                              */
/*                                                             */
/* *********************************************************** */

#pragma once

char *strdup2(const char *str) {
    char *copy = arena_alloc(strlen(str) + 1, sizeof(char));
    strcpy(copy, str);
    return copy;
}

char *strndup2(const char *str, size_t count) {
    char *copy = arena_alloc(count + 1, sizeof(char));
    strncpy(copy, str, count);
    return copy;
}

bool strcmp2(const char *left, const char *right) {
    return strcmp(left, right) == 0;
}

bool strncmp2(const char *left, const char *right, size_t count) {
    return strncmp(left, right, count) == 0;
}

bool ends_with(const char *str, const char *end) {
    size_t str_size = strlen(str);
    size_t end_size = strlen(end);
    bool fits = str_size >= end_size;
    return fits && strcmp(str + str_size - end_size, end) == 0;
}

bool strchr2(char *str, int to_find) {
    char *found = strchr(str, to_find);
    return to_find != '\0' && found != NULL && found[0] != '\0';
}

char *parse_escaped(char **escaped, ucgFile *file, int s, int e) {
    char *text = file->buff;
    char *out = arena_alloc(e - s + 1, sizeof(char));
    int count = 0;

    while (s < e) {
        if (text[s] == '"' && s + 1 < e) {
            s = strchr(text + s + 1, '"') - text + 1;
            continue;
        }
        if (text[s] != '\\' || s + 1 == e) {
            out[count++] = text[s++];
            continue;
        }
        char *escape = escaped[(unsigned char)text[s + 1]];
        if (escape == NULL) {
            ucg_error(
                file,
                s,
                s + 2,
                format("unknown escape '\\%c'", text[s + 1]),
                NULL
            );
        } else {
            out[count++] = escape[0];
        }
        s += 2;
    }
    return out;
}
