// Source - https://github.com/mmxsrup/simple-strace

#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "strace.h"
#include "syscall_table.h"

#include <sys/syscall.h>   /* For SYS_write etc */
#include <sys/reg.h>

const int long_size = sizeof(long);

void getdata(pid_t child, long addr, char *str, int len) {
    char *laddr;
    int i, j;

    union u {
        long val;
        char chars[long_size];
    } data;

    i = 0;
    j = len / long_size;
    laddr = str;

    while (i < j) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, long_size);
        printf("- %ld - %s \n", data.val, data.chars);
        ++i;
        laddr += long_size;
    }

    j = len % long_size;
    if (j != 0) {
        data.val = ptrace(PTRACE_PEEKDATA, child, addr + i * long_size, NULL);
        memcpy(laddr, data.chars, j);
    }
    str[len] = '\0';
}


int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: ./strace PROG [ARGS]\n");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("Failed frok");
        exit(1);
    } else if (pid == 0) { // child
        tracee(argv + 1);
    } else {
        tracer(pid);
    }

    return 0;
}

int tracee(char **cmd) {
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
        perror("Failed ptrace(PTRACE_TRACEME");
        exit(1);
    }
    return execvp(cmd[0], cmd);
}

int tracer(pid_t tracee_pid) {
    int status;
    waitpid(tracee_pid, &status, 0); // wait for execvp

    while (1) {
        if (ptrace(PTRACE_SYSCALL, tracee_pid, NULL, NULL) < 0) {
            perror("Failed ptrace(PTRACE_SYSCALL");
            exit(1);
        }

        int status;
        waitpid(tracee_pid, &status, 0); // Wait for syscall enter or exit

        if (WIFEXITED(status)) // Child terminated normally
            break;

        struct user_regs_struct regs;
        if (ptrace(PTRACE_GETREGS, tracee_pid, NULL, &regs) < 0) {
            perror("Faild ptrace(PTRACE_GETREGS)");
            exit(1);
        }

        // Check enter-stop or exit-stop
        if (regs.rax != -ENOSYS) { // Is enter-stop
            continue;
        }


// Ref code for ex 3
//        char *str;
//        str = (char *) malloc((params[2] + 1) * sizeof(char));
//        getdata(tracee_pid, addr, str, len);
//        printf("Write data = %s\n", str);

        int syscall_num = regs.orig_rax;
// Ref code for ex 2
//        printf("[%d] %s = %ld\n", syscall_num, syscall_table[syscall_num], result);
        printf("[%d] %s\n", syscall_num, syscall_table[syscall_num]);
    }
    return 0;
}