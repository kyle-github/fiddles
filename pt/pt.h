/*
 * Test code for Protothread implementation.
 *
 * This is based on Adam Dunkel's Protothreads.
 *
 */

#ifndef __PT_H__
#define __PT_H__

#include "../macros/macros.h"

typedef void *pt_context_p;

#define PT_TERMINATE (0)
#define PT_RESUME (1)

#define PT_MAKE_FUNC_ARG(T,V) PT_MAKE_FUNC_ARG_1(T,V)
#define PT_MAKE_FUNC_ARG_1(T,V) T V,
#define PT_MAKE_FUNC_ARG_LAST(T,V) PT_MAKE_FUNC_ARG_LAST_1(T,V)
#define PT_MAKE_FUNC_ARG_LAST_1(T,V) T V
#define PT_MAKE_STRUCT_FIELD(T,V) PT_MAKE_STRUCT_FIELD_1(T,V)
#define PT_MAKE_STRUCT_FIELD_1(T,V) T V;
#define PT_MAKE_ARG_ASSIGN(T,V) PT_MAKE_ARG_ASSIGN_1(T,V)
#define PT_MAKE_ARG_ASSIGN_1(T,V) pt_args->V = V;

#define PT_CONCAT(A,B) PT_CONCAT_1(A,B)
#define PT_CONCAT_1(A,B) A##B

#define PT_FUNC(name, ...) \
struct PT_CONCAT(pt_args_,name) { \
    FOREACH_PAIR(PT_MAKE_STRUCT_FIELD,__VA_ARGS__) \
}; \
static int PT_CONCAT(pt_func_,name)(void *args_arg, void **ctx_arg); \
int name(FOREACH_PAIR_LAST(PT_MAKE_FUNC_ARG, PT_MAKE_FUNC_ARG_LAST, __VA_ARGS__)) {\
    struct pt_args_##name *pt_args = calloc(1, sizeof(struct pt_args_##name)); \
    if(!pt_args) return -1; \
    FOREACH_PAIR(PT_MAKE_ARG_ASSIGN,__VA_ARGS__) \
    return pt_schedule(PT_CONCAT(pt_func_,name),pt_args); \
} \
int PT_CONCAT(pt_func_,name)(void *args_arg, void **ctx_arg) { \
    struct PT_CONCAT(pt_args_,name) *args = (struct PT_CONCAT(pt_args_,name) *)args_arg;

#define PT_CONTEXT(...) \
    struct pt_ctx { \
        int pt_state; \
        FOREACH_PAIR(PT_MAKE_STRUCT_FIELD,__VA_ARGS__) \
    } *ctx = (struct pt_ctx *)(*ctx_arg); \
    if(!ctx) { \
        ctx = calloc(1, sizeof(struct pt_ctx)); \
        if(!ctx) return PT_TERMINATE; \
        *ctx_arg = ctx; \
    }

#define PT_BODY \
    switch(ctx->pt_state) { \
        case 0:

#define PT_EXIT \
    do { ctx->pt_state = 0; return PT_TERMINATE; } while(0)

#define PT_END \
    default: \
        break; \
    } \
    PT_EXIT; \
}

#define PT_YIELD ctx->pt_state = __LINE__; return PT_RESUME; case __LINE__: do {} while(0)
#define PT_WAIT_WHILE(cond) do {ctx->pt_state = __LINE__; case __LINE__: if((cond)) return PT_RESUME; } while(0)
#define PT_WAIT_UNTIL(cond) do {ctx->pt_state = __LINE__; case __LINE__: if(!(cond)) return PT_RESUME; } while(0)

typedef int (*pt_func)(void *, void **);

extern int pt_schedule(pt_func func, void *args);
extern int pt_run(void);



#endif
