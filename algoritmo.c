#include <stdio.h>
#include <string.h>

/*Funcao que busca dentro de um arquivo .pla pelo numero de entradas*/
int buscaEntradas(FILE* tabela){
    int entradas = 0;
    while (1){
        int scanValido = fscanf(tabela, ".i %d", &entradas);
        if (scanValido == 1) break;
        if (feof(tabela) || scanValido == EOF) break;
        fgetc(tabela);
    }
    return entradas;
}

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabela){
    int in = buscaEntradas(tabela);
    if (!in) {
        printf("\nNumero de entradas nao encontrado ou invalido\n\n");
        return 1;
    }
    printf("\nNumero de entradas: %d\n\n", in); 
    
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