#ifndef ESTUDANTEGRADUACAO_HPP
#define ESTUDANTEGRADUACAO_HPP

#include "Estudante.hpp"             // Classe base: EstudanteGraduacao herda de Estudante

// ========== CLASSE ESTUDANTE GRADUAÇÃO ==========
// Representa um estudante de graduação (curso de bachelor/licenciatura)
// Herda de Estudante (dados pessoais, matrícula, curso, carteirinha, empréstimos)
// Adiciona informações específicas de graduação:
//   - Modalidade de ingresso (SISU ou TRANSF)
//   - Valor de refeição no RU específico para graduação
//   - Prazo de devolução de livros específico para graduação
class EstudanteGraduacao : public Estudante
{
private:
    // ========== ATRIBUTOS PRIVADOS ==========
    
    // Modalidade de ingresso na graduação (ex: "SISU" ou "TRANSF")
    // SISU = Sistema de Seleção Unificada (ENEM)
    // TRANSF = Transferência de outra instituição
    std::string modalidade;

    // ========== ATRIBUTOS ESTÁTICOS ==========
    // Compartilhados entre TODOS os EstudanteGraduacao
    
    // Valor da refeição no RU para alunos de graduação
    // Padrão: R$ 5,40 por refeição
    // Pode ser alterado globalmente pelo administrador
    // Afeta TODOS os EstudanteGraduacao existentes
    static double valorRU;

    // Prazo padrão de devolução de livros para alunos de graduação (dias)
    // Padrão: 7 dias a partir do empréstimo
    // Pode ser alterado globalmente pelo administrador
    // Afeta TODOS os EstudanteGraduacao existentes
    static int prazoDeDevolucao;

public:
    // ========== CONSTRUTOR ==========
    // Inicializa um estudante de graduação com todos os dados pessoais e modalidade
    // Chama construtor da classe base Estudante e inicializa modalidade
    // Parâmetros:
    //   _nome: nome completo do estudante
    //   _cpf: CPF do estudante (11 dígitos, somente números)
    //   _data_de_nascimento: data de nascimento (formato DD/MM/YYYY)
    //   _email: email institucional UFV (deve terminar em @ufv.br)
    //   _senha: senha de acesso (mínimo 6 caracteres, letra + número)
    //   _matricula: matrícula universitária (6 dígitos)
    //   _curso: código do curso (ex: "101" para Engenharia de Produção)
    //   _modalidade: forma de ingresso ("SISU" ou "TRANSF")
    EstudanteGraduacao(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento,
                       const std::string &_email, const std::string &_senha, const std::string &_matricula,
                       const std::string &_curso, const std::string &_modalidade);

    // ========== DESTRUTOR ==========
    // Destrutor virtual: garante destruição correta em cadeia de herança polimórfica
    // Não realiza limpeza especial (memória já é gerenciada por classes base e atributos)
    ~EstudanteGraduacao();

    // ========== GETTERS ==========
    
    // Retorna a modalidade de ingresso do estudante
    // Retorno: string com "SISU" ou "TRANSF"
    std::string get_modalidade() const;

    // Retorna o valor atual da refeição no RU para graduação (atributo estático)
    // Nota: método estático, não precisa de objeto para chamar
    // Retorno: double com valor em reais (ex: 5.40 = R$ 5,40)
    static double get_valorRU();

    // Retorna o prazo de devolução de livros para graduação
    // Implementa método abstrato puro da classe Estudante
    // Retorno: inteiro com número de dias (padrão: 7 dias)
    int get_prazoDeDevolucao() const override;

    // ========== SETTERS ==========
    
    // Define a modalidade de ingresso do estudante
    // Parâmetro:
    //   _modalidade: "SISU" ou "TRANSF"
    void set_modalidade(std::string _modalidade);

    // Altera o valor da refeição no RU para TODOS os estudantes de graduação (atributo estático)
    // Nota: método estático, afeta globalmente todos os EstudanteGraduacao
    // Usado pelo administrador para atualizar política de preços
    // Parâmetro:
    //   _valorRU: novo valor da refeição em reais (ex: 6.50 para R$ 6,50)
    static void set_valorRU(double _valorRU);

    // ========== OPERAÇÕES COM RU ==========
    
    // Registra consumo de refeição no Restaurante Universitário (RU)
    // Implementa método abstrato puro da classe Estudante
    // Funcionamento:
    //   1. Verifica se saldo da carteirinha >= valor do RU
    //   2. Se saldo insuficiente: lança exceção std::invalid_argument
    //   3. Se OK: debita o valor da carteirinha (registra como transação de débito)
    //   4. Exibe mensagem de confirmação com efeito de digitação lenta
    // Exceção: std::invalid_argument se saldo < valor do RU
    void comerRU() override;

    // ========== MÉTODOS DE EXIBIÇÃO E POLIMORFISMO ==========

    // Exibe os dados detalhados do estudante de graduação na consola
    // Implementa método abstrato puro da classe Estudante
    // Saída formatada:
    //   - Dados básicos (Nome, Matrícula, Curso)
    //   - Nível de ensino: "Graduação"
    //   - Modalidade de ingresso (ex: SISU, TRANSF)
    void exibirDados() const override;
};

#endif
