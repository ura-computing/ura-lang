/* *********************************************************** */
/*                                                             */
/*    _   _ ____      _        ps.h                            */
/*   | | | |  _ \    / \                                       */
/*   | | | | |_) |  / _ \      Mohammed Hrima                  */
/*   | |_| |  _ <  / ___ \     mohammedxhrima@gmail.com        */
/*    \___/|_| \_\/_/   \_\    created: 2026/11/26 03:00:11    */
/*                                                             */
/*   updated: 2026/11/26 03:01:26                              */
/*                                                             */
/* *********************************************************** */

#pragma once

int run_command(char **argv, char *out_path) {
    pid_t pid = fork();
    if (pid == 0) {
        int quiet = open("/dev/null", O_WRONLY);
        int out = out_path ? open(out_path, O_WRONLY | O_CREAT | O_TRUNC, 0644) : quiet;
        dup2(out, 1);
        dup2(quiet, 2);
        execvp(argv[0], argv);
        _exit(127);
    }
    int status;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}
