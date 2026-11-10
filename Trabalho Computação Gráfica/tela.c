#include <stdio.h>
#include <SDL2/SDL.h>
#include "tela.h"

//Aloca a tela utilizando o t�tulo passado como par�metro e as largura e altura definidas no tela.h
SDL_Window *criaTela(char *titulo){
    return SDL_CreateWindow( titulo, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT, SDL_WINDOW_ALLOW_HIGHDPI);
}

/*SDL_Window *criaTela(char *titulo){
    // Cria tela com 800 e 600 [cite: 72, 74]
    return SDL_CreateWindow(titulo, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);
}*/

//Desenha um segmento de reta na tela
void desenhaArestaTela(SDL_Renderer *renderer, float *ponto1, float *ponto2){
    int p1x, p1y, p2x, p2y;

    p1x = ((ponto1[0])+1) * WIDTH/2;
    p1y = (1-ponto1[1]) * HEIGHT/2;
    p2x = ((ponto2[0])+1) * WIDTH/2;
    p2y = (1-ponto2[1]) * HEIGHT/2;

    SDL_RenderDrawLine(renderer, p1x, p1y, p2x, p2y);

    return;
}

/*void desenhaArestaTela(SDL_Renderer *renderer, float *ponto1, float *ponto2){
    // Transformação Viewport (semelhante à sua colaTrabalho.c)
    // Perspectiva simples: divisão por Z para dar profundidade
    float z1 = ponto1[2] < 0.1f ? 0.1f : ponto1[2]; 
    float z2 = ponto2[2] < 0.1f ? 0.1f : ponto2[2]; 

    float x1 = ponto1[0] / z1; float y1 = ponto1[1] / z1;
    float x2 = ponto2[0] / z2; float y2 = ponto2[1] / z2;

    int xa = WIDTH * ((x1 + 1.0f) / 2.0f);
    int xb = WIDTH * ((x2 + 1.0f) / 2.0f);
    int ya = HEIGHT * ((-y1 + 1.0f) / 2.0f);
    int yb = HEIGHT * ((-y2 + 1.0f) / 2.0f);

    SDL_RenderDrawLine(renderer, xa, ya, xb, yb);
}*/



//Desaloca a tela
void desalocaTela(SDL_Window *window){
    SDL_DestroyWindow(window);
}
