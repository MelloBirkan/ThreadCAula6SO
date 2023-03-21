#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define BUFFER_SIZE 5 // Define o tamanho máximo do buffer

int buffer[BUFFER_SIZE]; // Buffer para armazenar os números
int count = 0; // Contador para controlar a quantidade de números no buffer
int producer_done = 0; // Variável para sinalizar que o produtor terminou

// Função do produtor
void *producer(void *arg) {
    int i;
    for (i = 1; i <= 10; i++) {
        while (count == BUFFER_SIZE) {
            // Aguarda espaço disponível no buffer
            sleep(1);
        }

        buffer[count++] = i; // Insere o número no buffer
        printf("Produtor criou: %d\n", i);
        sleep(rand() % 6 + 1); // Adormece por um tempo aleatório entre 1 e 6 segundos
    }
    producer_done = 1; // Indica que o produtor terminou
    return NULL;
}

// Função do consumidor
void *consumer(void *arg) {
    int item;
    while (!producer_done || count > 0) { // Continua enquanto o produtor não terminar ou houver itens no buffer
        while (count == 0) {
            if (producer_done) {
                return NULL;
            }
            // Aguarda itens disponíveis no buffer
            sleep(1);
        }

        item = buffer[--count]; // Consome o item do buffer
        buffer[count] = 0; // Atribui 0 à posição consumida do buffer
        printf("Consumidor usou: %d\n", item);
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

    pthread_create(&prod_thread, NULL, producer, NULL); // Cria a thread do produtor
    pthread_create(&cons_thread, NULL, consumer, NULL); // Cria a thread do consumidor

    pthread_exit(NULL); // Encerra a função main, mas permite que as threads continuem executando
}
