#define cimg_display 0
#include "Auxiliares.hpp"
#include <iostream>
#include <algorithm>
#include <string>
#include <iomanip>
#include <chrono>
#include <cctype>
#include <thread>
#include <vector>
#include <fstream>
#include <sstream>
#include "CImg.h"

using namespace cimg_library;

void apagarTerminal()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    std::system("clear");
#endif
}

void escreveDevagar(const std::string &texto, int ms)
{
    for (char c : texto)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

void pausa(int seg)
{
    std::this_thread::sleep_for(std::chrono::seconds(seg));
}

void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask)
{
    cimg_forXY(img, x, y)
    {
        if (mask(x, y, 0) > 0 || mask(x, y, 1) > 0 || mask(x, y, 2) > 0)
        {
            img(x, y, 0) = 0; // vermelho
            img(x, y, 1) = 0; // verde
            img(x, y, 2) = 0; // azul
        }
    }
}

std::string corta(std::string palavra, int n)
{
    std::string saida = palavra;

    if (saida.size() > (size_t)n)
        saida = saida.substr(0, n - 3) + "...";

    if (saida.size() < (size_t)n)
        saida += std::string(n - saida.size(), ' ');

    return saida;
}

std::string deixar_maiusculo(std::string &palavra)
{
    std::string resultado;

    for (char c : palavra)
        resultado += toupper(c);

    return resultado;
}

std::string addDias(const std::string &data_str, int dias)
{
    int d, m, a;
    char sep1, sep2;

    std::stringstream ss(data_str);
    ss >> d >> sep1 >> m >> sep2 >> a;

    std::tm data_tm = {};
    data_tm.tm_mday = d;
    data_tm.tm_mon = m - 1;
    data_tm.tm_year = a - 1900;

    std::time_t t = std::mktime(&data_tm);

    t += dias * 24 * 60 * 60;

    std::tm *nova_data = std::localtime(&t);

    std::stringstream out;
    out << std::setfill('0') << std::setw(2) << nova_data->tm_mday << "/"
        << std::setw(2) << nova_data->tm_mon + 1 << "/"
        << nova_data->tm_year + 1900;

    return out.str();
}

std::vector<std::string> split(const std::string &s, char delim)
{
    std::vector<std::string> elems;
    size_t start = 0;
    while (true)
    {
        size_t pos = s.find(delim, start);
        if (pos == std::string::npos)
        {
            elems.push_back(s.substr(start));
            break;
        }
        elems.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return elems;
}

std::string getDataFormatada(time_t &data)
{
    char buffer[80];
    std::tm timeinfo;

#if defined(_MSC_VER)
    localtime_s(&timeinfo, &data);
#elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
    localtime_r(&data, &timeinfo);
#else
    std::tm *tmp = std::localtime(&data);
    if (!tmp)
        return std::string();
    timeinfo = *tmp;
#endif

    strftime(buffer, sizeof(buffer), "%d/%m/%Y", &timeinfo);
    return std::string(buffer);
}

std::string stringMaiuscula(std::string str)
{
    std::string upper = str;
    for (char &c : upper)
    {
        c = std::toupper(c);
    }
    return upper;
}

time_t converterStringParaData(std::string &dataStr)
{
    struct tm tm = {0};
    if (sscanf(dataStr.c_str(), "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3)
    {
        throw std::invalid_argument("Falha ao interpretar a data. Use D/M/YYYY ou DD/MM/YYYY");
    }
    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    tm.tm_isdst = -1;
    time_t t = mktime(&tm);
    if (t == (time_t)-1)
    {
        throw std::runtime_error("Conversão de data inválida");
    }
    return t;
}

std::string getDataAtual()
{
    auto agora = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(agora);

    std::tm *data = std::localtime(&tt);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << data->tm_mday << "/"
       << std::setw(2) << data->tm_mon + 1 << "/"
       << data->tm_year + 1900;

    return ss.str();
}

std::string obterDataHora()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    char buffer[100];
    // Formato: Dia/Mes/Ano Hora:Minuto:Segundo
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", now);

    return std::string(buffer);
}

void limparString(std::string &string)
{
    auto it_reverso = std::find_if(string.rbegin(), string.rend(), [](int ch)
                                   { return !std::isspace(ch); });
    auto it_normal = it_reverso.base();
    string.erase(it_normal, string.end());
}

void escreveLog(std::string mensagem)
{
    std::ofstream fout("actionLog.txt", std::ios::app);

    if (!fout.is_open())
    {
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;
    }

    fout << "[" << obterDataHora() << "] " << mensagem << std::endl;

    fout.close();
}

bool validarNOME(const std::string &nome)
{
    if (nome.size() < 2)
    {
        throw std::invalid_argument("❌ Nome muito curto.");
    }

    for (unsigned char c : nome)
    {
        bool letraNormal = std::isalpha(c);
        bool espaco = (c == ' ');
        bool acentuadoOuUnicode = (c >= 128);

        if (!letraNormal && !espaco && !acentuadoOuUnicode)
        {
            throw std::invalid_argument("❌ Nome contém caracteres inválidos.");
        }
    }

    return true;
}

bool validarEMAIL(const std::string &email)
{
    int arroba = email.find('@');

    // precisa ter algo antes do @
    if (arroba < 1)
    {
        throw std::invalid_argument("❌ Email deve conter '@' e algo antes dele.");
    }

    // email só pode ter um @
    if (email.find('@', arroba + 1) != std::string::npos)
    {
        throw std::invalid_argument("❌ Email não pode ter dois '@'.");
    }

    // domínio obrigatório da UFV
    const std::string dominioUFV = "@ufv.br";

    // verificar se termina exatamente com @ufv.br
    if (email.size() <= dominioUFV.size() ||
        email.substr(email.size() - dominioUFV.size()) != dominioUFV)
    {
        throw std::invalid_argument("❌ Email inválido. Use apenas emails institucionais @ufv.br.");
    }

    return true;
}

bool validarCPF(const std::string &cpf)
{
    for (char c : cpf)
    {
        if (!isdigit((unsigned char)c))
        {
            throw std::invalid_argument("❌ CPF deve conter somente números.");
        }
    }

    if ((int)cpf.size() != 11)
    {
        throw std::invalid_argument("❌ CPF deve ter exatamente 11 dígitos.");
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
        throw std::invalid_argument("❌ CPF inválido — todos os dígitos são iguais.");
    }

    return true;
}

bool validarSENHA(const std::string &senha)
{
    if ((int)senha.size() < 6)
    {
        throw std::invalid_argument("❌ a senha deve ter pelo menos 6 caracteres.");
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
        throw std::invalid_argument("❌ a senha deve conter pelo menos uma letra.");
    }

    if (!numero)
    {
        throw std::invalid_argument("❌ a senha deve conter pelo menos um número.");
    }

    return true;
}

bool validarDATA(std::string &data)
{
    auto parts = split(data, '/'); 

    if (parts.size() != 3)
    {
        throw std::invalid_argument("❌ Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try
    {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    }
    catch (...)
    {
        throw std::invalid_argument("❌ Data contém caracteres inválidos (não são números inteiros)");
    }
    
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual)
    {
        throw std::invalid_argument("❌ Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12)
    {
        throw std::invalid_argument("❌ Mês inválido (deve ser entre 1 e 12)");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0))
    {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1])
    {
        throw std::invalid_argument("❌ Dia inválido para o mês especificado");
    }

    std::stringstream ss;
    
    ss << std::setfill('0') << std::setw(2) << dia << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano;

    data = ss.str(); 

    return true;
}

bool validarMATRICULA(std::string &matricula)
{
    //preencher com zeros a esquerda até inteirar os 6 digitos
    if(matricula.size() < 6)
    matricula = std::string(6-matricula.size(), '0') + matricula; //adiciona na frente da matricula 
    
    //caso a matricula supere o limite para matriculas
    if (matricula.size() > 6)
    {
        throw std::invalid_argument("❌ Matrícula deve ter no máximo 6 dígitos.");
    }
    
    // Verifica se todos são números
    for (char c : matricula)
    {
        if (!isdigit((unsigned char)c))
        {
            throw std::invalid_argument("❌ Matrícula deve conter APENAS números.");
        }
    }

    return true;
}

bool validarCURSO(std::string &cursoInput)
{
    if (cursoInput.empty())
    {
        throw std::invalid_argument("❌ O curso não pode estar vazio.");
    }

    for(char c: cursoInput){ //contra entrada de caracter alfabetico
        if(!isdigit((unsigned char) c))
            throw std::invalid_argument("❌ O código do curso deve conter APENAS números.");
    }
        
    int numero_curso = std::stoi(cursoInput);

    if(numero_curso < 101 || numero_curso > 150)
        throw std::invalid_argument("❌ O código informado deve ser válido.");
    
    std::ifstream arquivo("codigo_cursos.txt"); //
    if (!arquivo.is_open())
    {
        throw std::runtime_error("❌ Erro crítico: Não foi possível abrir o banco de dados de cursos (codigo_cursos.txt).");
    }

    std::string linha;
    std::string inputUpper = stringMaiuscula(cursoInput);
    bool header = true;

    while (std::getline(arquivo, linha))
    {
        if (header)
        {
            header = false;
            continue;
        }

        if (linha.empty())
            continue;

        size_t posEspaco = linha.find(' ');
        if (posEspaco == std::string::npos)
            continue;

        std::string codigoArquivo = linha.substr(0, posEspaco);
        std::string nomeArquivo = linha.substr(posEspaco + 1);

        limparString(nomeArquivo);

        if (inputUpper == codigoArquivo)
        {
            cursoInput = codigoArquivo;
            break;
        }

        if (inputUpper == stringMaiuscula(nomeArquivo))
        {
            cursoInput = codigoArquivo; // Salva o código no lugar do nome
            break;
        }
    }
    return true;
}

bool validarMODALIDADE(const std::string &modalidade)
{
    std::string t = stringMaiuscula(modalidade);

    if (t == "SISU" || t == "TRANSF")
    {
        return true;
    }

    throw std::invalid_argument("❌ Tipo de ingresso inválido. As opções permitidas são: SISU ou TRANSF.");
}
bool validarTIPOPOS(const std::string &tipopos)
{
    std::string t = stringMaiuscula(tipopos);

    if (t == "MESTRADO" || t == "DOUTORADO")
    {
        return true;
    }

    throw std::invalid_argument("❌ Tipo de pós-graduação inválido. As opções permitidas são: Mestrado ou Doutorado.");
}

bool validarTITULO(const std::string &titulo)
{
    if (titulo.empty())
    {
        throw std::invalid_argument("❌ O título não pode ser vazio.");
    }

    if (titulo.front() == ' ' || titulo.back() == ' ')
    {
        throw std::invalid_argument("❌ O título não pode começar ou terminar com espaço.");
    }

    bool ultimoEspaco = false;

    for (unsigned char c : titulo)
    {
        if (c == ' ')
        {
            if (ultimoEspaco)
            {
                throw std::invalid_argument("❌ O título não pode ter dois espaços seguidos.");
            }
            ultimoEspaco = true;
        }
        else
        {
            bool ehAlfanumericoBasico = std::isalnum(c);

            bool ehAcentuadoOuExtendido = (c >= 128);

            bool ehPontuacaoPermitida =
                c == '.' || c == ',' || c == ':' ||
                c == ';' || c == '-' || c == '\'' ||
                c == '"' || c == '!' || c == '?' ||
                c == '(' || c == ')';

            if (!ehAlfanumericoBasico && !ehAcentuadoOuExtendido && !ehPontuacaoPermitida)
            {
                throw std::invalid_argument("❌ Caractere inválido no título.");
            }

            ultimoEspaco = false;
        }
    }

    return true;
}
