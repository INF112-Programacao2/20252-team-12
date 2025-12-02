#include "EstudanteGraduacao.hpp"
#include "Auxiliares.hpp"             // Funções utilitárias (escreveDevagar, etc)
#include <stdexcept>                  // Para exceções padrão (std::invalid_argument)
#include <thread>                     // Para operações com threads (se necessário)
#include <chrono>                     // Para operações com tempo
#include <iostream>                   // Para entrada/saída padrão

// ========== INICIALIZAÇÃO DE ATRIBUTOS ESTÁTICOS ==========

// Valor da refeição no RU para alunos de graduação (compartilhado entre todos)
// Padrão: R$ 5,40 por refeição
// Pode ser alterado globalmente pelo administrador (set_valorRU)
double EstudanteGraduacao::valorRU = 5.40;

// Prazo máximo de devolução de livro para alunos de graduação (compartilhado entre todos)
// Padrão: 7 dias a partir do empréstimo
// Pode ser alterado globalmente pelo administrador
int EstudanteGraduacao::prazoDeDevolucao = 7;

// ========== CONSTRUTOR ==========
// Inicializa um estudante de graduação herdando de Estudante
// Adiciona o campo específico: modalidade (SISU ou TRANSF)
// Parâmetros:
//   _nome: nome completo do estudante
//   _cpf: CPF do estudante (11 dígitos)
//   _data_de_nascimento: data de nascimento (DD/MM/YYYY)
//   _email: email institucional UFV
//   _senha: senha de acesso
//   _matricula: matrícula universitária (6 dígitos)
//   _curso: código do curso (ex: "101")
//   _modalidade: forma de ingresso (SISU ou TRANSF)
EstudanteGraduacao::EstudanteGraduacao(
    const std::string& _nome, const std::string &_cpf, std::string& _data_de_nascimento,
    const std::string& _email, const std::string& _senha,
    const std::string &_matricula, const std::string &_curso,
    const std::string& _modalidade
) : Estudante(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso),
    modalidade(_modalidade) {}

// ========== DESTRUTOR ==========
// Destrutor virtual: garante destruição correta em cadeia de herança
// Não realiza limpeza especial (memoria já é gerenciada por Estudante)
EstudanteGraduacao::~EstudanteGraduacao() {}

// ========== GETTERS ==========

// Retorna a modalidade de ingresso do estudante de graduação
// Retorno: string com "SISU" ou "TRANSF"
std::string EstudanteGraduacao::get_modalidade() const {
    return this->modalidade;
}

// Retorna o valor atual da refeição no RU para graduação (atributo estático)
// Retorno: double com valor em reais (ex: 5.40 = R$ 5,40)
double EstudanteGraduacao::get_valorRU() {
    return valorRU;
}

// Retorna o prazo de devolução de livros para graduação (atributo estático)
// Implementação do método abstrato de Estudante
// Retorno: inteiro com número de dias (padrão: 7)
int EstudanteGraduacao::get_prazoDeDevolucao() const {
    return this->prazoDeDevolucao;
}

// ========== SETTERS ==========

// Define a modalidade de ingresso do estudante
// Parâmetro:
//   _modalidade: "SISU" ou "TRANSF"
void EstudanteGraduacao::set_modalidade(std::string _modalidade) {
    this->modalidade = _modalidade;
}

// Altera o valor global do RU para TODOS os alunos de graduação (atributo estático)
// Nota: método estático, afeta globalmente todos os EstudanteGraduacao
// Usado pelo administrador para atualizar política de valores
// Parâmetro:
//   _valorRU: novo valor da refeição em reais (ex: 6.50)
void EstudanteGraduacao::set_valorRU(double _valorRU) {
    EstudanteGraduacao::valorRU = _valorRU;
}

// ========== OPERAÇÕES COM RU ==========

// Registra consumo de refeição no Restaurante Universitário (RU)
// Funcionamento:
//   1. Verifica se saldo >= valor do RU
//   2. Se saldo insuficiente, lança exceção
//   3. Se OK, debita valor da carteirinha (registra como transação)
//   4. Exibe mensagem de confirmação com efeito de digitação lenta
// Exceção: std::invalid_argument se saldo < valor do RU
void EstudanteGraduacao::comerRU() {
    // --- VALIDAÇÃO DE SALDO ---
    // Verifica se estudante tem dinheiro suficiente na carteirinha
    if (this->get_carteirinha()->getSaldo() < this->get_valorRU()) {
        throw std::invalid_argument("O seu saldo é inferior ao valor do RU");
    }

    // --- COBRANÇA ---
    // Debita o valor do RU e registra como transação
    this->get_carteirinha()->debitar(this->get_valorRU());

    // --- CONFIRMAÇÃO ---
    // Exibe mensagem com efeito de digitação lenta
    escreveDevagar("\n✅ Aproveite sua refeição!\n", 50);
}
