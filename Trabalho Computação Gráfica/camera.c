#include <stdio.h>
#include <SDL2/SDL.h>
#include "camera.h"
#include "algebra.h"

//Aloca a estrutura de mem�ria para representar a c�mera


tCamera3d *criaCamera(){
    tCamera3d *cam = (tCamera3d *) malloc(sizeof(tCamera3d));
    cam->posicao = (float *) calloc(3, sizeof(float));
    cam->foco = (float *) calloc(3, sizeof(float));
    cam->cima = (float *) calloc(3, sizeof(float));
    
    cam->viewMatrix = (float **) malloc(4 * sizeof(float *));
    for(int i = 0; i < 4; i++) cam->viewMatrix[i] = (float *) malloc(4 * sizeof(float));
    criaIdentidade4d(cam->viewMatrix);
    
    return cam;
}

void defineCamera(tCamera3d *camera, float posX, float posY, float posZ, float focX, float focY, float focZ, float cimX, float cimY, float cimZ){
    // Preenche vetores da câmera
    camera->posicao[0] = posX; camera->posicao[1] = posY; camera->posicao[2] = posZ;
    camera->foco[0] = focX; camera->foco[1] = focY; camera->foco[2] = focZ;
    camera->cima[0] = cimX; camera->cima[1] = cimY; camera->cima[2] = cimZ;

    criaIdentidade4d(camera->viewMatrix);
    camera->viewMatrix[0][3] = -posX;
    camera->viewMatrix[1][3] = -posY;
    camera->viewMatrix[2][3] = -posZ;
}

void desalocaCamera(tCamera3d *camera){
    free(camera->posicao); free(camera->foco); free(camera->cima);
    for(int i=0; i<4; i++) free(camera->viewMatrix[i]);
    free(camera->viewMatrix);
    free(camera);
}