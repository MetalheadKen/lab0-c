#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "natsort/strnatcmp.h"
#include "queue.h"

static list_ele_t *merge_sort(list_ele_t *head);
static list_ele_t *merge(list_ele_t *left, list_ele_t *right);

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = &(q->head);
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    if (!q)
        return;

    while (q->head) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;

        free(tmp->value);
        free(tmp);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (!q)
        goto ERR;

    list_ele_t *newh;
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newh)
        goto ERR;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    int len = strlen(s) + 1;
    char *str = (char *) malloc(len);
    if (!str)
        goto ERR_FREE_LIST;

    memcpy(str, s, len);

    newh->value = str;
    newh->next = q->head;

    if (!q->head) {
        q->tail = &(newh->next);
    }
    q->head = newh;
    q->size++;

    return true;

ERR_FREE_LIST:
    free(newh);
ERR:
    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    if (!q)
        goto ERR;

    list_ele_t *newt = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (!newt)
        goto ERR;

    int len = strlen(s) + 1;
    char *str = (char *) malloc(len);
    if (!str)
        goto ERR_FREE_LIST;

    memcpy(str, s, len);

    newt->value = str;
    newt->next = NULL;

    *(q->tail) = newt;
    q->tail = &(newt->next);
    q->size++;

    return true;

ERR_FREE_LIST:
    free(newt);
ERR:
    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    if (!q || !q->head)
        return false;

    if (sp) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;
    q->tail = (--q->size == 0) ? &(q->head) : q->tail;

    free(tmp->value);
    free(tmp);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q || !q->head) {
        return 0;
    } else {
        return q->size;
    }
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;

    list_ele_t *prev, *curr, *next;

    prev = q->head;
    curr = q->head->next;
    q->head->next = NULL;
    q->tail = &(q->head->next);

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    if (!q || !q->head || !q->head->next)
        return;

    q->head = merge_sort(q->head);

    q->tail = &(q->head);
    while (*(q->tail)) {
        q->tail = &((*q->tail)->next);
    }
}

static list_ele_t *merge_sort(list_ele_t *head)
{
    if (!head || !head->next)
        return head;

    list_ele_t *slow = head;
    list_ele_t *fast = head->next;

    while (fast && fast->next) {
        slow = slow->next;
        fast = fast->next->next;
    }

    list_ele_t *mid = slow->next;
    slow->next = NULL;

    return merge(merge_sort(head), merge_sort(mid));
}

/* Merge two list in ascending order */
static list_ele_t *merge(list_ele_t *left, list_ele_t *right)
{
    if (!left)
        return right;
    if (!right)
        return left;

    list_ele_t *dummy = NULL;
    list_ele_t *curr = NULL;

    if (strnatcmp(left->value, right->value) > 0) {
        dummy = right;
        right = right->next;
    } else {
        dummy = left;
        left = left->next;
    }
    curr = dummy;

    while (left && right) {
        if (strnatcmp(left->value, right->value) > 0) {
            curr->next = right;
            right = right->next;
        } else {
            curr->next = left;
            left = left->next;
        }
        curr = curr->next;
    }

    if (left)
        curr->next = left;
    if (right)
        curr->next = right;

    return dummy;
}
