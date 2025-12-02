# 🎓 Carteirinha Digital Universitária

Sistema de carteirinha digital desenvolvido em C++, focado no gerenciamento de usuários (estudantes e administradores), controle de saldo, acesso a serviços da universidade (RU, Biblioteca) e persistência de dados.

---

## 📘 Visão Geral

Este projeto simula o ecossistema de uma **universidade digital**. O sistema é orquestrado pela classe **Sistema**, que gerencia o fluxo de navegação, menus e salvamento de dados.
Os **Estudantes** podem utilizar a carteirinha para comer no Restaurante Universitário (RU), pegar livros emprestados e consultar saldo/extrato.
Os **Administradores** possuem controle total para gerenciar usuários, configurar valores (RU, multas), realizar auditorias e promover mobilidade acadêmica.
Além disso, o sistema integra **bibliotecas**, **livros** e **transações** digitais, permitindo controle de acesso e empréstimos conforme o tipo de estudante (graduação ou pós-graduação).

---

## 🧩 Estrutura de Classes

### ⚙️ Sistema
A classe principal que orquestra a execução do programa.

**Principais responsabilidades:**
- Gerenciar o login e os menus (Estudante e Administrador).
- Carregar e salvar dados em arquivos (`txt`, `csv`, etc.).
- Centralizar as listas de estudantes, livros e o administrador.

### 🛠️ Auxiliares
Módulo de funções estáticas e utilitárias.

**Principais responsabilidades:**
- Validação de dados (CPF, E-mail, Matrícula, Senha).
- Manipulação de datas e strings.
- Processamento de imagens (via biblioteca `CImg`) para visualização da carteirinha.

### 👤 Usuario (Base)
Substitui a antiga classe `Pessoa`. Representa um usuário genérico.

**Principais responsabilidades:**
- Armazenar credenciais (CPF, Senha) e dados pessoais.
- Fornecer a base para autenticação no sistema.

### 🎓 Estudante (herda de Usuario)
Representa o aluno portador da carteirinha.

**Principais responsabilidades:**
- Realizar operações de empréstimo e devolução de livros.
- Utilizar o Restaurante Universitário (`comerRU`).
- Consultar saldo, extrato e visualizar a carteirinha digital.

### 🧑‍💼 Administrador (herda de Usuario)
Usuário com privilégios de gestão.

**Principais responsabilidades:**
- Cadastrar estudantes e livros.
- Alterar configurações globais (Valor do RU, Valor da Multa).
- Gerenciar mobilidade acadêmica (mudança de curso/status).
- Visualizar carteirinhas e transações de qualquer aluno.

### 💳 Carteirinha
Gerencia o aspecto financeiro do estudante.

**Principais responsabilidades:**
- Manter o saldo e o histórico de transações (extrato).
- Processar depósitos e débitos.

### 💰 Transação
Registra cada movimentação financeira.

**Principais responsabilidades:**
- Armazenar tipo, valor, data e origem da operação.
- Garantir histórico auditável.

### 📚 Biblioteca & 📖 Livro
Gerenciam o acervo físico.

**Principais responsabilidades:**
- Controle de exemplares disponíveis e totais.
- Organização por título, autor e gênero.

### 🤝 Emprestimo
Representa o vínculo entre um estudante e um livro.

**Principais responsabilidades:**
- Controlar datas de empréstimo e devolução.
- Calcular multas por atraso automaticamente.

### 🎓 EstudanteGraduacao & EstudantePosGraduacao
Especializações de estudante.

**Diferenciações:**
- **Graduação:** Possui modalidade específica e regras padrão de RU.
- **Pós-Graduação:** Possui linha de pesquisa, tipo de pós e regras diferenciadas (prazos de devolução maiores).

---

## 🧠 Diagrama CRC

| Classe | Responsabilidades | Colaborações |
|--------|--------------------|--------------|
| **Sistema** | Controle de fluxo, I/O de arquivos, Menus | Usuario, Administrador, Estudante, Biblioteca |
| **Auxiliares** | Validações, Datas, Imagem, Logs | Todas as classes |
| **Usuario** | Identificação base | Sistema |
| **Estudante** | Uso de serviços (RU, Biblio), Saldo | Carteirinha, Emprestimo, Sistema |
| **Administrador** | Gestão global, Configuração de valores | Sistema, Estudante, Livro |
| **Carteirinha** | Saldo e Extrato | Estudante, Transacao |
| **Emprestimo** | Regras de devolução e Multa | Estudante, Livro |
| **Biblioteca** | Acervo de livros | Livro, Sistema |

---

## 📋 User Stories - Carteirinha Digital Universitária

### 👤 Ator: Usuário (Genérico)
_Funcionalidades comuns a Estudantes e Administradores._

- [ ] **US01 - Autenticação no Sistema**
  > **Como** usuário,  
  > **Quero** realizar login informando meu CPF/E-mail e senha,  
  > **Para** que eu possa acessar o menu correspondente ao meu nível de permissão (Estudante ou Admin) com segurança.

- [ ] **US02 - Segurança de Credenciais**
  > **Como** usuário,  
  > **Quero** que o sistema valide se minha senha está correta,  
  > **Para** impedir que terceiros acessem meus dados financeiros ou acadêmicos.

---

### 🧑‍💼 Ator: Administrador
_Responsável pela gestão acadêmica, financeira e do acervo._

#### 🔹 Gestão de Estudantes
- [ ] **US03 - Cadastrar Novo Estudante**
  > **Como** administrador,  
  > **Quero** registrar novos alunos informando nome, CPF, matrícula, curso e senha,  
  > **Para** que eles possam começar a utilizar os serviços da universidade.

- [ ] **US04 - Editar Dados de Estudante**
  > **Como** administrador,  
  > **Quero** alterar informações cadastrais de um estudante existente,  
  > **Para** corrigir erros de digitação ou atualizar dados desatualizados.

- [ ] **US05 - Mobilidade Acadêmica**
  > **Como** administrador,  
  > **Quero** alterar o status acadêmico de um aluno (ex: mudar de curso ou modalidade),  
  > **Para** refletir transferências internas ou mudanças de nível (Graduação/Pós).

- [ ] **US06 - Listagem Geral**
  > **Como** administrador,  
  > **Quero** visualizar a lista de todos os estudantes cadastrados,  
  > **Para** ter controle sobre o corpo discente ativo no sistema.

#### 🔹 Gestão Financeira & Configuração
- [ ] **US07 - Recarga de Saldo Manual**
  > **Como** administrador,  
  > **Quero** inserir créditos manualmente na carteirinha de um estudante,  
  > **Para** realizar atendimentos presenciais de recarga.

- [ ] **US08 - Configurar Preço do RU**
  > **Como** administrador,  
  > **Quero** definir valores diferenciados para o Restaurante Universitário (Graduação vs. Pós),  
  > **Para** ajustar a cobrança conforme as políticas de subsídio da universidade.

- [ ] **US09 - Configurar Valor de Multa**
  > **Como** administrador,  
  > **Quero** alterar o valor da multa diária por atraso na biblioteca,  
  > **Para** atualizar as penalidades sem precisar recompilar o código.

- [ ] **US10 - Auditoria de Transações**
  > **Como** administrador,  
  > **Quero** consultar o extrato detalhado de qualquer estudante,  
  > **Para** verificar gastos, identificar inconsistências ou resolver disputas de saldo.

#### 🔹 Gestão da Biblioteca
- [ ] **US11 - Cadastrar Livros**
  > **Como** administrador,  
  > **Quero** adicionar novos títulos ao acervo (informando autor, gênero e quantidade),  
  > **Para** disponibilizá-los para empréstimo.

- [ ] **US12 - Monitoramento de Empréstimos**
  > **Como** administrador,  
  > **Quero** visualizar todos os livros que estão emprestados no momento,  
  > **Para** ter controle sobre o patrimônio da biblioteca.

---

### 🎓 Ator: Estudante
_Funcionalidades para alunos de Graduação e Pós-Graduação._

#### 🔹 Serviços Financeiros (Carteirinha)
- [ ] **US13 - Consultar Saldo**
  > **Como** estudante,  
  > **Quero** visualizar meu saldo atual na tela inicial,  
  > **Para** saber se tenho créditos suficientes para usar os serviços.

- [ ] **US14 - Utilizar o RU (Restaurante Universitário)**
  > **Como** estudante,  
  > **Quero** pagar minha refeição debitando diretamente da carteirinha,  
  > **Para** agilizar o atendimento e evitar o uso de dinheiro físico.

- [ ] **US15 - Visualizar Extrato**
  > **Como** estudante,  
  > **Quero** ver o histórico das minhas últimas transações (débitos e créditos),  
  > **Para** controlar meus gastos mensais.

- [ ] **US16 - Visualizar Carteirinha Digital**
  > **Como** estudante,  
  > **Quero** gerar uma visualização gráfica da minha carteirinha (com foto/dados),  
  > **Para** comprovar meu vínculo na entrada da universidade ou biblioteca.

#### 🔹 Biblioteca & Acervo
- [ ] **US17 - Realizar Empréstimo**
  > **Como** estudante,  
  > **Quero** pegar um livro emprestado caso ele esteja disponível e eu não tenha pendências,  
  > **Para** auxiliar em meus estudos acadêmicos.

- [ ] **US18 - Devolver Livro**
  > **Como** estudante,  
  > **Quero** registrar a devolução de um livro,  
  > **Para** liberar o exemplar e estancar a contagem de dias de posse.

- [ ] **US19 - Consultar Meus Empréstimos**
  > **Como** estudante,  
  > **Quero** listar os livros que estão comigo e suas datas de devolução,  
  > **Para** evitar atrasos e multas.

---

### ⚙️ Requisitos de Sistema (Back-end)
_Comportamentos automatizados e regras de negócio._

- [ ] **US20 - Persistência de Dados**
  > **Como** sistema,  
  > **Devo** salvar automaticamente estudantes, livros e transações em arquivos (.txt/csv) ao fechar,  
  > **Para** garantir que nenhum dado seja perdido entre execuções.

- [ ] **US21 - Validação de Dados Cadastrais**
  > **Como** sistema,  
  > **Devo** validar o formato de CPF, E-mail e Matrícula durante o cadastro,  
  > **Para** garantir a integridade e padronização do banco de dados.

- [ ] **US22 - Cálculo Automático de Multa**
  > **Como** sistema,  
  > **Devo** calcular o valor da multa baseada nos dias de atraso no ato da devolução,  
  > **Para** debitar o valor correto do saldo do estudante automaticamente.

- [ ] **US23 - Bloqueio por Saldo Insuficiente**
  > **Como** sistema,  
  > **Devo** impedir transações (RU/Multas) caso o saldo seja insuficiente,  
  > **Para** evitar que a carteirinha fique com saldo negativo (exceto em regras específicas).

---

## ⚙️ Funcionalidades Principais

- **Gestão Completa de Acervo:** Cadastro e listagem de livros.
- **Sistema Financeiro:** Recarga, débito (RU) e pagamento de multas.
- **Restaurante Universitário:** Diferenciação de cobrança.
- **Empréstimos Inteligentes:** Cálculo automático de dias de atraso e multa.
- **Validações Robustas:** CPF, formatos de string e datas.
- **Visualização Gráfica:** Geração de "imagem" da carteirinha (usando CImg).
- **Persistência:** Salvamento automático em arquivos.

---

## 🧰 Tecnologias Utilizadas

- **Linguagem:** C++  
- **Paradigma:** Programação Orientada a Objetos (POO)  
- **Arquitetura:** Classes CRC, Abstração, Encapsulamento, Herança e Polimorfismo
- **CImg.h:** Processamento e exibição de imagens.
- **Entrada/Saída:** Arquivos e terminal  
- **Possível extensão:** Persistência em banco de dados e interface gráfica (GUI)

---

## 🧪 Estrutura do Projeto

```
📁 carteirinha-digital
├── .vscode/ # Configurações do editor
├── images/ # Recursos visuais (templates, códigos de barra)
├── main.cpp # Ponto de entrada
├── Sistema.hpp/cpp # Orquestrador
├── Auxiliares.hpp/cpp # Utilitários e Validações
├── Usuario.hpp/cpp # Classe Base
├── Estudante.hpp/cpp # Classe Derivada
├── EstudanteGraduacao.hpp/cpp
├── EstudantePosGraduacao.hpp/cpp
├── Administrador.hpp/cpp
├── Carteirinha.hpp/cpp
├── Transacao.hpp/cpp
├── Biblioteca.hpp/cpp
├── Livro.hpp/cpp
├── Emprestimo.hpp/cpp
├── Makefile # Automação de compilação
└── *.txt # Bancos de dados (ex: banco_estudantes.txt)
```

---

## 🚀 Como Executar

### ⚠️ Pré-requisitos
Para que a geração e visualização da carteirinha funcionem corretamente, é necessário instalar as dependências gráficas do X11 (utilizadas pela biblioteca `CImg`).

**No Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install libx11-dev
sudo apt-get install libjpeg-dev libpng-dev zlib1g-dev
```

.  **Clone o repositório:**
    ```bash
    git clone [https://github.com/INF112-Programacao2/20252-team-12.git](https://github.com/INF112-Programacao2/20252-team-12.git)
    cd 20252-team-12
    ```

2.  **Compile usando Make (Recomendado):**
    ```bash
    make
    ```

3.  **Ou compile manualmente (exemplo):**
    _Nota: Pode ser necessário vincular bibliotecas gráficas (X11, pthread) dependendo do seu sistema operacional devido ao uso da CImg._
    ```bash
    g++ main.cpp Sistema.cpp Auxiliares.cpp Usuario.cpp Estudante.cpp EstudanteGraduacao.cpp EstudantePosGraduacao.cpp Administrador.cpp Carteirinha.cpp Transacao.cpp Biblioteca.cpp Livro.cpp Emprestimo.cpp -o carteirinha -lpthread -lX11
    ```

4.  **Execute:**
    ```bash
    ./carteirinha
    ```

---

## 👨‍💻 Autores

**Luiz Filipe Santos Oliveira**    
📧 _Dev-LFSO_

**Thales Rodrigues Neves**    
📧 _tnevessss_

**Paulo Henrique de Souza Castro**   
📧 _Dev-PHSC_

**João Victor Moreira Carvalho**   
📧 _vmcjoao_

---

## 📄 Licença

Projeto educacional — livre para uso e adaptação acadêmica.
