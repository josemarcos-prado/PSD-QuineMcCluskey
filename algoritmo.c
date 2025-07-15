#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabela){
    int entradas = buscaEntradas(tabela);
    if (!entradas) {
        printf("\nNumero de entradas nao encontrado ou invalido\n\n");
        return 1;
    }
    printf("\nNumero de entradas: %d\n\n", entradas);

    int quantiaMintermos1 = achaMintermos1(tabela, entradas);
    int mintermos1[quantiaMintermos1];
    armazenaMintermos1(tabela, entradas, mintermos1);

    printf("\nQuantidade de mintermos 1: %d\n", quantiaMintermos1);
    printf("Mintermos 1: ");
    imprimeArrayInt(mintermos1, quantiaMintermos1);
    printf("\n\n");
    
    fclose(tabela);
    return 0;
}
/*  Mapa de excecoes QuiMc:
    Excecao 1: numero invalido de entradas no arquivo
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
    Excecao 1: quantidade de argumentos inválida
    Excecao 2: formato inválido
    Excecao 3: arquivo invalido (erro ao abrir)
*/