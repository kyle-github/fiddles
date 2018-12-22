
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "bitvec.h"


struct bitvec {
    int size;
    uint64_t data[];
};

bitvec_p bitvec_create(int size)
{
    int actual_entries = (size + 63)/64;
    bitvec_p result = NULL;

    if((result = calloc(1, sizeof(*result) + (actual_entries * sizeof(uint64_t)))) == NULL) {
        return NULL;
    }

    result->size = size;

    return result;
}



void bitvec_destroy(bitvec_p bv)
{
    if(bv) {
        free(bv);
    }
}


int bitvec_get(bitvec_p bv, int index)
{
    uint64_t mask = 0;

    if(!bv) {
        return -1;
    }

    if(index <0 || index >= bv->size) {
        return -1;
    }

    mask = ((uint64_t)1 << (index % 64));

    return ((mask & bv->data[index/64]) ? 1 : 0);
}



int bitvec_set(bitvec_p bv, int index, int val)
{
    int result = 0;
    uint64_t mask = 0;

    result = bitvec_get(bv, index);

    if(result < 0) {
        return result;
    }

    mask = ((uint64_t)1 << (index % 64));

    if(val == 0) {
        bv->data[index/64] &= (~mask);
    } else {
        bv->data[index/64] |= mask;
    }

    return result;
}



void bitvec_clear(bitvec_p bv, int val)
{
    int elem_count = 0;
    uint64_t clear_val = (val ? 0xFFFFFFFFFFFFFFFF : 0x0000000000000000);

    if(!bv) {
        return;
    }

    elem_count = bv->size / 64;

    for(int i=0; i < elem_count; i++) {
        bv->data[i] = clear_val;
    }
}



int bitvec_find_first(bitvec_p bv, int val)
{
    int elem_count = 0;

    if(!bv) {
        return -1;
    }

    elem_count = bv->size / 64;

    for(int i=0; i < elem_count; i++) {
        uint64_t elem = (val ? bv->data[i] : ~(bv->data[i]));
        int bit_index = ffsll(elem);

        if(bit_index != 0) {
            return (i * 64) + (bit_index - 1); /* bit_index is 1-64, not 0-63.  0 means no bits set. */
        }
    }

    return -1;
}

