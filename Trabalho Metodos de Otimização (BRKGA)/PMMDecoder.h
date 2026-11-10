#ifndef PMMDecoder_H
#define PMMDecoder_H

#include <vector>
#include <algorithm>
#include <math.h>
#include "header.h"

class PMMDecoder {
public:
    PMMDecoder() { }
    ~PMMDecoder() { }

    double decode(const std::vector<double>& chromosome) const {
        
        std::vector<std::pair<double, int> > rank(num_itens);

        for(int i = 0; i < num_itens; ++i) {
            rank[i] = std::make_pair(chromosome[i], i);
        }

        std::sort(rank.rbegin(), rank.rend());

        double peso_atual = 0.0;
        double lucro_bruto = 0.0;
        
        int escolhidos[MAX_ITENS]; 
        for(int i = 0; i < num_itens; i++) 
            escolhidos[i] = 0;

        for(int k = 0; k < num_itens; ++k) {
            int item_idx = rank[k].second; 
            double peso_item = vet_pesos[item_idx];

            if (peso_atual + peso_item <= cap_mochila) {
                escolhidos[item_idx] = 1;
                peso_atual += peso_item;
                lucro_bruto += vet_lucros[item_idx];
            }
        }

        double penalidade_total = 0.0;
        for(int k = 0; k < num_pares; ++k) {
            int i = vet_par_i[k];
            int j = vet_par_j[k];

            if(escolhidos[i] == 1 && escolhidos[j] == 1) {
                penalidade_total += vet_custo_penalidade[k];
            }
        }

        double fo_real = lucro_bruto - penalidade_total;

        return -fo_real; 
    }
};

#endif