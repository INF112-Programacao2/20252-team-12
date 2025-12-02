#include <string>                   // Para std::string
#include <vector>                   // Para std::vector
#include "CImg.h"                   // Biblioteca CImg para manipulação de imagens

using namespace cimg_library;

/*============================================================
    ⚙️ FUNÇÕES UTILITÁRIAS DE STRING E FORMATAÇÃO
============================================================*/

// Remove espaços em branco no final de uma string
// Parâmetro (referência): string a ser limpa
void limparString(std::string &string);

// Retorna a data atual do sistema no formato DD/MM/YYYY
// Retorno: string com data formatada
std::string getDataAtual();

// Soma ou subtrai dias de uma data no formato DD/MM/YYYY
// Parâmetros: data_str (formato DD/MM/YYYY), dias (inteiro positivo ou negativo)
// Retorno: string com nova data formatada
std::string addDias(const std::string& data_str, int dias);

// Retorna data e hora atuais do sistema formatadas como DD/MM/YYYY HH:MM:SS
// Retorno: string com data e hora completa
std::string obterDataHora();

// Escreve uma mensagem de log no arquivo "actionLog.txt" com timestamp
// Formato: [DD/MM/YYYY HH:MM:SS] mensagem
// Parâmetro: mensagem (texto a ser registrado)
void escreveLog(std::string mensagem);

// Pausa a execução do programa por um número de segundos
// Parâmetro: seg (número de segundos a pausar)
void pausa(int seg);

// Converte toda a string para MAIÚSCULAS (modifica a string recebida por referência)
// Parâmetro (referência): palavra a ser convertida
// Retorno: string em maiúsculas
std::string deixar_maiusculo(std::string &palavra);

// Divide uma string em pedaços usando um delimitador especificado
// Exemplo: split("A/B/C", '/') retorna {"A","B","C"}
// Parâmetros: s (string a dividir), delim (caractere delimitador)
// Retorno: vetor de strings
std::vector<std::string> split(const std::string &s, char delim);

// Escreve um texto letra por letra com atraso entre cada caractere (efeito de digitação)
// Parâmetros: texto (string a exibir), ms (milissegundos de atraso entre caracteres)
void escreveDevagar(const std::string &texto, int ms);

// Converte um time_t para string formatada no padrão DD/MM/YYYY
// Parâmetro: data (time_t a ser convertido)
// Retorno: string formatada com a data
std::string getDataFormatada(time_t& data);

// Converte uma data em formato string (DD/MM/YYYY) para time_t
// Parâmetro (referência): dataStr (string com data a converter)
// Retorno: time_t correspondente à data
// Exceção: std::invalid_argument ou std::runtime_error se conversão falhar
time_t converterStringParaData(std::string& dataStr);

// Aplica efeito de texto preto em uma imagem CImg usando uma máscara
// Funcionamento: pixels onde máscara > 0 viram preto (0,0,0) na imagem
// Parâmetros: img (imagem a modificar), mask (máscara que define onde aplicar preto)
void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask);

// Retorna uma nova string totalmente em MAIÚSCULAS (sem modificar original)
// Parâmetro: str (string a converter)
// Retorno: string em maiúsculas
std::string stringMaiuscula(std::string str);

// Formata uma string para tamanho exato n (corta ou preenche com espaços)
// Funcionamento:
//   - Se string > n: corta e adiciona "..."
//   - Se string < n: preenche com espaços à direita
//   - Se string == n: retorna como está
// Parâmetros: palavra (string a formatar), n (tamanho final desejado)
// Retorno: string formatada com exatamente n caracteres
std::string corta(std::string palavra, int n);

// Limpa o terminal, funcionando em Windows, Linux e macOS
// Windows: executa "cls"
// Linux/macOS: executa "clear"
void apagarTerminal();

/*============================================================
    🧪 FUNÇÕES DE VALIDAÇÃO
============================================================*/

// Valida CPF conforme regras:
//   ✔ Contém somente números
//   ✔ Exatamente 11 dígitos
//   ✔ Não pode ter todos os dígitos iguais
// Nota: valida apenas formato, não calcula dígitos verificadores
// Parâmetro: cpf (string com CPF a validar)
// Retorno: true se válido
// Exceção: std::invalid_argument se CPF for inválido
bool validarCPF(const std::string &cpf);

// Valida email institucional UFV conforme regras:
//   ✔ Contém exatamente 1 "@"
//   ✔ Tem algo antes do "@"
//   ✔ Termina com "@ufv.br"
// Parâmetro: email (string com email a validar)
// Retorno: true se válido
// Exceção: std::invalid_argument se email for inválido
bool validarEMAIL(const std::string &email);

// Valida nome conforme regras:
//   ✔ Mínimo 2 caracteres
//   ✔ Contém apenas letras (com ou sem acento)
//   ✔ Espaços são permitidos
// Parâmetro: nome (string com nome a validar)
// Retorno: true se válido
// Exceção: std::invalid_argument se nome for inválido
bool validarNOME(const std::string &nome);

// Valida senha conforme regras de segurança:
//   ✔ Mínimo 6 caracteres
//   ✔ Pelo menos 1 letra (maiúscula ou minúscula)
//   ✔ Pelo menos 1 número
// Parâmetro: senha (string com senha a validar)
// Retorno: true se válida
// Exceção: std::invalid_argument se senha for fraca
bool validarSENHA(const std::string &senha);

// Valida data conforme regras:
//   ✔ Formato correto: D/M/YYYY ou DD/MM/YYYY
//   ✔ Ano entre 1900 e ano atual
//   ✔ Mês entre 1 e 12
//   ✔ Dia válido para o mês (considerando anos bissextos)
// Nota: reformata saída como DD/MM/YYYY
// Parâmetro (referência): data (string com data a validar, será reformatada)
// Retorno: true se válida
// Exceção: std::invalid_argument se data for inválida
bool validarDATA(std::string &data);

// Valida e formata matrícula conforme regras:
//   ✔ Somente números
//   ✔ Completa com zeros à esquerda até 6 dígitos
//   ✔ Máximo 6 dígitos
// Parâmetro (referência): matricula (será formatada com zeros à esquerda)
// Retorno: true se válida
// Exceção: std::invalid_argument se matrícula for inválida
bool validarMATRICULA(std::string &matricula);

// Valida tipo de pós-graduação conforme regras:
//   ✔ Aceita: MESTRADO
//   ✔ Aceita: DOUTORADO
// Parâmetro: tipopos (string com tipo de pós-graduação a validar)
// Retorno: true se válido
// Exceção: std::invalid_argument se tipo for inválido
bool validarTIPOPOS(const std::string &tipopos);

// Valida modalidade de ingresso na graduação conforme regras:
//   ✔ Aceita: SISU
//   ✔ Aceita: TRANSF (Transferência)
// Parâmetro: modalidade (string com modalidade a validar)
// Retorno: true se válida
// Exceção: std::invalid_argument se modalidade for inválida
bool validarMODALIDADE(const std::string &modalidade);

// Valida e normaliza código de curso conforme regras:
//   ✔ Não vazio
//   ✔ Somente números (código 101–150)
//   ✔ Verifica existência no arquivo "codigo_cursos.txt"
// Nota: lê arquivo de cursos e normaliza input para código padronizado
// Parâmetro (referência): cursoInput (código/nome do curso, será normalizado)
// Retorno: true se válido
// Exceção: std::invalid_argument se curso for inválido
//          std::runtime_error se arquivo de cursos não puder ser aberto
bool validarCURSO(std::string &cursoInput);

// Valida título de livro/trabalho conforme regras:
//   ✔ Não vazio
//   ✔ Não pode começar ou terminar com espaço
//   ✔ Não pode ter dois espaços seguidos
//   ✔ Aceita letras, números, acentuação e pontuação permitida
// Pontuação aceita: . , : ; - ' " ! ? ( )
// Parâmetro: titulo (string com título a validar)
// Retorno: true se válido
// Exceção: std::invalid_argument se título for inválido
bool validarTITULO(const std::string &titulo);
std::string removerACENTO(std::string nome);