#ifndef ADMINISTRADOR_HPP
#define ADMINISTRADOR_HPP

#include <string>
#include "Estudante.hpp"                    // Include de Estudante (usado como parâmetro em funções)
#include "Usuario.hpp"                      // Classe base: Administrador herda de Usuario
#include "EstudanteGraduacao.hpp"           // Subclasse de Estudante (necessária para dynamic_cast)
#include "EstudantePosGraduacao.hpp"        // Subclasse de Estudante (necessária para dynamic_cast)

// ========== FORWARD DECLARATIONS ==========
// Declarações antecipadas para evitar dependência circular (incluir headers causa problemas)
class Carteirinha;                          // Classe que gerencia saldo e transações do estudante
class Emprestimo;                           // Classe que representa um empréstimo de livro
class Livro;                                // Classe que representa um livro da biblioteca
class Transacao;                            // Classe que representa uma transação financeira

#include <string>
#include <iostream>

// ========== CLASSE ADMINISTRADOR ==========
// Herda de Usuario e gerencia operações administrativas do sistema
class Administrador : public Usuario
{
private:
    static int nextID;                      // Contador estático para gerar IDs únicos
    int id;                                 // ID único do administrador

public:
    // ========== CONSTRUTOR E DESTRUTOR ==========
    // Inicializa administrador com dados pessoais
    Administrador(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha);
    
    // Destrutor virtual (boas práticas com herança)
    ~Administrador();

    // ========== GETTERS E SETTERS ==========
    // Retorna o ID do administrador
    int get_id() const;
    
    // Define o ID do administrador
    void set_id(const int &_id);

    // ========== OPERAÇÕES COM LIVROS ==========
    // Cria e adiciona um novo livro à biblioteca com validação de entrada
    void criarLivro(Biblioteca &biblioteca);

    // ========== OPERAÇÕES COM ESTUDANTES ==========
    // Cria um novo estudante (Graduação ou Pós-Graduação) e o adiciona ao vetor
    void criarEstudante(std::vector<Estudante *> &estudantes);
    
    // Gera arquivo com lista formatada de todos os estudantes cadastrados
    void listarEstudante(std::vector<Estudante *> &estudantes);
    
    // Permite alterar nome, email, curso ou senha de um estudante específico
    // Retorna o código da operação realizada
    int alterarDadosEstudante(std::vector<Estudante *> &estudantes);
    
    // Permite que o administrador altere sua própria senha com confirmação
    void alterarSenhaAdministrador();

    // ========== OPERAÇÕES COM CARTEIRINHAS ==========
    // Permite recarregar o saldo da carteirinha de um estudante
    void recarregarCarteirinha(std::vector<Estudante *> &estudantes);
    
    // Exibe informações de carteirinhas (saldo, dados) de um ou todos os estudantes
    void visualizarCarteirinhas(std::vector<Estudante *> &estudantes);

    // ========== OPERAÇÕES COM TRANSAÇÕES ==========
    // Permite visualizar transações (depósitos/débitos) de todos ou um específico estudante
    void consultarTransacoes(std::vector<Estudante *> &estudantes);

    // ========== OPERAÇÕES COM EMPRÉSTIMOS ==========
    // Permite visualizar empréstimos de livros de todos ou um específico estudante
    void consultarEmprestimos(std::vector<Estudante *> &estudantes);

    // ========== OPERAÇÕES COM VALORES ADMINISTRATIVOS ==========
    // Permite alterar o valor da refeição (RU) para Graduação ou Pós-Graduação
    // Retorna qual nível foi alterado (Graduação ou Pós-Graduação)
    std::string alterarValorRU();
    
    // Permite alterar o valor da multa diária por atraso de empréstimo de livro
    void alterarValorMulta();

    // ========== OPERAÇÕES ACADÊMICAS ==========
    // Permite transferir um estudante de um curso para outro (mobilidade acadêmica)
    void mobilidadeAcademica(std::vector<Estudante *> &estudantes);
    
    // Busca e retorna o nome completo de um curso a partir de seu código
    // Lê do arquivo "codigo_cursos.txt"
    static std::string procurar_curso_por_codigo(std::string codigo);   
};

#endif