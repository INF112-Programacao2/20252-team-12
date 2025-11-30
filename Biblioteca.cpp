#include "Biblioteca.hpp"
#include <fstream>
#include <iomanip>
#include <string>

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
    // TODO: Adicionar o livro no txt (inicializador.txt) também para não perder depois
}

void Biblioteca::listarLivrosDisponiveis(){
    for (auto livro : acervo){
        if (livro->isDisponivel()){
            livro->exibirInformacoes();
        }
    }
}

std::string corta(std::string palavra, int n) {
    std::string saida = palavra;

    if(saida.size() > n)
        saida = saida.substr(0, n-3) + "...";
    
    if(saida.size() < n)
        saida += std::string(n-saida.size(), ' ');

    return saida;
}

// TODO: Organizar os livros no txt
void Biblioteca::listarLivros(){

    std::ofstream listaLivros;
    listaLivros.open("ListaLivros.txt");

    if(!listaLivros.is_open())
        throw std::runtime_error("❌ Não foi possível abrir o arquvivo: Lista Livros");

    listaLivros<<"LISTA DE LIVROS CADASTRADOS: "<<std::endl<<std::endl;
    listaLivros
    <<std::left
    <<std::setw(80)<< "TITULO"
    <<std::setw(50)<< "AUTOR"
    <<std::setw(30)<< "TIPO"
    <<std::right
    <<std::setw(10)<< "TOTAL"
    <<std::setw(10)<< "DISPONIVEL"
    << "\n--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n";

    for(auto livro: acervo){
        listaLivros
        <<std::left<<std::setw(80)<<corta(livro->getTitulo(),80)<<
        std::left<<std::setw(50)<<corta(livro->getAutor(),50)<<
        std::left<<std::setw(30)<<corta(livro->getTipo(),30)<<
        std::right<<std::setw(10)<<livro->getNumExemplaresTotal()<<
        std::right<<std::setw(10)<<livro->getNumExemplaresDisponiveis()<<std::endl;
    } 

    listaLivros.close();
    if(listaLivros.is_open())
        throw std::runtime_error("❌ Não foi possível fechar o arquvivo: Lista Livros");
}