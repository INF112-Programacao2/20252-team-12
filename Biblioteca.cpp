#include "Biblioteca.hpp"
#include <fstream>                  // Para operações com arquivos (std::ofstream)
#include <iomanip>                  // Para formatação de saída (std::setw, std::left)
#include <string>                   // Para std::string
#include <iostream>                 // Para entrada/saída padrão
#include <limits>                   // Para std::numeric_limits
#include "Auxiliares.hpp"           // Funções utilitárias (stringMaiuscula, corta, etc)

// ========== CONSTRUTOR ==========
// Inicializa a biblioteca com um nome e um acervo vazio
// Parâmetros:
//   _nome: nome da biblioteca (ex: "Biblioteca UFV")
Biblioteca::Biblioteca(const std::string& _nome) : nome(_nome), acervo() {}

// ========== DESTRUTOR ==========
// Limpa o vetor de ponteiros para livros
// Nota: não deleta os ponteiros pois os Livro* apontam para objetos gerenciados em outro lugar
Biblioteca::~Biblioteca(){
    acervo.clear();
}

// ========== GETTER: NOME DA BIBLIOTECA ==========
// Retorna o nome da biblioteca
// Retorno: string com o nome
std::string Biblioteca::getNome(){
    return this->nome;
}

// ========== GETTER: ACERVO ==========
// Retorna o acervo completo (vetor de ponteiros para Livro)
// Retorno: vetor constante de ponteiros para Livro
std::vector<Livro*> Biblioteca::getAcervo() const {
    return this->acervo;
}

// ========== SETTER: NOME DA BIBLIOTECA ==========
// Atualiza o nome da biblioteca
// Parâmetro:
//   _nome: novo nome para a biblioteca
void Biblioteca::setNome(std::string _nome){
    this->nome = _nome;
}

// ========== ADICIONAR LIVRO AO ACERVO ==========
// Adiciona um livro já existente ao acervo (recebe referência e guarda ponteiro)
// Parâmetro:
//   _livro: referência para um Livro já alocado
void Biblioteca::adicionarLivro(Livro& _livro){
    acervo.push_back(&_livro);
}

// ========== CRIAR LIVRO (SALVAR E ADICIONAR) ==========
// Cria um livro no arquivo de "banco de dados" e adiciona ao acervo
// Funcionamento:
//   1. Abre arquivo "livros.txt" em modo append (adiciona no final)
//   2. Salva dados do livro em formato CSV (Titulo,Autor,Tipo,NumExemplares)
//   3. Fecha arquivo
//   4. Adiciona livro ao vetor acervo
// Parâmetro:
//   _livro: referência para o Livro a ser salvo
void Biblioteca::criarLivro(Livro& _livro) {

    std::ofstream arquivo("livros.txt", std::ios::app);

    // Abre o arquivo e salva os dados em formato CSV
    if (arquivo.is_open()) {
        arquivo << "\n"
                << _livro.getTitulo() << ","
                << _livro.getAutor() << ","
                << _livro.getTipo() << ","
                << _livro.getNumExemplaresTotal();
        
        arquivo.close();
    } else {
        std::cerr << "❌ Não foi possível salvar o livro no banco de dados.\n";
    }

    // Depois de salvar no arquivo, adiciona ao vetor do acervo
    this->adicionarLivro(_livro);
}

// ========== LISTAR LIVROS DISPONÍVEIS ==========
// Lista apenas os livros que possuem exemplares disponíveis
// Iteração simples sem paginação (apenas exibe livros em estoque)
void Biblioteca::listarLivrosDisponiveis(){
    for (auto livro : acervo){
        if (livro->isDisponivel()){
            livro->exibirInformacoes();
        }
    }
}

// ========== LISTAR TODOS OS LIVROS (COM FILTRO E PAGINAÇÃO) ==========
// Lista todos os livros do acervo com:
//   ✔ Filtro por título, autor ou área
//   ✔ Paginação (10 livros por página)
//   ✔ Navegação entre páginas
//   ✔ Status de disponibilidade
void Biblioteca::listarLivros() const{

    // --- VALIDAÇÃO: ACERVO VAZIO ---
    // Se o acervo estiver vazio, avisa e interrompe
    if (acervo.empty()) {
        std::cout << "\n❌ Nenhum livro cadastrado no acervo.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return;
    }

    // --- ENTRADA DO FILTRO ---
    // Solicita ao usuário um termo de busca (título, autor ou área)
    std::string filtro;
    std::cout << "\n=================== 🔍 CONSULTA AO ACERVO 🔍 ===================\n";
    std::cout << "Digite um termo para filtrar. Ex: Titulo, Autor ou Area\n";
    std::cout << "Ou pressione [ENTER] para ver todos os livros: ";
    std::getline(std::cin, filtro);

    // --- APLICAÇÃO DO FILTRO ---
    std::vector<Livro*> livrosExibidos;
    std::string filtroUpper = stringMaiuscula(filtro);

    // Sem filtro → mostra todo o acervo
    if (filtro.empty()) {
        livrosExibidos = acervo; 
    }
    else {
        // Com filtro → faz busca por título, autor ou área (case-insensitive)
        for (auto livro : acervo) {
            if (stringMaiuscula(livro->getTitulo()).find(filtroUpper) != std::string::npos ||
                stringMaiuscula(livro->getAutor()).find(filtroUpper)  != std::string::npos ||
                stringMaiuscula(livro->getTipo()).find(filtroUpper)   != std::string::npos) {
                livrosExibidos.push_back(livro);
            }
        }
    }

    // --- VALIDAÇÃO: FILTRO SEM RESULTADOS ---
    // Se nada foi encontrado pelo filtro
    if (livrosExibidos.empty()) {
        std::cout << "\n❌ Nenhum livro encontrado para o termo: \"" << filtro << "\"\n";
        std::cout << "Pressione Enter para voltar...";
        std::cin.get(); 
        return;
    }

    // --- CONFIGURAÇÃO DE PAGINAÇÃO ---
    // Define quantos livros exibir por página e calcula total de páginas
    const int LIVROS_POR_PAGINA = 10;
    int totalLivros = livrosExibidos.size();
    int totalPaginas = (totalLivros + LIVROS_POR_PAGINA - 1) / LIVROS_POR_PAGINA;

    // --- LOOP DE PAGINAÇÃO ---
    // Itera por cada página, exibindo 10 livros por página
    for (int pagina = 0; pagina < totalPaginas; pagina++) {
        apagarTerminal(); // limpa tela a cada página

        // --- CABEÇALHO ---
        std::cout << "\n============ 📚 RESULTADO DA BUSCA (Pagina "
                  << (pagina + 1) << "/" << totalPaginas << ") ============\n";

        if (!filtro.empty())
            std::cout << "Filtro aplicado: \"" << filtro << "\"\n";
        
        // --- HEADER DA TABELA ---
        // Exibe cabeçalhos das colunas com formatação
        std::cout << std::left
                  << std::setw(4)  << "ID"
                  << std::setw(40) << "TITULO"
                  << std::setw(25) << "AUTOR"
                  << std::setw(20) << "AREA"
                  << std::setw(12) << "STATUS"
                  << std::endl;

        std::cout <<"--------------------------------------------------------------------------------------------\n";

        // --- CÁLCULO DO INTERVALO DA PÁGINA ---
        // Define quais livros (índices inicio e fim) serão exibidos nesta página
        int inicio = pagina * LIVROS_POR_PAGINA;
        int fim = std::min(inicio + LIVROS_POR_PAGINA, totalLivros);

        // --- EXIBIÇÃO DOS LIVROS ---
        // Itera pelos livros do intervalo e exibe com formatação
        for (int i = inicio; i < fim; i++) {
            Livro* livro = livrosExibidos[i];
            
            // Determina status de disponibilidade
            std::string status = (livro->getNumExemplaresDisponiveis() > 0) ? "DISPONIVEL" : "ESGOTADO";

            // Exibe linha formatada com dados do livro
            std::cout << std::left
                      << std::setw(4)  << livro->getId()
                      << std::setw(40) << corta(livro->getTitulo(), 38)
                      << std::setw(25) << corta(livro->getAutor(), 23)
                      << std::setw(20) << corta(livro->getTipo(), 18)
                      << std::setw(12) << status
                      << std::endl;
        }

        std::cout << "============================================================================================\n";
        
        // --- NAVEGAÇÃO ENTRE PÁGINAS ---
        // Se não for a última página, permite avançar ou sair
        if (pagina < totalPaginas - 1) {
            std::cout << "\n[Enter] Proxima Pagina  |  [S] Sair: ";
            std::string opcao;
            std::getline(std::cin, opcao); 
            if (opcao == "S" || opcao == "s") break;
        } 
        // Se for a última página, mostra mensagem diferente
        else {
            std::cout << "\n(Fim da lista) Pressione Enter para voltar...";
            std::cin.get(); 
        }
    }
}
