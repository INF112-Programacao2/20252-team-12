# ==========================================
#   Makefile - Carteirinha Digital UFV
# ==========================================

CXX = g++
CXXFLAGS = -Wall -std=c++17 -g \
           -Dcimg_use_jpeg -Dcimg_use_png

LDFLAGS = -ljpeg -lpng -lz -lm -lpthread -lX11

TARGET = main

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
       Auxiliares.cpp 

OBJS = $(SRCS:.cpp=.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linkando o executável: $(TARGET)..."
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"

%.o: %.cpp
	@echo "Compilando $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Limpando arquivos temporários..."
	rm -f $(OBJS) $(TARGET)

run: $(TARGET)
	@echo "Executando o programa..."
	./$(TARGET)
