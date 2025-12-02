#include "Transacao.hpp"
#include "Auxiliares.hpp"
#include <iostream>
#include <iomanip> // Para std::setprecision

// Contador estático para gerar IDs únicos de transações
int Transacao::nextID = 1;

// Construtor:
//  - Recebe tipo ("Crédito" / "Débito"), valor e data (string).
//  - Valida a data com validarDATA antes de armazenar.
//  - Atribui um ID único auto-incremental.
Transacao::Transacao(const std::string &_tipo, const double &_valor, std::string& _data):
    id(nextID++), tipo(_tipo), valor(_valor){
        if (validarDATA(_data)){
            this->data = _data;
        }
    }

// Destrutor (vazio, sem recursos dinâmicos próprios)
Transacao::~Transacao(){}

// ========== GETTERS ==========

// Retorna o ID da transação
int Transacao::get_id() const{
    return this->id;
}

// Retorna o tipo da transação (string)
std::string Transacao::get_tipo_transacao() const {
    return this->tipo;
}

// Retorna o valor da transação (double)
double Transacao::get_valor_transacao() const {
    return this->valor;
}

// Retorna ponteiro para o estudante origem da transação
Estudante* Transacao::get_origem_transacao() const {
    return this->origem;
}

// Retorna a data da transação (string)
std::string Transacao::get_data(){
    return this->data;
}

// ========== SETTERS ==========

// Define manualmente o ID (uso específico: restauração de arquivo)
void Transacao::set_id(const int &_id) {
    this->id = _id;
}

// Altera o tipo da transação
void Transacao::set_tipo_transacao(const std::string &_tipo_transacao){
    this->tipo = _tipo_transacao;
}

// Altera o valor da transação
void Transacao::set_valor(const double &_valor){
    this->valor = _valor;
}

// Define a origem (Estudante) desta transação
void Transacao::set_origem(Estudante &_origem){
    this->origem = &_origem;
}

// Define/atualiza a data da transação
void Transacao::set_data(std::string &_data) {
    this->data= _data;
}

// Exibe a transação formatada no console:
//  Tipo | Valor (com 2 decimais) | Data
void Transacao::exibir_transacao() {
    std::cout << "Tipo: " << this->tipo << " | Valor: R$ " << std::fixed << std::setprecision(2) << this->valor << " | Data: " << this->data << "\n";
}

