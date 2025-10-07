# 🎓 Carteirinha Digital Universitária

Sistema de carteirinha digital desenvolvido em C++, com foco em gerenciamento de usuários (estudantes e administradores) e controle de saldo para uso em serviços da universidade (restaurante, cópias, transporte, biblioteca, etc).

---

## 📘 Visão Geral

Este projeto simula o funcionamento de uma **carteirinha universitária digital**, permitindo que **estudantes** consultem e recarreguem seus saldos, enquanto **administradores** podem visualizar e ajustar as carteirinhas de todos os estudantes.  
Além disso, o sistema integra **bibliotecas**, **livros** e **transações** digitais, permitindo controle de acesso e empréstimos conforme o tipo de estudante (graduação ou pós-graduação).

---

## 🧩 Estrutura de Classes

### 🧍 Pessoa
Classe base que representa uma pessoa genérica no sistema.  
Contém informações básicas de identificação e autenticação.

**Principais responsabilidades:**
- Armazenar dados pessoais (nome, matrícula, e-mail).
- Fornecer identificação para as subclasses.

---

### 🎓 Estudante (herda de Pessoa)
Representa um aluno que possui uma carteirinha digital.

**Principais responsabilidades:**
- Consultar o saldo da sua carteirinha.
- Recarregar o saldo através de meios permitidos.
- Acessar serviços da universidade (como biblioteca e restaurante).

---

### 🧑‍💼 Administrador (herda de Pessoa)
Usuário com privilégios administrativos no sistema.

**Principais responsabilidades:**
- Consultar e modificar o saldo de carteirinhas.
- Gerenciar usuários (criar, desativar, redefinir senha).
- Visualizar histórico de transações dos estudantes.
- Gerenciar livros e bibliotecas.

---

### 💳 Carteirinha
Entidade que armazena e gerencia o saldo digital do estudante.

**Principais responsabilidades:**
- Manter o saldo atualizado.
- Validar operações e impedir saldo negativo.
- Servir como autorização para entrada na biblioteca.

---

### 💰 Transação
Representa uma operação que altera o saldo da carteirinha (crédito ou débito).

**Principais responsabilidades:**
- Registrar, validar e aplicar alterações de saldo.
- Garantir integridade e rastreabilidade das operações.
- Permitir auditoria por administradores.

---

### 📚 Biblioteca
Entidade que valida o acesso de estudantes e gerencia empréstimos.

**Principais responsabilidades:**
- Validar acesso de estudantes por carteirinha.
- Verificar situação do estudante (regular ou com pendências).
- Gerenciar empréstimos e devoluções de livros.

---

### 📖 Livro
Representa um livro do acervo da biblioteca.

**Principais responsabilidades:**
- Armazenar informações bibliográficas.
- Controlar disponibilidade para empréstimo.
- Registrar datas de empréstimo e devolução.

---

### 🎓 Graduação (herda de Estudante)
Representa um estudante de curso de graduação.

**Principais responsabilidades:**
- Consultar e recarregar saldo da carteirinha.
- Realizar empréstimos de livros conforme regras da graduação.

---

### 🎓 Pós-Graduação (herda de Estudante)
Representa um estudante de pós-graduação.

**Principais responsabilidades:**
- Consultar e recarregar saldo da carteirinha.
- Realizar empréstimos com prazos diferenciados.
- Acessar serviços avançados da universidade.

---

## 🧠 Diagrama CRC

| Classe | Responsabilidades | Colaborações |
|--------|--------------------|--------------|
| Pessoa | Identificação e autenticação | Estudante, Administrador |
| Estudante | Consultar e recarregar saldo | Pessoa, Carteirinha |
| Administrador | Gerenciar carteirinhas, usuários e livros | Pessoa, Estudante, Carteirinha, Biblioteca |
| Carteirinha | Controlar saldo e acesso | Estudante, Administrador, Biblioteca |
| Transação | Registrar e validar operações financeiras | Carteirinha, Estudante, Administrador |
| Biblioteca | Validar acesso e empréstimos | Carteirinha, Estudante, Livro |
| Livro | Armazenar e gerenciar disponibilidade | Biblioteca, Estudante |
| Graduação | Regras específicas de estudantes de graduação | Biblioteca, Transação |
| Pós-Graduação | Regras específicas de pós-graduação | Biblioteca, Transação |

---

## 🧾 User Stories

- **Pessoa:**  
  _Como pessoa, quero registrar meus dados básicos (nome, e-mail, data de nascimento), para que o sistema possa me identificar de forma única._

- **Estudante:**  
  _Como estudante, quero consultar e recarregar o saldo da minha carteirinha digital, para que eu possa utilizá-la para pagamentos e serviços dentro da universidade._

- **Administrador:**  
  _Como administrador, quero gerenciar os estudantes e suas carteirinhas, para que eu possa controlar saldos, corrigir erros e acompanhar as transações do sistema._

- **Carteirinha:**  
  _Como carteirinha digital, quero armazenar e atualizar o saldo de cada estudante com segurança, para que todas as transações sejam registradas e o uso seja confiável._

- **Transação:**  
  _Como transação, quero registrar e aplicar alterações de saldo na carteirinha digital com validação e rastreabilidade, para que o sistema possa garantir a integridade dos saldos e permitir auditoria._

- **Biblioteca:**  
  _Como biblioteca, quero validar o acesso de estudantes por meio da carteirinha digital, para que apenas usuários regulares possam utilizar o espaço físico e os serviços oferecidos._

- **Livro:**  
  _Como livro, quero armazenar minhas informações bibliográficas e estado de empréstimo, para que a biblioteca e os estudantes possam consultar minha disponibilidade e registrar empréstimos ou devoluções corretamente._

- **Graduação:**  
  _Como estudante de graduação, quero utilizar minha carteirinha para acessar serviços e realizar empréstimos conforme as regras da graduação._

- **Pós-Graduação:**  
  _Como estudante de pós-graduação, quero utilizar minha carteirinha digital para acessar serviços da universidade e empréstimos prolongados de livros, para que eu possa otimizar meu tempo e ter suporte às minhas atividades de pesquisa._

---

## ⚙️ Funcionalidades Principais

- Cadastro de pessoas e autenticação.  
- Consulta e recarga de saldo por estudante.  
- Registro e validação de transações financeiras.  
- Controle de acesso à biblioteca por carteirinha.  
- Empréstimo e devolução de livros.  
- Diferenciação de regras entre graduação e pós-graduação.  
- Ajustes administrativos e auditoria.  

---

## 🧰 Tecnologias Utilizadas

- **Linguagem:** C++  
- **Paradigma:** Programação Orientada a Objetos (POO)  
- **Arquitetura:** Classes CRC, Abstração, Encapsulamento, Herança e Polimorfismo  
- **Entrada/Saída:** Arquivos e terminal  
- **Possível extensão:** Persistência em banco de dados e interface gráfica (GUI)

---

## 🧪 Estrutura do Projeto

```
📁 carteirinha-digital
├── main.cpp
├── Pessoa.h / Pessoa.cpp
├── Estudante.h / Estudante.cpp
├── Administrador.h / Administrador.cpp
├── Carteirinha.h / Carteirinha.cpp
├── Biblioteca.h / Biblioteca.cpp
├── Livro.h / Livro.cpp
├── Transacao.h / Transacao.cpp
├── Graduacao.h / Graduacao.cpp
├── PosGraduacao.h / PosGraduacao.cpp
├── README.md
```

---

## 🚀 Como Executar

1. **Clone o repositório:**
   ```bash
   git clone https://github.com/INF112-Programacao2/20252-team-12.git
   cd carteirinha-digital
   ```

2. **Compile o projeto (exemplo com g++):**
   ```bash
   g++ main.cpp Pessoa.cpp Estudante.cpp Administrador.cpp Carteirinha.cpp Biblioteca.cpp Livro.cpp Transacao.cpp Graduacao.cpp PosGraduacao.cpp -o carteirinha
   ```

3. **Execute o programa:**
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
