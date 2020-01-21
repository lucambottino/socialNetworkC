// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// Fila estática que não precisa shiftar. 

# ifndef QUEUE_H
# define QUEUE_H

# define MAX_QUEUE_SIZE 100

typedef struct queue Queue;

// Cria uma nova fila.
Queue *create_queue();

// Insere um elemento no final da fila.
void push_queue(Queue *q, int e);

// Remove um elemento do início da fila.
int pop_queue(Queue *q);

// Pega o tamanho da fila.
int get_size_queue(Queue *q);

# endif