#include "Biblioteca.hpp"
#include <fstream>
#include <iomanip>
#include <string>
#include <iostream>

static void limparTela(){
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #else
        std::system("clear");
    #endif
}

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
    
    std::ofstream arquivo("livros.txt", std::ios::app);

    if (arquivo.is_open()) {
        arquivo << _livro.getTitulo() << ","
                << _livro.getAutor() << ","
                << _livro.getTipo() << ","
                << _livro.getNumExemplaresTotal() << "\n";
        
        arquivo.close();
    } else {
        std::cerr << "Erro: Não foi possível salvar o livro no banco de dados.\n";
    }
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

void Biblioteca::listarLivros(){
    if (acervo.empty()) {
        std::cout << "\n❌ Nenhum livro cadastrado no acervo.\n";
        return;
    }

    const int LIVROS_POR_PAGINA = 10;
    int totalLivros = acervo.size();
    int totalPaginas = (totalLivros + LIVROS_POR_PAGINA - 1) / LIVROS_POR_PAGINA;

    for (int pagina = 0; pagina < totalPaginas; pagina++) {
        limparTela();
        std::cout << "\n================ ACERVO DA BIBLIOTECA (Página " << (pagina + 1) << "/" << totalPaginas << ") ================\n";
        
        // Cabeçalho
        std::cout << std::left 
                  << std::setw(4)  << "ID" 
                  << std::setw(40) << "TITULO"
                  << std::setw(25) << "AUTOR"
                  << std::setw(20) << "AREA"
                  << std::setw(12) << "STATUS"
                  << std::endl;

        std::cout << "--------------------------------------------------------------------------------------------\n";

        int inicio = pagina * LIVROS_POR_PAGINA;
        int fim = std::min(inicio + LIVROS_POR_PAGINA, totalLivros);

        for (int i = inicio; i < fim; i++) {
            Livro* livro = acervo[i];
            
            std::string status = (livro->getNumExemplaresDisponiveis() > 0) ? "DISPONIVEL" : "ESGOTADO";

            std::cout << std::left
                      << std::setw(4)  << (i + 1) // ID Visual
                      << std::setw(40) << corta(livro->getTitulo(), 38)
                      << std::setw(25) << corta(livro->getAutor(), 23)
                      << std::setw(20) << corta(livro->getTipo(), 18)
                      << std::setw(12) << status
                      << std::endl;
        }
        std::cout << "============================================================================================\n";

        if (pagina < totalPaginas - 1) {
            std::cout << "\n[Enter] Próxima Página  |  [S] Sair da listagem: ";
            std::string opcao;
            std::getline(std::cin, opcao);
            
            if (opcao == "S" || opcao == "s") {
                break;
            }
        } else {
            std::cout << "\n(Fim da lista) Pressione Enter para voltar...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
}