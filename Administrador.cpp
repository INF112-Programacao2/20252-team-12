#define cimg_display 0
#include "Administrador.hpp"
#include <fstream>
#include <exception>
#include <iomanip>
#include <thread>
#include <chrono>
#include <ctime>
#include <cctype>
#include <sstream>
#include "CImg.h"

using namespace cimg_library;

static void escreveDevagar(const std::string &texto, int ms){
    for (char c : texto){
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }
}

static void pausa(int seg){
    std::this_thread::sleep_for(std::chrono::seconds(seg));
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

static void apagarTerminal(){
    #if defined(_WIN32) || defined(_WIN64)
        std::system("cls");
    #elif defined(__linux__) || defined(__APPLE__) || defined(__MACH__)
        std::system("clear");
    #endif
}

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
/*
static void aplicarTextoPreto(CImg<unsigned char> &img, CImg<unsigned char> &mask) {
    cimg_forXY(img, x, y) {
        if (mask(x, y, 0) > 0 || mask(x, y, 1) > 0 || mask(x, y, 2) > 0) {
            img(x, y, 0) = 0; // vermelho
            img(x, y, 1) = 0; // verde
            img(x, y, 2) = 0; // azul
        }
    }
}*/

static bool validaData(const std::string& data) {
    // aceita D/M/YYYY, DD/MM/YYYY, com '/' como separador
    auto parts = split(data, '/');
    if (parts.size() != 3) {
        throw std::invalid_argument("❌ Formato de data inválido. Use D/M/YYYY ou DD/MM/YYYY");
    }

    int dia, mes, ano;
    try {
        dia = std::stoi(parts[0]);
        mes = std::stoi(parts[1]);
        ano = std::stoi(parts[2]);
    } catch (...) {
        throw std::invalid_argument("❌ Data contém caracteres inválidos");
    }
    time_t agora = time(nullptr);
    struct tm *tnow = localtime(&agora);
    int ano_atual = tnow->tm_year + 1900;

    if (ano < 1900 || ano > ano_atual) {
        throw std::invalid_argument("❌ Ano fora do intervalo válido (1900 - ano atual)");
    }
    if (mes < 1 || mes > 12) {
        throw std::invalid_argument("❌ Mês inválido");
    }

    int diasPorMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if ((ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0)) {
        diasPorMes[1] = 29;
    }

    if (dia < 1 || dia > diasPorMes[mes - 1]) {
        throw std::invalid_argument("❌ Dia inválido para o mês especificado");
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

    std::cout << "\n============================================\n";
    std::cout << "         📚 MENU DE CRIAÇÃO DE LIVRO 📚\n";
    std::cout << "============================================\n";

    std::string _titulo, _autor, _tipo;
    int _numExemplares;

    std::cout<<"-> Titulo: ";
    std::getline(std::cin,_titulo);             // TODO: Tratamento de erro titulo

    std::cout << "--------------------------------------------\n";

    std::cout<<"-> Autor: ";
    std::getline(std::cin, _autor);             // TODO: Tratamento de erro autor

    std::cout << "--------------------------------------------\n";

    std::cout<<"-> Tipo: ";
    std::getline(std::cin, _tipo);              // TODO: Tratamento de erro tipo

    std::cout << "--------------------------------------------\n";

    std::cout<<"-> Numero de exemplares: ";
    std::cin>>_numExemplares;                   // TODO: Tratamento de erro numero_exemplares

    std::cout << "--------------------------------------------\n";

    Livro *novo_livro = new Livro(_titulo,_autor,_tipo,_numExemplares);
    biblioteca.adicionarLivro(*novo_livro);

    // (Escrever devagar)
    escreveDevagar(novo_livro->getTitulo() + " foi adicionado na Bibilioteca! ✅\n", 50);
}

void Administrador::criarEstudante(std::vector<Estudante*> &estudantes) {

    int opcao;

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CADASTRAMENTO DE ESTUDANTE 📚\n";
    std::cout << "============================================\n";

    std::cout<<"1 - Aluno Graduação"<<std::endl;
    std::cout<<"2 - Aluno Pós-Graduação"<<std::endl;
    std::cout << "--------------------------------------------\n";
    std::cout << "Opção: ";

    if (!(std::cin >> opcao)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Digite um número válido!");
    }

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout<<std::endl;
    std::cout<<"=== FAVOR PREENCHER AS INFORMACOES ABAIXO ==="<<std::endl<<std::endl;

    std::string _nome, _cpf,_data_de_nascimento, _email, _senha, _matricula, _curso;
    std::cout<<"-> Nome: "; std::getline(std::cin,_nome);               // TODO: Tratamento de erro nome
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> CPF: "; std::getline(std::cin, _cpf);                // TODO: Tratamento de erro cpf
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> Data de nascimento: "; std::getline(std::cin,_data_de_nascimento);               // TODO: Tratamento de erro data
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> Email: "; std::cin>>_email;      // TODO: Tratamento de erro email
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> Senha: "; std::cin>>_senha;      // TODO: Tratamento de erro senha
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> Matricula: "; std::cin>>_matricula;          // TODO: Tratamento de erro matricula
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "--------------------------------------------\n";
    std::cout<<"-> Curso: "; std::getline(std::cin, _curso);    // TODO: Tratamento de erro curso
    std::cout << "--------------------------------------------\n";

    if(opcao==1) {
        std::string _modalidade;
        std::cout<<"-> Modalidade de Graduação: "; std::cin>> _modalidade;      // TODO: Tratamento de erro modalidade
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        EstudanteGraduacao* _novoEstudante = new EstudanteGraduacao(_nome, _cpf,_data_de_nascimento, _email, _senha, _matricula, _curso, _modalidade);
        std::cout << "--------------------------------------------\n";
        estudantes.push_back(_novoEstudante);
    }
    else if(opcao==2) {
        std::string _tipoPos, _linhaDePesquisa;
        std::cout<<"-> Tipo Pos Graduacao: "; std::getline(std::cin, _tipoPos);         // TODO: Tratamento de erro tipo pos graduação
        std::cout << "--------------------------------------------\n";
        std::cout<<"-> Linha de Pesquisa: "; std::getline(std::cin, _linhaDePesquisa);          // TODO: Tratamento de erro linha de pesquisa
        std::cout << "--------------------------------------------\n";

        EstudantePosGraduacao* _novoEstudante = new EstudantePosGraduacao(_nome, _cpf, _data_de_nascimento, _email, _senha, _matricula, _curso, _tipoPos, _linhaDePesquisa);
        estudantes.push_back(_novoEstudante);
    }
    escreveDevagar("Estudante de matrícula " + _matricula + " foi cadastrado com sucesso! ✅\n", 50);
}

void Administrador::listarEstudante(std::vector<Estudante*> &estudantes) {

    std::ofstream fout;
    fout.open("ListaEstudantes.txt");
    if(!fout.is_open()){
        throw std::runtime_error("❌ Não foi possível abrir o arquvivo: Lista Estudantes ");
    }

    fout<<"LISTA DE ESTUDANTES CADASTRADOS: "<<std::endl<<std::endl;
    fout << std::left
    << std::setw(40) << "NOME"
    << std::setw(25) << "DATA DE NASCIMENTO"
    << std::setw(12) << "MATRICULA"
    << std::setw(12) << "CURSO"
    << "\n----------------------------------------------------------------------------------------------------------------\n";
    for(auto estudante :estudantes){
        fout<<std::setw(40)<<estudante->getNome()<<std::setw(25)<<estudante->getDataDeNascimento()<<std::setw(12)<<estudante->get_matricula()<<std::setw(12)<<estudante->get_curso()<<std::endl;
    }
    escreveDevagar("\nArquivo Lista Estudantes criado com sucesso ✅", 50);
    fout.close();
    if(fout.fail()){
        throw std::runtime_error("❌ Não foi possível fechar o arquvivo: ListaEstudantes ");
    }
}

// Modificar a função para perguntar qual o dado quer alterar
int Administrador::alterarDadosEstudante(std::vector<Estudante*> &estudantes) {
    std::cout << "\n============================================\n";
    std::cout << "   ✏️  MENU DE ALTERAÇÃO DE DADOS ✏️ \n";
    std::cout << "============================================\n";

    std::string matricula;
    std::cout << "-> Digite a matrícula do estudante: ";            
    std::cin >> matricula;          // TODO: Tratamento de erro matricula

    //acha o estudante alvo
    Estudante* estudanteAlvo = nullptr;
    for (auto* est : estudantes) {
        if (est->get_matricula() == matricula) {
            estudanteAlvo = est;
            break;
        }
    }

    if (estudanteAlvo == nullptr) {
        throw std::invalid_argument("❌ Estudante não encontrado com a matrícula: " + matricula);
    }

    int opcao;
    std::cout << "\nEstudante encontrado: " << estudanteAlvo->getNome() << "\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "1 - Alterar Nome\n";
    std::cout << "2 - Alterar Email\n";
    std::cout << "3 - Alterar Curso\n";
    std::cout << "4 - Alterar Senha\n";
    std::cout << "5 - Cancelar\n";
    std::cout << "--------------------------------------------\n";
    std::cout << "Opção: ";

    if (!(std::cin >> opcao)) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        throw std::invalid_argument("Digite um número válido!");
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::string novoDado;

    switch (opcao) {
        case 1:
            std::cout << "-> Novo Nome: ";
            std::getline(std::cin, novoDado);           // TODO: Tratamento de erro nome
            estudanteAlvo->setNome(novoDado);
            break;
        case 2:
            std::cout << "-> Novo Email: ";
            std::cin >> novoDado;                       // TODO: Tratamento de erro email
            estudanteAlvo->setEmail(novoDado);
            break;
        case 3:
            std::cout << "-> Novo Curso (Sigla ou Nome): ";
            std::getline(std::cin, novoDado);           // TODO: Tratamento de erro curso
            estudanteAlvo->set_curso(novoDado);
            break;
        case 4:
            std::cout << "-> Nova Senha: ";         
            std::cin >> novoDado;                       // TODO: Tratamento de erro senha
            estudanteAlvo->setSenha(novoDado);
            break;
        case 5:
            std::cout << "Operação cancelada.\n";
            return 5;
        default:
            throw std::invalid_argument("Opção inválida!");
    }

    escreveDevagar("\n✅ Dados atualizados com sucesso!\n", 50);
    return opcao;
}

void Administrador::alterarSenhaAdministrador() {
    std::cout << "--------------------------------------------\n";
    std::string nova_senha;
    std::cout << "-> Digite sua nova senha: "; std::getline(std::cin, nova_senha);          // TODO: Tratamento de erro senha
    this->setSenha(nova_senha);                         
    std::cout << "--------------------------------------------\n";
    escreveDevagar("✅ Senha alterada com sucesso!", 50);
}

void Administrador::consultarTransacoes(std::vector<Estudante*> &estudantes) {

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE CONSULTA DE TRANSAÇÕES 📚\n";
    std::cout << "============================================\n";

    char resposta;
    while (true){
        std::cout << "# Deseja visualizar todas as transações? (S/N)" << std::endl;
        std::cin >> resposta;
        if (resposta == 'S' || resposta == 's' || resposta == 'N' || resposta == 'n') break;
    }
    if (resposta == 'S' || resposta == 's'){
        for (auto estudante : estudantes){
            std::cout << "--------------------------------------------\n";
            std::cout << estudante->getNome() << " - " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            estudante->get_carteirinha()->exibir_extrato();
        }
    } else if (resposta == 'N' || resposta == 'n'){
        std::string matricula;
        std::cout << "-> Matrícula do aluno: ";
        std::cin >> matricula;                      // TODO: Tratamento de erro matricula
        apagarTerminal();
        for (auto estudante : estudantes){
            if (estudante->get_matricula() == matricula){
                std::cout << "--------------------------------------------\n";
                std::cout << estudante->getNome() << " - " << estudante->get_matricula() << std::endl;
                std::cout << "--------------------------------------------\n";
                estudante->get_carteirinha()->exibir_extrato();
                return;
            }
        }
        throw std::invalid_argument("❌ Não foi possível localizar o Estudante com matrícula " + matricula);
    }
}

void Administrador::consultarEmprestimos(std::vector<Estudante*> &estudantes) {

    std::cout << "\n============================================\n";
    std::cout << " 📚 MENU DE CADASTRAMENTO DE EMPRÉSTIMOS 📚\n";
    std::cout << "============================================\n";

    char resposta;
    while (true){
        std::cout << "# Deseja visualizar todas os empréstimos? (S/N)" << std::endl;
        std::cin >> resposta;
        if (resposta == 'S' || resposta == 's' || resposta == 'N' || resposta == 'n') break;
        apagarTerminal();
    }
    
    if (resposta == 'S' || resposta == 's'){
        for (auto estudante : estudantes){
            std::cout << "--------------------------------------------\n";
            std::cout << estudante->getNome() << " - " << estudante->get_matricula() << std::endl;
            std::cout << "--------------------------------------------\n";
            estudante->exibirEmprestimos();
        }
    } else if (resposta == 'N' || resposta == 'n'){
        std::string matricula;
        std::cout << "-> Matrícula do aluno: ";
        std::cin >> matricula;                  // TODO: Tratamento de erro matricula
        apagarTerminal();
        std::string lixo;
        for (auto estudante : estudantes){
            if (estudante->get_matricula() == matricula){
                std::cout << "--------------------------------------------\n";
                std::cout << estudante->getNome() << " - " << estudante->get_matricula() << std::endl;
                std::cout << "--------------------------------------------\n";
                estudante->exibirEmprestimos();
                return;
            }
        }
        throw std::invalid_argument("❌ Não foi possível localizar o Estudante com matrícula " + matricula);
    }
}

void Administrador::recarregarCarteirinha(std::vector<Estudante*> &estudantes) {

    std::cout << "\n============================================\n";
    std::cout << "  📚 MENU DE CARREGAMENTO DE CARTEIRINHA 📚\n";
    std::cout << "============================================\n";

    std::string matricula;
    std::cout << "-> Matrícula do aluno: ";
    std::cin >> matricula;                      // TODO: Tratamento de erro matricula

    for (auto estudante : estudantes){
        if (estudante->get_matricula() == matricula){
            estudante->recarregarCarteirinha();
            return;
        }
    }

    throw std::invalid_argument("❌ Não foi possível localizar o Estudante com matrícula " + matricula);
}

// TODO: (Thales) Implementar a função de gerar carteirinha
// Perguntar se deseja ver de todos ou de algum, perguntar matricula dele
// Pede para colocar a foto na pasta images com algum formato
// Verificar se existe e trata os erros, exibir erro se algum estudante não colocou a foto na pasta, mas gerar do resto se conseguiu ter
// Gera a carteirinha e coloca em uma pasta separada. Ex: 'Carteirinhas'
// Tenta seguir o padrão das funções quando for pedir algum dado escrito e no UI
void Administrador::visualizarCarteirinhas(std::vector<Estudante*> &estudantes){}

std::string Administrador::alterarValorRU() {

    std::cout << "\n============================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR RU 📚\n";
    std::cout << "============================================\n";

    std::string gradOuPos;
    char resposta;
    double novo_valor;
    std::cout << "Escolha o estudante:\n";
    std::cout << "1 - Graduação\n2 - Pós-Graduação\n";
    std::cout << "Opção: ";
    std::cin >> resposta;                                   // TODO: Tratamento de erro resposta
    if (resposta != '1' && resposta != '2'){
        throw std::invalid_argument("❌ Opção inválida");
    }
    std::cout << "-> Novo valor: ";
    std::cin >> novo_valor;                                 // TODO: Tratamento de erro novo_valor
    if (resposta == '1'){   
        EstudanteGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Graduação";
    } else if (resposta == '2'){
        EstudantePosGraduacao::set_valorRU(novo_valor);
        gradOuPos = "Pós-Graduação";
    }

    escreveDevagar("\nValor alterado com sucesso! ✅", 50);
    return gradOuPos;
}

void Administrador::alterarValorMulta() {

    std::cout << "\n=================================================================================\n";
    std::cout << "   📚 MENU DE ALTERAÇÃO DE VALOR NA MULTA DE EMPRÉSTIMO 📚\n";
    std::cout << "=================================================================================\n";

    double novo_valor;
    std::cout << "-> Valor da nova multa: ";
    std::cin >> novo_valor;                                     // TODO: Tratamento de erro novo_valor
    Emprestimo::setMulta(novo_valor);
    escreveDevagar("✅ Valor da multa alterado com sucesso!", 50);
}

static std::string deixar_maiusculo(std::string &palavra){
    std::string resultado;

    for(char c: palavra)
        resultado += toupper(c);

    return resultado;
}

void Administrador::mobilidadeAcademica(std::vector<Estudante*> &estudantes){
    
    std::ifstream leitura;
    leitura.open("codigo_cursos.txt");
    
    std::string matricula_aluno, parametro;

    while(true){
        std::cout<<"Matricula do Aluno: ";
        std::getline(std::cin,matricula_aluno);

        if(matricula_aluno.empty()) {
            std::cout<<"Entrada vazia. Digite novamente."<<std::endl;
            continue;
        }

        bool numero=true;

        for(char c:matricula_aluno) {
            if(!isdigit(c)){
                numero=false;
                break;
            }
        }

        if(!numero) {
            std::cout<<"A matricula deve conter apenas numeros."<<std::endl;
            continue;
        }
        
        break;
    }

    std::cout<<"Novo curso(codigo ou nome): ";
    std::getline(std::cin,parametro);

    Estudante* estudante = nullptr;

    for (auto aluno:estudantes){
        if(aluno->get_matricula() == matricula_aluno){
            estudante = aluno;
            break;
        }
    }

    if (estudante==nullptr)
        throw std::invalid_argument("Aluno não foi encontrado!");


    if(!leitura.is_open())
        throw std::runtime_error("Nao foi possivel abrir o arquivo de leitura");


    bool numerico = false; //supondo nome do curso -- TODO: verificar se essas verificações bastam para validar o tipo de parametro
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
    leitura.close();

    if(leitura.fail())
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
    if(leitura.fail())
        throw std::runtime_error("Nao foi possivel fechar o arquivo de leitura");

    return nome_curso;
}

//funcao auxiliar para o metodo a gerar carteirinha:
/*
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
} */