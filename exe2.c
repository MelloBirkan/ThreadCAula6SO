#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>

#define BUFFER_SIZE 1

int buffer[BUFFER_SIZE];
int count = 0;

sem_t empty; // Semáforo para controlar espaços vazios no buffer
sem_t full;  // Semáforo para controlar espaços preenchidos no buffer
pthread_mutex_t mutex; // Mutex para controlar o acesso ao buffer

void *producer(void *arg) {
    int i;
    for (i = 1; i <= 10; i++) {
        sem_wait(&empty); // Aguarda um espaço vazio no buffer
        pthread_mutex_lock(&mutex); // Bloqueia o acesso ao buffer

        buffer[count++] = i; // Insere o número no buffer
        printf("Produtor criou: %d\n", i);
        
        pthread_mutex_unlock(&mutex); // Libera o acesso ao buffer
        sem_post(&full); // Sinaliza que um espaço no buffer está preenchido
        sleep(rand() % 6 + 1); // Adormece por um tempo aleatório entre 1 e 6 segundos
    }
    return NULL;
}

void *consumer(void *arg) {
    int item, i;
    for (i = 0; i < 10; i++) {
        sem_wait(&full); // Aguarda um espaço preenchido no buffer
        pthread_mutex_lock(&mutex); // Bloqueia o acesso ao buffer

        item = buffer[--count]; // Consome o item do buffer
        buffer[count] = 0; // Atribui 0 à posição consumida do buffer
        printf("Consumidor usou: %d\n", item);

        pthread_mutex_unlock(&mutex); // Libera o acesso ao buffer
        sem_post(&empty); // Sinaliza que um espaço no buffer está vazio
        sleep(rand() % 10 + 1); // Adormece por um tempo aleatório entre 1 e 10 segundos
    }
    return NULL;
}

int main() {
    pthread_t prod_thread, cons_thread;

    // Inicializa o buffer com zeros
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0;
    }

    sem_init(&empty, 0, BUFFER_SIZE); // Inicializa o semáforo empty com o valor BUFFER_SIZE
    sem_init(&full, 0, 0); // Inicializa o semáforo full com o valor 0
    pthread_mutex_init(&mutex, NULL); // Inicializa o mutex

    pthread_create(&prod_thread, NULL, producer, NULL); // Cria a thread do produtor
    pthread_create(&cons_thread, NULL, consumer, NULL); // Cria a thread do consumidor

    pthread_join(prod_thread, NULL); // Sincroniza a thread produtora
    pthread_join(cons_thread, NULL); // Sincroniza a thread consumidora

    // Destroi os semáforos e o mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
