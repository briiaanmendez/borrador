#ifndef PROCESO1_H
#define PROCESO1_H


#include <stdint.h>
#include <stdbool.h>
#include "shared_utils.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netdb.h>
#include<commons/log.h>
#include<commons/collections/list.h>
#include<string.h>
#include<assert.h>




typedef enum
{
	PAQUETE
}op_code;



void* recibir_buffer(int*, int);

int iniciar_servidor(t_log*, char*,char*);
int esperar_cliente(t_log*,int);
t_list* recibir_paquete(t_log*,int);
int recibir_operacion(int);
void iterator(char* value);
int deserialize_int(int* output, char* input);


#endif