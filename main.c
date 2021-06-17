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

void fullscreen() { //Funçao tirada da internet para por a janela em fullscreen
    keybd_event(VK_MENU,
            0x38,
            0,
            0);
    keybd_event(VK_RETURN,
            0x1c,
            0,
            0);
    keybd_event(VK_RETURN,
            0x1c,
            KEYEVENTF_KEYUP,
            0);
    keybd_event(VK_MENU,
            0x38,
            KEYEVENTF_KEYUP,
            0);
}

void lista(panimal p) { //Função que faz um ciclo para imprimir todos os elementos da lista ligada (sem areas)
    while (p != NULL) {
        printf("\nNome:%s\nEspecie:%s\nNumero de serie:%d\n", p->nome, p->especie, p->numero);
        p = p->prox;
    }
}

void listaA(panimal p) {//Função que faz um ciclo para imprimir todos os elementos da lista ligada (com areas)
    while (p != NULL) {
        printf("\nNome:%s\nEspecie:%s\nArea:%s\nNumero de serie:%d\n", p->nome, p->especie, p->area, p->numero);
        p = p->prox;
    }
}

panimal fillLista(int *serie, areas* v, int tam) { //Função que lê o ficheiro binário e armazena a informação dos animais numa lista ligada
    panimal novo, lista = NULL, inicio;
    FILE *f;
    int i, j, total;
    f = fopen("Animais.dat", "rb");
    if (!f)
        return NULL;                            //Se a abertura do ficheiro falhar retorna NULL
    fseek(f, -sizeof (int), SEEK_END);          //A leitura é feita do fim para o princípio
    fread(&total, sizeof (int), 1, f);
    *serie = total;                             //Armazena-se o total em *serie para se poder atribuir os numeros de serie a animais futuros
    for (i = total - 1; i >= 0; i--) {
        fseek(f, sizeof (animals) * i, SEEK_SET);//Procura uma extrutura do tipo animals
        novo = malloc(sizeof (animals));         //Aloca-se memoria para o novo elemento
        if (novo == NULL) {
            fclose(f);
            return lista;
        }
        fread(novo, sizeof (animals), 1, f);     //É lido um elemento do ficheiro binario
        for(j=0;j<tam;j++)
        {
            if(strcmp(novo->area,v[j].nome)==0)
                break;
        }
        if(j==tam);
        else
        {
        novo->prox = lista;                      //O novo elemento é introduzido à cabeça da lista ligada
        lista = novo;}
    }
    fclose(f);
    inicio=lista;
    i=1;
    while(lista!=NULL)
    {
        lista->numero=i;
        lista=lista->prox;
        i++;
    }
    *serie=i;
    return inicio;
}




int lista_vazia(panimal p) {   //Devolve 0 se a lista ligada estiver vazia
    if (p == NULL)
        return 0;
    else
        return 1;
}


void showAnimais(panimal p, areas* v, int tam) {//Função 1ue faz listagem dos animais commpleta, por espécie e por área
    char op, especie[TAM], area[TAM];
    int existe = 0, cmp;
    while (op < '1' || op > '3') {
        printf("\n\nQual deseja que seja a listagem a fazer?\n\n1 - Completa\n2 - So de uma especie\n3 - So de uma area\n\nEscolha:");
        scanf("%c", &op);
    }
    switch (op) {
        case '1': while (p != NULL) {
                printf("\nNome:%s\nEspecie:%s\nNumero de serie:%d\n", p->nome, p->especie, p->numero);
                p = p->prox;
            }
            break;
        case '2':printf("Insira qual a especie que pretende listar:");
            scanf("%s", &especie);
            while (p != NULL) {
                if ((strcmp(p->especie, especie)) == 0) {
                    printf("\nNome:%s\nNumero de serie:%d\n", p->nome, p->numero);
                    existe++;
                }
                p = p->prox;
            }
            printf("\nExistem %d animais da especie %s neste zoo", existe, especie);
            break;
        case '3':for (;;) {
                cmp = 0;
                printf("\nInsira de que area pretende listar:");
                scanf("%s", &area);
                for (int i = 0; i < tam; i++) {
                    if ((strcmp(v[i].nome, area)) == 0)
                        cmp = 1;
                }
                if (cmp == 1)
                    break;
                printf("\aArea nao existe");
            }
            while (p != NULL) {
                if ((strcmp(p->area, area)) == 0) {
                    printf("\nNome:%s\nEspecie:%s\nNumero de serie:%d\n", p->nome, p->especie, p->numero);
                    existe++;
                }
                p = p->prox;
            }
            printf("\nExistem %d animais na area %s", existe, area);
            break;
    }
}

void showAnimal(panimal p) {//Função que mostra detalhadamente a informação sobre um animal
    int existe = 0, numero;
    panimal aux = p;
    lista(p);
    for (;;) {
        printf("\nInsira o numero do animal que pretende pesquisar:");
        scanf("%d", &numero);
        p = aux;
        while (p != NULL) {
            if (numero == p->numero) {
                existe = 1;
                break;
            }
            p = p->prox;
        }
        if (existe == 1)
            break;
        printf("\a\nNao existe nenhum animal desta especie com este numero\n");
    }
    printf("\nNome:%s\nEspecie:%s\nPeso:%d\nArea:%s\nNumero de Serie:%d", p->nome, p->especie, p->peso, p->area, p->numero);
    if (strcmp(p->pais[0], "\0") != 0)
        printf("\nPais:%s", p->pais[0]);
    if (strcmp(p->pais[1], "\0") != 0)
        printf("e %s", p->pais[1]);
    for (int i = 0; i < p->nfilhos; i++) {
        printf("\nFilho %d:%s", (i + 1), p->filhos[i]);
    }
}

panimal addAnimal(panimal p, areas *v, int tam, int *s) {//Função que adiciona um animal por ficheiro de texto ou por nascimento
    FILE * t;
    animals aux;
    panimal inicio = p, ant, novo, pai;
    char op = '0', nomeFich[TAM], nome[TAM], area[TAM];
    int erro = 1, conta = 0, i = 0, serie = *s, progenitor[2], concluido = 0, poss = 0;

    while (op > '3' || op < '1') {
        op = '0';
        printf("\nEscolha o metodo de insercao/adicao:\n\n1 - Atraves de ficheiro de texto\n2 - Por nascimento entre 2 progenitores\n3 - Nascimento com um progenitor\n\nEscolha:");
        scanf("%c", &op);
        fflush(stdin);
    }
    switch (op) {
        case '1':
            while (erro != 0) {
                printf("\nNome do ficheiro:");
                scanf("%s", &nomeFich);
                fflush(stdin);
                t = fopen(nomeFich, "r");
                if (t != NULL)
                    erro = 0;
                else;
            }
            while (fscanf(t, "%s %s %d %s", &aux.nome, &aux.especie, &aux.peso, &aux.area) == 4) {//Executa o ciclo enquanto conseguir ler todos os parâmetros necessários
                serie = *s;
                p = inicio;
                ant = p;
                while (p != NULL) {//Ciclo soma o total de peso já na area
                    if ((strcmp(aux.area, p->area)) == 0)
                        conta += p->peso;
                    ant = p;
                    p = p->prox;
                }
                for (i = 0; i < tam; i++) {
                    if ((strcmp(v[i].nome, aux.area)) == 0)
                        break;
                }
                if (i == tam) {
                    p = inicio;
                    ant = p;
                } else {
                    if (v[i].peso >= (conta + aux.peso)) {//Executa se o peso total na area somado com o do animal naosuperar o peso máximo
                        novo = malloc(sizeof (animals)*1);
                        strcpy(novo->nome, aux.nome);
                        strcpy(novo->especie, aux.especie);
                        strcpy(novo->area, aux.area);
                        novo->peso = aux.peso;
                        novo->numero = serie;
                        novo->nfilhos=0;
                        strcpy(novo->pais[0], "\0");
                        strcpy(novo->pais[1], "\0");
                        if (inicio == NULL) {
                            inicio = novo;
                            inicio->prox = NULL;
                        } else {
                            ant->prox = novo;
                            ant = ant->prox;
                            ant->prox = NULL;

                        }
                        *s = *s + 1;
                    } else {
                        for (;;) {
                            op = '0';
                            printf("\n\aA area nao pode suportar o %s!\n\nSelecione uma opcao:\n\n1 - Descartar animal\n2 - Escolher outra area\n", aux.nome);//A função dá a opção de o utilizador descartar um animal ou de o relocalizar
                            while (op < '1' || op > '2') {
                                op = '0';
                                printf("\nEscolha:");
                                scanf("%c", &op);
                                fflush(stdin);
                            }
                            if (op == '1')
                                break;
                            printf("\nAreas disponiveis:");
                            for (i = 0; i < tam; i++) {
                                p = inicio;
                                conta = 0;
                                while (p != NULL) {
                                    if (strcmp(v[i].nome, p->area) == 0)
                                        conta += p->peso;
                                    p = p->prox;
                                }
                                if (v[i].peso >= (conta + aux.peso))
                                    printf("%s/", v[i].nome);
                            }
                            printf("\n\nNome da nova area:");
                            scanf("%s", &nome);
                            fflush(stdin);
                            p = inicio;
                            for (i = 0; i < tam; i++) {
                                if (strcmp(nome, v[i].nome) == 0)
                                    break;
                            }
                            if (i == tam)
                                ;
                            else {
                                conta = 0;
                                while (p != NULL) {
                                    if (strcmp(p->area, v[i].nome) == 0)
                                        conta += p->peso;
                                    p = p->prox;
                                }
                                if (v[i].peso >= (conta + aux.peso)) {
                                    novo = malloc(sizeof (animals)*1);
                                    strcpy(novo->nome, aux.nome);
                                    strcpy(novo->especie, aux.especie);
                                    strcpy(novo->area, nome);
                                    novo->peso = aux.peso;
                                    novo->numero = serie;
                                    if (inicio == NULL) {
                                        inicio = novo;
                                        inicio->prox = NULL;
                                    } else {
                                        ant->prox = novo;
                                        ant = ant->prox;
                                        ant->prox = NULL;

                                    }
                                    *s = *s + 1;
                                    break;
                                } else
                                    ;

                            }
                        }
                    }
                }
            }
            return inicio;
            break;
        case '2':if (p == NULL) {                               //O programa verifica se existem animais no zoo. Se não a função termina                  
                printf("\n\aEste zoo ainda nao tem animais");
                return;
            }
            p = inicio;
            i = 0;
            int j = 0;
            for (;;) {
                p = inicio;
                for (; i < 0; i++) {
                    p = p->prox;
                }
                if (p == NULL)
                    break;
                j--;
                i = j;
                strcpy(p->especie, nome);
                strcpy(p->area, area);
                p = p->prox;
                while (p != NULL) {
                    if (strcmp(p->especie, nome) == 0 && strcmp(p->area, area) == 0) {
                        poss = 1;
                    }
                    p = p->prox;
                }
            }
            if (poss == 0) {
                printf("\n\aNao e possivel pois nao existem dois animais da mesma especie ou nao se encontram na mesma area");
                return inicio;
            }
            listaA(p);
            for (;;) {
                p = inicio;
                printf("\nIndique o numero do primeiro progenitor:");
                scanf("%d", &progenitor[0]);
                while (p != NULL) {
                    if (p->numero == progenitor[0]) {
                        if (p->nfilhos == 3) {
                            printf("\n\aO animal nao pode ter mais filhos");
                            break;
                        }
                        pai = p;
                        p = inicio;
                        printf("\nIndique o numero do segundo progenitor:");
                        scanf("%d", &progenitor[1]);
                        if (progenitor[0] == progenitor[1]) {
                            printf("\n\aUm animal nao consegue procriar sozinho");
                            break;
                        }
                        while (p != NULL) {
                            if (p->numero == progenitor[1]) {
                                if (p->nfilhos == 3) {
                                    printf("\n\aO animal nao pode ter mais filhos");
                                    erro = 1;
                                    break;
                                }
                                if (strcmp(pai->area, p->area) == 0) {
                                    if (strcmp(pai->especie, p->especie) == 0) {
                                        if ((novo = malloc(sizeof (animals)*1)) == NULL) {
                                            printf("\a\nErro na alocacao de memoria");
                                            break;
                                        }
                                        printf("\nInsira o nome do novo animal:");
                                        scanf("%s", &nome);
                                        strcpy(p->filhos[p->nfilhos], nome);
                                        strcpy(pai->filhos[pai->nfilhos], nome);
                                        p->nfilhos++;
                                        pai->nfilhos++;
                                        novo->peso = ((pai->peso)*0.2 + (p->peso)*0.2);
                                        strcpy(novo->pais[0], pai->nome);
                                        strcpy(novo->pais[1], p->nome);
                                        pai->peso *= 0.8;
                                        p->peso *= 0.8;
                                        while (p != NULL) {
                                            ant = p;
                                            p = p->prox;
                                        }

                                        strcpy(novo->nome, nome);
                                        strcpy(novo->especie, pai->especie);
                                        strcpy(novo->area, pai->area);
                                        novo->numero = serie;
                                        ant->prox = novo;
                                        ant = ant->prox;
                                        ant->prox = NULL;
                                        concluido = 1;
                                        *s = *s + 1;
                                        break;
                                    }
                                    printf("\n\aEsta funcionalidade nao esta disponivel para hibridos");
                                    break;
                                }
                                printf("\n\aOs animais tem de pertencer a mesma area");
                                break;
                            }
                            p = p->prox;
                        }
                        if (erro = 1)
                            break;
                    }
                    if (concluido == 1)
                        break;
                    p = p->prox;
                }
                if (concluido == 1)
                    break;
            }
            return inicio;
            break;
        case '3':if (p == NULL) {                                       //Função funciona da mesma forma que a de nascimento com dois progenitores, mas apenas com umm
                printf("\n\aEste zoo ainda nao tem animais");
                return;
            }
            listaA(p);
            for (;;) {
                p = inicio;
                printf("\nIndique o numero do primeiro progenitor:");
                scanf("%d", &progenitor[0]);
                while (p != NULL) {
                    if (p->numero == progenitor[0])
                        break;
                    p = p->prox;
                }
                if (p->nfilhos == 3) {
                    printf("\n\aO animal não pode ter mais filhos");
                } else {
                    if (p != NULL)
                        break;
                }
            }
            if ((novo = malloc(sizeof (animals)*1)) == NULL) {
                printf("\a\nErro na alocacao de memoria");
                break;
            }
            printf("\nInsira o nome do novo animal:");
            scanf("%s", &nome);
            strcpy(p->filhos[p->nfilhos], nome);
            p->nfilhos++;
            novo->peso = (p->peso)*0.2;
            strcpy(novo->pais[0], p->nome);
            p->peso *= 0.8;
            strcpy(novo->especie, p->especie);
            strcpy(novo->area, p->area);
            while (p != NULL) {
                ant = p;
                p = p->prox;
            }
            strcpy(novo->nome, nome);
            novo->numero = serie;
            ant->prox = novo;
            ant = ant->prox;
            ant->prox = NULL;
            *s = *s + 1;
            return inicio;
            break;
    }
}

void saveAnimal(panimal p) {                        //Função que guarda os animais num ficheiro binario de nome Animais.bat
    FILE *f;
    int conta = 0;
    f = fopen("Animais.dat", "wb");
    if (f == NULL) {
        printf("\n\aErro ao criar ficheiro\n");
        return;
    } else {
        while (p != NULL) {
            fwrite(p, sizeof (animals), 1, f);
            p = p->prox;
            conta++;
        }
        fwrite(&conta, sizeof (int), 1, f);
        fclose(f);
    }
}

panimal delAnimal(panimal p, int *s) {                                  //Função que elimina um animal
    int numero;
    panimal inicio = p, ant;
    char nome[TAM];
    for (;;) {
        ant = NULL;
        p = inicio;
        lista(p);
        printf("\nInsira o numero do animal que pretende eliminar:");
        scanf("%d", &numero);
        while (p != NULL) {
            if (p->numero == numero)
                break;
            ant = p;
            p = p->prox;
        }
        if (p != NULL)
            break;
        printf("\n\aNao existe nenhum animal com esse numero de serie\n");
    }
    if (ant == NULL) {
        strcpy(nome, inicio->nome);
        ant = inicio;
        inicio = inicio->prox;
        free(ant);
        p = inicio;
        while (p != NULL) {
            p->numero--;                            //Quando eliminado o animal o numero de serie de todos os animais seguintes é decrementado por uma questão de organização
            for (int i = 0; i < 2; i++) {
                if (strcmp(p->pais[i], nome) == 0)
                    strcpy(p->pais[i], "\0");
            }
            for (int i = 0; i < p->nfilhos; i++) {
                if (strcmp(p->filhos[i], nome) == 0) {
                    strcpy(p->filhos[i], "\0");
                    p->nfilhos--;
                    break;
                }
            }
            p = p->prox;
        }
        *s = *s - 1;
        return inicio;
    } else {
        strcpy(nome, p->nome);
        ant->prox = p->prox;
        free(p);
        p = inicio;
        while (p != NULL) {
            if (p->numero > numero)
                p->numero--;
            for (int i = 0; i < 2; i++) {
                if (strcmp(p->pais[i], nome) == 0)
                    strcpy(p->pais[i], "\0");
            }
            for (int i = 0; i < p->nfilhos; i++) {
                if (strcmp(p->filhos[i], nome) == 0) {
                    strcpy(p->filhos[i], "\0");
                    p->nfilhos--;
                    break;
                }
            }
            p = p->prox;
        }
        *s = *s - 1;
        return inicio;
    }
}

void transferir(panimal p, areas *v, int tam) {//Função que altera a area de um animal
    int numero, disp = 0, conta, i, peso, j;
    panimal inicio = p, muda;
    char nome[TAM];
    lista(p);
    for (;;) {
        p = inicio;
        printf("\nInsira o numero do animal que pretende mover:");
        scanf("%d", &numero);
        while (p != NULL) {
            if (numero == p->numero)
                break;
            p = p->prox;
        }
        if (p != NULL)
            break;
        printf("\n\aNao existe nenhum animal com esse numero\n");
    }
    muda = p;
    peso = p->peso;
    strcpy(nome, p->area);
    listAreas(tam, v);
    for (;;) {
        printf("\n\nNome da nova area:");
        scanf("%s", &nome);
        p = inicio;
        for (j = 0; j < tam; j++) {
            if (strcmp(nome, v[j].nome) == 0)
                break;
        }
        if (i == tam)
            ;
        else {
            for (i = 0; i < tam; i++) {
                if (strcmp(muda->area, v[i].nome) == 0)
                    break;
            }
            conta = 0;
            while (p != NULL) {
                if (strcmp(p->area, v[i].nome) == 0)
                    conta += p->peso;
                p = p->prox;
            }
            if (v[j].peso >= (conta + peso)) {
                if (strcmp(v[i].perto[0], v[j].nome) == 0) {
                    strcpy(muda->area, v[j].nome);
                    return;
                }
                if (strcmp(v[i].perto[1], v[j].nome) == 0) {
                    strcpy(muda->area, v[j].nome);
                    return;
                }
                if (strcmp(v[i].perto[0], v[j].nome) == 0) {
                    strcpy(muda->area, v[j].nome);
                    return;
                }

            } else
                ;

        }
    }
}

int main(int argc, char** argv) {
    fullscreen();
    int tam, erro, serie;
    char op;
    panimal lista = NULL;
    areas *vetorArea;
    serie++;
    tam = gettam();
    if (tam < 1)
        return 0;
    vetorArea = getVetor(tam);
    if (vetorArea == NULL) {
        printf("\a\nErro ao criar o vetor dinâmico de estruturas de areas");
        return 0;
    }
    erro = getArea(vetorArea);
    if (erro == 0) {
        printf("\a\nErro ao armazenar áreas\nPrograma encerrado\n");
        return 0;
    }
    lista = fillLista(&serie, vetorArea, tam);
    if (lista == NULL) {
        printf("O zoo está vazio\n");
        serie = 1;                              //Se o zoo não tem animais o indicador de número de serie é posto a 1
    }
    for (;;) {
        op = 'X';
        while (op > '9' || op < '0') {
            printf("\n\n1 - Listar animais\n");
            printf("2 - Mostrar detalhes um animal\n");
            printf("3 - Eliminar um animal\n");
            printf("4 - Adicionar um animal\n");
            printf("5 - Transferir um animal\n");
            printf("6 - Listar areas (nomes)\n");
            printf("7 - Adicionar area\n");
            printf("8 - Mostrar caracteristicas de uma area\n");
            printf("9 - Eliminar area\n");
            printf("0 - Encerrar o programa\n\n");
            printf("Selecione uma opcao: ");
            scanf(" %c", &op);
            fflush(stdin);
        }
        switch (op) {
            case '1':if (lista == NULL)
                    printf("\a\nEste zoo ainda nao tem animais\n");
                else
                    showAnimais(lista, vetorArea, tam);
                break;
            case '2':if (lista == NULL)
                    printf("\a\nEste zoo ainda nao tem animais\n");
                else
                    showAnimal(lista);
                break;
            case '3':if (lista == NULL)
                    printf("\a\nEste zoo ainda nao tem animais\n");
                lista = delAnimal(lista, &serie);
                break;
            case '4':lista = addAnimal(lista, vetorArea, tam, &serie);
                break;
            case '5':if (lista == NULL) {
                    printf("\a\nEste zoo ainda nao tem animais\n");
                    break;
                }
                if (tam == 1) {
                    printf("\n\aEste zoo apenas tem uma area");
                    break;
                }
                transferir(lista, vetorArea, tam);
                break;
            case '6':listAreas(tam, vetorArea);
                break;
            case '7':vetorArea = addArea(vetorArea, &tam);
                break;
            case '8':carArea(vetorArea, tam);
                break;
            case '9':if (tam == 1) {
                    printf("\a\nNao pode apagar areas pois so resta uma area");
                    break;
                }
                vetorArea = delArea(vetorArea, lista, &tam);
                break;
            case '0':saveArea(vetorArea, tam);
                if (lista != NULL)
                    saveAnimal(lista);
                free(vetorArea);
                return 0;
                break;
        }
    }
}

