📌 To-Do List com Interface Gráfica (GTK) e Estrutura de Dados

📖 Descrição do Projeto

Este projeto consiste em uma To-Do List desenvolvida em C, utilizando listas duplamente encadeadas para o gerenciamento de tarefas. A interface gráfica foi implementada com a biblioteca GTK, para uma experiência visual intuitiva.

✨ Funcionalidades

➕ Adicionar tarefas com descrição e horário.

❌ Remover tarefas da lista.

✅ Marcar tarefas como concluídas.

📋 Exibir tarefas pendentes e concluídas separadamente.

🎨 Interface estilizada com CSS para melhora o visual.

🛠 Tecnologias Utilizadas

Linguagem: C

Biblioteca Gráfica: GTK

Estrutura de Dados: Listas duplamente encadeadas e Filas Circulares Dinâmicas

Estilização: CSS

🏗 Estruturas de Dados Utilizadas

Este projeto utiliza duas principais estruturas de dados:

🔷 Lista Duplamente Encadeada

Implementada para armazenar e gerenciar as tarefas.

Utiliza um nó (Node) contendo:

Uma tarefa (task)

Um horário (time[2])

Ponteiros para o próximo e anterior nó.

Possui uma célula sentinela para facilitar a manipulação da lista.

🔶 Fila Circular Dinâmica

Baseada em um vetor dinâmico (Item[]).

Implementa redimensionamento dinâmico quando atinge a capacidade.

Gerencia os índices ini e fim para controle de inserção e remoção de maneira circular.

Nota: O projeto não utiliza uma fila duplamente encadeada.

🚀 Como Executar

📌 Requisitos

Antes de rodar o projeto, certifique-se de ter instalado:

GCC (Compilador C)
GTK (Interface gráfica)
