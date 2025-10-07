#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>

// Classe Usuario

class Usuario{

    private:
        std::string nome;                           // Lembrar de ler nome como uma linha inteira
        std::string data_de_nascimento;             // e dar cin.ignore() depois.
        std::string email;
        std::string senha;

        std::string getSenha();                     // Senha é privado e apenas a própria
        void setSenha(std::string _senha);          // classe e suas filhas podem pegar e alterar.

    public:
        Usuario(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha);
        ~Usuario();

        std::string getNome();
        std::string getDataDeNascimento();
        std::string getEmail();
        
        void setNome(std::string _nome);
        void setDataDeNascimento(std::string _data_de_nascimento);
        void setEmail(std::string _email);
};

#endif