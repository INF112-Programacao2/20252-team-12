#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <ctime>

// Classe Usuario

class Usuario{

    private:
        std::string nome;
        std::string cpf;
        std::string data_de_nascimento;
        std::string email;
        std::string senha;        

    public:
        Usuario(const std::string& _nome, const std::string &_cpf, std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha);
        ~Usuario();

        std::string getNome();
        std::string getDataDeNascimento();
        std::string getEmail();
        std::string getSenha();  
        std::string getCpf();
        
        void setNome(std::string _nome);
        void setDataDeNascimento(std::string _data_de_nascimento);
        void setEmail(std::string _email);
        void setSenha(std::string _senha);
};

#endif