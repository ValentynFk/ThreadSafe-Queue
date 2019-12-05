/* Written by Valentyn Faychuk at SoftServe on 04.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>

#include "threadsafe-queue.h"

void * thread_draining_routine(void * args) {
    /* Routine, that pops values from the queue */
    while (1) {
        printf("%d\n", pop_thread_safe_queue(( thread_safe_queue_t*)args));
    }
    return NULL;
}

void * thread_stuffing_routine(void * args) {
    /* Routine, that push values to the queue */
    int i = 0;
    while (1) {
        push_thread_safe_queue((thread_safe_queue_t*)args, i++);
    }
    return NULL;
}

int main (int argc, char ** argv)
{
    thread_safe_queue_t * my_queue = NULL;
    init_thread_safe_queue(&my_queue, 500);
    printf("Queue initialized\n");
    
    pthread_t tid[20];
    for (int i = 0; i < 10; ++i) {
        pthread_create(&(tid[i]), NULL, &thread_stuffing_routine, (void *)my_queue);
    }
    for (int i = 0; i < 10; ++i) {
        pthread_create(&(tid[i+10]), NULL, &thread_draining_routine, (void *)my_queue);
    }
   
    for (long i = 0; i < 1000000000; ++i);
    //destroy_thread_safe_queue(&my_queue);
    return 0;
}
