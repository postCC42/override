#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

/*
int clear_stdin()
{
	int result;

	do
		result = getchar();
	while ( (char)result != '\n' && (char)result != -1 );
	return result;
}

int get_unum()
{
	int i = 0;

	fflush(stdout);
	scanf("%u", &i);
	clear_stdin();
	return i;
}
*/

int decrypt(char a1)
{
	unsigned int i;
	unsigned int len;
	char str[] = "Q}|u`sfg~sf{}|a3";
	unsigned int canary;
	int result;

	canary = __readgsdword(0x14u);

	len = strlen(str);
	for ( i = 0; i < len; ++i )
	{
		str[i] ^= a1;
	}
	if ( strncmp("Congratulations!", str, 17) == 0 )
		result = system("/bin/sh");
	else
		result = puts("\nInvalid Password");
	
	if (canary != __readgsdword(0x14u))
		__stack_chk_fail();
	
	return (result);
}

int test(int a1, int a2)
{
	int i; // [esp+1Ch] [ebp-Ch]

	i = a2 - a1;
	switch ( i )
	{
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:
		case 9:
		case 16:
		case 17:
		case 18:
		case 19:
		case 20:
		case 21:
		return decrypt(i);
		break;
		case 22:
		default:
		return decrypt(rand());
		break;
	}
}

int main(int argc, const char **argv, const char **envp)
{
	int i;

	srand(time(0));
	puts("***********************************");
	puts("*\t\tlevel03\t\t**");
	puts("***********************************");
	printf("Password:");
	scanf("%d", &i);
	test(i, 322424845);
	return 0;
}