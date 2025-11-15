#include "Biblioteca.hpp"

Biblioteca::Biblioteca(const std::string& _nome) : nome(_nome), acervo() {}

Biblioteca::~Biblioteca(){
    acervo.clear();
}

std::string Biblioteca::getNome(){
    return this->nome;
}
void Biblioteca::setNome(std::string _nome){
    this->nome = _nome;
}

void Biblioteca::adicionarLivro(Livro& _livro){
    acervo.push_back(&_livro);
}

void Biblioteca::listarLivrosDisponíveis(){
    for (auto livro : acervo){
        if (livro->isDisponivel()){
            livro->exibirInformações();
        }
    }
}

void Biblioteca::listarLivros(){
    for (auto livro : acervo){
        livro->exibirInformações();
    }
}