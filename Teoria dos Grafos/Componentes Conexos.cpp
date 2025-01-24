#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Variaeis globais */
// A matriz "matriz" eh a matriz que guardara a matriz de adjacencia, ...
// no primeiro colchetes eh um vertice e no segundo colchetes sao os vertices que sao adjacentes ao vertice do primeiro colchetes
//
// A matriz "grupo" eh a matriz que quardara os grupos e seus vertices, ...
// no primeiro colchetes eh um grupo e no segundo colchetes sao os vertices que estao no grupo do primeiro colchetes
//
// o array "ocupado" eh o array que indicara se um grupo esta ocupado ou nao
// o array "adicionado" eh o array que indicara se um vertice foi adicionado ou nao em algum grupo
// a variavel "x" eh a variavel que guardara a quantidade de vertices
int **matriz, **grupo;
int *adj;
bool *ocupado, *adicionado;
int x;

/* Funcao para adicionar o vertice no grupo */
// A funcao "add_grupo" vai receber um vertice e um grupo como parametros
// No "for" ira percorrer o grupo recebido e adicionar o vertice recebido no grupo recebido e na primeira posicao vazia (onde tiver o numero 0)
// A variavel "add" sera utilizado para nao adicionar o vertice mais de uma vez
void add_grupo(int v1, int pos){
	bool add = 0;
	// Complexidade: O(N)
	for(int i = 0; i < x; i++){
		if(grupo[pos][i] == 0 && add == 0){
			grupo[pos][i] = v1;
			adicionado[v1] = 1;
			ocupado[pos] = 1;
			add = 1;
		}
	}
}

/* Funcao para verificar se um vertice possui grupo */
// A funcao "vericar" vai receber um vertice como parametro
// As variaveis "i" e "j" serao utilizadas como contadores
// O primeiro "for" ira passar por todos os grupos
// O segundo "for" ira passar por todas as posicoes do grupo indicado pelo "i"
// Se o vertice recebido estiver em algum grupo ira retornar o grupo ("i") que ele esta
// Se nao estiver em nenhum grupo ira retornar "-1"
int verificar(int v1){
	int i, j;
	// Complexidade: O(N) + O(N-1)
	for(i = 1; i < x+1; i++){
		for(j = 0; j < x; j++){
			if(grupo[i][j] == v1){
				return i;
			}
		}
	}
	return -1;
}

/* Funcao para adicionar os vertices adjacentes */
// A funcao "add_adjacentes" vai receber um vertice e uma posicao como parametro
// A variavel "i" sera utilizada como contador
// No 'for' ira percorre todos vertices adjacentes do vertice recebido
// Se o vertice adjacente nao foi adicionado a nenhum grupo, ele chamara a funcao "add_grupo" e "add_adjacentes" e vai passar esse vertice e o grupo recebido como parametros
void add_adjacentes(int v1, int pos){
	int i = 0;
	// Complexidade: O(N^c) por causa da quantidade de adjacencias que um vertice possui
	for(i = 0; i < adj[v1]; i++){
		if(adicionado[matriz[v1][i]] == 0){
			add_grupo(matriz[v1][i], pos);
			add_adjacentes(matriz[v1][i], pos);
		}
	}
}

/* Funcao para fazer a separacao dos grupos */
// A funcao "grupo" vai receber um vertice como parametro
// A variavel "i" sera utilizada como contador
// As variaveis "pos1" e "pos2" serao utilizadas para guardar um grupo
// A variavel "posfinal" sera utilizado para sair do loop que busca um grupo vazio
void grupos(int v1){
	bool posfinal = 0;
	int i, pos1 = 0, pos2 = 0;
	
	//Percorre os grupos ("ocupado") em busca de um grupo vazio
	//O primeiro grupo vazio que ele achar sera salvo no "pos1"
	//Complexidade: O(N)
	for(i = 1; i < x+1; i++){
		if(posfinal == 0 && ocupado[i] == 0){
			pos1 = i;
			posfinal = 1;
		}
	}
	
	//Verifica se o vertice recebido nao possui adjacencia
	if(adj[v1] == 0){
		//Depois adciona o vertice recebido no grupo da variavel "pos1"
		add_grupo(v1, pos1);
	}
	//Se o vertice recebido possuir adjacencia
	else{
		//Verifica o vertice recebido se ele tiver um grupo e guarda na variavel "pos2"
		//Se possuir grupo adiciona os vertice adjacentes no grupo "pos2"
		if((pos2 = verificar(v1)) != -1){
			add_adjacentes(v1, pos2);
		}
		//Se nao possuir, adiciona o vertice recebido e os vertices adjacentes no grupo pos1
		else{
			add_grupo(v1, pos1);
			add_adjacentes(v1, pos1);
		}
	}
}

/* Funcao principal da contagem dos componentes conexos */
//Vai receber uma String como parametro
//Vai retornar o numero de componentes conexos
int compconex(char *nome){
	FILE *arquivo;
	char vert;
	int h, i, j, n = 0;
	x = 0;
	
	// h, i e j sao variaveis utilizada como contadores
	// x eh a variavel que contem quantidade de vertices
	// n eh a variavel que sera retornada a quantidade de componentes conexos
	// vert eh a variavel utilizada para a leitura de cada caracter do arquivo
	
	//Junta o nome recebido pela funcao com ".txt" e em seguida abre o arquivo em modo de leitura
	strcat(nome, ".txt");
	arquivo = fopen(nome, "r");
    
    //Percorre o arquivo ate achar um caracter que seja diferente '\n' ou EOF, para contar a quantidade de vertices
    //Obs.: EOF eh o fim do arquivo e '\n' eh um pulo de linha
    // Complexidade: O(N)
	while((vert = fgetc(arquivo)) != EOF && vert != '\n'){
		if(vert != ' '){
			x++;
		}
	}
	
	//Move o ponteiro do arquivo para o inicio do arquivo
	rewind(arquivo);
	
	//Alocacao dinamica dos arrays "adj", "ocupado" e "adicionado" com a quantidade de vertice + 1
    adj = (int*)calloc(x+1, sizeof(int));
    
    ocupado = (bool*)calloc(x+1, sizeof(bool));
    
    adicionado = (bool*)calloc(x+1, sizeof(bool));
	
	//Torna o "i" igual 1 que sera o vertice para a proxima funcao
	i = 1;
	
	//Percorre o arquivo ate o final
	//Contabilizando a quantidade de adjacencias dos vertices
	//Se o caracter for igual a 1 (49 na tabela ascii), aumenta em 1 a quantidade de adjacencias dos vertices
	//Se o caracter for igual a '\n', passa para o proximo vertice
	// Complexidade: O(N)
	while((vert = fgetc(arquivo)) != EOF){
		if(vert == 49){
			adj[i]++;
		}
		if(vert == '\n'){
			i++;
		}
	}
	
	//Move o ponteiro do arquivo para o inicio do arquivo
    rewind(arquivo);
    
    //Alocacao dinamica das matrizes "matriz" e "grupo" as linhas com a quantidade de vertice + 1 e as colunas...
    //na "matriz" com a quantidade de adjacencias do vertice passado pelo "i"
    //no "grupo" com a quantidade de vertices
    // Complexidade: O(N)
    matriz = (int**)calloc(x+1, sizeof(int*));
		for(i = 0; i < x+1; i++){
			matriz[i] = (int*)calloc(adj[i], sizeof(int));
		}
		
	// Complexidade: O(N)
	grupo = (int**)calloc(x+1, sizeof(int*));
		for(i = 0; i < x+1; i++){
			grupo[i] = (int*)calloc(x, sizeof(int));
		}
    
    //Torna o "h" igual 1 que sera o numero do vertice para a proxima funcao
    //Torna o "i" igual 1 que sera o linha dentro da matriz para a proxima funcao
    //Torna o "j" igual 0 que sera a coluna dentro da matriz para a proxima funcao
    h = 1;
    i = 1;
    j = 0;
    
    //Percorre o arquivo ate o final
	//Se o "vert" for igual a 0, aumenta o "h" em 1
	//Se o "vert" for igual a 1, coloca esse vertice na matriz na linha "i" e na coluna "j", aumenta o "h" em 1 e aumenta o "j" em 1
	//Se o "vert" for igual a '\n', define o "j" para 0, define o "h" para 1 e aumenta o "i" em 1
	// Complexidade: O(N)
    while((vert = fgetc(arquivo)) != EOF){
		switch(vert){
			case 48:
				h++;
			break;
			case 49:
				matriz[i][j] = h;
				h++;
				j++;
			break;
		}
		if(vert == '\n'){
			i++;
			j = 0;
			h = 1;
		}
		//Opcional: Mostra a matriz de adjacencia
		//printf("%c", vert);
	}
	
	fflush(arquivo);
	
	//Fecha o arquivo
    fclose(arquivo);
	
	
	//Opcional: Mostra os vertices, o numero de adjacentes que estarao dentro do parenteses e suas adjacencias depois dos dois pontos (:)
	for(i = 1; i < x+1; i++){
		printf("V%d(%d): ", i, adj[i]);
		for(j = 0; j < adj[i]; j++){
			printf("v%d ", matriz[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	
	/* Separacao dos grupos */
	//Passa por cada vertice e passa ele como parametro para a funcao "grupo"
	// Complexidade: O(N)
	for(h = 1; h < x+1; h++){
		grupos(h);
	}
	
	
	//Opcional: Mostra os grupos e os vertices que estao nele
	for(i = 1; i < x+1; i++){
		printf("Grupo %d: ", i);
		for(j = 0; j < x; j++){
			if(grupo[i][j] != 0){
				printf("V%d ", grupo[i][j]);
			}
		}
		printf("\n");
	}
	printf("\n");
	
	
	/* Conta a quantidade de grupos ocupados */
	//Percorre cada grupo verificando se ele ta ocupado, se estiver aumenta em 1 o "n"
	// Complexidade: O(N)
	for(i = 0; i < x+1; i++){
		if(ocupado[i] == 1)
			n++;
	}
	
	//Libera espaco na memoria das alocacoes dinamicas
	free(adj);
	free(ocupado);
	free(adicionado);
	// Complexidade: O(N)
	for(i = 0; i < x+1; i++){
		free(matriz[i]);
		free(grupo[i]);
	}
	free(matriz);
	free(grupo);
	
	//Retorna "n"
	return n;
}

int main(){
	char arquivo[100]; //String que sera passada para a funcao "compconex" como parametro
	int continuar = 1; //Variavel para saber se o usuario quer continuar
	//Fica em loop ate o usuario decidir parar
	//No loop, ele le o nome que o usuario passou e salva na array "arquivo" e passa como parametro para a funcao "compconex" que vei retorna a quantidade de componentes conexos 
	//depois pergunta ao usuario se ele quer continuar
	// Complexidade: O(N)
	while(continuar == 1){
		printf("Nome do arquivo: ");
		fflush(stdout); 
		scanf("%s", &arquivo);
		printf("Quantidade de componentes conexos: %d\n", compconex(arquivo));
		printf("Continuar? (1 pra sim | 0 pra nao) ");
		scanf("%d", &continuar);
	}
	
	return 0;
}
//Complexidade final: O(N^c)
