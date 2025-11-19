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

static time_t converterStringParaData(const std::string& dataStr) {
    struct tm tm = {0};
    if (sscanf(dataStr.c_str(), "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3) {
        throw std::invalid_argument("Falha ao interpretar a data. Use D/M/YYYY ou DD/MM/YYYY");
    }
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    tm.tm_isdst = -1;
    time_t t = mktime(&tm);
    if (t == (time_t)-1) {
        throw std::runtime_error("Conversão de data inválida");
    }
    return t;
}

Usuario::Usuario(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha) : nome(_nome), cpf(_cpf),email(_email), senha(_senha) {
    if (validaData(_data_de_nascimento)){
        this->data_de_nascimento = converterStringParaData(_data_de_nascimento);
    }
}

Usuario::~Usuario(){}

std::string Usuario::getNome(){
    return this->nome;
}

time_t Usuario::getDataDeNascimento(){
    return this->data_de_nascimento;
}

std::string Usuario::getDataDeNascimentoFormatada(){
    char buffer[80];
    std::tm timeinfo;

    #if defined(_MSC_VER)
        localtime_s(&timeinfo, &this->data_de_nascimento);
    #elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
        localtime_r(&this->data_de_nascimento, &timeinfo);
    #else
        std::tm *tmp = std::localtime(&this->data_de_nascimento);
        if (!tmp) return std::string();
        timeinfo = *tmp;
    #endif

    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
    return std::string(buffer);
}

std::string Usuario::getEmail(){
    return this->email;
}

std::string Usuario::getSenha(){
    return this->senha;
}

void Usuario::setNome(std::string _nome){
    this->nome = _nome;
}

void Usuario::setDataDeNascimento(std::string _data_de_nascimento){
    if (validaData(_data_de_nascimento)){
        this->data_de_nascimento = converterStringParaData(_data_de_nascimento);
    }
}

void Usuario::setEmail(std::string _email){
    this->email = _email;
}

void Usuario::setSenha(std::string _senha){
    this->senha = _senha;
}