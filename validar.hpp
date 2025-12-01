#include <string>
#include <vector>
#include "CImg.h"

using namespace cimg_library;

void limparString(std::string &string);
std::string getDataAtual();
std::string addDias(const std::string& data_str, int dias);
std::string obterDataHora();
void escreveLog(std::string mensagem);
void pausa(int seg);
std::string deixar_maiusculo(std::string &palavra);
std::vector<std::string> split(const std::string &s, char delim);
void escreveDevagar(const std::string &texto, int ms);
std::string getDataFormatada(const time_t& data);
time_t converterStringParaData(const std::string& dataStr);
void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask);
std::string stringMaiuscula(std::string str);
void apagarTerminal();
bool validarCPF(const std::string &cpf);
bool validarEMAIL(const std::string &email);
bool validarNOME(const std::string &nome);
bool validarSENHA(const std::string &senha);
bool validarDATA(const std::string &data);
