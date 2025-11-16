#ifndef ESTUDANTEGRADUACAO_HPP
#define ESTUDANTEGRADUACAO_HPP

#include "Estudante.hpp"

class EstudanteGraduacao : public Estudante {
    private:
        int periodoAtual;
        int valorRU;
    public:
        EstudanteGraduacao(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const int &_periodoAtual, const int &_valorRU);
        ~EstudanteGraduacao();

        int get_periodoAtual() const;
        int get_valorRU() const;

        void set_periodoAtual(int _periodoAtual);
        void set_valorRU(int _valorRU);
        
        void comerRU() override;
};

#endif