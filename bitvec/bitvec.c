
#include <stdint.h>
#include <stdlib.h>
#include "bitvec.h"

#include <stdio.h>


struct bitvec {
    int size;
    uint64_t data[];
};

/* WARNING - this is not true when sizeof units are not 8-bit bytes! */
static const int ELEM_BITS = (int)(sizeof(uint64_t)*8);
static const uint64_t ALL_ONES = 0xFFFFFFFFFFFFFFFFL;



static int popcount64(uint64_t val);
static int find_first(uint64_t x);



bitvec_p bitvec_create(int size)
{
    int actual_entries = (size + (ELEM_BITS-1))/ELEM_BITS;
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

    mask = ((uint64_t)1 << (index % ELEM_BITS));

    return ((mask & bv->data[index/ELEM_BITS]) ? 1 : 0);
}



int bitvec_set_range(bitvec_p bv, int start, int size)
{
    int first_elem;
    int last_elem;
    uint64_t low_mask = 0;
    uint64_t high_mask = 0;

    if(!bv || start < 0 || size <= 0 || (start + size) > bv->size) {
        return 0;
    }

    /*
     * There are multiple cases to handle:
     *
     * 1) we are only setting one bit.
     * 2) the range is within one word.
     * 3) the range spans words.
     */

    /* case 1: pull this out for speed as it is common. */
    if(size == 1) {
        uint64_t mask = ((uint64_t)1 << (start % ELEM_BITS));
        bv->data[start/ELEM_BITS] |= mask;
        return 1;
    }

    first_elem = start/ELEM_BITS;
    last_elem = (start + size - 1)/ELEM_BITS;

    /* case 2 */
    if(first_elem == last_elem) {
        /*
         * low mask:
         *
         * We want a mask that starts at the first bit to be set and goes
         * up to the msb.
         *
         * first we set a single bit at the start of the range:
         *     msb 0...0000100000...0 lsb
         *                 ^
         *                 +--- start
         *
         * Then we subtract one to get all ones below this:
         *     msb 0...0000011111...1  lsb
         *                 ^
         *                 +--- start
         *
         * Then we invert it to get a mask that starts at the first bit and
         * goes up to the top bit in the word.
         *     msb 1...1111100000...0  lsb
         *                 ^
         *                 +--- start
         *
         * high mask:
         *
         * We want a mask that starts at the top bit to set and goes down
         * to the lsb.
         *
         * We short circuit if the top bit to set is the top bit of the word.  In that
         * case, the mask is all zero bits.
         *
         * If not, we set the bit at the top, this is just _past_ the top:
         *
         *     msb 0...0000100000...0 lsb
         *                 ^^
         *                 |+--- end
         *                 +---- start + size
         *
         * Then we subtract one to get all ones below this:
         *     msb 0...0000011111...1  lsb
         *                 ^^
         *                 |+--- end
         *                 +---- start + size
         *
         * Then we AND the two masks.
         *     msb 0...000111...111000...0 lsb
         *                ^       ^
         *                |       +--- start
         *                +----------- end
         */

        low_mask = ~(((uint64_t)1 << (start % ELEM_BITS))-1);
        high_mask = (((start + size) % ELEM_BITS) == 0 ? (uint64_t)0 : (((uint64_t)1 << ((start+size) % ELEM_BITS))-1));

        bv->data[start/ELEM_BITS] |= (low_mask & high_mask);

        return 1;
    }

    /* case 3 */

    /*
     * low mask:
     *
     * We want a mask that starts at the first bit to be set and goes
     * up to the msb.
     *
     * first we set a single bit at the start of the range:
     *     msb 0...0000100000...0 lsb
     *                 ^
     *                 +--- start
     *
     * Then we subtract one to get all ones below this:
     *     msb 0...0000011111...1  lsb
     *                 ^
     *                 +--- start
     *
     * Then we invert it to get a mask that starts at the first bit and
     * goes up to the top bit in the word.
     *     msb 1...1111100000...0  lsb
     *                 ^
     *                 +--- start
     *
     * high mask:
     *
     * We want a mask that starts at the top bit to set and goes down
     * to the lsb.
     *
     * We short circuit if the top bit to set is the top bit of the word.  In that
     * case, the mask is all zero bits.
     *
     * If not, we set the bit at the top, this is just _past_ the top:
     *
     *     msb 0...0000100000...0 lsb
     *                 ^^
     *                 |+--- end
     *                 +---- start + size
     *
     * Then we subtract one to get all ones below this:
     *     msb 0...0000011111...1  lsb
     *                 ^^
     *                 |+--- end
     *                 +---- start + size
     *
     * Then we AND the two masks.
     *     msb 0...000111...111000...0 lsb
     *                ^       ^
     *                |       +--- start
     *                +----------- end
     *
     * We set all the words in between to all ones.
     */

    low_mask = ~(((uint64_t)1 << (start % ELEM_BITS))-1);
    high_mask = (((start + size) % ELEM_BITS) == 0 ? (uint64_t)0 : (((uint64_t)1 << ((start+size) % ELEM_BITS))-1));

    /* loop over the words */
    for(int index = first_elem; index <= last_elem; index++) {
        if(index == first_elem) {
            bv->data[index] |= low_mask;
        } else if(index == last_elem) {
            bv->data[index] |= high_mask;
        } else {
            bv->data[index] = ALL_ONES;
        }
    }

    return 1;
}


int bitvec_clear_range(bitvec_p bv, int start, int size)
{
    int first_elem;
    int last_elem;
    uint64_t low_mask = 0;
    uint64_t high_mask = 0;

    if(!bv || start < 0 || size <= 0 || (start + size) > bv->size) {
        return 0;
    }

    /*
     * There are multiple cases to handle:
     *
     * 1) we are only clearing one bit.
     * 2) the range is within one word.
     * 3) the range spans words.
     */

    /* case 1: pull this out for speed as it is common. */
    if(size == 1) {
        uint64_t mask = ~((uint64_t)1 << (start % ELEM_BITS));
        bv->data[start/ELEM_BITS] &= mask;
        return 1;
    }

    first_elem = start/ELEM_BITS;
    last_elem = (start + size - 1)/ELEM_BITS;

    /* case 2 */
    if(first_elem == last_elem) {
        /*
         * low mask:
         *
         * We want a mask that starts at the first bit to be set and goes
         * up to the msb.
         *
         * first we set a single bit at the start of the range:
         *     msb 0...0000100000...0 lsb
         *                 ^
         *                 +--- start
         *
         * Then we subtract one to get all ones below this:
         *     msb 0...0000011111...1  lsb
         *                 ^
         *                 +--- start
         *
         * Then we invert it to get a mask that starts at the first bit and
         * goes up to the top bit in the word.
         *     msb 1...1111100000...0  lsb
         *                 ^
         *                 +--- start
         *
         * high mask:
         *
         * We want a mask that starts at the top bit to set and goes down
         * to the lsb.
         *
         * We short circuit if the top bit to set is the top bit of the word.  In that
         * case, the mask is all zero bits.
         *
         * If not, we set the bit at the top, this is just _past_ the top:
         *
         *     msb 0...0000100000...0 lsb
         *                 ^^
         *                 |+--- end
         *                 +---- start + size
         *
         * Then we subtract one to get all ones below this:
         *     msb 0...0000011111...1  lsb
         *                 ^^
         *                 |+--- end
         *                 +---- start + size
         *
         * Then we AND the two masks.
         *     msb 0...000111...111000...0 lsb
         *                ^       ^
         *                |       +--- start
         *                +----------- end
         *
         * This is the range we want to _clear_, so we invert it.
         *     msb 1...111000...000111...1 lsb
         *                ^       ^
         *                |       +--- start
         *                +----------- end
         *
         */

        low_mask = ~(((uint64_t)1 << (start % ELEM_BITS))-1);
        high_mask = (((start + size) % ELEM_BITS) == 0 ? (uint64_t)0 : (((uint64_t)1 << ((start+size) % ELEM_BITS))-1));

        bv->data[start/ELEM_BITS] &= ~(low_mask & high_mask);

        return 1;
    }

    /* case 3 */

    /*
     * low mask:
     *
     * We want a mask that starts below the first bit to be set and goes
     * down to the lsb.
     *
     * first we set a single bit at the start of the range:
     *     msb 0...0000100000...0 lsb
     *                 ^
     *                 +--- start
     *
     * Then we subtract one to get all ones below this:
     *     msb 0...0000011111...1  lsb
     *                 ^
     *                 +--- start
     *
     * high mask:
     *
     * We want a mask that starts above the high bit to set and goes up
     * to the msb.
     *
     * We short circuit if the top bit to set is the top bit of the word.  In that
     * case, the mask is all zero bits.
     *
     * If not, we set the bit at the top, this is just _past_ the top:
     *
     *     msb 0...0000100000...0 lsb
     *                 ^^
     *                 |+--- end
     *                 +---- start + size
     *
     * Then we subtract one to get all ones below this:
     *     msb 0...0000011111...1  lsb
     *                 ^^
     *                 |+--- end
     *                 +---- start + size
     *
     * Then we invert it to get the upper mask.
     *     msb 1...11111000000...0  lsb
     *                 ^^
     *                 |+--- end
     *                 +---- start + size
     *
     * We set all the words in between to all zeros.
     */

    low_mask = (((uint64_t)1 << (start % ELEM_BITS))-1);
    high_mask = (((start + size) % ELEM_BITS) == 0 ? (uint64_t)0 : ~(((uint64_t)1 << ((start+size) % ELEM_BITS))-1));

    /* loop over the words */
    for(int index = first_elem; index <= last_elem; index++) {
        if(index == first_elem) {
            bv->data[index] &= low_mask;
        } else if(index == last_elem) {
            bv->data[index] &= high_mask;
        } else {
            bv->data[index] = 0;
        }
    }

    return 1;
}



int bitvec_find_first(bitvec_p bv)
{
    int num_elems = 0;

    if(!bv) {
        return -1;
    }

    num_elems = bv->size / ELEM_BITS;

    //printf("bitvec_find_first() num_elems = %d\n",num_elems);

    for(int index=0; index < num_elems; index++) {
        if(bv->data[index] != 0) {
            //printf("bitvec_find_first() found first bit in element %d at position %d.\n", index, find_first(bv->data[index]));
            return (index * ELEM_BITS) + find_first(bv->data[index]);
        }
    }

    return -1;
}




int bitvec_popcount_upto(bitvec_p bv, int top)
{
    int max_elem = 0;
    int sum = 0;

    if(!bv || top >= bv->size) {
        return -1;
    }

    max_elem = top / ELEM_BITS;

    //printf("max_elem = %d\n",max_elem);

    for(int index = 0; index <= max_elem; index++) {
        if(index != max_elem) {
            sum += popcount64(bv->data[index]);
        } else {
            uint64_t mask = (((uint64_t)1 << (top % ELEM_BITS))-1);

            //printf("mask = %lx & data = %lx\n", mask, bv->data[index]);

            sum += popcount64(bv->data[index] & mask);
        }
    }

    return sum;
}



/* helper functions */

/*
 * Algorithm from Wikipedia.
 *
 * This implements a population count/Hamming weight count for a 64-bit
 * integer.  The assumption is that multiplication is fast.
 */



static const uint64_t m1  = 0x5555555555555555; //binary: 0101...
static const uint64_t m2  = 0x3333333333333333; //binary: 00110011..
static const uint64_t m4  = 0x0f0f0f0f0f0f0f0f; //binary:  4 zeros,  4 ones ...
static const uint64_t m8  = 0x00ff00ff00ff00ff; //binary:  8 zeros,  8 ones ...
static const uint64_t m16 = 0x0000ffff0000ffff; //binary: 16 zeros, 16 ones ...
static const uint64_t m32 = 0x00000000ffffffff; //binary: 32 zeros, 32 ones
static const uint64_t h01 = 0x0101010101010101; //the sum of 256 to the power of 0,1,2,3...


int popcount64(uint64_t x)
{
    if(x == ALL_ONES) {
        return ELEM_BITS;
    } else if(x == 0) {
        return 0;
    }

    x -= (x >> 1) & m1;             //put count of each 2 bits into those 2 bits
    x = (x & m2) + ((x >> 2) & m2); //put count of each 4 bits into those 4 bits
    x = (x + (x >> 4)) & m4;        //put count of each 8 bits into those 8 bits
    return (x * h01) >> 56;  //returns left 8 bits of x + (x<<8) + (x<<16) + (x<<24) + ...
}


/*
 * Thanks to Wikipedia, we know that ffs(x) = pop(x ^ (~(−x))).
 *
 * But we want one less than that because we want to start indexes at zero.
 */


int find_first(uint64_t x)
{
    return popcount64(x ^ (~(-(int64_t)x)))-1;
}
