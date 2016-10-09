
#include <stdio.h>
#include <stdlib.h>
#include "earlyinit.h"


static int test_val = 4;
static void *test_allocation = NULL;

R_INITIALIZER(test_initializer)
{
    printf("Test initializer called.\n");

    test_val = 42;

    test_allocation = malloc(1024);
}

R_DEINITIALIZER(test_deinitializer)
{
    printf("Test deinitializer called.\n");

    test_val = 4;

    free(test_allocation);
}




int main(int argc, char **argv)
{
    printf("test_val = %d\n",test_val);

    printf("test_allocation=%p\n",test_allocation);

    return 0;
}


