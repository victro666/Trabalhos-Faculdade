#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <vector>
#include <algorithm>
#include <string.h>
#include "MTRand.h"
#include "BRKGA.h"
#include "PMMDecoder.h"

#define MAX_ITENS 2000
#define MAX_PARES 8000

int num_itens;
int num_pares;
double cap_mochila;

double vet_lucros[MAX_ITENS];
double vet_pesos[MAX_ITENS];
int vet_par_i[MAX_PARES];
int vet_par_j[MAX_PARES];
double vet_custo_penalidade[MAX_PARES];

typedef struct {
    double melhor_fo;
    double tempo_total;
    double tempo_melhor; 
} StatsRodada;

StatsRodada rodar_brkga(unsigned p, double pe, double pm, double rho, unsigned K, unsigned MAXT, long unsigned seed, double tempo_limite, unsigned max_gen);
void ler_dados(const char* arq);
void salvar_arquivo_solucao(const char* nome_instancia, double melhor_fo, double media_fo, double desvio, double tempo_medio, double melhor_tempo);

int main(int argc, char* argv[]) {
    
    if (argc < 2) {
        printf("Uso: %s <instancia> parametros ex: --pop 999 --elite 0.999 ...\n", argv[0]);
        return 1;
    }

    //teste parametros sem irace
    unsigned p = 1000; //populacao
    double pe = 0.20; //pop elite
    double pm = 0.15; //pop mutantes
    double rho = 0.70; //percentual herenca elite
    unsigned K = 1; //pop
    unsigned MAXT = 4; //threads
    long unsigned semente_base = 1;
    double tempo_limite = 300.0; //tempo limite 5 minutos
    unsigned max_gen = 1000000;
    int modo_irace = 0;

    for(int i=1; i<argc; i++) {
        if(strcmp(argv[i], "--pop")==0) p = atoi(argv[i+1]);
        else if(strcmp(argv[i], "--elite")==0) pe = atof(argv[i+1]);
        else if(strcmp(argv[i], "--mutant")==0) pm = atof(argv[i+1]);
        else if(strcmp(argv[i], "--rho")==0) rho = atof(argv[i+1]);
        else if(strcmp(argv[i], "--seed")==0) semente_base = atoi(argv[i+1]);
        else if(strcmp(argv[i], "--time")==0) tempo_limite = atof(argv[i+1]);
        else if(strcmp(argv[i], "--threads")==0) MAXT = atoi(argv[i+1]);
        else if(strcmp(argv[i], "--irace")==0) modo_irace = 1;
    }

    ler_dados(argv[1]);

    if(modo_irace) {
        StatsRodada r = rodar_brkga(p, pe, pm, rho, K, MAXT, semente_base, tempo_limite, max_gen);
        printf("%.2f\n", -r.melhor_fo);
        return 0;
    }

    printf("Instancia: %s\n", argv[1]);
    
    double soma_fo = 0.0;
    double soma_tempo = 0.0;
    double melhor_tempo = 0.0;
    double melhor_fo_global = -1e15;

    for(int i=0; i<3; i++) {
        long unsigned seed = semente_base + i;
        printf("[teste %d/3] Seed %lu - ", i+1, seed);
        fflush(stdout);

        StatsRodada r = rodar_brkga(p, pe, pm, rho, K, MAXT, seed, tempo_limite, max_gen);
        printf("FO: %.2f (%.2fs)\n", r.melhor_fo, r.tempo_total);

        soma_fo += r.melhor_fo;
        soma_tempo += r.tempo_total;

        if(r.melhor_fo > melhor_fo_global){
            melhor_fo_global = r.melhor_fo;
            melhor_tempo = r.tempo_total;
        }
    }

    double media_fo = soma_fo / 3.0;
    double media_tempo = soma_tempo / 3.0;
    double desvio = (fabs(media_fo - melhor_fo_global) / melhor_fo_global) * 100.0;

    printf("\nRESULTADOS\n");

    //const char* nome = strrchr(argv[1], '/');
    //if(!nome) nome = argv[1]; else nome++;

    const char* nome_base = argv[1];
    for(const char* p = argv[1]; *p; p++) {
        if (*p == '/' || *p == '\\') {
            nome_base = p + 1;
        }
    }

    printf("Instancia = %s; MelhorFO = %.2f; MediaFO = %.2f; Desvio(%%) = %.4f; TempoMedio = %.2f; TempoMelhor = %.2f\n", 
           nome_base, melhor_fo_global, media_fo, desvio, media_tempo, melhor_tempo);

    salvar_arquivo_solucao(nome_base, melhor_fo_global, media_fo, desvio, media_tempo, melhor_tempo);

    return 0;
}

StatsRodada rodar_brkga(unsigned p, double pe, double pm, double rho, 
                        unsigned K, unsigned MAXT, long unsigned seed, 
                        double tempo_limite, unsigned max_gen) {
    
    StatsRodada stats;
    PMMDecoder decoder;
    MTRand rng(seed);

    BRKGA<PMMDecoder, MTRand> algorithm(num_itens, p, pe, pm, rho, decoder, rng, K, MAXT);

    clock_t inicio = clock();
    unsigned geracao = 0;
    
    double fo_maxima = -1e15;
    stats.tempo_melhor = 0;

    while (geracao < max_gen) {
        double decorrido = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        if (decorrido >= tempo_limite) 
            break;

        algorithm.evolve();

        double fo_atual = -algorithm.getBestFitness();
        
        if (fo_atual > fo_maxima) {
            fo_maxima = fo_atual;
            stats.tempo_melhor = decorrido;
        }
        geracao++;
    }

    //printf("passei aqui\n");

    stats.melhor_fo = fo_maxima;
    stats.tempo_total = (double)(clock() - inicio) / CLOCKS_PER_SEC;

    return stats;
}

void salvar_arquivo_solucao(const char* nome_instancia, double melhor_fo, double media_fo, double desvio, double tempo_medio, double melhor_tempo) {

    //printf("passei aqui\n");

    char nome_arq[256];
    
    sprintf(nome_arq, "resultados_instancia_%s", nome_instancia);
    FILE* f = fopen(nome_arq, "w");
    if(!f){
        printf("Erro ao criar o arquivo.\n");
        return;
    }

    fprintf(f, "Instancia: %s\n", nome_instancia);
    fprintf(f, "Melhor FO: %.2f; Media FO: %.2f; Desvio: %.4f\n", melhor_fo, media_fo, desvio);
    fprintf(f, "Tempo Medio: %.2f; Melhor Tempo: %.2f\n", tempo_medio, melhor_tempo);

    fprintf(f, "\n");
    fclose(f);
}

void ler_dados(const char* arq) {
    FILE* f = fopen(arq, "r");
    if (!f) {
        printf("Nao foi possivel abrir o arquivo %s\n", arq);
        exit(1);
    }

    fscanf(f, "%d %d %lf", &num_itens, &num_pares, &cap_mochila);

    for (int i = 0; i < num_itens; i++) 
        fscanf(f, "%lf", &vet_lucros[i]);
    for (int i = 0; i < num_itens; i++) 
        fscanf(f, "%lf", &vet_pesos[i]);

    int lixo;
    for (int k = 0; k < num_pares; k++) {
        fscanf(f, "%d %lf %d", &lixo, &vet_custo_penalidade[k], &lixo);
        fscanf(f, "%d %d", &vet_par_i[k], &vet_par_j[k]);
    }
    fclose(f);
}