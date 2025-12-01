# ==========================================
#   Makefile - Carteirinha Digital UFV
# ==========================================

CXX = g++

CXXFLAGS = -Wall -std=c++17 -g

TARGET = carteirinha

SRCS = main.cpp \
       Sistema.cpp \
       Usuario.cpp \
       Administrador.cpp \
       Estudante.cpp \
       EstudanteGraduacao.cpp \
       EstudantePosGraduacao.cpp \
       Carteirinha.cpp \
       Biblioteca.cpp \
       Livro.cpp \
       Transacao.cpp \
       Emprestimo.cpp \
       validar.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkando o executável: $(TARGET)..."
	@$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)
	@echo "Compilação concluída com sucesso!"

%.o: %.cpp
	@echo "Compilando $<..."
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Limpando arquivos temporários..."
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	@echo "Executando o programa..."
	./$(TARGET)