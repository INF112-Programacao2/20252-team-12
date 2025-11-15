#ifndef CARTEIRINHA_HPP
#define CARTEIRINHA_HPP
#include "Estudante.hpp"

class Carteirinha{
    private:
        static int nextId;
        int id;
        double saldo;
    public:
        Carteirinha();
        ~Carteirinha();

        double getSaldo();
        void setSaldo(double _saldo);

        void depositar(double _valor);
        void debitar(double _valor);
};

#endif