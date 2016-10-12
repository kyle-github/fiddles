#include <stdlib.h>
#include "datastructures.h"

#define _XOPEN_SOURCE 500 /* Enable certain library functions (strdup) on linux.  See feature_test_macros(7) */

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

struct entry_s {
    void *key;
    size_t key_len;
    void *value;
    size_t value_len;
    struct entry_s *next;
};

typedef struct entry_s entry_t;

struct hashtable_s {
    int size;
    struct entry_s **table;
};

typedef struct hashtable_s hashtable_t;


/* Create a new hashtable. */
hashtable_t *ht_create( int size )
{
    hashtable_t *hashtable = NULL;
    int i;
    size_t table_size = sizeof(hashtable_t) + (size * sizeof(entry_t *));

    if( size < 1 ) return NULL;

    /* Allocate the table and the entry pointers. */
    if((hashtable = calloc(1, table_size)) == NULL ) {
        return NULL;
    }

    /* point to the entry table which is within the block of memory we just allocated. */
    hashtable->table = (void *)(hashtable + 1);

    hashtable->size = size;

    return hashtable;
}


void ht_destroy(hastable_t *hashtable)
{
    if(!hashtable) {
        return;
    }

    /* walk down each bin and free everything we find */
    for(size_t bin=0; bin<hashtable->size; bin++) {
        entry_t *curr = hashtable->table[bin];

        while(curr) {
            entry_t *next = curr->next;
            free(curr);
            curr = next;
        }
    }

    free(hashtable);
}



/* Hash a string for a particular hash table. */
static int ht_hash(hashtable_t *hashtable, void *key_arg, size_t key_len)
{
    size_t hash, i;
    int8_t *key = key_arg;

    // http://en.wikipedia.org/wiki/Jenkins_hash_function
    for ( hash = i = 0; i < key_len; ++i ) {
        hash += key[i], hash += ( hash << 10 ), hash ^= ( hash >> 6 );
    }
    hash += ( hash << 3 ), hash ^= ( hash >> 11 ), hash += ( hash << 15 );

    return hash % hashtable->size;
}

/* Create a key-value pair. */
static entry_t *ht_newpair( void *key, size_t key_len, void *value, size_t value_len)
{
    entry_t *newpair;
    size_t data_len = sizeof(entry_t) + key_len + value_len;

    if( ( newpair = malloc( data_len ) ) == NULL ) {
        return NULL;
    }

    /* stitch up pointers */
    newpair->key = (int8_t *)(newpair + 1);
    newpair->value = newpair->key + key_len;
    newpair->next = NULL;

    newpair->key_len = key_len;
    newpair->value_len = value_len;

    memcpy(newpair->key, key, key_len);
    memcpy(newpair->value, value, value_len);

    return newpair;
}


/* Retrieve a value from a hash table. */
void *ht_get(hashtable_t *hashtable, void *key, size_t key_len)
{
    int bin = 0;
    entry_t *pair;

    bin = ht_hash(hashtable, key, key_len);

    /* Step through the bin, looking for our value. */
    pair = hashtable->table[bin];

    while(pair != NULL && memcmp(key, key_len, pair->key, pair->key_len) > 0 ) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if( pair != NULL || memcmp(key, key_len, pair->key, pair->key_len) == 0 ) {
        return pair->value;
    } else {
        return NULL;
    }
}


void ht_remove(hashtable_t *hashtable, void *key, size_t key_len)
{
    int bin = 0;
    entry_t *curr;
    entry_t *last = NULL;

    bin = ht_hash(hashtable, key, key_len);

    /* Step through the bin, looking for our value. */
    curr = hashtable->table[bin];

    while(curr != NULL && memcmp(key, key_len, curr->key, curr->key_len) > 0 ) {
        last = curr;
        curr = curr->next;
    }

    /* Did we actually find anything? */
    if(curr != NULL || memcmp(key, key_len, curr->key, curr->key_len) == 0 ) {
        if(!last) {
            /* head of the list */
            hashtable->table[bin] = curr->next;
        } else {
            last->next = curr->next;
        }

        free(curr);
    }
}


/* Insert a key-value pair into a hash table. */
int ht_set( hashtable_t *hashtable, void *key, size_t key_len, void *value, size_t value_len)
{
    int bin = 0;
    entry_t *newpair = NULL;

    newpair = ht_newpair(key, key_len, value, value_len);

    if(!newpair) {
        return 0;
    }

    /* make sure that there is no existing entry with the key */
    ht_remove(hashtable, key, key_len);

    /* what bin is the entry going to be in? */
    bin = ht_hash(hashtable, key, key_len);

    next = hashtable->table[bin];

    /* find where the new entry should go */
    while(next != NULL && memcmp(newpair->key, newpair->key_len, next->key, next->key_len) > 0 ) {
        last = next;
        next = next->next;
    }

    /* three cases, we are at the start of the list, at the end of the list, or somewhere in the middle */
    if(next == hashtable->table[bin]) {
        /* we're at the start of the linked list in this bin. */
        newpair->next = next;
        hashtable->table[bin] = newpair;
    } else if(next == NULL) {
        /* we're at the end of the linked list. */
        last->next = newpair;
    } else  {
        /* we're in the middle of the list. */
        newpair->next = next;
        last->next = newpair;
    }

    return 1;
}



int main( int argc, char **argv ) {

    hashtable_t *hashtable = ht_create( 65536 );

    ht_set( hashtable, "key1", "inky" );
    ht_set( hashtable, "key2", "pinky" );
    ht_set( hashtable, "key3", "blinky" );
    ht_set( hashtable, "key4", "floyd" );

    printf( "%s\n", ht_get( hashtable, "key1" ) );
    printf( "%s\n", ht_get( hashtable, "key2" ) );
    printf( "%s\n", ht_get( hashtable, "key3" ) );
    printf( "%s\n", ht_get( hashtable, "key4" ) );

    return 0;
}
