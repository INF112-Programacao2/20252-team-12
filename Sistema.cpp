#include "Sistema.hpp"
#include <iostream>                  // Para entrada/saída padrão
#include <string>                    // Para std::string
#include <sstream>                   // Para stringstream (parsing de CSV)
#include <fstream>                   // Para ifstream, ofstream (operações com arquivos)
#include <limits>                    // Para std::numeric_limits
#include <stdexcept>                 // Para exceções padrão
#include <cstdlib>                   // Para funções C (system, etc)
#include <chrono>                    // Para operações com tempo
#include <thread>                    // Para threads (se necessário)
#include "Auxiliares.hpp"            // Funções utilitárias (escreveLog, apagarTerminal, etc)

// ========== CONSTRUTOR ==========
// Inicializa o sistema completo:
//   1. Cria biblioteca vazia
//   2. Define valores padrão de RU e multa
//   3. Carrega dados persistidos do disco:
//      - Dados do administrador
//      - Estudantes cadastrados
//      - Livros do acervo
//      - Empréstimos pendentes
//      - Histórico de transações
Sistema::Sistema() : estudantes()
{
    this->biblioteca = new Biblioteca("Biblioteca UFV");  // Aloca nova biblioteca
    this->estudante_logado = nullptr;                      // Nenhum estudante logado inicialmente
    
    // Define valores padrão de RU
    EstudanteGraduacao::set_valorRU(5.4);                 // R$ 5,40 para graduação
    EstudantePosGraduacao::set_valorRU(2.7);              // R$ 2,70 para pós-graduação
    
    // Carrega dados do disco
    this->carregarAdmin();           // Carrega dados do administrador
    this->carregarDados();           // Carrega estudantes existentes
    this->carregarLivros();          // Carrega acervo de livros
    this->carregarEmprestimos();     // Carrega empréstimos pendentes
    this->carregarTransacoes();      // Carrega histórico de transações
}

// ========== DESTRUTOR ==========
// Libera memória e salva dados no disco antes de encerrar
// Funcionamento:
//   1. Salva todos os dados persistidos
//   2. Deleta ponteiros alocados dinamicamente
//   3. Evita vazamento de memória
Sistema::~Sistema()
{
    // Salva dados no disco
    this->salvarAdmin();
    this->salvarDados();
    this->salvarEmprestimos();
    this->salvarTransacoes();

    // Libera memória alocada
    for (auto estudante : this->estudantes)
    {
        delete estudante;           // Deleta cada estudante
    }
    delete this->admin;             // Deleta administrador
    delete this->biblioteca;        // Deleta biblioteca
}

// ========== SALVAR DADOS DOS ESTUDANTES ==========
// Salva todos os estudantes em arquivo CSV "banco_estudantes.txt"
// Formato: TIPO;NOME;CPF;DATA;EMAIL;SENHA;MATRICULA;CURSO;[ESPECÍFICO];SALDO
//   - GRAD: TIPO;...;MODALIDADE;SALDO
//   - POS: TIPO;...;TIPO_POS;LINHA_PESQUISA;SALDO
// Funcionamento:
//   1. Abre arquivo em modo escrita (sobrescreve conteúdo anterior)
//   2. Para cada estudante, identifica tipo (GRAD ou POS) usando dynamic_cast
//   3. Escreve dados formatados em CSV
//   4. Fechade arquivo e registra em log
void Sistema::salvarDados()
{
    std::ofstream file("banco_estudantes.txt");
    if (!file.is_open())
    {
        std::cerr << "Erro ao abrir ficheiro para salvar!\n";
        return;
    }

    // Itera sobre todos os estudantes
    for (Estudante *est : this->estudantes)
    {
        // Tenta converter para EstudanteGraduacao
        EstudanteGraduacao *grad = dynamic_cast<EstudanteGraduacao *>(est);
        // Tenta converter para EstudantePosGraduacao
        EstudantePosGraduacao *pos = dynamic_cast<EstudantePosGraduacao *>(est);

        // Se é estudante de graduação
        if (grad)
        {
            file << "GRAD;"
                 << grad->getNome() << ";"
                 << grad->getCpf() << ";"
                 << grad->getDataDeNascimento() << ";"
                 << grad->getEmail() << ";"
                 << grad->getSenha() << ";"
                 << grad->get_matricula() << ";"
                 << grad->get_curso() << ";"
                 << grad->get_modalidade() << ";"
                 << grad->get_carteirinha()->getSaldo() << "\n";
        }
        // Se é estudante de pós-graduação
        else if (pos)
        {
            file << "POS;"
                 << pos->getNome() << ";"
                 << pos->getCpf() << ";"
                 << pos->getDataDeNascimento() << ";"
                 << pos->getEmail() << ";"
                 << pos->getSenha() << ";"
                 << pos->get_matricula() << ";"
                 << pos->get_curso() << ";"
                 << pos->get_tipoPos() << ";"
                 << pos->get_linhaDePesquisa() << ";"
                 << pos->get_carteirinha()->getSaldo() << "\n";
        }
    }
    file.close();

    escreveLog("Dados salvos com sucesso!");
}

// ========== CARREGAR DADOS DOS ESTUDANTES ==========
// Carrega todos os estudantes do arquivo "banco_estudantes.txt"
// Formato esperado: TIPO;NOME;CPF;DATA;EMAIL;SENHA;MATRICULA;CURSO;[ESPECÍFICO];SALDO
// Funcionamento:
//   1. Tenta abrir arquivo (se não existir, assume primeira execução)
//   2. Para cada linha, faz parsing CSV separando por ";"
//   3. Identifica tipo (GRAD ou POS) e cria instância apropriada
//   4. Restaura saldo da carteirinha
//   5. Se arquivo vazio, cria estudante padrão
// Exceção: se arquivo não existir, registra em log e continua
void Sistema::carregarDados()
{
    std::ifstream file("banco_estudantes.txt");
    if (!file.is_open())
    {
        escreveLog("banco_estudantes.txt ainda não existe, sem dados para carregar.");
        return;
    }

    std::string linha;
    // Lê cada linha do arquivo
    while (std::getline(file, linha))
    {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        // Parse CSV: separa por ";"
        while (std::getline(ss, segmento, ';'))
        {
            dados.push_back(segmento);
        }

        // Validação: deve ter no mínimo 9 campos
        if (dados.size() >= 9)
        {
            // Extrai dados comuns
            std::string tipo = dados[0];
            std::string nome = dados[1];
            std::string cpf = dados[2];
            std::string data = dados[3];
            std::string email = dados[4];
            std::string senha = dados[5];
            std::string matricula = dados[6];
            std::string curso = dados[7];

            Estudante *novoEstudante = nullptr;

            // Se é graduação
            if (tipo == "GRAD")
            {
                std::string modalidade = dados[8];
                double saldo = std::stod(dados[9]);

                novoEstudante = new EstudanteGraduacao(nome, cpf, data, email, senha, matricula, curso, modalidade);
                novoEstudante->get_carteirinha()->setSaldo(saldo);
            }
            // Se é pós-graduação
            else if (tipo == "POS")
            {
                std::string tipoPos = dados[8];
                std::string linhaPesquisa = dados[9];
                double saldo = std::stod(dados[10]);

                novoEstudante = new EstudantePosGraduacao(nome, cpf, data, email, senha, matricula, curso, tipoPos, linhaPesquisa);
                novoEstudante->get_carteirinha()->setSaldo(saldo);
            }

            // Adiciona ao vetor se criado com sucesso
            if (novoEstudante != nullptr)
            {
                this->estudantes.push_back(novoEstudante);
            }
        }
    }
    file.close();

    // Se nenhum estudante foi carregado, cria um padrão
    if (this->estudantes.empty())
    {
        std::string data = "22/09/2006";
        EstudanteGraduacao *estudante = new EstudanteGraduacao("Luiz Filipe Santos Oliveira", "14422059629", data, "luiz.s.oliveira@ufv.br", "luiz", "120553", "141", "SISU");
        this->estudantes.push_back(estudante);
        return;
    }
}

// ========== SALVAR TRANSAÇÕES ==========
// Salva histórico de todas as transações de todas as carteirinhas em "banco_transacoes.txt"
// Formato: MATRICULA;TIPO;VALOR;DATA
// Funcionamento:
//   1. Abre arquivo em modo escrita
//   2. Para cada estudante, percorre seu extrato
//   3. Escreve cada transação em formato CSV
//   4. Registra conclusão em log
void Sistema::salvarTransacoes()
{
    std::ofstream file("banco_transacoes.txt");

    if (!file.is_open())
    {
        escreveLog("Erro ao salvar transações!");
        return;
    }

    // Itera sobre cada estudante
    for (Estudante *est : this->estudantes)
    {
        // Se carteirinha existe
        if (est->get_carteirinha())
        {
            // Para cada transação no extrato
            for (Transacao *t : est->get_carteirinha()->getExtrato())
            {
                file << est->get_matricula() << ";"
                     << t->get_tipo_transacao() << ";"
                     << t->get_valor_transacao() << ";"
                     << t->get_data() << "\n";
            }
        }
    }
    file.close();
    escreveLog("Log de transações salvo com sucesso.");
}

// ========== CARREGAR TRANSAÇÕES ==========
// Carrega histórico de transações do arquivo "banco_transacoes.txt"
// Formato esperado: MATRICULA;TIPO;VALOR;DATA
// Funcionamento:
//   1. Tenta abrir arquivo (se não existir, apenas registra em log)
//   2. Para cada linha, faz parsing CSV
//   3. Localiza estudante pela matrícula
//   4. Cria nova transação e adiciona ao extrato
// Exceção: se arquivo não existir, registra em log e continua
void Sistema::carregarTransacoes()
{
    std::ifstream file("banco_transacoes.txt");
    if (!file.is_open())
    {
        escreveLog("Arquivo banco_transacoes.txt não encontrado. Criando novo log.");
        return;
    }

    std::string linha;
    while (std::getline(file, linha))
    {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        // Parse CSV: separa por ";"
        while (std::getline(ss, segmento, ';'))
        {
            dados.push_back(segmento);
        }

        // Validação: deve ter 4 campos
        if (dados.size() >= 4)
        {
            std::string matricula = dados[0];
            std::string tipo = dados[1];
            double valor;
            
            // Tenta converter valor para double
            try {
                valor = std::stod(dados[2]);
            } catch (...) { continue; }  // Se falhar, pula linha
            
            std::string data = dados[3];

            // Procura estudante pela matrícula
            Estudante *estudanteAlvo = nullptr;
            for (auto est : this->estudantes)
            {
                if (est->get_matricula() == matricula)
                {
                    estudanteAlvo = est;
                    break;
                }
            }

            // Se encontrou estudante e carteirinha existe
            if (estudanteAlvo != nullptr && estudanteAlvo->get_carteirinha() != nullptr)
            {
                // Cria nova transação
                Transacao *novaTransacao = new Transacao(tipo, valor, data);

                novaTransacao->set_origem(*estudanteAlvo);

                // Adiciona ao extrato
                estudanteAlvo->get_carteirinha()->adicionarTransacao(novaTransacao);
            }
        }
    }
    file.close();
}

// ========== SALVAR EMPRÉSTIMOS ==========
// Salva todos os empréstimos pendentes em "banco_emprestimos.txt"
// Formato: MATRICULA;TITULO_LIVRO;DATA_EMPRESTIMO;DATA_DEVOLUCAO
// Nota: apenas salva empréstimos não devolvidos (isDevolvido() == false)
// Funcionamento:
//   1. Abre arquivo em modo escrita
//   2. Para cada estudante, percorre seus empréstimos
//   3. Escreve apenas os não devolvidos em formato CSV
//   4. Registra conclusão em log
void Sistema::salvarEmprestimos()
{
    std::ofstream file("banco_emprestimos.txt");
    if (!file.is_open())
    {
        escreveLog("Erro ao salvar empréstimos!");
        return;
    }

    // Para cada estudante
    for (Estudante *est : this->estudantes)
    {
        // Para cada empréstimo do estudante
        for (Emprestimo *emp : est->get_emprestimos())
        {
            // Se ainda não foi devolvido
            if (!emp->isDevolvido())
            {
                file << est->get_matricula() << ";"
                     << emp->getLivro()->getTitulo() << ";"
                     << emp->getDataDeEmprestimo() << ";"
                     << emp->getDataDeDevolucao() << "\n";
            }
        }
    }
    file.close();
    escreveLog("Empréstimos pendentes salvos com sucesso.");
}

// ========== CARREGAR EMPRÉSTIMOS ==========
// Carrega todos os empréstimos pendentes do arquivo "banco_emprestimos.txt"
// Formato esperado: MATRICULA;TITULO_LIVRO;DATA_EMPRESTIMO;DATA_DEVOLUCAO
// Funcionamento:
//   1. Tenta abrir arquivo (se não existir, apenas registra em log)
//   2. Para cada linha, faz parsing CSV
//   3. Localiza estudante pela matrícula e livro pelo título
//   4. Cria novo empréstimo e marca como não devolvido
//   5. Reduz exemplares disponíveis do livro
// Exceção: se arquivo não existir, registra em log e continua
void Sistema::carregarEmprestimos()
{
    std::ifstream file("banco_emprestimos.txt");
    if (!file.is_open())
    {
        escreveLog("Arquivo banco_emprestimos.txt não encontrado. Nenhum empréstimo carregado.");
        return;
    }

    std::string linha;
    while (std::getline(file, linha))
    {
        std::stringstream ss(linha);
        std::string segmento;
        std::vector<std::string> dados;

        // Parse CSV: separa por ";"
        while (std::getline(ss, segmento, ';'))
        {
            dados.push_back(segmento);
        }

        // Validação: deve ter 4 campos
        if (dados.size() >= 4)
        {
            std::string matricula = dados[0];
            std::string tituloLivro = dados[1];
            std::string dataEmp = dados[2];
            std::string dataDev = dados[3];

            // Procura estudante pela matrícula
            Estudante *estudanteAlvo = nullptr;
            for (auto est : this->estudantes)
            {
                if (est->get_matricula() == matricula)
                {
                    estudanteAlvo = est;
                    break;
                }
            }

            // Procura livro pelo título
            Livro *livroAlvo = nullptr;
            for (auto livro : this->biblioteca->getAcervo())
            {
                if (livro->getTitulo() == tituloLivro)
                {
                    livroAlvo = livro;
                    break;
                }
            }

            // Se encontrou ambos
            if (estudanteAlvo != nullptr && livroAlvo != nullptr)
            {
                // Cria novo empréstimo
                Emprestimo *novoEmprestimo = new Emprestimo(*estudanteAlvo, *livroAlvo, dataEmp, dataDev);

                novoEmprestimo->setDataDeEmprestimo(dataEmp);
                novoEmprestimo->setDataDeDevolucao(dataDev);
                novoEmprestimo->setDevolvido(false);

                // Adiciona ao estudante
                estudanteAlvo->adicionarEmprestimo(novoEmprestimo);

                // Reduz exemplares disponíveis
                livroAlvo->setNumExemplaresDisponiveis(livroAlvo->getNumExemplaresDisponiveis() - 1);
            }
        }
    }
    file.close();
}

// ========== GETTERS ==========

// Retorna vetor de todos os estudantes
std::vector<Estudante *> Sistema::get_estudantes()
{
    return this->estudantes;
}

// Retorna ponteiro para administrador
Administrador *Sistema::get_admin()
{
    return this->admin;
}

// ========== CARREGAR ADMINISTRADOR ==========
// Carrega dados do administrador do arquivo "admin_dados.txt"
// Formato: NOME;CPF;DATA;EMAIL;SENHA;VALOR_RU_GRAD;VALOR_RU_POS;VALOR_MULTA
// Funcionamento:
//   1. Tenta abrir arquivo
//   2. Se existe, parse primeira linha e restaura dados
//   3. Restaura valores de RU e multa
//   4. Se não existe, cria administrador padrão e salva
// Exceção: exceções são capturadas e registradas em log
void Sistema::carregarAdmin()
{
    try
    {
        std::ifstream file("admin_dados.txt");

        if (file.is_open())
        {
            std::string linha;

            // Lê primeira linha
            if (std::getline(file, linha))
            {
                std::stringstream ss(linha);
                std::string segmento;
                std::vector<std::string> dados;

                // Parse CSV: separa por ";"
                while (std::getline(ss, segmento, ';'))
                {
                    limparString(segmento);
                    dados.push_back(segmento);
                }

                // Validação: deve ter no mínimo 5 campos (nome, cpf, data, email, senha)
                if (dados.size() >= 5)
                {
                    // Cria administrador com dados
                    this->admin = new Administrador(dados[0], dados[1], dados[2], dados[3], dados[4]);
                    
                    // Restaura valores globais
                    if (dados.size() > 5)
                        EstudanteGraduacao::set_valorRU(std::stod(dados[5]));
                    if (dados.size() > 6)
                        EstudantePosGraduacao::set_valorRU(std::stod(dados[6]));
                    if (dados.size() > 7)
                        Emprestimo::setMulta(std::stod(dados[7]));
                    
                    file.close();
                    return;
                }
            }
            file.close();
        }

        // Se não conseguiu carregar, cria padrão
        escreveLog("Arquivo de admin não encontrado ou inválido. Criando admin padrão.");

        std::string dataAdmin = "23/04/1988";
        this->admin = new Administrador("Julio Cesar Soares dos Reis", "49588826691", dataAdmin, "jreis@ufv.br", "admin10");

        this->salvarAdmin();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// ========== SALVAR ADMINISTRADOR ==========
// Salva dados do administrador em arquivo "admin_dados.txt"
// Formato: NOME;CPF;DATA;EMAIL;SENHA;VALOR_RU_GRAD;VALOR_RU_POS;VALOR_MULTA
// Funcionamento:
//   1. Abre arquivo em modo escrita
//   2. Escreve dados do admin e valores globais
//   3. Fecha arquivo e registra em log
// Exceção: exceções são capturadas e registradas
void Sistema::salvarAdmin()
{
    try
    {
        std::ofstream file("admin_dados.txt");
        if (!file.is_open())
        {
            escreveLog("Erro ao abrir arquivo para salvar dados do administrador!");
            std::cerr << "❌ Erro ao salvar dados do administrador.\n";
            return;
        }

        // Salva no formato: NOME;CPF;DATA;EMAIL;SENHA;VALOR_RU_GRAD;VALOR_RU_POS_GRAD;VALOR_MULTA
        file << admin->getNome() << ";"
             << admin->getCpf() << ";"
             << admin->getDataDeNascimento() << ";"
             << admin->getEmail() << ";"
             << admin->getSenha() << ";"
             << EstudanteGraduacao::get_valorRU() << ";"
             << EstudantePosGraduacao::get_valorRU() << ";"
             << Emprestimo::getMulta() << "\n";

        file.close();

        escreveLog("Dados do administrador salvos.");
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// ========== CARREGAR LIVROS ==========
// Carrega todos os livros do arquivo "livros.txt"
// Formato esperado: TITULO,AUTOR,TIPO,NUM_EXEMPLARES
// Funcionamento:
//   1. Abre arquivo em modo leitura
//   2. Para cada linha, faz parsing CSV separando por ","
//   3. Cria novo livro e adiciona à biblioteca
//   4. Se linha malformada, registra aviso e pula
// Exceção: se arquivo não existir, lança exceção capturada
void Sistema::carregarLivros()
{
    try
    {
        std::ifstream fin("livros.txt");
        if (!fin.is_open())
        {
            throw std::invalid_argument("❌ Arquivo não encontrado!");
            return;
        }

        std::string linha;
        // Lê cada linha do arquivo
        while (std::getline(fin, linha))
        {
            std::stringstream ss(linha);
            std::string campo;

            std::string titulo, autor, tipo;
            int numExemplaresTotal = 0;

            std::vector<std::string> campos;

            // Parse CSV: separa por ","
            while (std::getline(ss, campo, ','))
            {
                campos.push_back(campo);
            }

            // Validação: deve ter exatamente 4 campos
            if (campos.size() == 4)
            {
                titulo = campos[0];
                autor = campos[1];
                tipo = campos[2];

                // Tenta converter número de exemplares
                try
                {
                    numExemplaresTotal = std::stoi(campos[3]);
                }
                catch (const std::exception &e)
                {
                    throw std::runtime_error("⚠️ Aviso: Linha mal formatada ou incompleta pulada: " + linha);
                    continue;
                }

                // Cria novo livro e adiciona à biblioteca
                Livro *novoLivro = new Livro(titulo, autor, tipo, numExemplaresTotal);
                this->biblioteca->adicionarLivro(*novoLivro);
            }
            else
            {
                throw std::runtime_error("⚠️ Aviso: Linha mal formatada ou incompleta pulada: " + linha);
            }
        }
        fin.close();
    }
    catch (std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

// ========== MENU DO ADMINISTRADOR ==========
// Exibe menu principal do administrador com 14 opções de gerenciamento
// Opções:
//   1. Criar Livro
//   2. Cadastrar Estudante
//   3. Listar Estudantes
//   4. Listar Livros
//   5. Visualizar Carteirinha de um Estudante
//   6. Alterar Dados de um Estudante
//   7. Alterar Sua Senha
//   8. Consultar Transações
//   9. Consultar Empréstimos
//   10. Recarregar Uma Carteirinha
//   11. Alterar o Valor do RU
//   12. Alterar o Valor da Multa do Empréstimo
//   13. Realizar Reopção de Curso (mobilidade acadêmica)
//   14. Sair
// Funcionamento:
//   1. Exibe menu em loop
//   2. Lê opção do usuário
//   3. Executa operação correspondente
//   4. Salva dados após cada operação
void Sistema::menuAdministrador()
{
    escreveLog("Administrador Logou no Sistema");
    int opcao;
    
    while (1)
    {
        // --- CABEÇALHO ---
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("👨 BEM-VINDO AO PAINEL DE ADMINISTRADOR 👨 \n", 50);
        escreveDevagar("--------------------------------------------\n", 10);
        
        // --- OPÇÕES DO MENU ---
        std::cout << "1 - Criar Livro\n";
        std::cout << "2 - Cadastrar Estudante\n";
        std::cout << "3 - Listar Estudantes\n";
        std::cout << "4 - Listar Livros\n";
        std::cout << "5 - Visualizar Carteirinha de um Estudante\n";
        std::cout << "6 - Alterar Dados de um Estudante\n";
        std::cout << "7 - Alterar Sua Senha\n";
        std::cout << "8 - Consultar Transações\n";
        std::cout << "9 - Consultar Empréstimos\n";
        std::cout << "10 - Recarregar Uma Carteirinha\n";
        std::cout << "11 - Alterar o Valor do RU\n";
        std::cout << "12 - Alterar o Valor da Multa do Empréstimo\n";
        std::cout << "13 - Realizar Reopção de Curso\n";
        std::cout << "14 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            // --- LEITURA DA OPÇÃO ---
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            apagarTerminal();

            // --- SWITCH COM OPÇÕES ---
            switch (opcao)
            {
            case 1:
                escreveLog("Administrador escolheu a opcao: 1 - Criar Livro");
                this->admin->criarLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 2:
                escreveLog("Administrador escolheu a opcao: 2 - Cadastrar Estudantes");
                this->admin->criarEstudante(this->estudantes);
                this->salvarDados();
                pausa(2);
                apagarTerminal();
                break;
            case 3:
                escreveLog("Administrador escolheu a opcao: 3 - Listar Estudantes");
                this->admin->listarEstudante(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 4:
                escreveLog("Administrador escolheu a opcao: 4 - Listar Livros");
                this->biblioteca->listarLivros();
                apagarTerminal();
                break;
            case 5:
                escreveLog("Administrador escolheu a opcao: 5 - Visualizar Carteirinha de um Estudante");
                this->admin->visualizarCarteirinhas(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 6:
            {
                escreveLog("Administrador escolheu a opcao: 6 - Alterar Dados de Estudante");
                int menuAltetacao = this->admin->alterarDadosEstudante(this->estudantes);
                escreveLog("Opcao " + std::to_string(menuAltetacao) + " escolhida dentro do menu de alteração de dados do estudante");
                this->salvarDados();
                pausa(2);
                apagarTerminal();
                break;
            }
            case 7:
                escreveLog("Administrador escolheu a opcao: 7 - Alterar sua Senha");
                this->admin->alterarSenhaAdministrador();
                this->salvarAdmin();
                pausa(2);
                apagarTerminal();
                break;
            case 8:
                escreveLog("Administrador escolheu a opcao: 8 - Consultar Transações");
                this->admin->consultarTransacoes(this->estudantes);
                std::cout << "\nAperte ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                apagarTerminal();
                break;
            case 9:
                escreveLog("Administrador escolheu a opcao: 9 - Consultar Empréstimos");
                this->admin->consultarEmprestimos(this->estudantes);
                std::cout << "\nAperte ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                apagarTerminal();
                break;
            case 10:
                escreveLog("Administrador escolheu a opcao: 10 - Recarregar uma Carteirinha");
                this->admin->recarregarCarteirinha(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 11:
            {
                escreveLog("Administrador escolheu a opcao: 11 - Alterar Valor do RU");
                std::string gradOuPos;
                gradOuPos = this->admin->alterarValorRU();
                escreveLog("Valor do RU para a " + gradOuPos + " alterado");
                pausa(2);
                apagarTerminal();
                break;
            }
            case 12:
                escreveLog("Administrador escolheu a opcao: 12 - Alterar o valor da Multa do Empréstimo");
                this->admin->alterarValorMulta();
                apagarTerminal();
                break;
            case 13:
                escreveLog("Administrador escolheu a opcao: 13 - Realizar reopção de curso");
                this->admin->mobilidadeAcademica(this->estudantes);
                pausa(2);
                apagarTerminal();
                break;
            case 14:
                escreveLog("Administrador escolheu a opcao: 14 - Sair");
                escreveLog("Logout realizado");
                escreveDevagar("📤 Fazendo logout...\n", 50);
                pausa(2);
                apagarTerminal();
                return;  // Sai do menu
            default:
                throw std::invalid_argument("Digite um número válido!");
                break;
            }
            
            // --- SALVA DADOS APÓS OPERAÇÃO ---
            this->salvarDados();
            this->salvarEmprestimos();
            this->salvarAdmin();
            this->salvarTransacoes();
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

// ========== MENU DO ESTUDANTE ==========
// Exibe menu principal do estudante com 10 opções
// Opções:
//   1. Consultar Saldo
//   2. Recarregar Carteirinha
//   3. Visualizar Carteirinha
//   4. Ver Extrato Financeiro
//   5. Buscar Livro no Acervo
//   6. Ver Meus Empréstimos
//   7. Pegar Livro Emprestado
//   8. Devolver Livro
//   9. Comer no RU
//   10. Sair
// Funcionamento:
//   1. Exibe menu em loop
//   2. Lê opção do usuário
//   3. Executa operação correspondente
//   4. Salva dados após cada operação
void Sistema::menuEstudante()
{
    int opcao;
    while (true)
    {
        // --- CABEÇALHO ---
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("🎓 BEM-VINDO AO PAINEL DO ESTUDANTE 🎓 \n", 50);
        escreveDevagar("============================================\n", 10);
        escreveDevagar("    Bem-vindo, " + this->estudante_logado->getNome() + "\n", 50);
        
        // --- OPÇÕES DO MENU ---
        std::cout << "--------------------------------------------\n";
        std::cout << "1 - Consultar Saldo\n";
        std::cout << "2 - Recarregar Carteirinha\n";
        std::cout << "3 - Visualizar Carteirinha\n";
        std::cout << "4 - Ver Extrato Financeiro\n";
        std::cout << "5 - Buscar Livro no Acervo\n";
        std::cout << "6 - Ver Meus Empréstimos\n";
        std::cout << "7 - Pegar Livro Emprestado\n";
        std::cout << "8 - Devolver Livro\n";
        std::cout << "9 - Comer no RU\n";
        std::cout << "10 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            // --- LEITURA DA OPÇÃO ---
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("Digite um número válido!");
            }
            apagarTerminal();

            // --- SWITCH COM OPÇÕES ---
            switch (opcao)
            {
            case 1:
                escreveLog("Estudante Escolheu a Opcao: 1 - Consultar Saldo");
                this->estudante_logado->consultarSaldo();
                pausa(2);
                apagarTerminal();
                break;
            case 2:
                escreveLog("Estudante Escolheu a Opcao: 2 - Recarregar Carteirinha");
                this->estudante_logado->recarregarCarteirinha();
                pausa(2);
                apagarTerminal();
                break;
            case 3:
                escreveLog("Estudante Escolheu a Opcao: 3 - Visualizar Carteirinha");
                this->estudante_logado->visualizarCarteirinha();
                pausa(2);
                apagarTerminal();
                break;
            case 4:
                escreveLog("Estudante Escolheu a Opcao: 4 - Ver Extrato Financeiro");
                this->estudante_logado->get_carteirinha()->exibir_extrato();
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.ignore();
                apagarTerminal();
                break;
            case 5:
                escreveLog("Estudante Escolheu a Opção: 5 - Buscar Livro no Acervo");
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                this->biblioteca->listarLivros();
                apagarTerminal();
                break;
            case 6:
                escreveLog("Estudante Escolheu a Opcao: 6 - Ver Meus Empréstimos");
                this->estudante_logado->exibirEmprestimos();
                std::cout << "\nPressione ENTER para continuar...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cin.ignore();
                apagarTerminal();
                break;
            case 7:
                escreveLog("Estudante Escolheu a Opcao: 7 - Pegar Livro Emprestado");
                apagarTerminal();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                this->estudante_logado->pegarLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 8:
                escreveLog("Estudante Escolheu a Opcao: 8 - Devolver Livro");
                this->estudante_logado->devolverLivro(*this->biblioteca);
                pausa(2);
                apagarTerminal();
                break;
            case 9:
                escreveLog("Estudante Escolheu a Opcao: 9 - ComerRU");
                this->estudante_logado->comerRU();
                apagarTerminal();
                break;
            case 10:
                escreveLog("Logout realizado");
                escreveDevagar("📤 Fazendo logout...\n", 50);
                pausa(2);
                apagarTerminal();
                this->estudante_logado = nullptr;
                return;  // Sai do menu
            default:
                throw std::invalid_argument("Digite um número válido!");
                break;
            }
            
            // --- SALVA DADOS APÓS OPERAÇÃO ---
            this->salvarDados();
            this->salvarEmprestimos();
            this->salvarTransacoes();
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro: " << e.what() << std::endl;
        }
    }
}

// ========== INICIAR SISTEMA ==========
// Função principal que inicia o loop de login e menus
// Funcionamento:
//   1. Exibe tela inicial de boas-vindas
//   2. Loop de autenticação:
//      - Lê email e senha
//      - Valida contra administrador ou estudantes
//      - Chama menu correspondente se autenticado
//      - Repete se falha autenticação
//   3. Saída segura (volta a menu inicial)
void Sistema::iniciarSistema()
{
    escreveLog("Sistema aberto");

    int opcao;

    while (1)
    {
        // --- TELA INICIAL ---
        escreveDevagar("\n============================================\n", 10);
        escreveDevagar("      📚 BEM-VINDO AO SISTEMA-UFV 📚\n", 50);
        escreveDevagar("--------------------------------------------\n", 10);
        std::cout << "1 - Fazer Login\n";
        std::cout << "2 - Sair\n";
        std::cout << "--------------------------------------------\n";
        std::cout << "Opção: ";

        try
        {
            // --- LEITURA DA OPÇÃO ---
            if (!(std::cin >> opcao))
            {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                throw std::invalid_argument("❌ Digite um número válido!");
            }

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            // --- OPÇÃO 2: SAIR ---
            if (opcao == 2)
            {
                escreveDevagar("👋 Obrigado por usar o SISTEMA-UFV. Até logo!\n", 80);
                break;
            }
            // --- OPÇÃO 1: FAZER LOGIN ---
            else if (opcao == 1)
            {
                bool logado = false;

                // --- LOOP DE AUTENTICAÇÃO ---
                while (!logado)
                {
                    std::string email, senha;

                    // --- LEITURA DO EMAIL ---
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Email: ";
                    std::getline(std::cin, email);

                    // --- LEITURA DA SENHA ---
                    std::cout << "--------------------------------------------\n";
                    std::cout << "Senha: ";
                    std::getline(std::cin, senha);

                    std::cout << "--------------------------------------------\n";

                    limparString(senha);
                    limparString(email);

                    // --- VALIDAÇÃO 1: ADMINISTRADOR ---
                    if (this->admin->getSenha() == senha && this->admin->getEmail() == email)
                    {
                        escreveDevagar("✅ Bem-Vindo " + this->admin->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuAdministrador();
                        logado = true;
                        escreveLog("Administrador logou no sistema");
                        break;
                    }

                    // --- VALIDAÇÃO 2: ESTUDANTE ---
                    bool encontrado = false;
                    for (auto estudante : this->estudantes)
                    {
                        if (estudante->getEmail() == email && estudante->getSenha() == senha)
                        {
                            this->estudante_logado = estudante;
                            encontrado = true;
                            escreveLog("Estudante " + estudante->get_matricula() + " logou no sistema");
                            break;
                        }
                    }

                    // Se encontrou estudante e logado com sucesso
                    if (encontrado && this->estudante_logado != nullptr)
                    {
                        escreveDevagar("✅ Bem-Vindo " + this->estudante_logado->getNome(), 50);
                        pausa(1);
                        apagarTerminal();
                        this->menuEstudante();
                        logado = true;
                        break;
                    }
                    // Se não encontrou ninguém com essas credenciais
                    else
                    {
                        throw std::invalid_argument("\n❌ Credenciais inválidas. Tente novamente.\n");
                    }
                }
            }
            // --- OPÇÃO INVÁLIDA ---
            else
            {
                throw std::invalid_argument("❌ Opção inválida. Por favor, escolha 1 ou 2.");
            }
        }
        catch (const std::exception &e)
        {
            apagarTerminal();
            std::cerr << e.what() << std::endl;
        }
        catch (...)
        {
            apagarTerminal();
            std::cerr << "\n❌ Ocorreu um erro desconhecido!\n";
        }
    }

    escreveLog("Sistema Fechado");
}