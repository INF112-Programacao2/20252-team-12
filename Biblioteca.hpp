#ifndef BIBLIOTECA_HPP
#define BIBLIOTECA_HPP

#include "Livro.hpp"                // Include da classe Livro (necessária para vetor de ponteiros)
#include <string>                   // Para std::string
#include <vector>                   // Para std::vector

// ========== CLASSE BIBLIOTECA ==========
// Gerencia o acervo de livros da biblioteca
// Responsabilidades:
//   - Armazenar coleção de livros (ponteiros)
//   - Criar e adicionar novos livros ao acervo
//   - Listar livros disponíveis e todos os livros com filtro/paginação
//   - Persistir dados em arquivo
class Biblioteca
{
private:
    // ========== ATRIBUTOS PRIVADOS ==========
    std::string nome;               // Nome da biblioteca (ex: "Biblioteca UFV")
    std::vector<Livro *> acervo;    // Vetor dinâmico que armazena ponteiros para todos os livros cadastrados

public:
    // ========== CONSTRUTOR ==========
    // Inicializa a biblioteca com um nome e acervo vazio
    // Parâmetro:
    //   _nome: nome da biblioteca (ex: "Biblioteca Central UFV")
    Biblioteca(const std::string &_nome);

    // ========== DESTRUTOR ==========
    // Limpa o acervo (limpa vetor, sem deletar ponteiros pois Livro* são gerenciados externamente)
    ~Biblioteca();

    // ========== GETTERS ==========
    
    // Retorna o nome da biblioteca
    // Retorno: string com o nome
    std::string getNome();

    // Retorna o vetor completo do acervo (constante)
    // Retorno: vetor constante de ponteiros para Livro
    std::vector<Livro *> getAcervo() const;

    // ========== SETTERS ==========
    
    // Atualiza o nome da biblioteca
    // Parâmetro:
    //   _nome: novo nome para a biblioteca
    void setNome(std::string _nome);

    // ========== OPERAÇÕES COM LIVROS ==========
    
    // Adiciona um livro já existente ao acervo (guarda ponteiro na memória)
    // Nota: apenas referencia o livro, não o copia
    // Parâmetro:
    //   _livro: referência para um Livro já alocado e inicializado
    void adicionarLivro(Livro &_livro);

    // Cria um novo livro, salva seus dados no arquivo "livros.txt" e adiciona ao acervo
    // Funcionamento:
    //   1. Abre/cria arquivo "livros.txt" em modo append (adiciona no final)
    //   2. Escreve dados do livro em formato CSV (Titulo,Autor,Tipo,NumExemplares)
    //   3. Fecha arquivo
    //   4. Adiciona livro ao vetor acervo
    // Parâmetro:
    //   _livro: referência para o Livro a ser criado e salvo
    void criarLivro(Livro &_livro);

    // ========== OPERAÇÕES DE LISTAGEM ==========
    
    // Lista apenas livros que possuem exemplares disponíveis em estoque
    // Exibição simples sem filtro ou paginação
    // Pré-requisito: acervo não vazio
    void listarLivrosDisponiveis();

    // Lista todos os livros do acervo com:
    //   ✔ Filtro por título, autor ou área (case-insensitive)
    //   ✔ Paginação (10 livros por página)
    //   ✔ Navegação entre páginas
    //   ✔ Status de disponibilidade (DISPONIVEL / ESGOTADO)
    // Funcionamento:
    //   1. Solicita termo de filtro ao usuário (ou Enter para ver todos)
    //   2. Filtra livros por título, autor ou área
    //   3. Pagina resultados em grupos de 10
    //   4. Exibe tabela formatada com ID, Título, Autor, Área, Status
    //   5. Permite navegação entre páginas
    void listarLivros() const;
};

#endif
