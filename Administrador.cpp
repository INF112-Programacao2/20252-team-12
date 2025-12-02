// ========== INCLUDES ==========
#define cimg_display 0
#include "Administrador.hpp"
#include <fstream>           // Para operações com arquivos
#include <exception>         // Para tratamento de exceções
#include <iomanip>           // Para formatação de saída
#include <thread>            // Para threads (se necessário)
#include <chrono>            // Para operações com tempo
#include <ctime>             // Para data/hora
#include <cctype>            // Para funções de caracteres (toupper, tolower)
#include <sstream>           // Para stringstream (conversão string)
#include <limits>            // Para numeric_limits
#include <algorithm>         // Para algoritmos STL
#include "CImg.h"            // Biblioteca CImg (imagens)
#include "Estudante.hpp"
#include "Auxiliares.hpp"

using namespace cimg_library;

// ========== INICIALIZAÇÃO DE ATRIBUTO ESTÁTICO ==========
int Administrador::nextID = 1;

// ========== CONSTRUTOR ==========
// Inicializa um administrador com dados pessoais e incrementa o ID único
Administrador::Administrador(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha) 
    : Usuario(_nome, _cpf, _data_de_nascimento, _email, _senha), id(nextID++) {}

// ========== DESTRUTOR ==========
Administrador::~Administrador() {}

// ========== GETTER: Retorna o ID do administrador ==========
int Administrador::get_id() const {
    return this->id;
}

// ========== SETTER: Define o ID do administrador ==========
void Administrador::set_id(const int &_id) {
    this->id = _id;
}

// ========== CRIAR LIVRO ==========
// Função para criar e adicionar um novo livro à biblioteca
// Valida entrada do usuário (título, autor, tipo, quantidade de exemplares)
void Administrador::criarLivro(Biblioteca &biblioteca) {

    std::cout << "\n============================================\n";
    std::cout << "         📚 MENU DE CRIAÇÃO DE LIVRO 📚\n";
    std::cout << "============================================\n";

    std::string _titulo, _autor, _tipo;
    int _numExemplares;

    // --- VALIDAÇÃO DO TÍTULO ---
    while (1) {
        try {
            std::cout << "-> Titulo: ";
            std::getline(std::cin, _titulo);
            if (validarTITULO(_titulo)) {
                break;
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // --- VALIDAÇÃO DO AUTOR ---
    while (1) {
        try {
            std::cout << "-> Autor: ";
            std::getline(std::cin, _autor);
            if (validarNOME(_autor)) {
                break;
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // --- VALIDAÇÃO DO TIPO ---
    while (1) {
        try {
            std::cout << "-> Tipo: ";
            std::getline(std::cin, _tipo);
            if (validarNOME(_tipo)) {
                break;
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // --- VALIDAÇÃO DO NÚMERO DE EXEMPLARES ---
    while (1) {
        try {
            std::cout << "-> Numero de exemplares: ";
            if (!(std::cin >> _numExemplares)) {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida! Digite apenas números.");
            }

            if (_numExemplares <= 0) {
                throw std::invalid_argument("❌ O número deve ser maior que zero.");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;

        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    std::cout << "--------------------------------------------\n";

    // --- CRIAÇÃO E ADIÇÃO DO LIVRO ---
    Livro *novo_livro = new Livro(_titulo, _autor, _tipo, _numExemplares);
    biblioteca.criarLivro(*novo_livro);

    escreveDevagar(novo_livro->getTitulo() + " foi adicionado na Bibilioteca! ✅\n", 50);
}

// ========== CRIAR ESTUDANTE ==========
// Cria um novo estudante (Graduação ou Pós-Graduação) e o adiciona ao vetor de estudantes
void Administrador::criarEstudante(std::vector<Estudante *> &estudantes) {
    int opcao = 0;

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CADASTRAMENTO DE ESTUDANTE 📚\n";
    std::cout << "============================================\n";

    std::cout << "1 - Aluno Graduação" << std::endl;
    std::cout << "2 - Aluno Pós-Graduação" << std::endl;
    std::cout << "--------------------------------------------\n";

    // --- SELEÇÃO DO TIPO DE ESTUDANTE ---
    while (true) {
        std::cout << "Opção: ";
        try {
            if (!(std::cin >> opcao)) {
                std::cin.clear(); 
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida! Digite apenas números.");
            }
            if (opcao != 1 && opcao != 2) {
                throw std::invalid_argument("❌ O número deve ser 1 ou 2.");
            }
            break;
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;
    std::cout << "=== FAVOR PREENCHER AS INFORMACOES ABAIXO ===" << std::endl << std::endl;

    std::string _nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso;

    // --- COLETA DE DADOS COMUNS A TODOS OS ESTUDANTES ---

    // NOME
    while (1) {
        try {
            std::cout << "-> Nome: ";
            std::getline(std::cin, _nome);
            if (validarNOME(_nome)) {
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // CPF
    while (1) {
        try {
            std::cout << "-> CPF: ";
            std::getline(std::cin, _cpf);
            if (validarCPF(_cpf)) {
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // DATA DE NASCIMENTO
    while (1) {
        try {
            std::cout << "-> Data de Nascimento (DD/MM/YYYY): ";
            std::getline(std::cin, _data_de_nascimento);
            if (validarDATA(_data_de_nascimento)) {
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // EMAIL (com validação de duplicação)
    while (1) {
        try {
            std::cout << "-> Email: ";
            std::getline(std::cin, _email);
            if (validarEMAIL(_email)) {
                // Verifica se email já existe
                for (auto est : estudantes) {
                    if (_email == est->getEmail()) {
                        throw std::invalid_argument("❌  Erro: O estudante com esse e-mail já está cadastrado.");
                    }
                }
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // SENHA
    while (1) {
        try {
            std::cout << "-> Senha: ";
            std::getline(std::cin, _senha);
            if (validarSENHA(_senha)) {
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // MATRÍCULA (com validação de duplicação)
    while (1) {
        try {
            std::cout << "-> Matrícula: ";
            std::getline(std::cin, _matricula);
            if (validarMATRICULA(_matricula)) {
                for (auto estudante : estudantes) {
                    if (estudante->get_matricula() == _matricula) {
                        throw std::invalid_argument("❌ Já existe um estudante com essa matrícula");
                    }
                }
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // CURSO
    while (1) {
        try {
            std::cout << "-> Curso: ";
            std::getline(std::cin, _curso);
            if (validarCURSO(_curso)) {
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cout << "--------------------------------------------\n";

    // --- DADOS ESPECÍFICOS (GRADUAÇÃO OU PÓS-GRADUAÇÃO) ---

    if (opcao == 1) {
        // GRADUAÇÃO - solicita modalidade de ingresso
        std::string _modalidade;
        while (1) {
            try {
                std::cout << "-> Modalidade de Ingresso: ";
                std::getline(std::cin, _modalidade);
                if (validarMODALIDADE(_modalidade)) {
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }

        EstudanteGraduacao *_novoEstudante = new EstudanteGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _modalidade);
        std::cout << "--------------------------------------------\n";
        estudantes.push_back(_novoEstudante);
    }
    else if (opcao == 2) {
        // PÓS-GRADUAÇÃO - solicita tipo de pós-graduação e linha de pesquisa
        std::string _tipoPos, _linhaDePesquisa;

        while (1) {
            try {
                std::cout << "-> Tipo de Pos: ";
                std::getline(std::cin, _tipoPos);
                if (validarTIPOPOS(_tipoPos)) {
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }

        std::cout << "--------------------------------------------\n";

        while (1) {
            try {
                std::cout << "-> Linha de Pesquisa: ";
                std::getline(std::cin, _linhaDePesquisa);
                break;
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }

        std::cout << "--------------------------------------------\n";

        EstudantePosGraduacao *_novoEstudante = new EstudantePosGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _tipoPos, _linhaDePesquisa);
        estudantes.push_back(_novoEstudante);
    }

    escreveDevagar("Estudante de matrícula " + _matricula + " foi cadastrado com sucesso! ✅\n", 50);
}

// ========== LISTAR ESTUDANTES ==========
// Gera um arquivo com lista formatada de todos os estudantes cadastrados
void Administrador::listarEstudante(std::vector<Estudante *> &estudantes) {
    std::ofstream fout;
    fout.open("ListaEstudantes.txt");
    if (!fout.is_open()) {
        throw std::runtime_error("❌ Não foi possível abrir o arquvivo: Lista Estudantes ");
    }

    fout << "LISTA DE ESTUDANTES CADASTRADOS: " << std::endl << std::endl;

    // Cabeçalho com formatação
    fout << std::left
         << std::setw(40) << "NOME"
         << std::setw(25) << "DATA DE NASCIMENTO"
         << std::setw(15) << "MATRICULA"
         << std::setw(15) << "CURSO"
         << std::setw(15) << "NIVEL"
         << "\n----------------------------------------------------------------------------------------------------------------------------------\n";

    // Iteração sobre todos os estudantes
    for (auto estudante : estudantes) {
        std::string nivel = "Indefinido";

        // Dynamic cast para determinar se é Graduação ou Pós-Graduação
        if (dynamic_cast<EstudanteGraduacao *>(estudante)) {
            nivel = "Graduacao";
        }
        else if (dynamic_cast<EstudantePosGraduacao *>(estudante)) {
            nivel = "Pos-Graduacao";
        }

        // Escreve dados formatados no arquivo
        fout << std::setw(40) << estudante->getNome()
             << std::setw(25) << estudante->getDataDeNascimento()
             << std::setw(15) << estudante->get_matricula()
             << std::setw(15) << estudante->get_curso()
             << std::setw(15) << nivel
             << std::endl;
    }

    escreveDevagar("\nArquivo Lista Estudantes criado com sucesso ✅", 50);
    fout.close();

    if (fout.fail()) {
        throw std::runtime_error("❌ Não foi possível fechar o arquvivo: ListaEstudantes ");
    }
}

// ========== ALTERAR DADOS DO ESTUDANTE ==========
// Permite ao administrador alterar nome, email, curso ou senha de um estudante
int Administrador::alterarDadosEstudante(std::vector<Estudante *> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "   ✏️  MENU DE ALTERAÇÃO DE DADOS ✏️ \n";
    std::cout << "============================================\n";

    std::string matricula;
    Estudante *estudanteAlvo = nullptr;

    // --- BUSCAR ESTUDANTE POR MATRÍCULA ---
    while (1) {
        try {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula;

            if (matricula == "0") return 0;

            if (validarMATRICULA(matricula)) {
                estudanteAlvo = nullptr; 

                for (auto *est : estudantes) {
                    if (est->get_matricula() == matricula) {
                        estudanteAlvo = est;
                        break;
                    }
                }
                
                if (estudanteAlvo == nullptr) {
                    throw std::invalid_argument("❌ Estudante não encontrado com a matrícula informada.");
                }
                break;
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // --- MENU DE OPÇÕES DE ALTERAÇÃO ---
    int opcao = 0;
    std::cout << "\nEstudante encontrado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "1 - Alterar Nome\n";
    std::cout << "2 - Alterar Email\n";
    std::cout << "3 - Alterar Curso\n";
    std::cout << "4 - Alterar Senha\n";
    std::cout << "5 - Cancelar\n";
    std::cout << "--------------------------------------------\n";

    while (1) {
        try {
            std::cout << "Opção: ";
            
            if (!(std::cin >> opcao)) {
                throw std::invalid_argument("❌ Opção inválida (digite apenas números)");
            }
            
            if (opcao < 1 || opcao > 5) {
                throw std::invalid_argument("❌ Opção inválida (escolha entre 1 e 5)");
            }
            break;
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            std::cin.clear(); 
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string novoDado;

    // --- EXECUTAR ALTERAÇÃO SELECIONADA ---
    switch (opcao) {
    case 1: // ALTERAR NOME
        while (true) {
            try {
                std::cout << "-> Novo Nome: ";
                std::getline(std::cin, novoDado);
                if (validarNOME(novoDado)) {
                    if (novoDado == estudanteAlvo->getNome()) {
                        throw std::invalid_argument("❌  O novo nome não pode ser igual ao atual.");
                    }
                    estudanteAlvo->setNome(novoDado);
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 2: // ALTERAR EMAIL
        while (true) {
            try {
                std::cout << "-> Novo Email: ";
                std::getline(std::cin, novoDado);

                if (validarEMAIL(novoDado)) {
                    if (novoDado == estudanteAlvo->getEmail()) {
                        throw std::invalid_argument("❌  O novo email não pode ser igual ao atual.");
                    }
                    estudanteAlvo->setEmail(novoDado);
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 3: // ALTERAR CURSO
        while (true) {
            try {
                std::cout << "-> Novo Curso (Sigla ou Nome): ";
                std::getline(std::cin, novoDado);

                if (validarCURSO(novoDado)) {
                    if (novoDado == estudanteAlvo->get_curso()) {
                        throw std::invalid_argument("❌  O novo curso não pode ser igual ao atual.");
                    }
                    estudanteAlvo->set_curso(novoDado);
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 4: // ALTERAR SENHA
        while (true) {
            try {
                std::cout << "-> Nova Senha (min 6 caracteres, letras e números): ";
                std::getline(std::cin, novoDado);

                if (validarSENHA(novoDado)) {
                    if (novoDado == estudanteAlvo->getSenha()) {
                        throw std::invalid_argument("❌  A nova senha não pode ser igual à atual.");
                    }
                    estudanteAlvo->setSenha(novoDado);
                    break;
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 5:
        std::cout << "✅ Operação cancelada com sucesso!.\n";
        return 5;
    }

    escreveDevagar("✅ Dados atualizados com sucesso!\n", 50);
    return opcao;
}

// ========== ALTERAR SENHA DO ADMINISTRADOR ==========
// Permite que o administrador altere sua própria senha com confirmação
void Administrador::alterarSenhaAdministrador() {
    std::cout << "--------------------------------------------\n";
    std::string nova_senha;
    std::string confirmacao_senha;

    while (true) {
        try {
            std::cout << "-> Digite sua nova senha (min 6 caracteres): ";
            std::getline(std::cin, nova_senha);

            std::cout << "-> Confirme a nova senha: ";
            std::getline(std::cin, confirmacao_senha);

            // Verifica se as senhas coincidem
            if (nova_senha != confirmacao_senha) {
                throw std::invalid_argument("❌  Erro: As senhas não coincidem. Tente novamente.");
            }

            if (validarSENHA(nova_senha)) {
                this->setSenha(nova_senha);
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            std::cout << "--------------------------------------------\n";
        }
    }
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Senha alterada com sucesso!", 50);
}

// ========== CONSULTAR TRANSAÇÕES ==========
// Permite visualizar transações de todos ou um específico estudante
void Administrador::consultarTransacoes(std::vector<Estudante *> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CONSULTA DE TRANSAÇÕES 📚\n";
    std::cout << "============================================\n";

    char resposta;

    // --- ESCOLHER VISUALIZAÇÃO ---
    while (true) {
        std::cout << "# Deseja visualizar todas as transações? (S/N): ";
        std::cin >> resposta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        resposta = toupper(resposta);
        if (resposta == 'S' || resposta == 'N')
            break;

        std::cout << "❌  Opção inválida. Digite apenas S ou N.\n";
    }

    // --- VER TODAS AS TRANSAÇÕES ---
    if (resposta == 'S') {
        if (estudantes.empty()) {
            std::cout << "❌  Nenhum estudante cadastrado no sistema.\n";
            return;
        }

        for (auto estudante : estudantes) {
            std::cout << "--------------------------------------------\n";
            std::cout << "Aluno: " << estudante->getNome() << " - Matrícula: " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            if (estudante->get_carteirinha()) {
                estudante->get_carteirinha()->exibir_extrato();
            }
            else {
                std::cout << "   (Sem carteirinha ativa)\n";
            }
        }
    }
    // --- VER TRANSAÇÕES DE UM ESTUDANTE ESPECÍFICO ---
    else {
        std::string matricula;
        Estudante *estudanteEncontrado = nullptr;

        while (true) {
            try {
                std::cout << "-> Matrícula do aluno (ou '0' para voltar): ";
                std::cin >> matricula;

                if (matricula == "0")
                    return;

                for (auto estudante : estudantes) {
                    if (estudante->get_matricula() == matricula) {
                        estudanteEncontrado = estudante;
                        break;
                    }
                }

                if (estudanteEncontrado == nullptr) {
                    throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com a matrícula " + matricula);
                }

                break;
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        apagarTerminal();

        std::cout << "--------------------------------------------\n";
        std::cout << "Aluno: " << estudanteEncontrado->getNome() << " - Matrícula: " << estudanteEncontrado->get_matricula() << std::endl;
        std::cout << "--------------------------------------------\n";

        if (estudanteEncontrado->get_carteirinha()) {
            estudanteEncontrado->get_carteirinha()->exibir_extrato();
        }
        else {
            std::cout << "❌  Este aluno não possui carteirinha ativa.\n";
        }
    }
}

// ========== CONSULTAR EMPRÉSTIMOS ==========
// Permite visualizar empréstimos de livros de todos ou um específico estudante
void Administrador::consultarEmprestimos(std::vector<Estudante *> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CONSULTA DE EMPRÉSTIMOS 📚\n";
    std::cout << "============================================\n";

    char resposta;

    // --- ESCOLHER VISUALIZAÇÃO ---
    while (true) {
        std::cout << "# Deseja visualizar todas os empréstimos? (S/N): ";
        std::cin >> resposta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        resposta = toupper(resposta);
        if (resposta == 'S' || resposta == 'N')
            break;

        std::cerr << "❌  Opção inválida. Digite apenas S ou N.\n";
    }

    apagarTerminal();

    // --- VER TODOS OS EMPRÉSTIMOS ---
    if (resposta == 'S') {
        if (estudantes.empty()) {
            std::cerr << "❌  Nenhum estudante cadastrado.\n";
            return;
        }

        for (auto estudante : estudantes) {
            std::cout << "--------------------------------------------\n";
            std::cout << "Aluno: " << estudante->getNome() << " - Matrícula: " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            estudante->exibirEmprestimos();
        }
    }
    // --- VER EMPRÉSTIMOS DE UM ESTUDANTE ESPECÍFICO ---
    else {
        std::string matricula;
        Estudante *estudanteEncontrado = nullptr;

        while (true) {
            try {
                std::cout << "-> Matrícula do aluno (ou '0' para voltar): ";
                std::cin >> matricula;

                if (matricula == "0")
                    return;

                if (!validarMATRICULA(matricula)) {
                    break;
                }

                for (auto estudante : estudantes) {
                    if (estudante->get_matricula() == matricula) {
                        estudanteEncontrado = estudante;
                        break;
                    }
                }

                if (estudanteEncontrado == nullptr) {
                    throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com matrícula " + matricula);
                }

                break;
            }
            catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cout << "--------------------------------------------\n";
        std::cout << estudanteEncontrado->getNome() << " - " << estudanteEncontrado->get_matricula() << std::endl;
        std::cout << "--------------------------------------------\n";
        estudanteEncontrado->exibirEmprestimos();
    }
}

// ========== RECARREGAR CARTEIRINHA ==========
// Permite recarregar o saldo da carteirinha de um estudante
void Administrador::recarregarCarteirinha(std::vector<Estudante *> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CARREGAMENTO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    std::string matricula;
    Estudante *estudanteAlvo = nullptr;

    // --- BUSCAR ESTUDANTE ---
    while (true) {
        try {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula;

            if (matricula == "0")
                return;

            if (!validarMATRICULA(matricula)) {
                break;
            }

            for (auto *est : estudantes) {
                if (est->get_matricula() == matricula) {
                    estudanteAlvo = est;
                    break;
                }
            }

            if (estudanteAlvo == nullptr) {
                throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com matrícula " + matricula);
            }

            break;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    // Chama método do estudante para recarregar carteirinha
    estudanteAlvo->recarregarCarteirinha();
}

// ========== VISUALIZAR CARTEIRINHAS ==========
// Exibe informações de carteirinhas de um ou todos os estudantes
void Administrador::visualizarCarteirinhas(std::vector<Estudante *> &estudantes) {
    
    std::cout << "\n============================================\n";
    std::cout << "  📚 VISUALIZAÇÃO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";
    
    int opcao;
    escreveDevagar("Por favor, escolha a opção de visualização desejada: ", 30);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("1 - Visualizar de um estudante", 20);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("2 - Visualizar de todos os estudantes", 20);
    std::cout << std::endl;

    // --- ESCOLHER OPÇÃO ---
    while (true) {
        try {
            std::cout << "-> Opção: ";
            if (!(std::cin >> opcao)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida! Digite um número.");
            }

            if (opcao < 1 || opcao > 2) {
                throw std::invalid_argument("❌ Opção inválida! Digite 1 ou 2.");
            }

            break;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // --- OPÇÃO 1: VISUALIZAR DE UM ESTUDANTE ---
    if (opcao == 1) {
        std::cout << "--------------------------------------------\n";
        escreveDevagar("Por favor, preencha o campo abaixo: \n", 20);
        std::cout << "--------------------------------------------\n";

        std::string matricula_aluno;
        Estudante *aluno_carteirinha = nullptr;

        while (true)
        {
            try
            {
                escreveDevagar("Matricula do aluno (ou '0' para voltar): ", 20);
                std::cin >> matricula_aluno;
                std::cout << "--------------------------------------------\n";

                if (matricula_aluno == "0")
                    return;

                if (validarMATRICULA(matricula_aluno))
                {
                    break;
                }

                for (auto estudante : estudantes)
                {
                    if (estudante->get_matricula() == matricula_aluno)
                    {
                        aluno_carteirinha = estudante;
                        break;
                    }
                }

                if (aluno_carteirinha == nullptr)
                {
                    throw std::invalid_argument("❌ Credenciais inválidas! Não foi possível localizar o aluno com essa matrícula.");
                }

                break;
            }
            catch (const std::exception &e)
            {
                std::cout << "\n" << e.what() << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer em caso de erro
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (aluno_carteirinha) {
                 aluno_carteirinha->visualizarCarteirinha();
             } 
            else {
               std::cout << "❌ Aluno não encontrado!\n";
            }

    }
    // --- OPÇÃO 2: VISUALIZAR DE TODOS OS ESTUDANTES ---
    else if (opcao == 2) {
        try {
            std::cout << std::endl;
            std::cout << "======================================================\n";
            escreveDevagar("Exibindo informações...", 30);
            std::cout << "\n\n";

            if (estudantes.empty()) {
                std::cout << "❌  Nenhum estudante cadastrado.\n";
            }

            for (auto estudante : estudantes) {
                if(!estudante){
                    std::cout<<"❌ Ponteiro inválido no vetor de estudantes, pulando...\n";
                    continue;
                }
                escreveDevagar("Exibindo carteirinha de: ", 20);
                std::cout << estudante->getNome() << " (" << estudante->get_matricula() << ")\n\n";
                estudante->visualizarCarteirinha();
                std::cout << "======================================================\n";
            }
        }
        catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    escreveDevagar("Voltando ao painel de administrador...", 40);
}

// ========== ALTERAR VALOR DO RU ==========
// Permite alterar o valor da refeição (RU) para Graduação ou Pós-Graduação
std::string Administrador::alterarValorRU() {
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR RU 📚\n";
    std::cout << "============================================\n";

    std::cout << "Escolha o nível do estudante:\n";
    std::cout << "1 - Graduação\n2 - Pós-Graduação\n";

    char resposta;
    double novo_valor;

    // --- ESCOLHER NÍVEL ---
    while (1) {
        try {
            std::cout << "-> Opção: ";
            std::cin >> resposta;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (resposta != '1' && resposta != '2') {
                throw std::invalid_argument("❌ Opção inválida. Digite 1 ou 2.");
            }

            break;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    // --- DIGITAR NOVO VALOR ---
    while (1) {
        try {
            std::cout << "-> Novo valor (R$): ";

            if (!(std::cin >> novo_valor)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida. Digite um número real (ex: 5.90).");
            }

            if (novo_valor < 0) {
                throw std::invalid_argument("❌ O valor não pode ser negativo.");
            }

            break;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string gradOuPos;

    // --- ATUALIZAR VALOR NA CLASSE APROPRIADA ---
    if (resposta == '1') {
        EstudanteGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Graduação";
    }
    else {
        EstudantePosGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Pós-Graduação";
    }

    escreveDevagar("✅ Valor alterado com sucesso para " + gradOuPos + "! \n", 50);
    return gradOuPos;
}

// ========== ALTERAR VALOR DA MULTA ==========
// Permite alterar o valor da multa diária por atraso de empréstimo
void Administrador::alterarValorMulta() {
    std::cout << "\n=================================================================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR NA MULTA DE EMPRÉSTIMO 📚\n";
    std::cout << "=================================================================================\n";

    double novo_valor;

    while (true) {
        try {
            std::cout << "-> Valor da nova multa: ";

            if (!(std::cin >> novo_valor)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida. Digite um número positivo.");
            }

            if (novo_valor < 0) {
                throw std::invalid_argument("❌ O valor da multa não pode ser negativo.");
            }

            break;
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Atualiza valor da multa na classe Emprestimo (atributo estático)
    Emprestimo::setMulta(novo_valor);
    escreveDevagar("✅ Valor da multa alterado com sucesso!", 50);
}

// ========== MOBILIDADE ACADÊMICA ==========
// Permite transferir um estudante para outro curso
void Administrador::mobilidadeAcademica(std::vector<Estudante *> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE MOBILIDADE ACADÊMICA 📚\n";
    std::cout << "============================================\n";

    std::string matricula_aluno;
    Estudante *estudanteAlvo = nullptr;

    // --- BUSCAR ESTUDANTE ---
    while (true) {
        try {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula_aluno;

            if (matricula_aluno == "0")
                return;

            if (validarMATRICULA(matricula_aluno)) { 
                for (auto *est : estudantes) {
                    if (est->get_matricula() == matricula_aluno) {
                        estudanteAlvo = est;
                        break;
                    }
                }

                if (estudanteAlvo == nullptr) {
                    throw std::invalid_argument("❌ Erro: Aluno não encontrado com a matrícula " + matricula_aluno);
                }

                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nAluno selecionado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "Curso atual: " << estudanteAlvo->get_curso() << "\n";
    std::cout << "--------------------------------------------\n";

    std::string novoCurso;

    // --- DIGITAR NOVO CURSO ---
    while (true) {
        try {
            std::cout << "-> Novo curso (Código ou Nome): ";
            std::getline(std::cin, novoCurso);

            if (validarCURSO(novoCurso)) {
                if (novoCurso == estudanteAlvo->get_curso()) {
                    throw std::invalid_argument("❌  O aluno já está matriculado neste curso.");
                }
                estudanteAlvo->set_curso(novoCurso);
                break;
            }
        }
        catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    }
    escreveDevagar("✅ Mobilidade realizada com sucesso! O aluno agora pertence ao curso: " + novoCurso + "\n", 50);
}

// ========== PROCURAR CURSO POR CÓDIGO ==========
// Busca o nome completo de um curso a partir de seu código
std::string Administrador::procurar_curso_por_codigo(std::string codigo) {
    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");

    if (!leitura.is_open())
        throw std::runtime_error("Nao foi possivel abrir o arquivo de leitura");

    std::string linha;
    bool aux = true, encontrou = false;
    std::string nome_curso;

    while (std::getline(leitura, linha)) {
        if (aux) {
            aux = false;  // Pula primeira linha (cabeçalho)
            continue;
        }
        auto pos = linha.find(' ');
        std::string codigo_linha = linha.substr(0, pos);
        if (codigo == codigo_linha) {
            encontrou = true;
            nome_curso = linha.substr(pos + 1);
            break;
        }
    }
    if (!encontrou)
        throw std::invalid_argument("Parametro invalido");

    leitura.close();
    if (leitura.fail())
        throw std::runtime_error("Nao foi possivel fechar o arquivo de leitura");

    return nome_curso;
}
