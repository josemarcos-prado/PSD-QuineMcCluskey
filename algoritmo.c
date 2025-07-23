#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/*Definição da struct Termo*/
typedef struct {
    char* binario;        
    int numBits;          
    int numUns;           
    int* mintermosCobertos; /*Array de mintermos originais cobertos por este termo*/
    int quantMintermosCobertos; 
    int combinado;        /*Flag: 0 se nao combinado, 1 se foi combinado (marcado por uma combinação)*/
} Termo;

/*Função auxiliar: contaUns*/
int contaUns(char* binario) {
    int count = 0;
    for (int i = 0; binario[i] != '\0'; i++) {
        if (binario[i] == '1') {
            count++;  
        }
    }
    return count;
}

/*Função auxiliar: criaTermo*/
Termo* criaTermo(char* binario, int numEntradas, int mintermoOriginal) {
    Termo* novoTermo = (Termo*) malloc(sizeof(Termo));
    if (!novoTermo) {
        perror("Erro ao alocar Termo");
        exit(EXIT_FAILURE); 
    }
    novoTermo->binario = strdup(binario); /*Copia a string*/
    if (!novoTermo->binario) {
        perror("Erro ao alocar binario para Termo");
        free(novoTermo); /*Libera o Termo já alocado antes de sair*/
        exit(EXIT_FAILURE);
    }
    novoTermo->numBits = numEntradas;
    novoTermo->numUns = contaUns(binario); /*Calcula e atribui a quantidade de 1s na string binária usando a função auxiliar*/
    novoTermo->mintermosCobertos = (int*) malloc(sizeof(int));
    if (!novoTermo->mintermosCobertos) {
        perror("Erro ao alocar mintermosCobertos");
        free(novoTermo->binario); /*Libera o binario antes de sair*/
        free(novoTermo);
        exit(EXIT_FAILURE);
    }
    novoTermo->mintermosCobertos[0] = mintermoOriginal;
    novoTermo->quantMintermosCobertos = 1;
    novoTermo->combinado = 0; /*Inicialmente não combinado*/
    return novoTermo;
}

/*Função auxiliar: liberaTermo*/
void liberaTermo(Termo* termo) {
    if (termo) {
        free(termo->binario);
        free(termo->mintermosCobertos);
        free(termo);
    }
}

/*Função auxiliar: diferemPorUmBit*/
int diferemPorUmBit(Termo* t1, Termo* t2, int* posDiferente) {
    if (t1->numBits != t2->numBits) return 0; /*Termos de tamanhos diferentes*/

    int diferencas = 0;
    *posDiferente = -1; /*Inicializa a posição diferente*/

    for (int i = 0; i < t1->numBits; i++) {
        /*Se ambos são '-' na mesma posição, não é uma diferença de bit para combinação*/
        if (t1->binario[i] == '-' && t2->binario[i] == '-') continue;
        /*Se um é '-' e o outro não, eles não podem ser combinados diretamente assim*/
        if (t1->binario[i] == '-' || t2->binario[i] == '-') return 0;

        if (t1->binario[i] != t2->binario[i]) {
            diferencas++;
            *posDiferente = i;
        }
    }
    return (diferencas == 1); /*Verdadeiro se diferem em exatamente um bit*/
}

/*Função auxiliar: combinaTermos*/
Termo* combinaTermos(Termo* t1, Termo* t2, int posDiferente) {
    char* novoBinario = strdup(t1->binario);
    if (!novoBinario) {
        perror("Erro ao alocar novoBinario em combinaTermos");
        exit(EXIT_FAILURE);
    }
    novoBinario[posDiferente] = '-'; /*Coloca o traço na posição diferente*/

    /*Copia e une as listas de mintermos cobertos*/
    /*Primeiro, aloca um array grande o suficiente para ambos*/
    int novaQuantMintermos = t1->quantMintermosCobertos + t2->quantMintermosCobertos;
    int* novosMintermos = (int*) malloc(novaQuantMintermos * sizeof(int));
    if (!novosMintermos) {
        perror("Erro ao alocar novosMintermos em combinaTermos");
        free(novoBinario);
        exit(EXIT_FAILURE);
    }

    /*Copia os mintermos de t1*/
    memcpy(novosMintermos, t1->mintermosCobertos, t1->quantMintermosCobertos * sizeof(int));
    /*Copia os mintermos de t2 para o restante do array*/
    memcpy(novosMintermos + t1->quantMintermosCobertos, t2->mintermosCobertos, t2->quantMintermosCobertos * sizeof(int));

    /*Cria o novo termo*/
    Termo* novoTermo = (Termo*) malloc(sizeof(Termo));
    if (!novoTermo) {
        perror("Erro ao alocar novoTermo em combinaTermos");
        free(novoBinario);
        free(novosMintermos);
        exit(EXIT_FAILURE);
    }
    novoTermo->binario = novoBinario;
    novoTermo->numBits = t1->numBits;
    novoTermo->numUns = contaUns(novoBinario); /*Reconta '1's (o traço não é contado)*/
    novoTermo->mintermosCobertos = novosMintermos;
    novoTermo->quantMintermosCobertos = novaQuantMintermos;
    novoTermo->combinado = 0; /*Um termo recém-combinado ainda não foi "combinado" na próxima etapa*/

    return novoTermo;
}



/*Funcao para facilitar a impressao de array de inteiros*/
void imprimeArrayInt(int* array, int tamanho){
    printf("[");
    for (int i = 0; i < tamanho-1; i++) printf("%d, ", array[i]);
    printf("%d]", array[tamanho-1]);
}

/*Funcao que busca dentro de um arquivo .pla pelo numero de entradas*/
int buscaEntradas(FILE* tabela){
    rewind(tabela);
    int entradas = 0;
    while (1){
        int scanValido = fscanf(tabela, ".i %d", &entradas);
        if (scanValido == 1) break;
        if (feof(tabela) || scanValido == EOF) break;
        fgetc(tabela);
    }
    return entradas;
}

/*Funcao que acha a quantidade de mintermos "1" na tabela-verdade*/
int achaMintermos1(FILE* tabela, int entradas){
    rewind(tabela);

    char linha[entradas+4];
    if (!linha){
        printf("\nErro ao tentar percorrer o arquivo\n\n");
        return -1;
    }
    int quantidadeDeMinTermos1 = 0;

    while(fgets(linha, sizeof(linha), tabela)){
        if (linha[0] == '.') continue;
        if (linha[strlen(linha) - 2] == '1') quantidadeDeMinTermos1++;
    };

    return quantidadeDeMinTermos1;
}

/*Funcao que armazena o valor dos mintermos "1" encontrados num array de inteiros*/
void armazenaMintermos1(FILE* tabela, int entradas, int* mintermos1){
    rewind(tabela);

    char linha[entradas+4];
    int numeroDaLinha = 0;
    int i = 0;

    while(fgets(linha, sizeof(linha), tabela)){
        if (linha[0] == '.') continue;
        if (linha[strlen(linha)-2] == '1') {
            mintermos1[i] = numeroDaLinha;
            i++;
        }
        numeroDaLinha++;
    }
}

/*Funcao que converte int em binario e armazena numa string*/
int intToBin (int numero, int bits, char binario[bits+1], size_t tamanhoDoBinario) {
    if (tamanhoDoBinario < bits+1) {
        printf("\nString incapaz de armazenar o binario\n\n");
        return 0;
    }
    if (numero > (pow(2, bits) - 1)) {
        printf("\nErro de conversao para binario (bits insuficientes)\n\n");
        return 0;
    }

    for (int i = 0, m = bits-1; m >= 0; m--, i++){
        int pot = pow(2, m);
        if ((numero - pot) >= 0){
            binario[i] = '1';
            numero -= pot;
        }
        else {
            binario[i] = '0';
        }
    }
    binario[bits] = '\0';
    return 1;
}

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabela){
    int entradas = buscaEntradas(tabela);
    if (!entradas) {
        printf("\nNumero de entradas nao encontrado ou invalido\n\n");
        return 1;
    }
    printf("\nNumero de entradas: %d\n\n", entradas);

    int quantiaMintermos1 = achaMintermos1(tabela, entradas);
    if (quantiaMintermos1 == 0) {
        printf("\nTabela-verdade dada representa uma contradicao\n\n");
        return 2;
    }
    int mintermos1[quantiaMintermos1];
    armazenaMintermos1(tabela, entradas, mintermos1);

    char binMintermos1[quantiaMintermos1][entradas+1];
    for (int i = 0; i < quantiaMintermos1; i++) intToBin(mintermos1[i], entradas, binMintermos1[i], sizeof(binMintermos1[i]));

    printf("\nQuantidade de mintermos 1: %d\n", quantiaMintermos1);
    printf("Mintermos 1: ");
    imprimeArrayInt(mintermos1, quantiaMintermos1);
    printf("\nMintermos 1 em binario: ");
    for (int i = 0; i < quantiaMintermos1; i++) printf("%s ", binMintermos1[i]);
    printf("\n\n");
    
    //Estabelece a lista inicial dos termos com base nos mintermos com saida 1
    Termo** termosAtuais = (Termo**) malloc(quantiaMintermos1 * sizeof(Termo*));
    for(int i = 0; i < quantiaMintermos1; i++){
		char bin[entradas + 1];
		intToBin(mintermos1[i], entradas, bin, sizeof(bin));
		termosAtuais[i] = criaTermo(bin, entradas, mintermos1[i]);
	}
	int quantiaTermosAtuais = quantiaMintermos1;
	
	Termo** impPrimos = NULL; //Array para armazenar os implicantes primos
	int quantiaImpPrimos = 0;
	
	while(quantiaTermosAtuais > 0){ //Loop que será executado enquanto existirem combinacoes possiveis
		Termo** prox = NULL; //Array que armazena os termos que serao criados em cada round
		int quantiaProx = 0;
		int combinacoes = 0;
		
		//Vai executar em cada par possível de termos atualmente na lista para tentar combiná-los
		for(int i = 0; i < quantiaTermosAtuais; i++){
			for(int j = i + 1; j < quantiaTermosAtuais; j++){
				//Estabelece que só é possível combinar termos se o número de 1s diferir apenas por 1 bit
				if(termosAtuais[j]->numUns == termosAtuais[i]->numUns + 1){
					int posDiferente; //Armazena a posicao do bit diferente
					//Checa se a diferença é apenas de 1 bit
					if(diferemPorUmBit(termosAtuais[i], termosAtuais[j], &posDiferente)){
						//Caso sim, cria o novo termo combinado
						Termo* novoTermo = combinaTermos(termosAtuais[i], termosAtuais[j], posDiferente);
						//Os termos pais se tornam combinados e impossibilitados de se tornarem primos
						termosAtuais[i]->combinado = 1;
						termosAtuais[j]->combinado = 1;
						combinacoes++;
						
						//Segmento que evita que termos duplicados se tornem parte da proxima iteracao da lista
						int duplo = 0;
						for(int k = 0; k < quantiaProx; k++){
							if(strcmp(prox[j]->binario, novoTermo->binario) == 0){
								duplo = 1;
								liberaTermo(novoTermo);
								break;
							}
						}
						if(!duplo){ //Caso nao seja duplicado, vira parte da proxima iteraao
							quantiaProx++;
							prox = (Termo**) realloc(prox, quantiaProx * sizeof(Termo*));
							prox[quantiaProx - 1] = novoTermo;
						}
					}
				}
			}
		}
		//Ultima checagem apos todas as combinacoes serem feitas
		for (int i = 0; i < quantiaTermosAtuais; i++){
			if(termosAtuais[i]->combinado == 0){ //Se o termo nao foi combinado, ele é um implicante primo e parte da lista final
				impPrimos = (Termo**) realloc(impPrimos, quantiaImpPrimos * sizeof(Termo*));
				impPrimos[quantiaImpPrimos - 1] = termosAtuais[i];
			} else{
				liberaTermo(termosAtuais[i]); //Caso ele tenha sido combinado, sua memória é liberada
			}
		}
		free(termosAtuais); //Libera o ponteiro da iteracao atual
		
		//Comeca a proxima iteracao, com a lista "prox" se tornando a atual
		termosAtuais = prox;
		quantiaTermosAtuais = quantiaProx;
	}
	
	printf("--- Implicantes Primos Determinados ---\n");
	for(int i = 0; i < quantiaImpPrimos; i++){
		printf("Termo: %s Cobre: ", impPrimos[i]->binario);
		imprimeArrayInt(impPrimos[i]->mintermosCobertos, impPrimos[i]->quantMintermosCobertos);
		printf("\n");
	}
	printf("\n");
    
    fclose(tabela);
    return 0;
}
/*  Mapa de excecoes QuiMc:
    Excecao 1: numero invalido de entradas no arquivo
    Excecao 2: tabela verdade representa uma contradicao (expressao logica = 0)
*/

/*Funcao checa se ha ".pla" no final do arquivo*/
int encontrouPla(const char* nome) {
        char *doPonto = strrchr(nome, '.'); //conteudo da string a partir do ultimo ponto
        if (doPonto) {
            return !strcmp(doPonto, ".pla");
        }
        return 0;
    }

/*Funcao que faz I/O com o usuario*/
int main (int argc, char *argv[]) {
    if (argc > 1) {
        if (!encontrouPla(argv[1])){
            printf("\nFormato de arquivo invalido\n");
            printf("\nUso: ./algoritmo [nomedoarquivo].pla\n\n");
            return 2;
        }
        FILE *tabela = fopen(argv[1], "r");
        if (!tabela){
            printf("\nNao foi possivel abrir este arquivo\n\n");
            return 3;
        }
        printf("\nArquivo aberto e lido\n\n");

        QuiMc(tabela);
    }
    else {
        printf("\nNumero de argumentos invalido\n");
        printf("\nUso: ./algoritmo [nomedoarquivo].pla\n\n");
        return 1;
    }
    return 0;
}
/*  Mapa de excecoes main:
    Excecao 1: quantidade de argumentos invalida
    Excecao 2: formato invalido
    Excecao 3: arquivo invalido (erro ao abrir)
*/
