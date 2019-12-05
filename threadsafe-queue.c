/* Written by Valentyn Faychuk at SoftServe on 05.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#include <string.h>

#include "threadsafe-queue.h"

int init_thread_safe_queue(thread_safe_queue_t ** queue, size_t size) {
    if (queue == NULL) {
        LOGERROR("init failed, queue is NULL");
        return -1;
    }
    if (*queue != NULL) {
        LOGERROR("init failed, queue was already initialized");
        return -1;
    }
    if (size == 0) {
        LOGERROR("init failed, size can\'t be 0");
        return -1;
    }
    *queue = malloc(sizeof(thread_safe_queue_t));
    sem_init(&((*queue) -> write_mutex), 0, 1);
    (*queue) -> size = size;
    (*queue) -> is_full = false;
    (*queue) -> begin_ptr = malloc(sizeof(QUEUE_TYPE) * size);
    (*queue) -> write_ptr = (*queue) -> read_ptr = (*queue) -> begin_ptr;
    return 0;
}

int destroy_thread_safe_queue(thread_safe_queue_t ** queue) {
    if (queue == NULL || *queue == NULL) {
        LOGERROR("failed, queue is NULL");
        return -1;
    }
    sem_destroy(&((*queue) -> write_mutex));
    free((*queue) -> begin_ptr);
    free(*queue);
    *queue = NULL;
    return 0;
}

int push_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE val) {
    if (queue == NULL) {
        LOGERROR("push failed, queue is NULL");
        return -1;
    }
    sem_wait(&(queue -> write_mutex));
    if (queue -> is_full) {
        sem_post(&(queue -> write_mutex));
        //LOGERROR("push failed, queue is full");
        return -1;
    }
    *(queue -> write_ptr) = val;
    if (queue -> write_ptr == queue -> begin_ptr + (queue -> size - 1)) {
        queue -> write_ptr = queue -> begin_ptr;
    } else {
        ++(queue -> write_ptr);
    }
    if (queue -> write_ptr == queue -> read_ptr) {
        queue -> is_full = true;
    }
    sem_post(&(queue -> write_mutex));
    return 0;
}

int pop_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE * ret_val) {
    if (queue == NULL) {
        LOGERROR("pop failed, queue is NULL");
        return -1;
    }
    sem_wait(&(queue -> write_mutex));
    if (!queue -> is_full && queue -> write_ptr == queue -> read_ptr) {
        sem_post(&(queue -> write_mutex));
        //LOGERROR("pop failed, queue is empty");
        return -1;
    }
    
    *ret_val = *(queue -> read_ptr);
    if (queue -> read_ptr == queue -> begin_ptr + (queue -> size - 1)) {
        queue -> read_ptr = queue -> begin_ptr;
    } else {
        ++(queue -> read_ptr);
    }
    queue -> is_full = false;
    
    sem_post(&(queue -> write_mutex));
    return 0;
}
