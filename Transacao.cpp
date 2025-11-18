#include "Transacao.hpp"
#include <iostream>

Transacao::Transacao(const int &_id, const std::string &_tipo, const double &_valor, Estudante &_origem, const std::time_t &_data):
    id(_id), tipo(_tipo), valor(_valor), origem(&_origem), data(_data){}

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

std::time_t Transacao::get_data() const{
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

void Transacao::set_data(std::time_t &_data) {
    this->data= _data;
}

void Transacao::exibir_transacao(){
    
}