#include <assert.h>
#include <stdio.h>
#include "bitvec.h"

#define BITVEC_SIZE (300)

int main(int argc, const char **argv)
{
    bitvec_p bv = bitvec_create(BITVEC_SIZE);
    int status = 0;
    int val = 0;
    int index = 0;
    int count = 0;

    printf("Creating bit vector.\n");
    bv = bitvec_create(BITVEC_SIZE);
    assert(bv != NULL);
    printf("OK\n");

    /* test == basic bit setting */
    /* case 1 test. */
    printf("Test setting single bit 40.\n");
    status = bitvec_set_range(bv, 40, 1);
    assert(status != 0);
    printf("OK\n");

    printf("Test individual bit 40\n");
    val = bitvec_get(bv, 40);
    assert(val == 1);
    printf("OK\n");

    /* get bit finding */
    printf("Test for first bit set is bit 40.\n");
    index = bitvec_find_first(bv);
    assert(index == 40);
    printf("OK\n");

    printf("Test setting bit range in a word from bits 5-9 inclusive.\n");
    status = bitvec_set_range(bv, 5, 5);
    assert(status != 0);
    printf("OK\n");

    printf("Test individual bit 0\n");
    val = bitvec_get(bv, 0);
    assert(val == 0);
    printf("OK\n");

    printf("Test individual bit 1\n");
    val = bitvec_get(bv, 1);
    assert(val == 0);
    printf("OK\n");

    printf("Test individual bit 2\n");
    val = bitvec_get(bv, 2);
    assert(val == 0);
    printf("OK\n");

    printf("Test individual bit 3\n");
    val = bitvec_get(bv, 3);
    assert(val == 0);
    printf("OK\n");

    printf("Test individual bit 4\n");
    val = bitvec_get(bv, 4);
    assert(val == 0);
    printf("OK\n");

    printf("Test individual bit 5\n");
    val = bitvec_get(bv, 5);
    assert(val == 1);
    printf("OK\n");

    printf("Test individual bit 6\n");
    val = bitvec_get(bv, 6);
    assert(val == 1);
    printf("OK\n");

    printf("Test individual bit 7\n");
    val = bitvec_get(bv, 7);
    assert(val == 1);
    printf("OK\n");

    printf("Test individual bit 8\n");
    val = bitvec_get(bv, 8);
    assert(val == 1);
    printf("OK\n");

    printf("Test individual bit 9\n");
    val = bitvec_get(bv, 9);
    assert(val == 1);
    printf("OK\n");

    printf("Test individual bit 10\n");
    val = bitvec_get(bv, 10);
    assert(val == 0);
    printf("OK\n");

    printf("Test for first bit set is bit 5.\n");
    index = bitvec_find_first(bv);
    assert(index == 5);
    printf("OK\n");

    printf("Test popcount up to 5.\n");
    count = bitvec_popcount_upto(bv, 5);
    assert(count == 0);
    printf("OK\n");

    printf("Test popcount up to 10.\n");
    count = bitvec_popcount_upto(bv, 10);
    assert(count == 5);
    printf("OK\n");

    printf("Test popcount up to 100.\n");
    count = bitvec_popcount_upto(bv, 100);
    assert(count == 6);
    printf("OK\n");

    printf("Test setting bit range in a word from bits 31-33 inclusive.\n");
    status = bitvec_set_range(bv, 31, 3);
    assert(status != 0);
    printf("OK\n");

    printf("Test popcount up to 100.\n");
    count = bitvec_popcount_upto(bv, 100);
    assert(count == 9);
    printf("OK\n");

    /* case 3 of set */
    printf("Test setting bit range across multiple words from bit 16 up to bit 144.\n");
    status = bitvec_set_range(bv, 16, 128);
    assert(status != 0);
    printf("OK\n");

    printf("Test popcount up to 150.\n");
    count = bitvec_popcount_upto(bv, 150);
    assert(count == 133);
    printf("OK\n");

    bitvec_destroy(bv);
    printf("All OK\n");

    return 0;
}
