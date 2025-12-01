#include "Livro.hpp"
#include <iostream>

// Atributo estático da classe para mudar em todas os objetos
int Livro::nextId = 1;

Livro::Livro(const std::string &_titulo, const std::string &_autor, const std::string &_tipo, const int &_numExemplaresTotal)
{
    this->id = nextId++;
    this->titulo = _titulo;
    this->autor = _autor;
    this->tipo = _tipo;
    this->disponivel = true;
    this->numExemplaresTotal = _numExemplaresTotal;
    this->numExemplaresDisponiveis = _numExemplaresTotal;
}

Livro::~Livro() {}

int Livro::getId()
{
    return this->id;
}
std::string Livro::getTitulo()
{
    return this->titulo;
}
std::string Livro::getAutor()
{
    return this->autor;
}
std::string Livro::getTipo()
{
    return this->tipo;
}
bool Livro::isDisponivel()
{
    this->setDisponivel();
    return this->disponivel;
}
int Livro::getNumExemplaresTotal()
{
    return this->numExemplaresTotal;
}
int Livro::getNumExemplaresDisponiveis()
{
    return this->numExemplaresDisponiveis;
}
void Livro::setId(int _id)
{
    this->id = _id;
}
void Livro::setTitulo(std::string _titulo)
{
    this->titulo = _titulo;
}
void Livro::setAutor(std::string _autor)
{
    this->autor = _autor;
}
void Livro::setTipo(std::string _tipo)
{
    this->tipo = _tipo;
}
void Livro::setDisponivel()
{
    this->disponivel = this->numExemplaresDisponiveis != 0;
}
void Livro::setNumExemplaresTotal(int _numExemplaresTotal)
{
    this->numExemplaresTotal = _numExemplaresTotal;
}
void Livro::setNumExemplaresDisponiveis(int _numExemplaresDisponiveis)
{
    this->numExemplaresDisponiveis = _numExemplaresDisponiveis;
}

void Livro::exibirInformacoes()
{
    std::cout << this->id << " -> " << this->titulo << " de " << this->autor << " - " << this->tipo << std::endl
              << "Atualmente com: " << this->numExemplaresDisponiveis << " exemplares disponíveis de " << this->numExemplaresTotal << " exemplares totais\n";
}