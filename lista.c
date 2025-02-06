#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

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
void inserirI(Lista *l, T data){
    Node *novo = (point)malloc(sizeof(Node));   // Aloca espaço na memória para um novo nó
    novo->data = data;                          // Atribui o dado do parâmetro ao conteúdo do nó    
    novo->prox = l->sent->prox;                 // Próximo aponta para o, até então, primeiro elemento da lista
    novo->ant = l->sent;                        // Anterior aponta para o sentinela
    novo->prox->ant = novo;                     // Ponteiro "anterior" do antigo primeiro elemento, aponta para o novo nó
    l->sent->prox = novo;                       // Atribui o novo nó ao ponteiro "próximo" do sentinela 
    l->size++;                                  // Incrementa o tamanho da lista
}

// Inserção no final (último elemento)
void inserirF(Lista *l, T data){
    Node *novo = (point)malloc(sizeof(Node));   // Aloca espaço na memória para um novo nó
    novo->data = data;                          // Atribui o dado do parâmetro ao conteúdo do nó
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