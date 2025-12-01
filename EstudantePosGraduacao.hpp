#ifndef ESTUDANTEPOSGRADUCAO_HPP
#define ESTUDANTEPOSGRADUCAO_HPP

#include "Estudante.hpp"
//(thales)
class EstudantePosGraduacao : public Estudante
{
private:
    static double valorRU;
    std::string tipoPos;
    std::string linhaDePesquisa;
    static int prazoDeDevolucao;

public:
    EstudantePosGraduacao(const std::string &_nome, const std::string &_cpf, const std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha, const std::string &_matricula, const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa);
    ~EstudantePosGraduacao();
    double get_valorRU() const;
    std::string get_tipoPos() const;
    std::string get_linhaDePesquisa() const;
    int get_prazoDeDevolucao() const override;

    void set_tipoPos(std::string _tipoPos);
    void set_linhaDePesquisa(std::string _linhaDePesquisa);
    static void set_valorRU(double _valorRU);

    void comerRU() override;
};

#endif