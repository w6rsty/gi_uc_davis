#ifndef DS_H
#define DS_H

#include <stdint.h>

/* Linked list */

typedef struct linked_list_node {
    void *data;
    struct linked_list_node *next;
} linked_list_node_t;

typedef struct {
    linked_list_node_t *head;
    linked_list_node_t *tail;
    uint32_t size; 
} linked_list_t;

void init_linked_list(linked_list_t *list);
void clear_linked_list(linked_list_t *list);
void insert_back_linked_list(linked_list_t *list, void *data);
void remove_back_linked_list(linked_list_t *list);
void* get_linked_list(const linked_list_t *list, uint32_t index);
void* get_back_linked_list(const linked_list_t *list);

#endif
