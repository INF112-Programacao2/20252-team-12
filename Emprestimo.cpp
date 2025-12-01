#include "Emprestimo.hpp"
#include "Estudante.hpp"
#include "Livro.hpp"
#include <ctime>
#include <iostream>
#include <string>

// Pega uma data e retorna uma string formatada
static std::string getDataFormatada(const time_t& data){
    char buffer[80];
    std::tm timeinfo;

    #if defined(_MSC_VER)
        localtime_s(&timeinfo, &data);
    #elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
        localtime_r(&data, &timeinfo);
    #else
        std::tm *tmp = std::localtime(&data);
        if (!tmp) return std::string();
        timeinfo = *tmp;
    #endif

    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
    return std::string(buffer);
}

// Função Auxiliar
static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delim, start);
        if (pos == std::string::npos) {
            elems.push_back(s.substr(start));
            break;
        }
        elems.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return elems;
}

static bool validaData(const std::string& data) {
    // aceita D/M/YYYY, DD/MM/YYYY, com '/' como separador
    auto parts = split(data, '/');
    if (parts.size() != 3) {
        throw std::invalid_argument("Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    } catch (...) {
        throw std::invalid_argument("Data contém caracteres inválidos");
    }
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual) {
        throw std::invalid_argument("Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("Mês inválido");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        throw std::invalid_argument("Dia inválido para o mês especificado");
    }

    return true;
}

static time_t converterStringParaData(const std::string& dataStr) {
    struct tm tm = {0};
    if (sscanf(dataStr.c_str(), "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3) {
        throw std::invalid_argument("Falha ao interpretar a data. Use D/M/YYYY ou DD/MM/YYYY");
    }
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    tm.tm_isdst = -1;
    time_t t = mktime(&tm);
    if (t == (time_t)-1) {
        throw std::runtime_error("Conversão de data inválida");
    }
    return t;
}

// Atributos estáticos da classe para mudar em todas os objetos
int Emprestimo::nextId = 1;
double Emprestimo::multa = 10.0;            // Pode ser modificado em todos os objetos

Emprestimo::Emprestimo(Estudante& _origem, Livro& _livro, const std::string& _dataDeEmprestimo, const std::string& _dataDeDevolucao){
    this->id = nextId++;
    this->origem = &_origem;
    this->livro = &_livro;
    this->devolvido = false;
    if (validaData(_dataDeEmprestimo)){
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);
    }
    if (validaData(_dataDeDevolucao)){
        this->dataDeDevolucao = converterStringParaData(_dataDeDevolucao);
    }
}

Emprestimo::~Emprestimo(){}

int Emprestimo::getId() {
    return this->id;
}

double Emprestimo::getMulta() {
    return this->multa;
}

bool Emprestimo::isDevolvido(){
    return this->devolvido;
}

Estudante* Emprestimo::getOrigem() {
    return this->origem;
}

Livro* Emprestimo::getLivro() {
    return this->livro;
}

std::string Emprestimo::getDataDeEmprestimo() {
    return getDataFormatada(this->dataDeEmprestimo);
}

std::string Emprestimo::getDataDeDevolucao() {
    return getDataFormatada(this->dataDeDevolucao);
}
void Emprestimo::setId(int _id) {
    this->id = _id;
}

void Emprestimo::setMulta(double _multa) {
    Emprestimo::multa = _multa;
}

void Emprestimo::setDevolvido(bool _devolvido){
    this->devolvido = _devolvido;
}

void Emprestimo::setOrigem(Estudante& _origem) {
    this->origem = &_origem;
}

void Emprestimo::setLivro(Livro& _livro) {
    this->livro = &_livro;
}

void Emprestimo::setDataDeEmprestimo(std::string _dataDeEmprestimo) {
    if (validaData(_dataDeEmprestimo)){
        this->dataDeEmprestimo = converterStringParaData(_dataDeEmprestimo);
    }
}

void Emprestimo::setDataDeDevolucao(std::string _dataDeDevolucao) {
    if (validaData(_dataDeDevolucao)){
        this->dataDeDevolucao = converterStringParaData(_dataDeDevolucao);
    }
}

double Emprestimo::calculaValorMulta() {
    int diasAtraso = this->getDiasDeAtraso();

    if (diasAtraso <= 0) {
        return 0.0;
    }
    
    return this->multa * diasAtraso;
}

int Emprestimo::getDiasDeAtraso() {
    time_t dataAtual;
    time(&dataAtual);  // Obtém o tempo atual
    
    double diferenca = difftime(dataAtual, dataDeDevolucao);
    
    int diasAtraso = static_cast<int>(diferenca / 86400);
    
    return diasAtraso;
}

void Emprestimo::exibirInformacoes(){
    std::cout << "ID: " << this->id << std::endl;
    std::cout << "TITULO: " << this->livro->getTitulo() << std::endl;
    std::cout << "STATUS: " << ((this->devolvido) ? "Devolvido" : "Não Devolvido") << std::endl;
    std::cout << "DATA DE EMPRÉSTIMO: " << getDataFormatada(this->dataDeEmprestimo) << std::endl;
<<<<<<< HEAD
    std::cout << "DATA DE MÁXIMA DE DEVOLUÇÃO: " << getDataFormatada(this->dataDeDevolucao) << std::endl;
=======
    std::cout << "DATA MÁXIMA DE DEVOLUÇÃO: " << getDataFormatada(this->dataDeDevolução) << std::endl;
>>>>>>> main
}