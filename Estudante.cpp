#define cimg_display 0
#include "Estudante.hpp"
#include "Usuario.hpp"                 // Classe base de Estudante
#include "Administrador.hpp"           // Para procurar_curso_por_codigo()
#include "Auxiliares.hpp"              // Funções utilitárias (getDataAtual, addDias, etc)
#include <limits>                      // Para std::numeric_limits
#include <stdexcept>                   // Para exceções padrão
#include <iostream>                    // Para entrada/saída padrão
#include <ctime>                       // Para operações com tempo
#include <sstream>                     // Para stringstream (conversão)
#include <chrono>                      // Para operações com tempo (chrono)
#include <iomanip>                     // Para formatação de saída (setprecision, fixed)
#include "CImg.h"                      // Biblioteca CImg para manipulação de imagens
#include <chrono>                      // Duplicado (pode ser removido)
#include <thread>                      // Para threads (se necessário)
#include <filesystem>                  // Para operações com diretórios (create_directory, exists)
#include <vector>                      // Para vetor de extensões de imagem

// ========== CONSTRUTOR ==========
// Inicializa um estudante herdando dados do Usuario e criando automaticamente uma carteirinha
// Parâmetros:
//   _nome: nome completo do estudante
//   _cpf: CPF do estudante
//   _data_de_nascimento: data de nascimento (DD/MM/YYYY)
//   _email: email institucional UFV
//   _senha: senha de acesso
//   _matricula: matrícula universitária (6 dígitos)
//   _curso: código do curso (ex: 101, 102)
// Efeito colateral: cria automaticamente uma Carteirinha nova (saldo zerado)
Estudante::Estudante(const std::string &_nome, const std::string &_cpf, std::string &_data_de_nascimento,
                     const std::string &_email, const std::string &_senha, const std::string &_matricula,
                     const std::string &_curso)
    : Usuario(_nome, _cpf, _data_de_nascimento, _email, _senha), matricula(_matricula), curso(_curso), emprestimos()
{
    this->carteirinha = new Carteirinha();  // Aloca nova carteirinha com saldo 0
}

// ========== DESTRUTOR ==========
// Libera memória alocada dinamicamente (empréstimos e carteirinha)
// Percorre vetor de empréstimos deletando cada ponteiro
// Depois deleta a carteirinha
Estudante::~Estudante()
{
    // Deleta todos os empréstimos
    for (auto emprestimo : this->emprestimos)
    {
        delete emprestimo;
    }
    // Deleta a carteirinha
    delete this->carteirinha;
}

// ========== ADICIONAR EMPRÉSTIMO ==========
// Adiciona um empréstimo já criado ao vetor de empréstimos do estudante
// Parâmetro:
//   e: ponteiro para um Emprestimo já alocado
void Estudante::adicionarEmprestimo(Emprestimo *e) {
    this->emprestimos.push_back(e);
}

// ========== EXIBIR EMPRÉSTIMOS ==========
// Lista todos os empréstimos ativos do estudante com informações formatadas
// Se não houver empréstimos, exibe mensagem informativa
void Estudante::exibirEmprestimos()
{
    std::cout << "\n============================================\n";
    std::cout << "          EMPRÉSTIMOS            ";
    std::cout << "\n============================================\n";

    // Verifica se há empréstimos pendentes
    if (this->get_emprestimos().size() == 0)
    {
        std::cout << "Nao ha emprestimos pendentes\n";
        return;
    }

    // Exibe informações de cada empréstimo
    for (auto emprestimo : this->emprestimos)
    {
        emprestimo->exibirInformacoes();
    }
}

// ========== PEGAR LIVRO EMPRESTADO ==========
// Realiza todo o processo de empréstimo de um livro:
//   1. Lista livros disponíveis na biblioteca
//   2. Solicita escolha do livro por ID
//   3. Valida disponibilidade
//   4. Cria registro de empréstimo com datas
//   5. Reduz quantidade disponível
// Parâmetro:
//   biblioteca: referência à Biblioteca (para listar e buscar livros)
// Exceções: tratadas com try-catch para validação de entrada e disponibilidade
void Estudante::pegarLivro(const Biblioteca &biblioteca)
{
    escreveDevagar("\n============================================\n", 10);
    escreveDevagar("      📕 BEM-VINDO À BIBLIOTECA 📕 ", 50);
    escreveDevagar("\n============================================\n", 10);

    Livro *livro_desejado = nullptr;
    biblioteca.listarLivros();  // Exibe livros com filtro e paginação

    // --- SELEÇÃO DO LIVRO COM VALIDAÇÃO ---
    while(1){
        std::cout << "-> Digite o ID do livro: ";
        int id_do_livro;

        try
        {
            // Lê ID com validação
            if (!(std::cin >> id_do_livro))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ ID inválido");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // Busca livro no acervo pelo ID
            for (Livro *livro : biblioteca.getAcervo())
            {
                if (id_do_livro == livro->getId())
                {
                    livro_desejado = livro;
                    break;
                }
            }

            // Validações
            if (!livro_desejado)
                throw std::invalid_argument("❌ O livro escolhido não foi encontrado!");

            if (livro_desejado->getNumExemplaresDisponiveis() == 0)
                throw std::invalid_argument("❌ Não há exemplares disponíveis!");

            break;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
            std::cout << "--------------------------------------------\n";
        }
    }

    // --- REGISTRO DO EMPRÉSTIMO ---
    // Reduz quantidade disponível
    livro_desejado->setNumExemplaresDisponiveis(livro_desejado->getNumExemplaresDisponiveis() - 1);

    // Calcula datas do empréstimo
    std::string dataDeEmprestimo = getDataAtual();                                   // Data de hoje
    std::string dataDeDevolucao = addDias(dataDeEmprestimo, this->get_prazoDeDevolucao());  // Data limite

    // Cria objeto Emprestimo e adiciona ao vetor
    Emprestimo *novoEmprestimo = new Emprestimo(*this, *livro_desejado, dataDeEmprestimo, dataDeDevolucao);
    this->emprestimos.push_back(novoEmprestimo);

    escreveDevagar("✅ Livro emprestado com sucesso!\n", 50);
}

// ========== DEVOLVER LIVRO ==========
// Realiza todo o processo de devolução de um livro:
//   1. Lista empréstimos não devolvidos
//   2. Solicita qual empréstimo devolver
//   3. Verifica atraso e calcula multa
//   4. Se atrasado, cobra multa (com opção de pagamento)
//   5. Marca como devolvido e reintegra livro ao acervo
// Parâmetro:
//   biblioteca: referência à Biblioteca (usada para contexto)
void Estudante::devolverLivro(const Biblioteca &biblioteca)
{
    escreveDevagar("\n============================================\n", 10);
    escreveDevagar("       📕 BEM-VINDO À BIBLIOTECA 📕 ", 50);
    escreveDevagar("\n============================================\n", 10);

    bool temADevolver = false;

    // --- LISTAR EMPRÉSTIMOS NÃO DEVOLVIDOS ---
    // Exibe apenas empréstimos pendentes (ainda não devolvidos)
    for (auto emprestimo : this->get_emprestimos())
    {
        if (!emprestimo->isDevolvido())
        {
            emprestimo->exibirInformacoes();
            temADevolver = true;
        }
    }

    Emprestimo *livro_devolvido = nullptr;

    // Se não há nada a devolver, interrompe
    if (!temADevolver)
    {
        std::cout << "Nao há empréstimos pendentes\n";
        return;
    }

    // --- SELEÇÃO DO EMPRÉSTIMO A DEVOLVER ---
    while (1)
    {
        std::cout << "--------------------------------------------\n";
        std::cout << "-> Escolha o ID do empréstimo: ";

        int id_livro_devolvido;

        try
        {
            if (!(std::cin >> id_livro_devolvido))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Entrada inválida!");
            }

            // Busca empréstimo pelo ID
            for (auto emprestimo : this->emprestimos)
            {
                if (emprestimo->getId() == id_livro_devolvido && !emprestimo->isDevolvido())
                    livro_devolvido = emprestimo;
            }

            if (!livro_devolvido)
                throw std::invalid_argument("❌ O empréstimo não foi encontrado!");

            break;
        }
        catch (std::invalid_argument &e)
        {
            std::cerr << e.what() << std::endl;
        }
    }

    // --- VERIFICAÇÃO DE ATRASO E MULTA ---
    // Se há atraso, calcula multa e oferece opcões de pagamento
    if (livro_devolvido->calculaValorMulta() > 0.0)
    {
        std::cout << "--------------------------------------------\n";

        // Formata mensagem com dias de atraso e valor da multa
        std::ostringstream oss;
        oss << "O livro está atrasado por " << livro_devolvido->getDiasDeAtraso()
            << " dia(s).\nMulta: R$"
            << std::fixed << std::setprecision(2)
            << livro_devolvido->calculaValorMulta() << "\n";

        escreveDevagar(oss.str(), 50);

        // --- VERIFICAÇÃO DE SALDO ---
        // Se tem saldo suficiente, pergunta se deseja pagar
        if (this->get_carteirinha()->getSaldo() >= livro_devolvido->calculaValorMulta())
        {
            char resposta;

            while (1)
            {
                std::cout << "--------------------------------------------\n";
                std::cout << "Deseja descontar a multa do saldo? (S/N): ";

                try
                {
                    std::cin >> resposta;

                    if (resposta != 'S' && resposta != 'N' && resposta != 's' && resposta != 'n')
                        throw std::invalid_argument("❌ Digite S ou N!");

                    break;
                }
                catch (...)
                {
                    std::cout << "❌ Entrada inválida.\n";
                }
            }

            // Se confirmar, debita a multa
            if (resposta == 'S' || resposta == 's')
                this->get_carteirinha()->debitar(livro_devolvido->calculaValorMulta());
        }
        // --- SALDO INSUFICIENTE ---
        // Oferece opção de recarregar carteirinha
        else
        {
            std::cout << "--------------------------------------------\n";
            std::cout << "Saldo insuficiente. Deseja recarregar?\n";

            while (1)
            {
                std::cout << "0 - Não\n1 - Sim\nOpção: ";
                int opcao;

                try
                {
                    if (!(std::cin >> opcao))
                    {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::invalid_argument("❌ Entrada inválida!");
                    }

                    if (opcao == 0)
                        return;                            // Cancela devolução

                    else if (opcao == 1)
                    {
                        this->recarregarCarteirinha();    // Recarrega
                        this->carteirinha->debitar(livro_devolvido->calculaValorMulta());  // Cobra multa
                    }
                    else
                    {
                        throw std::invalid_argument("❌ Opção inválida!");
                    }

                    break;
                }
                catch (...)
                {
                    std::cout << "❌ Erro na entrada.\n";
                }
            }
        }
    }

    // --- FINALIZAÇÃO DA DEVOLUÇÃO ---
    // Marca como devolvido e reintegra livro ao acervo
    livro_devolvido->setDevolvido(true);
    livro_devolvido->getLivro()->setNumExemplaresDisponiveis(
        livro_devolvido->getLivro()->getNumExemplaresDisponiveis() + 1);

    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Livro devolvido com sucesso!\n", 50);
}

// ========== RECARREGAR CARTEIRINHA ==========
// Permite ao estudante depositar dinheiro na carteirinha
// Funcionamento:
//   1. Exibe saldo atual
//   2. Solicita valor a depositar (aceita vírgula ou ponto decimal)
//   3. Valida se valor é positivo
//   4. Deposita na carteirinha
void Estudante::recarregarCarteirinha()
{
    this->consultarSaldo();         // Exibe saldo atual
    double valor = 0.0;
    std::string input;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // --- LEITURA E VALIDAÇÃO DO VALOR ---
    while (1)
    {
        std::cout << "-> Digite o valor a ser depositado: R$";
        std::getline(std::cin, input);

        try
        {
            // Aceita vírgula como separador decimal (Brasil)
            size_t pos = input.find(',');
            if (pos != std::string::npos)
                input.replace(pos, 1, ".");   // Converte vírgula para ponto

            valor = std::stod(input);         // Converte para double

            if (valor <= 0.0)
                throw std::invalid_argument("❌ Valor deve ser maior que zero.");

            break;
        }
        catch (...)
        {
            std::cout << "❌ Entrada inválida.\n";
        }
    }

    // Deposita na carteirinha
    this->carteirinha->depositar(valor);
    escreveDevagar("✅ Valor depositado com sucesso!\n", 50);
}

// ========== VISUALIZAR CARTEIRINHA ==========
// Gera arquivo de imagem da carteirinha com foto e dados do aluno
// Funcionamento:
//   1. Solicita extensão da foto (PNG, JPG, BMP)
//   2. Carrega template de carteirinha
//   3. Carrega foto do aluno (nome_matricula_foto.ext)
//   4. Insere foto e código de barras
//   5. Adiciona textos (nome, curso, matrícula, CPF, validade, data emissão)
//   6. Salva em carteirinhas/nome_matricula_CARTEIRINHA.bmp
// Exceções: lança std::runtime_error se não conseguir carregar arquivos de imagem
void Estudante::visualizarCarteirinha()
{
    std::cout << "\n============================================\n";
    std::cout << "  📚 VISUALIZAÇÃO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    escreveDevagar("Antes de visualizar, adicione a imagem do aluno na pasta images. A extensão da imagem pode ser: .png / .jpg / .bmp", 30);
    std::cout << "\n--------------------------------------------\n";

    // --- CARREGAMENTO DE ARQUIVOS DE IMAGEM ---
    CImg<unsigned char> img;
    CImg<unsigned char> barcode;

    try { img.assign("images/template.bmp"); }
    catch (const cimg_library::CImgIOException&) {
        std::cerr<<"❌ Erro: não foi possível carregar 'images/template.bmp'.\n";
        return;
     }

    try { barcode.assign("images/codigo_barra.bmp"); }
    catch (const cimg_library::CImgIOException&) { 
        std::cerr << "❌ Erro: não foi possível carregar 'images/codigo_barra.bmp'.\n";
        return;
    }

    // --- PREPARAÇÃO DE DADOS ---
    // Extrai primeiro nome do estudante
    std::string aux = getNome();
    auto pos = aux.find(" ");
    std::string primeiroNome = (pos == std::string::npos ? aux : aux.substr(0, pos));
    primeiroNome = deixar_maiusculo(primeiroNome);

    if(primeiroNome.empty()){
        std::cerr<<"❌ Nome do aluno inválido!\n";
        return;
        }

    // Monta caminho do arquivo da foto do aluno
    std::string base = "images/" + primeiroNome + "_" + get_matricula() + "_foto";
    
    //Descobrir extensão da imagem automaticamente
    std::vector <std::string> extensoes = {".png", ".jpg", ".jpeg", ".bmp"};

    std::string nome_foto;

    for(auto &extensao: extensoes){
        std::string tentativa = base + extensao;
        if(std::filesystem::exists(tentativa)){
            nome_foto = tentativa;
            break;
        }
    }

    if(nome_foto.empty()){
        std::cerr<<"❌ Nenhuma foto encontrada!\n";
        std::cerr<<"Procurei pelos arquivos:\n";
        for(auto &extensao : extensoes){
            std::cerr<<" - "<<base + extensao <<std::endl;
        }
        return;
    }

    CImg<unsigned char> aluno;
    std::cout<<"📸 Foto encontrada: " << nome_foto << "\n";
    std::cout<<"Carregando imagem..."<<std::endl;
    
    try { aluno.assign(nome_foto.c_str()); }
    catch (const cimg_library::CImgIOException &erro) { 
        std::cerr<<"❌ Não foi possivel carregar o arquivo da foto do aluno: " << nome_foto<<std::endl;
        return;
    }

    // --- REDIMENSIONAMENTO E INSERÇÃO DE ELEMENTOS ---
    aluno.resize(350, 525);        // Redimensiona foto para caber na carteirinha
    img.draw_image(33, 314, aluno);  // Insere foto no template

    barcode.resize(998, 192);      // Redimensiona código de barras
    img.draw_image(397, 749, barcode);  // Insere barcode

    // --- CRIAÇÃO DE MÁSCARA E ADIÇÃO DE TEXTOS ---
    CImg<unsigned char> mask(img.width(), img.height(), 1, 3, 0);
    unsigned char branco[] = {255, 255, 255};

    // Adiciona informações do aluno na máscara
    mask.draw_text(520, 321, getNome().c_str(), branco, 0, 255, 30);
    mask.draw_text(523, 404, Administrador::procurar_curso_por_codigo(get_curso()).c_str(), branco, 0, 255, 30);
    mask.draw_text(577, 488, get_matricula().c_str(), branco, 0, 255, 30);
    mask.draw_text(475, 572, getCpf().c_str(), branco, 0, 255, 30);
    mask.draw_text(564, 655, "06/2026", branco, 0, 255, 30);
    mask.draw_text(1173, 720, getDataAtual().c_str(), branco, 0, 255, 30);

    // Criação da pasta carteirinhas, se não existir
    if (!std::filesystem::exists("carteirinhas"))
        std::filesystem::create_directory("carteirinhas");

    // Salva carteirinha personalizada
    std::string nomeArquivo = "carteirinhas/" + primeiroNome + "_" + get_matricula() + "_CARTEIRINHA.bmp";

    aplicarTextoPreto(img, mask);
    img.save(nomeArquivo.c_str());

    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Carteirinha criada com sucesso!\n", 30);
}


// ========== CONSULTAR SALDO ==========
// Exibe o saldo atual da carteirinha formatado como moeda
void Estudante::consultarSaldo()
{
    std::cout << "--------------------------------------------\n";
    std::cout << std::fixed << std::setprecision(2)
              << "Seu saldo é de R$" << this->carteirinha->getSaldo() << "\n";
    std::cout << "--------------------------------------------\n";
}

// ========== GETTERS E SETTERS ==========
// Métodos de acesso (getters) e modificação (setters) para atributos privados
// Getters simples
std::string Estudante::get_matricula() const { return this->matricula; }
std::string Estudante::get_curso() const { return this->curso; }
Carteirinha *Estudante::get_carteirinha() const { return this->carteirinha; }
std::vector<Emprestimo *> Estudante::get_emprestimos() const { return this->emprestimos; }

// Setters simples
void Estudante::set_matricula(std::string _matricula) { this->matricula = _matricula; }
void Estudante::set_curso(std::string _curso) { this->curso = _curso; }
