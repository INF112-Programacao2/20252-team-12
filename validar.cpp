#include"validar.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <cctype> // Para std::isspace
//tratamentos de erro

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