#include "EstudantePosGraduacao.hpp"
#include "Auxiliares.hpp"             // Funções utilitárias (escreveDevagar, etc)
#include <stdexcept>                  // Para exceções padrão (std::invalid_argument)
#include <thread>                     // Para operações com threads (se necessário)
#include <chrono>                     // Para operações com tempo
#include <iostream>                   // Para entrada/saída padrão

// ========== INICIALIZAÇÃO DE ATRIBUTOS ESTÁTICOS ==========

// Valor da refeição no RU para alunos de pós-graduação (compartilhado entre todos)
// Padrão: R$ 10,80 por refeição (maior que graduação)
// Pode ser alterado globalmente pelo administrador (set_valorRU)
double EstudantePosGraduacao::valorRU = 10.80;

// Prazo máximo de devolução de livro para alunos de pós-graduação (compartilhado entre todos)
// Padrão: 14 dias a partir do empréstimo (maior que graduação)
// Pode ser alterado globalmente pelo administrador
int EstudantePosGraduacao::prazoDeDevolucao = 14;

// ========== CONSTRUTOR ==========
// Inicializa um estudante de pós-graduação herdando de Estudante
// Adiciona campos específicos: tipo de pós (MESTRADO/DOUTORADO) e linha de pesquisa
// Parâmetros:
//   _nome: nome completo do estudante
//   _cpf: CPF do estudante (11 dígitos)
//   _data_de_nascimento: data de nascimento (DD/MM/YYYY)
//   _email: email institucional UFV
//   _senha: senha de acesso
//   _matricula: matrícula universitária (6 dígitos)
//   _curso: código do curso/programa de pós-graduação
//   _tipoPos: tipo de pós-graduação ("MESTRADO" ou "DOUTORADO")
//   _linhaDePesquisa: linha de pesquisa do estudante (ex: "IA e Machine Learning")
// Nota: chama construtor da classe base Estudante via lista de inicialização
EstudantePosGraduacao::EstudantePosGraduacao(
    const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento,
    const std::string &_email, const std::string &_senha, const std::string &_matricula,
    const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa
) : Estudante(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso),
    tipoPos(_tipoPos), linhaDePesquisa(_linhaDePesquisa) {}

// ========== DESTRUTOR ==========
// Destrutor virtual: garante destruição correta em cadeia de herança
// Não realiza limpeza especial (memória já é gerenciada por Estudante)
EstudantePosGraduacao::~EstudantePosGraduacao() {}

// ========== GETTERS ==========

// Retorna o valor atual da refeição no RU para pós-graduação (atributo estático)
// Nota: método estático, não precisa de objeto para chamar
// Retorno: double com valor em reais (ex: 10.80 = R$ 10,80)
double EstudantePosGraduacao::get_valorRU()
{
    return valorRU;
}

// Retorna o tipo de pós-graduação do estudante
// Retorno: string com "MESTRADO" ou "DOUTORADO"
std::string EstudantePosGraduacao::get_tipoPos() const
{
    return this->tipoPos;
}

// Retorna a linha de pesquisa do estudante de pós-graduação
// Retorno: string com a área de pesquisa (ex: "Inteligência Artificial")
std::string EstudantePosGraduacao::get_linhaDePesquisa() const
{
    return this->linhaDePesquisa;
}

// Retorna o prazo de devolução de livros para pós-graduação
// Implementa método abstrato puro da classe Estudante
// Retorno: inteiro com número de dias (padrão: 14 dias)
int EstudantePosGraduacao::get_prazoDeDevolucao() const
{
    return this->prazoDeDevolucao;
}

// ========== SETTERS ==========

// Define o tipo de pós-graduação do estudante
// Parâmetro:
//   _tipoPos: "MESTRADO" ou "DOUTORADO"
void EstudantePosGraduacao::set_tipoPos(std::string _tipoPos)
{
    this->tipoPos = _tipoPos;
}

// Altera o valor da refeição no RU para TODOS os estudantes de pós-graduação (atributo estático)
// Nota: método estático, afeta globalmente todos os EstudantePosGraduacao
// Usado pelo administrador para atualizar política de preços
// Parâmetro:
//   _valorRU: novo valor da refeição em reais (ex: 12.00 para R$ 12,00)
void EstudantePosGraduacao::set_valorRU(double _valorRU)
{
    EstudantePosGraduacao::valorRU = _valorRU;
}

// Define a linha de pesquisa do estudante de pós-graduação
// Parâmetro:
//   _linhaDePesquisa: nova linha de pesquisa (ex: "Processamento de Linguagem Natural")
void EstudantePosGraduacao::set_linhaDePesquisa(std::string _linhaDePesquisa)
{
    this->linhaDePesquisa = _linhaDePesquisa;
}

// ========== OPERAÇÕES COM RU ==========

// Registra consumo de refeição no Restaurante Universitário (RU) para pós-graduação
// Implementa método abstrato puro da classe Estudante
// Funcionamento:
//   1. Verifica se saldo da carteirinha >= valor do RU
//   2. Se saldo insuficiente: lança exceção std::invalid_argument
//   3. Se OK: debita o valor da carteirinha (registra como transação de débito)
//   4. Exibe mensagem de confirmação com efeito de digitação lenta
// Exceção: std::invalid_argument se saldo < valor do RU
void EstudantePosGraduacao::comerRU()
{
    // --- VALIDAÇÃO DE SALDO ---
    // Verifica se estudante tem dinheiro suficiente na carteirinha
    if (this->get_carteirinha()->getSaldo() < this->get_valorRU())
    {
        throw std::invalid_argument("O seu saldo é inferior ao valor do RU");
    }

    // --- COBRANÇA ---
    // Debita o valor do RU e registra como transação
    this->get_carteirinha()->debitar(this->get_valorRU());

    // --- CONFIRMAÇÃO ---
    // Exibe mensagem com efeito de digitação lenta
    escreveDevagar("\n✅ Aproveite sua refeição!\n", 50);
}