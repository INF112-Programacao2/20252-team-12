#include"validar.hpp"
#include<iostream>
#include <cctype>
//tratamentos de erro
bool validarNOME(const std::string &nome)
{
    if ((int)nome.size() < 2)
    {
        std::cout << "[ERRO]: nome muito curto.\n";
        return false;
    }

    for (unsigned char c : nome)
    {
        bool letraOuNumero = std::isalpha(c);  // vai pegar as letras normais
        bool espaco = (c == ' ');

        // Qualquer coisa que não seja letra nem espaço é inválida
        if (!letraOuNumero && !espaco)
        {
            std::cout << "[ERRO]: nome contém caracteres inválidos.\n";
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
        std::cout << "[ERRO]: email deve conter '@' e algo antes dele.\n";
        return false;
    }

    if (ponto < arroba + 2)
    {
        std::cout << "[ERRO]: domínio do email inválido.\n";
        return false;
    }

    if (ponto == (int)email.size() - 1)
    {
        std::cout << "[ERRO]: email deve terminar com um domínio válido (.com, .br).\n";
        return false;
    }

    if (email.find('@', arroba + 1) != std::string::npos)
    {
        std::cout << "[ERRO]: email não pode ter dois '@'.\n";
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
            std::cout << "[ERRO]: CPF deve conter somente números.\n";
            return false;
        }
    }

    if ((int)cpf.size() != 11)
    {
        std::cout << "[ERRO]: CPF deve ter exatamente 11 dígitos.\n";
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
        std::cout << "[ERRO]: CPF inválido — todos os dígitos são iguais.\n";
        return false;
    }

    return true;
}

bool validarSENHA(const std::string &senha)
{
    if ((int)senha.size() < 6)
    {
        std::cout << "[ERRO]: a senha deve ter pelo menos 6 caracteres.\n";
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
        std::cout << "[ERRO]: a senha deve conter pelo menos uma letra.\n";
        return false;
    }

    if (!numero)
    {
        std::cout << "[ERRO]: a senha deve conter pelo menos um número.\n";
        return false;
    }

    return true;
}