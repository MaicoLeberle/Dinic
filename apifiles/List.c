#include "List.h"

/*                Next
list :   (First)---------->(object)----->(Last)----->Null
                 <----------
                  Previous
*/
struct _list_t {
    member_t first;
    member_t last;
    unsigned long length;
};

struct _member_t {
    void *member;
    member_t next;
    member_t previous;
};

list_t list_create(void) {
    list_t list = calloc(1, sizeof(struct _list_t));
    assert(list != NULL);
    
    list->first = NULL;
    list->last = NULL;
    list->length = 0;

    return list;
}

list_t list_destroy(list_t list, void *function_destroy(void*)) {
    assert(list != NULL);
    
    member_t current = list->first;
    member_t temp = current;
    
    while(current != NULL) {
        temp = temp->next;
        current = member_destroy(current, function_destroy);
        current = temp;
    }
    free(list);
    return NULL;
}

list_t list_destroy_keep_members(list_t list) {
    assert(list);

    member_t current = list->first;
    member_t temp;

    while(current) {
        temp = temp->next;
        current = member_destroy_keep_content(current);
        current = temp;
    }
    free(list);
    return NULL;
}

member_t member_create(void *object) {
    assert(object != NULL);
    
    member_t new_member = NULL;
    new_member = calloc(1, sizeof(struct _member_t));
    assert(new_member != NULL);
    
    new_member->member = object;
    new_member->next = NULL;
    new_member->previous = NULL;
    
    return new_member;
}

member_t member_destroy(member_t member, void *function_destroy(void*)) {
    assert(member != NULL);
    
    if(member->member) {
        function_destroy(member->member);
    }
    free(member);
    
    return NULL;
}

member_t member_destroy_keep_content(member_t member) {
    assert(member);

    free(member);
    return NULL;
}

list_t list_add(list_t list, void *object) {
    assert(list != NULL);
    assert(object != NULL);
    
    member_t new_member = member_create(object);
    
    if(list->first == NULL) {
        list->first = new_member;
        list->last = new_member;
    }
    else {
        new_member->previous = list->last;
        list->last->next = new_member;
        list->last = new_member;
    }
    list->length = list->length + 1;
    
    return list;
}

list_t list_direct_add(list_t list, member_t member) {
    assert(list);
    assert(member);
    
    if(list->first == NULL) {
        list->first = member;
        list->last = member;
    }
    else {
        member->previous = list->last;
        list->last->next = member;
        list->last = member;
    }
    list->length = list->length + 1;
    
    return list;
}


void *get_content(member_t member) {
    assert(member != NULL);
    assert(member->member != NULL);
    
    void *result = member->member;
    assert(result != NULL);
    
    return result;
}

void *list_search(list_t list, void *item, bool function_compare(void*, void*)) {
    assert(list != NULL);
    assert(item != NULL);
    
    member_t temp = list->first;
    void *result = NULL;
    bool found = false;
    
    while(!found && temp != NULL) {
        found = function_compare(temp->member, item);
        if(!found) {
            temp = temp->next;
        }
    }
    if(found) {
        result = temp->member;
    }
    return result;
}

bool list_empty(list_t list) {
    assert(list != NULL);
    
    return (list->length == 0);
}

unsigned long list_size(list_t list) {
    assert(list != NULL);
    
    return list->length;
}

member_t list_get_first(list_t list) {
    assert(list);
    
    return list->first;
}

member_t list_get_last(list_t list) {
    assert(list);

    return list->last;
}

member_t list_next(member_t member) {
    assert(member);
    
    return member->next;
}

member_t list_previous(member_t member) {
    assert(member);

    return member->previous;
}

member_t remove_last(list_t list, void *function_destroy(void*)) {
    assert(list);

    if((list->last)->member) {
        function_destroy((list->last)->member);
    }
    free(list->last);
    list->last = NULL;

    return NULL;
}

member_t remove_last_keep_content(list_t list) {
    assert(list);

    if(list->last) {
        free(list->last);
        list->last = NULL;
    }

    return NULL;
}