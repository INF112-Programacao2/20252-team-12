#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include "Estudante.hpp"
#include "Administrador.hpp"
#include "Biblioteca.hpp"
#include <vector>
#include <fstream>

// ========== CLASSE SISTEMA ==========
// Gerencia todo o funcionamento do aplicativo:
//   - Carregamento/salvamento persistente (arquivos)
//   - Controle de usuários (admin e estudantes)
//   - Integra Biblioteca, Empréstimos e Transações
//   - Fornece menus interativos para administrador e estudante
class Sistema
{
private:
    // Stream usada para leitura de dados de livro (pode ser usada em carregamento)
    std::ifstream arquivo_livros;

    // Lista de estudantes cadastrados no sistema (ponteiros gerenciados pelo Sistema)
    std::vector<Estudante *> estudantes;

    // Ponteiro para o estudante atualmente logado (nullptr se nenhum)
    Estudante *estudante_logado;

    // Ponteiro para o administrador do sistema
    Administrador *admin;

    // Ponteiro para a biblioteca do sistema (contém acervo de livros)
    Biblioteca *biblioteca;

    // ========== MÉTODOS PRIVADOS DE PERSISTÊNCIA E CARREGAMENTO ==========
    // Implementam leitura/escrita de arquivos e reconstrução do estado do sistema

    // Carrega livros a partir de arquivo (livros.txt) para a biblioteca
    void carregarLivros();

    // Carrega estudantes a partir de arquivo (banco_estudantes.txt)
    void carregarDados();

    // Salva estudantes em arquivo (banco_estudantes.txt)
    void salvarDados();

    // Carrega dados do administrador (admin_dados.txt)
    void carregarAdmin();

    // Salva dados do administrador (admin_dados.txt)
    void salvarAdmin();

    // Exibe e controla o menu do administrador (opções de gerenciamento)
    void menuAdministrador();

    // Exibe e controla o menu do estudante (opções de uso pessoal)
    void menuEstudante();

    // Carrega empréstimos pendentes a partir de arquivo (banco_emprestimos.txt)
    void carregarEmprestimos();

    // Salva empréstimos pendentes em arquivo (banco_emprestimos.txt)
    void salvarEmprestimos();

    // Carrega transações de carteirinhas (banco_transacoes.txt)
    void carregarTransacoes();

    // Salva histórico de transações (banco_transacoes.txt)
    void salvarTransacoes();

public:
    // Construtor: inicializa componentes, restaura estado a partir de arquivos
    Sistema();

    // Destrutor: salva estado e libera memória alocada (estudantes, admin, biblioteca)
    ~Sistema();

    // ========== GETTERS SIMPLES ==========
    // Retorna vetor de ponteiros para estudantes (cópia do vetor interno)
    std::vector<Estudante *> get_estudantes();

    // Retorna ponteiro para o administrador atual
    Administrador *get_admin();

    // Inicia o loop principal do sistema (tela inicial, autenticação, menus)
    void iniciarSistema();
};

#endif