#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "vector.h"

VECTOR_DECL(int_vec, int);

VECTOR_IMPL(int_vec, int)


void int_vec_print(int_vec *vec)
{
    printf("vec(%ld,%ld)[",int_vec_size(vec), int_vec_capacity(vec));
    for(size_t elem = 0; elem < int_vec_size(vec); elem++) {
        if(elem != 0) printf(", ");
        printf("%d",int_vec_get(vec, elem));
    }
    printf("]\n");
}

int main()
{
    int_vec *vec = int_vec_create(4,10);
    size_t index = 0;
    int tmp = 0;
    for(int i=0; i < 6; i++) {
        index = int_vec_splice(vec, index, &i, (size_t)1);
        int_vec_print(vec);
    }
    index = 4;
    tmp = 15;
    index = int_vec_splice(vec, index, &tmp, (size_t)1);
    int_vec_print(vec);
    index = int_vec_cut(vec, (size_t)2, (size_t)3);
    int_vec_print(vec);

    int_vec_destroy(vec);

    return 0;
}

