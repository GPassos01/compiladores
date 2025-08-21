# Makefile para o projeto de Compiladores
# Analisador Léxico - Lexer

# Compilador
CC = gcc

# Flags de compilação
CFLAGS = -Wall -Wextra -std=c99 -g

# Nome do executável
TARGET = lexer

# Arquivos fonte
SOURCES = main.c lexer.c

# Arquivos objeto (gerados automaticamente)
OBJECTS = $(SOURCES:.c=.o)

# Arquivo de cabeçalho
HEADERS = main.h

# Regra padrão (primeira regra)
all: $(TARGET)

# Regra para compilar o executável
$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET)

# Regra para compilar arquivos .c em .o
%.o: %.c $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

# Regra para limpar arquivos gerados
clean:
	rm -f $(OBJECTS) $(TARGET)

# Regra para executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para mostrar ajuda
help:
	@echo "Comandos disponíveis:"
	@echo "  make        - Compila o projeto"
	@echo "  make clean  - Remove arquivos gerados"
	@echo "  make run    - Compila e executa o programa"
	@echo "  make help   - Mostra esta ajuda"

# Regra para verificar sintaxe sem compilar
check:
	$(CC) $(CFLAGS) -fsyntax-only $(SOURCES)

# Regra para mostrar informações do projeto
info:
	@echo "Projeto: Analisador Léxico"
	@echo "Arquivos fonte: $(SOURCES)"
	@echo "Executável: $(TARGET)"
	@echo "Compilador: $(CC)"
	@echo "Flags: $(CFLAGS)"

# Regra .PHONY para comandos que não geram arquivos
.PHONY: all clean run help check info
