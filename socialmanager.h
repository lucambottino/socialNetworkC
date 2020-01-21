// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// COntém funções base para interagir com o grafo que representa a rede social.

#ifndef _SOCIALMANAGER_H_
#define _SOCIALMANAGER_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define SUCCESS 1
#define FAILURE 0
#define ERROR -32000
#define MAX 100

// Perfil de usuário.
typedef struct _profile {
    char name[40]; // Nome do usuário.
    int age; // Idade do usuário.
    char gender[3]; // O primeiro char é o sexo biológico do usuário sendo (m = homem, f = mulher), o segundo é em que sexo biológio o usuário está
                    // interessado (a opção 'b' pode ser usada para indicar ambos), o terceiro char deve ser um '\0'.
    int single; // Se o usuário está solteiro.
    char color[40], movie[40], book[40], musicgenre[40]; // Preferências do usuário.
    int id; // Id do usuário no grafo.
} PROFILE;

// Grafo.
typedef struct _graph {
    int vertex; // Número de vértices do grafo.
    int edges; // Número de arestas do grafo.
    int matrix[MAX][MAX]; // Matriz de adjacências do grafo ([i][j] == 0 -> id(i) e id(j) não são amigos, [i][j] == 1 -> id(i) e id(j) são amigos )
    PROFILE* userlist[MAX]; // Base de dados do grafo contendo a lista de usuários.
} GRAPH;

void *newGraph(void); // Cria um novo grafo.
void addEdgeToGraph(GRAPH *G, int row, int col); // Adiciona uma aresta (amizade) ao grafo.
void removeEdgeFromGraph(GRAPH *G, int row, int col); // Remove uma aresta (amizade) ao grafo.
void *searchInGraph(GRAPH *G, char* username); // Busca um usuário na base de dados do grafo e retorna seu perfil.
float calculateFit(PROFILE* a, PROFILE* b); // Calcula o fit entre dois usuários.
void printFriends(FILE *stream, GRAPH *G, int id); // Printa os vértices adjacentes (amigos) de um usuário para uma stream.
void printFriendNets(FILE *stream, GRAPH *G); // Printa as componentes conexas (redes de amigos) do grafo.
void putProfile(FILE *stream, PROFILE *P); // Printa um usuário para uma stream.
void putDatabase(FILE *stream, GRAPH *G); // Salva a base de dados em uma stream.
void getDatabase(FILE *stream, GRAPH **G); // Carrega a base de dados de uma stream.
void freeGraph(GRAPH **G); // Libera a memória do grafo.

#endif

