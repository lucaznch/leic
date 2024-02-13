#ifndef PARALLEL_PROCESSING_UTILS_H
#define PARALLEL_PROCESSING_UTILS_H

#include <pthread.h>
#include <semaphore.h>

/// Data type used to store the shared data between threads for synchronization purposes.
typedef struct {
    pthread_mutex_t output_write_mutex; /// Mutex to safely write to the output file descriptor.

    pthread_mutex_t events_general_mutex; /// Mutex to safely update the general events.

    pthread_mutex_t barrier_mod_mutex; /// Mutex to safely update the barrier state, etc. 
    sem_t barrier_sem_2; /// Component of the barrier implementation.
    sem_t barrier_sem_1; /// Component of the barrier implementation.
    int blocked_threads_counter; /// Number of threads blocked in the barrier.
} thread_shared_data;


/// Data type used to store the arguments passed to each thread.
typedef struct {
    int number_of_threads; /// Total number of threads.
    int thread_id; /// Thread id.

    int input_fd; /// Input file descriptor.
    int output_fd; /// Output file descriptor.
    
    thread_shared_data* shared_data; /// Shared data between threads for synchronization purposes.
} thread_args;

#endif // PARALLEL_PROCESSING_UTILS_H