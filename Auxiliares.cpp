// ========== INCLUDES ==========
#define cimg_display 0
#include "Auxiliares.hpp"
#include <iostream>                 // Para entrada/saída padrão
#include <algorithm>                // Para algoritmos STL (find_if, etc)
#include <string>                   // Para manipulação de strings
#include <iomanip>                  // Para formatação de saída (setfill, setw)
#include <chrono>                   // Para operações com tempo
#include <cctype>                   // Para funções de caracteres (isalpha, isdigit, toupper)
#include <thread>                   // Para sleep_for e threads
#include <vector>                   // Para std::vector
#include <fstream>                  // Para operações com arquivos
#include <sstream>                  // Para stringstream
#include "CImg.h"                   // Biblioteca CImg para manipulação de imagens

using namespace cimg_library;

/*============================================================
    ⚙️ FUNÇÕES UTILITÁRIAS GERAIS
============================================================*/

// ========== LIMPEZA DE TERMINAL ==========
/*
    Limpa o terminal, funcionando tanto no Windows quanto em Linux/macOS.
    - Windows: usa "cls"
    - Linux/macOS: usa "clear"
*/
void apagarTerminal()
{
#if defined(_WIN32) || defined(_WIN64)
    std::system("cls");
#elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
    std::system("clear");
#endif
}

// ========== ESCRITA COM ATRASO ==========
/*
    Escreve um texto caractere por caractere, com pausa entre eles.
    Cria efeito de "digitação lenta" no console.
    
    Parâmetros:
    - texto: string a ser exibida
    - ms: milissegundos de atraso entre cada caractere
*/
void escreveDevagar(const std::string &texto, int ms)
{
    for (char c : texto)
    {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

// ========== PAUSA DE EXECUÇÃO ==========
/*
    Pausa a execução do programa por "seg" segundos.
    
    Parâmetro:
    - seg: número de segundos a pausar
*/
void pausa(int seg)
{
    std::this_thread::sleep_for(std::chrono::seconds(seg));
}

// ========== MANIPULAÇÃO DE IMAGEM ==========
/*
    Aplica texto preto usando uma máscara em uma imagem CImg.
    
    Funcionamento:
    - Se a máscara tem qualquer valor RGB > 0 naquela posição,
      essa posição vira preto (0,0,0) na imagem final.
    
    Parâmetros:
    - img: imagem a ser modificada
    - mask: máscara que define onde aplicar o preto
*/
void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask)
{
    cimg_forXY(img, x, y)
    {
        if (mask(x, y, 0) > 0 || mask(x, y, 1) > 0 || mask(x, y, 2) > 0)
        {
            img(x, y, 0) = 0;
            img(x, y, 1) = 0;
            img(x, y, 2) = 0;
        }
    }
}

// ========== CORTE E PREENCHIMENTO DE STRING ==========
/*
    Corta/formata uma string para tamanho exato n.
    
    Funcionamento:
    - Se string > n: corta e adiciona "..."
    - Se string < n: preenche com espaços à direita
    - Se string == n: retorna como está
    
    Parâmetros:
    - palavra: string a ser formatada
    - n: tamanho final desejado
    
    Retorno: string formatada com exatamente n caracteres
*/
std::string corta(std::string palavra, int n)
{
    std::string saida = palavra;

    if (saida.size() > (size_t)n)
        saida = saida.substr(0, n - 3) + "...";

    if (saida.size() < (size_t)n)
        saida += std::string(n - saida.size(), ' ');

    return saida;
}

// ========== CONVERTER PARA MAIÚSCULAS ==========
/*
    Converte toda a string para MAIÚSCULAS.
    
    Parâmetro:
    - palavra: string a ser convertida
    
    Retorno: string em maiúsculas
*/
std::string deixar_maiusculo(std::string &palavra)
{
    std::string resultado;

    for (char c : palavra)
        resultado += toupper(c);

    return resultado;
}

// ========== ADICIONAR DIAS A UMA DATA ==========
/*
    Soma "dias" a uma data no formato DD/MM/YYYY.
    
    Parâmetros:
    - data_str: data inicial (formato: DD/MM/YYYY)
    - dias: número de dias a adicionar (pode ser negativo)
    
    Retorno: nova data formatada como DD/MM/YYYY
*/
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

// ========== DIVIDIR STRING POR DELIMITADOR ==========
/*
    Divide uma string pelo delimitador especificado.
    
    Exemplo:
    - split("A/B/C", '/') => {"A","B","C"}
    - split("João,Maria,Pedro", ',') => {"João","Maria","Pedro"}
    
    Parâmetros:
    - s: string a ser dividida
    - delim: caractere delimitador
    
    Retorno: vetor de strings resultante da divisão
*/
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

// ========== CONVERTER TIME_T PARA STRING FORMATADA ==========
/*
    Converte um time_t para string no formato DD/MM/YYYY.
    Compatível com Windows (MSVC), Linux e macOS.
    
    Parâmetro:
    - data: time_t a ser convertido
    
    Retorno: string formatada como DD/MM/YYYY
*/
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

// ========== CONVERTER STRING PARA MAIÚSCULAS ==========
/*
    Retorna string em maiúsculas.
    
    Parâmetro:
    - str: string a ser convertida
    
    Retorno: string em maiúsculas
*/
std::string stringMaiuscula(std::string str)
{
    std::string upper = str;
    for (char &c : upper)
        c = std::toupper(c);
    return upper;
}

// ========== CONVERTER STRING PARA TIME_T ==========
/*
    Converte data em formato "DD/MM/YYYY" para time_t.
    Dispara exceções se formato ou valores estiverem inválidos.
    
    Parâmetro:
    - dataStr: string com data (formato: DD/MM/YYYY ou D/M/YYYY)
    
    Retorno: time_t correspondente à data
    
    Exceções:
    - std::invalid_argument: se formato estiver errado
    - std::runtime_error: se conversão falhar
*/
time_t converterStringParaData(std::string &dataStr)
{
    struct tm tm = {0};

    if (sscanf(dataStr.c_str(), "%d/%d/%d", &tm.tm_mday, &tm.tm_mon, &tm.tm_year) != 3)
        throw std::invalid_argument("Falha ao interpretar a data. Use D/M/YYYY ou DD/MM/YYYY");

    tm.tm_mon -= 1;
    tm.tm_year -= 1900;
    tm.tm_isdst = -1;

    time_t t = mktime(&tm);

    if (t == (time_t)-1)
        throw std::runtime_error("Conversão de data inválida");

    return t;
}

// ========== OBTER DATA ATUAL ==========
/*
    Retorna a data atual do sistema formatada como DD/MM/YYYY.
    
    Retorno: string com data atual
*/
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

// ========== OBTER DATA E HORA ATUAL ==========
/*
    Retorna a data e hora atual do sistema.
    Formato: DD/MM/YYYY HH:MM:SS
    
    Retorno: string com data e hora completa
*/
std::string obterDataHora()
{
    std::time_t t = std::time(nullptr);
    std::tm *now = std::localtime(&t);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M:%S", now);
    return std::string(buffer);
}

// ========== LIMPAR ESPAÇOS NO FINAL ==========
/*
    Remove espaços em branco apenas no final (direita) da string.
    
    Parâmetro (referência):
    - string: string a ser modificada
*/
void limparString(std::string &string)
{
    auto it_reverso = std::find_if(string.rbegin(), string.rend(),
                                   [](int ch)
                                   { return !std::isspace(ch); });

    auto it_normal = it_reverso.base();
    string.erase(it_normal, string.end());
}

// ========== ESCREVER LOG EM ARQUIVO ==========
/*
    Escreve uma mensagem no arquivo de log com timestamp.
    Formato: [DD/MM/YYYY HH:MM:SS] mensagem
    
    Parâmetro:
    - mensagem: texto a ser registrado no log
*/
void escreveLog(std::string mensagem)
{
    std::ofstream fout("actionLog.txt", std::ios::app);

    if (!fout.is_open())
        std::cerr << "Erro ao abrir o arquivo de log." << std::endl;

    fout << "[" << obterDataHora() << "] " << mensagem << std::endl;
    fout.close();
}

/*============================================================
    🧪 FUNÇÕES DE VALIDAÇÃO DE CAMPOS
============================================================*/

// ========== VALIDAR NOME ==========
/*
    Valida se um nome é válido conforme regras:
    ✔ Mínimo 2 caracteres
    ✔ Contém apenas letras (com ou sem acento)
    ✔ Espaços são permitidos
    
    Parâmetro:
    - nome: string com o nome a validar
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se nome for inválido
*/
bool validarNOME(const std::string &nome)
{
    if (nome.size() < 2)
        throw std::invalid_argument("❌ Nome muito curto.");

    for (unsigned char c : nome)
    {
        bool letraNormal = std::isalpha(c);
        bool espaco = (c == ' ');
        bool acentuadoOuUnicode = (c >= 128);

        if (!letraNormal && !espaco && !acentuadoOuUnicode)
            throw std::invalid_argument("❌ Nome contém caracteres inválidos.");
    }

    return true;
}

// ========== VALIDAR EMAIL ==========
/*
    Valida se um email é institucional UFV:
    ✔ Contém exatamente 1 "@"
    ✔ Tem algo antes do "@"
    ✔ Termina com "@ufv.br"
    
    Parâmetro:
    - email: string com o email a validar
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se email for inválido
*/
bool validarEMAIL(const std::string &email)
{
    int arroba = email.find('@');

    if (arroba < 1)
        throw std::invalid_argument("❌ Email deve conter '@' e algo antes dele.");

    if (email.find('@', arroba + 1) != std::string::npos)
        throw std::invalid_argument("❌ Email não pode ter dois '@'.");

    const std::string dominioUFV = "@ufv.br";

    if (email.size() <= dominioUFV.size() ||
        email.substr(email.size() - dominioUFV.size()) != dominioUFV)
        throw std::invalid_argument("❌ Email inválido. Use apenas emails institucionais @ufv.br.");

    return true;
}

// ========== VALIDAR CPF ==========
/*
    Valida se um CPF é válido conforme regras:
    ✔ Contém somente números
    ✔ Exatamente 11 dígitos
    ✔ Não pode ter todos os dígitos iguais
    
    Nota: Esta função valida apenas formato, não calcula dígitos verificadores.
    
    Parâmetro:
    - cpf: string com CPF a validar
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se CPF for inválido
*/
bool validarCPF(const std::string &cpf)
{
    for (char c : cpf)
        if (!isdigit((unsigned char)c))
            throw std::invalid_argument("❌ CPF deve conter somente números.");

    if (cpf.size() != 11)
        throw std::invalid_argument("❌ CPF deve ter exatamente 11 dígitos.");

    bool todosIguais = true;
    for (int i = 1; i < 11; i++)
        if (cpf[i] != cpf[0])
            todosIguais = false;

    if (todosIguais)
        throw std::invalid_argument("❌ CPF inválido — todos os dígitos são iguais.");

    return true;
}

// ========== VALIDAR SENHA ==========
/*
    Valida se uma senha é forte conforme regras:
    ✔ Mínimo 6 caracteres
    ✔ Pelo menos 1 letra (maiúscula ou minúscula)
    ✔ Pelo menos 1 número
    
    Parâmetro:
    - senha: string com a senha a validar
    
    Retorno: true se válida
    
    Exceção:
    - std::invalid_argument: se senha for fraca
*/
bool validarSENHA(const std::string &senha)
{
    if (senha.size() < 6)
        throw std::invalid_argument("❌ a senha deve ter pelo menos 6 caracteres.");

    bool letra = false, numero = false;

    for (char c : senha)
    {
        if (isalpha((unsigned char)c)) letra = true;
        if (isdigit((unsigned char)c)) numero = true;
    }

    if (!letra)
        throw std::invalid_argument("❌ a senha deve conter pelo menos uma letra.");

    if (!numero)
        throw std::invalid_argument("❌ a senha deve conter pelo menos um número.");

    return true;
}

// ========== VALIDAR DATA ==========
/*
    Valida se uma data é válida conforme regras:
    ✔ Formato correto: D/M/YYYY ou DD/MM/YYYY
    ✔ Ano entre 1900 e ano atual
    ✔ Mês entre 1 e 12
    ✔ Dia válido para o mês (considerando anos bissextos)
    ✔ Formata saída como DD/MM/YYYY
    
    Parâmetro (referência):
    - data: string com data a validar (será reformatada)
    
    Retorno: true se válida
    
    Exceção:
    - std::invalid_argument: se data for inválida
*/
bool validarDATA(std::string &data)
{
    auto parts = split(data, '/');

    if (parts.size() != 3)
        throw std::invalid_argument("❌ Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");

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
    int ano_atual = localtime(&agora)->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual)
        throw std::invalid_argument("❌ Ano fora do intervalo válido (1900 - ano atual)");

    if (mes < 1 || mes > 12)
        throw std::invalid_argument("❌ Mês inválido (deve ser entre 1 e 12)");

    int diasPorMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0))
        diasPorMes[1] = 29;

    if (dia < 1 || dia > diasPorMes[mes - 1])
        throw std::invalid_argument("❌ Dia inválido para o mês especificado");

    std::stringstream ss;

    ss << std::setfill('0') << std::setw(2) << dia << "/" << std::setfill('0') << std::setw(2) << mes << "/" << ano;

    data = ss.str();

    return true;
}

// ========== VALIDAR MATRÍCULA ==========
/*
    Valida e formata uma matrícula conforme regras:
    ✔ Somente números
    ✔ Completa com zeros à esquerda até 6 dígitos
    ✔ Máximo 6 dígitos
    
    Parâmetro (referência):
    - matricula: string com matrícula (será formatada)
    
    Retorno: true se válida
    
    Exceção:
    - std::invalid_argument: se matrícula for inválida
*/
bool validarMATRICULA(std::string &matricula)
{
    // preencher com zeros a esquerda até inteirar os 6 digitos
    if (matricula.size() < 6)
        matricula = std::string(6 - matricula.size(), '0') + matricula; // adiciona na frente da matricula

    // caso a matricula supere o limite para matriculas
    if (matricula.size() > 6)
        throw std::invalid_argument("❌ Matrícula deve ter no máximo 6 dígitos.");
    }

    // Verifica se todos são números
    for (char c : matricula)
        if (!isdigit((unsigned char)c))
            throw std::invalid_argument("❌ Matrícula deve conter APENAS números.");

    return true;
}

// ========== VALIDAR CURSO ==========
/*
    Valida e normaliza um código de curso conforme regras:
    ✔ Não vazio
    ✔ Somente números (código 101–150)
    ✔ Verifica existência no arquivo "codigo_cursos.txt"
    ✔ Retorna o código padronizado
    
    Parâmetro (referência):
    - cursoInput: string com código/nome do curso (será normalizado)
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se curso for inválido
    - std::runtime_error: se arquivo de cursos não puder ser aberto
*/
bool validarCURSO(std::string &cursoInput)
{
    if (cursoInput.empty())
        throw std::invalid_argument("❌ O curso não pode estar vazio.");

    for (char c : cursoInput)
    { // contra entrada de caracter alfabetico
        if (!isdigit((unsigned char)c))
            throw std::invalid_argument("❌ O código do curso deve conter APENAS números.");
    }

    int numero_curso = std::stoi(cursoInput);

    if (numero_curso < 101 || numero_curso > 150)
        throw std::invalid_argument("❌ O código informado deve ser válido.");

    std::ifstream arquivo("codigo_cursos.txt"); //
    if (!arquivo.is_open())
        throw std::runtime_error("❌ Erro crítico: Não foi possível abrir banco de dados de cursos.");

    std::string linha;
    bool header = true;

    while (std::getline(arquivo, linha))
    {
        if (header) { header = false; continue; }

        if (linha.empty()) continue;

        size_t posEspaco = linha.find(' ');
        if (posEspaco == std::string::npos) continue;

        std::string codigo = linha.substr(0, posEspaco);
        std::string nome = linha.substr(posEspaco + 1);

        limparString(nome);

        if (stringMaiuscula(cursoInput) == codigo ||
            stringMaiuscula(cursoInput) == stringMaiuscula(nome))
        {
            cursoInput = codigo;
            break;
        }
    }

    return true;
}

// ========== VALIDAR MODALIDADE DE INGRESSO ==========
/*
    Valida modalidade de ingresso na graduação:
    ✔ Aceita: SISU
    ✔ Aceita: TRANSF
    
    Parâmetro:
    - modalidade: string com modalidade a validar
    
    Retorno: true se válida
    
    Exceção:
    - std::invalid_argument: se modalidade for inválida
*/
bool validarMODALIDADE(const std::string &modalidade)
{
    std::string t = stringMaiuscula(modalidade);

    if (t == "SISU" || t == "TRANSF")
        return true;

    throw std::invalid_argument("❌ Tipo de ingresso inválido. Use SISU ou TRANSF.");
}

// ========== VALIDAR TIPO DE PÓS-GRADUAÇÃO ==========
/*
    Valida tipo de pós-graduação:
    ✔ Aceita: MESTRADO
    ✔ Aceita: DOUTORADO
    
    Parâmetro:
    - tipopos: string com tipo de pós-graduação a validar
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se tipo for inválido
*/
bool validarTIPOPOS(const std::string &tipopos)
{
    std::string t = stringMaiuscula(tipopos);

    if (t == "MESTRADO" || t == "DOUTORADO")
        return true;

    throw std::invalid_argument("❌ Tipo de pós-graduação inválido.");
}

// ========== VALIDAR TÍTULO ==========
/*
    Valida título de trabalho/livro conforme regras:
    ✔ Não vazio
    ✔ Não pode começar ou terminar com espaço
    ✔ Não pode ter dois espaços seguidos
    ✔ Aceita letras, números, acentuação e pontuação permitida
    
    Pontuação aceita: . , : ; - ' " ! ? ( )
    
    Parâmetro:
    - titulo: string com título a validar
    
    Retorno: true se válido
    
    Exceção:
    - std::invalid_argument: se título for inválido
*/
bool validarTITULO(const std::string &titulo)
{
    if (titulo.empty())
        throw std::invalid_argument("❌ O título não pode ser vazio.");

    if (titulo.front() == ' ' || titulo.back() == ' ')
        throw std::invalid_argument("❌ O título não pode começar ou terminar com espaço.");

    bool ultimoEspaco = false;

    for (unsigned char c : titulo)
    {
        if (c == ' ')
        {
            if (ultimoEspaco)
                throw std::invalid_argument("❌ O título não pode ter dois espaços seguidos.");
            ultimoEspaco = true;
        }
        else
        {
            bool alfanumerico = std::isalnum(c);
            bool acentuado = (c >= 128);
            bool pontuacao =
                c == '.' || c == ',' || c == ':' ||
                c == ';' || c == '-' || c == '\'' ||
                c == '"' || c == '!' || c == '?' ||
                c == '(' || c == ')';

            if (!alfanumerico && !acentuado && !pontuacao)
                throw std::invalid_argument("❌ Caractere inválido no título.");

            ultimoEspaco = false;
        }
    }

    return true;
}

std::string removerACENTO(std::string texto)
{
    std::string resultado;

    for (size_t i = 0; i < texto.size();)
    {
        unsigned char c = texto[i];

        // ASCII normal
        if (c < 128)
        {
            resultado.push_back(c);
            i++;
        }
        else
        {
            // UTF-8 de 2 bytes
            if (i + 1 < texto.size())
            {
                unsigned char c2 = texto[i + 1];

                std::string seq = texto.substr(i, 2);

                char convertido = '\0';

                // ============================
                //   Tabela de conversões
                // ============================
                if (seq == "á" || seq == "à" || seq == "ã" || seq == "â" || seq == "ä")
                    convertido = 'a';
                else if (seq == "Á" || seq == "À" || seq == "Ã" || seq == "Â" || seq == "Ä")
                    convertido = 'A';

                else if (seq == "é" || seq == "è" || seq == "ê" || seq == "ë")
                    convertido = 'e';
                else if (seq == "É" || seq == "È" || seq == "Ê" || seq == "Ë")
                    convertido = 'E';

                else if (seq == "í" || seq == "ì" || seq == "î" || seq == "ï")
                    convertido = 'i';
                else if (seq == "Í" || seq == "Ì" || seq == "Î" || seq == "Ï")
                    convertido = 'I';

                else if (seq == "ó" || seq == "ò" || seq == "õ" || seq == "ô" || seq == "ö")
                    convertido = 'o';
                else if (seq == "Ó" || seq == "Ò" || seq == "Õ" || seq == "Ô" || seq == "Ö")
                    convertido = 'O';

                else if (seq == "ú" || seq == "ù" || seq == "û" || seq == "ü")
                    convertido = 'u';
                else if (seq == "Ú" || seq == "Ù" || seq == "Û" || seq == "Ü")
                    convertido = 'U';

                else if (seq == "ç")
                    convertido = 'c';
                else if (seq == "Ç")
                    convertido = 'C';

                // ============================

                if (convertido != '\0')
                {
                    resultado.push_back(convertido);
                }
                else
                {
                    // não reconhecido: copia os 2 bytes
                    resultado += seq;
                }

                i += 2;
            }
            else
            {
                // byte inválido no final
                resultado.push_back(c);
                i++;
            }
        }
    }

    return resultado;
}