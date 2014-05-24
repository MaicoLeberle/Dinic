#ifndef _LIST_H
#define _LIST_H

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct _list_t *list_t;
typedef struct _member_t *member_t;

list_t list_create(void);
list_t list_destroy(list_t list, void *(*function_destroy)(void*));
list_t list_destroy_keep_members(list_t list);
member_t member_create(void *node);
member_t member_destroy(member_t member, void *function_destroy(void*));
list_t list_add(list_t list, void *member);
list_t list_direct_add(list_t list, member_t member);
void *list_search(list_t list, void *item, bool function_compare(void*, void*));
unsigned long list_size(list_t list);
bool list_empty(list_t list);
void *get_content(member_t member);
member_t list_get_first(list_t list);
member_t list_get_last(list_t list);
member_t list_next(member_t member);
member_t list_previous(member_t member);
member_t remove_last(list_t list, void *function_destroy(void*));
member_t remove_last_keep_content(list_t list);
/*	member_t remove_first(member_t member, void *function_destroy(void*)); */

#endif
