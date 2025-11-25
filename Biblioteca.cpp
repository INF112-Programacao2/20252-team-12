#include "Biblioteca.hpp"

Biblioteca::Biblioteca(const std::string& _nome) : nome(_nome), acervo() {}

Biblioteca::~Biblioteca(){
    acervo.clear();
}

std::string Biblioteca::getNome(){
    return this->nome;
}

std::vector<Livro*> Biblioteca::getAcervo() const {
    return this->acervo;
}

void Biblioteca::setNome(std::string _nome){
    this->nome = _nome;
}

void Biblioteca::adicionarLivro(Livro& _livro){
    acervo.push_back(&_livro);
    // TODO: Adicionar o livro txt também para não perder depois
}

void Biblioteca::listarLivrosDisponiveis(){
    for (auto livro : acervo){
        if (livro->isDisponivel()){
            livro->exibirInformacoes();
        }
    }
}

// TODO: Fazer para listar em um txt
void Biblioteca::listarLivros(){
    for (auto livro : acervo){
        livro->exibirInformacoes();
    }
}