#include "Carteirinha.hpp"
#include "Auxiliares.hpp"           // Funções utilitárias (getDataAtual, etc)
#include <chrono>                   // Para operações com tempo
#include <iostream>                 // Para entrada/saída padrão
#include <iomanip>                  // Para formatação de saída (std::setprecision, std::fixed)
#include <sstream>                  // Para stringstream (conversão de tipos)

// ========== INICIALIZAÇÃO DE ATRIBUTO ESTÁTICO ==========
// Contador global que gera IDs únicos para todas as carteirinhas
// Incrementa a cada nova carteirinha criada
int Carteirinha::nextId = 1;

// ========== CONSTRUTOR ==========
// Inicializa uma nova carteirinha com:
//   - ID único e incrementado automaticamente
//   - Extrato vazio (sem transações)
//   - Saldo zerado (R$ 0,00)
Carteirinha::Carteirinha() : extrato(){
    this->id = nextId++;            // Atribui ID único e incrementa contador global
    this->saldo = 0.0;              // Saldo inicial zerado
}

// ========== DESTRUTOR ==========
// Libera memória de todas as transações armazenadas no vetor
// Percorre o vetor e deleta cada ponteiro para Transacao
// Depois limpa o vetor para evitar vazamento de memória
Carteirinha::~Carteirinha(){
    for (auto transacao : this->extrato){
        if (transacao != nullptr) {
            delete transacao;       // Evita vazamentos de memória
        }
    }
    this->extrato.clear();          // Limpa o vetor
}

// ========== GETTER: SALDO ==========
// Retorna o saldo atual da carteirinha em reais
// Retorno: double com o saldo
double Carteirinha::getSaldo(){
    return this->saldo;
}

// ========== GETTER: EXTRATO ==========
// Retorna o vetor completo de transações realizadas
// Retorno: vetor de ponteiros para Transacao
std::vector<Transacao*> Carteirinha::getExtrato(){
    return this->extrato;
}

// ========== ADICIONAR TRANSAÇÃO ==========
// Adiciona uma transação já criada ao vetor de extrato
// Parâmetro:
//   t: ponteiro para uma Transacao já alocada
void Carteirinha::adicionarTransacao(Transacao* t) {
    this->extrato.push_back(t);     // Armazena ponteiro no vetor
}

// ========== SETTER: SALDO ==========
// Define manualmente o saldo da carteirinha
// Nota: usado internamente por depositar() e debitar()
// Parâmetro:
//   _saldo: novo valor do saldo em reais
void Carteirinha::setSaldo(double _saldo){
    this->saldo = _saldo;
}

// ========== DEPOSITAR ==========
// Realiza um depósito (crédito) na carteirinha
// Funcionamento:
//   1. Obtém data atual do sistema
//   2. Cria nova Transacao de tipo "Crédito" com valor e data
//   3. Adiciona transação ao vetor extrato
//   4. Atualiza saldo (adiciona valor)
// Parâmetro:
//   _valor: valor a depositar em reais (positivo)
void Carteirinha::depositar(double _valor){
    std::string data_atual = getDataAtual();                                   // Obtém data DD/MM/YYYY
    Transacao* nova_transacao = new Transacao("Crédito", _valor, data_atual);  // Cria transação
    this->extrato.push_back(nova_transacao);                                   // Adiciona ao histórico
    this->setSaldo(this->getSaldo() + _valor);                                 // Atualiza saldo
}

// ========== DEBITAR ==========
// Realiza um débito (debit/consumo) na carteirinha
// Funcionamento:
//   1. Obtém data atual do sistema
//   2. Cria nova Transacao de tipo "Débito" com valor e data
//   3. Adiciona transação ao vetor extrato
//   4. Atualiza saldo (subtrai valor)
// Parâmetro:
//   _valor: valor a debitar em reais (positivo, será subtraído do saldo)
void Carteirinha::debitar(double _valor){
    std::string data_atual = getDataAtual();                                   // Obtém data DD/MM/YYYY
    Transacao* nova_transacao = new Transacao("Débito", _valor, data_atual);   // Cria transação
    this->extrato.push_back(nova_transacao);                                   // Adiciona ao histórico
    this->setSaldo(this->getSaldo() - _valor);                                 // Atualiza saldo
}

// ========== EXIBIR EXTRATO ==========
// Exibe o histórico de transações (extrato) e o saldo atual
// Funcionamento:
//   1. Verifica se há transações registradas
//   2. Se vazio, exibe mensagem informativa
//   3. Se não vazio, exibe tabela com:
//      - Número total de transações
//      - Lista de cada transação (tipo, valor, data)
//      - Saldo final formatado com 2 casas decimais
void Carteirinha::exibir_extrato(){
    // --- VALIDAÇÃO: EXTRATO VAZIO ---
    // Se não houver transações, avisa ao usuário
    if (this->extrato.empty()) {
        std::cout << "[INFO] Nenhuma transação registrada.\n";
        return;
    }

    // --- CABEÇALHO DO EXTRATO ---
    std::cout << "\n=== EXTRATO DA CARTEIRINHA ===\n";
    std::cout << "Total de transações: " << this->extrato.size() << "\n";
    std::cout << "------------------------------------\n";

    // --- EXIBIÇÃO DE TRANSAÇÕES ---
    // Itera sobre todas as transações e exibe cada uma
    for (size_t i = 0; i < this->extrato.size(); i++){
        this->extrato[i]->exibir_transacao();
    }

    // --- RODAPÉ COM SALDO ---
    std::cout << "------------------------------------\n";

    // Exibe saldo formatado com 2 casas decimais (moeda)
    std::cout << "Saldo atual: R$ " << std::fixed << std::setprecision(2) << this->saldo << "\n";
}
