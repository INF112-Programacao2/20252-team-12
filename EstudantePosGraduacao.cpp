#include "EstudantePosGraduacao.hpp"

double EstudantePosGraduacao::valorRU = 10.80;

EstudantePosGraduacao::EstudantePosGraduacao(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa):
    Estudante(_nome, _data_de_nascimento, _email, _senha, _matricula,_curso), tipoPos(_tipoPos), linhaDePesquisa(_linhaDePesquisa){} //usei o construtor da classe Estudante

EstudantePosGraduacao::~EstudantePosGraduacao(){}

int EstudantePosGraduacao::get_valorRU() const {
    return this->valorRU;
}

std::string EstudantePosGraduacao::get_tipoPos() const {
    return this->tipoPos;
}

std::string EstudantePosGraduacao::get_linhaDePesquisa() const {
    return this->linhaDePesquisa;
}

void EstudantePosGraduacao::set_tipoPos(std::string _tipoPos) {
    this -> tipoPos = _tipoPos;
}

void EstudantePosGraduacao::set_linhaDePesquisa(std::string _linhaDePesquisa) {
    this -> linhaDePesquisa = _linhaDePesquisa;
}

void EstudantePosGraduacao::comerRU() {
    this->get_carteirinha()->debitar(this->get_valorRU());
}