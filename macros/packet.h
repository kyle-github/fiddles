
/*
 * This is a collection of handy macros.
 */

/* select one out of a list */
#define GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11,_12,_13,_14,_15,_16,NAME,...) NAME

#define FOREACH_PAIR_2(WHAT, WHAT_LAST, X, Y) WHAT_LAST(X,Y)
#define FOREACH_PAIR_4(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_2(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_6(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_4(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_8(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_6(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_10(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_8(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_12(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_10(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_14(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_12(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_PAIR_16(WHAT, WHAT_LAST, X, Y, ...) WHAT(X,Y)FOREACH_PAIR_14(WHAT, WHAT_LAST, __VA_ARGS__)

/* run an action macro against all elements passed as arguments, handles arguments in PAIRS */
#define FOREACH_PAIR(action, ...) \
  GET_MACRO(__VA_ARGS__,FOREACH_PAIR_16,OOPS15,FOREACH_PAIR_14,OOPS13,FOREACH_PAIR_12,OOPS11,FOREACH_PAIR_10,OOPS9,FOREACH_PAIR_8,OOPS7,FOREACH_PAIR_6,OOPS5,FOREACH_PAIR_4,OOPS3,FOREACH_PAIR_2,OOPS1,)(action,action,__VA_ARGS__)

/* as above, but run a second macro function against the last element in the list */
#define FOREACH_PAIR_LAST(action, action_last, ...) \
  GET_MACRO(__VA_ARGS__,FOREACH_PAIR_16,OOPS15,FOREACH_PAIR_14,OOPS13,FOREACH_PAIR_12,OOPS11,FOREACH_PAIR_10,OOPS9,FOREACH_PAIR_8,OOPS7,FOREACH_PAIR_6,OOPS5,FOREACH_PAIR_4,OOPS3,FOREACH_PAIR_2,OOPS1,)(action,action_last,__VA_ARGS__)


#define FOREACH_1(WHAT, WHAT_LAST, X) WHAT_LAST(X)
#define FOREACH_2(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_1(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_3(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_2(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_4(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_3(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_5(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_4(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_6(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_5(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_7(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_6(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_8(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_7(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_9(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_8(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_10(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_9(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_11(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_10(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_12(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_11(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_13(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_12(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_14(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_13(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_15(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_14(WHAT, WHAT_LAST, __VA_ARGS__)
#define FOREACH_16(WHAT, WHAT_LAST, X, ...) WHAT(X)FOREACH_15(WHAT, WHAT_LAST, __VA_ARGS__)


/* run action macro against all elements in a list. */
#define FOREACH(action, ...) \
   GET_MACRO(__VA_ARGS__,FOREACH_16,FOREACH_15,FOREACH_14,FOREACH_13,FOREACH_12,FOREACH_11,FOREACH_10,FOREACH_9,FOREACH_8,FOREACH_7,FOREACH_6,FOREACH_5,FOREACH_4,FOREACH_3,FOREACH_2,FOREACH_1,)(action,action,__VA_ARGS__)

/* run action macro against all elements in a list. Run a different macro against the last element. */
#define FOREACH_LAST(action, action_last, ...) \
   GET_MACRO(__VA_ARGS__,FOREACH_16,FOREACH_15,FOREACH_14,FOREACH_13,FOREACH_12,FOREACH_11,FOREACH_10,FOREACH_9,FOREACH_8,FOREACH_7,FOREACH_6,FOREACH_5,FOREACH_4,FOREACH_3,FOREACH_2,FOREACH_1,)(action,action_last,__VA_ARGS__)


#define STRINGIFY(X) STRINGIFY_1(X)
#define STRINGIFY_1(X) STRINGIFY_2(X)
#define STRINGIFY_2(X) STRINGIFY_3(X)
#define STRINGIFY_3(X) STRINGIFY_4(X)
#define STRINGIFY_4(X) STRINGIFY_5(X)
#define STRINGIFY_5(X) #X


#define CONCAT(A,B) CONCAT_1(A,B)
#define CONCAT_1(A,B) CONCAT_2(A,B)
#define CONCAT_2(A,B) A ## B

#define MAKE_FIELD(T, N) MAKE_FIELD_1(T,N)
#define MAKE_FIELD_1(T,N) \
 T N; \


#define MAKE_STRUCT_NAME(base_name) CONCAT(packet_,base_name)
#define MAKE_STRUCT(base_name, ...) \
    struct MAKE_STRUCT_NAME(base_name) { \
    FOREACH_PAIR(MAKE_FIELD,__VA_ARGS__) \
}; \
typedef struct MAKE_STRUCT_NAME(base_name) *MAKE_STRUCT_NAME(base_name); \


#define MAKE_LENGTH_NAME(T) CONCAT(length_,T)
#define MAKE_FIELD_LENGTH(T,N) + MAKE_LENGTH_NAME(T)(N)
#define MAKE_LENGTH(base_name, ...) \
int MAKE_LENGTH_NAME(base_name)(MAKE_STRUCT_NAME(base_name) data) \
{ \
    return 0 FOREACH_PAIR(MAKE_FIELD_LENGTH,__VA_ARGS__) ; \
} \


#define MAKE_MARSHALL_NAME(T) CONCAT(marshall_,T)
#define MAKE_FIELD_MARSHALL(T,N) if(offset >= 0) {offset = MAKE_MARSHALL_NAME(T)(buf, offset, data->N);} else {pdebug(DEBUG_WARN,"Unable to marshal field " #N "! rc=%d",offset); }
#define MAKE_MARSHALL(base_name, ...) \
int MAKE_MARSHALL_NAME(base_name)(byte_buffer_p buf, int offset, MAKE_STRUCT_NAME(base_name) data) \
{ \
    FOREACH_PAIR(MAKE_FIELD_MARSHALL,__VA_ARGS__) \
    return offset; \
} \

#define MAKE_UNMARSHALL_NAME(T) CONCAT(unmarshall_,T)
#define MAKE_FIELD_UNMARSHALL(T,N) if(offset >= 0) {offset = MAKE_UNMARSHALL_NAME(T)(buf, offset, &data->N);} else {pdebug(DEBUG_WARN,"Unable to unmarshal field " #N "! rc=%d",offset); }
#define MAKE_UNMARSHALL(base_name, ...) \
int MAKE_UNMARSHALL_NAME(base_name)(byte_buffer_p buf, int offset, MAKE_STRUCT_NAME(base_name) *data) \
{ \
    FOREACH_PAIR(MAKE_FIELD_UNMARSHALL,__VA_ARGS__) \
    return offset; \
} \



#define PACKET(base_name, ...) \
    MAKE_STRUCT(base_name, __VA_ARGS__) \
    MAKE_LENGTH(base_name, __VA_ARGS__) \
    MAKE_MARSHALL(base_name, __VA_ARGS__) \
    MAKE_UNMARSHALL(base_name, __VA_ARGS__) \



PACKET(eip_header, \
    uint16_t,command, \
    uint16_t,length, \
    uint32_t,session_handle, \
    uint32_t,status, \
    uint64_t,session_seq, \
    uint32_t,options \
    )



PACKET(session_register, \
    packet_eip_header,header, \
    uint16_t,eip_version, \
    uint16_t,option_flags \
    )



