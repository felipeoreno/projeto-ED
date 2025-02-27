#ifndef LISTA_H
#define LISTA_H

// Definição do tipo de dados, no caso, T como um ponteiro para char
typedef char* T;
// Definição do tipo de dados H, inteiro usado para manipular horários no programa
typedef short H;

// Estrutura do nó da lista
typedef struct node_ {
    T task;              // Descrição da tarefa
    H time[2];           // Horário: [0]=hora, [1]=minuto
    struct node_ *prox;  // Ponteiro para o próximo elemento
    struct node_ *ant;   // Ponteiro para o elemento anterior
} Node;

typedef Node* point;

// Estrutura da lista com sentinela
typedef struct {
    Node *sent;    // Ponteiro para o nó sentinela
    unsigned size; // Tamanho da lista
} Lista;

// Estrutura do item (para a fila de tarefas concluídas)
typedef struct {
    T task;    // Descrição da tarefa
    H time[2]; // Horário: [0]=hora, [1]=minuto
} Item;

// Estrutura da fila
typedef struct {
    Item *itens;        // Vetor de itens
    int ini;            // Índice do início da fila
    int fim;            // Índice do fim da fila
    unsigned capacidade; // Capacidade máxima
    unsigned size;      // Número atual de itens
} Fila;

// Protótipos das funções da lista
void criarLista(Lista *l);
void deletarLista(Lista *l);
int listaVazia(Lista *l);
void inserirI(Lista *l, T task, H horas, H minutos);
void inserirF(Lista *l, T task, H horas, H minutos);
void removerI(Lista *l);
void removerF(Lista *l);
void removerM(Lista *l, unsigned n);
Item copiarItem(Lista *l, unsigned n);

// Protótipos das funções da fila
void criarFila(Fila *f, int tam);
int filaVazia(Fila *f);
int filaCheia(Fila *f);
void redimensionarFila(Fila *f);
void enfileirar(Fila *f, T new_task, H horas, H minutos);
Item desenfileirar(Fila *f);
void imprimeFila(Fila *f);
void esvaziarFila(Fila *f, short n);
void destruirFila(Fila *f);

// Documentação das funções em lista.c

#endif // LISTA_H
