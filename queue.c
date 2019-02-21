/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = (queue_t *) malloc(sizeof(queue_t));
    /* What if malloc returned NULL? */
    if (q == NULL)
        return NULL;

    q->head = NULL;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* How about freeing the list elements and the strings? */
    /* Free queue structure */
    if (q == NULL)
        return;

    while (q->head != NULL) {
        list_ele_t *tmp = q->head;
        q->head = q->head->next;

        free(tmp->value);
        free(tmp);
    }

    free(q);
    q = NULL;
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    if (q == NULL)
        goto ERR;

    list_ele_t *newh;
    /* What should you do if the q is NULL? */
    newh = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newh == NULL)
        goto ERR;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    char *str = (char *) malloc(strlen(s) + 1);
    if (str == NULL)
        goto ERR_FREE_LIST;

    strcpy(str, s);

    newh->value = str;
    newh->next = q->head;
    q->head = newh;
    return true;

ERR_FREE_LIST:
    free(newh);
ERR:
    return false;
}


/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    /* You need to write the complete code for this function */
    /* Remember: It should operate in O(1) time */
    if (q == NULL)
        goto ERR;

    list_ele_t *newt = (list_ele_t *) malloc(sizeof(list_ele_t));
    if (newt == NULL)
        goto ERR;

    char *str = (char *) malloc(strlen(s) + 1);
    if (str == NULL)
        goto ERR_FREE_LIST;

    strcpy(str, s);

    newt->value = str;
    newt->next = NULL;

    if (q->head == NULL) {
        q->head = newt;
    } else {
        list_ele_t *head = q->head;

        while (head->next != NULL) {
            head = head->next;
        }
        head->next = newt;
    }
    return true;

ERR_FREE_LIST:
    free(newt);
ERR:
    return false;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    /* You need to fix up this code. */
    if (q == NULL || q->head == NULL)
        return false;

    if (sp != NULL) {
        strncpy(sp, q->head->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }

    list_ele_t *tmp = q->head;
    q->head = q->head->next;

    free(tmp->value);
    free(tmp);
    return true;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    /* You need to write the code for this function */
    /* Remember: It should operate in O(1) time */
    int length = 0;
    list_ele_t *head = q->head;

    while (head != NULL) {
        length++;
        head = head->next;
    }

    return length;
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    /* You need to write the code for this function */
    if (q == NULL || q->head == NULL || q->head->next == NULL)
        return;

    list_ele_t *prev, *curr, *next;

    prev = q->head;
    curr = q->head->next;
    q->head->next = NULL;

    while (curr != NULL) {
        next = curr->next;
        curr->next = prev;
        prev = curr;
        curr = next;
    }
    q->head = prev;
}
