#ifndef EMPRESTIMO_HPP
#define EMPRESTIMO_HPP

#include <ctime>                    // Para time_t (tipo de data/hora Unix)
#include <string>                   // Para std::string

// ========== FORWARD DECLARATIONS ==========
// Declarações antecipadas para evitar dependência circular entre headers
class Estudante;                    // Forward declaration: Emprestimo guarda ponteiro para Estudante
class Livro;                        // Forward declaration: Emprestimo guarda ponteiro para Livro

// ========== CLASSE EMPRESTIMO ==========
// Representa um empréstimo de livro realizado por um estudante
// Gerencia:
//   - Identificação do empréstimo (ID único)
//   - Referência ao estudante e ao livro
//   - Datas de empréstimo e devolução
//   - Cálculo de multa por atraso
//   - Status de devolução
class Emprestimo
{
private:
    // ========== ATRIBUTOS ESTÁTICOS ==========
    // Compartilhados entre TODAS as instâncias de Emprestimo
    
    static int nextId;              // Próximo ID disponível para novos empréstimos
                                    // Auto-incrementa a cada novo empréstimo criado
    
    static double multa;            // Valor da multa diária por atraso (R$/dia)
                                    // Compartilhado entre todos os empréstimos
                                    // Pode ser alterado globalmente pelo administrador

    // ========== ATRIBUTOS DE INSTÂNCIA ==========
    // Únicos para cada empréstimo
    
    int id;                         // ID único do empréstimo (auto-incrementado)
    
    bool devolvido;                 // Flag que indica se o livro foi devolvido
                                    // true = devolvido, false = ainda pendente
    
    Estudante *origem;              // Ponteiro para o Estudante que realizou o empréstimo
    
    Livro *livro;                   // Ponteiro para o Livro emprestado
    
    time_t dataDeEmprestimo;        // Data inicial do empréstimo (timestamp Unix)
                                    // Armazenada como time_t, exibida como DD/MM/YYYY
    
    time_t dataDeDevolucao;         // Data limite para devolver o livro (timestamp Unix)
                                    // Usada para calcular multa por atraso

public:
    // ========== CONSTRUTOR ==========
    // Inicializa um novo empréstimo com dados do estudante, livro e datas
    // Parâmetros:
    //   _origem: referência ao Estudante responsável pelo empréstimo
    //   _livro: referência ao Livro sendo emprestado
    //   _dataDeEmprestimo: string com data inicial (DD/MM/YYYY)
    //   _dataDeDevolucao: string com data limite de devolução (DD/MM/YYYY)
    // Exceção: std::invalid_argument se datas forem inválidas
    Emprestimo(Estudante &_origem, Livro &_livro, std::string &_dataDeEmprestimo, std::string &_dataDeDevolucao);

    // ========== DESTRUTOR ==========
    // Não realiza limpeza especial (ponteiros são gerenciados externamente)
    ~Emprestimo();

    // ========== GETTERS ==========
    
    // Retorna o ID único do empréstimo
    // Retorno: inteiro com ID
    int getId();

    // Calcula e retorna quantos dias de atraso existem
    // Funcionamento: compara data de devolução com data/hora atual do sistema
    // Retorno: inteiro (negativo = antecipado, zero/positivo = atraso em dias)
    int getDiasDeAtraso();

    // Retorna o valor da multa diária (atributo estático compartilhado)
    // Retorno: double com valor em reais (ex: 10.0 = R$ 10,00/dia)
    static double getMulta();

    // Retorna se o empréstimo já foi devolvido
    // Retorno: true se devolvido, false se ainda pendente
    bool isDevolvido();

    // Retorna ponteiro para o Estudante que realizou o empréstimo
    // Retorno: ponteiro para Estudante
    Estudante *getOrigem();

    // Retorna ponteiro para o Livro emprestado
    // Retorno: ponteiro para Livro
    Livro *getLivro();

    // Retorna a data de empréstimo formatada como DD/MM/YYYY
    // Retorno: string com data formatada
    std::string getDataDeEmprestimo();

    // Retorna a data limite de devolução formatada como DD/MM/YYYY
    // Retorno: string com data formatada
    std::string getDataDeDevolucao();

    // ========== SETTERS ==========
    
    // Define manualmente o ID do empréstimo
    // Nota: usado internamente, preferir usar constructor para auto-incremento
    // Parâmetro:
    //   _id: novo ID para o empréstimo
    void setId(int _id);

    // Modifica a multa diária para TODOS os empréstimos (atributo estático)
    // Nota: método estático, afeta globalmente
    // Usado pelo administrador para alterar política de multas
    // Parâmetro:
    //   _multa: novo valor da multa diária em reais
    static void setMulta(double _multa);

    // Marca o empréstimo como devolvido ou não devolvido
    // Parâmetro:
    //   _devolvido: true para marcar como devolvido, false para não devolvido
    void setDevolvido(bool _devolvido);

    // Atualiza o estudante responsável pelo empréstimo
    // Parâmetro:
    //   _origem: referência para novo Estudante
    void setOrigem(Estudante &_origem);

    // Troca o livro associado ao empréstimo
    // Parâmetro:
    //   _livro: referência para novo Livro
    void setLivro(Livro &_livro);

    // Atualiza a data de empréstimo, validando o formato antes
    // Parâmetro:
    //   _dataDeEmprestimo: string com data (DD/MM/YYYY)
    // Exceção: std::invalid_argument se data for inválida
    void setDataDeEmprestimo(std::string _dataDeEmprestimo);

    // Atualiza a data de devolução, validando o formato antes
    // Parâmetro:
    //   _dataDeDevolucao: string com data (DD/MM/YYYY)
    // Exceção: std::invalid_argument se data for inválida
    void setDataDeDevolucao(std::string _dataDeDevolucao);

    // ========== OPERAÇÕES CALCULADAS ==========
    
    // Calcula o valor total da multa por atraso de devolução
    // Funcionamento:
    //   1. Obtém número de dias de atraso (getDiasDeAtraso)
    //   2. Se diasAtraso <= 0: retorna 0.0 (sem multa)
    //   3. Se diasAtraso > 0: calcula multa_diária × diasAtraso
    // Retorno: double com valor em reais (ex: 50.0 = R$ 50,00)
    // Exemplo: 5 dias de atraso com multa R$ 10/dia = R$ 50,00
    double calculaValorMulta();

    // ========== OPERAÇÕES DE EXIBIÇÃO ==========
    
    // Exibe todas as informações relevantes do empréstimo formatadas no console
    // Saída incluindo:
    //   - ID do empréstimo
    //   - Título do livro
    //   - Status (Devolvido / Não Devolvido)
    //   - Data de empréstimo (DD/MM/YYYY)
    //   - Data máxima de devolução (DD/MM/YYYY)
    void exibirInformacoes();
};

#endif
