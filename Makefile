APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

# Lista de arquivos objeto
OBJS = $(OBJ)/funcoesFornecidas.o $(OBJ)/sql.o $(OBJ)/archive.o

# Flags de compilação com suporte ao GDB
CFLAGS = -g -I $(INCLUDE) 

all: $(BIN)/trab1

# Regra para gerar o executável
$(BIN)/trab1: $(OBJS) $(APPS)/main.c
	gcc $(CFLAGS) $(OBJS) $(APPS)/main.c -o $(BIN)/trab1

# Regra para o arquivo objeto funcoesFornecidas.o
$(OBJ)/funcoesFornecidas.o: $(SRC)/funcoesFornecidas.c $(INCLUDE)/funcoesFornecidas.h
	gcc $(CFLAGS) -c $(SRC)/funcoesFornecidas.c -o $(OBJ)/funcoesFornecidas.o

# Regra para o arquivo objeto archive.o
$(OBJ)/archive.o: $(SRC)/archive.c $(INCLUDE)/archive.h
	gcc $(CFLAGS) -c $(SRC)/archive.c -o $(OBJ)/archive.o

# Regra para o arquivo objeto sql.o
$(OBJ)/sql.o: $(SRC)/sql.c $(INCLUDE)/sql.h
	gcc $(CFLAGS) -c $(SRC)/sql.c -o $(OBJ)/sql.o


# Regra para rodar o programa
run:
	$(BIN)/trab1

	
# Regra para rodar no GDB
debug: $(BIN)/trab1
	gdb $(BIN)/trab1

# Limpar arquivos objeto e executáveis
clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/trab1

