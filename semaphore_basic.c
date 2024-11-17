#include <stdio.h> // standard library
#include <stdlib.h> // standard library
#include <pthread.h> // POSIX Threads
#include <semaphore.h> // Semaphore
#include <errno.h> // backup library for error checking pthreads
#include <unistd.h> // for pause and sleep compatibility (pause is notably omitted, as pthread_join() fulfills its purpose)

#define PERMIT_NUMBER 5 // the number of threads I allow the semaphore to permit into the Critical Section. Can change at will
#define NUM_THREADS 10 // the number of threads in the program. Can change at will
pthread_t threads[NUM_THREADS]; // array of threads

sem_t sem; // semaphore

static void* thread_callback(void *arg) { // where thread execution occurs
    char* threadName = (char*) arg;

    printf("%s is about to enter the Critical Section\n", threadName);

    // Critical Section

    sem_wait(&sem); // permitting threads to proceed, and blocking once the PERMIT_NUMBER has been reached

    printf("%s has entered the Critical Section\n", threadName);

    for (int i = 0; i < NUM_THREADS; i++) {
        printf("%s is executing in a Critical Section\n", threadName);
        sleep(1);
    }

    sem_post(&sem); // Exiting CS and allowing remaining threads to proceed

    // Critical Section

    printf("%s has exited Critical Section\n", threadName);
}

int main(int argc, char **argv) {

    sem_init(&sem, 0, PERMIT_NUMBER); // Second parameter is 0 to allow semaphore to deal with threads, as opposed to 1 or other value to signify
    // dealing with processes
    
    for (int i = 0; i < NUM_THREADS; i++) {
        char* arg = malloc(20 * sizeof(char)); // dynamically allocated so that the lifetime of the string does not die within the main function
        sprintf(arg, "Thread %d", i); // converting output to string for printing/output purposes
        int rc = pthread_create(&threads[i], NULL, thread_callback, (void *) arg);
        if (rc != 0) {
            perror("No thread created");
            exit(0);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) { // joining to allow the main thread to pause and diverge, and allowing the other threads to execute/join in the end
        pthread_join(threads[i], NULL);
    }

    sem_destroy(&sem); // destroying semaphore for memory purposes

    return 0;
}