#define cimg_display 0
#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cctype>
#include <sstream>
#include <limits>
#include <algorithm>
#include <limits>
#include "CImg.h"
#include "Estudante.hpp"
#include "Validar.hpp"

using namespace cimg_library;

int Administrador::nextID = 1;
Administrador::Administrador(const std::string &_nome, const std::string &_cpf, const std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha) : Usuario(_nome, _cpf, _data_de_nascimento, _email, _senha), id(nextID++) {}

Administrador::~Administrador() {}

int Administrador::get_id() const
{
    return this->id;
}

void Administrador::set_id(const int &_id)
{
    this->id = _id;
}

//TODO: Autor e tipo não podem conter numeros
void Administrador::criarLivro(Biblioteca &biblioteca)
{

    std::cout << "\n============================================\n";
    std::cout << "         📚 MENU DE CRIAÇÃO DE LIVRO 📚\n";
    std::cout << "============================================\n";

    std::string _titulo, _autor, _tipo;
    int _numExemplares;

    do
    {
        std::cout << "-> Titulo: ";
        std::getline(std::cin, _titulo);

        if (_titulo.empty())
        {
            std::cout << "⚠️  Erro: O titulo nao pode estar vazio. Tente novamente.\n";
        }
    } while (_titulo.empty());

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> Autor: ";
        std::getline(std::cin, _autor);

        if (_autor.empty())
        {
            std::cout << "⚠️  Erro: O nome do autor e obrigatorio.\n";
        }
    } while (_autor.empty());

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> Tipo: ";
        std::getline(std::cin, _tipo);

        if (_tipo.empty())
        {
            std::cout << "⚠️  Erro: O tipo do livro e obrigatorio.\n";
        }
    } while (_tipo.empty());

    std::cout << "--------------------------------------------\n";

    while (true)
    {
        std::cout << "-> Numero de exemplares: ";
        std::cin >> _numExemplares;

        if (std::cin.fail() || _numExemplares <= 0)
        {
            std::cout << "⚠️  Erro: Digite um numero inteiro valido e maior que zero.\n";

            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    std::cout << "--------------------------------------------\n";

    Livro *novo_livro = new Livro(_titulo, _autor, _tipo, _numExemplares);
    biblioteca.criarLivro(*novo_livro);

    escreveDevagar(novo_livro->getTitulo() + " foi adicionado na Bibilioteca! ✅\n", 50);
}

//TODO: checar se já tem um estudante com esse email
void Administrador::criarEstudante(std::vector<Estudante *> &estudantes)
{

    int opcao = 0;

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CADASTRAMENTO DE ESTUDANTE 📚\n";
    std::cout << "============================================\n";

    std::cout << "1 - Aluno Graduação" << std::endl;
    std::cout << "2 - Aluno Pós-Graduação" << std::endl;
    std::cout << "--------------------------------------------\n";

    while (true)
    {
        std::cout << "Opção: ";
        if (std::cin >> opcao && (opcao == 1 || opcao == 2))
        {
            break;
        }
        else
        {
            std::cout << "❌ Opção inválida! Digite 1 ou 2.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << std::endl;
    std::cout << "=== FAVOR PREENCHER AS INFORMACOES ABAIXO ===" << std::endl
              << std::endl;

    std::string _nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso;

    // --- NOME ---
    do {
        try {
            std::cout << "-> Nome: ";
            std::getline(std::cin, _nome);
            validarNOME(_nome);
            break;
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } while (true);

    std::cout << "--------------------------------------------\n";

    // --- CPF ---
    do {
        try {
            std::cout << "-> CPF (apenas números): ";
            std::getline(std::cin, _cpf);
            validarCPF(_cpf);

            for (auto est : estudantes) {
                if (_cpf == est->getCpf()) {
                     throw std::invalid_argument("⚠️  Erro: O estudante com esse CPF já está cadastrado.");
                }
            }
            break;
        } catch (const std::exception &e) {
             std::cout << e.what() << std::endl;
             _cpf = "";
        }
    } while (true);

    std::cout << "--------------------------------------------\n";

    // --- DATA NASCIMENTO ---
    do {
        try {
            std::cout << "-> Data de nascimento (DD/MM/AAAA): ";
            std::getline(std::cin, _data_de_nascimento);
            validarDATA(_data_de_nascimento);
            
            std::time_t t = std::time(nullptr);
            std::tm* now = std::localtime(&t);
            int anoAtual = now->tm_year + 1900;
            int anoNasc = std::stoi(_data_de_nascimento.substr(_data_de_nascimento.size()-4));
            
            if (anoNasc > anoAtual - 17) {
                throw std::invalid_argument("⚠️  Erro: O estudante deve ter no mínimo 17 anos.");
            }
            break;
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } while (true);

    std::cout << "--------------------------------------------\n";

    // --- EMAIL ---
    do {
        try {
            std::cout << "-> Email: ";
            std::cin >> _email;
            validarEMAIL(_email);

            if (_email.substr(_email.size()-7) != "@ufv.br" || _email.size() <= 7) {
                throw std::invalid_argument("⚠️  Email inválido (Utilize somente email institucional '@ufv.br').");
            }
            break;
        } catch (const std::exception &e) {
             std::cout << e.what() << std::endl;
        }
    } while (true);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    // --- SENHA ---
    do {
        try {
            std::cout << "-> Senha: ";
            std::cin >> _senha;
            validarSENHA(_senha);
            break;
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } while (true);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    // --- MATRÍCULA ---
    do {
        try {
            std::cout << "-> Matrícula do aluno: ";
            std::cin >> _matricula;
            // TODO validarMATRICULA(_matricula);

            for (auto est : estudantes) {
                if (_matricula == est->get_matricula()) {
                    throw std::invalid_argument("⚠️  Erro: Já existe um estudante com essa matrícula.");
                }
            }
            break;
        } catch (const std::exception &e) {
             std::cout << e.what() << std::endl;
             std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 
        }
    } while (true);
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    do
    {
        try {
            std::cout << "-> Curso (Nome ou Código): ";
            std::getline(std::cin, _curso);
            
            // TODO validarCURSO(_curso); 
            
            std::cout << "   (Curso selecionado: " << _curso << ")\n"; 
            break;
        } catch (const std::exception &e) {
            std::cout << e.what() << std::endl;
        }
    } while (true);

    std::cout << "--------------------------------------------\n";

    if (opcao == 1)
    {
        std::string _modalidade;
        do
        {
            std::cout << "-> Modalidade de Graduação: ";
            std::getline(std::cin, _modalidade);
            if (_modalidade.empty())
                std::cout << "⚠️  Modalidade obrigatória.\n";
        } while (_modalidade.empty());

        EstudanteGraduacao *_novoEstudante = new EstudanteGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _modalidade);
        std::cout << "--------------------------------------------\n";
        estudantes.push_back(_novoEstudante);
    }
    else if (opcao == 2)
    {
        std::string _tipoPos, _linhaDePesquisa;

        do
        {
            std::cout << "-> Tipo Pós-Graduação (ex: Mestrado, Doutorado): ";
            std::getline(std::cin, _tipoPos);
            if (_tipoPos.empty())
                std::cout << "⚠️  Tipo obrigatório.\n";
        } while (_tipoPos.empty());

        std::cout << "--------------------------------------------\n";

        do
        {
            std::cout << "-> Linha de Pesquisa: ";
            std::getline(std::cin, _linhaDePesquisa);
            if (_linhaDePesquisa.empty())
                std::cout << "⚠️  Linha de pesquisa obrigatória.\n";
        } while (_linhaDePesquisa.empty());

        std::cout << "--------------------------------------------\n";

        EstudantePosGraduacao *_novoEstudante = new EstudantePosGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _tipoPos, _linhaDePesquisa);
        estudantes.push_back(_novoEstudante);
    }

    escreveDevagar("Estudante de matrícula " + _matricula + " foi cadastrado com sucesso! ✅\n", 50);
}

//TODO: campo de grad/posgrad
void Administrador::listarEstudante(std::vector<Estudante *> &estudantes)
{

    std::ofstream fout;
    fout.open("ListaEstudantes.txt");
    if (!fout.is_open())
    {
        throw std::runtime_error("❌ Não foi possível abrir o arquvivo: Lista Estudantes ");
    }

    fout << "LISTA DE ESTUDANTES CADASTRADOS: " << std::endl
         << std::endl;
    fout << std::left
         << std::setw(40) << "NOME"
         << std::setw(25) << "DATA DE NASCIMENTO"
         << std::setw(12) << "MATRICULA"
         << std::setw(12) << "CURSO"
         << "\n----------------------------------------------------------------------------------------------------------------\n";
    for (auto estudante : estudantes)
    {
        fout << std::setw(40) << estudante->getNome() << std::setw(25) << estudante->getDataDeNascimento() << std::setw(12) << estudante->get_matricula() << std::setw(12) << estudante->get_curso() << std::endl;
    }
    escreveDevagar("\nArquivo Lista Estudantes criado com sucesso ✅", 50);
    fout.close();
    if (fout.fail())
    {
        throw std::runtime_error("❌ Não foi possível fechar o arquvivo: ListaEstudantes ");
    }
}

//TODO: caso em que o usuario insere o mesmo valor que já havia antes (processamento do arquivo de dados todo novamente desnecessario,
// então é melhor avisar que não pode ser igual e sair/tentar denovo)
int Administrador::alterarDadosEstudante(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "   ✏️  MENU DE ALTERAÇÃO DE DADOS ✏️ \n";
    std::cout << "============================================\n";

    std::string matricula;
    Estudante *estudanteAlvo = nullptr;

    while (true)
    {
        try 
        {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula;

            if (matricula == "0") return 0;

            // TODO: validarMATRICULA(matricula);

            bool encontrado = false;
            for (auto *est : estudantes)
            {
                if (est->get_matricula() == matricula)
                {
                    estudanteAlvo = est;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado) {
                throw std::invalid_argument("❌ Estudante não encontrado com a matrícula informada.");
            }

            break; 
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer após cin >>

    int opcao = 0;
    std::cout << "\nEstudante encontrado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "1 - Alterar Nome\n";
    std::cout << "2 - Alterar Email\n";
    std::cout << "3 - Alterar Curso\n";
    std::cout << "4 - Alterar Senha\n";
    std::cout << "5 - Cancelar\n";
    std::cout << "--------------------------------------------\n";

    while (true)
    {
        std::cout << "Opção: ";
        if (std::cin >> opcao)
        {
            if (opcao >= 1 && opcao <= 5) break;
            std::cout << "❌ Opção inválida. Escolha entre 1 e 5.\n";
        }
        else
        {
            std::cout << "❌ Digite apenas números.\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string novoDado;

    switch (opcao)
    {
    case 1: // ====== ALTERAR NOME ======
        while (true) {
            try {
                std::cout << "-> Novo Nome: ";
                std::getline(std::cin, novoDado);
                validarNOME(novoDado);
                
                estudanteAlvo->setNome(novoDado);
                break;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 2: // ====== ALTERAR EMAIL ======
        while (true) {
            try {
                std::cout << "-> Novo Email: ";
                std::getline(std::cin, novoDado);
                validarEMAIL(novoDado);

                if (novoDado.substr(novoDado.size()-7) != "@ufv.br" || novoDado.size() <= 7) {
                     throw std::invalid_argument("⚠️  Email inválido (Utilize somente email institucional '@ufv.br').");
                }

                estudanteAlvo->setEmail(novoDado);
                break;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 3: // ====== ALTERAR CURSO ======
        while (true) {
            try {
                std::cout << "-> Novo Curso (Sigla ou Nome): ";
                std::getline(std::cin, novoDado);
                
                // TODO validarCURSO(novoDado);
                
                estudanteAlvo->set_curso(novoDado);
                break;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 4: // ====== ALTERAR SENHA ======
        while (true) {
            try {
                std::cout << "-> Nova Senha (min 6 caracteres, letras e números): ";
                std::getline(std::cin, novoDado);
                validarSENHA(novoDado);
                
                estudanteAlvo->setSenha(novoDado);
                break;
            } catch (const std::exception &e) {
                std::cout << e.what() << std::endl;
            }
        }
        break;

    case 5:
        std::cout << "Operação cancelada.\n";
        return 5;
    }

    escreveDevagar("\n✅ Dados atualizados com sucesso!\n", 50);
    return opcao;
}

void Administrador::alterarSenhaAdministrador()
{
    std::cout << "--------------------------------------------\n";
    std::string nova_senha;
    std::string confirmacao_senha;

    while (true)
    {
        try 
        {
            std::cout << "-> Digite sua nova senha (min 6 caracteres): ";
            std::getline(std::cin, nova_senha);

            validarSENHA(nova_senha); 

            std::cout << "-> Confirme a nova senha: ";
            std::getline(std::cin, confirmacao_senha);

            if (nova_senha != confirmacao_senha)
            {
                throw std::invalid_argument("⚠️  Erro: As senhas não coincidem. Tente novamente.");
            }

            break; 
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cout << "--------------------------------------------\n";
        }
    }

    this->setSenha(nova_senha);
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Senha alterada com sucesso!", 50);
}

// TODO: Tirar enter duplo para sair da funcao
void Administrador::consultarTransacoes(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CONSULTA DE TRANSAÇÕES 📚\n";
    std::cout << "============================================\n";

    char resposta;

    while (true)
    {
        std::cout << "# Deseja visualizar todas as transações? (S/N): ";
        std::cin >> resposta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        resposta = toupper(resposta);
        if (resposta == 'S' || resposta == 'N') break;

        std::cout << "⚠️  Opção inválida. Digite apenas S ou N.\n";
    }

    if (resposta == 'S')
    {
        if (estudantes.empty())
        {
            std::cout << "⚠️  Nenhum estudante cadastrado no sistema.\n";
            return;
        }

        for (auto estudante : estudantes)
        {
            std::cout << "--------------------------------------------\n";
            std::cout << "Aluno: " << estudante->getNome() << " - Matrícula: " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            if (estudante->get_carteirinha())
            {
                estudante->get_carteirinha()->exibir_extrato();
            }
            else
            {
                std::cout << "   (Sem carteirinha ativa)\n";
            }
        }
    }
    else
    {
        std::string matricula;
        Estudante* estudanteEncontrado = nullptr;

        while (true)
        {
            try 
            {
                std::cout << "-> Matrícula do aluno (ou '0' para voltar): ";
                std::cin >> matricula;

                if (matricula == "0") return;

                //TODO: validarMATRICULA(matricula);

                bool achou = false;
                for (auto estudante : estudantes)
                {
                    if (estudante->get_matricula() == matricula)
                    {
                        estudanteEncontrado = estudante;
                        achou = true;
                        break;
                    }
                }

                if (!achou) {
                    throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com a matrícula " + matricula);
                }

                break;
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        apagarTerminal();

        std::cout << "--------------------------------------------\n";
        std::cout << "Aluno: " << estudanteEncontrado->getNome() << " - Matrícula: " << estudanteEncontrado->get_matricula() << std::endl;
        std::cout << "--------------------------------------------\n";

        if (estudanteEncontrado->get_carteirinha())
        {
            estudanteEncontrado->get_carteirinha()->exibir_extrato();
        }
        else
        {
            std::cout << "⚠️  Este aluno não possui carteirinha ativa.\n";
        }
    }
}

// TODO: Tirar enter duplo para sair da funcao
void Administrador::consultarEmprestimos(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CONSULTA DE EMPRÉSTIMOS 📚\n";
    std::cout << "============================================\n";

    char resposta;

    while (true)
    {
        std::cout << "# Deseja visualizar todas os empréstimos? (S/N): ";
        std::cin >> resposta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        resposta = toupper(resposta);
        if (resposta == 'S' || resposta == 'N') break;

        std::cout << "⚠️  Opção inválida. Digite apenas S ou N.\n";
    }

    apagarTerminal();

    if (resposta == 'S')
    {
        if (estudantes.empty())
        {
            std::cout << "⚠️  Nenhum estudante cadastrado.\n";
            return;
        }

        for (auto estudante : estudantes)
        {
            std::cout << "--------------------------------------------\n";
            std::cout << "Aluno: " << estudante->getNome() << " - Matrícula: " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            estudante->exibirEmprestimos();
        }
    }
    else
    {
        std::string matricula;
        Estudante* estudanteEncontrado = nullptr;

        while (true)
        {
            try
            {
                std::cout << "-> Matrícula do aluno (ou '0' para voltar): ";
                std::cin >> matricula;

                if (matricula == "0") return;

                //TODO: validarMATRICULA(matricula);

                bool achou = false;
                for (auto estudante : estudantes)
                {
                    if (estudante->get_matricula() == matricula)
                    {
                        estudanteEncontrado = estudante;
                        achou = true;
                        break;
                    }
                }

                if (!achou) {
                    throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com matrícula " + matricula);
                }

                break;
            }
            catch (const std::exception &e)
            {
                std::cout << e.what() << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        std::cout << "--------------------------------------------\n";
        std::cout << estudanteEncontrado->getNome() << " - " << estudanteEncontrado->get_matricula() << std::endl;
        std::cout << "--------------------------------------------\n";
        estudanteEncontrado->exibirEmprestimos();
    }
}

void Administrador::recarregarCarteirinha(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CARREGAMENTO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    std::string matricula;
    Estudante *estudanteAlvo = nullptr;

    while (true)
    {
        try
        {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula;

            if (matricula == "0") return;

            //TODO: validarMATRICULA(matricula);

            bool encontrado = false;
            for (auto *est : estudantes)
            {
                if (est->get_matricula() == matricula)
                {
                    estudanteAlvo = est;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado)
            {
                throw std::invalid_argument("❌ Erro: Não foi possível localizar o Estudante com matrícula " + matricula);
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (estudanteAlvo != nullptr)
    {
        try {
            estudanteAlvo->recarregarCarteirinha();
        } catch (const std::exception &e) {
            std::cout << "❌ Falha na recarga: " << e.what() << std::endl;
        }
    }
}

// TODO: (Thales) Implementar a função de gerar carteirinha
// Perguntar se deseja ver de todos ou de algum, perguntar matricula dele
// Pede para colocar a foto na pasta images com algum formato
// Verificar se existe e trata os erros, exibir erro se algum estudante não colocou a foto na pasta, mas gerar do resto se conseguiu ter
// Gera a carteirinha e coloca em uma pasta separada. Ex: 'Carteirinhas'
// Tenta seguir o padrão das funções quando for pedir algum dado escrito e no UI


void Administrador::visualizarCarteirinhas(std::vector<Estudante *> &estudantes)
{
    int opcao;
    escreveDevagar("Por favor, escolha a opção de visualização desejada: ", 30);
    std::cout << std::endl;
    escreveDevagar("1 - Visualizar de um estudante", 20);
    std::cout << std::endl;
    escreveDevagar("2 - Visualizar de todos os estudantes", 20);
    std::cout << std::endl;

    while (true)
    {
        try
        {
            std::cout << "Opção: ";
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida! Digite um número.");
            }

            if (opcao < 1 || opcao > 2)
            {
                throw std::invalid_argument("❌ Opção inválida! Digite 1 ou 2.");
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl << std::endl;
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (opcao == 1)
    { 
        escreveDevagar("Por favor, preencha o campo abaixo: \n", 20);
        
        std::string matricula_aluno;
        Estudante *aluno_carteirinha = nullptr;

        while (true) 
        {
            try 
            {
                escreveDevagar("Matricula do aluno (ou '0' para voltar): ", 20);
                std::cin >> matricula_aluno;

                if (matricula_aluno == "0") return;

                //TODO: validarMATRICULA(matricula_aluno);

                bool encontrado = false;
                for (auto estudante : estudantes)
                {
                    if (estudante->get_matricula() == matricula_aluno)
                    {
                        aluno_carteirinha = estudante;
                        encontrado = true;
                        break;
                    }
                }

                if (!encontrado)
                {
                    throw std::invalid_argument("❌ Credenciais inválidas! Não foi possível localizar o aluno com essa matrícula.");
                }

                break;
            }
            catch (const std::exception &e)
            {
                std::cout << "\n" << e.what() << std::endl << std::endl;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa buffer em caso de erro
            }
        }
        
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); 

        if (aluno_carteirinha != nullptr) 
        {
            try {
                aluno_carteirinha->visualizarCarteirinha();
            } catch (const std::exception &e) {
                std::cout << "❌ Erro ao visualizar carteirinha: " << e.what() << std::endl;
            }
        }
    }
    else if (opcao == 2)
    { 
        std::cout << std::endl;
        escreveDevagar("Exibindo informações...", 30);
        std::cout << "\n\n";
        
        if (estudantes.empty()) {
            std::cout << "⚠️  Nenhum estudante cadastrado.\n";
        }

        for (auto estudante : estudantes)
        {
            escreveDevagar("Exibindo carteirinha de: ", 20);
            std::cout << estudante->getNome() << " (" << estudante->get_matricula() << ")\n\n";
            try
            {
                estudante->visualizarCarteirinha();
            }
            catch (const std::exception &e)
            {
                std::cerr << "❌ Não foi possível gerar a carteirinha de " << estudante->getNome() << ": " << e.what() << std::endl << std::endl;
            }
            std::cout << "======================================================\n";
        }
    }

    escreveDevagar("Voltando ao painel de administrador...", 40);
}

//TODO: pode ser 0?
std::string Administrador::alterarValorRU()
{
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR RU 📚\n";
    std::cout << "============================================\n";

    std::cout << "Escolha o nível do estudante:\n";
    std::cout << "1 - Graduação\n2 - Pós-Graduação\n";

    char resposta;
    double novo_valor;

    while (true)
    {
        try
        {
            std::cout << "Opção: ";
            std::cin >> resposta;
            
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            if (resposta != '1' && resposta != '2')
            {
                throw std::invalid_argument("❌ Opção inválida. Digite 1 ou 2.");
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }
    while (true)
    {
        try
        {
            std::cout << "-> Novo valor (R$): ";
            
            if (!(std::cin >> novo_valor))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida. Digite um número real (ex: 5.90).");
            }

            if (novo_valor < 0)
            {
                throw std::invalid_argument("❌ O valor não pode ser negativo.");
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string gradOuPos;

    if (resposta == '1')
    {
        EstudanteGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Graduação";
    }
    else
    {
        EstudantePosGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Pós-Graduação";
    }

    escreveDevagar("\n✅ Valor alterado com sucesso para " + gradOuPos + "! \n", 50);
    return gradOuPos;
}

void Administrador::alterarValorMulta()
{
    std::cout << "\n=================================================================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR NA MULTA DE EMPRÉSTIMO 📚\n";
    std::cout << "=================================================================================\n";

    double novo_valor;

    while (true)
    {
        try
        {
            std::cout << "-> Valor da nova multa: ";
            
            if (!(std::cin >> novo_valor))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida. Digite um número positivo.");
            }

            if (novo_valor < 0)
            {
                throw std::invalid_argument("❌ O valor da multa não pode ser negativo.");
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    Emprestimo::setMulta(novo_valor);
    escreveDevagar("✅ Valor da multa alterado com sucesso!", 50);
}

void Administrador::mobilidadeAcademica(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE MOBILIDADE ACADÊMICA 📚\n";
    std::cout << "============================================\n";

    std::string matricula_aluno;
    Estudante *estudanteAlvo = nullptr;

    while (true)
    {
        try
        {
            std::cout << "-> Matrícula do aluno (ou '0' para sair): ";
            std::cin >> matricula_aluno;

            if (matricula_aluno == "0") return;

            //TODO: validarMATRICULA(matricula_aluno);

            bool encontrado = false;
            for (auto *est : estudantes)
            {
                if (est->get_matricula() == matricula_aluno)
                {
                    estudanteAlvo = est;
                    encontrado = true;
                    break;
                }
            }

            if (!encontrado)
            {
                throw std::invalid_argument("❌ Erro: Aluno não encontrado com a matrícula " + matricula_aluno);
            }

            break; 
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "\nAluno selecionado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "Curso atual: " << estudanteAlvo->get_curso() << "\n";
    std::cout << "--------------------------------------------\n";

    std::string novoCurso;

    while (true)
    {
        try 
        {
            std::cout << "-> Novo curso (Código ou Nome): ";
            std::getline(std::cin, novoCurso);

            //TODO: validarCURSO(novoCurso); 

            if (novoCurso == estudanteAlvo->get_curso()) {
                 throw std::invalid_argument("⚠️  O aluno já está matriculado neste curso.");
            }

            break;
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    estudanteAlvo->set_curso(novoCurso); 
    escreveDevagar("\n✅ Mobilidade realizada com sucesso! O aluno agora pertence ao curso: " + novoCurso + "\n", 50);
}

std::string Administrador::procurar_curso_por_codigo(std::string codigo)
{
    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");

    if (!leitura.is_open())
        throw std::runtime_error("Nao foi possivel abrir o arquivo de leitura");

    std::string linha;
    bool aux = true, encontrou = false;
    std::string nome_curso;

    while (std::getline(leitura, linha))
    {
        if (aux)
        {
            aux = false;
            continue;
        }
        auto pos = linha.find(' ');
        std::string codigo_linha = linha.substr(0, pos);
        if (codigo == codigo_linha)
        {
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
