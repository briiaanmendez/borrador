#ifndef PROCESO1_H
#define PROCESO1_H


#include <stdint.h>
#include <stdbool.h>
#include "shared_utils.h"


#include<stdio.h>
#include<stdlib.h>
#include<commons/log.h>
#include<commons/string.h>
#include<commons/config.h>
#include<readline/readline.h>
#include<commons/collections/list.h>
#include<signal.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netdb.h>
#include<string.h>

typedef enum
{
	PAQUETE
}op_code;

typedef struct
{
	int size;
	void* stream;
} t_buffer;

typedef struct
{
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;


t_log* iniciar_logger(void);
t_config* iniciar_config(char *, t_log*);
void llenar_lista_de_instrucciones(FILE *,t_list* );
t_paquete* crear_paquete(void);
void llenar_Paquete(t_list* lista,t_paquete* paquete);
void agregar_a_paquete2(t_paquete* paquete,int tamanio);
void agregar_a_paquete(t_paquete* paquete, void* valor, int tamanio);
void cargar_lista_de_segmentos(t_list* ,char** );
int crear_conexion(t_log* logger,char* ip, char* puerto);
void enviar_paquete(t_paquete* paquete, int socket_cliente);
void eliminar_paquete(t_paquete* paquete);
void liberar_conexion(int socket_cliente);
void terminar_programa(int, t_log*, t_config*);

#endif