#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "vector.h"

VECTOR_MAKE_IMPL(int_vec, int)


//#include <stdint.h>
/*
#define VECTOR_ERROR SIZE_MAX

typedef struct {
    size_t capacity;
    size_t inc;
    size_t size;
    int *elements;
} int_vec;

int_vec *int_vec_create(size_t capacity, size_t inc)
{
    if(capacity <= 0) { return NULL; }
    int_vec *new_vec = calloc(1,sizeof(int_vec));
    if(!new_vec) { return NULL; }
    new_vec->elements = calloc(1,sizeof(int));
    if(!new_vec->elements) { free(new_vec); return NULL; }
    new_vec->capacity = capacity;
    new_vec->inc = inc;
    new_vec->size = 0;
    return new_vec;
}
static inline void int_vec_destroy(int_vec *vec)
{
    if(!vec) return;
    if(vec->elements) free(vec->elements);
    free(vec);
}
static inline size_t int_vec_size(int_vec *vec)
{
    if(vec) return vec->size; else return SIZE_MAX;
}
static inline size_t int_vec_capacity(int_vec *vec)
{
    if(vec) return vec->capacity; else return SIZE_MAX;
}
static inline int *int_vec_elements(int_vec *vec)
{
    if(!vec) return NULL;
    return vec->elements;
}
size_t int_vec_ensure_capacity(int_vec *vec, size_t new_size)
{
    if(!vec) return SIZE_MAX;
    if(new_size > vec->capacity) {
        size_t new_cap = ((new_size + vec->inc - 1) / vec->inc) * vec->inc;
        int *new_elements = calloc(new_cap, sizeof(int));
        if(!new_elements) return SIZE_MAX;
        for(size_t elem=0; elem < vec->size; elem++) {
            new_elements[elem] = vec->elements[elem];
        }
        free(vec->elements);
        vec->elements = new_elements;
        vec->capacity = new_cap;
    }
    return vec->capacity;
}
size_t int_vec_splice(int_vec *vec, size_t loc, int *data, size_t data_len)
{
    if(!vec || loc > vec->size) return SIZE_MAX;
    size_t new_size = vec->size + data_len;
    if(int_vec_ensure_capacity(vec, new_size) == SIZE_MAX) return SIZE_MAX;
    size_t move_up_amt = vec->size - loc;
    fprintf(stderr,"move_up_amt=%ld\n",move_up_amt);
    for( ; move_up_amt; move_up_amt--) {
        fprintf(stderr,"moving element (%d) %ld to %ld\n", vec->elements[loc + move_up_amt - 1], loc + move_up_amt - 1, loc + move_up_amt + data_len - 1);
        vec->elements[loc + move_up_amt + data_len - 1] = vec->elements[loc + move_up_amt - 1];
    }
    for(size_t elem = 0; elem < data_len; elem++) {
        fprintf(stderr,"moving element (%d) %ld to %ld\n", data[elem], elem, elem + loc);
        vec->elements[elem+loc] = data[elem];
    }
    vec->size = new_size;
    return loc + data_len;
}
size_t int_vec_cut(int_vec *vec, size_t loc, size_t len)
{
    if(!vec) return SIZE_MAX;
    if(loc > vec->size) return SIZE_MAX;
    if((loc + len) > vec->size) {
        len = vec->size - loc;
    }
    for(size_t elem = loc + len; elem < vec->size; elem++) {
        vec->elements[elem - len] = vec->elements[elem];
    }
    vec->size = vec->size - len;
    return loc;
}
static inline int int_vec_get(int_vec *vec, size_t loc)
{
    return vec->elements[loc];
}
static inline size_t int_vec_set(int_vec *vec, size_t loc, int val)
{
    if(!vec || loc >= vec->size) return SIZE_MAX;
    vec->elements[loc] = (val);
    return loc;
}
static inline size_t int_vec_push(int_vec *vec, int val)
{
    if(!vec) return SIZE_MAX;
    return int_vec_splice(vec, 0, &val, (size_t)1);
}
static inline size_t int_vec_pop(int_vec *vec, int *val)
{
    if(!vec || vec->size <= 0) return SIZE_MAX;
    *val = int_vec_get(vec, (size_t)0);
    return int_vec_cut(vec, (size_t)0, (size_t)1);
}
static inline size_t int_vec_append(int_vec *vec, int val)
{
    if(!vec || int_vec_ensure_capacity(vec, int_vec_size(vec)+1) == SIZE_MAX) return SIZE_MAX;
    vec->elements[vec->size] = val;
    vec->size++;
    return vec->size;
}
static inline size_t int_vec_trunc(int_vec *vec, int *val)
{
    if(!vec && int_vec_size(vec) <= 0) return SIZE_MAX;
    *val = vec->elements[vec->size - 1];
    vec->size--;
    return vec->size;
}
*/



#define test(msg, cond, ...) \
fprintf(stderr, "%s... ", msg); \
if(!(cond)) { \
    fprintf(stderr, __VA_ARGS__); \
    fprintf(stderr, "\n"); \
    abort(); \
} \
fprintf(stderr,"OK\n");


void int_vec_print(int_vec *vec)
{
    printf("vec(%ld,%ld)[",int_vec_size(vec), int_vec_capacity(vec));
    for(size_t elem = 0; elem < int_vec_size(vec); elem++) {
        if(elem != 0) printf(", ");
        printf("%d",int_vec_get(vec, elem));
    }
    printf("]\n");
}


int vector_compare(int_vec *vec, int *test_data, size_t data_size)
{
    if(int_vec_size(vec) != data_size) return 0;
    int_vec_print(vec);
    for(size_t index = 0; index < int_vec_size(vec); index++) {
        if(int_vec_get(vec,index) != test_data[index]) return 0;
    }
    return 1;
}

int main()
{
    int_vec *vec = NULL;
    size_t index = 0;
    int test_array[20] = { 0, };
    int tmp = 42;
    size_t test_array_size = (sizeof(test_array)/sizeof(int));

    /* test create */
    test("test vector creation.", (vec = int_vec_create(4, 10)) != NULL, "Vector was not created!");

    /* test push */
    test("push element into vector", int_vec_push(vec, 1) != VECTOR_ERROR, "Error pushing element into empty vector.");

    /* test vector size */
    test("test vector size == 1", int_vec_size(vec) == 1, "Error getting correct vector size, expected 1 got %ld.", int_vec_size(vec));

    /* test pop */
    test("pop element from vector", (int_vec_pop(vec, &tmp) != VECTOR_ERROR && tmp == 1), "Error popping element from vector.");

    /* test size again */
    test("test vector size after pop, size == 0?", int_vec_size(vec) == 0, "Error getting correct vector size after pop, expected 0 got %ld", int_vec_size(vec));

    /* test append */
    tmp = 10;
    test("test vector append", int_vec_append(vec, tmp) != VECTOR_ERROR, "Error appending to vector.");

    /* test size */
    test("test vector size after append, size == 1?", int_vec_size(vec) == 1, "Error getting vector size after append, got size %ld.", int_vec_size(vec));

    /* test get */
    test("test vector get", int_vec_get(vec,(size_t)0) == 10, "Vector get failed to return correct value, got %d.", int_vec_get(vec,(size_t)0));

    /* test truncate */
    tmp = 0;
    test("truncate element from vector", int_vec_trunc(vec, &tmp) != VECTOR_ERROR, "Error truncating element from vector.");

    /* test tmp value */
    test("test value after trunc", tmp == 10, "Error getting value from truncate.");

    /* set up vector with [ 0, 20 ] */
    test("test multiple push, push 20", int_vec_push(vec, 20) != VECTOR_ERROR, "First push failed.");
    test("test multiple push, push 0", int_vec_push(vec, 0) != VECTOR_ERROR, "second push failed.");
    test("test vector size after multiple push.", int_vec_size(vec) == 2, "vector size %ld not correct.", int_vec_size(vec));
    test("test first vector value after multiple push", int_vec_get(vec, (size_t)0) == 0, "Value not 0, value is %d", int_vec_get(vec, (size_t)0));
    test("test second vector value after multiple push", int_vec_get(vec, (size_t)1) == 20, "Value not 20, value is %d.", int_vec_get(vec, (size_t)1));

    /* test middle splice */
    test_array[0] = 31;
    test_array[1] = 32;
    test_array[2] = 33;
    test("test vector splice.", int_vec_splice(vec, (size_t)1, test_array, (size_t)3) != VECTOR_ERROR, "error while splicing.");
    test("test vector length == 5 after splice.", int_vec_size(vec) == 5, "vector size not correct, got %ld instead of 5.", int_vec_size(vec));
    int_vec_print(vec);
    test("test vector capacity == 10 after splice.", int_vec_capacity(vec) == 10, "vector capacity not correct, got %ld instead of 10.", int_vec_capacity(vec));
    test("test vector contents after splice, vec[0] == 0", int_vec_get(vec, (size_t)0) == 0, "vector contents not correct, expected 0 got %d", int_vec_get(vec, (size_t)0));
    test("test vector contents after splice, vec[1] == 31", int_vec_get(vec, (size_t)1) == 31, "vector contents not correct, expected 31 got %d", int_vec_get(vec, (size_t)1));
    test("test vector contents after splice, vec[2] == 32", int_vec_get(vec, (size_t)2) == 32, "vector contents not correct, expected 32 got %d", int_vec_get(vec, (size_t)2));
    test("test vector contents after splice, vec[3] == 33", int_vec_get(vec, (size_t)3) == 33, "vector contents not correct, expected 33 got %d", int_vec_get(vec, (size_t)3));
    test("test vector contents after splice, vec[4] == 20", int_vec_get(vec, (size_t)4) == 20, "vector contents not correct, expected 20 got %d", int_vec_get(vec, (size_t)4));

    /* test second splice */
    test_array[0] = 41;
    test_array[1] = 42;
    test_array[2] = 43;
    test_array[3] = 44;
    test_array[4] = 45;
    test("test second splice at beginning of vector.", int_vec_splice(vec, (size_t)0, test_array, (size_t)5) != VECTOR_ERROR, "error while splicing");
    test("test vector size after second splice", int_vec_size(vec) == (size_t)10, "expected size==10, but got %ld", int_vec_size(vec));
    test("test vector capacity after second splice", int_vec_capacity(vec) == (size_t)10, "expected capacity==10, but got %ld", int_vec_capacity(vec));
    test_array[0] = 41;
    test_array[1] = 42;
    test_array[2] = 43;
    test_array[3] = 44;
    test_array[4] = 45;
    test_array[5] = 0;
    test_array[6] = 31;
    test_array[7] = 32;
    test_array[8] = 33;
    test_array[9] = 20;
    test("test vector contents == [41, 42, 43, 44, 45, 0, 31, 32, 33, 20]", vector_compare(vec, test_array, (size_t)10), "vector data not correct!");

    /* test cut at end */
    test("test vector cut at the end by removing two elements.", int_vec_cut(vec, (size_t)8, (size_t)2) != VECTOR_ERROR, "vector cut failed!");
    test("test vector length after cut == 8", int_vec_size(vec) == 8, "vector length expected to be 8, but was %ld", int_vec_size(vec));
    test("test vector contents ==  [41, 42, 43, 44, 45, 0, 31, 32]", vector_compare(vec, test_array, (size_t)8), "vector data not correct!");

    /* test cut at beginning */
    test("test vector cut at the beginning by removing two elements.", int_vec_cut(vec,(size_t)0, (size_t)2) != VECTOR_ERROR, "vector cut failed!");
    test("test vector length after cut == 6", int_vec_size(vec) == 6, "vector length expected to be 6, but was %ld", int_vec_size(vec));
    test("test vector contents ==  [43, 44, 45, 0, 31, 32]", vector_compare(vec, &test_array[2], (size_t)6), "vector data not correct!");

    /* test cut in middle */
    test("test vector cut in the middle by removing four elements.", int_vec_cut(vec,(size_t)1, (size_t)4) != VECTOR_ERROR, "vector cut failed!");
    test("test vector length after cut == 2", int_vec_size(vec) == 2, "vector length expected to be 2, but was %ld", int_vec_size(vec));
    test("test vector contents ==  [43, 32]", (int_vec_get(vec,(size_t)0) == 43) && (int_vec_get(vec,(size_t)1) == 32), "vector data not correct!");

    /* test end splice */
    test("test splice at end of vector", int_vec_splice(vec, int_vec_size(vec), &test_array[6], (size_t)3) != VECTOR_ERROR, "vector splice failed.");
    test("test vector length after splice", int_vec_size(vec) == 5, "vector length wrong, expected 5 got %ld", int_vec_size(vec));
    test_array[0] = 43;
    test_array[1] = 32;
    test_array[2] = 31;
    test_array[3] = 32;
    test_array[4] = 33;
    test("test vector elements after splice", vector_compare(vec, test_array, (size_t)5), "vector data not correct!");

    /* test beginning splice */
    test_array[0] = -1;
    test_array[1] = -2;
    test_array[2] = 43;
    test_array[3] = 32;
    test_array[4] = 31;
    test_array[5] = 32;
    test_array[6] = 33;
    test("test splice at beginning of vector", int_vec_splice(vec, (size_t)0, &test_array[0], (size_t)2) != VECTOR_ERROR, "vector splice failed.");
    test("test vector size after splice == 7", int_vec_size(vec) == (size_t)7, "vector size expected 7, got %ld", int_vec_size(vec));
    test("test vector contents after splice", vector_compare(vec, test_array, (size_t)7), "vector data not correct!");

    /* test vector destroy */
    int_vec_destroy(vec);


    return 0;
}

