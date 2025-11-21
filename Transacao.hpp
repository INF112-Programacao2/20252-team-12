#ifndef TRANSACAO_HPP
#define TRANSACAO_HPP

#include <ctime>
#include <string>
#include "Estudante.hpp"

class Estudante;

class Transacao {
    private:
        static int nextID;
        int id;
        std::string tipo;
        double valor;
        Estudante* origem;
        std::string data;
    public:
        Transacao(const std::string &_tipo, const double &_valor, Estudante &_origem, const std::string& _data);
        ~Transacao();

        int get_id() const;
        std::string get_tipo_transacao() const;
        double get_valor_transacao() const;
        Estudante* get_origem_transacao() const;
        std::string get_data() const;

        void set_id(const int &_id);
        void set_tipo_transacao(const std::string &_tipo_transacao);
        void set_valor(const double &_valor);
        void set_origem(Estudante &_origem);
        void set_data(std::string &_data);

        void exibir_transacao();
};

#endif
