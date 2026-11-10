
#define MAX_ITENS 1100 //da maior instancia
#define MAX_PARES 7000 //da maior instancia

struct {
    int num_itens;
    int num_pares;
    double cap_mochila;
    double lucros[MAX_ITENS];
    double pesos[MAX_ITENS];
    int par_i[MAX_PARES];
    int par_J[MAX_PARES]; 
    double custo_penalidade[MAX_PARES];
} instancia;

typedef struct s_Solucao {
    double lim_inf;
    double lim_sup;
    double gap;
    double tempo;
    int sol_itens[MAX_ITENS]; 
    int num_sol_itens;
    int erro_gurobi;//erro de licença
} Solucao;

void lerInstancia(const char* arquivo);
void gerarModeloLP();
void solverCPLEX(Solucao *s);
void solverGurobi(Solucao *s);
double lerValorLog(const char* arquivo, const char* texto_procurado);
void lerSolucaoArquivo(const char* arquivo, Solucao* s);
void salvarLinhaTabela(const char* instNome, Solucao *cplex, Solucao *gurobi);
int verificarErroGurobi(const char* arqlog);