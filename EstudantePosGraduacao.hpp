#ifndef ESTUDANTEPOSGRADUCAO_HPP
#define ESTUDANTEPOSGRADUCAO_HPP

#include "Estudante.hpp"             // Classe base: EstudantePosGraduacao herda de Estudante

// ========== CLASSE ESTUDANTE PÓS-GRADUAÇÃO ==========
// Representa um estudante de pós-graduação (mestrado ou doutorado)
// Herda de Estudante (dados pessoais, matrícula, curso, carteirinha, empréstimos)
// Adiciona informações específicas de pós-graduação:
//   - Tipo de pós (MESTRADO ou DOUTORADO)
//   - Linha de pesquisa
//   - Valor de refeição no RU específico para pós-graduação
//   - Prazo de devolução de livros específico para pós-graduação
class EstudantePosGraduacao : public Estudante
{
private:
    // ========== ATRIBUTOS ESTÁTICOS ==========
    // Compartilhados entre TODOS os EstudantePosGraduacao
    
    // Valor da refeição no RU para alunos de pós-graduação
    // Padrão: R$ 10,80 por refeição (maior que graduação)
    // Pode ser alterado globalmente pelo administrador
    // Afeta TODOS os EstudantePosGraduacao existentes
    static double valorRU;

    // ========== ATRIBUTOS PRIVADOS ==========
    
    // Tipo de pós-graduação do estudante
    // Valores: "MESTRADO" ou "DOUTORADO"
    std::string tipoPos;

    // Linha de pesquisa/especialização do estudante de pós-graduação
    // Exemplos: "Inteligência Artificial", "Processamento de Imagens", etc
    std::string linhaDePesquisa;

    // Prazo padrão de devolução de livros para alunos de pós-graduação (dias)
    // Padrão: 14 dias a partir do empréstimo (maior que graduação)
    // Pode ser alterado globalmente pelo administrador
    // Afeta TODOS os EstudantePosGraduacao existentes
    static int prazoDeDevolucao;

public:
    // ========== CONSTRUTOR ==========
    // Inicializa um estudante de pós-graduação com todos os dados e especificidades
    // Chama construtor da classe base Estudante e inicializa atributos específicos
    // Parâmetros:
    //   _nome: nome completo do estudante
    //   _cpf: CPF do estudante (11 dígitos, somente números)
    //   _data_de_nascimento: data de nascimento (formato DD/MM/YYYY)
    //   _email: email institucional UFV (deve terminar em @ufv.br)
    //   _senha: senha de acesso (mínimo 6 caracteres, letra + número)
    //   _matricula: matrícula universitária (6 dígitos)
    //   _curso: código do curso/programa de pós-graduação (ex: "101")
    //   _tipoPos: tipo de pós-graduação ("MESTRADO" ou "DOUTORADO")
    //   _linhaDePesquisa: linha de pesquisa (ex: "IA e Machine Learning")
    EstudantePosGraduacao(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento,
                          const std::string &_email, const std::string &_senha, const std::string &_matricula,
                          const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa);

    // ========== DESTRUTOR ==========
    // Destrutor virtual: garante destruição correta em cadeia de herança polimórfica
    // Não realiza limpeza especial (memória já é gerenciada por classes base)
    ~EstudantePosGraduacao();

    // ========== GETTERS ==========
    
    // Retorna o valor atual da refeição no RU para pós-graduação (atributo estático)
    // Nota: método estático, não precisa de objeto para chamar
    // Retorno: double com valor em reais (ex: 10.80 = R$ 10,80)
    static double get_valorRU();

    // Retorna o tipo de pós-graduação do estudante
    // Retorno: string com "MESTRADO" ou "DOUTORADO"
    std::string get_tipoPos() const;

    // Retorna a linha de pesquisa/especialização do estudante
    // Retorno: string com a área de pesquisa (ex: "Inteligência Artificial")
    std::string get_linhaDePesquisa() const;

    // Retorna o prazo de devolução de livros para pós-graduação
    // Implementa método abstrato puro da classe Estudante
    // Marcado com override para indicar que sobrescreve método virtual
    // Retorno: inteiro com número de dias (padrão: 14 dias)
    int get_prazoDeDevolucao() const override;

    // ========== SETTERS ==========
    
    // Define o tipo de pós-graduação do estudante
    // Parâmetro:
    //   _tipoPos: "MESTRADO" ou "DOUTORADO"
    void set_tipoPos(std::string _tipoPos);

    // Define a linha de pesquisa do estudante de pós-graduação
    // Parâmetro:
    //   _linhaDePesquisa: nova linha de pesquisa (ex: "Processamento de Linguagem Natural")
    void set_linhaDePesquisa(std::string _linhaDePesquisa);

    // Altera o valor da refeição no RU para TODOS os estudantes de pós-graduação (atributo estático)
    // Nota: método estático, afeta globalmente todos os EstudantePosGraduacao
    // Usado pelo administrador para atualizar política de preços
    // Parâmetro:
    //   _valorRU: novo valor da refeição em reais (ex: 12.00 para R$ 12,00)
    static void set_valorRU(double _valorRU);

    // ========== OPERAÇÕES COM RU ==========
    
    // Registra consumo de refeição no Restaurante Universitário (RU) para pós-graduação
    // Implementa método abstrato puro da classe Estudante
    // Marcado com override para indicar que sobrescreve método virtual
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