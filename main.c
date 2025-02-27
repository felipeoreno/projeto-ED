#include <gtk/gtk.h>
#include <stdlib.h>
#include <stdio.h>
#include "../lista.h"

// Variáveis globais para a lista de tarefas pendentes e a fila de concluídas
Lista lista;
Fila fila;

// Widgets da interface
GtkWidget *initial_window;
GtkWidget *main_window;
GtkWidget *entry_title;        // Campo para o título da tarefa
GtkWidget *entry_task;         // Campo para a descrição da tarefa
GtkWidget *entry_hour_minute;  // Campo para o horário (HH:MM)
GtkWidget *list_box;           // GtkListBox para exibir os títulos das tarefas pendentes

// Função para exibir mensagens de erro em um diálogo
void mostrar_erro(const gchar *mensagem) {
    GtkWidget *dialog = gtk_message_dialog_new(NULL, GTK_DIALOG_MODAL,
                                                 GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "%s", mensagem);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Atualiza o GtkListBox com os títulos das tarefas pendentes
void atualizar_lista_box() {
    // Remove todos os filhos atuais do list_box
    GList *children = gtk_container_get_children(GTK_CONTAINER(list_box));
    for (GList *iter = children; iter != NULL; iter = g_list_next(iter))
        gtk_widget_destroy(GTK_WIDGET(iter->data));
    g_list_free(children);

    // Percorre a lista de tarefas pendentes
    Node *temp = lista.sent->prox;
    while (temp != lista.sent) {
        // Considerando que a tarefa está armazenada no formato "Título: Descrição",
        // separamos o título (antes dos dois pontos)
        gchar **parts = g_strsplit(temp->task, ":", 2);
        const gchar *titulo = parts[0];

        // Cria um GtkListBoxRow e insere o título
        GtkWidget *row = gtk_list_box_row_new();
        GtkWidget *label = gtk_label_new(titulo);
        gtk_container_add(GTK_CONTAINER(row), label);
        gtk_list_box_insert(GTK_LIST_BOX(list_box), row, -1);
        gtk_widget_show_all(row);

        g_strfreev(parts);
        temp = temp->prox;
    }
}

// Callback para adicionar uma nova tarefa à lista pendente
void adicionar(GtkWidget *widget, gpointer data) {
    const gchar *title_text = gtk_entry_get_text(GTK_ENTRY(entry_title));
    const gchar *task_text = gtk_entry_get_text(GTK_ENTRY(entry_task));
    const gchar *time_str = gtk_entry_get_text(GTK_ENTRY(entry_hour_minute));

    if (g_strcmp0(title_text, "") == 0) {
        mostrar_erro("Erro: O título da tarefa não pode estar vazio!");
        return;
    }
    if (g_strcmp0(task_text, "") == 0) {
        mostrar_erro("Erro: A descrição da tarefa não pode estar vazia!");
        return;
    }
    int hour, minute;
    if (sscanf(time_str, "%d:%d", &hour, &minute) != 2 || hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        mostrar_erro("Erro: Hora ou minuto inválido! Use o formato HH:MM (ex: 14:30).");
        return;
    }

    // Concatena título e descrição no formato "Título: Descrição"
    gchar *full_task = g_strdup_printf("%s: %s", title_text, task_text);
    inserirF(&lista, full_task, hour, minute);
    atualizar_lista_box();

    // Limpa os campos de entrada após adicionar a tarefa
    gtk_entry_set_text(GTK_ENTRY(entry_title), "");
    gtk_entry_set_text(GTK_ENTRY(entry_task), "");
    gtk_entry_set_text(GTK_ENTRY(entry_hour_minute), "");
}

// Callback chamada quando uma tarefa pendente (linha do list_box) é ativada (clicada)
void on_task_row_activated(GtkListBox *box, GtkListBoxRow *row, gpointer user_data) {
    // Obtém o índice (base 0) da linha clicada e converte para índice (base 1)
    int index = gtk_list_box_row_get_index(row) + 1;

    // Percorre a lista até chegar à tarefa correspondente
    Node *temp = lista.sent->prox;
    for (int i = 1; i < index; i++)
        temp = temp->prox;

    // Cria um diálogo para exibir os detalhes da tarefa
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Detalhes da Tarefa",
                                                    GTK_WINDOW(main_window),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "_Excluir", GTK_RESPONSE_REJECT,
                                                    "_Concluir", GTK_RESPONSE_ACCEPT,
                                                    "_Cancelar", GTK_RESPONSE_CANCEL,
                                                    NULL);
    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    gchar *info = g_strdup_printf("Detalhes:\n%s\nHorário: %02d:%02d",
                                  temp->task, temp->time[0], temp->time[1]);
    GtkWidget *label = gtk_label_new(info);
    gtk_box_pack_start(GTK_BOX(content_area), label, TRUE, TRUE, 10);
    gtk_widget_show_all(dialog);
    g_free(info);

    int response = gtk_dialog_run(GTK_DIALOG(dialog));
    if (response == GTK_RESPONSE_REJECT) { // Excluir a tarefa
        removerM(&lista, index);
    } else if (response == GTK_RESPONSE_ACCEPT) { // Concluir a tarefa
        Item item = copiarItem(&lista, index);
        removerM(&lista, index);
        enfileirar(&fila, g_strdup(item.task), item.time[0], item.time[1]);
    }
    gtk_widget_destroy(dialog);
    atualizar_lista_box();
}

// Callback para exibir as tarefas concluídas com um estilo aprimorado
void on_ver_concluidas_clicked(GtkWidget *widget, gpointer data) {
    GtkWidget *dialog = gtk_dialog_new_with_buttons("Tarefas Concluídas",
                                                    GTK_WINDOW(main_window),
                                                    GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT,
                                                    "_OK", GTK_RESPONSE_OK,
                                                    NULL);
    // Aumenta o tamanho da janela de tarefas concluídas
    gtk_window_set_default_size(GTK_WINDOW(dialog), 600, 400);

    GtkWidget *content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));

    // Cria uma janela rolável e um GtkListBox para listar as tarefas concluídas
    GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled), GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
    gtk_box_pack_start(GTK_BOX(content_area), scrolled, TRUE, TRUE, 10);

    GtkWidget *concluded_list_box = gtk_list_box_new();
    gtk_container_add(GTK_CONTAINER(scrolled), concluded_list_box);

    for (int i = 0; i < fila.size; i++) {
         int idx = (fila.ini + i) % fila.capacidade;
         gchar *task_info = g_strdup_printf("%d) %s - %02d:%02d", i+1, fila.itens[idx].task,
                                            fila.itens[idx].time[0], fila.itens[idx].time[1]);
         GtkWidget *row = gtk_list_box_row_new();
         GtkWidget *label = gtk_label_new(task_info);
         // Define margens para um melhor espaçamento
         gtk_widget_set_margin_start(label, 10);
         gtk_widget_set_margin_end(label, 10);
         gtk_widget_set_margin_top(label, 5);
         gtk_widget_set_margin_bottom(label, 5);
         gtk_container_add(GTK_CONTAINER(row), label);
         gtk_list_box_insert(GTK_LIST_BOX(concluded_list_box), row, -1);
         gtk_widget_show_all(row);
         g_free(task_info);
    }

    gtk_widget_show_all(dialog);
    gtk_dialog_run(GTK_DIALOG(dialog));
    gtk_widget_destroy(dialog);
}

// Callbacks para navegação entre janelas
void on_entrar_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(initial_window);
    gtk_widget_show_all(main_window);
}

void on_voltar_clicked(GtkWidget *widget, gpointer data) {
    gtk_widget_hide(main_window);
    gtk_widget_show_all(initial_window);
}

int main(int argc, char *argv[]){
    gtk_init(&argc, &argv);

    // Carrega o CSS (se houver)
    GtkCssProvider *cssProvider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(cssProvider, "css/style.css", NULL);
    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(cssProvider),
                                              GTK_STYLE_PROVIDER_PRIORITY_USER);
    g_object_unref(cssProvider);

    criarLista(&lista);
    criarFila(&fila, 5);

    /* ===== JANELA INICIAL ===== */
    initial_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(initial_window), "Bem-vindo");
    gtk_container_set_border_width(GTK_CONTAINER(initial_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(initial_window), 600, 400);
    gtk_widget_set_name(initial_window, "welcome-window");
    // Conecta o sinal "delete-event" para encerrar a aplicação ao fechar a janela
    g_signal_connect(initial_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *init_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(initial_window), init_vbox);

    GtkWidget *welcome_label = gtk_label_new("<b>Bem-vindo a sua To Do List</b>");
    gtk_label_set_use_markup(GTK_LABEL(welcome_label), TRUE);
    gtk_box_pack_start(GTK_BOX(init_vbox), welcome_label, FALSE, FALSE, 0);

    // Carrega e redimensiona a imagem de boas-vindas
    GError *error = NULL;
    GdkPixbuf *pixbuf = gdk_pixbuf_new_from_file("imagem/check-list.png", &error);
    GtkWidget *welcome_image;
    if (!pixbuf) {
        g_warning("Erro ao carregar imagem: %s", error->message);
        welcome_image = gtk_image_new();
        g_error_free(error);
    } else {
        int orig_width = gdk_pixbuf_get_width(pixbuf);
        int orig_height = gdk_pixbuf_get_height(pixbuf);
        int new_width = 200;
        int new_height = orig_height * new_width / orig_width;
        GdkPixbuf *scaled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, new_width, new_height, GDK_INTERP_BILINEAR);
        welcome_image = gtk_image_new_from_pixbuf(scaled_pixbuf);
        g_object_unref(pixbuf);
        g_object_unref(scaled_pixbuf);
    }
    gtk_box_pack_start(GTK_BOX(init_vbox), welcome_image, TRUE, TRUE, 0);

    // Botões "Entrar" e "Sair" na janela inicial
    GtkWidget *hbox_buttons = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_box_pack_start(GTK_BOX(init_vbox), hbox_buttons, FALSE, FALSE, 0);
    GtkWidget *btn_entrar = gtk_button_new_with_label("Entrar");
    GtkWidget *btn_sair   = gtk_button_new_with_label("Sair");
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_entrar, TRUE, TRUE, 0);
    gtk_box_pack_start(GTK_BOX(hbox_buttons), btn_sair, TRUE, TRUE, 0);
    g_signal_connect(initial_window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_sair, "clicked", G_CALLBACK(gtk_main_quit), NULL);
    g_signal_connect(btn_entrar, "clicked", G_CALLBACK(on_entrar_clicked), NULL);

    /* ===== JANELA PRINCIPAL (To Do List) ===== */
    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "To Do List");
    gtk_container_set_border_width(GTK_CONTAINER(main_window), 10);
    gtk_window_set_default_size(GTK_WINDOW(main_window), 800, 600);
    gtk_widget_set_name(main_window, "main-window");
    // Conecta o sinal "delete-event" para encerrar a aplicação ao fechar a janela
    g_signal_connect(main_window, "delete-event", G_CALLBACK(gtk_main_quit), NULL);

    GtkWidget *main_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add(GTK_CONTAINER(main_window), main_vbox);

    GtkWidget *label_title = gtk_label_new("<b>To Do List</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_title), TRUE);
    gtk_style_context_add_class(gtk_widget_get_style_context(label_title), "title");
    gtk_box_pack_start(GTK_BOX(main_vbox), label_title, FALSE, FALSE, 0);

    // Campos de entrada para nova tarefa
    GtkWidget *label_task_title = gtk_label_new("Título da Tarefa:");
    gtk_box_pack_start(GTK_BOX(main_vbox), label_task_title, FALSE, FALSE, 0);
    entry_title = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), entry_title, FALSE, FALSE, 0);

    GtkWidget *label_task = gtk_label_new("Descrição da Tarefa:");
    gtk_box_pack_start(GTK_BOX(main_vbox), label_task, FALSE, FALSE, 0);
    entry_task = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), entry_task, FALSE, FALSE, 0);

    GtkWidget *label_time = gtk_label_new("Hora (HH:MM):");
    gtk_box_pack_start(GTK_BOX(main_vbox), label_time, FALSE, FALSE, 0);
    entry_hour_minute = gtk_entry_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), entry_hour_minute, FALSE, FALSE, 0);

    // Botão para adicionar a tarefa
    GtkWidget *btn_add = gtk_button_new_with_label("Adicionar Tarefa");
    g_signal_connect(btn_add, "clicked", G_CALLBACK(adicionar), NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), btn_add, FALSE, FALSE, 0);

    // Adiciona um rótulo "Tarefas Pendentes:" antes do list_box
    GtkWidget *label_pendentes = gtk_label_new("<b>Tarefas Pendentes:</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_pendentes), TRUE);
    gtk_box_pack_start(GTK_BOX(main_vbox), label_pendentes, FALSE, FALSE, 0);

    GtkWidget *label_explicao = gtk_label_new("<b>Clique nelas para remover, concluir e ver mais detalhes</b>");
    gtk_label_set_use_markup(GTK_LABEL(label_explicao), TRUE);
    gtk_box_pack_start(GTK_BOX(main_vbox), label_explicao, FALSE, FALSE, 0);

    // Cria o GtkListBox para exibir os títulos das tarefas pendentes
    list_box = gtk_list_box_new();
    gtk_box_pack_start(GTK_BOX(main_vbox), list_box, TRUE, TRUE, 0);
    g_signal_connect(list_box, "row-activated", G_CALLBACK(on_task_row_activated), NULL);

    // Botão para visualizar as tarefas concluídas
    GtkWidget *btn_ver_concluidas = gtk_button_new_with_label("Ver Tarefas Concluídas");
    g_signal_connect(btn_ver_concluidas, "clicked", G_CALLBACK(on_ver_concluidas_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), btn_ver_concluidas, FALSE, FALSE, 0);

    // Botão para voltar à janela inicial
    GtkWidget *btn_voltar = gtk_button_new_with_label("Voltar para Página Inicial");
    g_signal_connect(btn_voltar, "clicked", G_CALLBACK(on_voltar_clicked), NULL);
    gtk_box_pack_start(GTK_BOX(main_vbox), btn_voltar, FALSE, FALSE, 0);

    gtk_widget_show_all(initial_window);
    gtk_main();

    deletarLista(&lista);
    destruirFila(&fila);
    return 0;
}
