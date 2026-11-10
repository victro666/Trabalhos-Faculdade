#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "objeto.h"
#include "tela.h"
#include "algebra.h"
#include "camera.h"

//desenha um objeto na tela
void desenhaObjetoTela(SDL_Renderer *renderer, tCamera3d *cam, tObjeto3d *objeto){
    for(int i = 0; i < objeto->nArestas; i++){
        int ind1 = objeto->arestas[i][0];
        int ind2 = objeto->arestas[i][1];

        float *p1_mod = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[ind1]);
        float *p2_mod = multMatriz4dPonto(objeto->modelMatrix, objeto->pontos[ind2]);

        float *p1_view = multMatriz4dPonto(cam->viewMatrix, p1_mod);
        float *p2_view = multMatriz4dPonto(cam->viewMatrix, p2_mod);

        desenhaArestaTela(renderer, p1_view, p2_view);

        free(p1_mod); free(p2_mod);
        free(p1_view); free(p2_view);
    }
}


int main( int argc, char * argv[] ){

    if (SDL_Init( SDL_INIT_EVERYTHING) < 0){
        printf("SDL n�o inicializou! SDL Erro: %s\n", SDL_GetError());
    }

    SDL_Window *window = criaTela("Hello SDL World!");

    if(window == NULL){
        printf("SDL n�o criou a janela! SDL Erro: %s\n", SDL_GetError());
        return EXIT_FAILURE;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_Event windowEvent;

    tObjeto3d *cubo = carregaObjeto("cubo.dcg");
    if(!cubo) {
        printf("Erro ao carregar o cubo.dcg!\n");
        return EXIT_FAILURE;
    }
    
    // Escala inicial para caber na tela
    escalaObjeto(cubo, 0.1f, 0.1f, 0.1f);

    tCamera3d *cam = criaCamera();
    defineCamera(cam, 0.0f, 0.0f, -5.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    float angulo = 1.0f; // Rotação contínua

    while(1){
        if( SDL_PollEvent(&windowEvent)){
            if(windowEvent.type == SDL_QUIT){
                break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

        //ESCREVA AQUI O SEU PROGRAMA
        rotacionaObjetoEixoX(cubo, angulo);
        rotacionaObjetoEixoY(cubo, angulo);
        desenhaObjetoTela(renderer, cam, cubo);
        SDL_Delay(16);
        SDL_RenderPresent(renderer);
    }

    desalocaTela(window);
    SDL_Quit();

    return EXIT_SUCCESS;
}
