#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DISK_SIZE 1600
#define REGISTRY_OFFSET 160
#define HEADER_OFFSET 21 //5*int + 1

typedef struct{
    char removed;
    int linkingRRN;
    int population;
    float size;
    char speed_unit;
    int velocity;
    char* name;
    char* specie;
    char* habitat;
    char* type;
    char* diet;
    char* food;
    }data;

typedef struct{
    char status;
    int top;
    int nextRRN;
    int remRegNum;
    int diskPageNum;
    int compQtt;
    char* lixo;
}header;
// Function to print all registers on a file
void print_info(char* address);
// Function to get file info
void get_file_data(FILE* file, data* file_data);
void get_file_string(FILE* file, char** dest);
// Function to print a register data
void print_file_data(data* file_data);
// Funcion to verify EOF
int eof_verification(FILE* file);
// Function to free data pointers
void free_data(data* d);
// Funcion to search and delete registers based on field input data
void search_and_delete_record(char* address, int n);
// Function to verify if a register is logically removed
int verify_removed_register(FILE* file);
// Function to remove a register by a number input
void remove_by_number(FILE* file, int is_int, int offset);
// Function to remove a register by a string input
void remove_by_string(FILE* file, int position);
// Function to compress a file
void compress_file(char* address);
// Function to write compressed file header
void write_compressed_file_header(FILE* old_file, FILE* new_file);
// Function to write a registry in a file
void write_registry(FILE* file, data* registry);
