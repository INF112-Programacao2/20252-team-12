#ifndef SISTEMA_HPP
#define SISTEMA_HPP

#include "Estudante.hpp"
#include "Administrador.hpp"
#include "Biblioteca.hpp"
#include <vector>
#include <fstream>

class Sistema
{
private:
    std::ifstream arquivo_livros;
    std::vector<Estudante *> estudantes;
    Estudante *estudante_logado;
    Administrador *admin;
    Biblioteca *biblioteca;

    void carregarLivros();
    void carregarDados();
    void salvarDados();
    void carregarAdmin();
    void salvarAdmin();
    void menuAdministrador();
    void menuEstudante();
    void carregarEmprestimos();
    void salvarEmprestimos();

public:
    Sistema();
    ~Sistema();

    std::vector<Estudante *> get_estudantes();
    Administrador *get_admin();

    void iniciarSistema();
};

#endif