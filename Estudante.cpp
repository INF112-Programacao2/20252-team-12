#include "Estudante.hpp"
#include "Usuario.hpp"
#include <limits>
#include <iostream>

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
    // Lógica para escolher livro e devolver

    // Empréstimo* empréstimo_do_livro = (Empréstimo)
    // if (empréstimo_do_livro->calculaMulta() > 0.0) -> perguntar se deseja debitar da carteirinha
    // else if (empréstimo_do_livro->calculaMulta() == 0.0)
    // empréstimo_do_livro->setDevolvido(true)
    // empréstimo_do_livro->livro->setNumExemplaresDisponíveis(empréstimo_do_livro->livro->getNumExemplaresDisponíveis()+1)

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