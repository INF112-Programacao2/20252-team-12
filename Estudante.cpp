#include "Estudante.hpp"
#include "Usuario.hpp"
#include <iostream>

Estudante::Estudante(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const std::string& _matricula, const std::string& _curso) : Usuario(_nome,_data_de_nascimento, _email, _senha), matricula(_matricula), curso(_curso), empréstimos(){
    this->carteirinha = new Carteirinha();
}

Estudante::~Estudante(){
    for (auto empréstimo : this->empréstimos){
        delete empréstimo;
    }
    delete this->carteirinha;
}

void Estudante::exibirEmpréstimos(){
    for (auto emprestimo : this->empréstimos){
        emprestimo->exibirInformações();
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
    // Lógica para recarregar Carteirinha


    // this->carteirinha.depositar(valor)

}

void Estudante::consultarSaldo(){
    std::cout << "Seu saldo é de " << this->carteirinha->getSaldo() << " reais\n";
}

void Estudante::comerRU(){}