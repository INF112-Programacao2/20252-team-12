#ifndef EMPRESTIMO_HPP
#define EMPRESTIMO_HPP
#include <ctime>
#include <string>

class Estudante;
class Livro;

class Emprestimo
{
private:
    static int nextId;
    static double multa;
    int id;
    bool devolvido;
    Estudante *origem;
    Livro *livro;
    time_t dataDeEmprestimo;
    time_t dataDeDevolucao;

public:
    Emprestimo(Estudante &_origem, Livro &_livro, const std::string &_dataDeEmprestimo, const std::string &_dataDeDevolucao);
    ~Emprestimo();

    int getId();
    int getDiasDeAtraso();
    static double getMulta();
    bool isDevolvido();
    Estudante *getOrigem();
    Livro *getLivro();
    std::string getDataDeEmprestimo();
    std::string getDataDeDevolucao();

    void setId(int _id);
    static void setMulta(double _multa);
    void setDevolvido(bool _devolvido);
    void setOrigem(Estudante &_origem);
    void setLivro(Livro &_livro);
    void setDataDeEmprestimo(std::string _dataDeEmprestimo);
    void setDataDeDevolucao(std::string _dataDeDevolucao);

    double calculaValorMulta();
    void exibirInformacoes();
};

#endif