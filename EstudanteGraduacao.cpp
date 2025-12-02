#include "EstudanteGraduacao.hpp"
#include "Auxiliares.hpp"
#include <stdexcept>
#include <thread>
#include <chrono>
#include <iostream>


double EstudanteGraduacao::valorRU = 5.40;
int EstudanteGraduacao::prazoDeDevolucao = 7;

EstudanteGraduacao::EstudanteGraduacao(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso,const std::string& _modalidade):
    Estudante(_nome, _cpf,_data_de_nascimento, _email, _senha,_matricula, _curso), modalidade(_modalidade){}

EstudanteGraduacao::~EstudanteGraduacao(){}

std::string EstudanteGraduacao::get_modalidade() const{
    return this->modalidade;
}

double EstudanteGraduacao::get_valorRU() {
    return valorRU;
}

int EstudanteGraduacao::get_prazoDeDevolucao() const {
    return this->prazoDeDevolucao;
}

void EstudanteGraduacao::set_modalidade(std::string _modalidade){
    this->modalidade = _modalidade;
}

void EstudanteGraduacao::set_valorRU(double _valorRU){
    EstudanteGraduacao::valorRU = _valorRU;
}

void EstudanteGraduacao::comerRU() {
    if (this->get_carteirinha()->getSaldo() < this->get_valorRU()) {
        throw std::invalid_argument("O seu saldo é inferior ao valor do RU");
    }
    this->get_carteirinha()->debitar(this->get_valorRU());
    escreveDevagar("\n✅ Aproveite sua refeição!\n", 50);
}