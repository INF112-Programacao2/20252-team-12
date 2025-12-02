#define cimg_display 0
#include "Estudante.hpp"
#include "Usuario.hpp"
#include "Administrador.hpp"
#include "Auxiliares.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>
#include "CImg.h"
#include <chrono>
#include <thread>
#include <filesystem>

Estudante::Estudante(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento, const std::string &_email, const std::string &_senha, const std::string &_matricula, const std::string &_curso) : Usuario(_nome, _cpf, _data_de_nascimento, _email, _senha), matricula(_matricula), curso(_curso), emprestimos()
{
    this->carteirinha = new Carteirinha();
}

Estudante::~Estudante()
{
    for (auto emprestimo : this->emprestimos)
    {
        delete emprestimo;
    }
    delete this->carteirinha;
}

void Estudante::adicionarEmprestimo(Emprestimo *e) {
    this->emprestimos.push_back(e);
}

void Estudante::exibirEmprestimos()
{
    std::cout << "\n============================================\n";
    std::cout << "          EMPRÉSTIMOS            ";
    std::cout << "\n============================================\n";
    if (this->get_emprestimos().size() == 0)
    {
        std::cout << "Nao ha emprestimos pendentes\n";
        return;
    }
    for (auto emprestimo : this->emprestimos)
    {
        emprestimo->exibirInformacoes();
    }
}

void Estudante::pegarLivro(const Biblioteca &biblioteca)
{
    escreveDevagar("\n============================================\n", 10);
    escreveDevagar("      📕 BEM-VINDO À BIBLIOTECA 📕 ", 50);
    escreveDevagar("\n============================================\n", 10);
    Livro *livro_desejado = nullptr;

    biblioteca.listarLivros();

    while(1){
        std::cout << "-> Digite o ID do livro: ";

        int id_do_livro;

        try
        {
            if (!(std::cin >> id_do_livro))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ ID inválido");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            for (Livro *livro : biblioteca.getAcervo())
            {
                if (id_do_livro == livro->getId())
                {
                    livro_desejado = livro;
                    break;
                }
            }

            if (livro_desejado == nullptr)
            {
                throw std::invalid_argument("❌ O livro escolhido nao foi encontrado!");
            }
            if (livro_desejado->getNumExemplaresDisponiveis() == 0)
            {
                throw std::invalid_argument("❌ Nao ha exemplares disponiveis para o livro selecionado!");
            }
            break;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
            std::cout << "--------------------------------------------\n";
        }
    }

    livro_desejado->setNumExemplaresDisponiveis(livro_desejado->getNumExemplaresDisponiveis() - 1);

    std::string dataDeEmprestimo = getDataAtual();

    std::string dataDeDevolucao = addDias(dataDeEmprestimo, this->get_prazoDeDevolucao());

    Emprestimo *novoEmprestimo = new Emprestimo(*this, *livro_desejado, dataDeEmprestimo, dataDeDevolucao);
    this->emprestimos.push_back(novoEmprestimo);

    escreveDevagar("✅ Livro emprestado com sucesso!\n", 50);
}

void Estudante::devolverLivro(const Biblioteca &biblioteca)
{
    escreveDevagar("\n============================================\n", 10);
    escreveDevagar("       📕 BEM-VINDO À BIBLIOTECA 📕 ", 50);
    escreveDevagar("\n============================================\n", 10);
    bool temADevolver = false;
    for (auto emprestimo : this->get_emprestimos())
    {
        if (!emprestimo->isDevolvido())
        {
            emprestimo->exibirInformacoes();
            temADevolver = true;
        }
    }
    Emprestimo *livro_devolvido = nullptr;
    if (!temADevolver || this->get_emprestimos().size() == 0)
    {
        std::cout << "Nao há empréstimos pendentes\n";
        return;
    }
    while (1)
    {
        std::cout << "--------------------------------------------\n";
        std::cout << "-> Escolha o ID do empréstimo que deseja devolver: ";

        int id_livro_devolvido;

        try
        {
            if (!(std::cin >> id_livro_devolvido))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida! Digite um número.");
            }

            if (id_livro_devolvido < 1 || id_livro_devolvido > this->emprestimos.size())
            {
                throw std::invalid_argument("❌ Opção inválida! Digite 1 ou 2.");
            }

            for (auto emprestimo : this->emprestimos)
            {
                if (emprestimo->getId() == id_livro_devolvido && !emprestimo->isDevolvido())
                {
                    livro_devolvido = emprestimo;
                }
            }
            if (livro_devolvido == nullptr)
            {
                throw std::invalid_argument("❌ O livro escolhido nao esta na sua lista de emprestimos!");
            }
            break;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    if (livro_devolvido->calculaValorMulta() > 0.0)
    {
        std::cout << "--------------------------------------------\n";
        std::ostringstream oss;
        oss << "O livro esta atrasado por " << livro_devolvido->getDiasDeAtraso() << " dia (s).\nA multa pelo atraso e de R$" << std::fixed << std::setprecision(2) << livro_devolvido->calculaValorMulta() << "\n";
        escreveDevagar(oss.str(), 50);

        if (this->get_carteirinha()->getSaldo() >= livro_devolvido->calculaValorMulta())
        {
            char resposta;
            while (1)
            {
                std::cout << "--------------------------------------------\n";
                std::cout << std::fixed << std::setprecision(2) << "Deseja descontar a multa do saldo da carteirinha? (RS" << this->get_carteirinha()->getSaldo() << ") S/N: ";

                try
                {
                    std::cin >> resposta;

                    if (resposta != 'S' && resposta != 'N' &&
                        resposta != 's' && resposta != 'n')
                        throw std::invalid_argument("❌ Responda apenas com S para 'sim' ou N para 'nao'");

                    break;
                }
                catch (std::invalid_argument &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }

            if (resposta == 'S' || resposta == 's')
            {
                this->get_carteirinha()->debitar(livro_devolvido->calculaValorMulta());
            }
        }
        else
        {
            std::cout << "--------------------------------------------\n";
            std::cout << std::fixed << std::setprecision(2) << "Deseja realizar uma recarga? (Valor necessário para o pagamento: "
                      << (livro_devolvido->calculaValorMulta() - this->get_carteirinha()->getSaldo())
                      << ")" << std::endl;

            while (1)
            {
                std::cout << "0 - Não desejo recarregar minha carteirinha (Voltar para o Menu)\n";
                std::cout << "1 - Sim, desejo recarregar minha carteirinha\n";
                std::cout << "--------------------------------------------\n";
                int opcao;
                std::cout << "-> Opção: ";

                try
                {
                    if (!(std::cin >> opcao))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::invalid_argument("❌ Digite um número válido!");
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (opcao == 0)
                    {
                        return;
                    }
                    else if (opcao == 1)
                    {
                        this->recarregarCarteirinha();
                        this->carteirinha->debitar(livro_devolvido->calculaValorMulta());
                    }
                    else
                    {
                        throw std::invalid_argument("❌ Digite uma opção válida!");
                    }
                    break;
                }
                catch (std::invalid_argument &e)
                {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }

    livro_devolvido->setDevolvido(true);
    livro_devolvido->getLivro()->setNumExemplaresDisponiveis(livro_devolvido->getLivro()->getNumExemplaresDisponiveis() + 1);
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Livro devolvido com sucesso!\n", 50);
}

void Estudante::recarregarCarteirinha()
{
    this->consultarSaldo();

    double valor = 0.0;
    std::string input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (1)
    {
        std::cout << "-> Digite o valor a ser depositado: R$";
        std::getline(std::cin, input);

        try
        {
            size_t pos = input.find(',');
            if (pos != std::string::npos) {
                input.replace(pos, 1, ".");
            }
            valor = std::stod(input);

            if (valor <= 0.0)
            {
                throw std::invalid_argument("❌ O valor deve ser maior que R$ 0.00.");
            }
            
            break; 
        }
        catch (const std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }
    this->carteirinha->depositar(valor);
    escreveDevagar("✅ Valor depositado com sucesso!\n", 50);
}

void Estudante::visualizarCarteirinha()
{

    std::cout << "\n============================================\n";
    std::cout << "  📚 VISUALIZAÇÃO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    escreveDevagar("Antes de visualizar a carteirinha, adicione a imagem do estudante na pasta images (PRIMEIRONOMEALUNO_MATRICULA_(formato da imagem))", 30);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("Selecione qual e a extensao do arquivo adicionado:", 30);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("1 - .PNG", 20);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("2 - .JPG/JPEG", 20);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";
    escreveDevagar("3 - .BMP", 20);
    std::cout << std::endl;
    std::cout << "--------------------------------------------\n";

    int opcao;

    while (true)
    {
        try
        {
            std::cout << "-> Opção: ";

            if (!(std::cin >> opcao))
            { // erro cin -- usuario digitou letra,simbolo,etc
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada invalida! Digite um numero 1,2 ou 3.");
            }

            if (opcao < 1 || opcao > 3)
            {
                throw std::invalid_argument("❌ Opcao invalida! Digite 1, 2 ou 3.");
            }

            break; // caso a entrada e a opcao seja valida
        }
        catch (const std::exception &e)
        {
            std::cout << e.what() << std::endl;
        }
    }

    // carregar as imagens de template base
    CImg<unsigned char> img;
    CImg<unsigned char> barcode;

    try
    {
        img.assign("images/template.bmp");
    }
    catch (const cimg_library::CImgIOException &erro)
    {
        throw std::runtime_error("❌ Não foi possível carregar template.bmp");
    }

    try
    {
        barcode.assign("images/codigo_barra.bmp");
    }
    catch (const cimg_library::CImgIOException &erro)
    {
        throw std::runtime_error("❌ Não foi possível carregar barcode.bmp");
    }

    // capturar o primeiro nome do estudante
    std::string aux = getNome();
    auto pos = aux.find(" ");
    std::string primeiroNome;
    if (pos == std::string::npos)
        primeiroNome = aux;
    else
        primeiroNome = aux.substr(0, pos);

    primeiroNome = deixar_maiusculo(primeiroNome);

    // procurar uma foto que é salva no formato "PRIMEIRONOMEALUNO_MATRICULA_foto.bmp" - ex: para Julio Soares dos Reis "JULIO_3213131_foto.bmp"
    std::string nome_foto_aluno = "images/";
    nome_foto_aluno += primeiroNome;
    nome_foto_aluno += "_";
    nome_foto_aluno += get_matricula();
    nome_foto_aluno += "_foto";

    if (opcao == 1)
        nome_foto_aluno += ".png";
    else if (opcao == 2)
        nome_foto_aluno += ".jpg";
    else if (opcao == 3)
        nome_foto_aluno += ".bmp";

    CImg<unsigned char> aluno;

    try
    {
        aluno.assign(nome_foto_aluno.c_str()); // verificar se existe o arquivo da foto do aluno
    }
    catch (const cimg_library::CImgIOException &erro)
    {
        throw std::runtime_error("❌ Não foi possivel carregar o arquivo da foto do aluno: " + nome_foto_aluno);
    }

    // colocar a imagem do aluno e do codigo de barra no local certo
    aluno.resize(350, 525);
    img.draw_image(33, 314, aluno);

    barcode.resize(998, 192);
    img.draw_image(397, 749, barcode);

    // criar mascara RGB para por o texto
    CImg<unsigned char> mask(img.width(), img.height(), 1, 3, 0);
    unsigned char branco[] = {255, 255, 255};

    // desenha o texto na mascara
    mask.draw_text(520, 321, getNome().c_str(), branco, 0, 255, 30, false); //.c_str() converte string para const char
    mask.draw_text(523, 404, Administrador::procurar_curso_por_codigo(get_curso()).c_str(), branco, 0, 255, 30, false);
    mask.draw_text(577, 488, get_matricula().c_str(), branco, 0, 255, 30, false);
    mask.draw_text(475, 572, getCpf().c_str(), branco, 0, 255, 30, false);
    mask.draw_text(564, 655, "06/2026", branco, 0, 255, 30, false);
    mask.draw_text(1173, 720, getDataAtual().c_str(), branco, 0, 255, 30, false);

    if (!std::filesystem::exists("carteirinhas"))          // caso nao haja uma pasta chamada 'carteirinhas' -- segurança extra
        std::filesystem::create_directory("carteirinhas"); // cria a pasta

    std::string nomeArquivo = "carteirinhas/" + primeiroNome + "_" + get_matricula() + "_CARTEIRINHA.bmp"; // salvar a carteirinha com nome personalizado conforme o primeiro nome  e a matricula do aluno

    aplicarTextoPreto(img, mask);
    img.save(nomeArquivo.c_str());
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Carteirinha criada com sucesso! Verifique a pasta 'Carteirinhas' para visualizar\n", 30);
}

void Estudante::consultarSaldo()
{
    std::cout << "--------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2) << "Seu saldo é de R$" << this->carteirinha->getSaldo() << "\n";
    std::cout << "--------------------------------------------\n";
}

std::string Estudante::get_matricula() const
{
    return this->matricula;
}

std::string Estudante::get_curso() const
{
    return this->curso;
}

Carteirinha *Estudante::get_carteirinha() const
{
    return this->carteirinha;
}

std::vector<Emprestimo *> Estudante::get_emprestimos() const
{
    return this->emprestimos;
}

void Estudante::set_matricula(std::string _matricula)
{
    this->matricula = _matricula;
}

void Estudante::set_curso(std::string _curso)
{
    this->curso = _curso;
}