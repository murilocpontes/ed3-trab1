#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my-lib.h"
#include "funcoesFornecidas.h"

int main(){
	int command;
	char *filename = (char *) malloc(50 * sizeof(char));
	scanf("%d %s",&command,filename);

	switch(command){
		case 1: {
<<<<<<< HEAD
			break;
		}
		case 2:{
			print_info(filename);
=======
            char *filename2 = (char *) malloc(50 * sizeof(char));
            scanf(" %s",filename2);
            read_csv(&filename,&filename2);
			break;
		}
		case 2:{
			//print_info(filename);
>>>>>>> bf634ed (pc pinkas)
			break;
		}  
		case 3:{
			int n;
<<<<<<< HEAD
			char *field = (char *) malloc(20 * sizeof(char));
			char *value = (char *) malloc(30 * sizeof(char)); 
			scanf(" %d",&n);
			for(int i = 0; i < n; i++){
				printf("Busca %d\n",i+1);
				scanf("%s ",field);  
				scan_quote_string(value);
				//search_records(filename,field,value);
				//printf("Numero de paginas de disco: %d");
			}
			free(field);
			field = NULL;
			free(value);
			value = NULL;
=======
            char *field = (char *) malloc(20 * sizeof(char));
            char *value = (char *) malloc(30 * sizeof(char));
            scanf(" %d",&n);
            for(int i = 0; i < n; i++){
                printf("Busca %d\n",i+1);
                scanf(" %s",field);
                scan_quote_string(value);
                search_records(&filename,field,value);
            }
            free(field);
            field = NULL;
            free(value);
            value = NULL;
>>>>>>> bf634ed (pc pinkas)
			break;
		}
		case 4:{
			int  n;
			scanf(" %d", &n);
<<<<<<< HEAD
			search_and_delete_record(filename, n);
			binarioNaTela(filename);
			break;
		}
		case 5:{
			break;
		}
		case 6:{
			compress_file(filename);
			binarioNaTela(filename);
=======
			//search_and_delete_record(filename, n);
			//binarioNaTela(filename);
			break;
		}
		case 5:{
            int n; // n�mero de inser��es a serem realizadas
            scanf(" %d",&n);
            insert_record(&filename,n);
			break;
		}
		case 6:{
			//compress_file(filename);
			//binarioNaTela(filename);
>>>>>>> bf634ed (pc pinkas)
			break;
		}
		default:{
			printf("Falha no processamento do arquivo\n");
			break;
		} 

	}
	free(filename);
	filename = NULL;

	return 0;
}