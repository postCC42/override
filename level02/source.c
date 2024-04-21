#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    char password[96]; // [rsp+10h] [rbp-110h] BYREF
    char file_buffer[48]; // [rsp+80h] [rbp-A0h] BYREF
    char username[96]; // [rsp+B0h] [rbp-70h] BYREF
    int read_result; // [rsp+114h] [rbp-Ch]
    FILE *stream; // [rsp+118h] [rbp-8h]

    memset(s, 0, sizeof(s));
    memset(file_buffer, 0, 41);
    memset(password, 0, sizeof(password));
    stream = 0LL;
    read_result = 0;

    stream = fopen("/home/users/level03/.pass", "r");
    if ( !stream ) {
        fwrite("ERROR: failed to open password file\n", 1, 0x24, stderr);
        exit(1);
    }
    read_result = fread(file_buffer, 1, 0x29, stream);
    file_buffer[strcspn(file_buffer, "\n")] = 0;

    if ( read_result != 41 ) {
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        fwrite("ERROR: failed to read password file\n", 1, 0x24, stderr);
        exit(1);
    }
    fclose(stream);

    puts("===== [ Secure Access System v1.0 ] =====");
    puts("/***************************************\\");
    puts("| You must login to access this system. |");
    puts("\\**************************************/");

    printf("--[ Username: ");
    fgets(username, 100, stdin);
    username[strcspn(username, "\n")] = 0;

    printf("--[ Password: ");
    fgets(password, 100, stdin);
    password[strcspn(password, "\n")] = 0;
    
    puts("*****************************************");

    if ( strncmp(file_buffer, password, 0x29) ) {
        printf(s);
        puts(" does not have access!");
        exit(1);
    }
    printf("Greetings, %s!\n", s);
    system("/bin/sh");
    return 0;
}