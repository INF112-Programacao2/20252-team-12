#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include <ctime>

// ========== CLASSE USUARIO ==========
// Representa dados básicos de um usuário do sistema (pessoa física).
// Responsabilidades:
//  - armazenar nome, CPF, data de nascimento, email e senha
//  - fornecer getters e setters simples
// Validações (como formato de data) são realizadas na implementação (.cpp)
class Usuario {

protected:
    std::string nome;                 // Nome completo do usuário
    std::string cpf;                  // CPF (número identificador)
    std::string data_de_nascimento;   // Data de nascimento (string, ex: "DD/MM/YYYY")
    std::string email;                // Email de contato/login
    std::string senha;                // Senha de acesso

public:
    // Construtor:
    // _nome, _cpf, _data_de_nascimento, _email, _senha — valores iniciais.
    // Observação: _data_de_nascimento é passado por referência não-const
    // porque a implementação atual valida/usa a string diretamente.
    Usuario(const std::string& _nome, const std::string &_cpf, std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha);

    // Destrutor padrão
    ~Usuario();

    // ========== GETTERS ==========
    // Retornam cópias dos campos correspondentes.
    std::string getNome();
    std::string getDataDeNascimento();
    std::string getEmail();
    std::string getSenha();
    std::string getCpf();

    // ========== SETTERS ==========
    // Atualizam os campos. A validação de formato (ex.: data) ocorre na implementação.
    void setNome(std::string _nome);
    void setDataDeNascimento(std::string _data_de_nascimento);
    void setEmail(std::string _email);
    void setSenha(std::string _senha);
};

#endif