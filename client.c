// Trabalho final da disciplina de grafos 2019/1
// Tema 3: rede social de amizade "verdadeira"
// João Vitor Nascimento Villaça - 10724239
// Luca Machado Bottino - 9760300
// Luís Eduardo Rozante de Freitas Pereira - 10734794

// Cliente principal da rede social.

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include "recommendation.h"
#include "socialmanager.h"

void print_menu(void) {
        printf("===========================================================================\n");
        printf("\t1  -- Carregar um banco de dados a partir de um arquivo .csv;\n");
        printf("\t2  -- Escrever o banco de dados num arquivo;\n");
        printf("\t3  -- Imprimir matriz de amizades;\n");
        printf("\t4  -- Tentar criar uma amizade entre 2 perfis;\n");
        printf("\t5  -- Desfazer uma amizade entre 2 perfis;\n");
        printf("\t6  -- Sugerir uma amizade para um perfil;\n");
        printf("\t7  -- Sugerir desfazer uma amizade um perfil;\n");
        printf("\t8  -- Imprimir os dados de um perfil em específico;\n");
        printf("\t9  -- Criar um perfil novo e adicioná-lo ao buffer do banco de dados;\n");
        printf("\t10 -- Sugerir namorado(a);\n");
        printf("\t11 -- Printar amigos;\n");
        printf("\t12 -- Printar redes de amigos;\n");
        printf("\t13 -- Imprimir este menu novamente na tela;\n");
        printf("\t0  -- Sair desta aplicação (esta opção NÃO salva o banco de dados automaticamente).\n");
        printf("===========================================================================\n");
        printf("\tDigite a opção e pressione enter: ");
        
}

int main(int argc, char **argv, char **envp) {

        printf("===========================================================================\n");
        printf("Bem vindo ao SocialManager, seu sistema de gerenciamento de redes sociais escrito em C!\n");
        printf("===========================================================================\n");
        printf("Para começar, escolha uma das opções abaixo:\n");

        int opcao = 0;

        print_menu();
        GRAPH* gbuffer = newGraph();

        do {
                scanf(" %d", &opcao); // Recebe a opção.

                if(opcao == 0) break;
                else if(opcao == 1) {
                        
                        // Recebe o nome do arquivo a ser lido.
                        printf("Digite o nome do arquivo (até 60 caracteres, precisa estar no mesmo diretório que este programa): ");
                        char read[61];
                        scanf(" %60[^\n]", read);

                        // Tenta abrir o arquivo.
                        FILE* bdr = fopen(read, "r");
                        if(bdr == NULL) printf("Erro ao abrir o arquivo!\n");
                        else {
                                getDatabase(bdr, &gbuffer); // Lê os dados do arquivo e depois o fecha.
                                fclose(bdr);
                        }
                } else if (opcao == 2) {

                        // Recebe o nome do arquivo a ser salvo.
                        printf("Digite o nome do arquivo (até 60 caracteres, precisa estar no mesmo diretório que este programa, caso um arquivo com o mesmo nome exista, ele será substituido): ");
                        char write[61];
                        scanf(" %60[^\n]", write);

                        // Tenta abrir o arquivo.
                        FILE* bdw = fopen(write, "w+");
                        if(bdw == NULL) printf("Erro ao abrir o arquivo!\n");
                        else {
                                putDatabase(bdw, gbuffer); // Escreve os dados para o arquivo e depois o fecha.
                                fclose(bdw);
                        }
                } else if (opcao == 3) {
                       
                        printf("Matriz de amizades:\n");
                        for(int i = 0; i < gbuffer->vertex; i++) { // Printa a matriz de amizades do grafo.
                                printf("| ");
                                for(int j = 0; j < gbuffer->vertex; j++)
                                        printf("%d ", gbuffer->matrix[i][j]);
                                printf("|\n");
                        }
                } else if (opcao == 4) { // Recebe o nome dos dois usuários.
                        printf("Nome do usuário 1: ");
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);
                        printf("Nome do usuário 2: ");
                        char usu2[50];
                        scanf(" %49[^\n\r]", usu2);
                        
                        PROFILE *a, *b;
                        a = searchInGraph(gbuffer, usu1); // Acha os usuários na base de dados do grafo.
                        b = searchInGraph(gbuffer, usu2);
                        if(a == NULL || b == NULL) printf("Erro: um dos usuários não existe na tabela do banco de dados!\n");
                        else if(a == b) printf("Erro: os usuários deve ser diferentes!\n");
                        else {

                                float fit = calculateFit(a, b);
                                int fitPercent = (int)(fit * 100.0); // Calcula a porcentagem de chance da amizade ser verdadeira.

                                printf("%s mandou uma solicitação de amizade para %s, a chance da amizade ser verdadeira é de %d%%, a amizade foi aceita? yes - no: ", a->name, b->name, fitPercent);
                                char buf[64];
                                scanf(" %63s", buf);
                                if(strcmp(buf, "yes\0") == 0) { // Pede confirmação para fazer a amizade.
                                        addEdgeToGraph(gbuffer, a->id, b->id);
                                        printf("Amizade criada com sucesso! =)\n");
                                } else
                                        printf("A amizade não foi aceita! =(\n");
                        }
                } else if (opcao == 5) { // Recebe o nome dos dois usuários.
                        printf("Nome do usuário 1: ");
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);
                        printf("Nome do usuário 2: ");
                        char usu2[50];
                        scanf(" %49[^\n\r]", usu2);
                        
                        PROFILE *a, *b;
                        a = searchInGraph(gbuffer, usu1); // Acha os usuários na base de dados do grafo.
                        b = searchInGraph(gbuffer, usu2);
                        if(a == NULL || b == NULL) printf("Erro: um dos usuários não existe na tabela do banco de dados!\n");
                        else if(a == b) printf("Erro: os usuários deve ser diferentes!\n");
                        else {
                                float fit = calculateFit(a, b);
                                int fitPercent = (int)(fit * 100.0); // Calcula a porcentagem de chance da amizade ser verdadeira.

                                printf("%s quer desfazer a amizade com %s, a chance da amizade ser verdadeira é de %d%%, a amizade foi desfeita? yes - no: ", a->name, b->name, fitPercent);
                                char buf[64];
                                scanf(" %63s", buf);
                                if(strcmp(buf, "yes\0") == 0) { // Pede confirmação para desfazer a amizade.
                                        addEdgeToGraph(gbuffer, a->id, b->id);
                                        printf("Amizade desfeita com sucesso! =(\n");
                                } else 
                                        printf("A amizade não foi desfeita! =)\n");
                        }
                } else if (opcao == 6) {

                        printf("Nome do usuário para quem sugerir: "); // Recebe o nome do para quem fazer a sugestão.
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);

                        PROFILE *a;
                        a = searchInGraph(gbuffer, usu1); // Acha o usuário na base de dados do grafo.

                        if(a == NULL) printf("Erro: usuário não existe na tabela do banco de dados!\n");
                        else {

                                float fit;
                                int s = recommend_friend(gbuffer, a->id, &fit); // Faz a recomendação.

                                if(s != -1)
                                        printf("Recomendo que %s se torne amigo(a) de %s. (avaliação = %f)\n", a->name, gbuffer->userlist[s]->name, fit);
                                else
                                        printf("Infelizmente não tenho nenhuma recomendação para %s. =(\n", a->name);

                        }

                } else if (opcao == 7) {

                        printf("Nome do usuário para quem sugerir: "); // Recebe o nome do para quem fazer a sugestão.
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);

                        PROFILE *a;
                        a = searchInGraph(gbuffer, usu1); // Acha o usuário na base de dados do grafo.

                        if(a == NULL) printf("Erro: usuário não existe na tabela do banco de dados!\n");
                        else {

                                float fit;
                                int s = recommend_unfriend(gbuffer, a->id, &fit); // Faz a recomendação.
                                if(s != -1)
                                        printf("Recomendo que %s desfaça a amizade com %s. (avaliação = %f)\n", a->name, gbuffer->userlist[s]->name, fit);
                                else
                                        printf("Infelizmente %s não tem nenhuma amizade para desfazer. =(\n", a->name);

                        }

                } else if (opcao == 8) {

                        printf("Nome do usuário para imprimir: "); // Recebe o nome do para quem fazer a sugestão.
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);

                        PROFILE *a;
                        a = searchInGraph(gbuffer, usu1); // Acha o usuário na base de dados do grafo.

                        if(a == NULL) printf("Erro: usuário não existe na tabela do banco de dados!\n");
                        else putProfile(stdout, a); // Printa o perfil.

                } else if (opcao == 9) {

                        PROFILE *newProfile = malloc(sizeof(PROFILE));

                        printf("Insira os dados do novo usuário:\n"); // Recebe os dados do novo usuário.

                        printf("\tNome: ");
                        scanf(" %39[^\n]", newProfile->name);
                        printf("\tGenêro: ");
                        scanf(" %2s", newProfile->gender);
                        printf("\tIdade: ");
                        scanf(" %d", &(newProfile->age));

                        printf("\tSolteiro(a)?: yes - no  ");
                        char singleBuffer[5];
                        scanf(" %5s", singleBuffer);
                        if(strcmp(singleBuffer, "yes") == 0) newProfile->single = 1;
                        else newProfile->single = 0;

                        printf("\t-- Favorite tags:\n");
                        printf("\tCor: ");
                        scanf(" %39[^\n]", newProfile->color);
                        printf("\tLivro: ");
                        scanf(" %39[^\n]", newProfile->book);
                        printf("\tFilme: ");
                        scanf(" %39[^\n]", newProfile->movie);
                        printf("\tGênero musical: ");
                        scanf(" %39[^\n]", newProfile->musicgenre);

                        gbuffer->userlist[gbuffer->vertex] = newProfile; // Adiciona o usuário a base de dados do grafo.
                        gbuffer->matrix[gbuffer->vertex][gbuffer->vertex] = 1; // Marca o usuário como 'amigo' de si mesmo por padrão.
                        gbuffer->vertex++; // Indica que um novo usuário (vértice) foi adicionado.

                        printf("Usuário adicionado!\n");

                } else if (opcao == 10) { 
                        
                        printf("Nome do usuário para quem sugerir: "); // Recebe o nome do para quem fazer a sugestão.
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);

                        PROFILE *a;
                        a = searchInGraph(gbuffer, usu1); // Acha o usuário na base de dados do grafo.

                        if(a == NULL) printf("Erro: usuário não existe na tabela do banco de dados!\n");
                        else {

                                float fit;
                                int s = recommend_rommance(gbuffer, a->id, &fit); // Faz a recomendação.
                                if(s != -1)
                                        printf("Recomendo que %s namore com %s. (avaliação = %f)\n", a->name, gbuffer->userlist[s]->name, fit);
                                else
                                        printf("Infelizmente não tenho nenhuma recomendação para %s. =(\n", a->name);

                        }
                } else if (opcao == 11) {

                        printf("Nome do usuário para imprimir: "); // Recebe o nome do para quem fazer a sugestão.
                        char usu1[50];
                        scanf(" %49[^\n\r]", usu1);

                        PROFILE *a;
                        a = searchInGraph(gbuffer, usu1); // Acha o usuário na base de dados do grafo.

                        if(a == NULL) printf("Erro: usuário não existe na tabela do banco de dados!\n");
                        else printFriends(stdout, gbuffer, a->id); // Printa os amigos de um usuário (vértices adjacentes em um grafo).
                } else if (opcao == 12) {
                        printFriendNets(stdout, gbuffer); // Printa as redes de amigos (componentes conexas em um grafo).
                } else if (opcao == 13) {
                        print_menu();
                } else {
                        printf("Opção inválida! Digite novamente: ");
                }

                if(opcao <= 13 && opcao >= 0) {
                        printf("===========================================================================\n");
                        printf("Retornando ao menu principal... Digite 13 para ver as opções novamente.\n");
                        printf("===========================================================================\n");
                }

        } while(opcao != 0);

        freeGraph(&gbuffer);
        return EXIT_SUCCESS;
        
}
