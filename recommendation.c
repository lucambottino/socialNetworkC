// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// Contém funções de recomendação para adição e remoção de amigos e de recomendação de namorado(a).

# include "queue.h"
# include "socialmanager.h"

# include <stdlib.h>

// Realiza uma BFS no grafo a partir de um nó s e retorna um vetor de distâncias.
int *bfs(int s, GRAPH *G) {

    Queue *q = create_queue();
    if(q == NULL) return NULL;

    int *dist = malloc(G->vertex * sizeof(int));
    if(dist == NULL) { // Verifica se a memória foi alocada com sucessor, retorna um erro caso contrário.
        return NULL;
    }
    memset(dist, -1, sizeof(int) * G->vertex);

    dist[s] = 0;
    push_queue(q, s);

    while (get_size_queue(q) > 0) {

        int u = pop_queue(q);
        
        for(int v = 0; v < G->vertex; v++) {

            if(G->matrix[u][v] == 1 && dist[v] == -1) {

                dist[v] = dist[u] + 1;
                push_queue(q, v);

            }
        }
    }

    free(q);
    return dist;

}

// Recomenda um novo amigo para um usuário baseado no fit, na distância entre amigos em comum e na proximidade dos amigos em comum.
int recommend_friend(GRAPH *G, int id, float *fitRet) {

    // Fator de por quanto a distância entre usuarios deve reduzir no fator de recomendação.
    const float distFactor = 0.2f;
    // Fator de quanto o fit de amigos em comum deve influenciar no fator de recomendação.
    const float commonFactor = 0.05f;
    // Distância máxima entre usuários a ser considerada.
    const int maxDist = 4;

    // Calcula a quantos amigos de distância os outros usuários estão do usuário inicial.
    int *dist = bfs(id, G);

    // DEBUG: Printa o resultado da BFS.
    # if DEBUG_RECOMMENDATION == 1
    for(int i = 0; i < G->vertex; i++) printf("%d ", dist[i]);
    putchar('\n');
    # endif

    // Guarda o fator de recomendação e o id do melhor usuário para recomendar como amigo.
    float maxRecmn = -1;
    int maxId = -1;

    // Calcula o fator de recomendação para todos os possíveis usuários.
    for(int i = 0; i < G->vertex; i++) {

        if(G->matrix[id][i] == 0 && i != id) { // Caso os usuários ainda não sejam amigos,
                                               // calcula o fit dividido pela distância.

            // Calcula um bonus para o fator baseando no número de amigos em comum diretos e considerando o fit entre as partes como um multiplicador.
            float common_friends = 0;
            for(int j = 0; j < G->vertex; j++) {

                if(G->matrix[id][j] == 1 && G->matrix[i][j] == 0 && i != j && id != j)
                    common_friends += (calculateFit(G->userlist[id], G->userlist[j]) + calculateFit(G->userlist[i], G->userlist[j])) / 2.0f;

            }

            // Limita a distância entre os usuários a ser considerada.
            int dist_i = (float)dist[i];
            if(dist_i == -1 || dist_i > maxDist) dist_i = maxDist;
    
            // Calcula o fator de recomendação.
            float recmn = (calculateFit(G->userlist[id], G->userlist[i]) + (common_friends * commonFactor)) / (1 + (distFactor * dist_i));

            // DEBUG:
             # if DEBUG_RECOMMENDATION == 1
            printf("%f ", recmn);
            # endif

            if(recmn > maxRecmn) { // Atualiza o melhor candidato a amigo se necessário.
                maxRecmn = recmn;
                maxId = i;

            }
        }

    }

    // Libera a memória.
    free(dist);

    *fitRet = maxRecmn; // Marca o quão recomendado foi o usuário.
    return maxId; // Retorna o id do usuário com o fit mais alto relativo a quantidade de amigos de distância. Retorna -1 se nenhum candidato válido
                  // for encontrado.


}

// Recomenda que um usuário defaça uma amizade caso ela não seja verdadeira.
int recommend_unfriend(GRAPH *G, int id, float *fitRet) {

    // Fator de quanto o fit de amigos em comum deve influenciar no fator de recomendação.
    const float commonFactor = 0.05f;
    
    // Guarda o fator de recomendação e o id do melhor usuário para recomendar como inimigo.
    float maxRecmn = -1;
    int maxId = -1;

    // Calcula o fator de recomendação para todos os possíveis usuários.
    for(int i = 0; i < G->vertex; i++) {

        if(G->matrix[id][i] == 1 && i != id) { // Caso os usuários sejam amigos calcula o fit.

            /// Calcula um bonus para o fator baseando no número de amigos em comum diretos e considerando o fit entre as partes como um multiplicador.
            float common_friends = 0;
            for(int j = 0; j < G->vertex; j++) {

                if(G->matrix[id][j] == 1 && G->matrix[i][j] == 0 && i != j && id != j)
                    common_friends += (calculateFit(G->userlist[id], G->userlist[j]) + calculateFit(G->userlist[i], G->userlist[j])) / 2.0f;

            }
    
            // Calcula o fator de recomendação.
            float recmn = commonFactor / (0.0001 + calculateFit(G->userlist[id], G->userlist[i]) + (common_friends * commonFactor)); // Um valor pequeno é adicionado para
                                                                                                                            // impedir divisões por 0.

            // DEBUG:
             # if DEBUG_RECOMMENDATION == 1
            printf("%f ", recmn);
            # endif

            if(recmn > maxRecmn) { // Atualiza o melhor candidato a amigo se necessário.
                maxRecmn = recmn;
                maxId = i;
            }
        }

    }

    *fitRet = maxRecmn; // Marca o quão recomendado foi o usuário.
    return maxId; // Retorna o id do usuário com o fit mais alto relativo a quantidade de amigos de distância. Retorna -1 se nenhum candidato válido
                  // for encontrado.

}

// Verifica se o gênero de duas pessoas é compatível.
int genderCompatible(char a[], char b[]) {

    if(a[1] == b[0] || a[1] == 'b')
        if(b[1] == a[0] || b[1] == 'b')
            return 1;

    return 0;

}

// Recomenda um namorado(a) para um usuário, b
int recommend_rommance(GRAPH *G, int id, float *fitRet) {

    float maxFit = -1;
    int maxId = -1;

    // Passa por todos os usuários e encontra aquele de maior fit.
    for(int i = 0; i < G->vertex; i++) {

        // Se não é o próprio usuário, suas prefências de gênero são compatíveis, verifica o fit e o outro usuário está solteiro.
        if(i != id && genderCompatible(G->userlist[id]->gender, G->userlist[i]->gender) && G->userlist[i]->single) {

            float fit = calculateFit(G->userlist[id], G->userlist[i]);

            if(fit > maxFit) {
                maxFit = fit;
                maxId = i;
            }
        }
    }

    *fitRet = maxFit; // Marca o quão recomendado foi o usuário.
    return maxId; // Retorna a Id do usuário de maior fit compatível, ou -1 se nenhum for encontrado.
}
