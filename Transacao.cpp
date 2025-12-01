#include "Transacao.hpp"
#include "Auxiliares.hpp"
#include <iostream>
#include <iomanip> // Para std::setprecision

int Transacao::nextID = 1;


Transacao::Transacao(const std::string &_tipo, const double &_valor, const std::string& _data):
    id(nextID++), tipo(_tipo), valor(_valor){
        if (validarDATA(_data)){
            this->data = _data;
        }
    }

Transacao::~Transacao(){}


int Transacao::get_id() const{
    return this->id;
}

std::string Transacao::get_tipo_transacao() const {
    return this->tipo;
}

double Transacao::get_valor_transacao() const {
    return this->valor;
}

Estudante* Transacao::get_origem_transacao() const {
    return this->origem;
}

std::string Transacao::get_data() const{
    return this->data;
}

void Transacao::set_id(const int &_id) {
    this->id = _id;
}

void Transacao::set_tipo_transacao(const std::string &_tipo_transacao){
    this->tipo = _tipo_transacao;
}

void Transacao::set_valor(const double &_valor){
    this->valor = _valor;
}

void Transacao::set_origem(Estudante &_origem){
    this->origem = &_origem;
}

void Transacao::set_data(std::string &_data) {
    this->data= _data;
}

void Transacao::exibir_transacao() {
    std::cout << "Tipo: " << this->tipo << " | Valor: R$ " << std::fixed << std::setprecision(2) << this->valor << " | Data: " << this->data << "\n";
}

