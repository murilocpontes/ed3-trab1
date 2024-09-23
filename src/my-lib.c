#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-lib.h"
#include "funcoesFornecidas.h"

void initialize_header(header *hea){ // fun��o para inicializar o registro de cabe�alho
    hea->status = '0';
    hea->top = -1;
    hea->nextRRN = 0;
    hea->remRegNum = 0;
    hea->discPageNum = 0;
    hea->compQtt = 0;
}

void initialize_data(data *reg){
    reg->name = (char *)malloc(30 * sizeof(char));
    reg->specie = (char *)malloc(30 * sizeof(char));
    reg->habitat = (char *)malloc(30 * sizeof(char));
    reg->type = (char *)malloc(30 * sizeof(char));
    reg->diet = (char *)malloc(30 * sizeof(char));
    reg->food = (char *)malloc(30 * sizeof(char));
}

void print_file_data(data *file_data){ // fun��o para imprimir os campos de um registro de dados
    if(file_data->removed == '0'){
        printf("Nome: %s\n",file_data->name);
        printf("Especie: %s\n",file_data->specie);
        if(strcmp(file_data->type, "") != 0)
            printf("Tipo: %s\n",file_data->type);
        printf("Dieta: %s\n",file_data->diet);
        if(strcmp(file_data->habitat, "") != 0)
            printf("Lugar que habitava: %s\n",file_data->habitat);
        if(file_data->size != -1){
            printf("Tamanho: %.1f m\n",file_data->size);
        }
        if(file_data->velocity != -1 && file_data->speed_unit != '$'){
            printf("Velocidade: %d %cm/h\n",file_data->velocity,file_data->speed_unit);
        }
        printf("\n");
    }
}

void free_data(data *d){ // fun��o para desalocar a mem�ria alocada dinamicamente
  free(d->name);
  free(d->specie);
  free(d->habitat);
  free(d->type);
  free(d->diet);
  free(d->food);
}

int define_status(FILE *file, char character){
    int n = ftell(file);                                      // salva o local do arquivo
    if(fseek(file,0,SEEK_SET) != 0) return (-1);              // vai para o local do status
    if(fwrite(&character,sizeof(char),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial de arquivo
    return 0;
}

int define_top(FILE *file, int top){
    int n = ftell(file);                                      // salva o local de arquivo
    if(fseek(file,1,SEEK_SET) != 0) return (-1);              // vai para o local de topo
    if(fwrite(&top,sizeof(int),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial do arquivo
    return 0;
}

int define_nextRRN(FILE *file, int next){
    int n = ftell(file);                                      // salva o local de arquivo
    if(fseek(file,5,SEEK_SET) != 0) return (-1);              // vai para o local de nextRRN
    if(fwrite(&next,sizeof(int),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial do arquivo
    return 0;
}

int define_remRegNum(FILE *file, int removed){
    int n = ftell(file);                                      // salva o local de arquivo
    if(fseek(file,9,SEEK_SET) != 0) return (-1);              // vai para o local do remRegNum
    if(fwrite(&removed,sizeof(int),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial do arquivo
    return 0;
}

int define_discPageNum(FILE *file, int pages){
    int n = ftell(file);                                      // salva o local de arquivo
    if(fseek(file,13,SEEK_SET) != 0) return (-1);              // vai para o local de discPageNum
    if(fwrite(&pages,sizeof(int),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial do arquivo
    return 0;
}

int define_compQtt(FILE *file, int quantity){
    int n = ftell(file);                                      // salva o local de arquivo
    if(fseek(file,17,SEEK_SET) != 0) return (-1);              // vai para o local de compQtt
    if(fwrite(&quantity,sizeof(int),1,file) <= 0) return (-1);
    if(fseek(file,n,SEEK_SET) != 0) return (-1);              // volta para o local inicial do arquivo
    return 0;
}

void write_record(FILE *file, data *reg){   // fun��o para escrever um registro de dados no arquivo bin�rio
    int x = 0, size = 18;          // inicialmente, size � o tamanho do registro de dados dos campos de tamanho fixo
    char trash = '$', delimiter = '#';

    // para os campos de tamanho fixo
    fwrite(&reg->removed, sizeof(char), 1, file);
    fwrite(&reg->linkingRRN, sizeof(int), 1, file);
    fwrite(&reg->population, sizeof(int), 1, file);
    fwrite(&reg->size,sizeof(float),1,file);
    fwrite(&reg->speed_unit,sizeof(char),1,file);
    fwrite(&reg->velocity,sizeof(int),1,file);


    // para campos de tamanho vari�vel
    x = strlen(reg->name);  // nome � n�o nulo
    fwrite(reg->name,sizeof(char),x,file);
    fwrite(&delimiter,sizeof(char),1,file); // delimitador
    size += ++x;

    x = strlen(reg->specie);   // especie � n�o nulo
    fwrite(reg->specie,sizeof(char),x,file);
    fwrite(&delimiter,sizeof(char),1,file);   // delimitador
    size += ++x;

    if(!strcmp(reg->habitat,"")){     // habitat pode ser nulo
        fwrite(&delimiter,sizeof(char),1,file);  // delimitador
        size++;
    } else {
        x = strlen(reg->habitat);
        fwrite(reg->habitat,sizeof(char),x,file);
        fwrite(&delimiter,sizeof(char),1,file);
        size += ++x;
    }

    if(!strcmp(reg->type,"")){  // tipo pode ser nulo
        fwrite(&delimiter,sizeof(char),1,file);  // delimitador
        size++;
    } else {
        x = strlen(reg->type);
        fwrite(reg->type,sizeof(char),x,file);
        fwrite(&delimiter,sizeof(char),1,file);
        size += ++x;
    }

    x = strlen(reg->diet);   // dieta � n�o nulo
    fwrite(reg->diet,sizeof(char),x,file);
    fwrite(&delimiter,sizeof(char),1,file);  // delimitador
    size += ++x;

    if(!strcmp(reg->food,"")){   // alimento pode ser nulo
        fwrite(&delimiter,sizeof(char),1,file); // delimitador
        size++;
    } else {
        x = strlen(reg->food);
        fwrite(reg->food,sizeof(char),x,file);
        fwrite(&delimiter,sizeof(char),1,file);
        size += ++x;
    }


    // preenchendo o restante do registro de dados com '$'
    size = DATA_OFFSET - size;

    for(; size != 0; size--){
        fwrite(&trash,sizeof(char),1,file);
    }
}

void read_header(FILE *file, header *hea, char *trash){  // fun��o para ler o registro de cabe�alho do arquivo bin�rio
    fseek(file,0,SEEK_SET);           //  ajusta o ponteiro "file" para a posi��o "0" a partir do inicio do arquivo �SEEK_SET�.

    fread(&hea->status, sizeof(char), 1, file);
    fread(&hea->top, sizeof(int), 1, file);
    fread(&hea->nextRRN, sizeof(int), 1, file);
    fread(&hea->remRegNum, sizeof(int), 1, file);
    fread(&hea->discPageNum, sizeof(int), 1, file);
    fread(&hea->compQtt, sizeof(int), 1, file);

    for(int i = 0; i < 1579; i++){
         fread(&trash[i], sizeof(char), 1, file);   // leitura do lixo
    }
}

void write_header(FILE *file, header *hea){  // fun��o para escrever o registro de cabe�alho no arquivo bin�rio

    char lixo = '$';                // caractere de lixo
    int n = DISK_SIZE;              // tamanho do cabe�alho � de uma p�gina de disco
    fseek(file, 0, SEEK_SET); // ajusta o ponteiro "file" para a posi��o "0" a partir do inicio do arquivo �SEEK_SET�.

    fwrite(&hea->status, sizeof(char), 1, file);
    fwrite(&hea->top, sizeof(int), 1, file);
    fwrite(&hea->nextRRN, sizeof(int), 1, file);
    fwrite(&hea->remRegNum, sizeof(int), 1, file);
    fwrite(&hea->discPageNum, sizeof(int), 1, file);
    fwrite(&hea->compQtt, sizeof(int), 1, file);

    for(n -= 21; n != 0; n--){
        fwrite(&lixo, sizeof(char),1, file);    // escreve os lixos no espa�o restante
    }
}

void read_csv_field(FILE *csv_file, int number, data *reg){  // fun��o que l� 10 campos do .csv, ou seja, uma linha e retorna um registro de dados lido

    int aux1 = -1;
    char aux2, campo[160];
    int len;

    for(int n = 0; n != 9; n++){
        aux1 = fscanf(csv_file,"%[^,]",campo);        // l� um campo at� encontrar uma v�rgula
        if(aux1 <= 0)                              // se campo lido esta vazio, j� que fscanf retorna o n�mero de itens lidos e atribuidos com sucesso
            strcpy(campo,"");
        fscanf(csv_file,"%c", &aux2);               // ignora a v�rgula de separa��o

        len = strlen(campo);   // verifica se o �ltimo caractere � '\r' e o substitui por '\0'
        if (len > 0 && campo[len - 1] == '\r') {
            campo[len - 1] = '\0';
        }

        if(!number){                              // se estiver lendo a primeira linha do csv, n�o salva
            if(n == 0){                         // nome - string - n�o pode assumir valores nulos
                strcpy(reg->name, campo);
            } else if(n == 1){                  // dieta - string - n�o pode assumir valores nulos
                strcpy(reg->diet, campo);
            } else if(n == 2){                  // habitat - string - pode assumir valores nulos
                strcpy(reg->habitat, campo);
            } else if(n == 3){                  // populacao - int  - pode assumir valores nulos
                if(!strcmp(campo,""))
                    reg->population = -1;     // se for NULO
                else
                    reg->population = atoi(campo);  // atoi() converte uma sequ�ncia de caracteres em um valor inteiro

            } else if(n == 4){                  // tipo - string - pode assumir valores nulos
                strcpy(reg->type, campo);
            } else if(n == 5){                  // velocidade - int - pode assumir valores nulos
                if(!strcmp(campo,""))
                    reg->velocity = -1;    // se for NULO
                else
                    reg->velocity = atoi(campo);  // atoi() converte uma sequ�ncia de caracteres em um valor inteiro

            } else if(n == 6){                  // unidadeMedida - char - pode assumir valores nulos
                if(!strcmp(campo,""))             // se for NULO
                    reg->speed_unit = '$';
                else
                    reg->speed_unit = campo[0];
            } else if(n == 7){                  // tamanho - float - pode assumir valores nulos
                if(!strcmp(campo,""))
                    reg->size = -1.0;    // caso seja NULO
                else
                    reg->size = atof(campo);     //  atof() converte sequ�ncia de caracteres em flutua��o

            } else if(n == 8){                  // especie - string - n�o pode assumir valores nulos
                strcpy(reg->specie, campo);
            }
        }
    }
    aux1 = fscanf(csv_file,"%[^\n]",campo);           // le o ultimo campo da linha que � terminado em \n
    if(aux1 <= 0)                                  // se campo lido esta vazio
        strcpy(campo,"");

    len = strlen(campo);       // verifica se o �ltimo caractere � '\r' e o substitui por '\0'
    if (len > 0 && campo[len - 1] == '\r') {
        campo[len - 1] = '\0';
    }

    fscanf(csv_file,"%c", &aux2);                   // ignora o \n de separacao

    if(!number){                                  // se estiver lendo a primeira linha do csv, n�o salva
        strcpy(reg->food, campo);          // alimento - string - pode assumir valores nulos
        reg->linkingRRN = -1;              // ainda nao ha um RRN de um pr�ximo registro de dados logicamente removido
        reg->removed = '0';                 // o registro de dados nao est� removido
    }
}

int eof_verification(FILE *file){
    int byte;
    if(fread(&byte,1,1,file) == 1){  // verifica se consegue ler um byte no arquivo
        fseek(file,-1,SEEK_CUR);
        return 0;
    }
    return 1;
}

void read_csv(char **csv_filename, char **binary_filename){ // funcionalidade 1: leitura de v�rios registros do csv

    FILE *csv_file = fopen(*csv_filename,"rt");
    if(!csv_file){
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    FILE *binary_file = fopen(*binary_filename,"wb");
    if(!binary_file){
        printf("Falha no processamento do arquivo.\n");
        fclose(csv_file);
        return;
    }

    long unsigned int counter = 0;
    int pages = 0, next_rrn = 0, aux;

    header hea;
    initialize_header(&hea);

    data reg;
    initialize_data(&reg);

    write_header(binary_file,&hea);

    read_csv_field(csv_file,1,&reg);  // ignora o cabe�alho do csv

    while(!eof_verification(csv_file)){
        read_csv_field(csv_file,0,&reg);
        write_record(binary_file,&reg);
        counter += DATA_OFFSET;
    }

    next_rrn = (counter/DATA_OFFSET);
    counter += DISK_SIZE;

    aux = counter % DISK_SIZE;
    pages = ((aux == 0) ? (counter / DISK_SIZE) : ((int)(counter / DISK_SIZE) + 1));

    define_status(binary_file,'1');
    define_nextRRN(binary_file,next_rrn);
    define_discPageNum(binary_file,pages);

    fclose(csv_file);
    fclose(binary_file);

    binarioNaTela(*binary_filename);
    free(*binary_filename);
    *binary_filename = NULL;

    free_data(&reg);
}

void read_record(FILE *file, data *reg){  // fun��o para ler o registro de dados do arquivo bin�rio

    char aux1[145], aux2[100];
    int n = 0;                                   // quantos bytes est�o sendo lidos

    // para os campos de tamanho fixo
    fread(&reg->removed,sizeof(char),1,file);
    fread(&reg->linkingRRN,sizeof(int),1,file);
    fread(&reg->population,sizeof(int),1,file);
    fread(&reg->size,sizeof(float),1,file);
    fread(&reg->speed_unit,sizeof(char),1,file);
    fread(&reg->velocity,sizeof(int),1,file);


    // para os campos de tamanho vari�vel
    fread(&aux1,sizeof(char),142,file);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo nome
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->name, aux2);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo especie
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->specie, aux2);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo habitat
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->habitat, aux2);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo tipo
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->type, aux2);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo dieta
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->diet, aux2);


    if(sscanf(&aux1[n],"%[^#]",aux2) == 0)          // le o campo alimento
        strcpy(aux2,"");
    if(!strcmp(aux2,""))                         // campo est� vazio
        n++;
    else
        n += (strlen(aux2) + 1);                // vai para o pr�ximo campo
    strcpy(reg->food, aux2);
}

void search_records(char **filename, char *field, char *value){ // funcionalidade 3: busca
    FILE *file;
    if(*filename == NULL || !(file = fopen(*filename,"rb"))){  // abre o file no modo leitura bin�ria
        printf("Falha no processamento do arquivo.\n");
        return;
    }

    header hea;
    initialize_header(&hea);
    char *trash = (char *) malloc(1579 * sizeof(char));
    read_header(file,&hea,trash);

    data reg;
    initialize_data(&reg);

    int k = 0;
    int pages_accessed = 1;    // cabe�alho est� na primeira p�gina
    int records_per_page = DISK_SIZE / DATA_OFFSET;   // 1600 / 160 = 10   -->> n�mero de registros de dados por p�gina de disco
    int record_counter = 0;

    while (!eof_verification(file)) {
        read_record(file,&reg);

        record_counter++;
        if(record_counter > records_per_page){
            pages_accessed++;
            record_counter = 1;
        }

        if (reg.removed == '0') {
            int match = 0;    // corresponde

            if(strcmp(field, "nome") == 0 && strcmp(reg.name, value) == 0){
                match = 1;
            } else if(strcmp(field, "especie") == 0 && strcmp(reg.specie, value) == 0){
                match = 1;
            } else if(strcmp(field, "habitat") == 0 && strcmp(reg.habitat, value) == 0){
                match = 1;
            } else if(strcmp(field, "tipo") == 0 && strcmp(reg.type, value) == 0){
                match = 1;
            } else if(strcmp(field, "dieta") == 0 && strcmp(reg.diet, value) == 0){
                match = 1;
            } else if(strcmp(field, "alimento") == 0 && strcmp(reg.food, value) == 0){
                match = 1;
            } else if(strcmp(field, "populacao") == 0 && reg.population == atoi(value)){  // atoi() converte uma sequ�ncia de caracteres em um valor inteiro
                match = 1;
            } else if(strcmp(field, "tamanho") == 0 && reg.size == atof(value)){  // atof() converte sequ�ncia de caracteres em flutua��o
                match = 1;
            } else if(strcmp(field, "velocidade") == 0 && reg.velocity == atoi(value)){
                match = 1;
            } else if(strcmp(field, "unidadeMedida") == 0 && strcmp(&reg.speed_unit, value) == 0){
                match = 1;
            } else if(strcmp(field, "encadeamento") == 0 && reg.linkingRRN == atoi(value)){
                match = 1;
            }
            if (match) {
                print_file_data(&reg);
                k = 1;
            }
        }
    }

    if(!k){
        printf("Registro inexistente.\n\n");
    }

    pages_accessed++;

    printf("Numero de paginas de disco: %d\n\n",pages_accessed);

    fclose(file);
    free_data(&reg);
}

void insert_record(char **binaryfilename, int n){     // funcionalidade 5: inserir novos registros de dados em um arquivo
    FILE *binary_file;
    if(*binaryfilename == NULL || !(binary_file = fopen(*binaryfilename,"rb+"))){  // abre o file no modo leitura e escrita bin�ria
        printf("Falha no processamento do arquivo.\n");
        free(*binaryfilename);
        *binaryfilename = NULL;
        return;
    }

    int next_rrn, pages;

    header hea;
    initialize_header(&hea); // inicializa o cabe�alho
    char *trash = (char *) malloc(1579 * sizeof(char));
    read_header(binary_file,&hea,trash);   // l� o cabe�alho do arquivo

    data new_reg;
    initialize_data(&new_reg);                                           // novo registro


    char population_buffer[30];
    char velocity_buffer[30];
    char speed_unit_buffer[30];
    char size_buffer[30];

    for(int i = 0; i < n; i++){
        scan_quote_string(new_reg.name);

        scan_quote_string(new_reg.diet);


        scan_quote_string(new_reg.habitat);


        //scanf(" %s",population_buffer);
        scan_quote_string(population_buffer);
        if(strcmp(population_buffer, "") == 0){
            new_reg.population = -1;
        } else {
            new_reg.population = atoi(population_buffer);   // atoi() converte uma sequ�ncia de caracteres em um valor inteiro
        }

        scan_quote_string(new_reg.type);

        //scanf(" %s",velocity_buffer);
        scan_quote_string(velocity_buffer);
        if(strcmp(velocity_buffer, "") == 0){
            new_reg.velocity = -1;
        } else {
            new_reg.velocity = atoi(velocity_buffer);       // atoi() converte uma sequ�ncia de caracteres em um valor inteiro
        }

        //scanf(" %s",speed_unit_buffer);
        scan_quote_string(speed_unit_buffer);
        if(strcmp(speed_unit_buffer, "") == 0){
            new_reg.speed_unit = '$';
        } else {
            new_reg.speed_unit = speed_unit_buffer[0];
        }

        //scanf(" %s",size_buffer);
        scan_quote_string(size_buffer);
        if(strcmp(size_buffer, "") == 0){
            new_reg.size = -1;
        } else {
            new_reg.size = atof(size_buffer);       // atof() converte sequ�ncia de caracteres em flutua��o
        }

        scan_quote_string(new_reg.specie);

        scan_quote_string(new_reg.food);

         // verifica se o registro j� existe
        if (record_exists(binary_file, &new_reg, &hea)) {
            continue;                               // pula para o pr�ximo registro (pr�xima itera��o)
        }


        // sizeof(data)
        // sizeof(header)
        if(hea.top == -1){        // hea.top == -1 : n�o h� registros logicamente removidos             insere no final do arquivo
            fseek(binary_file, 0, SEEK_END); // ajusta o ponteiro "bynary_file" para a posi��o "0" a partir do fim do arquivo �SEEK_END�.
            hea.nextRRN++;          // incrementa o pr�ximo RRN
            define_nextRRN(binary_file, hea.nextRRN);  // atualiza o pr�ximo RRN no cabe�alho
        } else {      // se h� registros logicamente removidos
            fseek(binary_file, DISK_SIZE + hea.top * DATA_OFFSET + 1, SEEK_SET); // posiciona no topo da pilha   // ajusta o ponteiro "bynary_file" para a posi��o "sizeof(header) + hea.top * sizeof(data)" a partir do inicio do arquivo �SEEK_SET�.
            fread(&hea.top, sizeof(int), 1, binary_file);
            fseek(binary_file, -(1+sizeof(int)), SEEK_CUR);
            hea.remRegNum--;
        }

        write_registry(binary_file, &new_reg);  // escreve o novo registro

        if(hea.nextRRN % 10 == 0){
            hea.discPageNum++;
        }
        define_top(binary_file, hea.top);
        define_status(binary_file,'1');   // defini o status do arquivo como consistente
        define_discPageNum(binary_file, hea.discPageNum);   // atualiza o n�mero de p�ginas de disco
        define_remRegNum(binary_file, hea.remRegNum);
    }

    free_data(&new_reg);   // desaloca a mem�ria para as strings dinamicamente alocadas     // desaloca a mem�ria para as strings dinamicamente alocadas
    fclose(binary_file);
    binarioNaTela(*binaryfilename);
    free(*binaryfilename);
    *binaryfilename = NULL;
}

int compare_records(data *reg1, data *reg2){    // compara todos os campos do registro para verificar se s�o iguais
    return strcmp(reg1->name, reg2->name) == 0 &&
           strcmp(reg1->specie, reg2->specie) == 0 &&
           strcmp(reg1->habitat, reg2->habitat) == 0 &&
           strcmp(reg1->type, reg2->type) == 0 &&
           strcmp(reg1->diet, reg2->diet) == 0 &&
           strcmp(reg1->food, reg2->food) == 0 &&
           reg1->population == reg2->population &&
           reg1->size == reg2->size &&
           reg1->velocity == reg2->velocity &&
           reg1->speed_unit == reg2->speed_unit;
}

int record_exists(FILE *file, data *new_reg, header *hea){    // v� se o registro que quer inserir j� existe
    data existing_reg;
    int rrn = 0;

    fseek(file, DISK_SIZE, SEEK_SET); // vai para o in�cio dos registros de dados
    while(!eof_verification(file)){
        if(verify_removed_register(file)){
            rrn++;
            fseek(file, DISK_SIZE + rrn*DATA_OFFSET, SEEK_SET);
            continue;
        }
        fseek(file, sizeof(int), SEEK_CUR);
        get_file_data(file, &existing_reg);
        if(compare_records(new_reg, &existing_reg)){
            free_data(&existing_reg);
            return 1;
        }
        rrn++;
        free_data(&existing_reg);
        fseek(file, DISK_SIZE + rrn * DATA_OFFSET, SEEK_SET);
    }
    return 0; // Nenhum registro duplicado encontrado
}


void write_registry(FILE* file, data* registry){
    int total_size = ftell(file); 
    registry->removed = '0';
    registry->linkingRRN = -1;
    char delim = '#';
    char trash[DATA_OFFSET];
    // writing registry data, and updating total_size with written strings size
    fwrite(&registry->removed, 1, 1, file);
    fwrite(&registry->linkingRRN, sizeof(int),1 , file);
    fwrite(&registry->population, sizeof(int), 1, file);
    fwrite(&registry->size, sizeof(float), 1, file);
    fwrite(&registry->speed_unit, sizeof(char), 1, file);
    fwrite(&registry->velocity, sizeof(int), 1, file);
    fwrite(registry->name, sizeof(char), strlen(registry->name), file);
    fwrite(&delim, sizeof(char), 1, file);
    fwrite(registry->specie, sizeof(char), strlen(registry->specie), file);
    fwrite(&delim, sizeof(char), 1, file);
    fwrite(registry->habitat, sizeof(char), strlen(registry->habitat), file);
    fwrite(&delim, sizeof(char), 1, file);
    fwrite(registry->type, sizeof(char), strlen(registry->type), file);
    fwrite(&delim, sizeof(char), 1, file);
    fwrite(registry->diet, sizeof(char), strlen(registry->diet), file);
    fwrite(&delim, sizeof(char), 1, file);
    fwrite(registry->food, sizeof(char), strlen(registry->food), file);
    fwrite(&delim, sizeof(char), 1, file);
    // setting trash string with remaining registry size and writing
    total_size = ftell(file) - total_size;
    memset(trash, '$', DATA_OFFSET - total_size);
    trash[DATA_OFFSET - total_size] = '\0';
    fwrite(trash, sizeof(char), DATA_OFFSET - total_size, file);
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