#ifndef ESTUDANTEPOSGRADUCAO_HPP
#define ESTUDANTEPOSGRADUCAO_HPP

#include "Estudante.hpp"
//(thales)
class EstudantePosGraduacao : public Estudante {
    private:
        static double valorRU;
        std::string tipoPos;
        std::string linhaDePesquisa;
    public:
        EstudantePosGraduacao(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string &_matricula, const std::string &_curso, const std::string &_tipoPos, const std::string &_linhaDePesquisa);
        ~EstudantePosGraduacao();
        int get_valorRU() const;
        std::string get_tipoPos() const;
        std::string get_linhaDePesquisa() const;
        
        //(thales) == o metodo set_valorRU é dessa classe ou de outra acima? -- vou deixar já implementado... tirem se não precisar
        void set_tipoPos(std::string _tipoPos);
        void set_linhaDePesquisa(std::string _linhaDePesquisa);
        
        void comerRU() override;
};

#endif