typedef struct bitvec *bitvec_p;

extern bitvec_p bitvec_create(int size);
extern void bitvec_destroy(bitvec_p bv);
extern int bitvec_get(bitvec_p bv, int index);
extern int bitvec_set_range(bitvec_p bv, int start, int size);
extern int bitvec_clear_range(bitvec_p bv, int start, int size);
extern int bitvec_find_first(bitvec_p bv);
extern int bitvec_popcount_upto(bitvec_p bv, int top);
