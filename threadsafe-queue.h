/* Written by Valentyn Faychuk at SoftServe on 05.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#ifndef THREADSAFE_QUEUE
#define THREADSAFE_QUEUE

#include <pthread.h> /* pthread_mutex_t */
#include <stdlib.h>  /* size_t */
#include <stdio.h>   /* printf() */

#define LOGERROR(err) printf("error: \"%s\"\n", (err))
#define QUEUE_TYPE int

typedef struct thread_safe_queue_t {
    pthread_mutex_t rw_lock;
    QUEUE_TYPE * collection;
    size_t alloc_size;
    size_t real_size;
    size_t max_size;
} thread_safe_queue_t;

/* Initialize queue */
void init_thread_safe_queue(thread_safe_queue_t ** queue, size_t max_size);
/* Destroy queue (queue must be first initialized */
void destroy_thread_safe_queue(thread_safe_queue_t ** queue);
/* Push to the back of queue */
void push_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE val);
QUEUE_TYPE pop_thread_safe_queue(thread_safe_queue_t * queue);

#endif
