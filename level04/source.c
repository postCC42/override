#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ptrace.h>
#include <linux/prctl.h>
#include <signal.h>
#include <stdio.h>
#include <string.h>
/*
 * Some other functions in dissasembly but never called
*/

int main(void) {
    int stat_loc = 0;
    char s[128];
    int v7;
    int ptrace_data = 0;
    pid_t pid;
    
    memset(s, 0, sizeof(s));

    pid = fork();
    if (pid != 0) {
        // Parent program
        while (ptrace_data != 11) {
            wait(&stat_loc);        // Wait for the child process
            if (WIFEXITED(stat_loc) || WIFSIGNALED(stat_loc) {
                puts("child is exiting...");
                return 0;
            }
            // Read the value in reg 44 from the child process
            ptrace_data = ptrace(PTRACE_PEEKUSER, pid, 44, 0);
        }
        puts("no exec() for you");
        kill(pid, 9);
    } else {
        // Child program
        prctl(PR_SET_PDEATHSIG, SIG_IGN);    // ignore the signal sent by the kernel when its parent process terminates
        ptrace(PTRACE_TRACEME, 0, 0, 0);    // Enable tracing of the process
        puts("Give me some shellcode, k");
        gets(s);
    }

    return 0;
}