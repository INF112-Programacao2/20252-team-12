#include "Sistema.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <limits>
#include <stdexcept>
#include <cstdlib>

Sistema::Sistema() : estudantes(){
    this->arquivo_inicializador = std::ifstream("inicializador.txt");
    this->biblioteca = new Biblioteca("Biblioteca UFV");
    this->admin = new Administrador("Julio Cesar Soares dos Reis", "49588826691", "23/04/1988", "jreis@ufv.br", "admin");
    this->estudante_logado = nullptr;
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

void Sistema::iniciarSistema(){
    // Criar menu, login, etc
    char opcao;
    std::string email;
    std::string senha;
    
    std::cin.exceptions(std::ios::failbit | std::ios::badbit);

    while (1) {
        // --- Menu de Boas-Vindas ---
        std::cout << "\n============================================\n";
        std::cout << "      📚 BEM-VINDO AO SISTEMA-UFV 📚\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Fazer Login\n";
        std::cout << "2 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try {
            // Tenta ler a opção do usuário
            std::cin >> opcao;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer

            if (opcao == '2') {
                std::cout << "\n👋 Obrigado por usar o SISTEMA-UFV. Até logo!\n";
                break;
            }
            if (opcao == '1') {
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
                            std::cout << "\n✅ Bem-Vindo " << this->admin->getNome() << "!\n";
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
                                std::cout << "\n✅ Bem-Vindo " << this->estudante_logado->getSenha() << "!\n";
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
                        break;
                    }
                }
            } else {
                throw("\n⚠️ Opção inválida. Por favor, escolha 1 ou 2.\n");
            }
        } catch (const std::exception& e) {
            #if defined(_WIN32) || defined(_WIN64)       // Verifica o sistema operacional e limpa a tela
                std::system("cls");
            #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
                std::system("clear");
            #endif
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        } catch (...) {
            #if defined(_WIN32) || defined(_WIN64)      // Verifica o sistema operacional e limpa a tela
                std::system("cls");
            #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
                std::system("clear");
            #endif
            std::cerr << "\n❌ Erro desconhecido ocorreu.\n";
        }
    }
}