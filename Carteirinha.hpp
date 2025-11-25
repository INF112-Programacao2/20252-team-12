#ifndef CARTEIRINHA_HPP
#define CARTEIRINHA_HPP
#include "Transacao.hpp"
#include <vector>

class Transacao;
class Estudante; // Trocado para forward declaration pq tinha dependencia circular

class Carteirinha{
    private:
        static int nextId;
        int id;
        double saldo;
        std::vector<Transacao*> extrato;
    public:
        Carteirinha();
        ~Carteirinha();

        double getSaldo();
        std::vector<Transacao*> getExtrato();
        void setSaldo(double _saldo);

        void depositar(double _valor);
        void debitar(double _valor);
        void exibir_extrato();
};

#endif