#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>

int Administrador::nextID = 1;
Administrador::Administrador(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha, const int &_id):
    id(nextID++), Usuario(_nome, _cpf,_data_de_nascimento, _email, _senha){}

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
    std::getline(std::cin,_titulo); 

    std::cout<<"Autor: ";
    std::getline(std::cin, _autor);

    std::cout<<"Tipo: "; //ainda acho que seria melhor gênero (thales) -- se concordare, favor alterar
    std::getline(std::cin, _tipo);

    std::cout<<"Numero de exemplares: ";
    std::cin>>_numExemplares;

    Livro *novo_livro = new Livro(_titulo,_autor,_tipo,_numExemplares); //criamos um objeto do tipo livro
    biblioteca.adicionarLivro(*novo_livro);  //o adicionamos no acervo da biblioteca
}

void Administrador::criarEstudante(std::vector<Estudante*> &estudantes) { 
    std::cout<<"Cadastrar: "<<std::endl;
    std::cout<<"1 - Aluno Graduacao"<<std::endl;
    std::cout<<"2 - Aluno Pos Graduacao"<<std::endl;

    int opcao; std::cout<<"Opcao: ";std::cin>>opcao;
    if(opcao!=1 && opcao!=2) { 
        std::cout<<"Erro: Digite uma opcao valida!"<<std::endl;
        while(opcao!=1 && opcao!=2) {
            std::cin>>opcao;
        }
    }

    std::cin.ignore(); //usamos cin para verificar a opcao do tipo do aluno
    std::cout<<std::endl;
    std::cout<<"=== FAVOR PREENCHER AS INFORMACOES ABAIXO ==="<<std::endl<<std::endl;

    std::string _nome, _cpf,_data_de_nascimento, _email, _senha, _matricula, _curso;
    std::cout<<"Nome: "; std::getline(std::cin,_nome);
    std::cout<<"CPF: "; std::getline(std::cin, _cpf);
    std::cout<<"Data de nascimento: "; std::getline(std::cin,_data_de_nascimento); //falta tratamento de erro para a formatação de entrada da data
    std::cout<<"Email: "; std::cin>>_email;
    std::cout<<"Senha: "; std::cin>>_senha; 
    std::cout<<"Matricula: "; std::cin>>_matricula;
    std::cout<<"Curso: "; std::cin>>_curso;

    if(opcao==1) {
        std::string _modalidade;
        std::cout<<"Modalidade de Graduação"; std::cin>> _modalidade; 
        

        EstudanteGraduacao* _novoEstudante = new EstudanteGraduacao(_nome, _cpf,_data_de_nascimento, _email, _senha, _matricula, _curso, _modalidade);
        estudantes.push_back(_novoEstudante);
    }

    else if(opcao==2) {
        std::string _tipoPos, _linhaDePesquisa;

        std::cin.ignore(); //a ultima entrada foi cin
        std::cout<<"Tipo Pos Graduacao: "; std::getline(std::cin, _tipoPos); //esse e o abaixo precisam de tratamento contra numeros e caracteres especias
        std::cout<<"Linha de Pesquisa: "; std::getline(std::cin, _linhaDePesquisa);

        EstudantePosGraduacao* _novoEstudante = new EstudantePosGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _tipoPos, _linhaDePesquisa);
        estudantes.push_back(_novoEstudante);
    }
}

void Administrador::listarEstudante(std::vector<Estudante*> &estudantes) {

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
    std::cout<<"Arquivo Lista Estudantes criado"<<std::endl;
    fout.close();
    if(fout.is_open()){
        throw std::runtime_error("[ERRO] Não foi possível fechar o arquvivo: Lista Estudantes ");
    }
}

void Administrador::alterarSenhaEstudante(std::vector<Estudante*> &estudantes) {
    std::string matricula;
    std::cout << "Digite a matrícula do estudante: ";
    std::cin >> matricula;

    for (auto estudante : estudantes){
        if (estudante->get_matricula() == matricula){
            std::string nova_senha;
            std::cout << "Digite a nova senha de " << estudante->getNome() << ": ";
            std::cin >> nova_senha;
            estudante->setSenha(nova_senha);
            std::cout << "Senha alterada com sucesso!" << std::endl;
            return;
        }
    }
    throw std::invalid_argument("[ERRO] Não foi possível localizar o Estudante com matrícula " + matricula);

}

void Administrador::alterarSenhaAdministrador() {
    std::string nova_senha;
    std::cout << "Digite sua nova senha: ";
    std::cin >> nova_senha;
    this->setSenha(nova_senha);
    std::cout << "Senha alterada com sucesso!" << std::endl;
}

void Administrador::consultarTransacoes(std::vector<Estudante*> &estudantes) {
    // Perguntar se quer ver todas ou so algumas
    // for (auto estudante : estudantes){ for (auto transacao : estudante.get_carteirinha().get_extrato()) }
}

void Administrador::consultarEmprestimos(std::vector<Estudante*> &estudantes) {
    // Perguntar se quer ver todos ou so alguns
    // for (auto estudante : estudantes){ estudante.exibirEmprestimos() }
}

void Administrador::recarregarCarteirinha(std::vector<Estudante*> &estudantes) {
    // Perguntar a matricula do aluno
    // for (auto estudante : estudantes){ if (estudante.get_matricula() == matricula) { estudante.recarregarCarteirinha() } }
}

void Administrador::alterarValorRU() {
    // Perguntar se é de Graduação ou de PósGraduação
    // if (Graduação){ EstudanteGraduacao::valorRU = novo_valor }
    // if (PosGraduação){ EstudantePosGraduacao::valorRU = novo_valor }
}

void Administrador::alterarValorMulta() {
    // Emprestimo::multa = novo_valor
}