class Grafo:

    def __init__(self, vertices):
        self.vertice = vertices
        self.grafo = [[0]*self.vertice for i in range(self.vertice)] #preenchendo tudo com 0
        self.arestas = [] #conjunto de arestas

    def adicionarAresta(self, u, v, valor):
        self.grafo[u][v] = valor
        self.arestas.append((u,v,valor))

    def MostrarGrafo(self):
        print("A matriz de adjacencia do grafo é: ")
        for i in range(self.vertice):
            print(self.grafo[i])

    def getGrafo(self):
        return self.grafo
    
    def kruskal(self):

        arvore_geradora_minima = []
        self.arestas.sort(key=lambda item: item[2])
        soma_pesos = 0
        floresta = [set([i]) for i in range(self.vertice)] #fazendo cada vertice virar um elemento no conjunto
        #onde cada vertice é uma arvore
        
        for u, v, peso in self.arestas:
            
            conjunto_u = None #conjunto para armazenar os vertices u
            conjunto_v = None #conjunto para armazenar os vertices v

            for conjunto in floresta:
                if u in conjunto: #se o vertice u estiver no conjunto
                    conjunto_u = conjunto #guardo esse vertice na variavel correspondente
                if v in conjunto: #o mesmo para aqui
                    conjunto_v = conjunto #aqui tambem

            if conjunto_u != conjunto_v: #se forem arvores diferentes, desconexas, então não formará um ciclo
                arvore_geradora_minima.append((u, v, peso))
                soma_pesos += peso
                conjunto_u.update(conjunto_v) #unimos o conjunto com árvores distintas
                floresta.remove(conjunto_v)

        return soma_pesos

def LerArquivos():
    try:
        with open('arq.txt', 'r+') as arq:
            try:
                dados = arq.readlines()
                return dados
            except Exception as e:
                print("Falha ao abrir ou ler o arquivo, tente novamente")
                print(e)
    except Exception as e:
        print("Não foi possível abrir o arquivo")
    
def ColetandoDados(dados):

    primeira_linha = dados[0].split()
    NumCidades = int(primeira_linha[0]) #numero de vertices
    EstradasPossiveis = int(primeira_linha[1]) #numero de arestas
    
    grafo = Grafo(NumCidades)
    
    ListaDeCaminhos = []
    for c in range(1, EstradasPossiveis+1):
        ListaDeCaminhos.append(dados[c].split())

    for caminho in ListaDeCaminhos:
        grafo.adicionarAresta(int(caminho[0]), int(caminho[1]), int(caminho[2]))
    

    return grafo

if __name__ == "__main__":

    try:
        dados = LerArquivos()
        grafo = ColetandoDados(dados)
        AGM = grafo.kruskal()
        print(AGM)
    except Exception as e:
        print("Erro ao abrir arquivo, tente novamente e insira o arquivo\n")