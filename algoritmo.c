#include <stdio.h>
#include <string.h>

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabelao){
    if (!tabelao){
        printf("\nNao foi possivel abrir este arquivo\n\n");
        return 1;
    }
    printf("\nArquivo aberto e lido\n\n");
    
    fclose(tabelao);
    return 0;
}
/*  Mapa de excecoes QuiMc:
    Excecao 1: ponteiro NULL
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