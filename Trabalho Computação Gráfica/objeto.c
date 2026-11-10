#include <stdio.h>
#include <SDL2/SDL.h>
#include "objeto.h"

tObjeto3d *carregaObjeto(char *nomeArquivo){
    FILE *f = fopen(nomeArquivo, "r");
    if(!f) return NULL;

    tObjeto3d *obj = (tObjeto3d *) malloc(sizeof(tObjeto3d));
    
    fscanf(f, "%d", &obj->nPontos);
    obj->pontos = (float **) malloc(obj->nPontos * sizeof(float *));
    for(int i = 0; i < obj->nPontos; i++){
        obj->pontos[i] = (float *) malloc(3 * sizeof(float));
        fscanf(f, "%f %f %f", &obj->pontos[i][0], &obj->pontos[i][1], &obj->pontos[i][2]);
    }

    fscanf(f, "%d", &obj->nArestas);
    obj->arestas = (int **) malloc(obj->nArestas * sizeof(int *));
    for(int i = 0; i < obj->nArestas; i++){
        obj->arestas[i] = (int *) malloc(2 * sizeof(int));
        fscanf(f, "%d %d", &obj->arestas[i][0], &obj->arestas[i][1]);
    }
    fclose(f);

    obj->transl = (float *) calloc(3, sizeof(float));
    obj->escala = (float *) calloc(3, sizeof(float));
    for(int i=0; i<3; i++) obj->escala[i] = 1.0f; // Escala inicial = 1
    obj->Rx = 0; obj->Ry = 0; obj->Rz = 0;

    obj->modelMatrix = (float **) malloc(4 * sizeof(float *));
    for(int i = 0; i < 4; i++) obj->modelMatrix[i] = (float *) malloc(4 * sizeof(float));
    criaIdentidade4d(obj->modelMatrix);

    return obj;
}

void atualizaModelMatrix(tObjeto3d *obj) {
    // Recalcula a matriz modelo com base nos dados atuais
    criaIdentidade4d(obj->modelMatrix);
}

void escalaObjeto(tObjeto3d *objeto, float escalaX, float escalaY, float escalaZ){
    float **S = (float **) malloc(4 * sizeof(float *));
    for(int i=0; i<4; i++) S[i] = (float *) calloc(4, sizeof(float));
    S[0][0] = escalaX; S[1][1] = escalaY; S[2][2] = escalaZ; S[3][3] = 1.0f;
    multMatriz4d(S, objeto->modelMatrix);
    for(int i=0; i<4; i++) free(S[i]); free(S);
}

void transladaObjeto(tObjeto3d *objeto, float transX, float transY, float transZ){
    float **T = (float **) malloc(4 * sizeof(float *));
    for(int i=0; i<4; i++) T[i] = (float *) calloc(4, sizeof(float));
    criaIdentidade4d(T);
    T[0][3] = transX; T[1][3] = transY; T[2][3] = transZ;
    multMatriz4d(T, objeto->modelMatrix);
    for(int i=0; i<4; i++) free(T[i]); free(T);
}

void rotacionaObjetoEixoX(tObjeto3d *objeto, float angulo){
    float rad = angulo * (3.14159f / 180.0f);
    float **R = (float **) malloc(4 * sizeof(float *));
    for(int i=0; i<4; i++) R[i] = (float *) calloc(4, sizeof(float));
    criaIdentidade4d(R);
    R[1][1] = cos(rad); R[1][2] = -sin(rad);
    R[2][1] = sin(rad); R[2][2] = cos(rad);
    multMatriz4d(R, objeto->modelMatrix);
    for(int i=0; i<4; i++) free(R[i]); free(R);
}

void rotacionaObjetoEixoY(tObjeto3d *objeto, float angulo){
    float rad = angulo * (3.14159f / 180.0f);
    float **R = (float **) malloc(4 * sizeof(float *));
    for(int i=0; i<4; i++) R[i] = (float *) calloc(4, sizeof(float));
    criaIdentidade4d(R);
    R[0][0] = cos(rad); R[0][2] = sin(rad);
    R[2][0] = -sin(rad); R[2][2] = cos(rad);
    multMatriz4d(R, objeto->modelMatrix);
    for(int i=0; i<4; i++) free(R[i]); free(R);
}

void rotacionaObjetoEixoZ(tObjeto3d *objeto, float angulo){
    float rad = angulo * (3.14159f / 180.0f);
    float **R = (float **) malloc(4 * sizeof(float *));
    for(int i=0; i<4; i++) R[i] = (float *) calloc(4, sizeof(float));
    criaIdentidade4d(R);
    R[0][0] = cos(rad); R[0][1] = -sin(rad);
    R[1][0] = sin(rad); R[1][1] = cos(rad);
    multMatriz4d(R, objeto->modelMatrix);
    for(int i=0; i<4; i++) free(R[i]); free(R);
}

void imprimeObjetoDBG(tObjeto3d *objeto){
    printf("Objeto com %d pontos e %d arestas.\n", objeto->nPontos, objeto->nArestas);
}

void desalocaObjeto(tObjeto3d *objeto){
    for(int i=0; i<objeto->nPontos; i++) free(objeto->pontos[i]);
    free(objeto->pontos);
    for(int i=0; i<objeto->nArestas; i++) free(objeto->arestas[i]);
    free(objeto->arestas);
    for(int i=0; i<4; i++) free(objeto->modelMatrix[i]);
    free(objeto->modelMatrix);
    free(objeto->transl); free(objeto->escala);
    free(objeto);
}
