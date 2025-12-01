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
#include "validar.hpp"

using namespace cimg_library;


static std::string deixar_maiusculo(std::string &palavra)
{
    std::string resultado;

    for (char c : palavra)
        resultado += toupper(c);

    return resultado;
}

static void escreveDevagar(const std::string &texto, int ms)
{
    for (char c : texto)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

static void pausa(int seg)
{
    std::this_thread::sleep_for(std::chrono::seconds(seg));
}

static std::string getDataAtual()
{
    auto agora = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(agora);

    std::tm *data = std::localtime(&tt);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << data->tm_mday << "/"
       << std::setw(2) << data->tm_mon + 1 << "/"
       << data->tm_year + 1900;

    return ss.str();
}

static void apagarTerminal()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    std::system("clear");
#endif
}

static std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    size_t start = 0;
    while (true)
    {
        size_t pos = s.find(delim, start);
        if (pos == std::string::npos)
        {
            elems.push_back(s.substr(start));
            break;
        }
        elems.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return elems;
}

static bool validaData(const std::string &data)
{
    // aceita D/M/YYYY, DD/MM/YYYY, com '/' como separador
    auto parts = split(data, '/');
    if (parts.size() != 3)
    {
        throw std::invalid_argument("❌ Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try
    {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    }
    catch (...)
    {
        throw std::invalid_argument("❌ Data contém caracteres inválidos");
    }
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual)
    {
        throw std::invalid_argument("❌ Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12)
    {
        throw std::invalid_argument("❌ Mês inválido");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
    {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1])
    {
        throw std::invalid_argument("❌ Dia inválido para o mês especificado");
    }

    return true;
}

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

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
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

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
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

    do
    {
        std::cout << "-> Nome: ";
        std::getline(std::cin, _nome);
        if (_nome.length() < 3)
            std::cout << "⚠️  Nome muito curto. Digite o nome completo.\n";
    } while (_nome.length() < 3);

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> CPF (apenas números, 11 dígitos): ";
        std::getline(std::cin, _cpf);
        bool apenasNumeros = std::all_of(_cpf.begin(), _cpf.end(), ::isdigit);

        if (_cpf.length() != 11 || !apenasNumeros)
        {
            std::cout << "⚠️  CPF inválido. Deve conter exatamente 11 números.\n";
            _cpf = "";
        }

        for (auto est : estudantes) {
            if (_matricula == est->get_matricula()) {
                std::cout << "⚠️  Erro: O estudante com esse CPF já está cadastrado.\n";
                return;
            }
        }

    } while (_cpf.empty());

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> Data de nascimento (DD/MM/AAAA): ";
        std::getline(std::cin, _data_de_nascimento);

        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        int anoAtual = now->tm_year + 1900;

        if (_data_de_nascimento.length() != 10) std::cout << "⚠️  Formato inválido. Use DD/MM/AAAA.\n";
        if (std::stoi(_data_de_nascimento.substr(_data_de_nascimento.size()-4)) > anoAtual-17 
         || std::stoi(_data_de_nascimento.substr(_data_de_nascimento.size()-4)) < anoAtual-120) {
            std::cout << "⚠️  Erro: O estudante não preenche os requisitos de idade para ingressar no ensino superior.\n";
            return;
        }
    } while (_data_de_nascimento.length() != 10);

    std::cout << "--------------------------------------------\n";

    do {
        std::cout<<"-> Email: ";
        std::cin>>_email;
        if (_email.substr(_email.size()-7) != "@ufv.br" || _email.size() <= 7) {
            std::cout << "⚠️  Email inválido (Utilize somente email institucional '@ufv.br').\n";
            _email = "";
        }
    } while (_email.empty());
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> Senha (min 6 caracteres): ";
        std::cin >> _senha;
        if (_senha.length() < 6)
        {
            std::cout << "⚠️  Senha muito fraca. Mínimo 6 caracteres.\n";
            _senha = "";
        }
    } while (_senha.empty());
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    bool entradaValida = false;

    do
    {
        std::cout << "-> Matrícula do aluno: ";
        std::cin >> _matricula;
        bool apenasNumeros = !_matricula.empty() && std::all_of(_matricula.begin(), _matricula.end(), ::isdigit);

        if (apenasNumeros)
        {
            entradaValida = true;

            for (auto est : estudantes) {
                if (_matricula == est->get_matricula()) {
                    std::cout << "⚠️  Erro: Já existe um estudante com essa matrícula.\n";
                    entradaValida = false;
                }
            }
        } else {
            std::cout << "⚠️  Erro: A matrícula deve conter APENAS números (sem letras ou símbolos).\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (!entradaValida);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "--------------------------------------------\n";

    do
    {
        std::cout << "-> Curso: ";
        std::getline(std::cin, _curso);
        if (_curso.empty())
            std::cout << "⚠️  Nome do curso obrigatório.\n";
    } while (_curso.empty());

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

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
int Administrador::alterarDadosEstudante(std::vector<Estudante *> &estudantes)
{
    std::cout << "\n============================================\n";
    std::cout << "   ✏️  MENU DE ALTERAÇÃO DE DADOS ✏️ \n";
    std::cout << "============================================\n";

    std::string matricula;
    bool entradaValida = false;

    // ====== LEITURA E VALIDAÇÃO DA MATRÍCULA ======
    do
    {
        std::cout << "-> Matrícula do aluno: ";
        std::cin >> matricula;

        bool apenasNumeros = !matricula.empty() &&
                             std::all_of(matricula.begin(), matricula.end(), ::isdigit);

        if (apenasNumeros)
        {
            entradaValida = true;
        }
        else
        {
            std::cout << "❌ A matrícula deve conter APENAS números (sem letras ou símbolos).\n";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (!entradaValida);

    // Busca o estudante pela matrícula
    Estudante *estudanteAlvo = nullptr;
    for (auto *est : estudantes)
    {
        if (est->get_matricula() == matricula)
        {
            estudanteAlvo = est;
            break;
        }
    }

    if (estudanteAlvo == nullptr)
    {
        throw std::invalid_argument("❌ Estudante não encontrado com a matrícula: " + matricula);
    }

    int opcao;
    std::cout << "\nEstudante encontrado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "1 - Alterar Nome\n";
    std::cout << "2 - Alterar Email\n";
    std::cout << "3 - Alterar Curso\n";
    std::cout << "4 - Alterar Senha\n";
    std::cout << "5 - Cancelar\n";
    std::cout << "--------------------------------------------\n";

    // ====== LEITURA SEGURA DA OPÇÃO ======
    while (true)
    {
        std::cout << "Opção: ";
        if (std::cin >> opcao)
        {
            break;
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
        do
        {
            std::cout << "-> Novo Nome: ";
            std::getline(std::cin, novoDado);
        } while (!validarNOME(novoDado)); // a função já imprime o motivo do erro

        estudanteAlvo->setNome(novoDado);
        break;

    case 2: // ====== ALTERAR EMAIL ======
        do
        {
            std::cout << "-> Novo Email: ";
            std::getline(std::cin, novoDado);
        } while (!validarEMAIL(novoDado)); // validarEmail já trata erro

        estudanteAlvo->setEmail(novoDado);
        break;

    case 3: // ====== ALTERAR CURSO ======
        do
        {
            std::cout << "-> Novo Curso (Sigla ou Nome): ";
            std::getline(std::cin, novoDado);

            if (novoDado.empty() || std::all_of(novoDado.begin(), novoDado.end(),
                                                [](char c)
                                                { return std::isspace((unsigned char)c); }))
            {
                std::cout << "❌ O curso não pode estar vazio.\n";
            }

        } while (novoDado.empty() || std::all_of(novoDado.begin(), novoDado.end(),
                                                 [](char c)
                                                 { return std::isspace((unsigned char)c); }));

        estudanteAlvo->set_curso(novoDado);
        break;

    case 4: // ====== ALTERAR SENHA ======
        do
        {
            std::cout << "-> Nova Senha (min 6 caracteres, com letra e número): ";
            std::getline(std::cin, novoDado);
        } while (!validarSENHA(novoDado)); // validarSenha já imprime o erro

        estudanteAlvo->setSenha(novoDado);
        break;

    case 5:
        std::cout << "Operação cancelada.\n";
        return 5;

    default:
        std::cout << "❌ Opção inválida selecionada.\n";
        return 0;
    }

    escreveDevagar("\n✅ Dados atualizados com sucesso!\n", 50);
    return opcao;
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::alterarSenhaAdministrador()
{
    std::cout << "--------------------------------------------\n";
    std::string nova_senha;
    std::string confirmacao_senha;

    do
    {
        std::cout << "-> Digite sua nova senha (min 6 caracteres): ";
        std::getline(std::cin, nova_senha);

        if (nova_senha.length() < 6)
        {
            std::cout << "⚠️  Erro: A senha é muito curta. Use no mínimo 6 caracteres.\n";
            continue;
        }

        std::cout << "-> Confirme a nova senha: ";
        std::getline(std::cin, confirmacao_senha);

        if (nova_senha != confirmacao_senha)
        {
            std::cout << "⚠️  Erro: As senhas não coincidem. Tente novamente.\n";
            std::cout << "--------------------------------------------\n";
        }

    } while (nova_senha.length() < 6 || nova_senha != confirmacao_senha);

    this->setSenha(nova_senha);
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Senha alterada com sucesso!", 50);
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
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

        resposta = toupper(resposta);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (resposta == 'S' || resposta == 'N')
        {
            break;
        }
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

        bool entradaValida = false;

        do
        {
            std::cout << "-> Matrícula do aluno: ";
            std::cin >> matricula;
            bool apenasNumeros = !matricula.empty() && std::all_of(matricula.begin(), matricula.end(), ::isdigit);

            if (apenasNumeros)
            {
                entradaValida = true;
            }
            else
            {
                std::cout << "⚠️  Erro: A matrícula deve conter APENAS números (sem letras ou símbolos).\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        } while (!entradaValida);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        apagarTerminal();

        bool encontrado = false;

        for (auto estudante : estudantes)
        {
            if (estudante->get_matricula() == matricula)
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
                    std::cout << "⚠️  Este aluno não possui carteirinha ativa.\n";
                }

                encontrado = true;
                return;
            }
        }

        if (!encontrado)
        {
            std::cout << "❌ Erro: Não foi possível localizar o Estudante com matrícula " << matricula << "\n";
        }
    }
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::consultarEmprestimos(std::vector<Estudante *> &estudantes)
{

    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CONSULTA DE EMPRÉSTIMOS 📚\n";
    std::cout << "============================================\n";

    char resposta;

    while (true)
    {
        std::cout << "# Deseja visualizar todas os empréstimos? (S/N)" << std::endl;
        std::cin >> resposta;
        if (resposta == 'S' || resposta == 's' || resposta == 'N' || resposta == 'n')
            break;
        apagarTerminal();
    }

    if (resposta == 'S' || resposta == 's')
    {
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

        bool entradaValida = false;

        do
        {
            std::cout << "-> Matrícula do aluno: ";
            std::cin >> matricula;
            bool apenasNumeros = !matricula.empty() && std::all_of(matricula.begin(), matricula.end(), ::isdigit);

            if (apenasNumeros)
            {
                entradaValida = true;
            }
            else
            {
                std::cout << "⚠️  Erro: A matrícula deve conter APENAS números (sem letras ou símbolos).\n";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }

        } while (!entradaValida);

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        apagarTerminal();

        for (auto estudante : estudantes)
        {
            if (estudante->get_matricula() == matricula)
            {
                std::cout << "--------------------------------------------\n";
                std::cout << estudante->getNome() << " - " << estudante->get_matricula() << std::endl;
                std::cout << "--------------------------------------------\n";
                estudante->exibirEmprestimos();
                return;
            }
        }
        std::cout << "❌ Não foi possível localizar o Estudante com matrícula " << matricula << "\n";
    }
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::recarregarCarteirinha(std::vector<Estudante *> &estudantes)
{

    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CARREGAMENTO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    std::string matricula;

    bool entradaValida = false;

    do
    {
        std::cout << "-> Matrícula do aluno: ";
        std::cin >> matricula;
        bool apenasNumeros = !matricula.empty() && std::all_of(matricula.begin(), matricula.end(), ::isdigit);

        if (apenasNumeros)
        {
            entradaValida = true;
        }
        else
        {
            std::cout << "⚠️  Erro: A matrícula deve conter APENAS números (sem letras ou símbolos).\n";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

    } while (!entradaValida);

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (auto estudante : estudantes)
    {
        if (estudante->get_matricula() == matricula)
        {
            estudante->recarregarCarteirinha();
            return;
        }
    }

    throw std::invalid_argument("❌ Não foi possível localizar o Estudante com matrícula " + matricula);
}

// TODO: (Thales) Implementar a função de gerar carteirinha
// Perguntar se deseja ver de todos ou de algum, perguntar matricula dele
// Pede para colocar a foto na pasta images com algum formato
// Verificar se existe e trata os erros, exibir erro se algum estudante não colocou a foto na pasta, mas gerar do resto se conseguiu ter
// Gera a carteirinha e coloca em uma pasta separada. Ex: 'Carteirinhas'
// Tenta seguir o padrão das funções quando for pedir algum dado escrito e no UI


// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::visualizarCarteirinhas(std::vector<Estudante *> &estudantes)
{
    int opcao;
    escreveDevagar("Por favor, escolha a opção de visualização desejada: ", 30);
    std::cout << std::endl;
    escreveDevagar("1 - Visualizar de um estudante", 20);
    std::cout << std::endl;
    escreveDevagar("2  - Visualizar de todos os estudantes", 20);
    std::cout << std::endl;

    while (true)
    {
        try
        {
            std::cout << "Opção: ";

            if (!(std::cin >> opcao))
            { // erro cin -- usuario digitou letra,simbolo,etc
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Entrada invalida! Digite um numero 1 ou 2.");
            }

            if (opcao < 1 || opcao > 2)
            {
                throw std::invalid_argument("Opcao invalida! Digite 1 ou 2.");
            }

            break; // caso a entrada e a opcao seja valida
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl
                      << std::endl;
        }
    }

    if (opcao == 1)
    { // visualizar de um estudante
        escreveDevagar("Por favor, preencha o campo abaixo: ", 20);
        std::cout << std::endl;
        std::string matricula_aluno;
        escreveDevagar("Matricula do aluno: ", 20);
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, matricula_aluno);

        Estudante *aluno_carteirinha = nullptr;
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
            escreveDevagar("Credenciais invalidas! Não foi possivel localizar o aluno", 30);
        }
        else
        {
            aluno_carteirinha->visualizarCarteirinha();
        }
    }

    else if (opcao == 2)
    { // visualizar de todos os estudantes
        escreveDevagar("Exibindo informações...", 30);
        std::cout << std::endl
                  << std::endl;
        for (auto estudante : estudantes)
        {
            escreveDevagar("Exibindo carteirinha de: ", 20);
            std::cout << estudante->getNome() << std::endl
                      << std::endl;
            try
            {
                estudante->visualizarCarteirinha();
            }
            catch (const std::exception &e)
            {
                std::cerr << "❌ Não foi possível gerar a carteirinha de " << estudante->getNome() << ": " << e.what() << std::endl
                          << std::endl;
            }
        }
    }
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
std::string Administrador::alterarValorRU()
{

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR RU 📚\n";
    std::cout << "============================================\n";

    std::string gradOuPos;
    char resposta;
    double novo_valor;
    std::cout << "Escolha o nível do estudante:\n";
    std::cout << "1 - Graduação\n2 - Pós-Graduação\n";
    std::cout << "Opção: ";

    while (true)
    {
        std::cin >> resposta;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (resposta == '1' || resposta == '2')
        {
            break;
        }
        std::cout << "❌ Opção inválida. Digite 1 ou 2: ";
    }

    std::cout << "-> Novo valor: ";

    while (true)
    {
        std::cin >> novo_valor;

        if (std::cin.fail() || novo_valor < 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Valor inválido. Digite um número positivo: ";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    if (resposta == '1')
    {
        EstudanteGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Graduação";
    }
    else if (resposta == '2')
    {
        EstudantePosGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Pós-Graduação";
    }

    escreveDevagar("\nValor alterado com sucesso! ✅", 50);
    return gradOuPos;
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::alterarValorMulta()
{

    std::cout << "\n=================================================================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR NA MULTA DE EMPRÉSTIMO 📚\n";
    std::cout << "=================================================================================\n";

    double novo_valor;
    std::cout << "-> Valor da nova multa: ";

    while (true)
    {
        std::cin >> novo_valor;

        if (std::cin.fail() || novo_valor < 0)
        {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "❌ Valor inválido. Digite um número positivo: ";
        }
        else
        {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
    }

    Emprestimo::setMulta(novo_valor);
    escreveDevagar("✅ Valor da multa alterado com sucesso!", 50);
}

// TODO: Consertar a função, não está utilizando os métodos de validação
// Os erros não devem ser impressos devem usar o throw, utilizar a lógica de while a entrada não for valida, nao mandar o throw la pra fora
void Administrador::mobilidadeAcademica(std::vector<Estudante *> &estudantes)
{

    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");

    // Verifica se o arquivo abriu antes de pedir dados ao usuário
    if (!leitura.is_open())
    {
        std::cout << "❌ Erro crítico: Não foi possível abrir o banco de dados de cursos (codigo_cursos.txt).\n";
        return;
    }

    std::string matricula_aluno, parametro;

    while (true)
    {
        std::cout << "Matricula do Aluno: ";
        std::getline(std::cin, matricula_aluno);

        if (matricula_aluno.empty())
        {
            std::cout << "Entrada vazia. Digite novamente." << std::endl;
            continue;
        }

        bool numero = true;

        for (char c : matricula_aluno)
        {
            if (!isdigit(c))
            {
                numero = false;
                break;
            }
        }

        if (!numero)
        {
            std::cout << "A matricula deve conter apenas numeros." << std::endl;
            continue;
        }

        break;
    }

    Estudante *estudante = nullptr;
    for (auto aluno : estudantes)
    {
        if (aluno->get_matricula() == matricula_aluno)
        {
            estudante = aluno;
            break;
        }
    }

    if (estudante == nullptr)
    {
        std::cout << "❌ Aluno não encontrado com a matrícula " << matricula_aluno << ".\n";
        leitura.close();
        return;
    }

    std::cout << "Aluno selecionado: " << estudante->getNome() << "\n";
    std::cout << "--------------------------------------------\n";

    while (true)
    {
        std::cout << "Novo curso (digite o CODIGO ou o NOME): ";
        std::getline(std::cin, parametro);

        if (!parametro.empty())
            break;
        std::cout << "⚠️  O campo não pode ser vazio.\n";
    }

    bool numerico = std::all_of(parametro.begin(), parametro.end(), ::isdigit);

    if (!numerico)                               // se o parametro for pelo nome do curso
        parametro = deixar_maiusculo(parametro); // deixamos o parametro todo em maisculo para procurar no .txt

    std::string linha;
    bool aux = true;
    std::string nome_curso, codigo_curso;
    bool encontrou = false;

    while (std::getline(leitura, linha))
    {
        if (aux)
        { // ignorar a primeira linha (lixo)
            aux = false;
            continue;
        }

        auto pos = linha.find(' '); // acha o espaço -- antes do espaço (codigo numerico) e depois do espaço (nome do curso)
        codigo_curso = linha.substr(0, pos);
        nome_curso = linha.substr(pos + 1);

        if (numerico)
        { // se o parametro passado para mobilidade é o codigo da disciplina
            if (codigo_curso == parametro)
            {
                encontrou = true;
                break; // nao precisamos mais ler o arquivo
            }
        }
        else
        { // se o parametro passado para mobilidade é o nome da disciplina
            if (nome_curso == parametro)
            {
                encontrou = true;
                break;
            }
        }
    }

    leitura.close();

    if (!encontrou)
    {
        std::cout << "❌ Curso não encontrado! Verifique se digitou o código ou nome corretamente.\n";
        return;
    }

    estudante->set_curso(codigo_curso); // altera o curso que o aluno está fazendo
    escreveDevagar("✅ Mobilidade realizada! O aluno agora pertence ao curso: " + codigo_curso + "\n", 50);
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
