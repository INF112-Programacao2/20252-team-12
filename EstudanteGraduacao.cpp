#include "EstudanteGraduacao.hpp"

EstudanteGraduacao::EstudanteGraduacao(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const int &_periodoAtual, const int &_valorRU):
    Estudante(_nome, _data_de_nascimento, _email, _senha,_matricula, _curso), periodoAtual(_periodoAtual), valorRU(_valorRU){}

EstudanteGraduacao::~EstudanteGraduacao(){}

int EstudanteGraduacao::get_periodoAtual() const{
    return this->periodoAtual;
}

int EstudanteGraduacao::get_valorRU() const {
    return this->valorRU;
}

void EstudanteGraduacao::set_periodoAtual(int _periodoAtual){
    this->periodoAtual = _periodoAtual;
}

void EstudanteGraduacao::set_valorRU(int _valorRU){
    this->valorRU = _valorRU;
}

void EstudanteGraduacao::comerRU() override {
    
}