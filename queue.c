// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// Fila estática que não precisa shiftar. 

# include "queue.h"
# include <stdlib.h>

struct queue
{
    
    int elements[MAX_QUEUE_SIZE]; // Guarda os elementos da fila.
    int size; // Guarda o tamanho da fila.
    int front; // Guarda a posição do primeiro elemento.
    int end; // Guarda a posição do último elemento.

};

// Cria uma nova fila.
Queue *create_queue() {

    Queue *q = malloc(sizeof(Queue));
    
    q->size = 0;
    q->front = 0;
    q->end = 0;

    return q;

}

// Insere um elemento no final da fila.
void push_queue(Queue *q, int e) {

    q->size++;
    q->elements[q->end] = e;
    q->end = (q->end + 1) % MAX_QUEUE_SIZE;

}

// Remove um elemento do início da fila.
int pop_queue(Queue *q) {

    q->size--;
    int r = q->elements[q->front];
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;

    return r;

}

// Pega o tamanho da fila.
int get_size_queue(Queue *q) { 

    return q->size;

}