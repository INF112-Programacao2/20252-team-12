#ifndef ESTUDANTEGRADUACAO_HPP
#define ESTUDANTEGRADUACAO_HPP

#include "Estudante.hpp"

class EstudanteGraduacao : public Estudante {
    private:
        std::string modalidade;
        static double valorRU;
        static int prazoDeDevolucao;

    public:
        EstudanteGraduacao(const std::string& _nome, const std::string &_cpf, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const std::string& _modalidade);
        ~EstudanteGraduacao();

        std::string get_modalidade() const;
        int get_valorRU() const;
        int get_prazoDeDevolucao() const override;

        void set_modalidade(std::string _modalidade);
        static void set_valorRU(double _valorRU);

        void comerRU() override;
};

#endif