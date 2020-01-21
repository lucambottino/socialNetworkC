// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// COntém funções base para interagir com o grafo que representa a rede social.

#include "socialmanager.h"

// Cria um novo grafo.
void *newGraph(void) {

    GRAPH *G = malloc(sizeof(GRAPH));
    if(G == NULL) return NULL;
    G->vertex = 0;
    G->edges = 0;
    for(int i = 0; i < MAX; i++){
        for(int j = 0; j < MAX; j++){
            G->matrix[i][j] = 0;
        }
    }
    return G;
}

// Adiciona uma aresta (amizade) ao grafo.
void addEdgeToGraph(GRAPH *G, int row, int col) {

    if(G->matrix[row][col] == 0 || G->matrix[col][row] == 0) {
        G->matrix[row][col] = 1;
        G->matrix[col][row] = 1;
        G->edges++;
    }
}

// Remove uma aresta (amizade) ao grafo.
void removeEdgeFromGraph(GRAPH *G, int row, int col) {

    if(G->matrix[row][col] == 1 || G->matrix[col][row] == 1) {
        G->matrix[row][col] = 0;
        G->matrix[col][row] = 0;
        G->edges--;
    }
}

// Busca um usuário na base de dados do grafo e retorna seu perfil.
void *searchInGraph(GRAPH *G, char* username) {

    PROFILE *found = NULL;

    for(int i = 0; i < G->vertex; i++) {
        if(!strcmp(username, G->userlist[i]->name)) {
            found = G->userlist[i];
            G->userlist[i]->id = i;
            break;
        }
    }
    return found;
}

// Calcula o fit entre dois usuários.
float calculateFit(PROFILE *a, PROFILE *b) {

    float fit = 0;

    // Adiciona 0.2f (20%) para cada preferência igual entre os usuários.
    if(strcmp(a->color, b->color) == 0){
        fit = fit + 0.2f;
    }
    if(strcmp(a->movie, b->movie) == 0){
        fit = fit + 0.2f;
    }
    if(strcmp(a->book, b->book) == 0){
        fit = fit + 0.2f;
    }
    if(strcmp(a->musicgenre, b->musicgenre) == 0){
        fit = fit + 0.2f;
    }

    // Adiciona até 0.2f (20%) baseado na diferença de idade dos usuários.
    float aux;
    int dif = a->age - b->age;

    if(dif < 0) dif *= -1;

    int max_age = a->age; // Calcula a idade máxima entre os usuários.
    if(b->age > max_age) max_age = b->age;

    aux = (float)dif/(float)max_age;
    fit = fit + 0.2f * aux;
    
    return fit;
}

// Printa os amigos de um usuário.
void printFriends(FILE *stream, GRAPH *G, int id) {

    int count = 0; // Conta quantos amigos foram encontrados.

    for(int i = 0; i < G->vertex; i++) { // Printa os amigos do usuário id.

        if(i != id) { // Verifica se não é o próprio usuário

            if(G->matrix[id][i] == 1) {

                if(count > 0) fprintf(stream, ", ");
                fprintf(stream, "%s", G->userlist[i]->name);
                count++;

            }
        }
    }

    // Printa uma mensagem baseado no número de amigos.
    if(count == 0) fprintf(stream, "%s não possui nenhum amigo! =(\n", G->userlist[id]->name);
    else if(count == 1) fprintf(stream, " é o único(a) amigo de %s!\n", G->userlist[id]->name);
    else fprintf(stream, " são os %d amigos de %s!\n", count, G->userlist[id]->name);

}

// Realiza uma DFS no grafo marcando o vetor vis e printando os nós visistados para a stream.
void dfs(FILE *stream, GRAPH *G, int *vis, int u) {

    if(G == NULL || vis == NULL || vis[u] == 1) return; // Retorna se há algo de errado.

    vis[u] = 1; // Marca o vértice como visitado.

    fprintf(stream, "%s,", G->userlist[u]->name); // Printa o nome do usuário no vértice de Id u para a stream.

    for(int v = 0; v < G->vertex; v++) {

        if(G->matrix[u][v] == 1 && vis[v] != 1) // Olha os vértices adjacentes e visita todos aqueles que ainda não fora.
            dfs(stream, G, vis, v);

    }

}

// Printa as componentes conexas (redes de amigos) do grafo.
void printFriendNets(FILE *stream, GRAPH *G) {

    fprintf(stream, "Redes de amigos:\n");

    int *vis = malloc(G->vertex * sizeof(int)); // Aloca e inicializa um vetor de visitados.
    for(int i = 0; i < G->vertex; i++) vis[i] = 0; 

    for(int i = 0; i < G->vertex; i++) { // Visita todos os nós do grafo.

        if(vis[i] == 0) {

            fprintf(stream, "{"); // Printa uma componente e a marca como visitada.
            dfs(stream, G, vis, i);
            fprintf(stream, "}");

        }
        
    }
    fprintf(stream, "\n");

    free(vis);

}

// Printa um usuário para uma stream.
void putProfile(FILE *stream, PROFILE *P) {

    if(P == NULL) return;
    if(stream == NULL) stream = stdout;

    fprintf(stream, "User <%s> data:\n", P->name);
    fprintf(stream, "- Gender: %s\n", P->gender);
    fprintf(stream, "- Age: %d\n", P->age);
    fprintf(stream, "- Is single?: %s\n", P->single == 1 ? "yes" : "no");
    fprintf(stream, "- Favorite tags:\n");
    fprintf(stream, "-- Color: %s\n", P->color);
    fprintf(stream, "-- Book: %s\n", P->book);
    fprintf(stream, "-- Movie: %s\n", P->movie);
    fprintf(stream, "-- Music genre: %s\n", P->musicgenre);

}

// Salva a base de dados em uma stream.
void putDatabase(FILE *stream, GRAPH *G) {

    if(G == NULL) return;
    if(stream == NULL) stream = stdout;

    fprintf(stream, "User amount: %d\n", G->vertex);

    for(int i = 0; i < G->vertex; i++)
        putProfile(stream, G->userlist[i]);

    for(int i = 0; i < G->vertex; i++) {
        for(int j = 0; j < G->vertex; j++)
            fprintf(stream, "%d ", G->matrix[i][j]);
        fprintf(stream, "\n");
    }
}

// Carrega a base de dados de uma stream.
void getDatabase(FILE *stream, GRAPH **G) {

    if(*G != NULL) freeGraph(G);

    GRAPH *g = newGraph();

    int numVertex;
    fscanf(stream, "User amount: %d ", &numVertex);

    for (int i = 0; i < numVertex; i++) {

        PROFILE *profile = malloc(sizeof(PROFILE));
        fscanf(stream, "User <%39[^>]> data: ", profile->name);
        fscanf(stream, " - Gender: %2s ", profile->gender);
        fscanf(stream, " - Age: %d ", &(profile->age));

        char buf[6];
        fscanf(stream, " - Is single?: %5s - Favorite tags: ", buf);

        if(strcmp(buf, "yes\0")) profile->single = 0;
        else profile->single = 1;

        fscanf(stream, "-- Color: %39[^\n\r] ", profile->color);
        fscanf(stream, "-- Book: %39[^\n\r] ", profile->book);
        fscanf(stream, "-- Movie: %39[^\n\r] ", profile->movie);
        fscanf(stream, "-- Music genre: %39[^\n]\r ", profile->musicgenre);

        g->userlist[i] = profile;
        g->vertex++;

    }

    for(int i = 0; i < g->vertex; i++)
        for(int j = 0; j < g->vertex; j++)
            fscanf(stream, " %d", &(g->matrix[i][j]));
    
    *G = g;

}

// Libera a memória do grafo.
void freeGraph(GRAPH **G) {

    for (int i = 0; i < (*G)->vertex; i++)
        free((*G)->userlist[i]);

    free(*G);
    *G = NULL;    

}






























