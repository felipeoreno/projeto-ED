#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"

int main(){

    Lista *lista = (Lista*)malloc(sizeof(Lista));
    criarLista(lista);

    inserirF(lista, "oi");
    inserirI(lista, "tchau");
    removerF(lista);

    printf("%s\n", lista->sent->ant->task);

    Fila *fila = (Fila*)malloc(sizeof(Fila));
    criarFila(fila, 10);

    enfileirar(fila, lista->sent->prox->task, 0, 0);

    esvaziarFila(fila, 0);

    enfileirar(fila, "Acordar", 8, 30);
    enfileirar(fila, "Cafe", 9, 0);
    enfileirar(fila, "Correr", 10, 0);

    esvaziarFila(fila, 1);

    deletarLista(lista);

    return 0;
}