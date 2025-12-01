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
#include "Auxiliares.hpp"

Sistema::Sistema() : estudantes()
{
    this->biblioteca = new Biblioteca("Biblioteca UFV");
    this->estudante_logado = nullptr;
    this->carregarAdmin();
    this->carregarDados(); // Carrega os estudantes existentes
    this->carregarLivros();
}

Sistema::~Sistema()
{
    this->salvarDados();
    for (auto estudante : this->estudantes)
    {
        delete estudante;
    }
    delete this->admin;
    delete this->biblioteca;
}

void Sistema::salvarDados()
{
    std::ofstream file("banco_estudantes.txt");
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir ficheiro para salvar!\n";
        return;
    }

    for (Estudante *est : this->estudantes)
    {
        EstudanteGraduacao *grad = dynamic_cast<EstudanteGraduacao *>(est);
        EstudantePosGraduacao *pos = dynamic_cast<EstudantePosGraduacao *>(est);

        // Formato: TIPO;NOME;CPF;DATA;EMAIL;SENHA;MATRICULA;CURSO;[ESPECIFICO];SALDO
        if (grad)
        {
            file << "GRAD;"
                 << grad->getNome() << ";"
                 << grad->getCpf() << ";"
                 << grad->getDataDeNascimento() << ";"
                 << grad->getEmail() << ";"
                 << grad->getSenha() << ";"
                 << grad->get_matricula() << ";"
                 << grad->get_curso() << ";"
                 << grad->get_modalidade() << ";"
                 << grad->get_carteirinha()->getSaldo() << "\n";
        }
        else if (pos)
        {
            file << "POS;"
                 << pos->getNome() << ";"
                 << pos->getCpf() << ";"
                 << pos->getDataDeNascimento() << ";"
                 << pos->getEmail() << ";"
                 << pos->getSenha() << ";"
                 << pos->get_matricula() << ";"
                 << pos->get_curso() << ";"
                 << pos->get_tipoPos() << ";"
                 << pos->get_linhaDePesquisa() << ";"
                 << pos->get_carteirinha()->getSaldo() << "\n";
        }
    }
    file.close();

    escreveLog("Dados salvos com sucesso!");
}

void Sistema::carregarDados()
{

    if (this->estudantes.empty())
    {
        EstudanteGraduacao *estudante = new EstudanteGraduacao("Luiz Filipe Santos Oliveira", "14422059629", "22/09/2006", "luiz.s.oliveira@ufv.br", "luiz", "120553", "141", "SISU");
        this->estudantes.push_back(estudante);
        return;
    }

    std::ifstream file("banco_estudantes.txt");
    if (!file.is_open())
    {
        escreveLog("banco_estudantes.txt ainda não existe, sem dados para carregar."); // Pode ser a primeira execução do sistema
        return;
    }

    std::string linha;
    while (std::getline(file, linha))
    {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        while (std::getline(ss, segmento, ';'))
        {
            dados.push_back(segmento);
        }

        if (dados.size() >= 9)
        {
            std::string tipo = dados[0];
            std::string nome = dados[1];
            std::string cpf = dados[2];
            std::string data = dados[3];
            std::string email = dados[4];
            std::string senha = dados[5];
            std::string matricula = dados[6];
            std::string curso = dados[7];

            Estudante *novoEstudante = nullptr;

            if (tipo == "GRAD")
            {
                std::string modalidade = dados[8];
                double saldo = std::stod(dados[9]);

                novoEstudante = new EstudanteGraduacao(nome, cpf, data, email, senha, matricula, curso, modalidade);
                novoEstudante->get_carteirinha()->setSaldo(saldo);
            }
            else if (tipo == "POS")
            {
                std::string tipoPos = dados[8];
                std::string linhaPesquisa = dados[9];
                double saldo = std::stod(dados[10]);

                novoEstudante = new EstudantePosGraduacao(nome, cpf, data, email, senha, matricula, curso, tipoPos, linhaPesquisa);
                novoEstudante->get_carteirinha()->setSaldo(saldo);
            }

            if (novoEstudante != nullptr)
            {
                this->estudantes.push_back(novoEstudante);
            }
        }
    }
    file.close();
}

std::vector<Estudante *> Sistema::get_estudantes()
{
    return this->estudantes;
}

Administrador *Sistema::get_admin()
{
    return this->admin;
}

void Sistema::carregarAdmin()
{
    try
    {
        std::ifstream file("admin_dados.txt");

        if (file.is_open())
        {
            std::string linha;

            if (std::getline(file, linha))
            {
                std::stringstream ss(linha);
                std::string segmento;
                std::vector<std::string> dados;

                while (std::getline(ss, segmento, ';'))
                {
                    limparString(segmento);
                    dados.push_back(segmento);
                }

                if (dados.size() >= 5)
                {
                    this->admin = new Administrador(dados[0], dados[1], dados[2], dados[3], dados[4]);
                    file.close();
                    return;
                }
            }
            file.close();
        }

        escreveLog("Arquivo de admin não encontrado ou inválido. Criando admin padrão.");

        this->admin = new Administrador("Julio Cesar Soares dos Reis", "49588826691", "23/04/1988", "jreis@ufv.br", "admin10");

        this->salvarAdmin();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Sistema::salvarAdmin()
{
    try
    {
        std::ofstream file("admin_dados.txt");
        if (!file.is_open())
        {
            escreveLog("Erro ao abrir arquivo para salvar dados do administrador!");
            std::cerr << "❌ Erro ao salvar dados do administrador.\n";
            return;
        }
        // Salvando no formato: NOME;CPF;DATA;EMAIL;SENHA
        file << admin->getNome() << ";"
             << admin->getCpf() << ";"
             << admin->getDataDeNascimento() << ";"
             << admin->getEmail() << ";"
             << admin->getSenha() << "\n";

        file.close();

        escreveLog("Dados do administrador salvos.");
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Sistema::carregarLivros()
{
    try
    {
        std::ifstream fin("livros.txt");
        if (!fin.is_open())
        {
            throw std::invalid_argument("❌ Arquivo não encontrado!");
            return;
        }

        std::string linha;
        while (std::getline(fin, linha))
        {
            std::stringstream ss(linha);
            std::string campo;

            std::string titulo, autor, tipo;
            int numExemplaresTotal = 0;

            std::vector<std::string> campos;

            while (std::getline(ss, campo, ','))
            {
                campos.push_back(campo);
            }
            if (campos.size() == 4)
            {
                titulo = campos[0];
                autor = campos[1];
                tipo = campos[2];

                try
                {
                    numExemplaresTotal = std::stoi(campos[3]);
                }
                catch (const std::exception &e)
                {
                    throw std::runtime_error("⚠️ Aviso: Linha mal formatada ou incompleta pulada: " + linha);
                    continue;
                }

                Livro *novoLivro = new Livro(titulo, autor, tipo, numExemplaresTotal);
                this->biblioteca->adicionarLivro(*novoLivro);
            }
            else
            {
                throw std::runtime_error("⚠️ Aviso: Linha mal formatada ou incompleta pulada: " + linha);
            }
        }
        fin.close();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Sistema::menuAdministrador()
{
    escreveLog("Administrador Logou no Sistema");
    int opcao;
    while (1)
    {
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("👨 BEM-VINDO AO PAINEL DE ADMINISTRADOR 👨 \n", 50);
        escreveDevagar("--------------------------------------------\n", 10);
        std::cout << "1 - Criar Livro\n";
        std::cout << "2 - Cadastrar Estudante\n";
        std::cout << "3 - Listar Estudantes\n";
        std::cout << "4 - Listar Livros\n";
        std::cout << "5 - Visualizar Carteirinha de um Estudante\n";
        std::cout << "6 - Alterar Dados de um Estudante\n";
        std::cout << "7 - Alterar Sua Senha\n";
        std::cout << "8 - Consultar Transações\n";
        std::cout << "9 - Consultar Empréstimos\n";
        std::cout << "10 - Recarregar Uma Carteirinha\n";
        std::cout << "11 - Alterar o Valor do RU\n";
        std::cout << "12 - Alterar o Valor da Multa do Empréstimo\n";
        std::cout << "13 - Realizar Reopção de Curso\n";
        std::cout << "14 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            apagarTerminal();

            switch (opcao)
            {
            case 1:
                escreveLog("Administrador escolheu a opcao: 1 - Criar Livro");
                this->admin->criarLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 2:
                escreveLog("Administrador escolheu a opcao: 2 - Cadastrar Estudantes");
                this->admin->criarEstudante(this->estudantes);
                this->salvarDados();
                pausa(2);
                apagarTerminal();
                break;
            case 3:
                escreveLog("Administrador escolheu a opcao: 3 - Listar Estudantes");
                this->admin->listarEstudante(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 4:
                escreveLog("Administrador escolheu a opcao: 4 - Listar Livros");
                this->biblioteca->listarLivros();
                pausa(2);
                apagarTerminal();
                break;
            case 5:
                escreveLog("Administrador escolheu a opcao: 5 - Visualizar Carteirinha de um Estudante");
                this->admin->visualizarCarteirinhas(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 6:
            {
                escreveLog("Administrador escolheu a opcao: 6 - Alterar Dados de Estudante");
                int menuAltetacao = this->admin->alterarDadosEstudante(this->estudantes);
                escreveLog("Opcao " + std::to_string(menuAltetacao) + " escolhida dentro do menu de alteração de dados do estudante");
                this->salvarDados();
                pausa(2);
                apagarTerminal();
                break;
            }
            case 7:
                escreveLog("Administrador escolheu a opcao: 7 - Alterar sua Senha");
                this->admin->alterarSenhaAdministrador();
                this->salvarAdmin();
                escreveDevagar("Nova senha salva com sucesso!", 20);
                pausa(2);
                apagarTerminal();
                break;
            case 8:
                escreveLog("Administrador escolheu a opcao: 8 - Consultar Transações");
                this->admin->consultarTransacoes(this->estudantes);
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nAperte ENTER para continuar...";
                std::cin.ignore();
                pausa(2);
                apagarTerminal();
                break;
            case 9:
                escreveLog("Administrador escolheu a opcao: 9 - Consultar Empréstimos");
                this->admin->consultarEmprestimos(this->estudantes);
                std::cout << "\nAperte ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.ignore();
                pausa(2);
                apagarTerminal();
                break;
            case 10:
                escreveLog("Administrador escolheu a opcao: 10 - Recarregar uma Carteirinha");
                this->admin->recarregarCarteirinha(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 11:
            {
                escreveLog("Administrador escolheu a opcao: 11 - Alterar Valor do RU");
                std::string gradOuPos;
                gradOuPos = this->admin->alterarValorRU();
                escreveLog("Valor do RU para a " + gradOuPos + " alterado");
                pausa(2);
                apagarTerminal();
                break;
            }
            case 12:
                escreveLog("Administrador escolheu a opcao: 12 - Alterar o valor da Multa do Empréstimo");
                this->admin->alterarValorMulta();
                apagarTerminal();
                break;
            case 13:
                escreveLog("Administrador escolheu a opcao: 13 - Realizar reopção de curso");
                escreveDevagar("Preencha os seguintes campos: ", 30);
                std::cout << std::endl;
                pausa(1);
                this->admin->mobilidadeAcademica(this->estudantes);
                escreveDevagar("Reopção realizada com sucesso!", 30);
                pausa(2);
                apagarTerminal();
                break;
            case 14:
                escreveLog("Administrador escolheu a opcao: 14 - Sair");
                escreveLog("Logout realizado");
                escreveDevagar("📤 Fazendo logout...\n", 50);
                pausa(2);
                apagarTerminal();
                return;
            default:
                throw std::invalid_argument("Digite um número válido!");
                break;
            }
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

void Sistema::menuEstudante()
{
    int opcao;
    while (1)
    {
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("🎓 BEM-VINDO AO PAINEL DO ESTUDANTE 🎓 \n", 50);
        escreveDevagar("============================================\n", 10);
        escreveDevagar("    Bem-vindo, " + this->estudante_logado->getNome() + "\n", 50);
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Consultar Saldo\n";
        std::cout << "2 - Recarregar Carteirinha\n";
        std::cout << "3 - Visualizar Carteirinha\n";
        std::cout << "4 - Ver Extrato Financeiro\n";
        std::cout << "5 - Buscar Livro no Acervo\n";
        std::cout << "6 - Ver Meus Empréstimos\n";
        std::cout << "7 - Pegar Livro Emprestado\n";
        std::cout << "8 - Devolver Livro\n";
        std::cout << "9 - Comer no RU\n";
        std::cout << "10 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            apagarTerminal();

            switch (opcao)
            {
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
                escreveLog("Estudante Escolheu a Opcao: 3 - Visualizar Carteirinha");
                this->estudante_logado->visualizarCarteirinha();
                pausa(2);
                apagarTerminal();
                break;
            case 4:
                escreveLog("Estudante Escolheu a Opcao: 4 - Ver Extrato Financeiro");
                this->estudante_logado->get_carteirinha()->exibir_extrato();
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.ignore();
                apagarTerminal();
                break;
            case 5:
                escreveLog("Estudante Escolheu a Opção: 5 - Buscar Livro no Acervo");
                this->biblioteca->listarLivros();
                apagarTerminal();
                break;
            case 6:
                escreveLog("Estudante Escolheu a Opcao: 6 - Ver Meus Empréstimos");
                this->estudante_logado->exibirEmprestimos();
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.ignore();
                apagarTerminal();
                break;
            case 7:
                escreveLog("Estudante Escolheu a Opcao: 7 - Pegar Livro Emprestado");
                this->estudante_logado->pegarLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 8:
                // TODO: salvar emprestimos em um arquivo, porque do jeito que está não tem como mostrar a logica de multas
                escreveLog("Estudante Escolheu a Opcao: 8 - Devolver Livro");
                this->estudante_logado->devolverLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 9:
                escreveLog("Estudante Escolheu a Opcao: 9 - ComerRU");
                this->estudante_logado->comerRU();
                apagarTerminal();
                break;
            case 10:
                escreveLog("Logout realizado");
                escreveDevagar("📤 Fazendo logout...\n", 50);
                pausa(2);
                apagarTerminal();
                this->estudante_logado = nullptr;
                return;
            default:
                throw std::invalid_argument("Digite um número válido!");
                break;
            }
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

void Sistema::iniciarSistema()
{
    escreveLog("Sistema aberto");

    int opcao;

    while (1)
    {
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("      📚 BEM-VINDO AO SISTEMA-UFV 📚\n", 50);
        escreveDevagar("--------------------------------------------\n", 10);
        std::cout << "1 - Fazer Login\n";
        std::cout << "2 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (opcao == 2)
            {
                std::cout << "\n👋 Obrigado por usar o SISTEMA-UFV. Até logo!\n";
                break;
            }
            else if (opcao == 1)
            {
                bool logado = false;

                while (!logado)
                {
                    std::string email, senha;

                    // ====== LEITURA DO EMAIL ======
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Email: ";
                    std::getline(std::cin, email);

                    // ====== LEITURA DA SENHA ======
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Senha: ";
                    std::getline(std::cin, senha);

                    std::cout << "--------------------------------------------\n";

                    limparString(senha);
                    limparString(email);

                    // Tenta Login como Administrador
                    if (this->admin->getSenha() == senha && this->admin->getEmail() == email)
                    {
                        escreveDevagar("✅ Bem-Vindo " + this->admin->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuAdministrador();
                        logado = true;
                        escreveLog("Administrador logou no sistema");
                        break;
                    }

                    // Tenta Login como Estudante
                    bool encontrado = false;
                    for (auto estudante : this->estudantes)
                    {
                        if (estudante->getEmail() == email && estudante->getSenha() == senha)
                        {
                            this->estudante_logado = estudante;
                            encontrado = true;
                            escreveLog("Estudante " + estudante->get_matricula() + " logou no sistema");
                            break;
                        }
                    }

                    if (encontrado && this->estudante_logado != nullptr)
                    {
                        escreveDevagar("✅ Bem-Vindo " + this->estudante_logado->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuEstudante();
                        logado = true;
                        break;
                    }
                    else
                    {
                        throw std::invalid_argument("\n❌ Credenciais inválidas. Tente novamente.\n");
                    }
                }
            }
            else
            {
                throw std::invalid_argument("❌ Opção inválida. Por favor, escolha 1 ou 2.");
            }
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << e.what() << std::endl;
        }
        catch (...)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro desconhecido!\n";
        }
    }

    escreveLog("Sistema Fechado");
}