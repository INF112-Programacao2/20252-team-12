#ifndef TRANSACAO_HPP
#define TRANSACAO_HPP

#include <ctime>        // Para tipos/operacoes com data (se necessário)
#include <string>       // Para std::string
#include "Estudante.hpp"// Dependência para set_origem/get_origem (forward também declarado abaixo)

// Forward declaration para evitar problema de inclusão circular
class Estudante;

// ========== CLASSE TRANSACAO ==========
// Representa uma operação financeira na carteirinha de um estudante.
// Cada transação tem:
//   - id único (nextID gera ids auto-incrementais)
//   - tipo (ex: "Crédito" ou "Débito")
//   - valor (double, positivo para créditos ou débitos conforme convenção do sistema)
//   - origem (ponteiro para o Estudante responsável pela transação)
//   - data (string no formato DD/MM/YYYY)
//
// Responsabilidades:
//   - armazenar informação da operação
//   - expor getters/setters para persistência e exibição
//   - formatar exibição (exibir_transacao)
class Transacao {
    private:
        static int nextID;      // Contador estático para IDs únicos (auto-incremental)
        int id;                 // ID da transação
        std::string tipo;       // Tipo da transação ("Credito" / "Debito" ou similar)
        double valor;           // Valor da transação
        Estudante* origem;      // Ponteiro para o Estudante que originou a transação
        std::string data;       // Data da transação (representada como string)

    public:
        // Construtor:
        //  _tipo: descrição do tipo da transação
        //  _valor: valor monetário da transação
        //  _data: data como string (espera-se formato válido, validado externamente)
        Transacao(const std::string &_tipo, const double &_valor, std::string& _data);

        // Destrutor padrão
        ~Transacao();

        // ========== GETTERS ==========
        int get_id() const;                      // Retorna o ID da transação
        std::string get_tipo_transacao() const;  // Retorna o tipo da transação
        double get_valor_transacao() const;      // Retorna o valor
        Estudante* get_origem_transacao() const; // Retorna ponteiro para o estudante origem
        std::string get_data();                  // Retorna a data como string

        // ========== SETTERS ==========
        void set_id(const int &_id);                         // Define ID manualmente (uso: restauração)
        void set_tipo_transacao(const std::string &_tipo_transacao); // Altera o tipo
        void set_valor(const double &_valor);                // Altera o valor
        void set_origem(Estudante &_origem);                 // Define origem da transação
        void set_data(std::string &_data);                   // Altera a data

        // Exibe a transação formatada no console (utilitário)
        void exibir_transacao();
};

#endif
