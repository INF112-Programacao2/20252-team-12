#ifndef ESTUDANTEGRADUACAO_HPP
#define ESTUDANTEGRADUACAO_HPP

#include "Estudante.hpp"

class EstudanteGraduacao : public Estudante {
    private:
        std::string modalidade;
        static double valorRU;
    public:
        EstudanteGraduacao(const std::string& _nome, const std::string &_cpf, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const std::string& _modalidade);
        ~EstudanteGraduacao();

        std::string get_modalidade() const;
        int get_valorRU() const;

        void set_modalidade(std::string _modalidade);
        
        void comerRU() override;
};

#endif