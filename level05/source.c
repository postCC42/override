#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
	char		str[100];

	fgets(str, 100, stdin);

	for (int i = 0; i < strlen(str); i++)
	{
		if (str[i] >= 'A' && str[i] <= 'Z')
		{
			str[i] ^= 32;
		}
	}

	printf(str);
	exit(0);
}