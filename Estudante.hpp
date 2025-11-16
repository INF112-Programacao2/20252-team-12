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
    protected: //alterei private para protected --- atributor passados por herança para as subclasses (thales)
        std::string matricula;
        std::string curso;
        Carteirinha* carteirinha;
        std::vector<Emprestimo*> emprestimos; //recomendo remover o acento (thales)
    public:
        Estudante(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string& _matricula, const std::string& _curso);
        virtual ~Estudante(); //override porque irá chamar o destrutor das subclasses (thales)

        void exibirEmprestimos();
        void pegarLivro(const Biblioteca& biblioteca);
        void devolverLivro(const Biblioteca& biblioteca);
        void consultarSaldo();
        void recarregarCarteirinha();
        virtual void comerRU();

        //(thales) vou adicionar os getters e setters dos atributos de estudante

        //getters
        std::string get_matricula() const;
        std::string get_curso() const;
        Carteirinha* get_carteirinha() const;
        std::vector<Emprestimo*> get_emprestimos() const;

        //setters
        void set_matricula(std::string _matricula);
        void set_curso (std::string _curso);
        void set_carteirinha(Carteirinha *_carteirinha);
};

#endif