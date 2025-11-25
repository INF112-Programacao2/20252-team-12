#ifndef ADMINISTRADOR_HPP
#define ADMINISTRADOR_HPP

#include <string>
#include "Estudante.hpp" //dei include em Estudante porque usamos parâmetros do tipo Estudante, favor incluir  as classes que forem usadas como parametro (thales)
#include "Usuario.hpp"  
#include "EstudanteGraduacao.hpp"
#include "EstudantePosGraduacao.hpp"
class Carteirinha; //usando foward-Declaration para evitar dependencia circular (thales)
class Emprestimo;
class Livro;
class Transacao;
//#include "Carteirinha.hpp"
//#include "Emprestimo.hpp"
//#include "Livro.hpp"
//#include "Transacao.hpp"

#include <string>
#include <iostream>

class Administrador : public Usuario{
    private:
        static int nextID;
        int id;
    public:
        Administrador(const std::string& _nome, const std::string &_cpf, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha);
        ~Administrador();

        int get_id() const;
        void set_id(const int &_id);

        void criarLivro(Biblioteca &biblioteca);
        void criarEstudante(std::vector<Estudante*> &estudantes);
        void listarEstudante(std::vector<Estudante*> &estudantes);
        void alterarDadosEstudante(std::vector<Estudante*> &estudantes);
        void alterarSenhaAdministrador();
        void consultarTransacoes(std::vector<Estudante*> &estudantes);
        void consultarEmprestimos(std::vector<Estudante*> &estudantes);
        void recarregarCarteirinha(std::vector<Estudante*> &estudantes);
        void alterarValorRU();
        void alterarValorMulta();
        void mobilidadeAcademica(EstudanteGraduacao *estudante, std::string parametro);
        std::string procurar_curso_por_codigo(std::string codigo);
        void gerarCarteirinha(Estudante *estudante);
};

#endif