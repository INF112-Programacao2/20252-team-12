#include "Emprestimo.hpp"
#include "Estudante.hpp"
#include "Validar.hpp"
#include "Livro.hpp"
#include <ctime>
#include <iostream>
#include <string>

// Atributos estáticos da classe para mudar em todas os objetos
int Emprestimo::nextId = 1;
double Emprestimo::multa = 10.0;            // Pode ser modificado em todos os objetos

Emprestimo::Emprestimo(Estudante& _origem, Livro& _livro, const std::string& _dataDeEmprestimo, const std::string& _dataDeDevolução){
    this->id = nextId++;
    this->origem = &_origem;
    this->livro = &_livro;
    this->devolvido = false;
    if (validarDATA(_dataDeEmprestimo)){
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);
    }
    if (validarDATA(_dataDeDevolução)){
        this->dataDeDevolução = converterStringParaData(_dataDeDevolução);
    }
}

Emprestimo::~Emprestimo(){}

int Emprestimo::getId() {
    return this->id;
}

double Emprestimo::getMulta() {
    return this->multa;
}

bool Emprestimo::isDevolvido(){
    return this->devolvido;
}

Estudante* Emprestimo::getOrigem() {
    return this->origem;
}

Livro* Emprestimo::getLivro() {
    return this->livro;
}

std::string Emprestimo::getDataDeEmprestimo() {
    return getDataFormatada(this->dataDeEmprestimo);
}

std::string Emprestimo::getDataDeDevolução() {
    return getDataFormatada(this->dataDeDevolução);
}
void Emprestimo::setId(int _id) {
    this->id = _id;
}

void Emprestimo::setMulta(double _multa) {
    Emprestimo::multa = _multa;
}

void Emprestimo::setDevolvido(bool _devolvido){
    this->devolvido = _devolvido;
}

void Emprestimo::setOrigem(Estudante& _origem) {
    this->origem = &_origem;
}

void Emprestimo::setLivro(Livro& _livro) {
    this->livro = &_livro;
}

void Emprestimo::setDataDeEmprestimo(std::string _dataDeEmprestimo) {
    if (validarDATA(_dataDeEmprestimo)){
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);
    }
}

void Emprestimo::setDataDeDevolução(std::string _dataDeDevolução) {
    if (validarDATA(_dataDeDevolução)){
        this->dataDeDevolução = converterStringParaData(_dataDeDevolução);
    }
}

double Emprestimo::calculaValorMulta() {
    int diasAtraso = this->getDiasDeAtraso();

    if (diasAtraso <= 0) {
        return 0.0;
    }
    
    return this->multa * diasAtraso;
}

int Emprestimo::getDiasDeAtraso() {
    time_t dataAtual;
    time(&dataAtual);  // Obtém o tempo atual
    
    double diferenca = difftime(dataAtual, dataDeDevolução);
    
    int diasAtraso = static_cast<int>(diferenca / 86400);
    
    return diasAtraso;
}

void Emprestimo::exibirInformacoes(){
    std::cout << "ID: " << this->id << std::endl;
    std::cout << "TITULO: " << this->livro->getTitulo() << std::endl;
    std::cout << "STATUS: " << ((this->devolvido) ? "Devolvido" : "Não Devolvido") << std::endl;
    std::cout << "DATA DE EMPRÉSTIMO: " << getDataFormatada(this->dataDeEmprestimo) << std::endl;
    std::cout << "DATA MÁXIMA DE DEVOLUÇÃO: " << getDataFormatada(this->dataDeDevolução) << std::endl;
}