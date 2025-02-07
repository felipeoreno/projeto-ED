#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

// A FAZER: remoção do meio da lista/deque com iteração para concluir tarefas



void criarLista(Lista *l){
    Node *n = (point)malloc(sizeof(Node));  // Alocação de um ponteiro para o primeiro nó (sentinela)
    l->sent = n;                            // Define o primeiro nó "n" como sentinela
    n->prox = n->ant = l->sent;             // Atribui o próprio sentinela a seus ponteiros de anterior e de próximo
    l->size = 0;                            // Define como 0 o tamanho da lista
}

void deletarLista(Lista *l){
    while(!listaVazia(l)){
        removerI(l);        // Enquanto a lista não estiver vazia, remove cada um de seus elementos
    }
    free(l->sent);          // Libera a memória que armazenava o sentinela, o último nó restante
}

int listaVazia(Lista *l){
    return (l->sent->prox == l->sent);      // Retorna 1 se o ponteiro de próximo do sentinela apontar para ele mesmo (lista vazia)
}

// Inserção no início (primeiro elemento)
void inserirI(Lista *l, T task, H horas, H minutos){
    Node *novo = (point)malloc(sizeof(Node));   // Aloca espaço na memória para um novo nó
    novo->task = task;                          // Atribui o dado do parâmetro ao conteúdo do nó
    novo->time[0] = horas;
    novo->time[1] = minutos;
    novo->prox = l->sent->prox;                 // Próximo aponta para o, até então, primeiro elemento da lista
    novo->ant = l->sent;                        // Anterior aponta para o sentinela
    novo->prox->ant = novo;                     // Ponteiro "anterior" do antigo primeiro elemento, aponta para o novo nó
    l->sent->prox = novo;                       // Atribui o novo nó ao ponteiro "próximo" do sentinela 
    l->size++;                                  // Incrementa o tamanho da lista
}

// Inserção no final (último elemento)
void inserirF(Lista *l, T task, H horas, H minutos){
    Node *novo = (point)malloc(sizeof(Node));   // Aloca espaço na memória para um novo nó
    novo->task = task;                          // Atribui o dado do parâmetro ao conteúdo do nó
    novo->time[0] = horas;
    novo->time[1] = minutos;
    novo->prox = l->sent;                       // Próximo aponta para o sentinela
    novo->ant = l->sent->ant;                   // Anterior aponta para o, até então, últiimo elemento da lista
    novo->ant->prox = novo;                     // Ponteiro "próximo" do antigo último elemento, aponta para o novo nó
    l->sent->ant = novo;                        // Atribui o novo nó ao ponteiro "anterior" do sentinela 
    l->size++;                                  // Incrementa o tamanho da lista
}

// Remoção no início (primeiro elemento)
void removerI(Lista *l){
    Node *temp = l->sent->prox;             // Define o elemento a ser removido (primeiro)
    if(temp != l->sent){                    // Evita a remoção do nó sentinela
        temp->ant->prox = temp->prox;       // Ajusta o ponteiro "próximo" do nó anterior, que apontará para o elemento seguinte
        temp->prox->ant = temp->ant;        // Ajusta o ponteiro "anterior" do próximo nó, que apontará para o novo anterior
        free(temp);                         // Libera a memória alocada
        l->size--;                          // Decrementa o tamanho da lista
    }
}


// Remoção no final (último elemento)
void removerF(Lista *l){
    Node *temp = l->sent->ant;             // Define o elemento a ser removido (último)
    if(temp != l->sent){                   // Evita a remoção do nó sentinela
        temp->ant->prox = temp->prox;      // Ajusta o ponteiro "próximo" do nó anterior, que apontará para o elemento seguinte
        temp->prox->ant = temp->ant;       // Ajusta o ponteiro "anterior" do próximo nó, que apontará para o novo anterior
        free(temp);                        // Libera a memória alocada
        l->size--;                         // Decrementa o tamanho da lista
    }
}

Item copiarItem(Lista *l, unsigned n){
    Node *temp = l->sent->prox;

    for(int i = 1; i < n; i++){
        temp = temp->prox;
    }
    Item copy;
    copy.task = temp->task;
    copy.time[0] = temp->time[0];
    copy.time[1] = temp->time[1];

    return copy;
}

void criarFila(Fila *f, int tam){
    f->itens = (Item*)malloc(tam * sizeof(Item));
    f->ini = 0;
    f->fim = -1;
    f->capacidade = tam;
    f->size = 0;
}

int filaVazia(Fila *f){
    return f->size == 0;
}

int filaCheia(Fila *f){
    return f->size == f->capacidade;
}

void redimensionarFila(Fila *f){
    f->capacidade *= 2;
    f->itens = (Item*)realloc(f->itens, f->capacidade * sizeof(Item));

    if(f->itens == NULL){
        printf("Erro ao realocar memória.\n");
        return;
    }
}

void enfileirar(Fila *f, T new_task, H horas, H minutos){
    if(filaCheia(f)){
        redimensionarFila(f);
    }

    f->fim = (f->fim + 1) % f->capacidade;
    f->itens[f->fim].task = new_task;
    f->itens[f->fim].time[0] = horas;
    f->itens[f->fim].time[1] = minutos;
    f->size++;
}

Item desenfileirar(Fila *f){
    Item temp;

    if(filaVazia(f)){
        temp.task = "Fila vazia.";
        return temp;
    }

    temp = f->itens[f->ini];
    f->ini = (f->ini + 1) % f->capacidade;
    f->size--;
    return temp;
}

void imprimeFila(Fila *f){
    if(filaVazia(f)){
        return;
    }
    
    for(int i = 0; i < f->size; i++){
        int idx = (f->ini + i) % f->capacidade;
        printf("%s %02hd:%02hd\n", f->itens[idx].task, f->itens[idx].time[0], f->itens[idx].time[1]);
    }
}

void esvaziarFila(Fila *f, short n){
    if(filaVazia(f)){
        return;
    }
    
    while(!filaVazia(f)){
        Item itm = desenfileirar(f);
        if(n){
            printf("%s %02hd:%02hd\n", itm.task, itm.time[0], itm.time[1]);
        }
    }
}

void destruirFila(Fila *f){
    free(f->itens);
    free(f);
}