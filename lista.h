#ifndef LISTA_H
#define LISTA_H

// Definição do tipo de dados, no caso, T como um ponteiro para char
typedef char* T;

// Estrutura do nó da lista
struct node_ {
    T data;              // Dado armazenado no nó
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

// Função para criar uma lista
void criarLista(Lista *l);

// Função para deletar uma lista
void deletarLista(Lista *l);

// Função para verificar se a lista está vazia
int listaVazia(Lista *l);

// Função para inserir um elemento no início da lista
void inserirI(Lista *l, T data);

// Função para inserir um elemento no final da lista
void inserirF(Lista *l, T data);

// Função para remover um elemento do início da lista
void removerI(Lista *l);

// Função para remover um elemento do final da lista
void removerF(Lista *l);





#endif // LISTA_H