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
        int id;
    public:
        Administrador(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const int &_id);
        ~Administrador();

        int get_id() const;
        void set_id(const int &_id);

        //outros metodos - vou deixar apenas declarado, podendo haver falta de parâmetros --- favor verificar (thales)
        void criarLivro(Biblioteca &biblioteca);
        void criarEstudante();
        void criarAdministrador();
        void listarEstudante();
        void alterarSenhaEstudante(Usuario& _estudante, const std::string &_nova_senha);
        void alterarSenhaAdministrador(Usuario& _administrador, const std::string &_nova_senha);
        void consultarTransacoes();
        void consultarEmprestimos(Estudante &_estudante);
        void recarregarCarteirinha(Estudante &_estudante, double _valor_depositado); //acredito que seja esse o parametro, mas também acho que precisaria receber alguma coisa relacionada a transacao(thales)
        void alterarValorRU(Estudante &_estudante, double novo_valor); //coloquei esse como parâmetro também mas não tenho certeza porque pode ser tanto PosGraduacao quanto Graduacao (thales)
        void alterarValorMulta();
};

#endif