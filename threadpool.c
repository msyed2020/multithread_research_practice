#include "list.c"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

typedef struct thread_elem {
    struct list_elem elem;
    pthread_t thread;
} thread_elem_t;

typedef struct thread_pool {
    struct list tasks;
    pthread_mutex_t mutex;
} thread_pool_t;

void threadpool_init(thread_pool_t *threadpool) {
    list_init(&threadpool->tasks);
    pthread_mutex_init(&threadpool->mutex, NULL);
}

void threadpool_thread_insert(thread_pool_t *threadpool, pthread_t thread) {
    pthread_mutex_lock(&threadpool->mutex);

    thread_elem_t *threadItem = malloc(sizeof(thread_elem_t));

    if (threadItem == NULL) {
        pthread_mutex_unlock(&threadpool->mutex);
        return;
    }

    threadItem->thread = thread;
    list_push_back(&threadpool->tasks, &threadItem->elem);

    pthread_mutex_unlock(&threadpool->mutex);
}

pthread_t threadpool_get_thread(thread_pool_t *threadpool) {
    
    pthread_mutex_lock(&threadpool->mutex);
    if (list_empty(&threadpool->tasks)) {
        pthread_mutex_unlock(&threadpool->mutex);
        return (pthread_t) NULL;
    }
    struct list_elem* element = list_pop_front(&threadpool->tasks);
    thread_elem_t *threadItem = list_entry(element, thread_elem_t, elem); // not to be confused with elem, thread_elem_t's object, and element, the elem
    // we are casting back
    pthread_t thread = threadItem->thread;
    free(threadItem);

    pthread_mutex_unlock(&threadpool->mutex);
    return thread;
}

int main() {

    thread_pool_t threadpool;

    threadpool_init(&threadpool);
    
    return 0;
}



