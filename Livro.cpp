#include "Livro.hpp"
#include <iostream>                  // Para entrada/saída padrão

// ========== INICIALIZAÇÃO DE ATRIBUTO ESTÁTICO ==========
// Contador global que gera IDs únicos para todos os livros
// Incrementa a cada novo livro criado (começa em 1)
int Livro::nextId = 1;

// ========== CONSTRUTOR ==========
// Inicializa um novo livro com:
//   - ID único e auto-incrementado
//   - Título, autor e tipo (área/gênero)
//   - Total de exemplares
//   - Status inicial como disponível
//   - Exemplares disponíveis = exemplares totais
// Parâmetros:
//   _titulo: título do livro
//   _autor: nome do autor
//   _tipo: gênero/área (ex: "Ficção Científica", "Programação")
//   _numExemplaresTotal: quantidade total de exemplares da biblioteca
Livro::Livro(const std::string &_titulo, const std::string &_autor, const std::string &_tipo, const int &_numExemplaresTotal)
{
    this->id = nextId++;                                    // Atribui ID único e incrementa contador
    this->titulo = _titulo;                                 // Armazena título
    this->autor = _autor;                                   // Armazena autor
    this->tipo = _tipo;                                     // Armazena tipo/gênero
    this->disponivel = true;                                // Começa como disponível
    this->numExemplaresTotal = _numExemplaresTotal;         // Total de exemplares
    this->numExemplaresDisponiveis = _numExemplaresTotal;   // Inicialmente todos disponíveis
}

// ========== DESTRUTOR ==========
// Não realiza limpeza especial (sem alocação dinâmica)
Livro::~Livro() {}

// ========== GETTERS ==========

// Retorna o ID único do livro
// Retorno: inteiro com ID
int Livro::getId()
{
    return this->id;
}

// Retorna o título do livro
// Retorno: string com o título
std::string Livro::getTitulo()
{
    return this->titulo;
}

// Retorna o nome do autor do livro
// Retorno: string com o autor
std::string Livro::getAutor()
{
    return this->autor;
}

// Retorna o tipo/gênero/área do livro
// Retorno: string com o tipo (ex: "Ficção", "Técnico")
std::string Livro::getTipo()
{
    return this->tipo;
}

// Retorna se o livro possui exemplares disponíveis
// Funcionamento: atualiza flag de disponibilidade antes de retornar
// Retorno: true se há exemplares disponíveis, false se esgotado
bool Livro::isDisponivel()
{
    this->setDisponivel();          // Atualiza status baseado em exemplares
    return this->disponivel;        // Retorna flag
}

// Retorna a quantidade total de exemplares da biblioteca
// Retorno: inteiro com total de exemplares
int Livro::getNumExemplaresTotal()
{
    return this->numExemplaresTotal;
}

// Retorna a quantidade de exemplares disponíveis (não emprestados)
// Retorno: inteiro com exemplares disponíveis
int Livro::getNumExemplaresDisponiveis()
{
    return this->numExemplaresDisponiveis;
}

// ========== SETTERS ==========

// Define manualmente o ID do livro (usado em casos específicos como carregamento de arquivo)
// Parâmetro:
//   _id: novo ID para o livro
void Livro::setId(int _id)
{
    this->id = _id;
}

// Atualiza o título do livro
// Parâmetro:
//   _titulo: novo título
void Livro::setTitulo(std::string _titulo)
{
    this->titulo = _titulo;
}

// Atualiza o autor do livro
// Parâmetro:
//   _autor: novo autor
void Livro::setAutor(std::string _autor)
{
    this->autor = _autor;
}

// Atualiza o tipo/gênero/área do livro
// Parâmetro:
//   _tipo: novo tipo
void Livro::setTipo(std::string _tipo)
{
    this->tipo = _tipo;
}

// Atualiza automaticamente a flag de disponibilidade baseado em exemplares disponíveis
// Lógica:
//   - Se numExemplaresDisponiveis > 0: disponível = true
//   - Se numExemplaresDisponiveis == 0: disponível = false
void Livro::setDisponivel()
{
    this->disponivel = (this->numExemplaresDisponiveis != 0);
}

// Atualiza a quantidade total de exemplares
// Parâmetro:
//   _numExemplaresTotal: novo total de exemplares
void Livro::setNumExemplaresTotal(int _numExemplaresTotal)
{
    this->numExemplaresTotal = _numExemplaresTotal;
}

// Atualiza a quantidade de exemplares disponíveis (não emprestados)
// Nota: quando um livro é emprestado, este valor diminui
//       quando um livro é devolvido, este valor aumenta
// Parâmetro:
//   _numExemplaresDisponiveis: nova quantidade de exemplares disponíveis
void Livro::setNumExemplaresDisponiveis(int _numExemplaresDisponiveis)
{
    this->numExemplaresDisponiveis = _numExemplaresDisponiveis;
}

// ========== OPERAÇÕES DE EXIBIÇÃO ==========

// Exibe as informações completas do livro de forma formatada no console
// Saída:
//   - ID do livro
//   - Título e autor
//   - Tipo/gênero
//   - Quantidade de exemplares disponíveis e total
void Livro::exibirInformacoes()
{
    std::cout << this->id << " -> " << this->titulo << " de " << this->autor << " - " << this->tipo << std::endl
              << "Atualmente com: " << this->numExemplaresDisponiveis << " exemplares disponíveis de " 
              << this->numExemplaresTotal << " exemplares totais\n";
}