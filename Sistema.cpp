#include "Sistema.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <thread>

static std::string obterDataHora() {
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    
    char buffer[100];
    // Formato: Dia/Mes/Ano Hora:Minuto:Segundo
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", now);
    
    return std::string(buffer);
} 

static void escreveLog(std::string mensagem) {
    std::ofstream fout("actionLog.txt", std::ios::app); 

    if (!fout.is_open()) {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
    }

    fout << "[" << obterDataHora() << "] " << mensagem << std::endl;

    fout.close();
}

static void apagarTerminal(){
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
        std::system("clear");
    #endif
}

void escreveDevagar(const std::string &texto, int ms){
    for (char c : texto){
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
    escreveLog("Administrador Logou no Sistema");
    int opcao;
    while (1){
        //std::cout << "\n============================================\n";
        escreveDevagar("\n============================================\n",10);
        //std::cout << "👨 BEM-VINDO AO PAINEL DE ADMINISTRADOR 👨 \n";
        escreveDevagar("👨 BEM-VINDO AO PAINEL DE ADMINISTRADOR 👨 \n",50);
        //std::cout << "--------------------------------------------\n";
        escreveDevagar("--------------------------------------------\n",10);
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
        std::cout << "12 - Sair\n";
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
            
            //escreveLog("Administrador escolheu a opcao: " + std::to_string(opcao));

            switch (opcao) {
                case 1:
                    escreveLog("Administrador escolheu a opcao: 1 - Criar Livro");
                    this->admin->criarLivro(*this->biblioteca);
                    apagarTerminal();
                    break;
                case 2:
                    escreveLog("Administrador escolheu a opcao: 2 - Cadastrar Estudantes");                
                    this->admin->criarEstudante(this->estudantes);
                    apagarTerminal();
                    break;
                case 3:
                    escreveLog("Administrador escolheu a opcao: 3 - Listar Estudantes");
                    this->admin->listarEstudante(this->estudantes);
                    apagarTerminal();
                    break;
                case 4:
                    escreveLog("Administrador escolheu a opcao: 4 - Listar Livros");
                    this->biblioteca->listarLivros();
                    apagarTerminal();
                    break;
                case 5:{
                    escreveLog("Administrador escolheu a opcao: 5 - Alterar Dados de Estudante");
                    int menuAltetacao = this->admin->alterarDadosEstudante(this->estudantes);
                    escreveLog("Opcao " + std::to_string(menuAltetacao) + " escolhida dentro do menu de alteração de dados do estudante");
                    apagarTerminal();
                    break;
                    }
                case 6:
                    escreveLog("Administrador escolheu a opcao: 6 - Alterar sua Senha");
                    this->admin->alterarSenhaAdministrador();
                    apagarTerminal();
                    break;
                case 7:
                    escreveLog("Administrador escolheu a opcao: 7 - Consultar Transações");
                    this->admin->consultarTransacoes(this->estudantes);
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "\nAperte ENTER para continuar...";
                    std::cin.ignore();
                    apagarTerminal();
                    break;
                case 8:
                    escreveLog("Administrador escolheu a opcao: 8 - Consultar Empréstimos");
                    this->admin->consultarEmprestimos(this->estudantes);
                    std::cout << "\nAperte ENTER para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.ignore();
                    apagarTerminal();
                    break;
                case 9:
                    escreveLog("Administrador escolheu a opcao: 9 - Recarregar uma Carteirinha");
                    this->admin->recarregarCarteirinha(this->estudantes);
                    apagarTerminal();
                    break;
                case 10:{
                    escreveLog("Administrador escolheu a opcao: 10 - Alterar Valor do RU");
                    std::string gradOuPos;
                    gradOuPos = this->admin->alterarValorRU();
                    escreveLog("Valor do RU para a " + gradOuPos + " alterado");
                    apagarTerminal();
                    break;
                    }
                case 11:
                    escreveLog("Administrador escolheu a opcao: 11 - Alterar o valor da Multa do Empréstimo");
                    this->admin->alterarValorMulta();
                    apagarTerminal();
                    break;
                case 12:
                    escreveLog("Administrador escolheu a opcao: 12 - Sair");
                    escreveLog("Logout realizado");
                    escreveDevagar("📤 Fazendo logout...\n", 50);
                    pausa(2);
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

void Sistema::menuEstudante() {
    int opcao;
    while (true) {
        std::cout << "\n============================================\n";
        std::cout << "    🎓 BEM-VINDO AO PAINEL DO ESTUDANTE 🎓 \n";
        std::cout << "    Bem-vindo, " << this->estudante_logado->getNome() << "\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Consultar Saldo\n";
        std::cout << "2 - Recarregar Carteirinha\n";
        std::cout << "3 - Ver Extrato Financeiro\n";
        std::cout << "4 - Ver Meus Empréstimos\n";
        std::cout << "5 - Pegar Livro Emprestado\n";
        std::cout << "6 - Devolver Livro\n";
        std::cout << "7 - Comer no RU\n";
        std::cout << "8 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";
        
        try {
            if (!(std::cin >> opcao)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            apagarTerminal();

            //escreveLog("Estudante escolheu a opção: " + std::to_string(opcao));
            
            switch (opcao) {
                case 1:
                    escreveLog("Estudante Escolheu a Opcao: 1 - Consultar Saldo");
                    this->estudante_logado->consultarSaldo();
                    pausa(2);
                    apagarTerminal();
                    break;
                case 2:
                    escreveLog("Estudante Escolheu a Opcao: 2 - Recarregar Carteirinha");
                    this->estudante_logado->recarregarCarteirinha();
                    pausa(2);
                    apagarTerminal();
                    break;
                case 3:
                    escreveLog("Estudante Escolheu a Opcao: 3 - Ver Extrato Financeiro");
                    this->estudante_logado->get_carteirinha()->exibir_extrato();
                    std::cout << "\nPressione ENTER para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.ignore();
                    apagarTerminal();
                    break;
                case 4:
                    escreveLog("Estudante Escolheu a Opcao: 4 - Ver Meus Empréstimos");
                    this->estudante_logado->exibirEmprestimos();
                    std::cout << "\nPressione ENTER para continuar...";
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cin.ignore();
                    apagarTerminal();
                    break;
                case 5:
                    escreveLog("Estudante Escolheu a Opcao: 5 - Pegar Livro Emprestado");
                    this->estudante_logado->pegarLivro(*this->biblioteca);
                    pausa(2);
                    apagarTerminal();
                    break;
                case 6:
                    escreveLog("Estudante Escolheu a Opcao: 7 - Comer no RU");
                    this->estudante_logado->devolverLivro(*this->biblioteca);
                    pausa(2);
                    apagarTerminal();
                    break;
                case 7:
                    escreveLog("Estudante Escolheu a Opcao: 8 - Sair");
                    this->estudante_logado->comerRU();
                    apagarTerminal();
                    break;
                case 8:
                    escreveLog("Logout realizado");
                    escreveDevagar("📤 Fazendo logout...\n", 50);
                    pausa(2);
                    apagarTerminal();
                    this->estudante_logado = nullptr;
                    return;
                default:
                    throw std::invalid_argument("Digite um número válido!");
            }
        } catch (const std::exception& e) {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

void Sistema::iniciarSistema(){
    escreveLog("Sistema aberto");
    
    int opcao;

    while (1) {
        //std::cout << "\n============================================\n";
        escreveDevagar("\n============================================\n",10);
        //std::cout << "      📚 BEM-VINDO AO SISTEMA-UFV 📚\n";
        escreveDevagar("      📚 BEM-VINDO AO SISTEMA-UFV 📚\n",50);
        //std::cout << "--------------------------------------------\n";
        escreveDevagar("--------------------------------------------\n",10);
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

            std::cin.ignore();

            if (opcao == 2) {
                std::cout << "\n👋 Obrigado por usar o SISTEMA-UFV. Até logo!\n";
                break;
            }
            else if (opcao == 1) {
                bool logado = false;
                
                while (!logado) {
                    std::string email, senha;

                    std::cout << "--------------------------------------------\n";
                    std::cout << "Email: ";
                    std::getline(std::cin, email);                                      // TODO: Tratamento de erro email
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Senha: ";
                    std::getline(std::cin, senha);                                      // TODO: Tratamento de erro senha
                    std::cout << "--------------------------------------------\n";

                    // Tenta Login como Administrador
                    if (this->admin->getEmail() == email && this->admin->getSenha() == senha) {
                        escreveDevagar("\n✅ Bem-Vindo " + this->admin->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuAdministrador();
                        logado = true;
                        escreveLog("Administrador logou no sistema");
                        break;
                    }
                    
                    // Tenta Login como Estudante
                    bool encontrado = false;
                    for (auto estudante : this->estudantes) {
                        if (estudante->getEmail() == email && estudante->getSenha() == senha) {
                            this->estudante_logado = estudante;
                            encontrado = true;
                            escreveLog("Estudante " + estudante->get_matricula() + " logou no sistema");
                            break;
                        }
                    }
                    
                    if (encontrado && this->estudante_logado != nullptr) {
                        escreveDevagar("\n✅ Bem-Vindo " + this->estudante_logado->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuEstudante();
                        logado = true;
                        break;
                    } else {
                        std::cout << "\n❌ Erro: Credenciais inválidas. Tente novamente.\n";
                    }
                }
            }
            else {
                throw std::invalid_argument("⚠️ Opção inválida. Por favor, escolha 1 ou 2.");
            }
        } catch (const std::exception& e) {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        } catch (...) {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro desconhecido!\n";
        }
    }

    escreveLog("Sistema Fechado");
}