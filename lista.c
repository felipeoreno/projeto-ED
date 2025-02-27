#include <stdlib.h>
#include <stdio.h>
#include "lista.h"

// Inicializa a lista
void criarLista(Lista *l) {
    Node *n = (point)malloc(sizeof(Node));  // Alocação de um ponteiro para o primeiro nó (sentinela)
    l->sent = n;                            // Define o primeiro nó "n" como sentinela
    n->prox = n->ant = n;                   // Atribui o próprio sentinela a seus ponteiros de anterior e de próximo
    l->size = 0;                            // Define como 0 o tamanho da lista
}

// Remove e libera a memória dos elementos da lista e, por fim, da estrutura
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
        free(temp->task);                   // Libera a memória da string
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
        free(temp->task);                  // Libera a memória da string
        free(temp);                        // Libera a memória alocada
        l->size--;                         // Decrementa o tamanho da lista
    }
}

// Remoção no meio da lista (elemento escolhido através de sua posição na estrutura)
void removerM(Lista *l, unsigned n){
    if(!n || n > l->size) // Finaliza a função caso n seja zero ou maior que o tamanho da lista
        return;
    Node *temp = l->sent;

    // Se o elemento escolhido estiver antes ou na metade da lista a iteração segue no sentido do ponteiro prox,
    // e se estiver depois da metade a iteração segue pelo sentido do ponteiro ant
    if(n <= l->size / 2)
        for(unsigned i = 1; i <= n; i++){
            temp = temp->prox;
        }
    else
        for(unsigned i = 0; i <= (l->size - n); i++){ // i começa em zero para fazer mais uma iteração
            temp = temp->ant;
        }

    temp->ant->prox = temp->prox;      // Ajusta o ponteiro "próximo" do nó anterior, que apontará para o elemento seguinte
    temp->prox->ant = temp->ant;       // Ajusta o ponteiro "anterior" do próximo nó, que apontará para o novo anterior
    free(temp);                        // Libera a memória alocada
    l->size--;
}

// Copia um determinado item da lista, o qual deve ser acessado a partir de sua posição
Item copiarItem(Lista *l, unsigned n){
    Node *temp = l->sent->prox;

    // Acesso pela iteração
    for (unsigned i = 1; i < n; i++) {
        temp = temp->prox;
    }

    Item copy;
    // Aqui, usamos a mesma string
    copy.task = temp->task;
    copy.time[0] = temp->time[0];
    copy.time[1] = temp->time[1];
    return copy;
}

// Funções da fila

// Cria uma fila com capacidade inicial especificada
void criarFila(Fila *f, int tam){
    f->itens = (Item*)malloc(tam * sizeof(Item)); // Aloca espaço para os itens da fila
    f->ini = 0;                                   // Inicializa o índice do início da fila
    f->fim = -1;                                  // Inicializa o índice do fim da fila
    f->capacidade = tam;                          // Define a capacidade da fila
    f->size = 0;                                  // Define o tamanho inicial da fila como 0
}

// Verifica se a fila está vazia
int filaVazia(Fila *f){
    return (f->size == 0); // Retorna 1 se a fila estiver vazia, caso contrário retorna 0
}

// Verifica se a fila está cheia
int filaCheia(Fila *f){
    return (f->size == f->capacidade); // Retorna 1 se a fila estiver cheia, caso contrário retorna 0
}

// Redimensiona a capacidade da fila para o dobro do tamanho atual
void redimensionarFila(Fila *f){
    f->capacidade *= 2;                                                // Dobra a capacidade da fila
    f->itens = (Item*)realloc(f->itens, f->capacidade * sizeof(Item)); // Realoca memória para os itens da fila
    if (f->itens == NULL) {
        printf("Erro ao realocar memória.\n");                         // Imprime mensagem de erro se a realocação falhar e retorna
        return;
    }
}

// Adiciona um novo item à fila
void enfileirar(Fila *f, T new_task, H horas, H minutos){
    if (filaCheia(f))
        redimensionarFila(f);              // Redimensiona a fila se estiver cheia
    f->fim = (f->fim + 1) % f->capacidade; // Atualiza o índice do fim da fila
    f->itens[f->fim].task = new_task;      // Atribui a nova tarefa ao item no fim da fila
    f->itens[f->fim].time[0] = horas;      // Atribui as horas ao item no fim da fila
    f->itens[f->fim].time[1] = minutos;    // Atribui os minutos ao item no fim da fila
    f->size++;                             // Incrementa o tamanho da fila
}

// Remove e retorna o item no início da fila
Item desenfileirar(Fila *f){
    Item temp;
    if (filaVazia(f)) {
        temp.task = "Fila vazia.";         // Retorna uma mensagem se a fila estiver vazia
        return temp;
    }
    temp = f->itens[f->ini];               // Armazena o item no início da fila
    f->ini = (f->ini + 1) % f->capacidade; // Atualiza o índice do início da fila
    f->size--;                             // Decrementa o tamanho da fila
    return temp;                           // Retorna o item removido
}

void imprimeFila(Fila *f){
    if (filaVazia(f))       // Retorna se a fila estiver vazia
        return;
    for (int i = 0; i < f->size; i++) {
        int idx = (f->ini + i) % f->capacidade;     // Adequa i para funcionar corretamente como índice da fila 
        printf("%s %02hd:%02hd\n", f->itens[idx].task, f->itens[idx].time[0], f->itens[idx].time[1]);   // Imprime a tarefa e seu respectivo horário
    }
}

// Esvazia a fila sem destruí-la; para a chave n != 0, imprime cada elemento da estrutura
void esvaziarFila(Fila *f, short n){
    if (filaVazia(f))       // Retorna se a fila estver vazia
        return;
    while (!filaVazia(f)) {
        Item itm = desenfileirar(f);    // Remove até acabarem todos os elementos da fila
        if (n) {
            printf("%s %02hd:%02hd\n", itm.task, itm.time[0], itm.time[1]);
        }
    }
}

void destruirFila(Fila *f){
    free(f->itens);
    // Caso f tenha sido alocado dinamicamente, libere-o também
}
