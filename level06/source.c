#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/ptrace.h>

bool auth(char *s, int serial) {
    int i;
    int hash;
    int s_len;

    s[strcspn(s, "\n")] = 0;
    s_len = strnlen(s, 32);
    if (s_len <= 5)
        return 1;

    if (ptrace(PTRACE_TRACEME, 0, 1, 0) == -1) {
        puts("\x1B[32m.---------------------------.");
        puts("\x1B[31m| !! TAMPERING DETECTED !!  |");
        puts("\x1B[32m'---------------------------'");
        return 1;
    }
    
    hash = (s[3] ^ 0x1337) + 6221293;
    for ( i = 0; i < s_len; ++i ) {
        if (s[i] <= 31)
            return 1;
        hash += (hash ^ (unsigned int)s[i]) % 0x539;
    }

    return serial != hash;
}

int main(void) {
    int serial;
    char login[32];

    puts("***********************************");
    puts("*\t\tlevel06\t\t  *");
    puts("***********************************");

    printf("-> Enter Login: ");
    fgets(login, 32, stdin);

    puts("***********************************");
    puts("***** NEW ACCOUNT DETECTED ********");
    puts("***********************************");

    printf("-> Enter Serial: ");
    scanf("%u", &serial);

    if ( auth(login, serial) )
         return 1;
    puts("Authenticated!");
    system("/bin/sh");
    return 0;
}