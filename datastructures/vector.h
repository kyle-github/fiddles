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
static inline size_t CONCAT(T,_size)(T *vec) \
{ \
    if(vec) return vec->size; \
    else return 0; \
} \
static inline size_t CONCAT(T,_capacity)(T *vec) \
{ \
    if(vec) return vec->capacity; \
    else return 0; \
} \
size_t CONCAT(T,_splice)(T *vec, size_t loc, ELEM_TYPE *data, size_t data_len); \
size_t CONCAT(T,_cut)(T *vec, size_t loc, size_t len); \
static inline ELEM_TYPE CONCAT(T,_get)(T *vec, size_t loc) { return vec->elements[loc]; } \
static inline size_t CONCAT(T,_set)(T *vec, size_t loc, ELEM_TYPE val) { vec->elements[loc] = (val); return loc; }



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
} \
void CONCAT(T,_destroy)(T *vec) \
{ \
    if(!vec) return; \
    if(vec->elements) free(vec->elements); \
    free(vec); \
} \
size_t CONCAT(T,_splice)(T *vec, size_t loc, ELEM_TYPE *data, size_t data_len) \
{ \
    if(!vec) return 0; \
    size_t new_size = vec->size + data_len; \
    if(new_size > vec->capacity) { \
        size_t new_cap = ((new_size + vec->inc - 1) / vec->inc) * vec->inc; \
        ELEM_TYPE *new_elements = calloc(new_cap, sizeof(ELEM_TYPE)); \
        if(!new_elements) return 0; \
        for(size_t elem=0; elem < vec->size; elem++) { new_elements[elem] = vec->elements[elem]; } \
        free(vec->elements); \
        vec->elements = new_elements; \
        vec->capacity = new_cap; \
    } \
    if(loc < vec->size) { for(size_t elem = vec->size - 1; elem >= loc; elem--) { vec->elements[elem+data_len] = vec->elements[elem]; } } \
    for(size_t elem = 0; elem < data_len; elem++) { vec->elements[elem+loc] = data[elem]; } \
    vec->size = new_size; \
    return loc + data_len; \
} \
size_t CONCAT(T,_cut)(T *vec, size_t loc, size_t len) \
{ \
    if(!vec) return 0; \
    if(loc > vec->size) return 0; \
    if((loc + len) > vec->size) { len = vec->size - loc; } \
    for(size_t elem = loc + len; elem < vec->size; elem++) { \
        vec->elements[elem - len] = vec->elements[elem]; \
    } \
    vec->size = vec->size - len; \
    return loc; \
}



#endif

