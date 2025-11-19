#include "Estudante.hpp"
#include "Usuario.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>
#include <ctime>
#include <sstream>
#include <chrono>
#include <iomanip>

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
        else{
            // Deseja recarregar (livro_devolvido->calculaValorMulta() - this->get_carteirinha()->getSaldo()) para pagar?
        }
    }

    livro_devolvido->setDevolvido(true);
    livro_devolvido->getLivro()->setNumExemplaresDisponiveis(livro_devolvido->getLivro()->getNumExemplaresDisponiveis()+1);
}

void Estudante::recarregarCarteirinha(){
    this->consultarSaldo(); // Opcional, mostrar o saldo antes

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

void Estudante::consultarSaldo(){
    std::cout << "Seu saldo é de R$" << this->carteirinha->getSaldo() << "\n";
}

//getters (thales)

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

//setters (thales)

void Estudante::set_matricula(std::string _matricula) {
    this->matricula = _matricula;
}

void Estudante::set_curso(std::string _curso) {
    this->curso = _curso;
}