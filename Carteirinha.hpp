#ifndef CARTEIRINHA_HPP
#define CARTEIRINHA_HPP

#include "Transacao.hpp"             // Include da classe Transacao (necessária para vetor de ponteiros)
#include <vector>                    // Para std::vector

// ========== FORWARD DECLARATIONS ==========
// Declarações antecipadas para evitar dependência circular entre headers
class Transacao;                    // Classe que representa uma transação (crédito ou débito)
class Estudante;                    // Classe Estudante (usa Carteirinha, mas Carteirinha não precisa de seus detalhes)

// ========== CLASSE CARTEIRINHA ==========
// Gerencia o saldo e histórico de transações de um estudante
// Responsabilidades:
//   - Armazenar saldo em reais (R$)
//   - Registrar histórico de transações (depósitos e débitos)
//   - Permitir depósitos (créditos) e débitos (consumos)
//   - Exibir extrato com todas as transações
// Cada estudante possui uma carteirinha única
class Carteirinha
{
private:
    // ========== ATRIBUTOS PRIVADOS ==========
    
    static int nextId;              // Contador estático global que gera IDs únicos para cada carteirinha
                                    // Incrementa automaticamente a cada nova instância criada
    
    int id;                         // ID único da carteirinha (auto-incrementado)
    
    double saldo;                   // Saldo disponível em reais (R$)
                                    // Inicialmente zerado, aumenta com depósitos, diminui com débitos
    
    std::vector<Transacao *> extrato;  // Vetor dinâmico de ponteiros para todas as transações realizadas
                                       // Armazena histórico completo de créditos e débitos

public:
    // ========== CONSTRUTOR ==========
    // Inicializa uma nova carteirinha com:
    //   - ID único e auto-incrementado
    //   - Saldo inicial zerado (R$ 0,00)
    //   - Extrato vazio (sem transações)
    Carteirinha();

    // ========== DESTRUTOR ==========
    // Libera memória de todas as transações armazenadas no vetor extrato
    // Evita vazamento de memória ao deletar ponteiros alocados com new
    ~Carteirinha();

    // ========== GETTERS ==========
    
    // Retorna o saldo atual da carteirinha em reais
    // Retorno: double com valor do saldo (ex: 50.75 para R$ 50,75)
    double getSaldo();

    // Retorna o vetor completo de todas as transações realizadas
    // Retorno: vetor de ponteiros para Transacao
    std::vector<Transacao *> getExtrato();

    // ========== SETTERS ==========
    
    // Define manualmente o saldo da carteirinha
    // Nota: uso interno, preferir usar depositar() ou debitar()
    // Parâmetro:
    //   _saldo: novo valor do saldo em reais
    void setSaldo(double _saldo);

    // ========== OPERAÇÕES COM TRANSAÇÕES ==========
    
    // Adiciona uma transação já criada ao vetor de histórico
    // Nota: não cria a transação, apenas referencia um ponteiro existente
    // Parâmetro:
    //   t: ponteiro para uma Transacao já alocada e inicializada
    void adicionarTransacao(Transacao* t);

    // ========== OPERAÇÕES FINANCEIRAS ==========
    
    // Realiza um depósito (crédito/recarrga) na carteirinha
    // Funcionamento:
    //   1. Obtém data atual do sistema (DD/MM/YYYY)
    //   2. Cria nova Transacao com tipo "Crédito", valor e data
    //   3. Adiciona transação ao vetor extrato (histórico)
    //   4. Aumenta o saldo com o valor depositado
    // Parâmetro:
    //   _valor: valor a depositar em reais (positivo)
    // Exemplo: carteirinha.depositar(50.00) → aumenta saldo em R$ 50,00
    void depositar(double _valor);

    // Realiza um débito (consumo/gasto) na carteirinha
    // Funcionamento:
    //   1. Obtém data atual do sistema (DD/MM/YYYY)
    //   2. Cria nova Transacao com tipo "Débito", valor e data
    //   3. Adiciona transação ao vetor extrato (histórico)
    //   4. Diminui o saldo com o valor debitado
    // Parâmetro:
    //   _valor: valor a debitar em reais (positivo, será subtraído do saldo)
    // Exemplo: carteirinha.debitar(12.50) → diminui saldo em R$ 12,50
    void debitar(double _valor);

    // ========== OPERAÇÕES DE EXIBIÇÃO ==========
    
    // Exibe o extrato completo (histórico de transações) e saldo atual
    // Funcionamento:
    //   1. Verifica se há transações registradas
    //   2. Se vazio, exibe mensagem informativa
    //   3. Se não vazio, exibe tabela formatada com:
    //      - Total de transações
    //      - Lista de cada transação (tipo, valor, data)
    //      - Saldo final em formato monetário (R$ X,XX)
    // Saída: exibição no console com formatação clara
    void exibir_extrato();
};

#endif
