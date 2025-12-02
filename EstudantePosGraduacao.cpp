#include "EstudantePosGraduacao.hpp"
#include "Auxiliares.hpp"
#include <stdexcept>
#include <thread>
#include <chrono>
#include <iostream>

double EstudantePosGraduacao::valorRU = 10.80;
int EstudantePosGraduacao::prazoDeDevolucao = 14;

EstudantePosGraduacao::EstudantePosGraduacao(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha, const std::string &_matricula, const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa) : Estudante(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso), tipoPos(_tipoPos), linhaDePesquisa(_linhaDePesquisa) {} // usei o construtor da classe Estudante

EstudantePosGraduacao::~EstudantePosGraduacao() {}

double EstudantePosGraduacao::get_valorRU() const
{
    return this->valorRU;
}

std::string EstudantePosGraduacao::get_tipoPos() const
{
    return this->tipoPos;
}

std::string EstudantePosGraduacao::get_linhaDePesquisa() const
{
    return this->linhaDePesquisa;
}

int EstudantePosGraduacao::get_prazoDeDevolucao() const
{
    return this->prazoDeDevolucao;
}

void EstudantePosGraduacao::set_tipoPos(std::string _tipoPos)
{
    this->tipoPos = _tipoPos;
}

void EstudantePosGraduacao::set_valorRU(double _valorRU)
{
    EstudantePosGraduacao::valorRU = _valorRU;
}

void EstudantePosGraduacao::set_linhaDePesquisa(std::string _linhaDePesquisa)
{
    this->linhaDePesquisa = _linhaDePesquisa;
}

void EstudantePosGraduacao::comerRU()
{
    if (this->get_carteirinha()->getSaldo() < this->get_valorRU())
    {
        throw std::invalid_argument("O seu saldo é inferior ao valor do RU");
    }
    this->get_carteirinha()->debitar(this->get_valorRU());
    escreveDevagar("\n✅ Aproveite sua refeição!\n", 50);
}