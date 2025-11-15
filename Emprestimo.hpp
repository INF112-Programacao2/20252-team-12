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
        time_t dataDeEmpréstimo;
        time_t dataDeDevolução;
    public:
        Emprestimo(Estudante& _origem, Livro& _livro, const std::string& _dataDeEmpréstimo, const std::string& _dataDeDevolução);
        ~Emprestimo();

        int getId();
        double getMulta();
        bool isDevolvido();
        Estudante* getOrigem();
        Livro* getLivro();
        std::string getDataDeEmpréstimo();
        std::string getDataDeDevolução();

        void setId(int _id);
        static void setMulta(double _multa);
        void setDevolvido(bool _devolvido);
        void setOrigem(Estudante& _origem);
        void setLivro(Livro& _livro);
        void setDataDeEmpréstimo(std::string _dataDeEmpréstimo);
        void setDataDeDevolução(std::string _dataDeDevolução);

        double calculaValorMulta();
        void exibirInformações();
};

#endif