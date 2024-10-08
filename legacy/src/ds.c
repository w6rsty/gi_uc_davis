#include "ds.h"

#include <stdlib.h>

void init_linked_list(linked_list_t *list) {
    list->head = NULL;
    list->tail = NULL;
    list->size = 0;
}

void clear_linked_list(linked_list_t *list) {
    linked_list_node_t *node = list->head;
    while (node != NULL) {
        list->head = list->head->next;
        free(node);
        node = list->head;
    }
    list->tail = NULL;
    list->size = 0;
}

void insert_back_linked_list(linked_list_t *list, void *data) {
    linked_list_node_t *node = malloc(sizeof(linked_list_node_t));
    node->data = data;
    node->next = NULL;
    if (list->head == NULL) {
        list->head = node; 
        list->tail = node;
    } else {
        list->tail->next = node;
        list->tail = node;
    }
    ++list->size;
}

void remove_back_linked_list(linked_list_t *list) {
    linked_list_node_t *node = list->tail;
    if (node != NULL) {
        free(node);
        node = list->head;
        while (node->next != NULL) {
            node = node->next;
        }
        list->tail = node;
        --list->size;
    }
}

void* get_linked_list(const linked_list_t *list, uint32_t index) {
    if (index + 1 > list->size) {
        return NULL;
    }
    linked_list_node_t *node = list->head;
    while (index != 0) {
        node = node->next;
        --index;
    }
    return node->data;
}

void* get_back_linked_list(const linked_list_t *list) {
    if (list->tail == NULL) {
        return NULL;
    } else {
        return list->tail->data;
    }
}
