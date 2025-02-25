#ifndef LISTA_H
#define LISTA_H

// Definição do tipo de dados, no caso, T como um ponteiro para char
typedef char* T;
// Definição do tipo de dados H, inteiro usado para manipular horários
typedef short H;

// Estrutura do nó da lista
struct node_ {
    T task;              // Dado armazenado no nó
    H time[2];           // Vetor para armazenar horário (horas e minutos)
    struct node_ *prox;  // Ponteiro para o próximo elemento
    struct node_ *ant;   // Ponteiro para o elemento anterior
};
typedef struct node_ Node;  // Renomeia node

typedef Node* point;  // Definição do tipo point como um ponteiro para node

// Estrutura da lista com sentinela
typedef struct {
    Node *sent;    // Ponteiro para o nó sentinela
    unsigned size; // Tamanho da lista
} Lista;



// Definição do tipo de dados dos itens do vetor da fila
typedef struct {
    T task;              // String com o nome da tarefa
    H time[2];           // Vetor para armazenar horário (horas e minutos)
} Item;

typedef struct {
    Item *itens;        // Vetor para armazenar os elementos
    int ini;             // Índice do início da fila
    int fim;             // Índice do fim da fila
    unsigned capacidade; // Tamanho máximo da fila
    unsigned size;       // Tamanho atual da fila
} Fila;

// Função para criar uma lista
void criarLista(Lista *l);

// Função para deletar uma lista
void deletarLista(Lista *l);

// Função para verificar se a lista está vazia
int listaVazia(Lista *l);

// Função para inserir um elemento no início da lista
void inserirI(Lista *l, T task, H horas, H minutos);

// Função para inserir um elemento no final da lista
void inserirF(Lista *l, T task, H horas, H minutos);

// Função para remover um elemento do início da lista
void removerI(Lista *l);

// Função para remover um elemento do final da lista
void removerF(Lista *l);

// Função para remover um elemento do meio da lista
void removerM(Lista *l, unsigned n);

Item copiarItem(Lista *l, unsigned n);

void criarFila(Fila *f, int tam);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void redimensionarFila(Fila *f);
void enfileirar(Fila *f, T new_task, H horas, H minutos);
Item desenfileirar(Fila *f);
void imprimeFila(Fila *f);
void esvaziarFila(Fila *f, short n);
void destruirFila(Fila *f);

#endif // LISTA_H