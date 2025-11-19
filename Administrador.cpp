#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>
Administrador::Administrador(const std::string& _nome, const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const int &_id):
    id(_id), Usuario(_nome, _data_de_nascimento, _email, _senha){}

Administrador::~Administrador(){}

int Administrador::get_id() const {
    return this->id;
}

void Administrador::set_id(const int &_id){
    this->id = _id;
}

void Administrador::criarLivro(Biblioteca &biblioteca) {
    std::string _titulo, _autor, _tipo;
    int _numExemplares;

    std::cout<<"Titulo: ";
    std::getline(std::cin,_titulo); //usando getline, então pode ser interessante colocar um cin.ignore(), o que acham? (thales)

    std::cout<<"Autor: ";
    std::getline(std::cin, _autor);

    std::cout<<"Tipo: "; //ainda acho que seria melhor gênero (thales) -- se concordare, favor alterar
    std::getline(std::cin, _tipo);

    std::cout<<"Numero de exemplares: ";
    std::cin>>_numExemplares;

    Livro *novo_livro = new Livro(_titulo,_autor,_tipo,_numExemplares); //criamos um objeto do tipo livro
    biblioteca.adicionarLivro(*novo_livro);  //o adicionamos no acervo da biblioteca
}

void Administrador::criarEstudante() { //vou implementar mas o objeto ainda não será alocado em nenhuma lista porque ainda não temos a classe sistema (thales)
    std::cout<<"Cadastrar: "<<std::endl;
    std::cout<<"1 - Aluno Graduacao"<<std::endl;
    std::cout<<"2 - Aluno Pos Graduacao"<<std::endl;

    int opcao; std::cout<<"Opcao: ";std::cin>>opcao;
    if(opcao!=1 && opcao!=2) { //tratamento de erro provisório, quando criarmos as classes de erro, fazemos alterações (thales)
        std::cout<<"Erro: Digite uma opcao valida!"<<std::endl;
        while(opcao!=1 && opcao!=2) {
            std::cin>>opcao;
        }
    }

    std::cin.ignore(); //usamos cin para verificar a opcao do tipo do aluno
    std::cout<<std::endl;
    std::cout<<"=== FAVOR PREENCHER AS INFORMACOES ABAIXO ==="<<std::endl<<std::endl;

    std::string _nome, _data_de_nascimento, _email, _senha, _matricula, _curso;
    std::cout<<"Nome: "; std::getline(std::cin,_nome);
    std::cout<<"Data de nascimento: "; std::getline(std::cin,_data_de_nascimento); //falta tratamento de erro para a formatação de entrada da data
    std::cout<<"Email: "; std::cin>>_email;
    std::cout<<"Senha: "; std::cin>>_senha; //eu acho quer seria legal colocarmos restrições de senha, ex: letra maiscula, numero, caractere especial, etc (thales)
    std::cout<<"Matricula: "; std::cin>>_matricula;
    std::cout<<"Curso: "; std::cin>>_curso;

    if(opcao==1) {
        int _periodoAtual, _valorRU;
        std::cout<<"Periodo atual (apenas números): "; std::cin>>_periodoAtual; //sugiro tratamento de erro para periodo atual <=0 e >=13, também tratamento contra letras ou outros caracteres (thales)
        std::cout<<"Valor a ser pago no Restaurante Universitario: R$"; std::cin>>_valorRU; //tratamento de erro para apenas numeros

        EstudanteGraduacao* _novoEstudante = new EstudanteGraduacao(_nome, _data_de_nascimento, _email, _senha, _matricula, _curso, _periodoAtual, _valorRU);
        //aqui o aluno precisa ser colocado na lista de alunos da classe Sistema
    }

    else if(opcao==2) {
        std::string _tipoPos, _linhaDePesquisa;
        int _valorRU;

        std::cin.ignore(); //a ultima entrada foi cin
        std::cout<<"Tipo Pos Graduacao: "; std::getline(std::cin, _tipoPos); //esse e o abaixo precisam de tratamento contra numeros e caracteres especias
        std::cout<<"Linha de Pesquisa: "; std::getline(std::cin, _linhaDePesquisa);
        std::cout<<"Valor a ser pago no Restaurante Universitario: R$"; std::cin>>_valorRU; //tratamento para apenas numeros

        EstudantePosGraduacao* _novoEstudante = new EstudantePosGraduacao(_nome, _data_de_nascimento, _email, _senha, _matricula, _curso, _valorRU,_tipoPos, _linhaDePesquisa);
        //aqui o aluno deve ser colocado na lista de alunos da classe Sistema
    }
}

void Administrador::listarEstudante(std::vector<Estudante*>estudantes) {

    std::ofstream fout;
    fout.open("Lista Estudantes.txt");
    if(!fout.is_open()){
        throw std::runtime_error("[ERRO] Não foi possível abrir o arquvivo: Lista Estudantes ");
    }

    fout<<"LISTA DE ESTUDANTES CADASTRADOS: "<<std::endl<<std::endl;
    fout << std::left
    << std::setw(40) << "NOME"
    << std::setw(25) << "DATA DE NASCIMENTO"
    << std::setw(12) << "MATRICULA"
    << std::setw(12) << "CURSO"
    << "\n----------------------------------------------------------------------------------------------------------------\n";
    for(auto estudante :estudantes ){
        fout<<std::setw(40)<<estudante->getNome()<<std::setw(25)<<estudante->getDataDeNascimentoFormatada()<<std::setw(12)<<estudante->get_matricula()<<std::setw(12)<<estudante->get_curso()<<std::endl;
    }
    std::cout<<"Arquivo Lista Estudantes criado "<<std::endl;
    fout.close();
    if(fout.is_open()){
        throw std::runtime_error("[ERRO] Não foi possível fechar o arquvivo: Lista Estudantes ");
    }
}

void Administrador::alterarSenhaEstudante(Usuario& _estudante, const std::string &_nova_senha) {
    _estudante.setSenha(_nova_senha);
}

void Administrador::alterarSenhaAdministrador(Usuario &_administrador, const std::string &_nova_senha) {
    _administrador.setSenha(_nova_senha);
}

void Administrador::consultarTransacoes() {

}

void Administrador::consultarEmprestimos(Estudante &_estudante) {
    for(auto emprestimo : _estudante.get_emprestimos())
        std::cout<<emprestimo<<std::endl;
}

void Administrador::recarregarCarteirinha(Estudante &_estudante, double _valor_depositado) {
    _estudante.get_carteirinha()->depositar(_valor_depositado);
}

void Administrador::alterarValorRU(Estudante &_estudante, double _novo_valor) { //falta diferenciar aluno de PosGraduacao e de Graduacao (thales)
}

void Administrador::alterarValorMulta() {
    
}