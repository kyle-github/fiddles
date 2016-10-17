/*
 * generic vector type
 *
 * This implements a very simple vector type.
 */


#ifndef __VECTOR_H__
#define __VECTOR_H__ 1

#include <stdint.h>

#ifndef CONCAT
#define CONCAT(A,B) CONCAT_1(A,B)
#define CONCAT_1(A,B) A##B
#endif

#define VECTOR_ERROR SIZE_MAX


#define VECTOR_MAKE_IMPL(T,ELEM_TYPE) typedef struct { \
    size_t capacity; \
    size_t inc; \
    size_t size; \
    ELEM_TYPE *elements; \
} T; \
T *CONCAT(T,_create)(size_t capacity, size_t inc) \
{ \
    if(capacity <= 0) { \
        return NULL; \
    } \
    T *new_vec = calloc(1,sizeof(T)); \
    if(!new_vec) { \
        return NULL; \
    } \
    new_vec->elements = calloc(1,sizeof(ELEM_TYPE)); \
    if(!new_vec->elements) { \
        free(new_vec); \
        return NULL; \
    } \
    new_vec->capacity = capacity; \
    new_vec->inc = inc; \
    new_vec->size = 0; \
    return new_vec; \
} \
void CONCAT(T,_destroy)(T *vec) {  if(!vec) return; if(vec->elements) free(vec->elements); free(vec); } \
size_t CONCAT(T,_size)(T *vec) { if(vec) return vec->size; else return VECTOR_ERROR; } \
size_t CONCAT(T,_capacity)(T *vec) { if(vec) return vec->capacity; else return VECTOR_ERROR; } \
ELEM_TYPE *CONCAT(T,_elements)(T *vec) { if(!vec) return NULL; return vec->elements; } \
size_t CONCAT(T,_ensure_capacity)(T *vec, size_t new_size) \
{ \
    if(!vec) return VECTOR_ERROR; \
    if(new_size > vec->capacity) { \
        size_t new_cap = ((new_size + vec->inc - 1) / vec->inc) * vec->inc; \
        ELEM_TYPE *new_elements = calloc(new_cap, sizeof(ELEM_TYPE)); \
        if(!new_elements) return VECTOR_ERROR; \
        for(size_t elem=0; elem < vec->size; elem++) { new_elements[elem] = vec->elements[elem]; } \
        free(vec->elements); \
        vec->elements = new_elements; \
        vec->capacity = new_cap; \
    } \
    return vec->capacity; \
} \
size_t CONCAT(T,_splice)(T *vec, size_t loc, ELEM_TYPE *data, size_t data_len) \
{ \
    if(!vec || loc > vec->size) return VECTOR_ERROR; \
    size_t new_size = vec->size + data_len; \
    if(CONCAT(T,_ensure_capacity)(vec, new_size) == VECTOR_ERROR) return VECTOR_ERROR; \
    size_t move_up_amt = vec->size - loc; \
    for( ; move_up_amt; move_up_amt--) { \
        vec->elements[loc + move_up_amt + data_len - 1] = vec->elements[loc + move_up_amt - 1]; \
    } \
    for(size_t elem = 0; elem < data_len; elem++) { \
        vec->elements[elem+loc] = data[elem]; \
    } \
    vec->size = new_size; \
    return loc + data_len; \
} \
size_t CONCAT(T,_cut)(T *vec, size_t loc, size_t len) \
{ \
    if(!vec) return VECTOR_ERROR; \
    if(loc > vec->size) return VECTOR_ERROR; \
    if((loc + len) > vec->size) { len = vec->size - loc; } \
    for(size_t elem = loc + len; elem < vec->size; elem++) { \
        vec->elements[elem - len] = vec->elements[elem]; \
    } \
    vec->size = vec->size - len; \
    return loc; \
} \
ELEM_TYPE CONCAT(T,_get)(T *vec, size_t loc) { return vec->elements[loc]; } \
size_t CONCAT(T,_set)(T *vec, size_t loc, ELEM_TYPE val) { if(!vec || loc >= vec->size) return VECTOR_ERROR; vec->elements[loc] = (val); return loc; } \
size_t CONCAT(T,_push)(T *vec, ELEM_TYPE val) { if(!vec) return VECTOR_ERROR; return CONCAT(T,_splice)(vec, 0, &val, (size_t)1); } \
size_t CONCAT(T,_pop)(T *vec, ELEM_TYPE *val) { if(!vec || vec->size <= 0) return VECTOR_ERROR; *val = CONCAT(T,_get)(vec, (size_t)0); return CONCAT(T,_cut)(vec, (size_t)0, (size_t)1); } \
size_t CONCAT(T,_append)(T *vec, ELEM_TYPE val) { if(!vec || CONCAT(T,_ensure_capacity)(vec, vec->size+1) == VECTOR_ERROR) return VECTOR_ERROR; vec->elements[vec->size] = val; vec->size++; return vec->size; } \
size_t CONCAT(T,_trunc)(T *vec, ELEM_TYPE *val) { if(!vec && vec->size <= 0) return VECTOR_ERROR; *val = vec->elements[vec->size - 1]; vec->size--; return vec->size; }


#endif

