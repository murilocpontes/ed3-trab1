APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

# Lista de arquivos objeto
OBJS = $(OBJ)/funcoesFornecidas.o $(OBJ)/my-lib.o

# Flags de compilação com suporte ao GDB
CFLAGS = -g -I $(INCLUDE) 

all: $(BIN)/ex-pratico-1

# Regra para gerar o executável
$(BIN)/ex-pratico-1: $(OBJS) $(APPS)/main.c
	gcc $(CFLAGS) $(OBJS) $(APPS)/main.c -o $(BIN)/ex-pratico-1

# Regra para o arquivo objeto funcoesFornecidas.o
$(OBJ)/funcoesFornecidas.o: $(SRC)/funcoesFornecidas.c $(INCLUDE)/funcoesFornecidas.h
	gcc $(CFLAGS) -c $(SRC)/funcoesFornecidas.c -o $(OBJ)/funcoesFornecidas.o

# Regra para o arquivo objeto my-lib.o
$(OBJ)/my-lib.o: $(SRC)/my-lib.c $(INCLUDE)/my-lib.h
	gcc $(CFLAGS) -c $(SRC)/my-lib.c -o $(OBJ)/my-lib.o

# Regra para rodar o programa
run:
	$(BIN)/ex-pratico-1

	
# Regra para rodar no GDB
debug: $(BIN)/ex-pratico-1
	gdb $(BIN)/ex-pratico-1

# Limpar arquivos objeto e executáveis
clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/ex-pratico-1
