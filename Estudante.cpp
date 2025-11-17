#include "Estudante.hpp"
#include "Usuario.hpp"
#include <limits>
#include <stdexcept>
#include <iostream>

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


Estudante::Estudante(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string& _matricula, const std::string& _curso) : Usuario(_nome,_data_de_nascimento, _email, _senha), matricula(_matricula), curso(_curso), emprestimos(){
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
void Estudante::pegarLivro(const Biblioteca& biblioteca){
    // Lógica para escolher livro e pegar

    // Livro* livro_escolhido = (livro_escolhido)
    // livro_escolhido->setNumExemplaresDisponíveis(empréstimo_do_livro->livro->getNumExemplaresDisponíveis()-1)
    // dataDeEmpréstimo = (data atual)
    // dataDeDevolução = dataDeEmpréstimo + (valor se Estudante é Graduação ou Pós)
    // Empréstimo* empréstimo = new Empréstimo(*this, *livro, dataDeEmpréstimo, dataDeDevolução)
    // this->empréstimos.push_back(empréstimo)
}
void Estudante::devolverLivro(const Biblioteca& biblioteca){
    if (this->get_emprestimos().size() == 0) {
        std::cout << "Nao ha emprestimos pendentes\n";
        return;
    }
    
    for (auto emprestimo : this->get_emprestimos()) {
        std::cout << emprestimo->getLivro()->getTitulo() << std::endl;
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
        std::cout << "O livro esta atrasado " << livro_devolvido->getDiasDeAtraso() << 
        ".\nA multa pelo atraso e de " << livro_devolvido->calculaValorMulta() << "\n";

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
    }

    livro_devolvido->setDevolvido(true);
    livro_devolvido->getLivro()->setNumExemplaresDisponiveis(livro_devolvido->getLivro()->getNumExemplaresDisponiveis()+1);
}


void Estudante::recarregarCarteirinha(){
    this->consultarSaldo(); // Opcional

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

            // Limite de deposito?

            break;
        }
        catch (std::invalid_argument &e) {
            std::cout << e.what() << std::endl;
        }
    }

    this->carteirinha->depositar(valor);

}

void Estudante::consultarSaldo(){
    std::cout << "Seu saldo é de " << this->carteirinha->getSaldo() << " reais\n";
}

void Estudante::comerRU(){}


//getters (thales)

std::string Estudante::get_matricula () const {
    return this-> matricula;
}

std::string Estudante::get_curso() const {
    return this->   curso;
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

void Estudante::set_carteirinha(Carteirinha *_carteirinha){
    this->carteirinha = _carteirinha;
}