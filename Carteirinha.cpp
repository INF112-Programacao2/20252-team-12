#include "Carteirinha.hpp"
#include <chrono>
#include <iostream>
#include <iomanip>

static std::string getDataAtual() {
    auto agora = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(agora);

    std::tm* data = std::localtime(&tt);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << data->tm_mday << "/"
       << std::setw(2) << data->tm_mon + 1 << "/"
       << data->tm_year + 1900;

    return ss.str();
}

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

void Carteirinha::setSaldo(double _saldo){
    this->saldo = _saldo;
}

void Carteirinha::depositar(double _valor){
    Transacao* nova_transacao = new Transacao("Crédito", _valor, getDataAtual());
    this->extrato.push_back(nova_transacao);
    this->setSaldo(this->getSaldo() + _valor);
}

void Carteirinha::debitar(double _valor){
    Transacao* nova_transacao = new Transacao("Débito", _valor, getDataAtual());
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

    for (int i = 0; i < this->extrato.size(); i++){
        this->extrato[i]->exibir_transacao();
    }

    std::cout << "------------------------------------\n";
    std::cout << "Saldo atual: R$ " << std::fixed << std::setprecision(2) << this->saldo << "\n";
}