// Created by Valentyn Faychuk on 04.12.2019
// My email:
//     faitchouk.valentyn@gmail.com
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LOGERROR(err) printf("error: \"%s\"\n", (err))
#define QUEUE_TYPE int

struct thread_safe_queue_t {
    QUEUE_TYPE * collection;
    int alloc_size;
    int real_size;
};

pthread_mutex_t g_queue_rw_lock;

void init_thread_safe_queue(struct thread_safe_queue_t ** queue);
void destroy_thread_safe_queue(struct thread_safe_queue_t ** queue);
// Push to the back of queue
void push_thread_safe_queue(struct thread_safe_queue_t * queue, int val);
// Pop from the front of queue
int pop_thread_safe_queue(struct thread_safe_queue_t * queue);

int main (int argc, char ** argv)
{
    struct thread_safe_queue_t * my_queue = NULL;
    init_thread_safe_queue(&my_queue);
    push_thread_safe_queue(my_queue, 10);
    push_thread_safe_queue(my_queue, 20);
    printf("first value is: %d\n", pop_thread_safe_queue(my_queue));
    printf("second value is: %d\n", pop_thread_safe_queue(my_queue));
    destroy_thread_safe_queue(&my_queue);
    return 0;
}

static void fetch_queue_allocation(struct thread_safe_queue_t * queue) {
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

void init_thread_safe_queue(struct thread_safe_queue_t ** queue) {
    if (queue == NULL || *queue != NULL) {
        LOGERROR("initialization failure");
        return;
    }
    if (pthread_mutex_init(&g_queue_rw_lock, NULL) != 0) {
        LOGERROR("mutex initialization failure");
        return;
    }
    *queue = malloc(sizeof(struct thread_safe_queue_t));
    (*queue) -> alloc_size = 0;
    (*queue) -> real_size = 0;
    fetch_queue_allocation(*queue);
}

void destroy_thread_safe_queue(struct thread_safe_queue_t ** queue) {
    if (queue == NULL || *queue == NULL) {
        LOGERROR("destruction failure");
        return;
    }
    pthread_mutex_destroy(&g_queue_rw_lock);
    free((*queue) -> collection);
    free(*queue);
    *queue = NULL;
}

void push_thread_safe_queue(struct thread_safe_queue_t * queue, int val) {
    if (queue == NULL) {
        LOGERROR("push failure");
        return;
    }
    pthread_mutex_lock(&g_queue_rw_lock);
    fetch_queue_allocation(queue);
    (queue -> collection)[queue -> real_size] = val;
    ++(queue -> real_size);
    pthread_mutex_unlock(&g_queue_rw_lock);
}

int pop_thread_safe_queue(struct thread_safe_queue_t * queue) {
    if (queue == NULL) {
        LOGERROR("pop failure");
        return 0;
    }
    pthread_mutex_lock(&g_queue_rw_lock);
    fetch_queue_allocation(queue);
    QUEUE_TYPE val;
    val = (queue -> collection)[0];
    for (int i = 1; i < (queue -> real_size); ++i) {
        (queue -> collection)[i-1] = (queue->collection)[i];
    }
    --(queue -> real_size);
    pthread_mutex_unlock(&g_queue_rw_lock);
    return val;
}
