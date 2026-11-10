#include <stdio.h>

#include "algebra.h"
#include <stdlib.h>

void criaIdentidade4d(float **novaMatriz){
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            novaMatriz[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void imprimeMatriz4dDBG(float **matriz){
    printf("\nMatriz 4x4:\n");
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            printf("%6.2f ", matriz[i][j]);
        }
        printf("\n");
    }
}

float *multMatriz4dPonto(float **matriz, float *ponto){
    // Ponto em 3D vira vetor 4D (x, y, z, 1) para calculo
    float *resultado = (float *) malloc(4 * sizeof(float));
    float p4[4] = {ponto[0], ponto[1], ponto[2], 1.0f};

    for(int i = 0; i < 4; i++){
        resultado[i] = 0;
        for(int j = 0; j < 4; j++){
            resultado[i] += matriz[i][j] * p4[j];
        }
    }
    return resultado;
}

void multMatriz4d(float **matrizA, float **matrizB){
    // Multiplica A x B e salva o resultado em B
    float temp[4][4];
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            temp[i][j] = 0;
            for(int k = 0; k < 4; k++){
                temp[i][j] += matrizA[i][k] * matrizB[k][j];
            }
        }
    }
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            matrizB[i][j] = temp[i][j];
        }
    }
}