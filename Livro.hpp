#ifndef LIVRO_HPP
#define LIVRO_HPP

#include <string>                    // Para std::string

// ========== CLASSE LIVRO ==========
// Representa um livro no acervo da biblioteca
// Gerencia informações e disponibilidade de exemplares
class Livro
{
private:
    // ========== ATRIBUTOS ESTÁTICOS ==========
    // Compartilhado entre TODOS os livros
    
    static int nextId;              // Próximo ID disponível para novo livro
                                    // Auto-incrementa a cada novo livro criado

    // ========== ATRIBUTOS PRIVADOS ==========
    
    int id;                         // ID único do livro (auto-incrementado)
    
    std::string titulo;             // Título do livro
    
    std::string autor;              // Nome do autor do livro
    
    std::string tipo;               // Tipo/gênero/área do livro
                                    // Exemplos: "Ficção Científica", "Programação", "História"
                                    // Usado para filtragem e categorização
    
    bool disponivel;                // Flag que indica se há exemplares disponíveis
                                    // true = há exemplares disponíveis para empréstimo
                                    // false = todos os exemplares estão emprestados (esgotado)
    
    int numExemplaresTotal;         // Quantidade total de exemplares da biblioteca
                                    // Nunca muda (quantidade fixa registrada)
    
    int numExemplaresDisponiveis;   // Quantidade de exemplares disponíveis (não emprestados)
                                    // Diminui quando livro é emprestado
                                    // Aumenta quando livro é devolvido

public:
    // ========== CONSTRUTOR ==========
    // Inicializa um novo livro com informações e exemplares
    // Parâmetros:
    //   _titulo: título do livro
    //   _autor: nome do autor
    //   _tipo: tipo/gênero/área (ex: "Ficção Científica", "Técnico")
    //   _numExemplaresTotal: quantidade total de exemplares (quantidade fixa)
    // Efeito colateral: atribui ID único auto-incrementado
    Livro(const std::string &_titulo, const std::string &_autor, const std::string &_tipo, const int &_numExemplaresTotal);

    // ========== DESTRUTOR ==========
    // Libera recursos (se houver alocação dinâmica)
    // Nota: sem alocação dinâmica nesta classe
    ~Livro();

    // ========== GETTERS ==========
    
    // Retorna o ID único do livro
    // Retorno: inteiro com ID
    int getId();

    // Retorna o título do livro
    // Retorno: string com o título
    std::string getTitulo();

    // Retorna o nome do autor do livro
    // Retorno: string com o autor
    std::string getAutor();

    // Retorna o tipo/gênero/área do livro
    // Retorno: string com tipo (ex: "Ficção", "Técnico", "História")
    std::string getTipo();

    // Retorna se o livro possui exemplares disponíveis
    // Funcionamento: atualiza flag de disponibilidade antes de retornar
    // Retorno: true se há exemplares disponíveis para empréstimo
    //          false se todos os exemplares estão emprestados (esgotado)
    bool isDisponivel();

    // Retorna a quantidade TOTAL de exemplares da biblioteca
    // Retorno: inteiro com total de exemplares (quantidade fixa)
    int getNumExemplaresTotal();

    // Retorna a quantidade de exemplares DISPONÍVEIS (não emprestados)
    // Retorno: inteiro com exemplares livres para empréstimo
    int getNumExemplaresDisponiveis();

    // ========== SETTERS ==========
    
    // Define manualmente o ID do livro
    // Nota: usado em casos específicos (carregamento de arquivo, migração de dados)
    // Parâmetro:
    //   _id: novo ID para o livro
    void setId(int _id);

    // Atualiza o título do livro
    // Parâmetro:
    //   _titulo: novo título
    void setTitulo(std::string _titulo);

    // Atualiza o autor do livro
    // Parâmetro:
    //   _autor: novo autor
    void setAutor(std::string _autor);

    // Atualiza o tipo/gênero/área do livro
    // Parâmetro:
    //   _tipo: novo tipo (ex: "Romance", "Técnico")
    void setTipo(std::string _tipo);

    // Atualiza automaticamente a flag de disponibilidade
    // Lógica:
    //   - Se numExemplaresDisponiveis > 0: disponível = true
    //   - Se numExemplaresDisponiveis == 0: disponível = false
    // Nota: chamada automaticamente por isDisponivel()
    void setDisponivel();

    // Atualiza a quantidade total de exemplares
    // Parâmetro:
    //   _numExemplaresTotal: novo total de exemplares
    void setNumExemplaresTotal(int _numExemplaresTotal);

    // Atualiza a quantidade de exemplares disponíveis (não emprestados)
    // Funcionamento:
    //   - Quando um livro é emprestado: valor diminui em 1
    //   - Quando um livro é devolvido: valor aumenta em 1
    // Parâmetro:
    //   _numExemplaresDisponiveis: nova quantidade de exemplares disponíveis
    void setNumExemplaresDisponiveis(int _numExemplaresDisponiveis);

    // ========== OPERAÇÕES DE EXIBIÇÃO ==========
    
    // Exibe as informações completas do livro de forma formatada no console
    // Saída no console:
    //   - ID do livro
    //   - Título e autor
    //   - Tipo/gênero/área
    //   - Quantidade de exemplares disponíveis e total
    void exibirInformacoes();
};

#endif