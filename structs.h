#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#define TAM 50

typedef struct animal animals, *panimal;
typedef struct area areas;

struct area {
    char nome[TAM];
    int peso;
    int proximos;
    char perto[3][TAM];
};

struct animal {
    char nome[TAM];
    char especie[TAM];
    int peso;
    char area[TAM];
    char pais[2][TAM];
    char filhos[3][TAM];
    int nfilhos;
    int numero;
    panimal prox;
};

