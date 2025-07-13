#include <stdio.h>
#include <string.h>

/*Funcao que implementea o algoritmo de Quine-McCluskey*/
int QuiMc (FILE* tabelao){
    if (!tabelao){
        printf("Nao foi possivel abrir este arquivo\n");
        return 1;
    }

    
    fclose(tabelao);
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
            printf("Formato inválido\n");
            return 2;
        }
        QuiMc(fopen(argv[1], "r"));
    }
    else {
        printf("\nUso: ./algoritmo [nomedoarquivo.pla]");
        return 1;
    }
    return 0;
}
/*  Mapa de excecoes main:
    Excecao 1: quantidade de argumentos inválida
    Excecao 2: formato inválido
*/