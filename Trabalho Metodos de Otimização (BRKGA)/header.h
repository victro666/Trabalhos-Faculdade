#ifndef header_H
#define header_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_ITENS 2000
#define MAX_PARES 8000

extern int num_itens;
extern int num_pares;
extern double cap_mochila;

extern double vet_lucros[MAX_ITENS];
extern double vet_pesos[MAX_ITENS];
extern int vet_par_i[MAX_PARES];
extern int vet_par_j[MAX_PARES];
extern double vet_custo_penalidade[MAX_PARES];

#endif