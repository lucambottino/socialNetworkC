// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// Contém funções de recomendação para adição e remoção de amigos e de recomendação de namorado(a).

# ifndef RECOMMENDATION_H
# define RECOMMENDATION_H

# include "socialmanager.h"

# define DEBUG_RECOMMENDATION 1

// Recomenda uma novo amigo para um usuário. 
int recommend_friend(GRAPH *G, int id, float *fitRet);

// Recomenda que um usuário defaça sua amizade menos verdadeira.
int recommend_unfriend(GRAPH *G, int id, float *fitRet);

// Recomenda um namorado(a) para um usuário, b
int recommend_rommance(GRAPH *G, int id, float *fitRet);

# endif