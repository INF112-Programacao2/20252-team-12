#include <iostream>
#include "Usuario.hpp"

Usuario::Usuario(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha) : nome(_nome), data_de_nascimento(_data_de_nascimento), email(_email), senha(_senha) {}

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

void Usuario::setNome(std::string _nome){
    this->nome = _nome;
}

void Usuario::setDataDeNascimento(std::string _data_de_nascimento){
    this->data_de_nascimento = _data_de_nascimento;
}

void Usuario::setEmail(std::string _email){
    this->email = _email;
}

void Usuario::setSenha(std::string _senha){
    this->senha = _senha;
}