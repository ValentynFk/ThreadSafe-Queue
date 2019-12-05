/* Written by Valentyn Faychuk at SoftServe on 05.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "threadsafe-queue.h"

static void fetch_queue_allocation(thread_safe_queue_t * queue) {
    if (queue -> alloc_size == 0) {
        queue -> collection = malloc(sizeof(QUEUE_TYPE));
        ++(queue -> alloc_size);
        return;
    }
    if (queue -> alloc_size <= queue -> real_size) {
        QUEUE_TYPE * saved_collection = queue -> collection;
        queue -> alloc_size *= 2;
        queue -> collection = malloc(sizeof(QUEUE_TYPE) * queue -> alloc_size);
        memcpy(queue -> collection, saved_collection, sizeof(QUEUE_TYPE) * queue -> real_size);
        free(saved_collection);
        return;
    }
    if (queue -> real_size != 0 && queue -> alloc_size > queue -> real_size * 2) {
        QUEUE_TYPE * saved_collection = queue -> collection;
        queue -> alloc_size /= 2;
        queue -> collection = malloc(sizeof(QUEUE_TYPE) * queue -> alloc_size);
        memcpy(queue -> collection, saved_collection, sizeof(QUEUE_TYPE) * queue -> real_size);
        free(saved_collection);
        return;
    }
}

void init_thread_safe_queue(thread_safe_queue_t ** queue, size_t max_size) {
    if (queue == NULL || *queue != NULL) {
        LOGERROR("queue initialization failed");
        return;
    }
    *queue = malloc(sizeof(thread_safe_queue_t));
    if (pthread_mutex_init(&((*queue) -> rw_lock), NULL) != 0) {
        LOGERROR("mutex initialization failed");
        return;
    }
    (*queue) -> max_size = max_size;
    (*queue) -> alloc_size = 0;
    (*queue) -> real_size = 0;
    fetch_queue_allocation(*queue);
}

void destroy_thread_safe_queue(thread_safe_queue_t ** queue) {
    if (queue == NULL || *queue == NULL) {
        LOGERROR("failed, queue is NULL");
        return;
    }
    pthread_mutex_destroy(&((*queue) -> rw_lock));
    free((*queue) -> collection);
    free(*queue);
    *queue = NULL;
}

int push_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE val) {
    if (queue == NULL) {
        LOGERROR("push failed, queue is NULL");
        return -1;
    }
    pthread_mutex_lock(&(queue -> rw_lock));
    if (queue -> real_size >= queue -> max_size) {
        return -1;
    }
    fetch_queue_allocation(queue);
    (queue -> collection)[queue -> real_size] = val;
    ++(queue -> real_size);
    pthread_mutex_unlock(&(queue -> rw_lock));
    return 0;
}

int pop_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE * p_val) {
    if (queue == NULL) {
        LOGERROR("pop failed, queue is NULL");
        return -1;
    }
    pthread_mutex_lock(&(queue -> rw_lock));
    if (queue -> real_size <= 0) {
        return -1;
    }
    fetch_queue_allocation(queue);
    *p_val = (queue -> collection)[0];
    for (size_t i = 1; i < (queue -> real_size); ++i) {
        (queue -> collection)[i-1] = (queue->collection)[i];
    }
    --(queue -> real_size);
    pthread_mutex_unlock(&(queue -> rw_lock));
    return 0;
}
