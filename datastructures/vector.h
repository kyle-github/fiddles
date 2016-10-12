/*
 * generic vector type
 *
 * This implements a very simple vector type.
 */


#ifndef __VECTOR_H__
#define __VECTOR_H__ 1

#ifndef CONCAT
#define CONCAT(A,B) CONCAT_1(A,B)
#define CONCAT_1(A,B) A##B
#endif

#ifdef VECTOR
#warning Vector is defined!
#endif

#undef VECTOR_DECL
#define VECTOR_DECL(T,ELEM_TYPE) typedef struct { \
    size_t capacity; \
    size_t inc; \
    size_t size; \
    ELEM_TYPE *elements; \
} T; \
T *CONCAT(T,_create)(size_t capacity, size_t inc); \
void CONCAT(T,_destroy)(T *vec); \
size_t CONCAT(T,_size)(T *vec); \
size_t CONCAT(T,_capacity)(T *vec); \
size_t CONCAT(T,_splice)(T *vec, size_t loc, ELEM_TYPE *data, size_t data_len); \
size_t CONCAT(T,_cut)(T *vec, size_t loc, size_t len);



#define VECTOR_IMPL(T,ELEM_TYPE) \
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
}\
size_t CONCAT(T,_size)(T *vec) \
{ \
    if(vec) return vec->size; \
    else return 0; \
} \
size_t CONCAT(T,_capacity)(T *vec) \
{ \
    if(vec) return vec->capacity; \
    else return 0; \
} \
size_t CONCAT(T,_splice)(T *vec, size_t loc, ELEM_TYPE *data, size_t data_len) \
{ \
    size_t new_size = vec->size + data_len; \
    if(!vec) return 0; \
    if(new_size > capacity) { \
        size_t new_cap = ((new_size + vec->inc - 1) / vec->inc) * vec->inc; \
        ELEM_TYPE *new_elements = calloc(new_cap, sizeof(ELEM_TYPE)); \
        if(!new_elements) return 0; \
        vec->capacity = new_cap; \
    } \
    for(size_t elem = 0; elem < new_size; elem++) { \
        if(elem < loc) { new_elements[elem] = vec->elements[elem]; } \
        else if(elem < (loc+data_len) { new_elements[elem] = data[elem - loc]; } \
        else { new_elements[elem] = vec->elements[elem - data_len]; } \
    } \
    free(vec->elements); \
    vec->elements = new_elements; \
    return loc + data_len; \
}



#endif

