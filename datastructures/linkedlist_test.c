#include <stdio.h>

#include "linkedlist.h"

struct test_data {
    int i;
    list_node_t list;
};

int main()
{
    list_node_p list = NULL;
    struct test_data nodes[3] = { {0,NULL}, {1,NULL}, {2,NULL}};

    list_insert(&list,&(nodes[2].list));
    list_insert(&list,&(nodes[0].list));
    list_insert(&list,&(nodes[1].list));

    for(list_node_p node_p = list; node_p; node_p = node_p->next) {
        struct test_data *data_p = container_of(node_p, struct test_data, list);
        printf("Node data %d\n",data_p->i);
    }

    return 0;
}
