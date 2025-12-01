#include"validar.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype> 
#include <vector>

void limparString(std::string &string){
    auto it_reverso = std::find_if(string.rbegin(), string.rend(), [](int ch) {
        return !std::isspace(ch);
    });
    auto it_normal = it_reverso.base();
    string.erase(it_normal, string.end());
}

bool validarNOME(const std::string &nome)
{
    if ((int)nome.size() < 2)
    {
        throw std::invalid_argument("❌ Nome muito curto.\n");
        return false;
    }

    for (unsigned char c : nome)
    {
        bool letraOuNumero = std::isalpha(c);  // vai pegar as letras normais
        bool espaco = (c == ' ');

        // Qualquer coisa que não seja letra nem espaço é inválida
        if (!letraOuNumero && !espaco)
        {
            throw std::invalid_argument("❌ Nome contém caracteres inválidos.\n");
            return false;
        }
    }

    return true;
}

bool validarEMAIL(const std::string &email)
{
    int arroba = email.find('@');
    int ponto = email.rfind('.');

    if (arroba < 1)
    {
        throw std::invalid_argument("❌ Email deve conter '@' e algo antes dele.\n");
        return false;
    }

    if (ponto < arroba + 2)
    {
        throw std::invalid_argument("❌ Domínio do email inválido.\n");
        return false;
    }

    if (ponto == (int)email.size() - 1)
    {
        throw std::invalid_argument("❌ Email deve terminar com um domínio válido (.com, .br).\n");
        return false;
    }

    if (email.find('@', arroba + 1) != std::string::npos)
    {
        throw std::invalid_argument("❌ Email não pode ter dois '@'.\n");
        return false;
    }

    return true;
}

bool validarCPF(const std::string &cpf)
{
    for (char c : cpf)
    {
        if (!isdigit((unsigned char)c))
        {
            throw std::invalid_argument("❌ CPF deve conter somente números.\n");
            return false;
        }
    }

    if ((int)cpf.size() != 11)
    {
        throw std::invalid_argument("❌ CPF deve ter exatamente 11 dígitos.\n");
        return false;
    }

    bool todosIguais = true;
    for (int i = 1; i < 11; i++)
    {
        if (cpf[i] != cpf[0])
        {
            todosIguais = false;
            break;
        }
    }

    if (todosIguais)
    {
        throw std::invalid_argument("❌ CPF inválido — todos os dígitos são iguais.\n");
        return false;
    }

    return true;
}

bool validarSENHA(const std::string &senha)
{
    if ((int)senha.size() < 6)
    {
        throw std::invalid_argument("❌ a senha deve ter pelo menos 6 caracteres.\n");
        return false;
    }

    bool letra = false, numero = false;

    for (char c : senha)
    {
        if (isalpha((unsigned char)c))
            letra = true;
        if (isdigit((unsigned char)c))
            numero = true;
    }

    if (!letra)
    {
        throw std::invalid_argument("❌ a senha deve conter pelo menos uma letra.\n");
        return false;
    }

    if (!numero)
    {
        throw std::invalid_argument("❌ a senha deve conter pelo menos um número.\n");
        return false;
    }

    return true;
}

static std::vector<std::string> split(const std::string &s, char delim) {
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

bool validarDATA(const std::string &data)
{
    auto parts = split(data, '/');
    if (parts.size() != 3)
    {
        throw std::invalid_argument("❌ Formato de data inválido. Use DD/MM/AAAA.");
    }

    int dia, mes, ano;
    try {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    } catch (...) {
        throw std::invalid_argument("❌ Data contém caracteres inválidos.");
    }

    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    int anoAtual = now->tm_year + 1900;

    if (ano < 1900 || ano > anoAtual) {
        throw std::invalid_argument("❌ Ano fora do intervalo válido.");
    }
    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("❌ Mês inválido.");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    // Verifica ano bissexto
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        throw std::invalid_argument("❌ Dia inválido para o mês especificado.");
    }

    return true;
}

bool validarMATRICULA(const std::string &matricula)
{
    if (matricula.empty()) {
        throw std::invalid_argument("❌ A matrícula não pode estar vazia.");
    }
    
    bool apenasNumeros = std::all_of(matricula.begin(), matricula.end(), ::isdigit);
    if (!apenasNumeros) {
        throw std::invalid_argument("❌ A matrícula deve conter APENAS números.");
    }
    
    if (matricula.size() != 6) throw std::invalid_argument("❌ Matrícula deve conter 6 dígitos.");

    return true;
}