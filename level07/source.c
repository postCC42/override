#include <string.h>
#include <stdio.h>

int clear_stdin() {
    int result;

    do
      result = getchar();
    while ( (_BYTE)result != 10 && (_BYTE)result != 0xFF );
    return result;
}

int get_unum() {
    int v;

    v = 0;
    fflush(stdout);
    scanf("%u", v);
    clear_stdin();
    return v;
}

int store_number(int *data) {
    unsigned int number;
    unsigned int index;

    printf(" Number: ");
    number = get_unum();  
    printf(" Index: ");
    index = get_unum();
    if ( index % 3 == 0 || (number >> 24) == 183 ) {
      puts(" *** ERROR! ***");
      puts("   This index is reserved for wil!");
      puts(" *** ERROR! ***");
      return 1;
    }
    
    data[index] = number;
    return 0;
}

int read_number(int *data) {
    int index;

    printf(" Index: ");
    index = get_unum();
    printf(" Number at data[%u] is %u\n", index, data[index]);
    return 0;
}

int main(int argc, const char **argv, const char **envp) {
    int data[100];
    int number = 0;
    char cmd[20] = { 0 };

    while ( *argv ) {
        memset((void *)*argv, 0, strlen(*argv));
        ++argv;
    }
    while ( *envp ) {
        memset((void *)*envp, 0, strlen(*envp));
        ++envp;
    }

    puts(
        "----------------------------------------------------\n"
        "  Welcome to wil's crappy number storage service!   \n"
        "----------------------------------------------------\n"
        " Commands:                                          \n"
        "    store - store a number into the data storage    \n"
        "    read  - read a number from the data storage     \n"
        "    quit  - exit the program                        \n"
        "----------------------------------------------------\n"
        "   wil has reserved some storage :>                 \n"
        "----------------------------------------------------\n");

    while ( 1 ) {
        printf("Input command: ");
        number = 1;

        fgets(cmd, 20, stdin);
        cmd[strlen(cmd) - 1] = 0;

        if ( !memcmp(s, "store", 5u) )
            number = store_number(data);

        if ( !memcmp(s, "read", 4u) )
            number = read_number(data);

        if ( !memcmp(s, "quit", 4u) )
            return 0;

        if ( number )
            printf(" Failed to do %s command\n", s);
        else
            printf(" Completed %s command successfully\n", s);
    }
}