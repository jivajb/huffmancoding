// JIVAJ BRAR pq.c
#include "pq.h"

#include "node.h"

#include <stdio.h>

typedef struct ListElement ListElement;

struct ListElement {
    Node *tree;
    ListElement *next;
};

struct PriorityQueue {
    ListElement *list;
};

PriorityQueue *pq_create(void) {
    PriorityQueue *pq = calloc(1, sizeof(PriorityQueue));
    if (pq == NULL) {
        exit(1);
    }
    return pq;
}
void pq_free(PriorityQueue **q) {
    if (*q != NULL) {
        free(*q);
        *q = NULL;
    }
}
bool pq_is_empty(PriorityQueue *q) {
    if (q->list == NULL) {
        return true;
    } else {
        return false;
    }
}
bool pq_size_is_1(PriorityQueue *q) {
    if (q->list == NULL) {
        return false;
    } else if (q->list->next == NULL) {
        return true;
    } else {
        return false;
    }
}
bool pq_less_than(ListElement *e1, ListElement *e2) {
    if (e1->tree->weight < e2->tree->weight) {
        return true;
    } else if (e1->tree->weight == e2->tree->weight) {
        if (e1->tree->symbol < e2->tree->symbol) {
            return true;
        } else {
            return false;
        }

    } else {
        return false;
    }
}
void enqueue(PriorityQueue *q, Node *tree) {
    ListElement *new_element = calloc(1, sizeof(ListElement));
    new_element->tree = tree;
    if (pq_is_empty(q) == true) {
        q->list = new_element;
    } else if (pq_less_than(new_element, q->list) == true) {
        new_element->next = q->list;
        q->list = new_element;
    } else {
        ListElement *temp = q->list;
        while (temp->next != NULL && !pq_less_than(new_element, temp->next)) {
            temp = temp->next;
        }
        new_element->next = temp->next;
        temp->next = new_element;
    }
}
Node *dequeue(PriorityQueue *q) {
    if (pq_is_empty(q) == true) {
        exit(1); // error
    } else {
        ListElement *removed = q->list;
        Node *removed_node = removed->tree;
        q->list = q->list->next;
        free(removed);
        return removed_node;
    }
}
void pq_print(PriorityQueue *q) {
    assert(q != NULL);
    ListElement *e = q->list;
    int position = 1;
    while (e != NULL) {
        if (position++ == 1) {
            printf("=============================================\n");
        } else {
            printf("---------------------------------------------\n");
        }
        node_print_tree(e->tree);
        e = e->next;
    }
    printf("=============================================\n");
}
