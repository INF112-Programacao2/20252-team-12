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
std::string getDataFormatada(time_t& data);
time_t converterStringParaData(std::string& dataStr);
void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask);
std::string stringMaiuscula(std::string str);
std::string corta(std::string palavra, int n);
void apagarTerminal();
bool validarCPF(const std::string &cpf);
bool validarEMAIL(const std::string &email);
bool validarNOME(const std::string &nome);
bool validarSENHA(const std::string &senha);
bool validarDATA(std::string &data);
bool validarMATRICULA(const std::string &matricula);
bool validarLINHAPESQUISA(const std::string &linhapesquisa);
bool validarTIPOPOS(const std::string &tipopos);
bool validarMODALIDADE(const std::string &modalidade);
bool validarCURSO(std::string &cursoInput);
bool validarTITULO(const std::string &titulo);
