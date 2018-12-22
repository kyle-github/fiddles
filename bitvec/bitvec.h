typedef struct bitvec *bitvec_p;

extern bitvec_p bitvec_create(int size);
extern void bitvec_destroy(bitvec_p bv);
extern int bitvec_get(bitvec_p bv, int index);
extern int bitvec_set(bitvec_p bv, int index, int val);
extern void bitvec_clear(bitvec_p, int val);
extern int bitvec_find_first(bitvec_p bv, int val);
