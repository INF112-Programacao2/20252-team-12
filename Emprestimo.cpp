#include "Emprestimo.hpp"
#include "Estudante.hpp"             // Include de Estudante (necessário para ponteiro origem)
#include "Auxiliares.hpp"            // Funções utilitárias (validarDATA, converterStringParaData, etc)
#include "Livro.hpp"                 // Include de Livro (necessário para ponteiro livro)
#include <ctime>                     // Para time_t, time(), difftime()
#include <iostream>                  // Para entrada/saída padrão
#include <string>                    // Para std::string

// ========== INICIALIZAÇÃO DE ATRIBUTOS ESTÁTICOS ==========

// Atributo estático: gera IDs únicos para todos os empréstimos
// Incrementa a cada novo empréstimo criado (começa em 1)
int Emprestimo::nextId = 1;

// Multa diária aplicada para atrasos de devolução
// Valor compartilhado entre TODOS os empréstimos
// Pode ser modificado pelo administrador (setMulta)
double Emprestimo::multa = 10.0;

// ========== CONSTRUTOR ==========
// Inicializa um novo empréstimo com:
//   - ID único auto-incrementado
//   - Referência ao estudante que pegou o livro
//   - Referência ao livro emprestado
//   - Datas de empréstimo e devolução (convertidas de string para time_t)
//   - Status inicial como "não devolvido" (devolvido = false)
// Parâmetros:
//   _origem: referência ao Estudante responsável pelo empréstimo
//   _livro: referência ao Livro sendo emprestado
//   _dataDeEmprestimo: string com data inicial (DD/MM/YYYY)
//   _dataDeDevolucao: string com data limite de devolução (DD/MM/YYYY)
// Exceção: std::invalid_argument se datas forem inválidas
Emprestimo::Emprestimo(Estudante &_origem, Livro &_livro,
                       std::string &_dataDeEmprestimo, std::string &_dataDeDevolucao)
{
    this->id = nextId++;            // Atribui ID único e incrementa contador
    this->origem = &_origem;        // Armazena ponteiro para o estudante
    this->livro = &_livro;          // Armazena ponteiro para o livro
    this->devolvido = false;        // Empréstimo começa como "não devolvido"

    // --- VALIDAÇÃO E CONVERSÃO DE DATAS ---
    // Converte strings de data (DD/MM/YYYY) para time_t (timestamp Unix)
    if (validarDATA(_dataDeEmprestimo))
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);

    if (validarDATA(_dataDeDevolucao))
        this->dataDeDevolucao = converterStringParaData(_dataDeDevolucao);
}

// ========== DESTRUTOR ==========
// Não realiza limpeza especial pois os ponteiros (origem, livro)
// são gerenciados por outras classes (Sistema, Biblioteca)
Emprestimo::~Emprestimo() {}

// ========== GETTERS ==========

// Retorna o ID único do empréstimo
// Retorno: inteiro com ID
int Emprestimo::getId()
{
    return this->id;
}

// Retorna a multa diária atual (valor compartilhado entre todos)
// Retorno: double com valor em reais (ex: 10.0 = R$ 10,00/dia)
double Emprestimo::getMulta()
{
    return multa;
}

// Retorna se o empréstimo já foi devolvido
// Retorno: true se devolvido, false se ainda pendente
bool Emprestimo::isDevolvido()
{
    return this->devolvido;
}

// Retorna ponteiro para o estudante que pegou o livro
// Retorno: ponteiro para Estudante
Estudante *Emprestimo::getOrigem()
{
    return this->origem;
}

// Retorna ponteiro para o livro emprestado
// Retorno: ponteiro para Livro
Livro *Emprestimo::getLivro()
{
    return this->livro;
}

// Retorna a data de empréstimo formatada como DD/MM/YYYY
// Retorno: string com data formatada
std::string Emprestimo::getDataDeEmprestimo()
{
    return getDataFormatada(this->dataDeEmprestimo);
}

// Retorna a data limite de devolução formatada como DD/MM/YYYY
// Retorno: string com data formatada
std::string Emprestimo::getDataDeDevolucao()
{
    return getDataFormatada(this->dataDeDevolucao);
}

// ========== SETTERS ==========

// Define ID manualmente (usado em casos específicos como carregamento de arquivo)
// Parâmetro:
//   _id: novo ID para o empréstimo
void Emprestimo::setId(int _id)
{
    this->id = _id;
}

// Modifica a multa diária para TODOS os empréstimos (atributo estático)
// Nota: usado pelo administrador para alterar política de multas
// Parâmetro:
//   _multa: novo valor da multa diária em reais
void Emprestimo::setMulta(double _multa)
{
    Emprestimo::multa = _multa;
}

// Marca o empréstimo como devolvido ou não devolvido
// Parâmetro:
//   _devolvido: true para marcar como devolvido, false para não devolvido
void Emprestimo::setDevolvido(bool _devolvido)
{
    this->devolvido = _devolvido;
}

// Atualiza o estudante responsável pelo empréstimo
// Parâmetro:
//   _origem: referência ao novo Estudante
void Emprestimo::setOrigem(Estudante &_origem)
{
    this->origem = &_origem;
}

// Troca o livro associado ao empréstimo
// Parâmetro:
//   _livro: referência ao novo Livro
void Emprestimo::setLivro(Livro &_livro)
{
    this->livro = &_livro;
}

// Atualiza a data de empréstimo, validando o formato antes
// Parâmetro:
//   _dataDeEmprestimo: string com data (DD/MM/YYYY)
// Exceção: std::invalid_argument se data for inválida
void Emprestimo::setDataDeEmprestimo(std::string _dataDeEmprestimo)
{
    if (validarDATA(_dataDeEmprestimo))
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);
}

// Atualiza a data de devolução, validando o formato antes
// Parâmetro:
//   _dataDeDevolucao: string com data (DD/MM/YYYY)
// Exceção: std::invalid_argument se data for inválida
void Emprestimo::setDataDeDevolucao(std::string _dataDeDevolucao)
{
    if (validarDATA(_dataDeDevolucao))
        this->dataDeDevolucao = converterStringParaData(_dataDeDevolucao);
}

// ========== OPERAÇÕES CALCULADAS ==========

// Calcula o valor total da multa por atraso
// Funcionamento:
//   1. Obtém número de dias de atraso (getDiasDeAtraso)
//   2. Se diasAtraso <= 0: sem multa (return 0.0)
//   3. Se diasAtraso > 0: multa = multa_diária × diasAtraso
// Retorno: double com valor em reais (ex: 50.0 = R$ 50,00)
double Emprestimo::calculaValorMulta()
{
    int diasAtraso = this->getDiasDeAtraso();

    // Se não há atraso, multa é zero
    if (diasAtraso <= 0)
        return 0.0;

    // Calcula multiplicando valor da multa diária pelo número de dias atrasado
    return this->multa * diasAtraso;
}

// Retorna quantos dias de atraso existem em relação à data atual
// Funcionamento:
//   1. Obtém data/hora atual do sistema (time_t dataAtual)
//   2. Calcula diferença entre agora e dataDeDevolucao usando difftime()
//   3. Converte segundos para dias (dividindo por 86400)
//   4. Retorna como inteiro
// Retorno: inteiro com dias de atraso (negativo se antecipado, zero/positivo se atrasado)
int Emprestimo::getDiasDeAtraso()
{
    time_t dataAtual;
    time(&dataAtual);               // Obtém timestamp do momento atual

    // difftime retorna diferença em segundos (agora - dataDeDevolucao)
    double diferenca = difftime(dataAtual, dataDeDevolucao);

    // Converte segundos para dias (86400 segundos = 1 dia)
    int diasAtraso = static_cast<int>(diferenca / 86400);

    return diasAtraso;
}

// ========== OPERAÇÕES DE EXIBIÇÃO ==========

// Exibe todas as informações relevantes do empréstimo no console
// Saída formatada incluindo:
//   - ID do empréstimo
//   - Título do livro
//   - Status (Devolvido / Não Devolvido)
//   - Data de empréstimo (DD/MM/YYYY)
//   - Data máxima de devolução (DD/MM/YYYY)
void Emprestimo::exibirInformacoes(){
    std::cout << "ID EMPRÉSTIMO: " << this->id << std::endl;
    std::cout << "TITULO: " << this->livro->getTitulo() << std::endl;
    std::cout << "STATUS: " << ((this->devolvido) ? "Devolvido" : "Não Devolvido") << std::endl;
    std::cout << "DATA DE EMPRÉSTIMO: " << getDataFormatada(this->dataDeEmprestimo) << std::endl;
    std::cout << "DATA DE MÁXIMA DE DEVOLUÇÃO: " << getDataFormatada(this->dataDeDevolucao) << std::endl;
}