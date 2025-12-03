#ifndef ESTUDANTE_HPP
#define ESTUDANTE_HPP

#include "Usuario.hpp"               // Classe base: Estudante herda de Usuario
#include "Carteirinha.hpp"           // Gerencia saldo e transações
#include "Emprestimo.hpp"            // Representa empréstimos de livros
#include "Biblioteca.hpp"            // Acesso ao acervo de livros
#include <string>                    // Para std::string
#include <vector>                    // Para std::vector

// ========== FORWARD DECLARATIONS ==========
// Declarações antecipadas para evitar dependência circular
class Carteirinha;                  // Classe que gerencia saldo e transações do estudante
class Emprestimo;                   // Classe que representa um empréstimo de livro

// ========== CLASSE ESTUDANTE ==========
// Classe abstrata que representa um estudante da universidade
// Herda de Usuario (dados pessoais: nome, cpf, email, senha)
// Responsabilidades:
//   - Gerenciar matrícula e curso
//   - Possuir carteirinha digital (saldo)
//   - Realizar empréstimos de livros
//   - Consumir serviços (RU - Restaurante Universitário)
// Subclasses: EstudanteGraduacao e EstudantePosGraduacao
class Estudante : public Usuario
{
protected:
    // ========== ATRIBUTOS PROTEGIDOS ==========
    // Protegidos para acesso direto das subclasses
    
    std::string matricula;          // Matrícula institucional do estudante (6 dígitos, ex: "202301")
    
    std::string curso;              // Código do curso (ex: "101" para Engenharia de Produção)
    
    Carteirinha *carteirinha;       // Ponteiro para carteirinha digital
                                    // Gerencia saldo em reais (R$) e histórico de transações
                                    // Alocada dinamicamente no construtor
    
    std::vector<Emprestimo *> emprestimos;  // Vetor dinâmico de empréstimos
                                            // Armazena todos os empréstimos (ativos e histórico)
    
    int prazoDeDevolucao;           // Prazo máximo para devolver livro (dias)
                                    // Varia conforme tipo: Graduação (14 dias) ou Pós-Grad (21 dias)

public:
    // ========== CONSTRUTOR ==========
    // Inicializa um estudante herdando dados do Usuario e criando carteirinha automática
    // Parâmetros:
    //   _nome: nome completo do estudante
    //   _cpf: CPF do estudante (11 dígitos)
    //   _data_de_nascimento: data de nascimento (DD/MM/YYYY)
    //   _email: email institucional UFV (@ufv.br)
    //   _senha: senha de acesso (mínimo 6 caracteres, com letra e número)
    //   _matricula: matrícula universitária (6 dígitos)
    //   _curso: código do curso (ex: "101", "102")
    // Efeito colateral: cria automaticamente uma Carteirinha com saldo zerado
    Estudante(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento,
              const std::string &_email, const std::string &_senha, const std::string &_matricula,
              const std::string &_curso);

    // ========== DESTRUTOR ==========
    // Destrutor virtual: garante destruição correta em herança polimórfica
    // Libera memória alocada (carteirinha e empréstimos)
    // Evita vazamento de memória
    virtual ~Estudante();

    // ========== OPERAÇÕES COM EMPRÉSTIMOS ==========
    
    // Adiciona um empréstimo já criado ao vetor de empréstimos do estudante
    // Nota: não cria o empréstimo, apenas referencia
    // Parâmetro:
    //   e: ponteiro para um Emprestimo já alocado
    void adicionarEmprestimo(Emprestimo *e);

    // Exibe na tela todos os empréstimos ativos do estudante com informações formatadas
    // Saída: lista de empréstimos (ID, livro, datas, status)
    void exibirEmprestimos();

    // Fluxo completo para pegar um livro emprestado na biblioteca:
    //   1. Lista livros disponíveis (com filtro e paginação)
    //   2. Solicita escolha do livro por ID
    //   3. Valida disponibilidade
    //   4. Cria registro de empréstimo com datas
    //   5. Reduz quantidade disponível
    // Parâmetro:
    //   biblioteca: referência à Biblioteca (para listar e buscar livros)
    void pegarLivro(const Biblioteca &biblioteca);

    // Fluxo completo para devolver um livro:
    //   1. Lista empréstimos não devolvidos
    //   2. Solicita qual devolver
    //   3. Calcula multa por atraso (se houver)
    //   4. Cobra multa da carteirinha (com opção de recarregar)
    //   5. Marca como devolvido e reintegra livro ao acervo
    // Parâmetro:
    //   biblioteca: referência à Biblioteca (contexto)
    void devolverLivro(const Biblioteca &biblioteca);

    // ========== OPERAÇÕES COM CARTEIRINHA ==========
    
    // Permite ao estudante consultar o saldo atual da carteirinha
    // Saída: exibição formatada com 2 casas decimais (R$ X,XX)
    void consultarSaldo();

    // Processo para o estudante recarregar (depositar dinheiro na) carteirinha
    // Funcionamento:
    //   1. Exibe saldo atual
    //   2. Solicita valor a depositar
    //   3. Valida formato (aceita vírgula ou ponto decimal)
    //   4. Deposita na carteirinha (registra como transação de crédito)
    void recarregarCarteirinha();

    // Gera e salva a imagem da carteirinha digital do estudante
    // Funcionamento:
    //   1. Solicita extensão da foto (PNG, JPG, BMP)
    //   2. Carrega template de carteirinha e código de barras
    //   3. Insere foto do aluno (nome_matricula_foto.ext)
    //   4. Adiciona textos (nome, curso, matrícula, CPF, validade, data emissão)
    //   5. Salva em carteirinhas/nome_matricula_CARTEIRINHA.bmp
    // Exceções: lança std::runtime_error se não conseguir carregar arquivos de imagem
    void visualizarCarteirinha();

    // ========== MÉTODO ABSTRATO ==========
    
    // Método abstrato: comportamento específico de cada subclasse
    // Registra consumo no Restaurante Universitário (RU)
    // Implementação:
    //   - EstudanteGraduacao: cobra valor diferente de Pós-Grad
    //   - EstudantePosGraduacao: cobra valor diferente de Graduação
    // Nota: torna Estudante uma classe abstrata (não pode ser instanciada diretamente)
    virtual void comerRU() = 0;
    
    // Método virtual puro:
    // - Obriga todas as subclasses concretas (Graduação/Pos) a implementar
    //   uma rotina para exibir os dados do estudante.
    // - O '= 0' torna a classe abstrata (não pode instanciá-la diretamente).
    // - O 'const' indica que a função não modifica o objeto.
    virtual void exibirDados() const = 0;

    // ========== GETTERS ==========
    
    // Retorna a matrícula do estudante
    // Retorno: string com matrícula (ex: "202301")
    std::string get_matricula() const;

    // Retorna o código do curso
    // Retorno: string com código (ex: "101")
    std::string get_curso() const;

    // Retorna ponteiro para a carteirinha digital do estudante
    // Retorno: ponteiro para Carteirinha
    Carteirinha *get_carteirinha() const;

    // Retorna vetor de todos os empréstimos do estudante
    // Retorno: vetor de ponteiros para Emprestimo
    std::vector<Emprestimo *> get_emprestimos() const;

    // Retorna o prazo de devolução (definido pelas subclasses)
    // Método abstrato puro: cada subclasse define seu próprio prazo
    //   - EstudanteGraduacao: 14 dias
    //   - EstudantePosGraduacao: 21 dias
    // Retorno: inteiro com número de dias
    virtual int get_prazoDeDevolucao() const = 0;

    // ========== SETTERS ==========
    
    // Define a matrícula do estudante
    // Parâmetro:
    //   _matricula: nova matrícula (6 dígitos)
    void set_matricula(std::string _matricula);

    // Define o curso do estudante
    // Parâmetro:
    //   _curso: novo código de curso (ex: "101")
    void set_curso(std::string _curso);
};

#endif
