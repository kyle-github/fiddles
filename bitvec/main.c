#include <assert.h>
#include <stdio.h>
#include "bitvec.h"

#define BITVEC_SIZE (101)

int main(int argc, const char **argv)
{
    bitvec_p bv = bitvec_create(BITVEC_SIZE);
    int old_val = 0;
    int index = 0;

    assert(bv != NULL);

    /* test == basic bit setting */
    old_val = bitvec_set(bv, 40, 1);
    assert(old_val == 0);

    /* get bit finding */
    index = bitvec_find_first(bv, 1);
    assert(index == 40);

    old_val = bitvec_set(bv, 0, 1);
    assert(old_val == 0);

    index = bitvec_find_first(bv, 1);
    assert(index == 0);

    index = bitvec_find_first(bv, 0);
    assert(index == 1);


    bitvec_destroy(bv);

    return 0;
}
