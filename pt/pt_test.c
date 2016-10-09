#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "pt.h"


typedef int (*pt_func)(void *, void **);

struct pt_entry {
    struct pt_entry *next;
    void *args;
    void *ctx;
    pt_func func;
};


static struct pt_entry *pt_list;

int pt_schedule(pt_func func, void *args)
{
    struct pt_entry *entry = calloc(1,sizeof(struct pt_entry));
    if(!entry) {
        fprintf(stderr,"oops, cannot allocate new pt entry!");
        return -1;
    }

    entry->func = func;
    entry->args = args;

    entry->next = pt_list;
    pt_list = entry;

    return 1;
}

int pt_run(void)
{
    struct pt_entry *curr = pt_list;
    struct pt_entry *last = NULL;

    while(pt_list) {
        if(curr->func(curr->args,&curr->ctx) == PT_TERMINATE) {
            struct pt_entry *next = curr->next;

            if(curr->args) free(curr->args);
            if(curr->ctx) free(curr->ctx);
            free(curr);

            if(!last) {
                pt_list = next;
            } else {
                last->next = next;
            }

            curr = next;
        } else {
            last = curr;
            curr = curr->next;
        }

        if(!curr) {
            curr = pt_list;
            last = NULL;
        }
    }

    return 1;
}




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
