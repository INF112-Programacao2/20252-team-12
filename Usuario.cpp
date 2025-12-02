#include <iostream>
#include "Auxiliares.hpp"
#include <vector>
#include "Usuario.hpp"

// Implementação da classe Usuario
// Responsabilidade: armazenar dados pessoais básicos (nome, CPF, data de nascimento, email, senha)
// Validações: utiliza validarDATA() (definida em Auxiliares) para aceitar somente datas no formato esperado.

// Construtor:
// Recebe nome, cpf, data de nascimento, email e senha.
// Armazena os campos e valida data antes de atribuir.
Usuario::Usuario(const std::string& _nome, const std::string &_cpf, std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha)
    : nome(_nome), cpf(_cpf), email(_email), senha(_senha)
{
    if (validarDATA(_data_de_nascimento)) {    // valida formato DD/MM/YYYY
        this->data_de_nascimento = _data_de_nascimento;
    }
}

// Destrutor padrão (sem alocação dinâmica na classe)
Usuario::~Usuario(){}

// ========== GETTERS ==========
// Métodos simples que retornam cópias dos atributos

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

// ========== SETTERS ==========
// Atualizam campos públicos do usuário. Validam data ao atribuir.

void Usuario::setNome(std::string _nome){
    this->nome = _nome;
}

void Usuario::setDataDeNascimento(std::string _data_de_nascimento){
    if (validarDATA(_data_de_nascimento)) {   // só atualiza se formato for válido
        this->data_de_nascimento = _data_de_nascimento;
    }
}

void Usuario::setEmail(std::string _email){
    this->email = _email;
}

void Usuario::setSenha(std::string _senha){
    this->senha = _senha;
}