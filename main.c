#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"

typedef char* task;

int main(){

    Lista *lista = (Lista*)malloc(sizeof(Lista));
    criarLista(lista);

    inserirF(lista, "oi");
    inserirI(lista, "tchau");
    removerF(lista);

    printf("%s", lista->sent->ant->data);

    deletarLista(lista);

    return 0;
}