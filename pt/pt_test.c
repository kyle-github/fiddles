#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pt.h"





PT_FUNC(test,char*,name,int,start_val,int,end_val)
PT_CONTEXT(int,i)
PT_BODY
    for(ctx->i = args->start_val; ctx->i <= args->end_val; ctx->i++) {
        printf("%s count %d\n",args->name,ctx->i);
        PT_YIELD;
    }
PT_END


int main(int argc, char **argv)
{
    test("Test 1",3,7);
    test("Test 2", 10,20);
    test("Test 3", 1,5);

    pt_run();

    return 0;
}
