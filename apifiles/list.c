#include "queue.h"

/*                Next
Queue :   (First)---------->(object)----->(Last)----->Null
                 <----------
                  Previous
*/
struct _queue_t {
    member_t first;
    member_t last;
    unsigned long length;
};

struct _member_t {
    void *member;
    member_t next;
    member_t previous;
};

queue_t queue_create(void) {
    queue_t queue = calloc(1, sizeof(struct _queue_t));
    assert(queue != NULL);
    
    queue->first = NULL;
    queue->last = NULL;
    queue->length = 0;

    return queue;
}

queue_t queue_destroy(queue_t queue, void *function_destroy(void*)) {
    assert(queue != NULL);
    
    member_t current = queue->first;
    member_t temp = current;
    
    while(current != NULL) {
        temp = temp->next;
        current = member_destroy(current, function_destroy);
        
        current = temp;
    }
    free(queue);
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
    
    function_destroy(member->member);
    free(member);
    
    return NULL;
}

queue_t enqueue(queue_t queue, void *object) {
    assert(queue != NULL);
    assert(object != NULL);
    
    member_t new_member = member_create(object);
    
    if(queue->first == NULL) {
        queue->first = new_member;
        queue->last = new_member;
    }
    else {
        new_member->previous = queue->last;
        queue->last->next = new_member;
        queue->last = new_member;
    }
    queue->length = queue->length + 1;
    
    return queue;
}

void *pop(queue_t queue) {
    assert(queue != NULL);
    assert(queue->length > 0);
    
    void *result = NULL;
    
    member_t tmp = queue->first;
    
    queue->first = queue->first->next;
    if(queue->first == NULL) {
        queue->last = NULL;
    }
    else {
        queue->first->previous = NULL;
    }
    queue->length = queue->length - 1;
    result = get_content(tmp);
    free(tmp);
    return result;
}

void *get_content(member_t member) {
    assert(member != NULL);
    assert(member->member != NULL);
    
    void *result = member->member;
    assert(result != NULL);
    
    return result;
}

void *queue_search(queue_t queue, void *item, bool function_compare(void*, void*)) {
    assert(queue != NULL);
    assert(item != NULL);
    
    void *temp = queue->first;
    bool found = false;
    
    while(!found && temp != NULL) {
        found = function_compare(get_content(temp), item);
        temp = temp->next;
    }
    return temp;
}

bool queue_empty(queue_t queue) {
    assert(queue != NULL);
    
    return (queue->length == 0);
}

unsigned long queue_size(queue_t queue) {
    assert(queue != NULL);
    
    return queue->length;
}
