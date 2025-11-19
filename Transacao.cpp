#include "Transacao.hpp"
#include <iostream>

int Transacao::nextID = 1;

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delim, start);
        if (pos == std::string::npos) {
            elems.push_back(s.substr(start));
            break;
        }
        elems.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return elems;
}

static bool validaData(const std::string& data) {
    // aceita D/M/YYYY, DD/MM/YYYY, com '/' como separador
    auto parts = split(data, '/');
    if (parts.size() != 3) {
        throw std::invalid_argument("Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    } catch (...) {
        throw std::invalid_argument("Data contém caracteres inválidos");
    }
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual) {
        throw std::invalid_argument("Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("Mês inválido");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        throw std::invalid_argument("Dia inválido para o mês especificado");
    }

    return true;
}

Transacao::Transacao(const std::string &_tipo, const double &_valor, Estudante &_origem, const std::string& _data):
    id(nextID++), tipo(_tipo), valor(_valor), origem(&_origem){
        if (validaData(_data)){
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

void Transacao::exibir_transacao(){
    std::cout << this->origem->getNome()<<" realizou uma transação do tipo "<< this->get_tipo_transacao()<<" com o valor de "<<this->get_valor_transacao()<<" na data "<<this->get_data()<<" com o id: "<<this->get_id()<<std::endl;
}

