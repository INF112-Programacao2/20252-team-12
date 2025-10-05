# 🎓 Carteirinha Digital Universitária

Sistema de carteirinha digital desenvolvido em C++, com foco em gerenciamento de usuários (estudantes e administradores) e controle de saldo para uso em serviços da universidade (restaurante, cópias, transporte, etc).

---

## 📘 Visão Geral

Este projeto simula o funcionamento de uma **carteirinha universitária digital**, permitindo que **estudantes** consultem e recarreguem seus saldos, enquanto **administradores** podem visualizar e ajustar as carteirinhas de todos os estudantes.

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

---

### 🧑‍💼 Administrador (herda de Pessoa)
Usuário com privilégios administrativos no sistema.

**Principais responsabilidades:**
- Consultar e modificar o saldo de carteirinhas.
- Gerenciar usuários (criar, desativar).
- Visualizar histórico de transações dos estudantes.

---

### 💳 Carteirinha
Entidade que armazena e gerencia o saldo digital do estudante.

**Principais responsabilidades:**
- Manter o saldo atualizado.
- Validar operações e impedir saldo negativo.

---

## 🧠 Diagrama CRC

O projeto segue o modelo CRC (**Class–Responsibility–Collaboration**) para o design de classes.

| Classe | Responsabilidades | Colaborações |
|--------|--------------------|--------------|
| Pessoa | Identificação e autenticação | Estudante, Administrador |
| Estudante | Consultar e recarregar saldo | Pessoa, Carteirinha |
| Administrador | Gerenciar carteirinhas e usuários | Pessoa, Estudante, Carteirinha |
| Carteirinha | Controlar saldo | Estudante, Administrador |

---

## 🧾 User Stories

- **Pessoa:**  
  _Como pessoa, quero ter meus dados básicos registrados e autenticados no sistema, para que eu possa ser identificado corretamente._

- **Estudante:**  
  _Como estudante, quero consultar e recarregar o saldo da minha carteirinha digital, para que eu possa utilizá-la dentro da universidade._

- **Administrador:**  
  _Como administrador, quero gerenciar os estudantes e suas carteirinhas, para que eu possa controlar e corrigir saldos e acompanhar transações._

- **Carteirinha:**  
  _Como carteirinha digital, quero armazenar e atualizar o saldo de cada estudante com segurança, para que todas as transações sejam confiáveis._

---

## ⚙️ Funcionalidades Principais

✅ Cadastro de pessoas.  
✅ Consulta de saldo por estudante.  
✅ Recarga de carteirinha (crédito).  
✅ Débito automático em compras/sistemas simulados.  
✅ Ajustes administrativos de saldo.   
✅ Hierarquia de permissões (Estudante / Administrador).

---

## 🧰 Tecnologias Utilizadas

- **Linguagem:** C++  
- **Paradigma:** Programação Orientada a Objetos (POO)  
- **Arquitetura:** Classes CRC, Abstração, Encapsulamento, Herança e Polimorfismo
- **Entrada/Saída:** Arquivos e terminal  
- **Possível extensão:** Persistência em banco de dados e GUI

---

## 🧪 Estrutura do Projeto

```
📁 carteirinha-digital
├── main.cpp
├── Pessoa.h / Pessoa.cpp
├── Estudante.h / Estudante.cpp
├── Administrador.h / Administrador.cpp
├── Carteirinha.h / Carteirinha.cpp
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
   g++ main.cpp Pessoa.cpp Estudante.cpp Administrador.cpp Carteirinha.cpp -o carteirinha
   ```

3. **Execute o programa:**
   ```bash
   ./carteirinha
   ```

---

## 🧭 Próximos Passos
 
- [ ] Adicionar autenticação com login/senha.  
- [ ] Criar interface visual simples (CLI ou GUI).  
- [ ] Adicionar classe "Transacao" para controle de extrato. 

---

## 👨‍💻 Autor

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

Este projeto é de uso educacional e pode ser livremente adaptado para fins acadêmicos ou de aprendizado.
