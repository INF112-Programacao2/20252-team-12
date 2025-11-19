#include "EstudanteGraduacao.hpp"

double EstudanteGraduacao::valorRU = 5.40;

EstudanteGraduacao::EstudanteGraduacao(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso,const std::string& _modalidade):
    Estudante(_nome, _cpf,_data_de_nascimento, _email, _senha,_matricula, _curso), modalidade(_modalidade){}

EstudanteGraduacao::~EstudanteGraduacao(){}

std::string EstudanteGraduacao::get_modalidade() const{
    return this->modalidade;
}

int EstudanteGraduacao::get_valorRU() const {
    return this->valorRU;
}

void EstudanteGraduacao::set_modalidade(std::string _modalidade){
    this->modalidade = _modalidade;
}

void EstudanteGraduacao::comerRU() {
    this->get_carteirinha()->debitar(this->get_valorRU());
}