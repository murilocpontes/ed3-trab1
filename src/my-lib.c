// ainda n testei pq n fiz o main XD
#include "my-lib.h"
#include "funcoesFornecidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_info(char *address){
    FILE *file = fopen(address, "rb+");
    data file_data;
    char tmp[2];
    int disk_page_num;
    int is_deleted, status;
    int flag = 0;
    if (file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    fgets(tmp, 2, file);
    status = atoi(tmp);
    if (!status){
        printf("Falha no processamento do arquivo\n");
		return;
	}
	fseek(file, - 1, SEEK_CUR);
    fputc('0', file);
    fseek(file, 3 * sizeof(int), SEEK_CUR);
    fread(&disk_page_num, sizeof(int), 1, file);
    fseek(file, DISK_SIZE - (1 + 4 * sizeof(int)), SEEK_CUR);
    while (!eof_verification(file)){
        is_deleted = atoi(fgetc(file));
        if (is_deleted){
            fseek(file,  - 1, SEEK_CUR);
            continue;}get_file_data(file, &file_data);
        print_file_data(&file_data);
        free_data(&file_data);
        flag++;
    }
    fseek(file, 0, SEEK_SET);
    fputc('1', file);
    fclose(file);

    if (flag == 0){
        printf("Registro inexistente.\n");
        return;
    }

    printf("Numero de paginas de disco: %d\n\n", disk_page_num);
}

void get_file_data(FILE *file, data *file_data){
    fread(&file_data->population, sizeof(int), 1, file);
    fread(&file_data->size, sizeof(float), 1, file);
    get_file_string(file, file_data->speed_unit);
    fread(&file_data->velocity, sizeof(float), 1, file);
    get_file_string(file, file_data->name);
    get_file_string(file, file_data->specie);
    get_file_string(file, file_data->habitat);
    get_file_string(file, file_data->type);
    get_file_string(file, file_data->diet);
    get_file_string(file, file_data->food);
}

void get_file_string(FILE *file, char *dest){
    int size = 1;
    char tmp;
    tmp = fgetc(file);
    while (tmp != '#' && !eof_verification(file)){
        fgetc(file);
        size++;
    }
    fseek(file, -size, SEEK_CUR);
    dest = (char*)malloc(1 + size);
    fread(dest, 1, size, file);
    dest[size] = '\0';
}

void print_file_data(data *file_data){
    printf("Nome: %s\n", file_data->name);
    printf("Especie: %s\n", file_data->specie);
    printf("Tipo: %s\n", file_data->type);
    printf("Dieta: %s\n", file_data->diet);
    printf("Lugar que habitava: %s", file_data->habitat);
    if (file_data->population != -1)
        printf("População: %d\n", file_data->population);
    if (file_data->size != -1)
        printf("Tamanho: %.2f\n", file_data->size);
    if (file_data->velocity != -1 && strcmp(file_data->speed_unit, "#"))
        printf("Velocidade: %d %s/h\n", file_data->population,
               file_data->speed_unit);
    printf("\n");
}

int eof_verification(FILE *file){
    int is_deleted;
    if (fread(&is_deleted, 1, 1, file) == 0) // verifica fim do arquivo
        return 1;

    fseek(file, -1, SEEK_CUR);
    return 0;
}

void free_data(data *d){
    free(d->name);
    free(d->specie);
    free(d->habitat);
    free(d->type);
    free(d->diet);
    free(d->food);
}

void search_and_delete_record(char* address, int n){
	FILE* file = fopen(address, "rb+");
	int status;
    char tmp[2];
    char data_type[20];
	if (file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    fgets(tmp, 2, file);
    status = atoi(tmp);
    if (!status){
        printf("Falha no processamento do arquivo\n");
		return;
	}
    fseek(file, - 1, SEEK_CUR);
    fputc('0', file);
	for(int i =0; i < n; i++){
		fseek(file, DISK_SIZE, SEEK_SET);
        scanf("%s", data_type);
        if(strcmp(data_type, "populacao") == 0)
            remove_by_number(file, 1, 8);
        
        else if(strcmp(data_type, "tamanho") == 0)
            remove_by_number(file, 1, 12);
        
        else if(strcmp(data_type, "velocidade") == 0)
            remove_by_number(file, 0, 17);
        
        else if(strcmp(data_type, "nome") == 0)
            remove_by_string(file, 0);

        else if(strcmp(data_type, "especie") == 0)
            remove_by_string(file, 1);

        else if(strcmp(data_type, "habitat") == 0)
            remove_by_string(file, 2);

        else if(strcmp(data_type, "tipo") == 0)
            remove_by_string(file, 3);

        else if(strcmp(data_type, "dieta") == 0)
            remove_by_string(file, 4);

        else if(strcmp(data_type, "alimento") == 0)
            remove_by_string(file, 5);
	}
}

int verify_removed_register(FILE* file){
    int flag = 0;
    flag = atoi(fgetc(file));
    if(flag)
        return 1;
    return 0;
}

void remove_by_number(FILE* file, int is_int, int offset){
    void* read_data;
    void* file_data;
    size_t data_size = is_int ? sizeof(int) : sizeof(float);
    
    if(is_int) {
        read_data = malloc(sizeof(int));
        scanf("%d", (int*)read_data);
    } else {
        read_data = malloc(sizeof(float));
        scanf("%f", (float*)read_data);
    }

    file_data = malloc(data_size);

    while(!eof_verification(file)){
        if (verify_removed_register(file)){
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }

        fseek(file, offset - data_size, SEEK_CUR);
        fread(file_data, data_size, 1, file);

        if (is_int ? (*(int*)file_data == *(int*)read_data) : (*(float*)file_data == *(float*)read_data)) {
            fseek(file, -(offset + 1), SEEK_CUR);
            fputc('1', file);
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }

        fseek(file, REGISTRY_OFFSET - (offset + 1), SEEK_CUR);
    }

    free(read_data);
    free(file_data);
}

void remove_by_string(FILE* file, int position){
    char* file_data;
    char read_data[30];
    int offset;
    scan_quote_string(read_data);
    while(!eof_verification(file)){
        if(verify_removed_register(file)){
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }
        fseek(file, 17, SEEK_CUR);
        offset = 0;
        for(int i = 0, j = 0; i < position; i++){
            char tmp = fgetc(file);
            while(tmp != '#' && !eof_verification(file))
                j++;
            offset += j;
        }
        get_file_string(file, file_data);
        int len_offset = strlen(file_data);
        if(strcmp(file_data, read_data) == 0){
            fseek(file, -(18 + offset + len_offset), SEEK_CUR);
            free(file_data);
            fputc('1', file);
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }        
        free(file_data);
        fseek(file, REGISTRY_OFFSET -(18 + offset + len_offset), SEEK_CUR);
    }
}
