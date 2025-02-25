#include <gtk/gtk.h>
#include "../lista.h"
#include <ctype.h>

// Definindo uma estrutura para passar os dados
typedef struct {
    GtkWidget *entrada_titulo;
    GtkWidget *entrada_descricao;
    GtkWidget *entrada_horario;
} DadosTarefa;

GtkWidget *janela_principal, *janela_tarefas, *listbox, *listbox_concluidas;
Lista lista;

void aplicar_css() {
    GtkCssProvider *provider = gtk_css_provider_new();
    gtk_css_provider_load_from_path(provider, "css/style.css", NULL);
    
    GtkStyleContext *contexto = gtk_widget_get_style_context(janela_principal);
    gtk_style_context_add_provider(contexto, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    
    contexto = gtk_widget_get_style_context(janela_tarefas);
    gtk_style_context_add_provider(contexto, GTK_STYLE_PROVIDER(provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
}

// Remove uma tarefa da lista e da interface
gboolean remover_tarefa(GtkWidget *widget, gpointer linha) {
    gtk_widget_destroy(GTK_WIDGET(linha));
    return FALSE;
}

// Marca uma tarefa como concluída
static void concluir_tarefa(GtkWidget *widget, gpointer dado) {
    GtkWidget *linha = GTK_WIDGET(dado);
    GList *filhos = gtk_container_get_children(GTK_CONTAINER(linha));
    GtkWidget *rotulo = GTK_WIDGET(g_list_nth_data(filhos, 0));
    const char *texto_tarefa = gtk_label_get_text(GTK_LABEL(rotulo));
    
    gtk_widget_destroy(linha);
    
    // Adiciona à lista de tarefas concluídas com animação
    GtkWidget *novo_rotulo = gtk_label_new(texto_tarefa);
    gtk_widget_set_name(novo_rotulo, "tarefa-concluida");
    gtk_list_box_insert(GTK_LIST_BOX(listbox_concluidas), novo_rotulo, -1);
    gtk_widget_show_all(listbox_concluidas);
}

// Valida o formato de horário (HH:MM) - você pode ajustar isso conforme necessário
static gboolean validar_horario(const char *horario) {
    if (strlen(horario) != 5) return FALSE;
    
    // Verifica se o formato é "HH:MM"
    for (int i = 0; i < 5; i++) {
        if ((i == 2 && horario[i] != ':') || (i != 2 && !isdigit(horario[i]))) {
            return FALSE;
        }
    }
    
    int hora = (horario[0] - '0') * 10 + (horario[1] - '0');
    int minuto = (horario[3] - '0') * 10 + (horario[4] - '0');
    
    // Valida se a hora está no intervalo correto (00-23) e minuto (00-59)
    return (hora >= 0 && hora < 24 && minuto >= 0 && minuto < 60);
}

// Adiciona uma nova tarefa à lista
static void adicionar_tarefa(GtkWidget *widget, gpointer dados) {
    DadosTarefa *dados_tarefa = (DadosTarefa *)dados;
    const char *titulo = gtk_entry_get_text(GTK_ENTRY(dados_tarefa->entrada_titulo));
    const char *descricao = gtk_entry_get_text(GTK_ENTRY(dados_tarefa->entrada_descricao));
    const char *horario = gtk_entry_get_text(GTK_ENTRY(dados_tarefa->entrada_horario));
    
    if (strlen(titulo) == 0 || strlen(descricao) == 0 || strlen(horario) == 0) return;
    
    // Validação do formato do horário
    if (!validar_horario(horario)) {
        GtkWidget *dialog = gtk_message_dialog_new(GTK_WINDOW(janela_tarefas), GTK_DIALOG_MODAL, GTK_MESSAGE_ERROR, GTK_BUTTONS_OK, "Horário inválido. O formato correto é HH:MM.");
        gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        return;
    }
    
    // Inserir dados na lista
    inserirF(&lista, (char*)titulo, 0, 0);
    
    // Criar a linha de tarefa com rótulos
    GtkWidget *linha = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    GtkWidget *rotulo = gtk_label_new(titulo);
    GtkWidget *descricao_rotulo = gtk_label_new(descricao);
    GtkWidget *horario_rotulo = gtk_label_new(horario);
    GtkWidget *botao_remover = gtk_button_new_with_label("❌");
    GtkWidget *botao_concluir = gtk_button_new_with_label("✔");
    
    gtk_widget_set_name(linha, "linha-tarefa");
    gtk_widget_set_name(botao_remover, "botao-remover");
    gtk_widget_set_name(botao_concluir, "botao-concluir");
    
    g_signal_connect(botao_remover, "clicked", G_CALLBACK(remover_tarefa), linha);
    g_signal_connect(botao_concluir, "clicked", G_CALLBACK(concluir_tarefa), linha);
    

    // Empacotar os elementos na linha de tarefa
    gtk_box_pack_start(GTK_BOX(linha), rotulo, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(linha), descricao_rotulo, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(linha), horario_rotulo, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(linha), botao_concluir, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(linha), botao_remover, FALSE, FALSE, 5);
    
    // Inserir a linha na lista
    gtk_list_box_insert(GTK_LIST_BOX(listbox), linha, -1);
    gtk_widget_show_all(listbox);
}

// Mostra a janela de tarefas
static void mostrar_tarefas(GtkWidget *widget, gpointer dado) {
    gtk_widget_show_all(janela_tarefas);
    gtk_widget_hide(janela_principal);
}

// Voltar para a tela principal sem encerrar o programa
static void voltar_para_principal(GtkWidget *widget, gpointer dado) {
    gtk_widget_hide(janela_tarefas);
    gtk_widget_show_all(janela_principal);
}

// Fecha o aplicativo
static void sair_app(GtkWidget *widget, gpointer dado) {
    deletarLista(&lista);
    gtk_main_quit();
}

int main(int argc, char *argv[]) {
    gtk_init(&argc, &argv);
    criarLista(&lista);
    
    // Janela principal
    janela_principal = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janela_principal), "Bem-vindo");
    gtk_window_set_default_size(GTK_WINDOW(janela_principal), 500, 400);
    g_signal_connect(janela_principal, "destroy", G_CALLBACK(sair_app), NULL);
    
    GtkWidget *vbox_principal = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
    GtkWidget *imagem = gtk_image_new_from_pixbuf(gdk_pixbuf_new_from_file_at_scale("imagem/check-list.png", 80, 80, TRUE, NULL));
    GtkWidget *rotulo_bemvindo = gtk_label_new("Bem-vindo à sua To-Do List");
    GtkWidget *botao_entrar = gtk_button_new_with_label("Entrar");
    GtkWidget *botao_sair_principal = gtk_button_new_with_label("Sair");
    gtk_widget_set_size_request(botao_entrar, 100, 30);
    g_signal_connect(botao_entrar, "clicked", G_CALLBACK(mostrar_tarefas), NULL);
    g_signal_connect(botao_sair_principal, "clicked", G_CALLBACK(sair_app), NULL);
    
    gtk_box_pack_start(GTK_BOX(vbox_principal), imagem, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_principal), rotulo_bemvindo, TRUE, TRUE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_principal), botao_entrar, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_principal), botao_sair_principal, FALSE, FALSE, 10);
    gtk_container_add(GTK_CONTAINER(janela_principal), vbox_principal);
    
    // Janela de tarefas
    janela_tarefas = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(janela_tarefas), "Lista de Tarefas");
    gtk_window_set_default_size(GTK_WINDOW(janela_tarefas), 500, 400);
    g_signal_connect(janela_tarefas, "destroy", G_CALLBACK(sair_app), NULL);
    
    GtkWidget *vbox_tarefas = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    listbox = gtk_list_box_new();
    listbox_concluidas = gtk_list_box_new();
    
    GtkWidget *entrada_titulo = gtk_entry_new();
    GtkWidget *entrada_descricao = gtk_entry_new();
    GtkWidget *entrada_horario = gtk_entry_new();
    
    // Adicionando os placeholders
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_titulo), "Digite o título da tarefa");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_descricao), "Digite a descrição da tarefa");
    gtk_entry_set_placeholder_text(GTK_ENTRY(entrada_horario), "Digite o horário (HH:MM)");
    
    // Rótulos para os campos de entrada
    GtkWidget *rotulo_titulo = gtk_label_new("Título:");
    GtkWidget *rotulo_descricao = gtk_label_new("Descrição:");
    GtkWidget *rotulo_horario = gtk_label_new("Horário (HH:MM):");
    
    GtkWidget *botao_adicionar = gtk_button_new_with_label("Adicionar Tarefa");
    
    DadosTarefa *dados_tarefa = g_malloc(sizeof(DadosTarefa));
    dados_tarefa->entrada_titulo = entrada_titulo;
    dados_tarefa->entrada_descricao = entrada_descricao;
    dados_tarefa->entrada_horario = entrada_horario;
    
    g_signal_connect(botao_adicionar, "clicked", G_CALLBACK(adicionar_tarefa), dados_tarefa);

    
    GtkWidget *botao_sair_tarefas = gtk_button_new_with_label("Voltar");
    g_signal_connect(botao_sair_tarefas, "clicked", G_CALLBACK(voltar_para_principal), NULL);
    
    GtkWidget *rotulo_concluidas = gtk_label_new("Tarefas Concluídas:");

    gtk_box_pack_start(GTK_BOX(vbox_tarefas), rotulo_titulo, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), entrada_titulo, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), rotulo_descricao, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), entrada_descricao, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), rotulo_horario, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), entrada_horario, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), botao_adicionar, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), botao_sair_tarefas, FALSE, FALSE, 10);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), listbox, TRUE, TRUE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), rotulo_concluidas, FALSE, FALSE, 5);
    gtk_box_pack_start(GTK_BOX(vbox_tarefas), listbox_concluidas, TRUE, TRUE, 5);
    
    gtk_container_add(GTK_CONTAINER(janela_tarefas), vbox_tarefas);
    
    // Aplica o CSS
    aplicar_css();
    
    gtk_widget_show_all(janela_principal);
    gtk_main();
    
    return 0;
}
