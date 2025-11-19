#include "Carteirinha.hpp"

// Atributos estáticos da classe para mudar em todas os objetos
int Carteirinha::nextId = 1;

Carteirinha::Carteirinha(){
    this->id = nextId++;
    this->saldo = 0.0;
}

Carteirinha::~Carteirinha(){}

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
    this->saldo += _valor;
}

void Carteirinha::debitar(double _valor){
    this->saldo -= _valor;
}

void Carteirinha::exibir_extrato(){
    for (auto transacao : this->extrato){
        transacao->exibir_transacao();
    }
}