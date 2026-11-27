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
    char *res = arena_alloc(strlen(str) + 1, sizeof(char));
    strcpy(res, str);
    return res;
}

char *strndup2(const char *str, size_t n) {
    char *res = arena_alloc(n + 1, sizeof(char));
    strncpy(res, str, n);
    return res;
}

bool strcmp2(const char *left, const char *right) {
    return strcmp(left, right) == 0;
}

bool strncmp2(const char *left, const char *right, size_t n) {
    return strncmp(left, right, n) == 0;
}

bool ends_with(const char *str, const char *end) {
    size_t n = strlen(str);
    size_t m = strlen(end);
    return n >= m && strcmp(str + n - m, end) == 0;
}

bool strchr2(char *str, int c) {
    char *res = strchr(str, c);
    return c != '\0' && res != NULL && res[0] != '\0';
}

char *parse_escaped(char **escaped, ucgFile *file, int s, int e) {
    char *input = file->buff;
    char *res = arena_alloc(e - s + 1, sizeof(char));
    int r = 0;

    while (s < e) {
        if (input[s] == '"' && s + 1 < e) {
            s = strchr(input + s + 1, '"') - input + 1;
            continue;
        }
        if (input[s] != '\\' || s + 1 == e) {
            res[r++] = input[s++];
            continue;
        }
        char *str = escaped[(unsigned char)input[s + 1]];
        if (str == NULL) {
            ucg_error(
                file,
                s,
                s + 2,
                format("unknown escape '\\%c'", input[s + 1]),
                NULL
            );
        } else {
            res[r++] = str[0];
        }
        s += 2;
    }
    return res;
}
