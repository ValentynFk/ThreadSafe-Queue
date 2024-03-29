/* Written by Valentyn Faychuk at SoftServe on 05.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#ifndef THREADSAFE_QUEUE
#define THREADSAFE_QUEUE

#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define LOGERROR(err) printf("error: \"%s\"\n", (err))
#ifndef QUEUE_TYPE
#define QUEUE_TYPE int
#endif

typedef struct thread_safe_queue_t {
    size_t size;
    sem_t write_mutex;
    QUEUE_TYPE * begin_ptr;
    QUEUE_TYPE * write_ptr;
    QUEUE_TYPE * read_ptr;
    bool is_full;
} thread_safe_queue_t;

/* Initialize queue */
int init_thread_safe_queue(thread_safe_queue_t ** queue, size_t max_size);
/* Destroy queue (queue must be first initialized */
int destroy_thread_safe_queue(thread_safe_queue_t ** queue);
/* Push to the back of queue */
int push_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE val);
/* Pop value from the fornt of queue */
int pop_thread_safe_queue(thread_safe_queue_t * queue, QUEUE_TYPE *val);

#endif
