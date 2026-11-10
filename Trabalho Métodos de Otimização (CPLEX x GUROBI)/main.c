#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>
#include <time.h>

#include "header.h"


int main() {

    const char* lista_instancias[] = {
        "01_id_101_objs_500_size_1500_sets_3000_maxNumConflicts_2_maxCost_15_seme_2097.txt",
        "01_id_106_objs_700_size_2100_sets_4200_maxNumConflicts_2_maxCost_15_seme_1107.txt",
        "01_id_111_objs_800_size_2400_sets_4800_maxNumConflicts_2_maxCost_15_seme_1719.txt",
        "02_id_102_objs_500_size_1500_sets_3000_maxNumConflicts_2_maxCost_15_seme_1518.txt",
        "02_id_107_objs_700_size_2100_sets_4200_maxNumConflicts_2_maxCost_15_seme_2739.txt",
        "02_id_112_objs_800_size_2400_sets_4800_maxNumConflicts_2_maxCost_15_seme_1579.txt"
    };

    int total_instancias = 6; 

    double soma_linf_c = 0, soma_lsup_c = 0, soma_gap_c = 0, soma_tempo_c = 0;
    double soma_linf_g = 0, soma_lsup_g = 0, soma_gap_g = 0, soma_tempo_g = 0;
    int qtd_sucesso_gurobi = 0;

    FILE* f = fopen("resultados.txt", "w");
    if (f) {
        fprintf(f, "Instancia;LB_Cplex;UB_Cplex;GAP_Cplex;Tempo_Cplex;LB_Gurobi;UB_Gurobi;GAP_Gurobi;Tempo_Gurobi\n");
        fclose(f);
    }

    printf("(%d instancias) ===\n", total_instancias);

    for (int i = 0; i < total_instancias; i++) {
        const char* nome_arq = lista_instancias[i];
        
        printf("\n--------------------------------------------------\n");
        printf("lendo [%d/%d] tempo de execucao 60 segundos: %s\n", i+1, total_instancias, nome_arq);

        lerInstancia(nome_arq);
        gerarModeloLP();

        printf("Resolvendo CPLEX\n");
        Solucao sol_cplex;
        memset(&sol_cplex, 0, sizeof(Solucao)); 
        solverCPLEX(&sol_cplex);

        printf("Resolvendo GUROBI\n");
        Solucao sol_gurobi;
        memset(&sol_gurobi, 0, sizeof(Solucao)); 
        solverGurobi(&sol_gurobi);

        salvarLinhaTabela(nome_arq, &sol_cplex, &sol_gurobi);

        if (sol_gurobi.erro_gurobi) {
            printf("ERRO (Licenca/Tamanho da instancia) - Pulando leitura e execucao do GUROBI\n");
        }

        soma_linf_c += sol_cplex.lim_inf;
        soma_lsup_c += sol_cplex.lim_sup;
        soma_gap_c  += sol_cplex.gap;
        soma_tempo_c += sol_cplex.tempo;

        if (!sol_gurobi.erro_gurobi) {
            soma_linf_g += sol_gurobi.lim_inf;
            soma_lsup_g += sol_gurobi.lim_sup;
            soma_gap_g  += sol_gurobi.gap;
            soma_tempo_g += sol_gurobi.tempo;
            qtd_sucesso_gurobi++;
        }
    }

    int div_g = (qtd_sucesso_gurobi > 0) ? qtd_sucesso_gurobi : 1;

    printf("--------------------------------------------------\n");
    printf("MÉDIA das Soluções\n");
    printf("MEDIA CPLEX; %.2f; %.2f; %.4f%%; %.4f;\n", (soma_linf_c / total_instancias), (soma_lsup_c / total_instancias), (soma_gap_c / total_instancias) * 100.0, 
            (soma_tempo_c / total_instancias));
    printf("MEDIA GUROBI; %.2f; %.2f; %.4f%%; %.4f;\n",(soma_linf_g / div_g), (soma_lsup_g / div_g), (soma_gap_g / div_g) * 100.0, 
            (soma_tempo_g / div_g));

    FILE* f_med = fopen("resultados.txt", "a");
    if (f_med) {
        fprintf(f_med, "MEDIA;%.2f;%.2f;%.4f%%;%.4f;%.2f;%.2f;%.4f%%;%.4f\n",
            soma_linf_c / total_instancias, 
            soma_lsup_c / total_instancias, 
            (soma_gap_c / total_instancias) * 100.0, 
            soma_tempo_c / total_instancias,
            
            soma_linf_g / div_g, 
            soma_lsup_g / div_g, 
            (soma_gap_g / div_g) * 100.0, 
            soma_tempo_g / div_g);
        fclose(f_med);
    }

    printf("arquivo gerado - resultados.txt'.\n");
    return 0;
}

void salvarLinhaTabela(const char* instName, Solucao *cplex, Solucao *gurobi) {
    FILE* f = fopen("resultados.txt", "a");
    if (!f) return;

    const char* nomeLimpo = strrchr(instName, '/');
    if (nomeLimpo) 
        nomeLimpo++;
    else 
        nomeLimpo = instName;

    printf("\nInstancia: %s\n", nomeLimpo);
    printf("resultado CPLEX == LB: %8.2f | UB: %8.2f | GAP: %6.4f%% | Tempo: %6.2fs\n", 
           cplex->lim_inf, cplex->lim_sup, cplex->gap * 100.0, cplex->tempo);
    printf("gurobi_lb: %.2f; gurobi_ub: %.2f; gurobi_gap: %.4f%%; gurobi_tempo: %.4f\n", 
            gurobi->lim_inf, gurobi->lim_sup, gurobi->gap * 100.0, gurobi->tempo);
    
    printf("Solucao CPLEX Valor: %.2f\nItens: ", cplex->lim_inf);
        for(int i=0; i < cplex->num_sol_itens; i++) 
            printf("%d ", cplex->sol_itens[i]);

    if (!(gurobi->erro_gurobi)){
        printf("Solucao GUROBI Valor: %.2f\nItens: ", gurobi->lim_inf);
            for(int i=0; i<gurobi->num_sol_itens; i++) 
                printf("%d ", gurobi->sol_itens[i]);
    }

    fprintf(f, "instancia: %s\n", nomeLimpo);
    fprintf(f, "cplex_lb: %.2f; cplex_ub: %.2f; cplex_gap: %.4f; cplex_tempo: %.4f\n", 
            cplex->lim_inf, cplex->lim_sup, cplex->gap * 100.0, cplex->tempo);
    
    if (gurobi->erro_gurobi) {
        fprintf(f, "gurobi_lb: ERRO; gurobi_ub: ERRO; gurobi_gap: ERRO; gurobi_tempo: %.4f\n", gurobi->tempo);
    } else {
        fprintf(f, "gurobi_lb: %.2f; gurobi_ub: %.2f; gurobi_gap: %.4f%%; gurobi_tempo: %.4f\n", 
                gurobi->lim_inf, gurobi->lim_sup, gurobi->gap * 100.0, gurobi->tempo);
    }
    
    fprintf(f, "----------------------------------\n");
    fclose(f);

    char nomeSol[200];
    
    sprintf(nomeSol, "sol_cplex_%s", nomeLimpo);
    FILE* fc = fopen(nomeSol, "w");
    if(fc) {
        fprintf(fc, "Solucao CPLEX Valor: %.2f\nItens: ", cplex->lim_inf);
        for(int i=0; i < cplex->num_sol_itens; i++) 
            fprintf(fc, "%d ", cplex->sol_itens[i]);
        fclose(fc);
    }

    if (!gurobi->erro_gurobi) {
        sprintf(nomeSol, "sol_gurobi_%s", nomeLimpo);
        FILE* fg = fopen(nomeSol, "w");
        if(fg) {
            fprintf(fg, "Solucao GUROBI Valor: %.2f\nItens: ", gurobi->lim_inf);
            for(int i=0; i<gurobi->num_sol_itens; i++) 
                fprintf(fg, "%d ", gurobi->sol_itens[i]);
            fclose(fg);
        }
    }
}

void lerInstancia(const char* arquivo){
    FILE* f = fopen(arquivo, "r");
    if (!f) { 
        printf("Erro ao abrir arquivo %s. Verifique o caminho!\n", arquivo); 
        exit(1); 
    }
    fscanf(f, "%d %d %lf", &instancia.num_itens, &instancia.num_pares, &instancia.cap_mochila);
    for (int i = 0; i < instancia.num_itens; i++) 
        fscanf(f, "%lf", &instancia.lucros[i]);
    for (int i = 0; i < instancia.num_itens; i++) 
        fscanf(f, "%lf", &instancia.pesos[i]);
    int lixo;
    for (int k = 0; k < instancia.num_pares; k++) {
        fscanf(f, "%d %lf %d", &lixo, &instancia.custo_penalidade[k], &lixo);
        fscanf(f, "%d %d", &instancia.par_i[k], &instancia.par_J[k]);
    }
    fclose(f);
}

void gerarModeloLP() {
    FILE* f = fopen("modelo.lp", "w");
    if (!f) 
        exit(1);
    fprintf(f, "Maximize\n obj: ");
    for (int i = 0; i < instancia.num_itens; i++) 
        fprintf(f, "+ %.0f x%d ", instancia.lucros[i], i);
    for (int k = 0; k < instancia.num_pares; k++) 
        fprintf(f, "- %.0f z%d ", instancia.custo_penalidade[k], k);
    fprintf(f, "\nSubject To\n cap: ");
    for (int i = 0; i < instancia.num_itens; i++) 
        fprintf(f, "+ %.0f x%d ", instancia.pesos[i], i);
    fprintf(f, "<= %.0f\n", instancia.cap_mochila);
    for (int k = 0; k < instancia.num_pares; k++) {
        fprintf(f, " l1_%d: z%d - x%d <= 0\n", k, k, instancia.par_i[k]);
        fprintf(f, " l2_%d: z%d - x%d <= 0\n", k, k, instancia.par_J[k]);
        fprintf(f, " l3_%d: x%d + x%d - z%d <= 1\n", k, instancia.par_i[k], instancia.par_J[k], k);
    }
    fprintf(f, "Binary\n");
    for (int i = 0; i < instancia.num_itens; i++) 
        fprintf(f, " x%d\n", i);
    for (int k = 0; k < instancia.num_pares; k++) 
        fprintf(f, " z%d\n", k);
    fprintf(f, "End\n");
    fclose(f);
}

void solverCPLEX(Solucao *s) {
    char cmd[1000];
    sprintf(cmd, "cplex -c \"read modelo.lp\" \"set timelimit 3600\" \"optimize\" \"write cplex.sol\" > cplex.log");
    
    clock_t t1 = clock();
    system(cmd);
    clock_t t2 = clock();
    s->tempo = (double)(t2 - t1) / CLOCKS_PER_SEC;

    s->lim_inf = lerValorLog("cplex.log", "Objective =");
    s->lim_sup = lerValorLog("cplex.log", "Best bound =");

    if (s->lim_sup == 0 && s->lim_inf != 0) 
        s->lim_sup = s->lim_inf;

    if (s->lim_sup != 0) {
        s->gap = (s->lim_sup - s->lim_inf) / s->lim_sup;
        if (s->gap < 0) 
            s->gap = 0;
    }
    lerSolucaoArquivo("cplex.sol", s);
}

void solverGurobi(Solucao *s) {

    remove("gurobi.log"); 
    remove("gurobi.sol");
    const char* lic_path = "C:\\Users\\almei\\gurobi.lic";
    char cmd[2000];
    sprintf(cmd, "set GRB_LICENSE_FILE=%s & gurobi_cl TimeLimit=3600 ResultFile=gurobi.sol LogFile=gurobi.log modelo.lp > nul", lic_path);
    
    clock_t t1 = clock();
    system(cmd);
    clock_t t2 = clock();
    s->tempo = (double)(t2 - t1) / CLOCKS_PER_SEC;
    if (verificarErroGurobi("gurobi.log")) {
        s->erro_gurobi = 1;
        s->lim_inf = 0; s->lim_sup = 0; s->gap = 0;
        return; 
    }
    s->lim_inf = lerValorLog("gurobi.log", "Best objective");
    s->lim_sup = lerValorLog("gurobi.log", "best bound");
    if (s->lim_sup != 0) {
        s->gap = (s->lim_sup - s->lim_inf) / s->lim_sup;
        if(s->gap < 0) 
            s->gap = 0;
    } else {
        s->gap = lerValorLog("gurobi.log", "gap");
    }
    
    lerSolucaoArquivo("gurobi.sol", s);
}

int verificarErroGurobi(const char* arqlog) {
    FILE* f = fopen(arqlog, "r");
    if (!f) return 0;
    char linha[1000];
    int erro = 0;
    while(fgets(linha, 1000, f)) {
        if (strstr(linha, "Model too large") != NULL || strstr(linha, "Error 10010") != NULL || strstr(linha, "size-limited license") != NULL) {
            erro = 1;
            break;
        }
    }
    fclose(f);
    return erro;
}

double lerValorLog(const char* arquivo, const char* texto_procurado) {
    FILE* f = fopen(arquivo, "r");
    if (!f) return 0.0;
    char linha[1000];
    double valor_final = 0.0;
    int achou_algum = 0;

    while (fgets(linha, 1000, f) != NULL) {
        char* ponteiro = strstr(linha, texto_procurado);
        if (ponteiro != NULL) {
            ponteiro += strlen(texto_procurado);
            double temp = 0.0;
            if (sscanf(ponteiro, "%lf", &temp) == 1) {
                valor_final = temp;
                achou_algum = 1;
            }
        }
    }
    fclose(f);
    return valor_final;
}

void lerSolucaoArquivo(const char* arquivo, Solucao* s) {
    FILE* f = fopen(arquivo, "r");
    if (!f) return;
    char linha[1000];
    s->num_sol_itens = 0;
    while (fgets(linha, 1000, f) != NULL) {
        if (linha[0] == '#') continue; 
        
        char* pos_x = strstr(linha, "x");
        int valor_um = 0;
        
        if (strstr(linha, "=\"1\"")) valor_um = 1;
        if (strstr(linha, " 1")) valor_um = 1;
        
        double val_g = 0;
        char temp_nome[50];
        if (sscanf(linha, "%s %lf", temp_nome, &val_g) == 2) {
            if (temp_nome[0] == 'x' && val_g > 0.9) valor_um = 1;
        }

        if (pos_x && valor_um) {
            int id = 0;
            if (sscanf(pos_x + 1, "%d", &id) == 1) {
                if (s->num_sol_itens < MAX_ITENS) {
                    s->sol_itens[s->num_sol_itens++] = id;
                }
            }
        }
    }
    fclose(f);
}