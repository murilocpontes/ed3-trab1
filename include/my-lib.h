#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define DISK_SIZE 1600    // tamanho da p�gina de disco
#define DATA_OFFSET 160   // tamanho de um registro

typedef struct {
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
} data;

typedef struct {
    char status;
    int top;
    int nextRRN;
    int remRegNum;
    int discPageNum;
    int compQtt;
} header;

void initialize_header(header *hea);
void initialize_data(data *reg);
void print_file_data(data *file_data);
void free_data(data *d);
void write_record(FILE *file, data *reg);
void read_header(FILE *file, header *hea, char *trash);
void write_header(FILE *file, header *hea);
void read_csv_field(FILE *csv_file, int first, data *reg);
void read_csv(char **csv_filename, char **binary_filename);
void read_record(FILE *file, data *reg);
void search_records(char **filename, char *field, char *value);
void insert_record(char **binaryfilename, int n);
int define_status(FILE *file, char character);
int define_top(FILE *file, int top);
int define_nextRRN(FILE *file, int next);
int define_remRegNum(FILE *file, int removed);
int define_discPageNum(FILE *file, int pages);
int define_compQtt(FILE *file, int quantity);
int eof_verification(FILE *file);

int compare_records(data *reg1, data *reg2);
int record_exists(FILE *file, data *new_reg, header *hea);

void write_registry(FILE* file, data* registry);

void get_file_data(FILE *file, data *file_data);
void get_file_string(FILE *file, char** dest);

int verify_removed_register(FILE* file);