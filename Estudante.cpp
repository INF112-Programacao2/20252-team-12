#define cimg_display 0
#include "Estudante.hpp"
#include "Usuario.hpp"
#include "Administrador.hpp"
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

using namespace cimg_library;

std::string getDataAtual() {
    auto agora = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(agora);

    std::tm* data = std::localtime(&tt);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << data->tm_mday << "/"
       << std::setw(2) << data->tm_mon + 1 << "/"
       << data->tm_year + 1900;

    return ss.str();
}

std::string addDias(const std::string& data_str, int dias) {
    int d, m, a;
    char sep1, sep2;

    std::stringstream ss(data_str);
    ss >> d >> sep1 >> m >> sep2 >> a;

    std::tm data_tm = {};
    data_tm.tm_mday = d;
    data_tm.tm_mon  = m - 1;
    data_tm.tm_year = a - 1900;

    std::time_t t = std::mktime(&data_tm);

    t += dias * 24 * 60 * 60;

    std::tm* nova_data = std::localtime(&t);

    std::stringstream out;
    out << std::setfill('0') << std::setw(2) << nova_data->tm_mday << "/"
        << std::setw(2) << nova_data->tm_mon + 1 << "/"
        << nova_data->tm_year + 1900;

    return out.str();
}

// Funcao de comparacao case insensitive para conferir nomes de livros
bool caseInsensitiveComp(const std::string a, const std::string b) {
    if (a.size() != b.size())
        return false;

    for (size_t i = 0; i < a.size(); i++) {
        if (std::toupper((unsigned char)a[i]) != std::toupper((unsigned char)b[i]))
            return false;
    }

    return true;
}

//funcoes auxiliares para o metodo visualizarCarteirinha
static void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask) {
    cimg_forXY(img, x, y) {
        if (mask(x, y, 0) > 0 || mask(x, y, 1) > 0 || mask(x, y, 2) > 0) {
            img(x, y, 0) = 0; // vermelho
            img(x, y, 1) = 0; // verde
            img(x, y, 2) = 0; // azul
        }
    }
}

static std::string deixar_maiusculo(std::string &palavra){
    std::string resultado;

    for(char c: palavra)
        resultado += toupper(c);

    return resultado;
}

void EscreveDevagar(const std::string &texto, int ms){
    for (char c : texto){
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

Estudante::Estudante(const std::string& _nome, const std::string &_cpf, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string& _matricula, const std::string& _curso) : Usuario(_nome,_cpf,_data_de_nascimento, _email, _senha), matricula(_matricula), curso(_curso), emprestimos(){
    this->carteirinha = new Carteirinha();
}

Estudante::~Estudante(){
    for (auto emprestimo : this->emprestimos){
        delete emprestimo;
    }
    delete this->carteirinha;
}

void Estudante::exibirEmprestimos(){
    for (auto emprestimo : this->emprestimos){
        emprestimo->exibirInformacoes();
    }
}
void Estudante::pegarLivro(const Biblioteca& biblioteca) {
    std::cout << "Digite o nome do livro que deseja pegar emprestado (sem acentos): ";
    Livro* livro_desejado = nullptr;
    
    std::string nome_do_livro;
    std::getline(std::cin, nome_do_livro);

    try {
        for (Livro* livro : biblioteca.getAcervo()) {
            if (caseInsensitiveComp(nome_do_livro, livro->getTitulo())) {
                livro_desejado = livro;
            }
        }
        if (livro_desejado == nullptr) {
            throw std::invalid_argument("O livro escolhido nao foi encontrado");
        }
        if (livro_desejado->getNumExemplaresDisponiveis() == 0) {
            throw std::invalid_argument("Nao ha exemplares disponiveis para o livro selecionado");
        }
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }

    livro_desejado->setNumExemplaresDisponiveis(livro_desejado->getNumExemplaresDisponiveis() - 1);

    std::string dataDeEmprestimo = getDataAtual();

    std::string dataDeDevolucao = addDias(dataDeEmprestimo, this->get_prazoDeDevolucao());

    Emprestimo* novoEmprestimo = new Emprestimo(*this, *livro_desejado, dataDeEmprestimo, dataDeDevolucao);
    this->emprestimos.push_back(novoEmprestimo);
}
void Estudante::devolverLivro(const Biblioteca& biblioteca){
    if (this->get_emprestimos().size() == 0) {
        std::cout << "Nao ha emprestimos pendentes\n";
        return;
    }
    
    for (auto emprestimo : this->get_emprestimos()) {
        emprestimo->exibirInformacoes();
    }
    
    std::cout << "Escolha qual livro deseja devolver: \n";
    
    std::string titulo_livro_devolvido;
    Emprestimo* livro_devolvido = nullptr;
    std::getline(std::cin, titulo_livro_devolvido);

    try {
        for (auto emprestimo : this->get_emprestimos()) {
            if (caseInsensitiveComp(emprestimo->getLivro()->getTitulo(), titulo_livro_devolvido)) {
                livro_devolvido = emprestimo;
            }
        }
        if (livro_devolvido == nullptr) {
            throw std::invalid_argument("O livro escolhido nao esta na sua lista de emprestimos, confira a ortografia (nao use acentos)");
        }
    } catch (std::invalid_argument &e) {
        std::cerr << e.what() << std::endl;
    }
    
    if (livro_devolvido->calculaValorMulta() > 0.0) {
        std::cout << "O livro esta atrasado por " << livro_devolvido->getDiasDeAtraso() << 
        " dias.\nA multa pelo atraso e de R$" << livro_devolvido->calculaValorMulta() << "\n";

        if (this->get_carteirinha()->getSaldo() >= livro_devolvido->calculaValorMulta()) {
            char resposta;
            while(1) {
                std::cout << "Deseja descontar a multa do saldo da carteirinha? (RS" << this->get_carteirinha()->getSaldo() << ") S/N: ";
                
                try {
                    std::cin >> resposta;

                    if (resposta != 'S' && resposta != 'N' &&
                        resposta != 's' && resposta != 'n') 
                        throw std::invalid_argument("Responda apenas com S para 'sim' ou N para 'nao'\n");
                    
                    break;
                } catch (std::invalid_argument &e) {
                    std::cerr << e.what() << std::endl;
                }
            }

            if (resposta == 'S' || resposta == 's') {
                this->get_carteirinha()->debitar(livro_devolvido->calculaValorMulta());
            }
            else {
                std::cout << "Aguarde seu boleto esta sendo gerado\n";
                /**
                ...
                ...
                ...
                **/
            }
        }
        else {
            std::cout << "Deseja realizar uma recarga? (Valor necessário para o RU: " 
            << (livro_devolvido->calculaValorMulta() - this->get_carteirinha()->getSaldo())
            << ")" << std::endl;

            while(1) {
                std::cout << "0 - Não desejo recarregar minha carteirinha (Voltar para o Menu)\n";
                std::cout << "1 - Sim, desejo recarregar minha carteirinha\n";
                int opcao;

                try {
                    if (!(std::cin >> opcao)) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        throw std::invalid_argument("Digite um número válido!");
                    }
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (opcao == 0) {
                        return;
                    }
                    else if (opcao == 1) {
                        this->recarregarCarteirinha();
                    } 
                    else 
                        throw std::invalid_argument("Digite uma opção válida!");
                } catch (std::invalid_argument &e) {
                    std::cerr << e.what() << std::endl;
                }
            }
        }
    }

    livro_devolvido->setDevolvido(true);
    livro_devolvido->getLivro()->setNumExemplaresDisponiveis(livro_devolvido->getLivro()->getNumExemplaresDisponiveis()+1);
}

void Estudante::recarregarCarteirinha(){
    std::cout << "--------------------------------------------\n";
    this->consultarSaldo();
    std::cout << "--------------------------------------------\n";

    double valor;
    
    while(1) {
        std::cout << "Digite o valor a ser depositado: ";
        std::cin >> valor;

        try {
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("O valor deve ser um número\n");
            }
            if (valor <= 0) {
                throw std::invalid_argument("O valor deve ser maior que 0\n");
            }
            break;
        }
        catch (std::invalid_argument &e) {
            std::cerr << e.what() << std::endl;
        }
    }
    this->carteirinha->depositar(valor);
}

// TODO: (Thales) Implementar a função de gerar carteirinha
// Pede para colocar a foto na pasta images com algum formato
// Verificar se existe e trata os erros
// Gera a carteirinha e coloca em uma pasta separada. Ex: 'Carteirinhas'
// Tenta seguir o padrão das funções quando for pedir algum dado escrito e no UI
void Estudante::visualizarCarteirinha(){
    EscreveDevagar("Antes de visualizar a carteirinha, adicione a imagem do estudante na pasta imagens (PRIMEIRONOMEALUNO_MATRICULA_(formato da imagem))",30);
    std::cout<<std::endl;
    EscreveDevagar("Selecione qual e a extensao do arquivo adicionado:" ,30);
    std::cout<<std::endl;
    EscreveDevagar("1 - .PNG",20);
    std::cout<<std::endl;
    EscreveDevagar("2 - .JPG/JPEG",20);
    std::cout<<std::endl;
    EscreveDevagar("3 - .BMP",20);
    std::cout<<std::endl;

    int opcao;
    
    while (true) {
        try {
            std::cout<<"Opcao: ";

            if(!(std::cin>>opcao)){ //erro cin -- usuario digitou letra,simbolo,etc
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Entrada invalida! Digite um numero 1,2 ou 3.");
            }

            if(opcao < 1 || opcao > 3) {
                throw std::invalid_argument("Opcao invalida! Digite 1, 2 ou 3.");
            }

            break; //caso a entrada e a opcao seja valida
        }
        catch (const std::exception &e) {
            std::cout<<e.what()<<std::endl<<std::endl;
        }
    }

    //carregar as imagens de template base
    CImg<unsigned char> img;
    CImg<unsigned char> barcode;
    
    try {
        img.assign("images/template.bmp");
    }   catch (const cimg_library::CImgIOException &erro){
        throw std::runtime_error("❌ Não foi possível carregar template.bmp");
    }

    try {
        barcode.assign("images/codigo_barra.bmp");
    } catch (const cimg_library::CImgIOException &erro) {
        throw std::runtime_error("❌ Não foi possível carregar barcode.bmp");
    }

    //capturar o primeiro nome do estudante
    std::string aux = getNome();
    auto pos = aux.find(" ");
    std::string primeiroNome;
    if(pos == std::string::npos)
        primeiroNome = aux;
    else
        primeiroNome = aux.substr(0,pos);

    primeiroNome = deixar_maiusculo(primeiroNome);

    //procurar uma foto que é salva no formato "PRIMEIRONOMEALUNO_MATRICULA_foto.bmp" - ex: para Julio Soares dos Reis "JULIO_3213131_foto.bmp"
    std::string nome_foto_aluno = "images/";
    nome_foto_aluno += primeiroNome;
    nome_foto_aluno += "_";
    nome_foto_aluno += get_matricula();
    nome_foto_aluno += "_foto";
    
    if(opcao==1)
        nome_foto_aluno+=".png";
    else if(opcao==2)
        nome_foto_aluno+=".jpg";
    else if (opcao==3)
        nome_foto_aluno+=".bmp";

    CImg<unsigned char> aluno;

    try {
        aluno.assign(nome_foto_aluno.c_str());  //verificar se existe o arquivo da foto do aluno
    } catch (const cimg_library::CImgIOException &erro) {
        throw std::runtime_error("❌ Não foi possivel carregar o arquivo da foto do aluno: " + nome_foto_aluno);
    }

    //colocar a imagem do aluno e do codigo de barra no local certo
    aluno.resize(350,525);
    img.draw_image(33,314,aluno);

    barcode.resize(998,192);
    img.draw_image(397,749,barcode);

    //criar mascara RGB para por o texto
    CImg <unsigned char> mask (img.width(),img.height(),1,3,0);
    unsigned char branco[] = {255,255,255};

    //desenha o texto na mascara 
    mask.draw_text(520,321, getNome().c_str(), branco, 0, 255, 30, false); //.c_str() converte string para const char
    mask.draw_text(523,404, Administrador::procurar_curso_por_codigo(get_curso()).c_str(), branco, 0, 255, 30, false);
    mask.draw_text(577,488, get_matricula().c_str(), branco,0,255, 30, false);
    mask.draw_text(475,572, getCpf().c_str(), branco, 0 , 255, 30, false);
    mask.draw_text(564,655, "06/2026", branco, 0, 255, 30, false);
    mask.draw_text(1173,720, getDataAtual().c_str(), branco, 0, 255, 30, false);

    if(!std::filesystem::exists("carteirinhas")) //caso nao haja uma pasta chamada 'carteirinhas' -- segurança extra
        std::filesystem::create_directory("carteirinhas"); //cria a pasta 

    std::string nomeArquivo = "carteirinhas/" + primeiroNome + "_" + get_matricula() + "_CARTEIRINHA.bmp"; //salvar a carteirinha com nome personalizado conforme o primeiro nome  e a matricula do aluno

    aplicarTextoPreto(img,mask);
    img.save(nomeArquivo.c_str());
    EscreveDevagar("Carteirinha criada com sucesso! Verifique a pasta 'Carteirinhas' para visualizar",30);
}

void Estudante::consultarSaldo(){
    std::cout << "Seu saldo é de R$" << this->carteirinha->getSaldo() << "\n";
}


std::string Estudante::get_matricula () const {
    return this->matricula;
}

std::string Estudante::get_curso() const {
    return this->curso;
}


Carteirinha* Estudante::get_carteirinha() const {
    return this ->carteirinha;
}

std::vector<Emprestimo*> Estudante::get_emprestimos() const {
    return this->emprestimos;
}


void Estudante::set_matricula(std::string _matricula) {
    this->matricula = _matricula;
}

void Estudante::set_curso(std::string _curso) {
    this->curso = _curso;
}