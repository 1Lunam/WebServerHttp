#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_LOGS 1000
#define MAX_MSG_LEN 512

pthread_t thread;

// coda di 100 x 512
char log_queue[MAX_LOGS][MAX_MSG_LEN];

int head = 0;
int tail = 0;

pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;


// thread che si occupa dei log
void *logger_worker(void *args){

    while(1){

        pthread_mutex_lock(&log_mutex);

        if(head != tail){

            char msg[MAX_MSG_LEN];

            strcpy(msg, log_queue[head]);

            // ritorno a 0 una volta arrivato all'indice massimo
            head = (head + 1) % MAX_LOGS;

            pthread_mutex_unlock(&log_mutex);

            // apertura e scrittura del file di log
            FILE *log_file = fopen("access.log", "a");

            if(log_file != NULL){
                fprintf(log_file, "%s\n", msg);
                printf("LOGGER: %s\n", msg);
                fclose(log_file);
            }
        }else{

            pthread_mutex_unlock(&log_mutex);
            sleep(1);
        }
    }

    return NULL;
}


// avvio del thread
void logger_thread(){
    pthread_create(&thread, NULL, logger_worker, NULL);
}


// aggiunge un log alla coda richiamato dal file_handler quando riceve 200 ok
void add_log(const char *log_msg){

    pthread_mutex_lock(&log_mutex);

    int next_tail = (tail + 1) % MAX_LOGS;

    // coda piena
    if(next_tail == head){
        pthread_mutex_unlock(&log_mutex);
        return;
    }

    strcpy(log_queue[tail], log_msg);

    tail = next_tail;

    pthread_mutex_unlock(&log_mutex);
}