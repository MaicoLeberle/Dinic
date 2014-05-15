#ifndef _QUEUE_H
#define _QUEUE_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _queue_t *queue_t;
typedef struct _member_t *member_t;

queue_t queue_create(void);
queue_t queue_destroy(queue_t queue, void *(*function_destroy)(void*));
member_t member_create(void *node);
member_t member_destroy(member_t member, void *function_destroy(void*));
queue_t enqueue(queue_t queue, void *member);
void *pop(queue_t queue);
unsigned long queue_size(queue_t queue);
bool queue_empty(queue_t queue);
void *get_content(member_t member);
#endif
