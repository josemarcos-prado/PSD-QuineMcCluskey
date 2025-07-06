#include <stdio.h>

int QuiMc (FILE* tabelao){
    
    fclose(tabelao);
}

int main (int argc, char *argv[]) {
    if (argc > 1) {
        QuiMc(fopen(argv[1], "r"));
    }
    else {
        printf("\nUso: ./algoritmo [nomedoarquivo.pla]");
        return 1;
    }
    return 0;
}