#define cimg_display 0
#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>
#include <thread>
#include <chrono>
#include "CImg.h"

using namespace cimg_library;

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

static std::string getDataAtual() {
    auto agora = std::chrono::system_clock::now();

    std::time_t tt = std::chrono::system_clock::to_time_t(agora);

    std::tm* data = std::localtime(&tt);

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << data->tm_mday << "/"
       << std::setw(2) << data->tm_mon + 1 << "/"
       << data->tm_year + 1900;

    return ss.str();
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

static std::string deixar_maiusculo(std::string &palavra){
    std::string resultado;

    for(char c: palavra)
        resultado += toupper(c);

    return resultado;
}

void Administrador::mobilidadeAcademica(EstudanteGraduacao *estudante, std::string parametro){
    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");

    if(!leitura.is_open())
        throw std::runtime_error("Nao foi possivel abrir o arquivo de leitura");

    bool numerico = false; //supondo nome do curso
    for(char c:parametro){
        if(isdigit(c)){ //é codigo numerico
            numerico = true;
            break;
        }
    }

    if(!numerico) //se o parametro for pelo nome do curso
        parametro = deixar_maiusculo(parametro); //deixamos o parametro todo em maisculo para procurar no .txt

    std::string linha;
    bool aux = true;
    std::string nome_curso, codigo_curso;
    bool encontrou = false;

    while(std::getline(leitura,linha)){
        if(aux) { //ignorar a primeira linha (lixo)
            aux = false;
            continue;
        }

        auto pos = linha.find(' ');//acha o espaço -- antes do espaço (codigo numerico) e depois do espaço (nome do curso)
        codigo_curso = linha.substr(0,pos);
        nome_curso = linha.substr(pos+1);

        if(numerico){ //se o parametro passado para mobilidade é o codigo da disciplina
            if(codigo_curso == parametro){
                encontrou = true;
                break; //nao precisamos mais ler o arquivo
            }
        }
        else { //se o parametro passado para mobilidade é o nome da disciplina
            if(nome_curso == parametro){
                encontrou = true;
                break;
            }
        }
    }

    if(!encontrou)
        throw std::invalid_argument("Parametro invalido para mobilidade academica");

    estudante->set_curso(codigo_curso); //altera o curso que o aluno está fazendo
    std::cout<<"Mobilidade academica realizada com sucesso!"<<std::endl;
    leitura.close();

    if(leitura.is_open())
        throw std::runtime_error("Nao foi possivel fechar o arquivo");
}


std::string Administrador::procurar_curso_por_codigo(std::string codigo){
    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");

    if(!leitura.is_open())
        throw std::runtime_error("Nao foi possivel abrir o arquivo de leitura");
    
    std::string linha;
    bool aux= true, encontrou = false;
    std::string nome_curso;

    while(std::getline(leitura, linha)){
        if(aux) {
            aux = false;
            continue;
        }
        auto pos = linha.find(' ');
        std::string codigo_linha = linha.substr(0,pos);
        if(codigo == codigo_linha){
            encontrou = true;
            nome_curso = linha.substr(pos+1);
            break;
        }
    }
    if(!encontrou)
        throw std::invalid_argument("Parametro invalido");
    
    leitura.close();
    if(leitura.is_open())
        throw std::runtime_error("Nao foi possivel fechar o arquivo de leitura");

    return nome_curso;
}

//funcao auxiliar para o metodo a gerar carteirinha:

static void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask) {
    cimg_forXY(img, x, y) {
        if (mask(x, y, 0) > 0 || mask(x, y, 1) > 0 || mask(x, y, 2) > 0) {
            img(x, y, 0) = 0; // vermelho
            img(x, y, 1) = 0; // verde
            img(x, y, 2) = 0; // azul
        }
    }
}

void Administrador::gerarCarteirinha(Estudante *estudante) { //adicionar como parametro como o usuario quer salvar o arquivo final -- sugestao: nome_aluno + "_carteirinha"
    //carregar as imagens
    CImg<unsigned char> img ("images/template.bmp");
    CImg<unsigned char> aluno("images/foto_aluno.bmp");

    //colocar a imagem do aluno no local certo
    aluno.resize(350,525);
    img.draw_image(33,314,aluno);

    //criar mascara RGB para por o texto
    CImg <unsigned char> mask (img.width(),img.height(),1,3,0);
    unsigned char branco[] = {255,255,255};

    //desenha o texto na mascara 
    mask.draw_text(520,321, estudante->getNome().c_str(), branco, 0, 255, 30, false); //.c_str() converte string para const char
    mask.draw_text(523,404, Administrador::procurar_curso_por_codigo(estudante->get_curso()).c_str(), branco, 0, 255, 30, false);
    mask.draw_text(577,488, estudante->get_matricula().c_str(), branco,0,255, 30, false);
    mask.draw_text(475,572, estudante->getCpf().c_str(), branco, 0 , 255, 30, false);
    mask.draw_text(564,655, "06/2026", branco, 0, 255, 30, false);
    mask.draw_text(1173,720, getDataAtual().c_str(), branco, 0, 255, 30, false);

    std::string aux = estudante->getNome();
    auto pos = aux.find(" ");
    std::string primeiroNome;
    if(pos == std::string::npos)
        primeiroNome = aux;
    else
        primeiroNome = aux.substr(0,pos);

    primeiroNome = deixar_maiusculo(primeiroNome);
    std::string nomeArquivo = "images/" + primeiroNome + "_" + estudante->get_matricula() + "_CARTEIRINHA.bmp"; //salvar a carteirinha com nome personalizado conforme o primeiro nome do aluno

    aplicarTextoPreto(img,mask);
    img.save(nomeArquivo.c_str());
}