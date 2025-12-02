#include "Carteirinha.hpp"
#include "Auxiliares.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>
#include <sstream>


// Atributos estáticos da classe para mudar em todas os objetos
int Carteirinha::nextId = 1;

Carteirinha::Carteirinha() : extrato(){
    this->id = nextId++;
    this->saldo = 0.0;
}

Carteirinha::~Carteirinha(){
    for (auto transacao : this->extrato){
        if (transacao != nullptr) {
            delete transacao;
        }
    }
    this->extrato.clear();
}

double Carteirinha::getSaldo(){
    return this->saldo;
}

std::vector<Transacao*> Carteirinha::getExtrato(){
    return this->extrato;
}

void Carteirinha::adicionarTransacao(Transacao* t) {
    this->extrato.push_back(t);
}

void Carteirinha::setSaldo(double _saldo){
    this->saldo = _saldo;
}

void Carteirinha::depositar(double _valor){
    std::string data_atual = getDataAtual();
    Transacao* nova_transacao = new Transacao("Crédito", _valor, data_atual);
    this->extrato.push_back(nova_transacao);
    this->setSaldo(this->getSaldo() + _valor);
}

void Carteirinha::debitar(double _valor){
    std::string data_atual = getDataAtual();
    Transacao* nova_transacao = new Transacao("Débito", _valor, data_atual);
    this->extrato.push_back(nova_transacao);
    this->setSaldo(this->getSaldo() - _valor);
}

void Carteirinha::exibir_extrato(){
    if (this->extrato.empty()) {
        std::cout << "[INFO] Nenhuma transação registrada.\n";
        return;
    }
    std::cout << "\n=== EXTRATO DA CARTEIRINHA ===\n";
    std::cout << "Total de transações: " << this->extrato.size() << "\n";
    std::cout << "------------------------------------\n";

    for (size_t i = 0; i < this->extrato.size(); i++){
        this->extrato[i]->exibir_transacao();
    }

    std::cout << "------------------------------------\n";
    std::cout << "Saldo atual: R$ " << std::fixed << std::setprecision(2) << this->saldo << "\n";
}