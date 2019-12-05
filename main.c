/* Written by Valentyn Faychuk at SoftServe on 04.12.2019
 * email:
 * 		faitchouk.valentyn@gmail.com
 */
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

/* You can redefine type of the queue.
 * Just type before including queue:
 * #define QUEUE_TYPE char
 */

#include "threadsafe-queue.h"

bool is_program_finished = false;
pthread_mutex_t g_count_mutex;
int g_counter = 0;

void * thread_draining_routine(void * args) {
    int tid = pthread_self();
    QUEUE_TYPE val;
    /* Routine, that pops values from the queue */
    while (!is_program_finished) {
        if (pop_thread_safe_queue((thread_safe_queue_t*)args, &val) == 0) {
            printf("thread %u: %d\n", tid, val);
        }
    }
    pthread_exit(NULL);
}

void * thread_stuffing_routine(void * args) {
    /* Routine, that pushes values to the queue */
    while (!is_program_finished) {
        if (push_thread_safe_queue((thread_safe_queue_t*)args, g_counter) == 0) {
            pthread_mutex_lock(&g_count_mutex);
            ++g_counter;
            pthread_mutex_unlock(&g_count_mutex);
        }
    }
    pthread_exit(NULL);
}

int main (int argc, char ** argv)
{
    thread_safe_queue_t * my_queue = NULL;
    init_thread_safe_queue(&my_queue, 500);
    printf("Queue initialized\n");
    
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    pthread_mutex_init(&g_count_mutex, NULL);
    pthread_t tid[20];
    for (int i = 0; i < 10; ++i) {
        pthread_create(&(tid[i]), &attr, &thread_stuffing_routine, (void *)my_queue);
    }
    for (int i = 0; i < 10; ++i) {
        pthread_create(&(tid[i+10]), &attr, &thread_draining_routine, (void *)my_queue);
    }
   
    sleep(5);

    is_program_finished = true;
    for (int i = 0; i < 20; ++i) {
        pthread_join(tid[i], NULL);
    }
    destroy_thread_safe_queue(&my_queue);
    pthread_mutex_destroy(&g_count_mutex);
    pthread_attr_destroy(&attr);
    return 0;
}
