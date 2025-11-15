#ifndef ESTUDANTE_HPP
#define ESTUDANTE_HPP
#include "Usuario.hpp"
#include "Carteirinha.hpp"
#include "Emprestimo.hpp"
#include "Biblioteca.hpp"
#include <string>
#include <vector>

class Carteirinha;
class Emprestimo;

class Estudante : public Usuario{
    private:
        std::string matricula;
        std::string curso;
        Carteirinha* carteirinha;
        std::vector<Emprestimo*> empréstimos;
    public:
        Estudante(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string& _matricula, const std::string& _curso);
        ~Estudante();

        void exibirEmpréstimos();
        void pegarLivro(const Biblioteca& biblioteca);
        void devolverLivro(const Biblioteca& biblioteca);
        void consultarSaldo();
        void recarregarCarteirinha();
        virtual void comerRU();
};

#endif