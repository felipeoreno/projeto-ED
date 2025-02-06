#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lista.h"

#define HR 0
#define MIN 1

int main(){

    Lista *lista = (Lista*)malloc(sizeof(Lista));
    criarLista(lista);

    inserirF(lista, "oi", 0, 0);
    inserirI(lista, "tchau", 0, 30);
    removerF(lista);

    inserirF(lista, "um", 0, 40);
    inserirF(lista, "dois", 0, 50);

    printf("%s\n", lista->sent->ant->task);

    Fila *fila = (Fila*)malloc(sizeof(Fila));
    criarFila(fila, 10);

    enfileirar(fila, lista->sent->prox->task, 0, 0);

    esvaziarFila(fila, 0);

    enfileirar(fila, "Acordar", 8, 30);
    enfileirar(fila, "Cafe", 9, 0);
    enfileirar(fila, "Correr", 10, 0);

    Item x = copiarItem(lista, 1);
    Item y = copiarItem(lista, 3);

    esvaziarFila(fila, 1);

    printf("- %s\n- %s %02hd:%02hd\n", x.task, y.task, y.time[HR], y.time[MIN]);

    deletarLista(lista);

    return 0;
}