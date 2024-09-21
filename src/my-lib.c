#include "my-lib.h"
#include "funcoesFornecidas.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_info(char *address){
    FILE *file = fopen(address, "rb+");
    data file_data;
    char tmp[2];
    int disk_page_num, is_deleted, status, flag = 0, i = 0;

    if (file == NULL){
        printf("Falha no processamento do arquivo\n");
        return;
    }
    // Validating file condition and setting '0' while reading file info
    fgets(tmp, 2, file);
    status = atoi(tmp);
    if (!status){
        printf("Falha no processamento do arquivo\n");
		return;
	}
	fseek(file, - 1, SEEK_CUR);
    fputc('0', file);
    // Reading file disk page number
    fseek(file, 3 * sizeof(int), SEEK_CUR);
    fread(&disk_page_num, sizeof(int), 1, file);
    // Seeking to first register, getting file data and printing
    fseek(file, DISK_SIZE, SEEK_SET);
    while (!eof_verification(file)){ 
        if (verify_removed_register(file)){ // skip if is a removed register
            i++;
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }
        fseek(file, sizeof(int), SEEK_CUR);
        get_file_data(file, &file_data);
        print_file_data(&file_data);
        free_data(&file_data);
        i++;
        fseek(file, DISK_SIZE + i*REGISTRY_OFFSET, SEEK_SET); // fseek to next register
        flag++;
    }
    // Restore file condition and closing file
    fseek(file, 0, SEEK_SET);
    fputc('1', file);
    fclose(file);
    // flag == 0 means all registers were removed
    if (flag == 0){
        printf("Registro inexistente.\n");
        return;
    }

    printf("Numero de paginas de disco: %d\n\n", disk_page_num);
}

void get_file_data(FILE *file, data *file_data){
    // After skiping removed and linking variables, reads file info
    fread(&file_data->population, sizeof(int), 1, file);
    fread(&file_data->size, sizeof(float), 1, file);
    fread(&file_data->speed_unit, sizeof(char), 1, file);
    fread(&file_data->velocity, sizeof(int), 1, file);
    get_file_string(file, &file_data->name);
    get_file_string(file, &file_data->specie);
    get_file_string(file, &file_data->habitat);
    get_file_string(file, &file_data->type);
    get_file_string(file, &file_data->diet);
    get_file_string(file, &file_data->food);
}

void get_file_string(FILE *file, char** dest){
    int size = 1;
    char tmp;
    tmp = fgetc(file);
    // Reads until separator or file end to find end of string
    while (tmp != '#' && !eof_verification(file)){
        tmp = fgetc(file);
        size++;
    }
    // Back to string beginning and actually reading it, in allocated dest string, setting '\0' instead of ''#'
    fseek(file, -size, SEEK_CUR);
    *dest = (char*)malloc(size);
    fread(*dest, 1, size, file);
    (*dest)[size - 1] = '\0';
}

void print_file_data(data *file_data){
    // Printing all collected info (population if needed)
    printf("Nome: %s\n", file_data->name);
    printf("Especie: %s\n", file_data->specie);
    if(strcmp(file_data->type, "") != 0)
        printf("Tipo: %s\n", file_data->type);
    printf("Dieta: %s\n", file_data->diet);
    if(strcmp(file_data->habitat, "") != 0)
    printf("Lugar que habitava: %s\n", file_data->habitat);
    // if (file_data->population != -1)
    //     printf("População: %d\n", file_data->population);
    if (file_data->size != -1)
        printf("Tamanho: %.1f m\n", file_data->size);
    if (file_data->velocity != -1 && file_data->speed_unit != '$')
        printf("Velocidade: %d %cm/h\n", file_data->velocity,
               file_data->speed_unit);
    printf("\n");
}

int eof_verification(FILE *file){
    // Verifies if fread can read a byte
    int byte;
    if (fread(&byte, 1, 1, file) == 1){
        fseek(file, -1, SEEK_CUR);
        return 0;
    } 
    // Else, test if is end of file
    return feof(file)? 1 : 0;
}

void free_data(data *d){
    // Free the dynamic data strings
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
    // Validating file status and updating while is open
    fgets(tmp, 2, file);
    status = atoi(tmp);
    if (!status){
        printf("Falha no processamento do arquivo\n");
		return;
	}
    fseek(file, - 1, SEEK_CUR);
    fputc('0', file);
    // setting how to remove registers in file based on user entry n times
	for(int i =0; i < n; i++){
		fseek(file, 1, SEEK_SET);
        scanf(" %s", data_type);
        if(strcmp(data_type, "populacao") == 0)
            remove_by_number(file, 1, 4);
        
        else if(strcmp(data_type, "tamanho") == 0)
            remove_by_number(file, 0, 8);
        
        else if(strcmp(data_type, "velocidade") == 0)
            remove_by_number(file, 1, 13);
        
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
    fseek(file, 0, SEEK_SET);
    fputc('1', file);
    fclose(file);
}

int verify_removed_register(FILE* file){
    // Reads "removed" registry data and returns if it is removed or not
    int flag = 0;
    char tmp[2];
    fgets(tmp, 2, file);
    flag = atoi(tmp);
    if(flag == 1)
        return 1;
    return 0;
}

// is_int defines if the number to be removed is interger or float
// offset is the number of bytes to be skipped to reach the desired field
void remove_by_number(FILE* file, int is_int, int offset){
    void* read_data;
    void* file_data;
    int top, nroRegRem, rrn = 0;
    // setting data type based on is_int
    size_t data_size = is_int ? sizeof(int) : sizeof(float);
    // setting trash string to overwrite removed register data
    char trash[REGISTRY_OFFSET-4];
    memset(trash, '$', REGISTRY_OFFSET-5);
    trash[REGISTRY_OFFSET-5] = '\0';
    // collecting value to delete matching registers
    if(is_int) {
        read_data = malloc(sizeof(int));
        scanf("%d", (int*)read_data);
    } else {
        read_data = malloc(sizeof(float));
        scanf("%f", (float*)read_data);
    }
    // setting file_data size equal to read_data size
    file_data = malloc(data_size);
    // getting top and number of removed register data from file
    fread(&top, sizeof(int), 1, file);
    fseek(file, sizeof(int), SEEK_CUR);
    fread(&nroRegRem, sizeof(int), 1, file);
    // until end of file, verify if file_data is equal to read_data
    while(!eof_verification(file)){
        fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
        if (verify_removed_register(file)){
            rrn++;
            fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
            continue;
        }

        fseek(file, offset, SEEK_CUR);
        fread(file_data, data_size, 1, file);
        // if int -> compares data as int : compares data as float
        if (is_int ? (*(int*)file_data == *(int*)read_data) : (*(float*)file_data == *(float*)read_data)) {
            // Returning to the register beggining, setting as removed and adding to the linked stack list
            fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
            fputc('1', file);
            fwrite(&top, sizeof(int), 1, file);
            int ret = fwrite(&trash, sizeof(char), REGISTRY_OFFSET - 5, file);
            fseek(file, 1, SEEK_SET);
            fwrite(&rrn, sizeof(int), 1, file);
            top = rrn;
            nroRegRem++;
            rrn++;
            fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
            continue;
        }
        rrn++;
        fseek(file,DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
    }
    // After removing all registers that match the read_data, update the removed register number and list top
    fseek(file, 1, SEEK_SET);
    fwrite(&top, sizeof(int), 1, file);
    fseek(file, 1 + 2*sizeof(int), SEEK_SET);
    fwrite(&nroRegRem, sizeof(int), 1, file);

    free(read_data);
    free(file_data);
}

// position is the number of the string in the sequence (name, specie, ...)
void remove_by_string(FILE* file, int position){
    char* file_data;
    char read_data[30];
    int nroRegRem, top, rrn = 0;
    // setting trash string to overwrite registry data
    char trash[REGISTRY_OFFSET-4];
    memset(trash, '$', REGISTRY_OFFSET-5);
    trash[REGISTRY_OFFSET-5] = '\0';
    // getting top of linked list and the number of deleted registers
    fread(&top, sizeof(int), 1, file);
    fseek(file, sizeof(int), SEEK_CUR);
    fread(&nroRegRem, sizeof(int), 1, file);
    // reading string to search match registers
    scan_quote_string(read_data);
    // until end of file, search match registers and delete them
    while(!eof_verification(file)){
        fseek(file, DISK_SIZE + rrn*REGISTRY_OFFSET, SEEK_SET);
        if(verify_removed_register(file)){
            rrn++;
            fseek(file, REGISTRY_OFFSET - 1, SEEK_CUR);
            continue;
        }
        // skip fixed size fields
        fseek(file, 17, SEEK_CUR);
        // skip field strings written before desired field
        for(int i = 0; i < position; i++){
            char tmp = fgetc(file);
            while(tmp != '#' && !eof_verification(file))
                tmp = fgetc(file);
        }
        get_file_string(file, &file_data);
        // if equal, set as removed, overwrite data and add to the linked list
        if(strcmp(file_data, read_data) == 0){
            fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
            free(file_data);
            fputc('1', file);
            fwrite(&top, sizeof(int), 1, file);
            fwrite(&trash, sizeof(char), REGISTRY_OFFSET - 5, file);
            fseek(file, 1, SEEK_SET);
            fwrite(&rrn, sizeof(int), 1, file);
            top = rrn;
            nroRegRem++;
            rrn++;
            // skip to next register
            fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
            continue;
        }        
        // skip to next register
        free(file_data);
        rrn++;
        fseek(file, DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
    }
    // after reaching eof, update number of deleted registers and list top
    fseek(file, 1, SEEK_SET);
    fwrite(&top, sizeof(int), 1, file);
    fseek(file, 1 + 2*sizeof(int), SEEK_SET);
    fwrite(&nroRegRem, sizeof(int), 1, file);
}

void compress_file(char* address){
    FILE* new_file = fopen("tmp", "wb");
    FILE* old_file = fopen(address, "rb");
    char tmp[2];
    int status, is_deleted, rrn = 0, qttPagDisk = 0, remRegNum = 0;
    data registry;
    
    if(old_file == NULL){
        printf("Falha no processamento do arquivo\n");
        fclose(new_file);
        return;
    }
    // Verifies file status and set to 0 while open
    fgets(tmp, 2, old_file);
    status = atoi(tmp);

    if(!status){
        printf("Falha no processamento do arquivo\n");
        fclose(new_file);
        fclose(old_file);
        return;
    }

    fseek(old_file, - 1, SEEK_CUR);
    fputc('0', old_file);
    // writing compressed_file_header
    write_compressed_file_header(old_file, new_file);
    // skipping to first register
    fseek(old_file, DISK_SIZE, SEEK_SET);
    // until end of file, write register on new file if not removed
    while(!eof_verification(old_file)){
        if(verify_removed_register(old_file)){
            fseek(old_file, REGISTRY_OFFSET - 1, SEEK_CUR);  
            remRegNum++;
        }
        else{
            fseek(old_file, sizeof(int), SEEK_CUR);
            get_file_data(old_file, &registry);
            write_registry(new_file, &registry);
            free_data(&registry);
        }
        rrn++;
        fseek(old_file,DISK_SIZE + rrn * REGISTRY_OFFSET, SEEK_SET);
    }
    // calculating new quantity of disks occupied
    if((rrn - remRegNum) % 10 == 0)
        qttPagDisk = (rrn - remRegNum)/10;
    else
        qttPagDisk = ((rrn - remRegNum)/10) + 1;
    // calculating next_rrn
    rrn = rrn - remRegNum + 1;
    // setting new values on file header
    fseek(new_file, 1+sizeof(int), SEEK_SET);
    fwrite(&rrn, sizeof(int), 1, new_file);
    fseek(new_file, sizeof(int), SEEK_CUR);
    fwrite(&qttPagDisk, sizeof(int), 1, new_file);  
    fclose(old_file);
    fclose(new_file);
    // after closing, remove old file and rename new file
    remove(address);
    rename("tmp", address);
}

void write_compressed_file_header(FILE* old_file, FILE* new_file){
    int compQtt;
    header compressed_header;
    // setting trash string
    char trash[DISK_SIZE - HEADER_OFFSET + 1];
    memset(trash, '$', DISK_SIZE - HEADER_OFFSET);
    trash[DISK_SIZE - HEADER_OFFSET] = '\0';
    // getting compQtt from old_file
    fseek(old_file, 1 + 4*sizeof(int), SEEK_CUR);
    fread(&compQtt, sizeof(int), 1, old_file);
    // setting new header fields 
    compressed_header.status = '1';
    compressed_header.top = -1;
    compressed_header.nextRRN = 0;
    compressed_header.remRegNum = 0;
    compressed_header.diskPageNum = 0;
    compressed_header.compQtt = compQtt +1;
    // writing header on new_file 
    fwrite(&compressed_header.status, sizeof(char), 1, new_file);
    fwrite(&compressed_header.top, sizeof(int), 1, new_file);
    fwrite(&compressed_header.nextRRN, sizeof(int), 1, new_file);
    fwrite(&compressed_header.remRegNum, sizeof(int), 1, new_file);
    fwrite(&compressed_header.diskPageNum, sizeof(int), 1, new_file);
    fwrite(&compressed_header.compQtt, sizeof(int), 1, new_file);
    fwrite(&trash, sizeof(char), DISK_SIZE - HEADER_OFFSET, new_file);
}

void write_registry(FILE* file, data* registry){
    int total_size = 24; // fixed field size + separators
    registry->removed = '0';
    registry->linkingRRN = -1;
    char delim = '#';
    char trash[REGISTRY_OFFSET];
    // writing registry data, and updating total_size with written strings size
    fwrite(&registry->removed, 1, 1, file);
    fwrite(&registry->linkingRRN, sizeof(int),1 , file);
    fwrite(&registry->population, sizeof(int), 1, file);
    fwrite(&registry->size, sizeof(float), 1, file);
    fwrite(&registry->speed_unit, sizeof(char), 1, file);
    fwrite(&registry->velocity, sizeof(int), 1, file);
    total_size += fwrite(registry->name, sizeof(char), strlen(registry->name), file);
    fwrite(&delim, sizeof(char), 1, file);
    total_size += fwrite(registry->specie, sizeof(char), strlen(registry->specie), file);
    fwrite(&delim, sizeof(char), 1, file);
    total_size += fwrite(registry->habitat, sizeof(char), strlen(registry->habitat), file);
    fwrite(&delim, sizeof(char), 1, file);
    total_size += fwrite(registry->type, sizeof(char), strlen(registry->type), file);
    fwrite(&delim, sizeof(char), 1, file);
    total_size += fwrite(registry->diet, sizeof(char), strlen(registry->diet), file);
    fwrite(&delim, sizeof(char), 1, file);
    total_size += fwrite(registry->food, sizeof(char), strlen(registry->food), file);
    fwrite(&delim, sizeof(char), 1, file);
    // setting trash string with remaining registry size and writing
    memset(trash, '$', REGISTRY_OFFSET - total_size);
    trash[REGISTRY_OFFSET-total_size] = '\0';
    fwrite(trash, sizeof(char), REGISTRY_OFFSET - total_size, file);
}