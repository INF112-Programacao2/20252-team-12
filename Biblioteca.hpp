#ifndef BIBLIOTECA_HPP
#define BIBLIOTECA_HPP

#include "Livro.hpp"
#include <string>
#include <vector>

class Biblioteca{
    private:
        std::string nome;
        std::vector<Livro*> acervo;
    public:
        Biblioteca(const std::string& _nome);
        ~Biblioteca();

        std::string getNome();
        std::vector<Livro*> getAcervo() const;
        void setNome(std::string _nome);

        void adicionarLivro(Livro& _livro);
        void listarLivrosDisponiveis();
        void listarLivros();
};

#endif