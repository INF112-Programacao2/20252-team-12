#ifndef EMPRESTIMO_HPP
#define EMPRESTIMO_HPP
#include <ctime>
#include <string>

class Estudante;
class Livro;

class Emprestimo{
    private:
        static int nextId;
        static double multa;
        int id;
        bool devolvido;
        Estudante* origem;
        Livro* livro;
        time_t dataDeEmprestimo;
        time_t dataDeDevolução;
    public:
        Emprestimo(Estudante& _origem, Livro& _livro, const std::string& _dataDeEmprestimo, const std::string& _dataDeDevolução);
        ~Emprestimo();

        int getId();
        double getMulta();
        bool isDevolvido();
        Estudante* getOrigem();
        Livro* getLivro();
        std::string getDataDeEmprestimo();
        std::string getDataDeDevolução();

        void setId(int _id);
        static void setMulta(double _multa);
        void setDevolvido(bool _devolvido);
        void setOrigem(Estudante& _origem);
        void setLivro(Livro& _livro);
        void setDataDeEmprestimo(std::string _dataDeEmprestimo);
        void setDataDeDevolução(std::string _dataDeDevolução);

        double calculaValorMulta();
        void exibirInformações();
};

#endif