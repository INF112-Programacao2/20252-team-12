#include <iostream>
#include <vector>
#include "Usuario.hpp"

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

    // ano mínimo razoável e ano máximo = ano atual
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

Usuario::Usuario(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha) : nome(_nome), cpf(_cpf),email(_email), senha(_senha) {
    if (validaData(_data_de_nascimento)){
        this->data_de_nascimento = _data_de_nascimento;
    }
}

Usuario::~Usuario(){}

std::string Usuario::getNome(){
    return this->nome;
}

std::string Usuario::getDataDeNascimento(){
    return this->data_de_nascimento;
}

std::string Usuario::getEmail(){
    return this->email;
}

std::string Usuario::getSenha(){
    return this->senha;
}

std::string Usuario::getCpf() {
    return this->cpf;
}

void Usuario::setNome(std::string _nome){
    this->nome = _nome;
}

void Usuario::setDataDeNascimento(std::string _data_de_nascimento){
    if (validaData(_data_de_nascimento)){
        this->data_de_nascimento = _data_de_nascimento;
    }
}

void Usuario::setEmail(std::string _email){
    this->email = _email;
}

void Usuario::setSenha(std::string _senha){
    this->senha = _senha;
}