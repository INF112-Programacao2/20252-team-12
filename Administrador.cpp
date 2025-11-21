#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>

static std::vector<std::string> split(const std::string& s, char delim) {
    std::vector<std::string> elems;
    size_t start = 0;
    while (true) {
        size_t pos = s.find(delim, start);
        if (pos == std::string::npos) {
            elems.push_back(s.substr(start));
            break;
        }
        elems.push_back(s.substr(start, pos - start));
        start = pos + 1;
    }
    return elems;
}

static bool validaData(const std::string& data) {
    // aceita D/M/YYYY, DD/MM/YYYY, com '/' como separador
    auto parts = split(data, '/');
    if (parts.size() != 3) {
        throw std::invalid_argument("[ERRO] Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    } catch (...) {
        throw std::invalid_argument("[ERRO] Data contém caracteres inválidos");
    }
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual) {
        throw std::invalid_argument("[ERRO] Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("[ERRO] Mês inválido");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        throw std::invalid_argument("[ERRO] Dia inválido para o mês especificado");
    }

    return true;
}

int Administrador::nextID = 1;
Administrador::Administrador(const std::string& _nome, const std::string &_cpf,const std::string& _data_de_nascimento, const std::string& _email, const std::string& _senha):
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

    std::cout<<"Tipo: ";
    std::getline(std::cin, _tipo);

    std::cout<<"Numero de exemplares: ";
    std::cin>>_numExemplares;

    Livro *novo_livro = new Livro(_titulo,_autor,_tipo,_numExemplares);
    biblioteca.adicionarLivro(*novo_livro);
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

    std::cin.ignore();
    std::cout<<std::endl;
    std::cout<<"=== FAVOR PREENCHER AS INFORMACOES ABAIXO ==="<<std::endl<<std::endl;

    std::string _nome, _cpf,_data_de_nascimento, _email, _senha, _matricula, _curso;
    std::cout<<"Nome: "; std::getline(std::cin,_nome);
    std::cout<<"CPF: "; std::getline(std::cin, _cpf);
    while (1){
        try{
            std::cout<<"Data de nascimento: "; std::getline(std::cin,_data_de_nascimento);
            break;
        } catch (std::exception &e){
            std::cerr << e.what() << std::endl;
        }
    }
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

        std::cin.ignore();
        std::cout<<"Tipo Pos Graduacao: "; std::getline(std::cin, _tipoPos);
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
        fout<<std::setw(40)<<estudante->getNome()<<std::setw(25)<<estudante->getDataDeNascimento()<<std::setw(12)<<estudante->get_matricula()<<std::setw(12)<<estudante->get_curso()<<std::endl;
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
    char resposta;
    while (true){
        std::cout << "Deseja visualizar todas as transações? (S/N)" << std::endl;
        std::cin >> resposta;
        if (resposta == 'S' || resposta == 's' || resposta == 'N' || resposta == 'n') break;
    }
    
    if (resposta == 'S' || resposta == 's'){
        for (auto estudante : estudantes){
            estudante->get_carteirinha()->exibir_extrato();
        }
    } else if (resposta == 'N' || resposta == 'n'){
        std::string matricula;
        std::cout << "Matrícula do aluno: ";
        std::cin >> matricula;
        for (auto estudante : estudantes){
            if (estudante->get_matricula() == matricula){
                estudante->get_carteirinha()->exibir_extrato();
                return;
            }
        }
        throw std::invalid_argument("[ERRO] Não foi possível localizar o Estudante com matrícula " + matricula);
    }
}

void Administrador::consultarEmprestimos(std::vector<Estudante*> &estudantes) {
    char resposta;
    while (true){
        std::cout << "Deseja visualizar todas os empréstimos? (S/N)" << std::endl;
        std::cin >> resposta;
        if (resposta == 'S' || resposta == 's' || resposta == 'N' || resposta == 'n') break;
    }
    
    if (resposta == 'S' || resposta == 's'){
        for (auto estudante : estudantes){
            estudante->exibirEmprestimos();
        }
    } else if (resposta == 'N' || resposta == 'n'){
        std::string matricula;
        std::cout << "Matrícula do aluno: ";
        std::cin >> matricula;
        for (auto estudante : estudantes){
            if (estudante->get_matricula() == matricula){
                estudante->exibirEmprestimos();
                return;
            }
        }
        throw std::invalid_argument("[ERRO] Não foi possível localizar o Estudante com matrícula " + matricula);
    }
}

void Administrador::recarregarCarteirinha(std::vector<Estudante*> &estudantes) {
    std::string matricula;
    std::cout << "Matriícula do aluno: ";
    std::cin >> matricula;

    for (auto estudante : estudantes){
        if (estudante->get_matricula() == matricula){
            estudante->recarregarCarteirinha();
            return;
        }
    }

    throw std::invalid_argument("[ERRO] Não foi possível localizar o Estudante com matrícula " + matricula);
}

void Administrador::alterarValorRU() {
    char resposta;
    double novo_valor;
    std::cout << "Escolha o estudante:\n";
    std::cout << "1 - Graduação\n2 - Pós-Graduação\n";
    std::cin >> resposta;
    if (resposta != '1' && resposta != '2'){
        throw std::invalid_argument("[ERRO] Opção inválida");
    }
    std::cout << "Novo valor: ";
    std::cin >> novo_valor;
    if (resposta == '1'){
        EstudanteGraduacao::set_valorRU(novo_valor);
    } else if (resposta == '2'){
        EstudantePosGraduacao::set_valorRU(novo_valor);
    }

    std::cout << "Valor alterado com sucesso!" << std::endl;

}

void Administrador::alterarValorMulta() {
    double novo_valor;
    std::cout << "Valor da nova multa: ";
    std::cin >> novo_valor;
    Emprestimo::setMulta(novo_valor);
    std::cout << "Valor da multa alterado!";
}