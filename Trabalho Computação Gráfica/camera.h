#include <stdio.h>
#include <SDL2/SDL.h>

#ifndef tCam
#define tCam

typedef struct _tCam{
	float *posicao;
	float *foco;
	float *cima;
	float **viewMatrix;
} tCamera3d;
#endif // tCam


//Aloca a estrutura de mem�ria para representar a c�mera
tCamera3d *criaCamera();

//Recebe os par�metros espec�ficos da c�mera e calcula viewMatrix
void defineCamera(tCamera3d *camera, float posX, float posY, float posZ, float focX, float focY, float focZ, float cimX, float cimY, float cimZ);

//Desaloca a c�mera
void desalocaCamera(tCamera3d *objeto);
