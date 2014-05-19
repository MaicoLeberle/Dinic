#ifndef _LIST_H
#define _LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _list_t *list_t;
typedef struct _member_t *member_t;

list_t list_create(void);
list_t list_destroy(list_t list, void *(*function_destroy)(void*));
member_t member_create(void *node);
member_t member_destroy(member_t member, void *function_destroy(void*));
list_t list_add(list_t list, void *member);
void *list_search(list_t list, void *item, bool function_compare(void*, void*));
unsigned long list_size(list_t list);
bool list_empty(list_t list);
void *get_content(member_t member);
#endif
