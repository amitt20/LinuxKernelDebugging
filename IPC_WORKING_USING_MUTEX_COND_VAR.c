#<include <stdio.h>
#include <pthread.h>   //System V standard specific posix thread calls 
#include <unistd.h>

#define NO_THREADS          2
#define INVALID_RETURN_VALUE (-1)

int             count = 0;
pthread_mutex_t lock_var = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  cond_var   = PTHREAD_COND_INITIALIZER;

void
thread_1(void *ptr)
{
    char thread_name[15];

    pthread_setname_np("even_thread");  //Setting the name of thread for ODD counting
    pthread_getname_np(pthread_self(), thread_name, 15);

    while (1) {
        sleep(1);
        while (!((count - 1) % 2)) {
            /* A thread which is entering the wait operation, a thread that must wait will automatically release the mutex and place itself on the waitqueue
        which is associated with the condition variable.when the thread is woken up , it will automatically reacquire the mutex before actually exiting the wait operation. */
            pthread_cond_wait(&cond_var, &lock_var);  //if the cond_var not used, mutex will have to check for the event to release or acquire the lock.
            count++;
            printf("Thread_name = %s count = %d\n", thread_name,
                   count);
        }
        pthread_cond_signal(&cond_var);          //signal for the change of conditional variable
        pthread_mutex_unlock(&lock_var);         //lock released by mutex
    }
}

void
thread_2(void *ptr)
{
    char thread_name[15];

    pthread_setname_np("odd_thread");   ///setting the name for thread fro even counting
    pthread_getname_np(pthread_self(), thread_name, 15);

    while (1) {
        sleep(1);
        pthread_mutex_lock(&lock_var);
        while (!(count % 2)) {
            pthread_cond_wait(&cond_var, &lock_var);
            count++;
            printf("Thread_name = %s  count = %d\n", thread_name,
                   count);
        }
        pthread_cond_signal(&cond_var);
        pthread_mutex_unlock(&lock_var);
    }
}

int
main(int argc, char **argv)
{
    int i;
    int rc = INVALID_RETURN_VALUE;
    pthread_t thread[NO_THREADS];

    char thread_name[10];

    pthread_setname_np("main");
    pthread_getname_np(pthread_self(), thread_name, 10);

    pthread_create(&thread[0], NULL, (void *) &thread_1, NULL);     //creation of thread 1 and 2
    pthread_create(&thread[1], NULL, (void *) &thread_2, NULL);


    for(i = 0; i < NO_THREADS; i++) {
        pthread_join(thread[i], NULL);     //waiting for the thread to get the termination value 
    }

    /*
     * Cleanup
     */
    rc = pthread_mutex_destroy(&lock_var);     //Destroy the lock after use

    if (rc) {
        printf("Unable to destroy lock = %s\n", thread_name);
    }

    rc = pthread_cond_destroy(&cond_var);      //Destroy the cond_var after use

    if (rc) {
        printf("Unable to destroy cv = %s\n", thread_name);
    }

    return 0;
}
