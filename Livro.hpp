#ifndef LIVRO_HPP
#define LIVRO_HPP

#include <string>

class Livro{
    private:
        static int nextId;
        int id;
        std::string titulo;
        std::string autor;
        std::string tipo; //não seria melhor gênero? (thales)
        bool disponivel;
        int numExemplaresTotal;
        int numExemplaresDisponiveis;
    public:
        Livro(const std::string& _titulo, const std::string& _autor, const std::string& _tipo, const int& _numExemplaresTotal);
        ~Livro();

        int getId();
        std::string getTitulo();
        std::string getAutor();
        std::string getTipo();
        bool isDisponivel();
        int getNumExemplaresTotal();
        int getNumExemplaresDisponiveis();

        void setId(int _id);
        void setTitulo(std::string _titulo);
        void setAutor(std::string _autor);
        void setTipo(std::string _tipo);
        void setDisponivel();
        void setNumExemplaresTotal(int _numExemplaresTotal);
        void setNumExemplaresDisponiveis(int _numExemplaresDisponiveis);
        
        void exibirInformacoes();
};

#endif