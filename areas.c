/*
 * Autor: Guilherme Curado Lousada Pais da Silva
 * Numero: 21260315
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "structs.h"
#include "header.h"
#define TAM 50

int getArea(areas *v) {                         //Função que lê um fichiro de texto com a informação relativa às áreas
    int k = 0, tam;
    FILE *f;
    if ((f = fopen("Areas.txt", "rt")) == NULL) {   //Abre-se o ficheiro para leitura
        printf("Erro ao ler o ficheiro de areas\n");
        fclose(f);
        return 0;
    }
    fscanf(f, " %d\n", &tam);                       //Procura-se o inteiro que se encontra no inicio do ficheiro com o número de areas existentes no mesmo
    for (int j = 0; j < tam; j++) {
        fscanf(f, " %s %d ", v[j].nome, &v[j].peso);//Lê-se o nome, o peso máximo, o número de áreas próximas e o nome das mesmas da área
        fscanf(f, " %d ", &v[j].proximos);
        for (k = 0; k < v[j].proximos; k++)
            fscanf(f, " %s ", v[j].perto[k]);
    }
    fclose(f);
    return 1;
}

int gettam() {                                      //Função que obtem apenas o tamanho do vetor de areas para poder alocar memoria para o mesmo
    int tam;
    FILE *f;
    if ((f = fopen("Areas.txt", "rt")) == NULL) {
        printf("Erro ao ler o ficheiro de areas\n");
        fclose(f);
        return 0;
    }
    fscanf(f, " %d ", &tam);
    fclose(f);
    return tam;
}

int verifyProximas(areas* v, int tam) {             // Função que imprime quais as areas que ainda podem ter areas proximas
    int limite = 0;
    printf("\nAreas que podem ser proximas:");
    for (int i = 0; i < tam; i++) {
        if (v[i].proximos < 3) {
            printf("%s", v[i].nome);
            limite++;
            if ((tam - i) > 1)
                printf("/");
        }
    }
    return limite;
}

areas* getVetor(int tam) {                         //Função que aloca memoria para o vetor das áreas
    return malloc(sizeof (areas) * tam);
}

void listAreas(int tam, areas *vetorArea) {        //Função que lista todas as áreas existentes no zoo
    printf("\n\nAreas do zoo:\n");
    for (int i = 0; i < tam; i++) {
        printf(" %s \n", vetorArea[i].nome);
    }
    return;
}

areas* addArea(areas *v, int *t) {                  //Função que adiciona uma área ao vetor de areas do zoo
    int erro, tam = *t, lim;
    int i;
    if ((v = realloc(v, sizeof (areas) * (tam + 1))) == NULL) { //Realocação de memória para poder adicionar a nova area
        printf("\n\n\7Erro na realocação de memória");
        return;
    }
    for (;;) {
        erro = 0;
        printf("\n\nNome da nova area:");
        scanf(" %s", &v[tam].nome);
        for (i = 0; i < tam; i++) {                             //Verifica se o nome da área já não foi usado
            if (strcmp(v[tam].nome, v[i].nome) == 0) {
                printf("Nome ja existe!");
                erro = 1;
            }
        }
        if (erro == 0)
            break;
    }
    printf("Peso maximo da area:");
    scanf(" %d", &v[tam].peso);
    for (;;) {
        lim = verifyProximas(v, tam);
        printf("\nNumero de areas proximas:");
        scanf(" %d", &v[tam].proximos);
        if (v[tam].proximos < 4 && v[tam].proximos >= 0 && v[tam].proximos <= (lim))
            break;
    }
    for (i = 0; i < v[tam].proximos; i++) {
        for (;;) {
            erro = 1;
            printf("Nome da area proxima %d:", (i + 1));
            scanf(" %s", &v[tam].perto[i]);
            if ((strcmp(v[tam].perto[i], v[tam].perto[i - 1])) == 0 || (strcmp(v[tam].perto[i], v[tam].perto[i - 2])) == 0) //Verifica se a área proxima introduzida já foi introduzida antes
                printf("\aArea ja introduzida\n");
            else {
                for (int j = 0; j < tam; j++) {
                    if (strcmp(v[tam].perto[i], v[j].nome) == 0) {
                        erro = 0;
                        if (v[j].proximos == 3) {                               //Se a area já estiver no limite de areas proximas o programa nao autoriza a operação
                            printf("\aArea nao pode ter mais areas proximas\n");
                            erro = 1;
                        } else {
                            v[j].proximos++;
                            strcpy(v[j].perto[v[j].proximos - 1], v[tam].nome); //Põe o nome da area criada num dos lugares disponiveis para areas proximas da area proxima introduzida

                        }
                        break;
                    }
                }
            }
            if (erro == 0)
                break;
        }
    }
    *t = *t + 1;
    return v;
}

void carArea(areas *v, int tam) { //Função que imprime as caracteristicas todas de uma area espicifica
    int corresponde = 1, i;
    char nome[TAM];
    listAreas(tam, v);
    printf("\nIntroduza o nome da area:");
    for (;;) {
        scanf(" %s", &nome);
        for (i = 0; i < tam; i++) {
            if (strcmp(nome, v[i].nome) == 0) {
                corresponde = 0;
                break;
            }
        }
        if (corresponde == 1)
            printf("\aNome nao existe\nIntroduza o nome da area:");
        else
            break;
    }
    printf("Nome:%s\nPeso maximo:%d", v[i].nome, v[i].peso);
    printf("\nAreas proximas:");
    for (int j = 0; j < v[i].proximos; j++)
        printf("%s ", v[i].perto[j]);
    return;
}

void saveArea(areas* v, int tam) { //Função que guarda a informação das areas num ficheiro de texto
    FILE *f;
    if ((f = fopen("Areas.txt", "wt")) == NULL) {
        printf("\aErro ao guardar dados");
        fclose(f);
        return;
    }
    fprintf(f, "%d\n", tam);
    for (int i = 0; i < tam; i++) {
        fprintf(f, "%s %d ", v[i].nome, v[i].peso);
        fprintf(f, "%d ", v[i].proximos);
        for (int j = 0; j < v[i].proximos; j++)
            fprintf(f, "%s ", v[i].perto[j]);
        fprintf(f, ("\n"));
    }
    fclose(f);
}

areas* delArea(areas *v, panimal p, int *t) { //Função que apaga uma area
    int i = 0, tam = *t;
    char nome[TAM];
    listAreas(tam, v);
    printf("\nDigite o nome da area que pretende eliminar:");
    for (;;) {
        scanf("%s", &nome);
        i = 0;
        while (i < tam) {
            if ((strcmp(nome, v[i].nome)) == 0)
                break;
            i++;
        }
        if ((strcmp(nome, v[i].nome)) == 0)
            break;
        printf("\aNome inexistente\nDigite novamente:");
    }
    while (p != NULL) {
        if (strcmp(nome, p->area) == 0) {
            printf("\aAinda ha animais nesta area!");
            return v;
        }
        p = p->prox;
    }
    *t = *t - 1;
    tam = *t;
    v[i] = v[tam];
    v = realloc(v, sizeof (areas)*(tam));  
    for (i = 0; i < tam; i++) {                         //Aqui o programa retira o nome da area eliminada do campo de areas proximas das suas areas proximas
        if (strcmp(v[i].perto[0], nome) == 0) {
            strcpy(v[i].perto[0], v[i].perto[1]);
            strcpy(v[i].perto[1], v[i].perto[2]);
            strcpy(v[i].perto[2], "\0");
            v[i].proximos--;
        }
        if (strcmp(v[i].perto[1], nome) == 0) {
            strcpy(v[i].perto[1], v[i].perto[2]);
            strcpy(v[i].perto[2], "\0");
            v[i].proximos--;
        }
        if (strcmp(v[i].perto[2], nome) == 0) {
            strcpy(v[i].perto[2], "\0");
            v[i].proximos--;
        }
    }
    return v;
}


