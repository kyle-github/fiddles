#include <stddef.h>

struct list_node_t {
    struct list_node_t *next;
};

typedef struct list_node_t *list_node_p;
typedef struct list_node_t list_node_t;

int list_insert(list_node_p *list, list_node_p node)
{
    node->next = *list;
    *list = node;

    return 0;
}


int list_remove(list_node_p *list, list_node_p node)
{
    list_node_p *indirect_next = list;

    while(*indirect_next && *indirect_next != node) {
        indirect_next = &(*indirect_next)->next;
    }

    if(*indirect_next == node) {
        *indirect_next = node->next;
    }

    return 0;
}

#define container_of(ptr, type, member) ((type *)((char *)(ptr) - offsetof(type, member)))
