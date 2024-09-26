APPS = ./apps
BIN = ./bin
INCLUDE = ./include
OBJ = ./obj
SRC = ./src

OBJS = $(OBJ)/funcoesFornecidas.o $(OBJ)/sql.o $(OBJ)/archive.o

CFLAGS = -g -I $(INCLUDE) 

all: $(BIN)/trab1

$(BIN)/trab1: $(OBJS) $(APPS)/main.c
	gcc $(CFLAGS) $(OBJS) $(APPS)/main.c -o $(BIN)/trab1

$(OBJ)/funcoesFornecidas.o: $(SRC)/funcoesFornecidas.c $(INCLUDE)/funcoesFornecidas.h
	gcc $(CFLAGS) -c $(SRC)/funcoesFornecidas.c -o $(OBJ)/funcoesFornecidas.o

$(OBJ)/archive.o: $(SRC)/archive.c $(INCLUDE)/archive.h
	gcc $(CFLAGS) -c $(SRC)/archive.c -o $(OBJ)/archive.o

$(OBJ)/sql.o: $(SRC)/sql.c $(INCLUDE)/sql.h
	gcc $(CFLAGS) -c $(SRC)/sql.c -o $(OBJ)/sql.o


run:
	$(BIN)/trab1

debug: $(BIN)/trab1
	gdb $(BIN)/trab1

clean:
	rm -f $(OBJ)/*.o
	rm -f $(BIN)/trab1

