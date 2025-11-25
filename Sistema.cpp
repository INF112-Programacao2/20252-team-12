#include "Sistema.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>

void escreveDevagar(const std::string &texto, int ms){
    for (char c:texto){
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void pausa(int seg){
    std::this_thread::sleep_for(std::chrono::seconds(seg));
}

Sistema::Sistema() : estudantes(){
    this->arquivo_inicializador = std::ifstream("inicializador.txt");
    this->biblioteca = new Biblioteca("Biblioteca UFV");
    this->admin = new Administrador("Julio Cesar Soares dos Reis", "49588826691", "23/04/1988", "jreis@ufv.br", "admin");
    this->estudante_logado = nullptr;
    EstudanteGraduacao* estudante = new EstudanteGraduacao("Luiz Filipe Santos Oliveira", "14422059629", "22/09/2006", "luiz.s.oliveira@ufv.br", "luiz", "120553", "CCP", "SISU");
    this->estudantes.push_back(estudante);
    this->criarLivros();
}

Sistema::~Sistema(){
    for (auto estudante : this->estudantes){
        delete estudante;
    }
    delete this->admin;
    delete this->biblioteca;
}

std::vector<Estudante*> Sistema::get_estudantes(){
    return this->estudantes;
}

Administrador* Sistema::get_admin(){
    return this->admin;
}

static void apagarTerminal(){
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
        std::system("clear");
    #endif
}

void Sistema::criarLivros(){
    if (!this->arquivo_inicializador.is_open()){
        throw std::invalid_argument("Arquivo não encontrado!");
        return;
    }

    std::string linha;
    while (std::getline(this->arquivo_inicializador, linha)) {
        std::stringstream ss(linha);
        std::string campo;

        std::string titulo, autor, tipo;
        int numExemplaresTotal = 0;
        
        std::vector<std::string> campos;

        while (std::getline(ss, campo, ',')) {
            campos.push_back(campo);
        }
        if (campos.size() == 4) {
            titulo = campos[0];
            autor = campos[1];
            tipo = campos[2];
            
            try {
                numExemplaresTotal = std::stoi(campos[3]);
            } catch (const std::exception& e) {
                throw std::runtime_error("Aviso: Linha mal formatada ou incompleta pulada: " + linha);
                continue;
            }

            // Cria um novo objeto Livro e adiciona ao vetor
            Livro* novoLivro = new Livro(titulo, autor, tipo, numExemplaresTotal);
            this->biblioteca->adicionarLivro(*novoLivro);

        } else {
            throw std::runtime_error("Aviso: Linha mal formatada ou incompleta pulada: " + linha);
        }
    }
    this->arquivo_inicializador.close();
}

void Sistema::menuAdministrador(){
    int opcao;
    while (1){
        std::cout << "\n============================================\n";
        std::cout << "👨 BEM-VINDO AO PAINEL DE ADMINISTRADOR 👨 \n";
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Criar Livro\n";
        std::cout << "2 - Cadastrar Estudante\n";
        std::cout << "3 - Listar Estudantes\n";
        std::cout << "4 - Listar Livros\n";
        std::cout << "5 - Alterar Dados de um Estudante\n";
        std::cout << "6 - Alterar Sua Senha\n";
        std::cout << "7 - Consultar Transações\n";
        std::cout << "8 - Consultar Empréstimos\n";
        std::cout << "9 - Recarregar Uma Carteirinha\n";
        std::cout << "10 - Alterar o Valor do RU\n";
        std::cout << "11 - Alterar o Valor da Multa do Empréstimo\n";
        std::cout << "12 - Voltar\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try{
            if (!(std::cin >> opcao)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            apagarTerminal();
            switch (opcao) {
                case 1:
                    this->admin->criarLivro(*this->biblioteca);
                    apagarTerminal();
                    break;
                case 2:
                    this->admin->criarEstudante(this->estudantes);
                    apagarTerminal();
                    break;
                case 3:
                    this->admin->listarEstudante(this->estudantes);
                    apagarTerminal();
                    break;
                case 4:
                    this->biblioteca->listarLivros();
                    apagarTerminal();
                    break;
                case 5:
                    this->admin->alterarDadosEstudante(this->estudantes);
                    apagarTerminal();
                    break;
                case 6:
                    this->admin->alterarSenhaAdministrador();
                    apagarTerminal();
                    break;
                case 7:
                    this->admin->consultarTransacoes(this->estudantes);
                    break;
                case 8:
                    this->admin->consultarEmprestimos(this->estudantes);
                    apagarTerminal();
                    break;
                case 9:
                    this->admin->recarregarCarteirinha(this->estudantes);
                    apagarTerminal();
                    break;
                case 10:
                    this->admin->alterarValorRU();
                    apagarTerminal();
                    break;
                case 11:
                    this->admin->alterarValorMulta();
                    apagarTerminal();
                    break;
                case 12:
                apagarTerminal();
                    return;
                default:
                    throw std::invalid_argument("Digite um número válido!");
                    break;
            }
        } catch (const std::exception& e){
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

void Sistema::iniciarSistema(){
    int opcao;
    std::string email;
    std::string senha;

    while (1) {
        std::cout << "\n============================================\n";
        std::cout << "      📚 BEM-VINDO AO SISTEMA-UFV 📚\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Fazer Login\n";
        std::cout << "2 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try {
            if (!(std::cin >> opcao)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (opcao == 2) {
                std::cout << "\n👋 Obrigado por usar o SISTEMA-UFV. Até logo!\n";
                break;
            }
            else if (opcao == 1) {
                bool logado = false;
                while (!logado) {
                    std::string email;
                    std::string senha;

                    std::cout << "--------------------------------------------\n";
                    std::cout << "Email: ";
                    std::getline(std::cin, email);
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Senha: ";
                    std::getline(std::cin, senha);
                    std::cout << "--------------------------------------------\n";

                    // 1. Tenta Login como Administrador
                    try{
                        if (this->admin->getEmail() == email && this->admin->getSenha() == senha) {
                            // (Thales) coloca pra escrever devagar
                            // A fazer
                            escreveDevagar("\n✅ Bem-Vindo " + this->admin->getNome(), 50);
                            pausa(1);
                            // Chamar menu admin
                            #if defined(_WIN32) || defined(_WIN64)  // Verifica o sistema operacional e limpa a tela
                                std::system("cls");
                            #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
                                std::system("clear");
                            #endif
                            logado = true;
                            break;
                        }
                        // 2. Se falhou como Admin, tenta Login como Estudante
                        else {
                            for (auto estudante : this->estudantes){
                                if (estudante->getEmail() == email && estudante->getSenha() == senha){
                                    this->estudante_logado = estudante;
                                }
                            }
                            if (estudante_logado != nullptr) {
                                // (Thales) coloca pra escrever devagar
                                escreveDevagar( "\n✅ Bem-Vindo " + this->estudante_logado->getSenha(), 50);
                                pausa(1);
                                // Chama o menu do estudante
                                logado = true;
                                break;
                            } else {
                                throw std::invalid_argument("\n❌ Erro: Credenciais inválidas. Tente novamente.\n");
                            }
                        }
                    } catch (const std::exception& e) {
                        #if defined(_WIN32) || defined(_WIN64)       // Verifica o sistema operacional e limpa a tela
                            std::system("cls");
                        #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
                            std::system("clear");
                        #endif
                        std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
                    if (this->admin->getEmail() == email && this->admin->getSenha() == senha) {
                        std::cout << "\n✅ Bem-Vindo " << this->admin->getNome() << "!\n";
                        apagarTerminal();
                        this->menuAdministrador();
                        logado = true;
                        break;
                    }
                    else {
                        bool encontrado = false;
                        for (auto estudante : this->estudantes){
                            if (estudante->getEmail() == email && estudante->getSenha() == senha){
                                this->estudante_logado = estudante;
                                encontrado = true;
                                break;
                            }
                        }
                        if (encontrado && this->estudante_logado != nullptr) {
                            std::cout << "\n✅ Bem-Vindo " << this->estudante_logado->getNome() << "!\n"; // <-- getNome() em vez de getSenha()
                            apagarTerminal();
                            // Chama o menu do estudante aqui
                            logado = true;
                            break;
                        } else {
                            throw std::invalid_argument("Credenciais inválidas. Tente novamente.\n");
                        }
                    }
                }
            }
            else {
                throw std::invalid_argument("⚠️ Opção inválida. Por favor, escolha 1 ou 2.\n");
            }
        } catch (const std::exception& e) {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        } catch (...) {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro desconhecido!\n";
        }
    }
}