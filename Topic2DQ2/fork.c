/* Fork program */
#include <stdio.h>
#include <unistd.h>
int main()
{
    if (fork() == 0) {
	printf("Child Created\n");
    }

    if (fork() == 0) {
        printf("Child Created\n");
    }
}

