#include <stdio.h>
#include <string.h>

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabelao){
    if (!tabelao){
        printf("\nNao foi possivel abrir este arquivo\n\n");
        return 1;
    }
    printf("\nArquivo aberto e lido\n\n");

    int in = 0;
    while (1){
        int scanValido = fscanf(tabelao, ".i %d", &in);
        if (scanValido == 1) break;
        if (feof(tabelao) || scanValido == EOF) break;
        fgetc(tabelao);
    }
    if (!in){
        printf("\nNumero de entradas nao encontrado ou invalido\n\n");
        return 2;
    }
    printf("\nNumero de entradas: %d\n\n", in);
    
    fclose(tabelao);
    return 0;
}
/*  Mapa de excecoes QuiMc:
    Excecao 1: ponteiro NULL
    Excecao 2: numero invalido de entradas no arquivo
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
            printf("\nUso: ./algoritmo [nomedoarquivo.pla]\n\n");
            return 2;
        }
        QuiMc(fopen(argv[1], "r"));
    }
    else {
        printf("\nNumero de argumentos invalido\n");
        printf("\nUso: ./algoritmo [nomedoarquivo.pla]\n\n");
        return 1;
    }
    return 0;
}
/*  Mapa de excecoes main:
    Excecao 1: quantidade de argumentos inválida
    Excecao 2: formato inválido
*/