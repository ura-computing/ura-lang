/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        fs.h                            */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 02:39:19    */
/*                                                             */
/*   updated: 2026/11/26 03:35:03                              */
/*                                                             */
/* *********************************************************** */

#pragma once

char *getcwd2(void) {
    char *path = getcwd(NULL, 0);
    if (path == NULL) {
        eprint("%s:%d getcwd failed\n", FILE, LINE);
        exit(1);
    }
    char *res = strdup2(path);
    free(path);
    return res;
}

char *home2(void) {
    char *home = getenv("HOME");
    if (home == NULL)
        home = getenv("USERPROFILE");
    return home ? home : ".";
}

char *dirname2(char *path) {
    return dirname(strdup2(path));
}

char *realpath2(char *path) {
    char *real = realpath(path, NULL);
    if (real == NULL)
        return path;
    char *res = strdup2(real);
    free(real);
    return res;
}

char *relative_path(char *path) {
    char *cwd = getcwd2();
    size_t cwd_len = strlen(cwd);
    if (strncmp2(path, cwd, cwd_len) && path[cwd_len] == '/')
        return path + cwd_len + 1;
    return path;
}

void write_file(char *path, char *text) {
    ioFile fp = fopen(path, "w");
    if (fp == NULL) {
        eprint("%s:%d can't write %s\n", FILE, LINE, path);
        exit(1);
    }
    fputs(text, fp);
    fclose(fp);
}

void mkdir2(char *path) {
    if (mkdir(path, 0755) != 0 && errno != EEXIST) {
        eprint("%s:%d mkdir failed\n", FILE, LINE);
        exit(1);
    }
}

void symlink2(char *target, char *path) {
#if WINDOWS_MACHINE
    DWORD flags =
        SYMBOLIC_LINK_FLAG_DIRECTORY | SYMBOLIC_LINK_FLAG_ALLOW_UNPRIVILEGED_CREATE;
    RemoveDirectoryA(path);
    bool is_linked = CreateSymbolicLinkA(path, target, flags) != 0;
#else
    unlink(path);
    bool is_linked = symlink(target, path) == 0;
#endif
    if (!is_linked) {
        eprint("%s:%d can't link %s to %s\n", FILE, LINE, path, target);
        exit(1);
    }
}

void remove2(char *path) {
    char *rm[] = { "rm", "-rf", path, NULL };
    run_command(rm, NULL);
}

char **readdir2(char *path) {
    DIR *dir = opendir(path);
    expand(char *, names);
    zero_init(names);

    rewinddir(dir);
    do {
        struct dirent *entry = readdir(dir);
        if (entry == NULL)
            break;
        if (entry->d_name[0] != '.') {
            append(names, strdup2(entry->d_name));
        }
    } while (1);
    closedir(dir);
    return names;
}
